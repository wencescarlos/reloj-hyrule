// ===========================================================================
//  ajustes.cpp  -  Guardar y leer los ajustes (archivo de texto clave=valor)
//  Reloj de Hyrule · Proyecto de fans SIN ÁNIMO DE LUCRO · No oficial
//  CC BY-NC-SA 4.0 · https://github.com/wencescarlos/reloj-hyrule
// ===========================================================================
#include <LittleFS.h>
#include "ajustes.h"
#include "configuracion.h"

Ajustes ajustes;

namespace {

  enum Tipo : uint8_t { TEXTO, NUMERO, SECRETO };

  struct Campo {
    const char* nombre;
    Tipo tipo;
    void* dato;
    uint8_t tam;       // tamaño del texto
    uint8_t minimo;
    uint8_t maximo;
  };

  #define TXT(n, v)        { n, TEXTO,   ajustes.v, sizeof(ajustes.v), 0, 0 }
  #define SEC(n, v)        { n, SECRETO, ajustes.v, sizeof(ajustes.v), 0, 0 }
  #define NUM(n, v, a, b)  { n, NUMERO,  &ajustes.v, 1, a, b }

  // Lista de todos los ajustes: nombre en el archivo / en la web
  const Campo CAMPOS[] = {
    NUM("idioma", idioma, 0, 1),
    TXT("nombre", nombre),
    NUM("cumpleDia", cumpleDia, 0, 31),
    NUM("cumpleMes", cumpleMes, 0, 12),
    TXT("mensaje", mensaje),
    NUM("saludos", saludos, 0, 1),
    NUM("formato24", formato24, 0, 1),
    NUM("ceroIzquierda", ceroIzquierda, 0, 1),
    NUM("modoFecha", modoFecha, 0, 2),
    NUM("fechaCadaMin", fechaCadaMin, 1, 60),
    NUM("modoSegundos", modoSegundos, 0, 2),
    NUM("parpadeo", parpadeo, 0, 1),
    NUM("horaEnPunto", horaEnPunto, 0, 1),
    NUM("animCadaMin", animCadaMin, 0, 120),
    NUM("frases", frases, 0, 1),
    NUM("nocheActiva", nocheActiva, 0, 1),
    NUM("nocheIniH", nocheIniH, 0, 23),
    NUM("nocheIniM", nocheIniM, 0, 59),
    NUM("nocheFinH", nocheFinH, 0, 23),
    NUM("nocheFinM", nocheFinM, 0, 59),
    NUM("nocheApagar", nocheApagar, 0, 1),
    NUM("brilloNoche", brilloNoche, 0, 15),
    NUM("brillo", brillo, 0, 15),
    NUM("velocidad", velocidad, 10, 120),
    NUM("rotacion", rotacion, 0, 3),
    NUM("invertirModulos", invertirModulos, 0, 1),
    NUM("espejo", espejo, 0, 1),
    TXT("zona", zona),
    TXT("servidorHora", servidorHora),
    TXT("wifiNombre", wifiNombre),
    SEC("wifiClave", wifiClave),
    SEC("claveAdmin", claveAdmin),
    TXT("urlActualizacion", urlActualizacion),
    NUM("autoActualizar", autoActualizar, 0, 1),
  };
  const size_t NUM_CAMPOS = sizeof(CAMPOS) / sizeof(CAMPOS[0]);

  void copiar(char* destino, size_t tam, const String& origen) {
    String limpio = origen;
    limpio.replace("\r", "");
    limpio.replace("\n", " ");
    strncpy(destino, limpio.c_str(), tam - 1);
    destino[tam - 1] = '\0';
  }

  String escaparJson(const char* s) {
    String r;
    for (const char* p = s; *p; p++) {
      char c = *p;
      if (c == '"' || c == '\\') { r += '\\'; r += c; }
      else if ((uint8_t)c < 0x20) r += ' ';
      else r += c;
    }
    return r;
  }
}

