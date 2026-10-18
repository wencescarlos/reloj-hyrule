// ===========================================================================
//  sprites.h  -  Dibujos originales en píxeles del Reloj de Hyrule
//  Reloj de Hyrule · Proyecto de fans SIN ÁNIMO DE LUCRO · No oficial
//  CC BY-NC-SA 4.0 · https://github.com/wencescarlos/reloj-hyrule
//  Cada sprite se guarda por columnas (bit 0 = fila de arriba), hasta 16 filas.
//  Homenaje sin ánimo de lucro: dibujos propios, hechos píxel a píxel.
// ===========================================================================
#pragma once
#include <Arduino.h>

struct Sprite {
  uint8_t ancho;
  uint8_t alto;
  const uint16_t* columnas;   // en PROGMEM
};

const uint16_t SPR_HEROE_A_DATOS[] PROGMEM = { 0x0024, 0x0096, 0x007B, 0x003F, 0x007B, 0x009E, 0x0020, 0x0010 };
const Sprite SPR_HEROE_A = { 8, 8, SPR_HEROE_A_DATOS };

const uint16_t SPR_HEROE_B_DATOS[] PROGMEM = { 0x0024, 0x0016, 0x00FB, 0x003F, 0x00FB, 0x001E, 0x0020, 0x0010 };
const Sprite SPR_HEROE_B = { 8, 8, SPR_HEROE_B_DATOS };

const uint16_t SPR_HEROE_ATAQUE_DATOS[] PROGMEM = { 0x0024, 0x0096, 0x007B, 0x003F, 0x007B, 0x009E, 0x0010, 0x0000, 0x0010, 0x0010, 0x0010, 0x0010 };
const Sprite SPR_HEROE_ATAQUE = { 12, 8, SPR_HEROE_ATAQUE_DATOS };

const uint16_t SPR_HEROE_TESORO_DATOS[] PROGMEM = { 0x0003, 0x00AC, 0x0076, 0x003E, 0x0076, 0x00AC, 0x0003, 0x0000 };
const Sprite SPR_HEROE_TESORO = { 8, 8, SPR_HEROE_TESORO_DATOS };

const uint16_t SPR_CORAZON_DATOS[] PROGMEM = { 0x0006, 0x000F, 0x001F, 0x003E, 0x001F, 0x000F, 0x0006 };
const Sprite SPR_CORAZON = { 7, 6, SPR_CORAZON_DATOS };

const uint16_t SPR_CORAZON_VACIO_DATOS[] PROGMEM = { 0x0006, 0x0009, 0x0011, 0x0022, 0x0011, 0x0009, 0x0006 };
const Sprite SPR_CORAZON_VACIO = { 7, 6, SPR_CORAZON_VACIO_DATOS };

const uint16_t SPR_RUPIA_DATOS[] PROGMEM = { 0x001C, 0x0022, 0x005D, 0x0022, 0x001C };
const Sprite SPR_RUPIA = { 5, 7, SPR_RUPIA_DATOS };

const uint16_t SPR_RUPIA_MINI_DATOS[] PROGMEM = { 0x0006, 0x000F, 0x0006 };
const Sprite SPR_RUPIA_MINI = { 3, 4, SPR_RUPIA_MINI_DATOS };

const uint16_t SPR_COFRE_DATOS[] PROGMEM = { 0x007E, 0x0045, 0x0045, 0x0055, 0x005D, 0x0055, 0x0045, 0x0045, 0x007E };
const Sprite SPR_COFRE = { 9, 7, SPR_COFRE_DATOS };

const uint16_t SPR_COFRE_ABIERTO_DATOS[] PROGMEM = { 0x007B, 0x0049, 0x0049, 0x0059, 0x0059, 0x0059, 0x0049, 0x0049, 0x007B };
const Sprite SPR_COFRE_ABIERTO = { 9, 7, SPR_COFRE_ABIERTO_DATOS };

const uint16_t SPR_TRIANGULO_DATOS[] PROGMEM = { 0x0008, 0x000C, 0x000E, 0x000F, 0x000E, 0x000C, 0x0008 };
const Sprite SPR_TRIANGULO = { 7, 4, SPR_TRIANGULO_DATOS };

const uint16_t SPR_HADA_A_DATOS[] PROGMEM = { 0x001B, 0x000A, 0x0004, 0x000A, 0x001B };
const Sprite SPR_HADA_A = { 5, 5, SPR_HADA_A_DATOS };

const uint16_t SPR_HADA_B_DATOS[] PROGMEM = { 0x0004, 0x000E, 0x0004, 0x000A, 0x0004 };
const Sprite SPR_HADA_B = { 5, 5, SPR_HADA_B_DATOS };

const uint16_t SPR_MONSTRUO_A_DATOS[] PROGMEM = { 0x005C, 0x003E, 0x001B, 0x007F, 0x007F, 0x001B, 0x003E, 0x005C };
const Sprite SPR_MONSTRUO_A = { 8, 7, SPR_MONSTRUO_A_DATOS };

const uint16_t SPR_MONSTRUO_B_DATOS[] PROGMEM = { 0x0078, 0x007C, 0x0036, 0x007E, 0x007E, 0x0036, 0x007C, 0x0078 };
const Sprite SPR_MONSTRUO_B = { 8, 7, SPR_MONSTRUO_B_DATOS };

const uint16_t SPR_HUMO_1_DATOS[] PROGMEM = { 0x0000, 0x0014, 0x002A, 0x0014, 0x002A, 0x0014, 0x0000 };
const Sprite SPR_HUMO_1 = { 7, 7, SPR_HUMO_1_DATOS };

const uint16_t SPR_HUMO_2_DATOS[] PROGMEM = { 0x0049, 0x0000, 0x0014, 0x0041, 0x0014, 0x0000, 0x0049 };
const Sprite SPR_HUMO_2 = { 7, 7, SPR_HUMO_2_DATOS };

const uint16_t SPR_BOMBA_DATOS[] PROGMEM = { 0x0070, 0x00D8, 0x00EC, 0x00FC, 0x00FC, 0x00FA, 0x0071 };
const Sprite SPR_BOMBA = { 7, 8, SPR_BOMBA_DATOS };

const uint16_t SPR_ESPADA_DATOS[] PROGMEM = { 0x0300, 0x11FE, 0x1F03, 0x11FE, 0x0300 };
const Sprite SPR_ESPADA = { 5, 13, SPR_ESPADA_DATOS };

const uint16_t SPR_LUNA_DATOS[] PROGMEM = { 0x001C, 0x003E, 0x0063, 0x0041, 0x0041, 0x0000 };
const Sprite SPR_LUNA = { 6, 7, SPR_LUNA_DATOS };

const uint16_t SPR_TARTA_DATOS[] PROGMEM = { 0x0078, 0x00EC, 0x00FF, 0x00EC, 0x00F8, 0x00EC, 0x00FF, 0x00EC, 0x00F8, 0x00EC, 0x00FF, 0x00EC, 0x0078 };
const Sprite SPR_TARTA = { 13, 8, SPR_TARTA_DATOS };

const uint16_t SPR_LLAMA_DATOS[] PROGMEM = { 0x0001 };
const Sprite SPR_LLAMA = { 1, 1, SPR_LLAMA_DATOS };

