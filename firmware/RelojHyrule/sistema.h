// ===========================================================================
//  sistema.h  -  Utilidades compartidas por todos los módulos
//  Reloj de Hyrule · Proyecto de fans SIN ÁNIMO DE LUCRO · No oficial
//  CC BY-NC-SA 4.0 · https://github.com/wencescarlos/reloj-hyrule
// ===========================================================================
#pragma once
#include <Arduino.h>

// Espera los milisegundos indicados SIN bloquear el reloj: mientras tanto
// atiende la página web, el WiFi y las actualizaciones por la red.
void esperar(uint32_t ms);
