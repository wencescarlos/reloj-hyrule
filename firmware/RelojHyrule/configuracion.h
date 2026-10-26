// ===========================================================================
//  configuracion.h  -  Constantes generales del Reloj de Hyrule
//  Reloj de Hyrule · Proyecto de fans SIN ÁNIMO DE LUCRO · No oficial
//  CC BY-NC-SA 4.0 · https://github.com/wencescarlos/reloj-hyrule
//
//  Proyecto de fans SIN ÁNIMO DE LUCRO. No oficial: no está relacionado
//  con Nintendo. The Legend of Zelda es una marca de Nintendo.
//  Prohibida la venta de este proyecto o de relojes hechos con él.
// ===========================================================================
#pragma once
#include <Arduino.h>

// --- Versión del programa (súbela cada vez que publiques una actualización) ---
#define VERSION_RELOJ        "1.4.1"

// --- Conexiones Wemos D1 mini -> matrices MAX7219 (de fábrica) ---
//     CLK -> D5 (SCK)   DIN -> D7 (MOSI)   CS -> D6
//     Se pueden cambiar desde la página del reloj (Diagnóstico de pantalla),
//     por ejemplo si una pata del Wemos se ha estropeado.
#define PIN_DIN_FABRICA      7                    // D7
#define PIN_CLK_FABRICA      5                    // D5
#define PIN_CS_FABRICA       6                    // D6
#define MINUTOS_DIAGNOSTICO  10                   // el diagnóstico se cierra solo

// --- Pantalla ---
#define NUM_MODULOS          4                    // matrices de 8x8 en fila
#define ANCHO_PANTALLA       (NUM_MODULOS * 8)    // 32 columnas
#define ALTO_PANTALLA        8

// --- Red ---
#define NOMBRE_EQUIPO        "reloj-hyrule"       // http://reloj-hyrule.local
#define WIFI_PROPIO_NOMBRE   "Reloj-Hyrule"       // red que crea si no hay WiFi
#define WIFI_PROPIO_CLAVE    "trifuerza"          // mínimo 8 letras
#define SEGUNDOS_CONECTAR    20                   // espera al conectar al WiFi
#define MINUTOS_REINTENTO    2                    // reintento si el WiFi falla

// --- Actualizaciones automáticas por internet (GitHub Pages del proyecto) ---
//     Ahí se publican version.txt y firmware.bin (ver .github/workflows)
#define URL_ACTUALIZACIONES  "https://wencescarlos.github.io/reloj-hyrule/"
#define URL_PROYECTO         "https://github.com/wencescarlos/reloj-hyrule"

// --- Archivo donde se guardan los ajustes (memoria interna LittleFS) ---
#define ARCHIVO_AJUSTES      "/ajustes.cfg"
