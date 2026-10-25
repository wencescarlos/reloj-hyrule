// ===========================================================================
//  ajustes.h  -  Ajustes del reloj guardados en la memoria interna
//  Reloj de Hyrule · Proyecto de fans SIN ÁNIMO DE LUCRO · No oficial
//  CC BY-NC-SA 4.0 · https://github.com/wencescarlos/reloj-hyrule
// ===========================================================================
#pragma once
#include <Arduino.h>

// Formas de enseñar la fecha
enum ModoFecha : uint8_t {
  FECHA_NUNCA      = 0,   // solo la hora
  FECHA_DESPLAZAR  = 1,   // "DOMINGO 18 DE OCTUBRE" cada X minutos
  FECHA_ALTERNAR   = 2,   // "18/10" unos segundos cada medio minuto
};

// Indicador de los segundos en la fila de abajo
enum ModoSegundos : uint8_t {
  SEG_NADA  = 0,
  SEG_BARRA = 1,          // barra que se llena durante el minuto
  SEG_HADA  = 2,          // un hada (un punto) que va y viene
};

// Idioma de los textos del reloj y de su página web
enum Idioma : uint8_t {
  IDIOMA_ES = 0,          // español
  IDIOMA_EN = 1,          // inglés
};

struct Ajustes {
  // --- Idioma ---
  uint8_t idioma;              // Idioma

  // --- Héroe ---
  char    nombre[24];          // nombre del niño o niña
  uint8_t cumpleDia;           // 0 = sin cumpleaños
  uint8_t cumpleMes;
  char    mensaje[96];         // mensaje personalizado (vacío = ninguno)
  uint8_t saludos;             // buenos días / buenas noches

  // --- Reloj ---
  uint8_t formato24;           // 1 = 24 horas, 0 = 12 horas con AM / PM
  uint8_t ceroIzquierda;       // 09:05 en lugar de 9:05 (modo 24 h)
  uint8_t modoFecha;           // ModoFecha
  uint8_t fechaCadaMin;        // para FECHA_DESPLAZAR
  uint8_t modoSegundos;        // ModoSegundos
  uint8_t parpadeo;            // los dos puntos parpadean
  uint8_t horaEnPunto;         // animación especial en las horas en punto

  // --- Aventuras (animaciones) ---
  uint8_t animCadaMin;         // 0 = desactivadas
  uint8_t frases;              // mezclar frases de ánimo entre animaciones

  // --- Noche ---
  uint8_t nocheActiva;
  uint8_t nocheIniH, nocheIniM;
  uint8_t nocheFinH, nocheFinM;
  uint8_t nocheApagar;         // 1 = pantalla apagada, 0 = muy tenue
  uint8_t brilloNoche;

  // --- Pantalla ---
  uint8_t brillo;              // 0..15
  uint8_t velocidad;           // ms por columna al desplazar texto
  uint8_t rotacion;            // 0..3
  uint8_t invertirModulos;
  uint8_t espejo;

  // --- Hora por internet ---
  char    zona[56];            // zona horaria en formato POSIX
  char    servidorHora[48];

  // --- Red y seguridad ---
  char    wifiNombre[33];
  char    wifiClave[65];
  char    claveAdmin[33];      // para actualizar el programa

  // --- Actualizaciones por internet ---
  char    urlActualizacion[128];
  uint8_t autoActualizar;

  // --- Conexión de la pantalla (número de la pata: 7 = D7) ---
  uint8_t pinDin;
  uint8_t pinClk;
  uint8_t pinCs;
  uint8_t spiLento;            // 1 = señales lentas (cables largos o dudosos)

  // --- Sonido (zumbador pasivo) ---
  uint8_t sonidoActivo;
  uint8_t volumen;             // 1..10
  uint8_t pinSonido;           // número de la pata: 1 = D1

  // --- Alarma de la mañana ---
  uint8_t alarmaActiva;
  uint8_t alarmaH, alarmaM;
  uint8_t alarmaDias;          // bit 0 = domingo, 1 = lunes ... 6 = sábado
  uint8_t alarmaMelodia;
  uint8_t alarmaSegundos;      // cuánto suena (3..60)
  char    alarmaTexto[48];     // vacío = "¡BUENOS DÍAS, {N}!"

  // --- Modo concentración con horario (deberes) ---
  uint8_t estudioActivo;
  uint8_t estudioIniH, estudioIniM;
  uint8_t estudioFinH, estudioFinM;
  uint8_t estudioDias;         // como alarmaDias
  uint8_t estudioAviso;        // al terminar: melodía y "¡BUEN TRABAJO!"
};

extern Ajustes ajustes;

// Elige el texto según el idioma: tr("HOLA", "HELLO")
inline bool enIngles() { return ajustes.idioma == IDIOMA_EN; }
inline const char* tr(const char* es, const char* en) { return enIngles() ? en : es; }

namespace Config {
  void valoresPorDefecto();
  bool cargar();
  bool guardar();
  // Cambia un ajuste a partir de su nombre y su valor en texto
  bool asignar(const String& clave, const String& valor);
  // Devuelve los ajustes en formato JSON (sin contraseñas)
  String json();
}
