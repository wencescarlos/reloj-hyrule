// ===========================================================================
//  frases.h  -  Frases de ánimo del Reloj de Hyrule
//  Reloj de Hyrule · Proyecto de fans SIN ÁNIMO DE LUCRO · No oficial
//  CC BY-NC-SA 4.0 · https://github.com/wencescarlos/reloj-hyrule
//  {N} se cambia por el nombre del niño o niña (o por "HÉROE" si no hay).
//  \x01 = corazón   \x02 = rupia   \x03 = trifuerza   \x04 = destello
// ===========================================================================
#pragma once

const char* const FRASES[] = {
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
const uint8_t NUM_FRASES = sizeof(FRASES) / sizeof(FRASES[0]);

// Objetos que puede haber dentro del cofre
const char* const TESOROS[] = {
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
const uint8_t NUM_TESOROS = sizeof(TESOROS) / sizeof(TESOROS[0]);
