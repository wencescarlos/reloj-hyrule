// ===========================================================================
//  sonido.h  -  Zumbador: avisos y alarma con melodías ORIGINALES
//  Reloj de Hyrule · Proyecto de fans SIN ÁNIMO DE LUCRO · No oficial
//  CC BY-NC-SA 4.0 · https://github.com/wencescarlos/reloj-hyrule
//
//  Todas las melodías son composiciones originales de este proyecto, con aire
//  de videojuego de aventuras. El reloj NO reproduce música de los juegos.
//  El reloj solo suena cuando se pide: un aviso enviado desde la web o la
//  alarma de la mañana. Las aventuras y la hora no hacen ruido.
// ===========================================================================
#pragma once
#include <Arduino.h>

namespace Sonido {
  void        iniciar();                  // aplica pata y volumen de los ajustes
  bool        disponible();               // hay zumbador configurado y activado
  // Toca una melodía (0..total()-1). Si segundos > 0, la repite hasta cumplirlos.
  void        tocar(uint8_t melodia, uint16_t segundos = 0);
  void        parar();
  bool        sonando();
  void        servicio();                 // se llama muy a menudo (desde esperar)
  uint8_t     total();
  const char* nombre(uint8_t n);          // en el idioma del reloj
}
