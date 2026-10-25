// ===========================================================================
//  red.cpp  -  WiFi, portal cautivo, servidor web y API del reloj
//  Reloj de Hyrule · Proyecto de fans SIN ÁNIMO DE LUCRO · No oficial
//  CC BY-NC-SA 4.0 · https://github.com/wencescarlos/reloj-hyrule
// ===========================================================================
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <ESP8266HTTPUpdateServer.h>
#include <DNSServer.h>
#include <ArduinoOTA.h>
#include <LittleFS.h>
#include "red.h"
#include "configuracion.h"
#include "ajustes.h"
#include "animaciones.h"
#include "actualizacion.h"
#include "tiempo.h"
#include "sistema.h"
#include "pagina_web.h"
#include "pagina_mensaje.h"
#include "pantalla.h"
#include "sonido.h"
#include "principal.h"

Peticiones peticiones;

namespace {
  ESP8266WebServer        servidor(80);
  ESP8266HTTPUpdateServer subidaPrograma;
  DNSServer               dns;
  const IPAddress         IP_PORTAL(192, 168, 4, 1);

  bool     portal = false;
  bool     serviciosListos = false;
  uint32_t ultimoReintento = 0;

  // ---------------------------------------------------------------- utilidades
  String escapar(const String& s) {
    String r;
    for (size_t i = 0; i < s.length(); i++) {
      char c = s[i];
      if (c == '"' || c == '\\') { r += '\\'; r += c; }
      else if ((uint8_t)c < 0x20) r += ' ';
      else r += c;
    }
    return r;
  }

  void responderJson(const String& json, int codigo = 200) {
    servidor.sendHeader("Cache-Control", "no-store");
    servidor.send(codigo, "application/json; charset=utf-8", json);
  }

  void ok() { responderJson("{\"ok\":true}"); }

  // --------------------------------------------------------------- páginas
  void paginaPrincipal() {
    servidor.sendHeader("Cache-Control", "no-store");
    servidor.send_P(200, "text/html; charset=utf-8", PAGINA_PRINCIPAL);
  }

  void apiEstado() {
    struct tm t;
    bool hayHora = Tiempo::ahora(t);
    char hora[12] = "--:--:--";
    if (hayHora) snprintf(hora, sizeof(hora), "%02d:%02d:%02d", t.tm_hour, t.tm_min, t.tm_sec);
    String j = "{";
    j += "\"version\":\"" VERSION_RELOJ "\"";
    j += ",\"hora\":\""; j += hora; j += '"';
    j += ",\"fecha\":\""; j += hayHora ? escapar(Tiempo::fechaLarga(t)) : String(""); j += '"';
    j += ",\"sincronizado\":"; j += hayHora ? "true" : "false";
    j += ",\"portal\":"; j += portal ? "true" : "false";
    j += ",\"wifi\":\""; j += escapar(WiFi.SSID()); j += '"';
    j += ",\"conectado\":"; j += Red::conectado() ? "true" : "false";
    j += ",\"senal\":"; j += WiFi.RSSI();
    j += ",\"ip\":\""; j += Red::ip(); j += '"';
    j += ",\"memoria\":"; j += ESP.getFreeHeap();
    j += ",\"encendido\":"; j += millis() / 1000;
    j += ",\"diasCumple\":"; j += hayHora ? Tiempo::diasParaCumple(t) : -1;
    j += ",\"actualizacion\":\""; j += escapar(Actualizacion::estado()); j += '"';
    j += ",\"concentracion\":"; j += Concentracion::activa() ? "true" : "false";
    j += ",\"concQueda\":"; j += Concentracion::segundosQuedan();
    j += ",\"concHorario\":"; j += Concentracion::porHorario() ? "true" : "false";
    j += ",\"sonido\":"; j += Sonido::disponible() ? "true" : "false";
    j += ",\"mensajeActivo\":"; j += Anim::mensajeEnCurso() ? "true" : "false";
    j += ",\"mensajeQueda\":"; j += Anim::segundosMensaje();
    j += ",\"nombre\":\""; j += escapar(ajustes.nombre); j += '"';
    j += ",\"idioma\":"; j += ajustes.idioma;
    j += ",\"pines\":\""; j += escapar(Pantalla::descripcionPines()); j += '"';
    j += ",\"diagnostico\":\""; j += escapar(estadoDiagnostico); j += '"';
    j += '}';
    responderJson(j);
  }

