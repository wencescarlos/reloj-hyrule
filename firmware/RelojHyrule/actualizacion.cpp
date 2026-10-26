// ===========================================================================
//  actualizacion.cpp  -  Actualizaciones por internet y por WiFi
//  Reloj de Hyrule · Proyecto de fans SIN ÁNIMO DE LUCRO · No oficial
//  CC BY-NC-SA 4.0 · https://github.com/wencescarlos/reloj-hyrule
// ===========================================================================
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266httpUpdate.h>
#include <WiFiClientSecureBearSSL.h>
#include <ArduinoOTA.h>
#include <memory>
#include "actualizacion.h"
#include "configuracion.h"
#include "ajustes.h"
#include "animaciones.h"
#include "pantalla.h"
#include "red.h"

namespace {
  // Último resultado de la comprobación (el texto se da en el idioma del reloj)
  enum Estado : uint8_t {
    SIN_COMPROBAR, FALTA_URL, SIN_INTERNET, NO_ABRE, SIN_VERSION_TXT,
    AL_DIA, ERROR_ACTUALIZAR, SIN_CAMBIOS, ACTUALIZADO, SIN_MEMORIA, COLGADO, RECIEN_ACTUALIZADO
  };
  Estado   ultimoEstado = SIN_COMPROBAR;
  String   detalleEstado;                              // código o error
  String   versionPublicada;                           // la que hay en internet
  bool     comprobando = false;
  uint32_t proximaRevision = 60UL * 1000UL;          // 1 minuto tras encender
  const uint32_t UN_DIA = 24UL * 60UL * 60UL * 1000UL;

  // Marca en la memoria RTC (sobrevive a un reinicio, no a un apagón): si el
  // reloj se reinicia en mitad de una comprobación, al arrancar lo sabremos y
  // no se volverá a intentar enseguida (así nunca entra en un bucle de reinicios).
  const uint32_t MARCA_COMPROBANDO  = 0x48595255;  // "HYRU": comprobando
  const uint32_t MARCA_ACTUALIZADO  = 0x4F4B4F4B;  // "OKOK": se ha instalado una versión nueva
  void escribirMarca(uint32_t v) { ESP.rtcUserMemoryWrite(0, &v, sizeof(v)); }
  void marcar(bool si) { escribirMarca(si ? MARCA_COMPROBANDO : 0); }
  // Memoria que necesita una conexión segura (HTTPS) con GitHub
  const uint32_t MEMORIA_HTTPS = 22000;

  // "1.2.10" -> 1002010 para poder comparar versiones
  uint32_t numeroVersion(const String& v) {
    uint32_t partes[3] = { 0, 0, 0 };
    int parte = 0;
    for (size_t i = 0; i < v.length() && parte < 3; i++) {
      char c = v[i];
      if (c >= '0' && c <= '9') partes[parte] = partes[parte] * 10 + (c - '0');
      else if (c == '.') parte++;
    }
    return partes[0] * 1000000UL + partes[1] * 1000UL + partes[2];
  }

  // Explica los códigos de error de la conexión
  String explicarCodigo(int c) {
    if (c == 404) return tr("404: todavía no está publicada; revisa GitHub Pages", "404: not published yet; check GitHub Pages");
    if (c == -1)  return tr("no conecta; revisa la dirección o el WiFi", "can't connect; check the address or the WiFi");
    if (c == -11) return tr("el servidor tarda demasiado", "the server took too long");
    if (c < 0)    return String(tr("error de conexión ", "connection error ")) + c;
    return String(tr("código ", "code ")) + c;
  }

  String unirUrl(const char* base, const char* archivo) {
    String u = base;
    u.trim();
    if (!u.endsWith("/")) u += '/';
    return u + archivo;
  }
}

