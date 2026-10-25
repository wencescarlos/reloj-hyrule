// ===========================================================================
//  principal.h  -  Arranque y bucle principal del reloj
//  Reloj de Hyrule · Proyecto de fans SIN ÁNIMO DE LUCRO · No oficial
//  CC BY-NC-SA 4.0 · https://github.com/wencescarlos/reloj-hyrule
// ===========================================================================
#pragma once

// Modo concentración: solo la hora, quieta, sin fecha ni aventuras ni saludos
namespace Concentracion {
  void    empezar(int minutos);    // minutos > 0; -1 = hasta que se quite
  void    terminar();              // la quita (también la del horario, hasta que acabe)
  bool    activa();
  bool    porHorario();            // la ha puesto el horario de estudio
  int32_t segundosQuedan();        // -1 = sin fin
}

namespace Principal {
  void iniciar();   // se llama una vez al encender
  void bucle();     // se llama continuamente
}
