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

  // Mensaje desde la página del móvil (/mensaje):
  //   intro: 0 = sin personaje, 1..PERSONAJES = un personaje, INTRO_SORPRESA = al azar,
  //          INTRO_AVENTURA + n = la aventura n completa antes del mensaje
  //   segundos: 0 = pasa una vez; si no, se repite hasta cumplir ese tiempo
  //   msPorColumna: velocidad (más = más despacio); 0 = la de los ajustes
  const uint8_t  PERSONAJES     = 6;
  const int16_t  INTRO_SORPRESA = 99;
  const int16_t  INTRO_AVENTURA = 100;
  void     mensajeEspecial(const char* texto, int16_t intro, uint16_t segundos, uint8_t msPorColumna);
  bool     mensajeEnCurso();
  uint16_t segundosMensaje();              // segundos que le quedan (0 = ninguno)
  void     pararMensaje();
  void patronPrueba(bool corta = false);
  void progreso(uint8_t porcentaje);       // barra de actualización

  // Cambia {N} por el nombre del niño o niña
  String personalizar(const char* texto);
}