  void apiAjustesLeer() { responderJson(Config::json()); }

  void apiAjustesGuardar() {
    for (int i = 0; i < servidor.args(); i++) {
      String clave = servidor.argName(i);
      String valor = servidor.arg(i);
      // Las claves solo se cambian si se escribe una nueva
      if ((clave == "claveAdmin" || clave == "wifiClave") && valor.length() == 0) continue;
      if (clave == "wifiNombre" || clave == "wifiClave") continue;   // se cambian en /api/wifi
      Config::asignar(clave, valor);
    }
    Config::guardar();
    peticiones.ajustesCambiados = true;
    ok();
  }

  void apiAventuras() {
    String j = "[";
    for (uint8_t i = 0; i < Anim::total(); i++) {
      if (i) j += ',';
      j += '"'; j += escapar(Anim::nombre(i)); j += '"';
    }
    j += ']';
    responderJson(j);
  }

  void apiAnimacion() {
    peticiones.animacion = servidor.hasArg("n") ? servidor.arg("n").toInt() : 255;
    ok();
  }

  void apiMensaje() {
    String t = servidor.arg("texto");
    t.trim();
    if (t.length() == 0) { responderJson("{\"ok\":false}", 400); return; }
    if (t.length() > 200) t = t.substring(0, 200);
    // Opcionales (página del móvil): intro=0..6 | s | a<n>, segundos, velocidad
    int16_t intro = 1;                                   // de siempre: llega el héroe
    if (servidor.hasArg("intro")) {
      String i = servidor.arg("intro");
      if (i == "s")                 intro = Anim::INTRO_SORPRESA;
      else if (i.startsWith("a"))   intro = Anim::INTRO_AVENTURA + i.substring(1).toInt();
      else                          intro = constrain(i.toInt(), 0, (int)Anim::PERSONAJES);
    }
    peticiones.mensajeIntro     = intro;
    peticiones.mensajeSegundos  = servidor.hasArg("segundos") ? constrain(servidor.arg("segundos").toInt(), 0, 1800) : 0;
    peticiones.mensajeVelocidad = servidor.hasArg("velocidad") ? constrain(servidor.arg("velocidad").toInt(), 15, 150) : 0;
    peticiones.mensajeSonido    = servidor.hasArg("sonido") ? constrain(servidor.arg("sonido").toInt(), 0, (int)Sonido::total()) : 0;
    peticiones.mensaje = t;
    Anim::pararMensaje();                                // si había otro, se corta
    ok();
  }

  void apiMensajeParar() { Anim::pararMensaje(); Sonido::parar(); peticiones.mensaje = ""; ok(); }

  // Modo concentración: minutos > 0, -1 = hasta que se quite, 0 = quitar
  void apiConcentracion() {
    int m = servidor.arg("minutos").toInt();
    if (m == 0) Concentracion::terminar();
    else Concentracion::empezar(m < 0 ? -1 : constrain(m, 1, 600));
    ok();
  }

  // Probar una melodía (melodia=n) o parar (sin parámetro)
  void apiSonido() {
    if (!Sonido::disponible()) { responderJson("{\"ok\":false,\"motivo\":\"sin zumbador\"}", 409); return; }
    if (servidor.hasArg("melodia")) Sonido::tocar(servidor.arg("melodia").toInt());
    else Sonido::parar();
    ok();
  }

  void apiMelodias() {
    String j = "[";
    for (uint8_t i = 0; i < Sonido::total(); i++) {
      if (i) j += ',';
      j += '"'; j += escapar(Sonido::nombre(i)); j += '"';
    }
    j += ']';
    responderJson(j);
  }

