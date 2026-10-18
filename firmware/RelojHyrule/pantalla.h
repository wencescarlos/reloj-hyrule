// ===========================================================================
//  pantalla.h  -  Control de las 4 matrices LED MAX7219 (32 x 8 píxeles)
//  Reloj de Hyrule · Proyecto de fans SIN ÁNIMO DE LUCRO · No oficial
//  CC BY-NC-SA 4.0 · https://github.com/wencescarlos/reloj-hyrule
//  Controlador propio: memoria de imagen, sprites, texto y desplazamiento.
// ===========================================================================
#pragma once
#include <Arduino.h>
#include "configuracion.h"
#include "sprites.h"

namespace Pantalla {

  // Puesta en marcha y orientación de los módulos
  void iniciar();
  void orientar(uint8_t rotacion, bool invertirModulos, bool espejo);

  // Brillo (0..15) y apagado total
  void brillo(uint8_t nivel);
  void encender(bool si);

  // Memoria de imagen
  void limpiar();
  void pixel(int x, int y, bool encendido = true);
  bool leer(int x, int y);
  void invertir();                 // cambia todos los píxeles
  void mostrar();                  // envía la imagen a las matrices
  void guardar(uint8_t* copia);    // copia la imagen (32 bytes)
  void restaurar(const uint8_t* copia);

  // Dibujo
  void sprite(const Sprite& s, int x, int y, bool espejoH = false);
  void lineaH(int x0, int x1, int y, bool encendido = true);
  void rectangulo(int x, int y, int ancho, int alto, bool relleno = false);
  void mini(const uint8_t* letra3x5, int x, int y);   // letra pequeña 3x5

  // Texto (UTF-8, en español con tildes y Ñ; se escribe en mayúsculas)
  int  anchoTexto(const char* texto);
  int  texto(const char* texto, int x, int y = 0);    // devuelve x final
  int  digito(uint8_t d, int x, int y);               // cifra de 5x7

  // Desplaza un texto de derecha a izquierda (bloqueante, pero atiende la red)
  void desplazar(const char* texto, uint16_t msPorColumna);

  // Transiciones entre dos imágenes
  void cortinaSalida(uint16_t ms);   // borra columna a columna
}
