// ===========================================================================
//  frases.h  -  Frases de ánimo del Reloj de Hyrule
//  Reloj de Hyrule · Proyecto de fans SIN ÁNIMO DE LUCRO · No oficial
//  CC BY-NC-SA 4.0 · https://github.com/wencescarlos/reloj-hyrule
//  Frases en español y en inglés (se elige según el ajuste "idioma").
//  {N} se cambia por el nombre del niño o niña (o por "HÉROE" / "HERO").
//  \x01 = corazón   \x02 = rupia   \x03 = trifuerza   \x04 = destello
// ===========================================================================
#pragma once
#include "ajustes.h"

const char* const FRASES_ES[] = {
  "¡ES PELIGROSO IR SOLO! LLÉVATE ESTE RELOJ, {N}",
  "¡ESCUCHA! ¡ESCUCHA! {N}, HOY VA A SER UN GRAN DÍA",
  "{N}, HÉROE DE HYRULE \x03",
  "EL VALOR DE UN HÉROE ESTÁ EN SU CORAZÓN \x01",
  "HOY ES UN DÍA PERFECTO PARA UNA AVENTURA",
  "UN GRAN HÉROE TAMBIÉN SE LAVA LOS DIENTES \x04",
  "LOS HÉROES DE VERDAD RECOGEN SU CUARTO",
  "¿HAS BEBIDO AGUA, {N}? HASTA LOS HÉROES TIENEN SED",
  "LA TRIFUERZA DEL VALOR BRILLA EN TI \x03",
  "¡LA PRINCESA ZELDA TE MANDA UN SALUDO, {N}!",
  "CADA DÍA APRENDES ALGO NUEVO: ¡SUBES DE NIVEL!",
  "HAS CONSEGUIDO 100 RUPIAS DE PURA SIMPATÍA \x02",
  "UN HÉROE AYUDA A LOS DEMÁS \x01",
  "SI ALGO SALE MAL, LO INTENTAMOS OTRA VEZ. ¡ASÍ JUEGAN LOS HÉROES!",
  "TU HADA DICE: ¡QUÉ BIEN LO ESTÁS HACIENDO! \x04",
  "LA AVENTURA DE HOY: ¡SER AMABLE CON TODOS!",
  "¿HAS HECHO LOS DEBERES, {N}? \x04",
  "LEER UN LIBRO TAMBIÉN ES UNA AVENTURA",
  "HYRULE ES MÁS BONITO CUANDO SONRÍES \x01",
  "{N} + VALOR + SABIDURÍA + FUERZA = \x03",
};
const uint8_t NUM_FRASES_ES = sizeof(FRASES_ES) / sizeof(FRASES_ES[0]);

const char* const FRASES_EN[] = {
  "IT'S DANGEROUS TO GO ALONE! TAKE THIS CLOCK, {N}",
  "HEY! LISTEN! {N}, TODAY IS GOING TO BE A GREAT DAY",
  "{N}, HERO OF HYRULE \x03",
  "A HERO'S COURAGE LIVES IN THEIR HEART \x01",
  "TODAY IS A PERFECT DAY FOR AN ADVENTURE",
  "GREAT HEROES BRUSH THEIR TEETH TOO \x04",
  "REAL HEROES TIDY UP THEIR ROOM",
  "HAVE YOU HAD SOME WATER, {N}? EVEN HEROES GET THIRSTY",
  "THE TRIFORCE OF COURAGE SHINES IN YOU \x03",
  "PRINCESS ZELDA SAYS HELLO, {N}!",
  "EVERY DAY YOU LEARN SOMETHING NEW: LEVEL UP!",
  "YOU GOT 100 RUPEES OF PURE KINDNESS \x02",
  "A HERO HELPS OTHERS \x01",
  "IF SOMETHING GOES WRONG, WE TRY AGAIN. THAT'S HOW HEROES PLAY!",
  "YOUR FAIRY SAYS: YOU'RE DOING GREAT! \x04",
  "TODAY'S QUEST: BE KIND TO EVERYONE!",
  "HAVE YOU DONE YOUR HOMEWORK, {N}? \x04",
  "READING A BOOK IS AN ADVENTURE TOO",
  "HYRULE IS PRETTIER WHEN YOU SMILE \x01",
  "{N} + COURAGE + WISDOM + POWER = \x03",
};
const uint8_t NUM_FRASES_EN = sizeof(FRASES_EN) / sizeof(FRASES_EN[0]);

// Objetos que puede haber dentro del cofre
const char* const TESOROS_ES[] = {
  "¡HAS CONSEGUIDO UNA RUPIA VERDE! \x02",
  "¡HAS CONSEGUIDO UNA RUPIA AZUL! VALE 5 \x02",
  "¡HAS CONSEGUIDO UNA RUPIA ROJA! VALE 20 \x02",
  "¡HAS CONSEGUIDO UNA LLAVE PEQUEÑA!",
  "¡HAS CONSEGUIDO UN BOOMERANG!",
  "¡HAS CONSEGUIDO UN ARCO Y FLECHAS!",
  "¡HAS CONSEGUIDO UN ABRAZO GIGANTE! \x01",
  "¡HAS CONSEGUIDO UNA BOTELLA CON UN HADA! \x04",
  "¡HAS CONSEGUIDO EL MAPA DE LA MAZMORRA!",
};
const uint8_t NUM_TESOROS_ES = sizeof(TESOROS_ES) / sizeof(TESOROS_ES[0]);

const char* const TESOROS_EN[] = {
  "YOU GOT A GREEN RUPEE! \x02",
  "YOU GOT A BLUE RUPEE! IT'S WORTH 5 \x02",
  "YOU GOT A RED RUPEE! IT'S WORTH 20 \x02",
  "YOU GOT A SMALL KEY!",
  "YOU GOT A BOOMERANG!",
  "YOU GOT A BOW AND ARROWS!",
  "YOU GOT A GIANT HUG! \x01",
  "YOU GOT A FAIRY IN A BOTTLE! \x04",
  "YOU GOT THE DUNGEON MAP!",
};
const uint8_t NUM_TESOROS_EN = sizeof(TESOROS_EN) / sizeof(TESOROS_EN[0]);

// Una frase o un tesoro al azar en el idioma elegido
inline const char* fraseAlAzar() {
  return enIngles() ? FRASES_EN[random(NUM_FRASES_EN)] : FRASES_ES[random(NUM_FRASES_ES)];
}
inline const char* tesoroAlAzar() {
  return enIngles() ? TESOROS_EN[random(NUM_TESOROS_EN)] : TESOROS_ES[random(NUM_TESOROS_ES)];
}
