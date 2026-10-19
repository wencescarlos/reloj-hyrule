// ===========================================================================
//  tiempo.h  -  La hora exacta por internet (NTP) con cambio de hora automático
//  Reloj de Hyrule · Proyecto de fans SIN ÁNIMO DE LUCRO · No oficial
//  CC BY-NC-SA 4.0 · https://github.com/wencescarlos/reloj-hyrule
// ===========================================================================
#pragma once
#include <Arduino.h>
#include <time.h>

namespace Tiempo {
  void   iniciar();                     // aplica zona horaria y servidor
  bool   valido();                      // ¿ya sabemos la hora?
  bool   ahora(struct tm& t);           // hora local
  const char* nombreDia(int diaSemana); // 0 = domingo
  const char* nombreMes(int mes);       // 0 = enero
  String fechaLarga(const struct tm& t);        // "DOMINGO 18 DE OCTUBRE" / "SUNDAY, OCTOBER 18"
  String horaHablada(const struct tm& t);       // "¡SON LAS CINCO DE LA TARDE!" / "IT'S FIVE O'CLOCK..."
  bool   esNoche(const struct tm& t);
  bool   esCumple(const struct tm& t);
  int    diasParaCumple(const struct tm& t);    // -1 si no hay cumpleaños
}