  // Página del móvil para mandar mensajes (se puede añadir a la pantalla de inicio)
  void paginaMensaje() {
    servidor.sendHeader("Cache-Control", "no-store");
    servidor.send_P(200, "text/html; charset=utf-8", PAGINA_MENSAJE);
  }
  void icono() {
    servidor.sendHeader("Cache-Control", "max-age=604800");
    servidor.send_P(200, "image/png", (const char*)ICONO_PNG, sizeof(ICONO_PNG));
  }
  void manifiesto() {
    servidor.sendHeader("Cache-Control", "max-age=86400");
    servidor.send_P(200, "application/manifest+json", MANIFIESTO);
  }

  void apiPrueba()        { peticiones.prueba = servidor.hasArg("corta") ? 2 : 1; ok(); }

  // Diagnóstico de pantalla: /api/diagnostico?accion=todos|modulo|pata|fin&n=...
  void apiDiagnostico() {
    String a = servidor.arg("accion");
    int n = servidor.arg("n").toInt();
    if      (a == "todos")                        peticiones.diagnostico = 0;
    else if (a == "modulo" && n >= 1 && n <= 4)   peticiones.diagnostico = n;        // 1..4
    else if (a == "pata"   && n >= 0 && n <= 2)   peticiones.diagnostico = 10 + n;   // 10..12
    else if (a == "fin")                          peticiones.diagnostico = 99;
    else { responderJson("{\"ok\":false}", 400); return; }
    ok();
  }
  void apiBuscarVersion() { peticiones.buscarActualizacion = true; ok(); }
  void apiReiniciar()     { peticiones.reiniciarEn = millis() + 1500; ok(); }

  void apiFabrica() {
    LittleFS.remove(ARCHIVO_AJUSTES);
    peticiones.reiniciarEn = millis() + 1500;
    ok();
  }

  void apiRedes() {
    int n = WiFi.scanNetworks();
    String j = "[";
    for (int i = 0; i < n; i++) {
      if (WiFi.SSID(i).length() == 0) continue;
      if (j.length() > 1) j += ',';
      j += "{\"nombre\":\""; j += escapar(WiFi.SSID(i));
      j += "\",\"senal\":"; j += WiFi.RSSI(i);
      j += ",\"abierta\":"; j += (WiFi.encryptionType(i) == ENC_TYPE_NONE) ? "true" : "false";
      j += '}';
    }
    j += ']';
    WiFi.scanDelete();
    responderJson(j);
  }

  void apiWifi() {
    String nombre = servidor.arg("nombre");
    String clave  = servidor.arg("clave");
    nombre.trim();
    if (nombre.length() == 0) { responderJson("{\"ok\":false}", 400); return; }
    Config::asignar("wifiNombre", nombre);
    Config::asignar("wifiClave", clave);
    Config::guardar();
    peticiones.reiniciarEn = millis() + 2000;
    ok();
  }

  // En modo portal, cualquier dirección lleva a la página del reloj
  void noEncontrado() {
    if (portal) {
      servidor.sendHeader("Location", String("http://") + IP_PORTAL.toString() + "/", true);
      servidor.send(302, "text/plain", "");
      return;
    }
    servidor.send(404, "text/plain; charset=utf-8", "No encontrado");
  }

  void prepararServicios() {
    if (serviciosListos) return;
    servidor.on("/",                     HTTP_GET,  paginaPrincipal);
    servidor.on("/api/estado",           HTTP_GET,  apiEstado);
    servidor.on("/api/ajustes",          HTTP_GET,  apiAjustesLeer);
    servidor.on("/api/ajustes",          HTTP_POST, apiAjustesGuardar);
    servidor.on("/api/aventuras",        HTTP_GET,  apiAventuras);
    servidor.on("/api/animacion",        HTTP_POST, apiAnimacion);
    servidor.on("/api/mensaje",          HTTP_POST, apiMensaje);
    servidor.on("/api/mensaje/parar",    HTTP_POST, apiMensajeParar);
    servidor.on("/api/concentracion",    HTTP_POST, apiConcentracion);
    servidor.on("/api/sonido",           HTTP_POST, apiSonido);
    servidor.on("/api/melodias",         HTTP_GET,  apiMelodias);
    servidor.on("/mensaje",              HTTP_GET,  paginaMensaje);
    servidor.on("/icono.png",            HTTP_GET,  icono);
    servidor.on("/apple-touch-icon.png", HTTP_GET,  icono);
    servidor.on("/manifest.json",        HTTP_GET,  manifiesto);
    servidor.on("/api/prueba",           HTTP_POST, apiPrueba);
    servidor.on("/api/diagnostico",      HTTP_POST, apiDiagnostico);
    servidor.on("/api/buscar-version",   HTTP_POST, apiBuscarVersion);
    servidor.on("/api/reiniciar",        HTTP_POST, apiReiniciar);
    servidor.on("/api/fabrica",          HTTP_POST, apiFabrica);
    servidor.on("/api/redes",            HTTP_GET,  apiRedes);
    servidor.on("/api/wifi",             HTTP_POST, apiWifi);
    servidor.onNotFound(noEncontrado);
    // Subir un archivo .bin desde el navegador (protegido con la clave)
    subidaPrograma.setup(&servidor, "/actualizar", "admin", ajustes.claveAdmin);
    servidor.begin();

    MDNS.begin(NOMBRE_EQUIPO);
    MDNS.addService("http", "tcp", 80);
    Actualizacion::iniciarOTA();
    serviciosListos = true;
  }

