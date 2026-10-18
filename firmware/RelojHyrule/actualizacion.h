// ===========================================================================
//  actualizacion.h  -  Actualizar el programa por internet
//  Reloj de Hyrule · Proyecto de fans SIN ÁNIMO DE LUCRO · No oficial
//  CC BY-NC-SA 4.0 · https://github.com/wencescarlos/reloj-hyrule
//
//  Tres caminos:
//   1. Automático: el reloj mira cada día <url>/version.txt y, si hay una
//      versión nueva, descarga <url>/firmware.bin y se reinicia solo.
//   2. Desde la página web: subir el archivo .bin en http://reloj-hyrule.local/actualizar
//   3. Desde el Arduino IDE por WiFi (puerto de red "reloj-hyrule").
// ===========================================================================
#pragma once
#include <Arduino.h>

namespace Actualizacion {
  void   iniciarOTA();              // actualización desde el Arduino IDE
  void   servicio();                // revisión diaria automática
  void   comprobar();               // mirar ahora si hay versión nueva
  String estado();                  // último resultado, para la web
}