namespace Config {

void valoresPorDefecto() {
  memset(&ajustes, 0, sizeof(ajustes));
  ajustes.idioma = IDIOMA_ES;
  strcpy(ajustes.nombre, "");
  ajustes.cumpleDia = 0;
  ajustes.cumpleMes = 0;
  strcpy(ajustes.mensaje, "");
  ajustes.saludos = 1;

  ajustes.formato24 = 1;
  ajustes.ceroIzquierda = 1;
  ajustes.modoFecha = FECHA_ALTERNAR;
  ajustes.fechaCadaMin = 10;
  ajustes.modoSegundos = SEG_HADA;
  ajustes.parpadeo = 1;
  ajustes.horaEnPunto = 1;

  ajustes.animCadaMin = 10;
  ajustes.frases = 1;

  ajustes.nocheActiva = 1;
  ajustes.nocheIniH = 21; ajustes.nocheIniM = 30;
  ajustes.nocheFinH = 7;  ajustes.nocheFinM = 30;
  ajustes.nocheApagar = 0;
  ajustes.brilloNoche = 0;

  ajustes.brillo = 2;
  ajustes.velocidad = 35;
  ajustes.rotacion = 0;
  ajustes.invertirModulos = 0;
  ajustes.espejo = 0;

  // España peninsular con cambio de hora automático
  strcpy(ajustes.zona, "CET-1CEST,M3.5.0,M10.5.0/3");
  strcpy(ajustes.servidorHora, "es.pool.ntp.org");

  strcpy(ajustes.claveAdmin, "hyrule");
  // Las versiones nuevas se publican en GitHub Pages (ver README del proyecto)
  strcpy(ajustes.urlActualizacion, URL_ACTUALIZACIONES);
  ajustes.autoActualizar = 1;
}

bool asignar(const String& clave, const String& valor) {
  for (size_t i = 0; i < NUM_CAMPOS; i++) {
    const Campo& c = CAMPOS[i];
    if (clave != c.nombre) continue;
    if (c.tipo == NUMERO) {
      long n = valor.toInt();
      if (n < c.minimo) n = c.minimo;
      if (n > c.maximo) n = c.maximo;
      *(uint8_t*)c.dato = (uint8_t)n;
    } else {
      copiar((char*)c.dato, c.tam, valor);
    }
    return true;
  }
  return false;
}

bool cargar() {
  valoresPorDefecto();
  if (!LittleFS.begin()) {
    Serial.println(F("[ajustes] No se pudo abrir la memoria; se formatea."));
    LittleFS.format();
    if (!LittleFS.begin()) return false;
  }
  File f = LittleFS.open(ARCHIVO_AJUSTES, "r");
  if (!f) {
    Serial.println(F("[ajustes] Primera vez: ajustes de fábrica."));
    return false;
  }
  while (f.available()) {
    String linea = f.readStringUntil('\n');
    linea.replace("\r", "");
    int igual = linea.indexOf('=');
    if (igual <= 0) continue;
    asignar(linea.substring(0, igual), linea.substring(igual + 1));
  }
  f.close();
  Serial.println(F("[ajustes] Cargados."));
  return true;
}

bool guardar() {
  File f = LittleFS.open(ARCHIVO_AJUSTES, "w");
  if (!f) return false;
  for (size_t i = 0; i < NUM_CAMPOS; i++) {
    const Campo& c = CAMPOS[i];
    f.print(c.nombre);
    f.print('=');
    if (c.tipo == NUMERO) f.println(*(uint8_t*)c.dato);
    else                  f.println((const char*)c.dato);
  }
  f.close();
  Serial.println(F("[ajustes] Guardados."));
  return true;
}

String json() {
  String r = "{";
  for (size_t i = 0; i < NUM_CAMPOS; i++) {
    const Campo& c = CAMPOS[i];
    if (c.tipo == SECRETO) continue;          // las claves no salen nunca
    if (r.length() > 1) r += ',';
    r += '"'; r += c.nombre; r += "\":";
    if (c.tipo == NUMERO) r += String(*(uint8_t*)c.dato);
    else { r += '"'; r += escaparJson((const char*)c.dato); r += '"'; }
  }
  r += '}';
  return r;
}

} // namespace Config
