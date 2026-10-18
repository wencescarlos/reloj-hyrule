// ===========================================================================
//
//      ▲        RELOJ DE HYRULE
//     ▲ ▲       Reloj con aventuras para pequeños héroes
//
//  Placa:     Wemos D1 mini (ESP8266)
//  Pantalla:  4 matrices LED MAX7219 de 8x8 (32 x 8 píxeles)
//  Conexión:  CLK -> D5   CS -> D6   DIN -> D7   VCC -> 5V   GND -> G
//
//  - Hora exacta por internet con cambio de hora automático
//  - Formato de 24 horas o de 12 horas con AM / PM, y fecha opcional
//  - Aventuras del héroe al azar: cofres, monstruos, hadas, la Trifuerza...
//  - Nombre del niño, cumpleaños, buenos días y buenas noches
//  - Modo noche, brillo suave y portal de configuración en el móvil
//  - Actualizaciones por internet
//
//  Proyecto de fans SIN ÁNIMO DE LUCRO: homenaje a The Legend of Zelda.
//  No oficial y sin relación con Nintendo (The Legend of Zelda es una marca
//  de Nintendo). Todos los dibujos, textos y el código son originales.
//  Prohibida la venta. Licencia CC BY-NC-SA 4.0 (ver LICENSE).
//  https://github.com/wencescarlos/reloj-hyrule
// ===========================================================================

// La lógica está en principal.cpp; aquí solo se arranca.
// (Así compila igual en el Arduino IDE, en PlatformIO y en GitHub Actions.)

#include "principal.h"

void setup() {
  Principal::iniciar();
}

void loop() {
  Principal::bucle();
}
