// ===========================================================================
//  red.h  -  WiFi, portal de configuración y página web del reloj
//  Reloj de Hyrule · Proyecto de fans SIN ÁNIMO DE LUCRO · No oficial
//  CC BY-NC-SA 4.0 · https://github.com/wencescarlos/reloj-hyrule
// ===========================================================================
#pragma once
#include <Arduino.h>

// Lo que se pide desde la página web; el bucle principal lo atiende
// cuando termina lo que está haciendo (así nunca se corta una animación).
struct Peticiones {
  int16_t animacion = -1;          // número de aventura, 255 = sorpresa
  String  mensaje;                 // texto para enseñar ya
  bool    prueba = false;          // patrón de prueba de pantalla
  bool    ajustesCambiados = false;
  bool    buscarActualizacion = false;
  uint32_t reiniciarEn = 0;        // millis() en que hay que reiniciar
};

extern Peticiones peticiones;

namespace Red {
  void iniciar();          // conecta al WiFi o crea la red propia
  void servicio();         // atender la web; se llama muy a menudo
  bool conectado();        // conectado al WiFi de casa
  bool modoPortal();       // con la red propia "Reloj-Hyrule" activa
  String ip();
}