  void abrirPortal() {
    portal = true;
    WiFi.mode(WIFI_AP_STA);
    WiFi.softAPConfig(IP_PORTAL, IP_PORTAL, IPAddress(255, 255, 255, 0));
    WiFi.softAP(WIFI_PROPIO_NOMBRE, WIFI_PROPIO_CLAVE);
    dns.setErrorReplyCode(DNSReplyCode::NoError);
    dns.start(53, "*", IP_PORTAL);
    ultimoReintento = millis();
    Serial.println(F("[red] Portal abierto: WiFi " WIFI_PROPIO_NOMBRE " / 192.168.4.1"));
  }
}

// ---------------------------------------------------------------------------
void esperar(uint32_t ms) {
  uint32_t inicio = millis();
  do {
    Red::servicio();
    Sonido::servicio();
    delay(1);
  } while (millis() - inicio < ms);
}

namespace Red {

bool conectado()  { return WiFi.status() == WL_CONNECTED; }
bool modoPortal() { return portal; }

String ip() {
  if (conectado()) return WiFi.localIP().toString();
  if (portal) return IP_PORTAL.toString();
  return "";
}

void iniciar() {
  WiFi.persistent(false);
  WiFi.setAutoReconnect(true);
  WiFi.hostname(NOMBRE_EQUIPO);
  WiFi.mode(WIFI_STA);

  if (ajustes.wifiNombre[0]) {
    Serial.printf("[red] Conectando a %s", ajustes.wifiNombre);
    WiFi.begin(ajustes.wifiNombre, ajustes.wifiClave);
    for (uint16_t paso = 0; paso < SEGUNDOS_CONECTAR * 4 && !conectado(); paso++) {
      Anim::conectando(paso);
      delay(250);
      Serial.print('.');
    }
    Serial.println();
  }

  if (conectado()) {
    Serial.printf("[red] Conectado. IP: %s\n", WiFi.localIP().toString().c_str());
  } else {
    abrirPortal();
  }
  prepararServicios();
}

void servicio() {
  if (!serviciosListos) return;
  if (portal) dns.processNextRequest();
  servidor.handleClient();
  MDNS.update();
  ArduinoOTA.handle();

  // En modo portal: si hay WiFi guardado, reintentar de vez en cuando
  // (por ejemplo, tras un apagón el router tarda más en arrancar que el reloj)
  if (portal && ajustes.wifiNombre[0]) {
    if (conectado()) {
      Serial.println(F("[red] El WiFi de casa ha vuelto. Reiniciando..."));
      peticiones.reiniciarEn = millis() + 500;
    } else if (millis() - ultimoReintento > MINUTOS_REINTENTO * 60000UL &&
               WiFi.softAPgetStationNum() == 0) {      // nadie usando el portal
      ultimoReintento = millis();
      WiFi.begin(ajustes.wifiNombre, ajustes.wifiClave);
    }
  }

  if (peticiones.reiniciarEn && (int32_t)(millis() - peticiones.reiniciarEn) >= 0) {
    ESP.restart();
  }
}

} // namespace Red
