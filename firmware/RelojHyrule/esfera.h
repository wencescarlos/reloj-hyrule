// ===========================================================================
//  esfera.h  -  Cómo se dibuja la hora y la fecha en la pantalla
//  Reloj de Hyrule · Proyecto de fans SIN ÁNIMO DE LUCRO · No oficial
//  CC BY-NC-SA 4.0 · https://github.com/wencescarlos/reloj-hyrule
// ===========================================================================
#pragma once
#include <Arduino.h>
#include <time.h>

namespace Esfera {
  // Dibuja la hora (no llama a mostrar()). msDelSegundo: 0..999
  void hora(const struct tm& t, uint16_t msDelSegundo);
  // Dibuja la fecha corta "18/10"
  void fechaCorta(const struct tm& t);
}
