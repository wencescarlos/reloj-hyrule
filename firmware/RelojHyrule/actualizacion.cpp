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
  String   ultimoEstado = "Todavía no se ha comprobado.";
  uint32_t proximaRevision = 60UL * 1000UL;          // 1 minuto tras encender
  const uint32_t UN_DIA = 24UL * 60UL * 60UL * 1000UL;

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

  String unirUrl(const char* base, const char* archivo) {
    String u = base;
    u.trim();
    if (!u.endsWith("/")) u += '/';
    return u + archivo;
  }
}

namespace Actualizacion {

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

String estado() { return ultimoEstado; }

void servicio() {
  if (!ajustes.autoActualizar || !ajustes.urlActualizacion[0]) return;
  if (!Red::conectado()) return;
  if ((int32_t)(millis() - proximaRevision) < 0) return;
  proximaRevision = millis() + UN_DIA;
  comprobar();
}

void comprobar() {
  if (!ajustes.urlActualizacion[0]) { ultimoEstado = "Falta la dirección de actualizaciones."; return; }
  if (!Red::conectado())            { ultimoEstado = "Sin conexión a internet."; return; }

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
    ultimoEstado = "No se pudo abrir la dirección."; return;
  }
  int codigo = http.GET();
  if (codigo != HTTP_CODE_OK) {
    ultimoEstado = String("No se encontró version.txt (código ") + codigo + ").";
    http.end(); return;
  }
  String publicada = http.getString();
  http.end();
  publicada.trim();

  if (numeroVersion(publicada) <= numeroVersion(VERSION_RELOJ)) {
    ultimoEstado = "Tienes la última versión (" VERSION_RELOJ ").";
    return;
  }

  // 2) Hay versión nueva: descargar e instalar
  Serial.printf("[actualizacion] Nueva versión %s\n", publicada.c_str());
  Pantalla::encender(true);
  Pantalla::brillo(ajustes.brillo);
  Anim::mensaje("¡NUEVA VERSIÓN! ACTUALIZANDO...");
  ESPhttpUpdate.setFollowRedirects(HTTPC_STRICT_FOLLOW_REDIRECTS);
  ESPhttpUpdate.rebootOnUpdate(true);
  ESPhttpUpdate.onProgress([](int hecho, int total) {
    Anim::progreso(total > 0 ? (hecho * 100L) / total : 0);
  });
  t_httpUpdate_return r = ESPhttpUpdate.update(*cliente,
                            unirUrl(ajustes.urlActualizacion, "firmware.bin"),
                            VERSION_RELOJ);
  switch (r) {
    case HTTP_UPDATE_FAILED:
      ultimoEstado = String("Error al actualizar: ") + ESPhttpUpdate.getLastErrorString();
      break;
    case HTTP_UPDATE_NO_UPDATES:
      ultimoEstado = "El servidor dice que no hay cambios.";
      break;
    case HTTP_UPDATE_OK:
      ultimoEstado = "Actualizado. Reiniciando...";   // normalmente ya se ha reiniciado
      break;
  }
}

} // namespace Actualizacion
