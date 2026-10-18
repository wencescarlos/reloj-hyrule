// ===========================================================================
//  tiempo.cpp  -  Hora por internet y textos de fecha en español
//  Reloj de Hyrule · Proyecto de fans SIN ÁNIMO DE LUCRO · No oficial
//  CC BY-NC-SA 4.0 · https://github.com/wencescarlos/reloj-hyrule
// ===========================================================================
#include "tiempo.h"
#include "ajustes.h"

namespace {
  const char* const DIAS[]  = { "DOMINGO", "LUNES", "MARTES", "MIÉRCOLES",
                                "JUEVES", "VIERNES", "SÁBADO" };
  const char* const MESES[] = { "ENERO", "FEBRERO", "MARZO", "ABRIL", "MAYO",
                                "JUNIO", "JULIO", "AGOSTO", "SEPTIEMBRE",
                                "OCTUBRE", "NOVIEMBRE", "DICIEMBRE" };
  const char* const NUMEROS[] = { "DOCE", "UNA", "DOS", "TRES", "CUATRO",
                                  "CINCO", "SEIS", "SIETE", "OCHO", "NUEVE",
                                  "DIEZ", "ONCE", "DOCE" };

  int minutosDelDia(int h, int m) { return h * 60 + m; }

  // Día del año (0..365) de una fecha cualquiera, para contar días
  int diaDelAnio(int anio, int mes, int dia) {
    struct tm t = {};
    t.tm_year = anio - 1900; t.tm_mon = mes; t.tm_mday = dia; t.tm_hour = 12;
    mktime(&t);
    return t.tm_yday;
  }
  bool bisiesto(int a) { return (a % 4 == 0 && a % 100 != 0) || a % 400 == 0; }
}

namespace Tiempo {

void iniciar() {
  configTime(ajustes.zona, ajustes.servidorHora, "pool.ntp.org", "time.google.com");
}

bool valido() {
  return time(nullptr) > 1700000000;   // fecha posterior a noviembre de 2023
}

bool ahora(struct tm& t) {
  time_t s = time(nullptr);
  localtime_r(&s, &t);
  return valido();
}

const char* nombreDia(int d) { return DIAS[(d % 7 + 7) % 7]; }
const char* nombreMes(int m) { return MESES[(m % 12 + 12) % 12]; }

String fechaLarga(const struct tm& t) {
  String r = nombreDia(t.tm_wday);
  r += ' '; r += t.tm_mday; r += " DE "; r += nombreMes(t.tm_mon);
  return r;
}

String horaHablada(const struct tm& t) {
  int h12 = t.tm_hour % 12;
  String r = (h12 == 1) ? "¡ES LA " : "¡SON LAS ";
  r += NUMEROS[h12];
  if (t.tm_min == 0) r += " EN PUNTO";
  else if (t.tm_min == 15) r += " Y CUARTO";
  else if (t.tm_min == 30) r += " Y MEDIA";
  if      (t.tm_hour < 6)  r += " DE LA MADRUGADA!";
  else if (t.tm_hour < 12) r += " DE LA MAÑANA!";
  else if (t.tm_hour == 12 && t.tm_min == 0) r = "¡SON LAS DOCE DEL MEDIODÍA!";
  else if (t.tm_hour < 21) r += " DE LA TARDE!";
  else                     r += " DE LA NOCHE!";
  return r;
}

bool esNoche(const struct tm& t) {
  if (!ajustes.nocheActiva) return false;
  int ahora  = minutosDelDia(t.tm_hour, t.tm_min);
  int inicio = minutosDelDia(ajustes.nocheIniH, ajustes.nocheIniM);
  int fin    = minutosDelDia(ajustes.nocheFinH, ajustes.nocheFinM);
  if (inicio == fin) return false;
  if (inicio < fin) return ahora >= inicio && ahora < fin;
  return ahora >= inicio || ahora < fin;     // la noche cruza las 00:00
}

bool esCumple(const struct tm& t) {
  return ajustes.cumpleDia && ajustes.cumpleMes &&
         t.tm_mday == ajustes.cumpleDia && t.tm_mon + 1 == ajustes.cumpleMes;
}

int diasParaCumple(const struct tm& t) {
  if (!ajustes.cumpleDia || !ajustes.cumpleMes) return -1;
  int anio = t.tm_year + 1900;
  int hoy = t.tm_yday;
  int cumple = diaDelAnio(anio, ajustes.cumpleMes - 1, ajustes.cumpleDia);
  if (cumple >= hoy) return cumple - hoy;
  int diasAnio = bisiesto(anio) ? 366 : 365;
  return diasAnio - hoy + diaDelAnio(anio + 1, ajustes.cumpleMes - 1, ajustes.cumpleDia);
}

} // namespace Tiempo
