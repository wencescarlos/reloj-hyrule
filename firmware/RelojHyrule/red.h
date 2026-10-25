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
  int16_t  mensajeIntro = 1;       // personaje o aventura antes del mensaje (ver Anim::mensajeEspecial)
  uint16_t mensajeSegundos = 0;    // 0 = una vez
  uint8_t  mensajeVelocidad = 0;   // ms por columna (0 = la de los ajustes)
  uint8_t  mensajeSonido = 0;      // 0 = sin sonido; n = melodía n-1
  uint8_t prueba = 0;              // patrón de prueba: 1 = completo, 2 = solo 1 2 3 4
  bool    ajustesCambiados = false;
  int8_t  diagnostico = -1;        // orden de diagnóstico pendiente (ver principal.cpp)
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

// Estado del diagnóstico de pantalla (lo pone principal.cpp, lo lee la web)
extern String estadoDiagnostico;
