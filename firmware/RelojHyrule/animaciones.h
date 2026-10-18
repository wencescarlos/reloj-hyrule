// ===========================================================================
//  animaciones.h  -  Las aventuras del héroe en la pantalla
//  Reloj de Hyrule · Proyecto de fans SIN ÁNIMO DE LUCRO · No oficial
//  CC BY-NC-SA 4.0 · https://github.com/wencescarlos/reloj-hyrule
// ===========================================================================
#pragma once
#include <Arduino.h>
#include <time.h>

namespace Anim {
  // Aventuras que salen al azar (también se pueden lanzar desde la web)
  uint8_t     total();
  const char* nombre(uint8_t n);
  void        lanzar(uint8_t n);
  void        alAzar(const struct tm& t);

  // Momentos especiales
  void arranque();
  void conectando(uint8_t paso);           // un cuadro de "conectando"
  void instruccionesWifi();
  void buscandoHora();
  void horaEnPunto(const struct tm& t);
  void buenosDias(const struct tm& t);
  void buenasNoches();
  void cumple();
  void fecha(const struct tm& t);
  void mensaje(const char* texto);
  void patronPrueba();
  void progreso(uint8_t porcentaje);       // barra de actualización

  // Cambia {N} por el nombre del niño o niña
  String personalizar(const char* texto);
}