namespace Actualizacion {

// Solo es un cuelgue si el chip se reinició por un fallo del programa o porque
// se quedó bloqueado. Los reinicios voluntarios (botón, WiFi, subir un programa
// por WiFi, instalar una versión) no cuentan.
static bool reinicioPorFallo() {
  uint32_t r = ESP.getResetInfoPtr()->reason;
  return r == REASON_EXCEPTION_RST || r == REASON_SOFT_WDT_RST || r == REASON_WDT_RST;
}

void alArrancar() {
  uint32_t v = 0;
  ESP.rtcUserMemoryRead(0, &v, sizeof(v));
  if (v == MARCA_ACTUALIZADO) {
    ultimoEstado = RECIEN_ACTUALIZADO;
    Serial.println(F("[actualizacion] Versión nueva instalada por internet."));
  } else if (v == MARCA_COMPROBANDO && reinicioPorFallo()) {
    ultimoEstado = COLGADO;
    proximaRevision = millis() + UN_DIA;          // nada de reintentar al minuto
    Serial.println(F("[actualizacion] La comprobación anterior no terminó: se aplaza un día."));
  }
  marcar(false);
}

void iniciarOTA() {
  ArduinoOTA.setHostname(NOMBRE_EQUIPO);
  if (ajustes.claveAdmin[0]) ArduinoOTA.setPassword(ajustes.claveAdmin);
  ArduinoOTA.onStart([]() { Pantalla::encender(true); Anim::progreso(0); });
  ArduinoOTA.onProgress([](unsigned int hecho, unsigned int total) {
    Anim::progreso(total ? (hecho * 100UL) / total : 0);
  });
  ArduinoOTA.onEnd([]() { Anim::progreso(100); });
  ArduinoOTA.begin();
}

String estado() {
  switch (ultimoEstado) {
    case SIN_COMPROBAR:    return comprobando ? tr("Buscando...", "Checking...") : tr("Todavía no se ha comprobado.", "Not checked yet.");
    case FALTA_URL:        return tr("Falta la dirección de actualizaciones.", "The update address is missing.");
    case SIN_INTERNET:     return tr("Sin conexión a internet.", "No internet connection.");
    case NO_ABRE:          return tr("No se pudo abrir la dirección.", "Could not open the address.");
    case SIN_VERSION_TXT:  return String(tr("No se pudo leer la versión publicada: ", "Could not read the published version: "))
                                  + detalleEstado + ".";
    case AL_DIA:           return String(tr("Ya tienes la última versión (" VERSION_RELOJ "). En internet: ",
                                            "You already have the latest version (" VERSION_RELOJ "). Online: "))
                                  + versionPublicada + ".";
    case ERROR_ACTUALIZAR: return String(tr("Error al actualizar: ", "Update error: ")) + detalleEstado;
    case SIN_CAMBIOS:      return tr("El servidor dice que no hay cambios.", "The server says there are no changes.");
    case ACTUALIZADO:      return tr("Actualizado. Reiniciando...", "Updated. Restarting...");
    case SIN_MEMORIA:      return String(tr("No hay memoria libre suficiente para conectar (", "Not enough free memory to connect ("))
                                  + detalleEstado + tr(" bytes). Reinicia el reloj y vuelve a probar.", " bytes). Restart the clock and try again.");
    case RECIEN_ACTUALIZADO: return tr("✓ Actualizado por internet a la versión " VERSION_RELOJ ".",
                                       "✓ Updated over the internet to version " VERSION_RELOJ ".");
    case COLGADO:          return tr("La última comprobación reinició el reloj. Se volverá a intentar mañana; puedes probar a mano.",
                                     "The last check restarted the clock. It will try again tomorrow; you can try by hand.");
  }
  return "";
}

void servicio() {
  if (!ajustes.autoActualizar || !ajustes.urlActualizacion[0]) return;
  if (!Red::conectado()) return;
  if ((int32_t)(millis() - proximaRevision) < 0) return;
  proximaRevision = millis() + UN_DIA;
  comprobar();
}

void comprobar() {
  comprobando = true;
  ultimoEstado = SIN_COMPROBAR;
  struct Fin { ~Fin() { comprobando = false; } } fin;     // pase lo que pase, deja de "buscar"
  if (!ajustes.urlActualizacion[0]) { ultimoEstado = FALTA_URL; return; }
  if (!Red::conectado())            { ultimoEstado = SIN_INTERNET; return; }
  if (ESP.getMaxFreeBlockSize() < MEMORIA_HTTPS) {
    ultimoEstado = SIN_MEMORIA; detalleEstado = String(ESP.getMaxFreeBlockSize());
    return;
  }
  marcar(true);
  struct Desmarcar { ~Desmarcar() { marcar(false); } } desmarcar;   // al terminar, pase lo que pase

  String base = ajustes.urlActualizacion;
  bool segura = base.startsWith("https");

  // Cliente normal o seguro (https) según la dirección
  std::unique_ptr<WiFiClient> cliente;
  if (segura) {
    auto* c = new BearSSL::WiFiClientSecure();
    c->setInsecure();      // no se comprueba el certificado (un reloj no guarda certificados)
    // Si el servidor lo permite, usar búferes pequeños para ahorrar memoria
    String host = base.substring(base.indexOf("://") + 3);
    int barra = host.indexOf('/');
    if (barra >= 0) host = host.substring(0, barra);
    if (BearSSL::WiFiClientSecure::probeMaxFragmentLength(host.c_str(), 443, 1024))
      c->setBufferSizes(1024, 1024);
    cliente.reset(c);
  } else {
    cliente.reset(new WiFiClient());
  }

  // 1) ¿Qué versión hay publicada?
  HTTPClient http;
  http.setFollowRedirects(HTTPC_STRICT_FOLLOW_REDIRECTS);
  http.setTimeout(10000);
  if (!http.begin(*cliente, unirUrl(ajustes.urlActualizacion, "version.txt"))) {
    ultimoEstado = NO_ABRE; return;
  }
  int codigo = http.GET();
  if (codigo != HTTP_CODE_OK) {
    ultimoEstado = SIN_VERSION_TXT; detalleEstado = explicarCodigo(codigo);
    Serial.printf("[actualizacion] version.txt: %d (memoria libre %u)\n", codigo, ESP.getFreeHeap());
    http.end(); return;
  }
  String publicada = http.getString();
  http.end();
  publicada.trim();
  versionPublicada = publicada;
  Serial.printf("[actualizacion] Publicada %s, instalada %s\n", publicada.c_str(), VERSION_RELOJ);

  if (numeroVersion(publicada) <= numeroVersion(VERSION_RELOJ)) {
    ultimoEstado = AL_DIA;
    return;
  }

  // 2) Hay versión nueva: descargar e instalar
  Serial.printf("[actualizacion] Nueva versión %s\n", publicada.c_str());
  Pantalla::encender(true);
  Pantalla::brillo(ajustes.brillo);
  Anim::mensaje(tr("¡NUEVA VERSIÓN! ACTUALIZANDO...", "NEW VERSION! UPDATING..."));
  ESPhttpUpdate.setFollowRedirects(HTTPC_STRICT_FOLLOW_REDIRECTS);
  ESPhttpUpdate.rebootOnUpdate(false);          // reiniciamos nosotros, tras dejar la marca
  ESPhttpUpdate.onProgress([](int hecho, int total) {
    Anim::progreso(total > 0 ? (hecho * 100L) / total : 0);
  });
  t_httpUpdate_return r = ESPhttpUpdate.update(*cliente,
                            unirUrl(ajustes.urlActualizacion, "firmware.bin"),
                            VERSION_RELOJ);
  switch (r) {
    case HTTP_UPDATE_FAILED:
      ultimoEstado = ERROR_ACTUALIZAR; detalleEstado = ESPhttpUpdate.getLastErrorString();
      break;
    case HTTP_UPDATE_NO_UPDATES:
      ultimoEstado = SIN_CAMBIOS;
      break;
    case HTTP_UPDATE_OK:
      ultimoEstado = ACTUALIZADO;
      escribirMarca(MARCA_ACTUALIZADO);        // al arrancar dirá "actualizado", no "colgado"
      delay(300);
      ESP.restart();                           // no vuelve: el destructor no borra la marca
      break;
  }
}

} // namespace Actualizacion
