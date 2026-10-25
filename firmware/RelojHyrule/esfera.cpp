// ===========================================================================
//  esfera.cpp  -  La hora: cifras grandes de 5x7, AM/PM y el hada de los segundos
//  Reloj de Hyrule · Proyecto de fans SIN ÁNIMO DE LUCRO · No oficial
//  CC BY-NC-SA 4.0 · https://github.com/wencescarlos/reloj-hyrule
// ===========================================================================
#include "esfera.h"
#include "pantalla.h"
#include "ajustes.h"
#include "fuente.h"

namespace Esfera {

void hora(const struct tm& t, uint16_t ms, bool tranquila) {
  int h = t.tm_hour;
  bool doce = !ajustes.formato24;
  if (doce) { h = h % 12; if (h == 0) h = 12; }

  // Ancho total "HH:MM" = 5+1+5 +1+1+1+ 5+1+5 = 25 columnas
  // 24 h: centrado. 12 h: pegado a la izquierda para dejar sitio a la A / P
  int x = doce ? 0 : 3;

  bool primeraCifra = (h >= 10) || (!doce && ajustes.ceroIzquierda);
  if (primeraCifra) Pantalla::digito(h / 10, x, 0);
  Pantalla::digito(h % 10, x + 6, 0);

  // Los dos puntos: encendidos la primera mitad de cada segundo
  bool puntos = tranquila || !ajustes.parpadeo || ms < 500;
  if (puntos) {
    Pantalla::pixel(x + 12, 1);
    Pantalla::pixel(x + 12, 2);
    Pantalla::pixel(x + 12, 4);
    Pantalla::pixel(x + 12, 5);
  }
  Pantalla::digito(t.tm_min / 10, x + 14, 0);
  Pantalla::digito(t.tm_min % 10, x + 20, 0);

  if (doce) Pantalla::mini(t.tm_hour < 12 ? MINI_A : MINI_P, 28, 0);

  // Fila de abajo: los segundos (nunca en modo concentración)
  if (tranquila) return;
  if (ajustes.modoSegundos == SEG_BARRA) {
    int largo = (t.tm_sec * ANCHO_PANTALLA) / 60;
    if (largo > 0) Pantalla::lineaH(0, largo - 1, 7);
  } else if (ajustes.modoSegundos == SEG_HADA) {
    // El hada recorre la pantalla de ida y vuelta cada minuto
    float avance = (t.tm_sec + ms / 1000.0f) / 30.0f;      // 0..2
    float pos = avance <= 1.0f ? avance : 2.0f - avance;
    int hx = (int)(pos * (ANCHO_PANTALLA - 1) + 0.5f);
    Pantalla::pixel(hx, 7);
    if ((ms / 125) % 2) {                                  // estela brillante
      Pantalla::pixel(hx + (avance <= 1.0f ? -2 : 2), 7);
    }
  }
}

void fechaCorta(const struct tm& t) {
  int x = 1;
  int dia = t.tm_mday, mes = t.tm_mon + 1;
  Pantalla::digito(dia / 10, x, 0);
  Pantalla::digito(dia % 10, x + 6, 0);
  Pantalla::texto("/", x + 12, -1);
  Pantalla::digito(mes / 10, x + 18, 0);
  Pantalla::digito(mes % 10, x + 24, 0);
}

} // namespace Esfera
