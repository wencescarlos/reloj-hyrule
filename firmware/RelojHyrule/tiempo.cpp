// ===========================================================================
//  tiempo.cpp  -  Hora por internet y textos de fecha (español e inglés)
//  Reloj de Hyrule · Proyecto de fans SIN ÁNIMO DE LUCRO · No oficial
//  CC BY-NC-SA 4.0 · https://github.com/wencescarlos/reloj-hyrule
// ===========================================================================
#include "tiempo.h"
#include "ajustes.h"

namespace {
  const char* const DIAS_ES[]  = { "DOMINGO", "LUNES", "MARTES", "MIÉRCOLES",
                                "JUEVES", "VIERNES", "SÁBADO" };
  const char* const MESES_ES[] = { "ENERO", "FEBRERO", "MARZO", "ABRIL", "MAYO",
                                "JUNIO", "JULIO", "AGOSTO", "SEPTIEMBRE",
                                "OCTUBRE", "NOVIEMBRE", "DICIEMBRE" };
  const char* const NUMEROS_ES[] = { "DOCE", "UNA", "DOS", "TRES", "CUATRO",
                                  "CINCO", "SEIS", "SIETE", "OCHO", "NUEVE",
                                  "DIEZ", "ONCE", "DOCE" };

  const char* const DIAS_EN[]  = { "SUNDAY", "MONDAY", "TUESDAY", "WEDNESDAY",
                                   "THURSDAY", "FRIDAY", "SATURDAY" };
  const char* const MESES_EN[] = { "JANUARY", "FEBRUARY", "MARCH", "APRIL", "MAY",
                                   "JUNE", "JULY", "AUGUST", "SEPTEMBER",
                                   "OCTOBER", "NOVEMBER", "DECEMBER" };
  const char* const NUMEROS_EN[] = { "TWELVE", "ONE", "TWO", "THREE", "FOUR",
                                     "FIVE", "SIX", "SEVEN", "EIGHT", "NINE",
                                     "TEN", "ELEVEN", "TWELVE" };

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

const char* nombreDia(int d) {
  d = (d % 7 + 7) % 7;
  return enIngles() ? DIAS_EN[d] : DIAS_ES[d];
}
const char* nombreMes(int m) {
  m = (m % 12 + 12) % 12;
  return enIngles() ? MESES_EN[m] : MESES_ES[m];
}

String fechaLarga(const struct tm& t) {
  String r = nombreDia(t.tm_wday);
  if (enIngles()) {                       // "SUNDAY, OCTOBER 18"
    r += ", "; r += nombreMes(t.tm_mon); r += ' '; r += t.tm_mday;
  } else {                                // "DOMINGO 18 DE OCTUBRE"
    r += ' '; r += t.tm_mday; r += " DE "; r += nombreMes(t.tm_mon);
  }
  return r;
}

namespace {
  // "IT'S FIVE O'CLOCK IN THE AFTERNOON!"
  String horaHabladaIngles(const struct tm& t) {
    int h12 = t.tm_hour % 12;
    if (t.tm_hour == 12 && t.tm_min == 0) return "IT'S TWELVE NOON!";
    if (t.tm_hour == 0 && t.tm_min == 0)  return "IT'S MIDNIGHT!";
    String r = "IT'S ";
    if (t.tm_min == 15)      { r += "QUARTER PAST "; r += NUMEROS_EN[h12]; }
    else if (t.tm_min == 30) { r += "HALF PAST ";    r += NUMEROS_EN[h12]; }
    else {
      r += NUMEROS_EN[h12];
      if (t.tm_min == 0) r += " O'CLOCK";
    }
    if      (t.tm_hour < 6)  r += " AT NIGHT!";
    else if (t.tm_hour < 12) r += " IN THE MORNING!";
    else if (t.tm_hour < 18) r += " IN THE AFTERNOON!";
    else if (t.tm_hour < 21) r += " IN THE EVENING!";
    else                     r += " AT NIGHT!";
    return r;
  }
}

String horaHablada(const struct tm& t) {
  if (enIngles()) return horaHabladaIngles(t);
  int h12 = t.tm_hour % 12;
  String r = (h12 == 1) ? "¡ES LA " : "¡SON LAS ";
  r += NUMEROS_ES[h12];
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
