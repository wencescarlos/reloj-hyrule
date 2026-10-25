// ===========================================================================
//  sonido.cpp  -  Reproductor de melodías sin bloquear (PWM en una pata)
//  Reloj de Hyrule · Proyecto de fans SIN ÁNIMO DE LUCRO · No oficial
//  CC BY-NC-SA 4.0 · https://github.com/wencescarlos/reloj-hyrule
//
//  El volumen de un zumbador pasivo se regula con el ciclo de trabajo de la
//  onda cuadrada: al 50 % suena lo más fuerte posible; con pulsos más cortos,
//  más flojo. La frecuencia de la onda es la nota.
// ===========================================================================
#include "sonido.h"
#include "ajustes.h"

namespace {
  // Notas (Hz)
  enum : uint16_t {
    SIL = 0,
    NG4 = 392, NA4 = 440, NB4 = 494,
    NC5 = 523, ND5 = 587, NE5 = 659, NF5 = 698, NG5 = 784, NA5 = 880, NB5 = 988,
    NC6 = 1047, ND6 = 1175, NE6 = 1319, NF6 = 1397, NG6 = 1568, NA6 = 1760, NC7 = 2093,
    NDS5 = 622, NFS5 = 740, NGS5 = 831, NAS5 = 932,
  };
  struct Nota { uint16_t f; uint16_t ms; };
  #define FIN { 0, 0 }

  // ---- Melodías originales (compuestas para este reloj) ----
  const Nota AVENTURA[] PROGMEM = {          // fanfarria de salida
    {NC5,120},{NE5,120},{NG5,120},{NC6,260},{SIL,60},{NG5,120},{NA5,120},{NG5,120},{NE5,120},{NC6,480}, FIN };
  const Nota CAMPANITAS[] PROGMEM = {        // campanitas brillantes
    {NG6,90},{NE6,90},{NA6,90},{NG6,140},{SIL,50},{ND6,90},{NE6,90},{NC6,240}, FIN };
  const Nota AMANECER[] PROGMEM = {          // para despertar, va subiendo
    {NC5,260},{ND5,260},{NE5,260},{NG5,420},{SIL,120},{NE5,200},{NG5,200},{NA5,200},{NC6,600}, FIN };
  const Nota VICTORIA[] PROGMEM = {          // ¡trabajo terminado!
    {NE5,110},{NG5,110},{NC6,320},{SIL,50},{ND6,110},{NC6,110},{NG5,110},{NC6,520}, FIN };
  const Nota AVISO[] PROGMEM = {             // dos tonos, corto
    {NA5,140},{SIL,40},{NE6,140},{SIL,120},{NA5,140},{SIL,40},{NE6,220}, FIN };
  const Nota MISTERIO[] PROGMEM = {          // algo se esconde...
    {NA5,200},{NG5,200},{NE5,200},{NC5,260},{SIL,80},{NDS5,200},{NFS5,200},{NB5,520}, FIN };
  const Nota MARCHA[] PROGMEM = {            // paso de héroe
    {NC5,150},{NE5,150},{ND5,150},{NF5,150},{NE5,150},{NG5,150},{SIL,40},{NG5,110},{NA5,110},{NB5,110},{NC6,420}, FIN };
  const Nota NANA[] PROGMEM = {              // suave, para la noche
    {NE5,380},{NC5,380},{ND5,380},{NG4,620},{SIL,150},{NG4,300},{ND5,300},{NE5,300},{NC5,700}, FIN };

  struct Melodia { const char* es; const char* en; const Nota* notas; };
  const Melodia MELODIAS[] = {
    { "Aventura",    "Adventure",  AVENTURA },
    { "Campanitas",  "Little bells", CAMPANITAS },
    { "Amanecer",    "Sunrise",    AMANECER },
    { "Victoria",    "Victory",    VICTORIA },
    { "Aviso",       "Alert",      AVISO },
    { "Misterio",    "Mystery",    MISTERIO },
    { "Marcha",      "March",      MARCHA },
    { "Nana",        "Lullaby",    NANA },
  };
  const uint8_t NUM = sizeof(MELODIAS) / sizeof(MELODIAS[0]);

  const uint8_t  GPIO_D[9] = {16, 5, 4, 0, 2, 14, 12, 13, 15};   // D0..D8
  // Ciclo de trabajo (sobre 1023) para cada volumen 1..10: casi lineal al oído
  const uint16_t CICLO[11] = {0, 4, 9, 16, 28, 45, 70, 110, 170, 280, 512};

  int8_t   pata = -1;            // GPIO del zumbador (-1 = ninguno)
  const Nota* actual = nullptr;
  uint8_t  numMelodia = 0;
  uint16_t indice = 0;
  uint32_t finNota = 0;
  uint32_t finRepetir = 0;       // millis() hasta el que se repite (0 = una vez)

  void silencio() { if (pata >= 0) analogWrite(pata, 0); }

  void sonar(uint16_t f) {
    if (pata < 0) return;
    if (f == 0) { silencio(); return; }
    analogWriteFreq(f);
    uint8_t v = ajustes.volumen > 10 ? 10 : ajustes.volumen;
    analogWrite(pata, CICLO[v]);
  }
}

namespace Sonido {

void iniciar() {
  silencio();
  actual = nullptr;
  pata = -1;
  uint8_t p = ajustes.pinSonido;
  bool ocupada = (p == ajustes.pinDin || p == ajustes.pinClk || p == ajustes.pinCs);
  if (!ajustes.sonidoActivo || p > 8 || ocupada) {
    if (ocupada) Serial.println(F("[sonido] La pata del zumbador la usa la pantalla: sin sonido."));
    return;
  }
  pata = GPIO_D[p];
  pinMode(pata, OUTPUT);
  digitalWrite(pata, LOW);
  analogWriteRange(1023);
  Serial.printf("[sonido] Zumbador en D%u, volumen %u\n", p, ajustes.volumen);
}

bool disponible() { return pata >= 0; }

void tocar(uint8_t melodia, uint16_t segundos) {
  if (pata < 0 || melodia >= NUM) return;
  numMelodia = melodia;
  actual = MELODIAS[melodia].notas;
  indice = 0;
  finNota = millis();                       // empieza en el siguiente servicio()
  finRepetir = segundos ? millis() + segundos * 1000UL : 0;
  servicio();
}

void parar() { actual = nullptr; finRepetir = 0; silencio(); }

bool sonando() { return actual != nullptr; }

void servicio() {
  if (!actual) return;
  if ((int32_t)(millis() - finNota) < 0) return;
  Nota n;
  memcpy_P(&n, &actual[indice], sizeof(Nota));
  if (n.ms == 0) {                          // fin de la melodía
    if (finRepetir && (int32_t)(millis() - finRepetir) < 0) {
      indice = 0;                           // otra vez, tras una pausa
      silencio();
      finNota = millis() + 700;
      return;
    }
    parar();
    return;
  }
  sonar(n.f);
  finNota = millis() + n.ms;
  indice++;
}

uint8_t total() { return NUM; }

const char* nombre(uint8_t n) {
  if (n >= NUM) return "";
  return tr(MELODIAS[n].es, MELODIAS[n].en);
}

} // namespace Sonido
