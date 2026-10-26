// ===========================================================================
//  pantalla.cpp  -  Controlador propio de las matrices MAX7219
//  Reloj de Hyrule · Proyecto de fans SIN ÁNIMO DE LUCRO · No oficial
//  CC BY-NC-SA 4.0 · https://github.com/wencescarlos/reloj-hyrule
// ===========================================================================
#include <SPI.h>
#include "pantalla.h"
#include "fuente.h"
#include "sistema.h"

namespace {
  // Memoria de imagen: una columna por byte, bit 0 = fila de arriba
  uint8_t imagen[ANCHO_PANTALLA];

  uint8_t rotacionModulo = 0;     // 0, 1, 2, 3  (x 90 grados)
  bool    ordenInvertido = false; // el primer módulo está a la derecha
  bool    espejoModulo   = false; // cada módulo se ve al revés

  // Patas de la pantalla (GPIO del ESP8266) y forma de enviar
  const uint8_t GPIO_D[9] = {16, 5, 4, 0, 2, 14, 12, 13, 15};   // D0..D8
  uint8_t dDin = PIN_DIN_FABRICA, dClk = PIN_CLK_FABRICA, dCs = PIN_CS_FABRICA;
  uint8_t gDin = 13, gClk = 14, gCs = 12;
  bool    spiChip = true;         // SPI del chip (rápido) o señales a mano
  bool    lento   = false;
  bool    spiAbierto = false;

  // Estado que se vuelve a enviar cada segundo (ver reconfigurar)
  uint8_t  nivelBrillo = 1;
  bool     pantallaEncendida = true;
  uint32_t ultimaConfig = 0;
  volatile bool cortarYa = false;

  inline void retardo() { delayMicroseconds(lento ? 5 : 1); }

  // Envía un byte, el bit más alto primero (modo 0: el MAX7219 lee al subir CLK)
  void byteSalida(uint8_t b) {
    if (spiChip) { SPI.transfer(b); return; }
    for (uint8_t i = 0; i < 8; i++) {
      digitalWrite(gDin, (b & 0x80) ? HIGH : LOW);
      b <<= 1;
      retardo();
      digitalWrite(gClk, HIGH);
      retardo();
      digitalWrite(gClk, LOW);
    }
  }

  // Registros del MAX7219
  const uint8_t REG_DECODIFICAR = 0x09;
  const uint8_t REG_INTENSIDAD  = 0x0A;
  const uint8_t REG_LIMITE      = 0x0B;
  const uint8_t REG_APAGADO     = 0x0C;
  const uint8_t REG_PRUEBA      = 0x0F;

  // Envía el mismo registro/valor a todos los módulos de la cadena
  void enviarTodos(uint8_t registro, uint8_t valor) {
    digitalWrite(gCs, LOW);
    retardo();
    for (uint8_t i = 0; i < NUM_MODULOS; i++) {
      byteSalida(registro);
      byteSalida(valor);
    }
    digitalWrite(gCs, HIGH);
    retardo();
  }

  // Convierte un trozo de UTF-8 en el código interno de la fuente
  uint8_t siguienteCaracter(const char*& p) {
    uint8_t c = (uint8_t)*p++;
    if (c >= 'a' && c <= 'z') return c - 32;              // a mayúsculas
    if (c == 0xC3) {                                      // letras con tilde
      uint8_t d = (uint8_t)*p; if (d) p++;
      switch (d) {
        case 0xA1: case 0x81: return CAR_A_TILDE;   // á Á
        case 0xA9: case 0x89: return CAR_E_TILDE;   // é É
        case 0xAD: case 0x8D: return CAR_I_TILDE;   // í Í
        case 0xB3: case 0x93: return CAR_O_TILDE;   // ó Ó
        case 0xBA: case 0x9A: return CAR_U_TILDE;   // ú Ú
        case 0xBC: case 0x9C: return CAR_U_DIERESIS;// ü Ü
        case 0xB1: case 0x91: return CAR_ENIE;      // ñ Ñ
        case 0xA0: case 0xA4: case 0x80: return 'A';
        case 0xA8: case 0x88: return 'E';
        case 0xB2: case 0x92: return 'O';
        case 0xA7: case 0x87: return 'C';           // ç
        default: return '?';
      }
    }
    if (c == 0xC2) {
      uint8_t d = (uint8_t)*p; if (d) p++;
      if (d == 0xA1) return CAR_ABRE_EXCL;    // ¡
      if (d == 0xBF) return CAR_ABRE_INTERR;  // ¿
      if (d == 0xB0 || d == 0xBA) return CAR_GRADO;
      return ' ';
    }
    if (c >= 0x80) {                                      // otro UTF-8: saltar
      while (((uint8_t)*p & 0xC0) == 0x80) p++;
      return '?';
    }
    return c;
  }

  uint8_t glifo(uint8_t codigo) {
    uint8_t g = pgm_read_byte(&FUENTE_MAPA[codigo]);
    if (g == 255) g = pgm_read_byte(&FUENTE_MAPA['?']);
    return g;
  }

  // Dibuja un glifo y devuelve su ancho
  int dibujarGlifo(uint8_t codigo, int x, int y) {
    if (codigo == ' ') return 3;
    uint8_t g = glifo(codigo);
    uint16_t inicio = pgm_read_word(&FUENTE_INICIO[g]);
    uint8_t ancho = pgm_read_byte(&FUENTE_ANCHO[g]);
    for (uint8_t i = 0; i < ancho; i++) {
      int cx = x + i;
      if (cx < 0 || cx >= ANCHO_PANTALLA) continue;
      uint8_t col = pgm_read_byte(&FUENTE_DATOS[inicio + i]);
      for (uint8_t f = 0; f < 8; f++)
        if (col & (1 << f)) Pantalla::pixel(cx, f + y, true);
    }
    return ancho;
  }

  int anchoGlifo(uint8_t codigo) {
    if (codigo == ' ') return 3;
    return pgm_read_byte(&FUENTE_ANCHO[glifo(codigo)]);
  }
}

namespace Pantalla {

void pines(uint8_t din, uint8_t clk, uint8_t cs, bool modoLento) {
  if (din > 8) din = PIN_DIN_FABRICA;
  if (clk > 8) clk = PIN_CLK_FABRICA;
  if (cs > 8)  cs  = PIN_CS_FABRICA;
  if (din == clk || din == cs || clk == cs) {        // patas repetidas: de fábrica
    din = PIN_DIN_FABRICA; clk = PIN_CLK_FABRICA; cs = PIN_CS_FABRICA;
  }
  dDin = din; dClk = clk; dCs = cs;
  gDin = GPIO_D[din]; gClk = GPIO_D[clk]; gCs = GPIO_D[cs];
  lento = modoLento;
  // Con las patas de fábrica siempre se usa el SPI del chip (también en modo
  // lento: solo baja la velocidad). Con otras patas, las señales van a mano.
  spiChip = (din == 7 && clk == 5);
}

String descripcionPines() {
  String r = "DIN=D"; r += dDin; r += " CLK=D"; r += dClk; r += " CS=D"; r += dCs;
  r += spiChip ? F(" \xC2\xB7 SPI") : F(" \xC2\xB7 manual");
  if (lento) r += F(" \xC2\xB7 lento");
  return r;
}

void iniciar() {
  if (spiAbierto) { SPI.end(); spiAbierto = false; }
  if (spiChip) {
    SPI.begin();
    // Velocidad baja a propósito: con señales de 3,3 V y cables dupont, cada
    // flanco tiene tiempo de llegar limpio. 500 kHz (lento: 125 kHz) sobra para
    // 30 imágenes por segundo: una imagen completa tarda 1,7 ms (lento: 7 ms).
    SPI.setFrequency(lento ? 125000 : 500000);
    SPI.setBitOrder(MSBFIRST);
    SPI.setDataMode(SPI_MODE0);
    spiAbierto = true;
  } else {
    pinMode(gDin, OUTPUT); digitalWrite(gDin, LOW);
    pinMode(gClk, OUTPUT); digitalWrite(gClk, LOW);
  }
  // CS se configura DESPUÉS de SPI.begin(): D6 (GPIO12) es la pata MISO del SPI
  // y SPI.begin() se la queda. Si se hace antes, CS no cambia nunca y la matriz
  // no recibe ninguna orden (pantalla apagada).
  pinMode(gCs, OUTPUT);
  digitalWrite(gCs, HIGH);
  Serial.printf("[pantalla] %s\n", descripcionPines().c_str());
  nivelBrillo = 1;
  pantallaEncendida = true;
  reconfigurar();
  limpiar();
  mostrar();
}

void orientar(uint8_t rotacion, bool invertirModulos, bool espejo) {
  rotacionModulo = rotacion & 3;
  ordenInvertido = invertirModulos;
  espejoModulo = espejo;
}

void pruebaLeds(int modulo) {
  if (modulo < 0) { iniciar(); mostrar(); return; }
  // Varias veces seguidas: si una orden llega mal, la siguiente la corrige
  for (uint8_t vez = 0; vez < 3; vez++) {
    digitalWrite(gCs, LOW);
    retardo();
    for (int m = NUM_MODULOS - 1; m >= 0; m--) {     // el primero enviado va al más lejano
      bool si = (modulo == 0) || (m == modulo - 1);
      byteSalida(REG_PRUEBA);
      byteSalida(si ? 1 : 0);
    }
    digitalWrite(gCs, HIGH);
    retardo();
  }
}

void pataFija(uint8_t cual) {
  if (spiAbierto) { SPI.end(); spiAbierto = false; }
  const uint8_t g[3] = {gDin, gClk, gCs};
  for (uint8_t i = 0; i < 3; i++) {
    pinMode(g[i], OUTPUT);
    digitalWrite(g[i], i == cual ? HIGH : LOW);
  }
}

void brillo(uint8_t nivel) { nivelBrillo = nivel > 15 ? 15 : nivel; enviarTodos(REG_INTENSIDAD, nivelBrillo); }
void encender(bool si)      { pantallaEncendida = si; enviarTodos(REG_APAGADO, si ? 1 : 0); }

// Vuelve a enviar toda la configuración del MAX7219. Con señales de 3,3 V una
// orden puede llegar mal (calor, ruido, bajones al transmitir el WiFi) y dejar un
// módulo desconfigurado: modo decodificar, menos filas, modo prueba... Repetirla
// cada segundo hace que cualquier fallo se corrija solo, sin que se note.
void reconfigurar() {
  enviarTodos(REG_PRUEBA, 0);
  enviarTodos(REG_DECODIFICAR, 0);
  enviarTodos(REG_LIMITE, 7);
  enviarTodos(REG_INTENSIDAD, nivelBrillo);
  enviarTodos(REG_APAGADO, pantallaEncendida ? 1 : 0);
  ultimaConfig = millis();
}

void limpiar() { memset(imagen, 0, sizeof(imagen)); }

void pixel(int x, int y, bool encendido) {
  if (x < 0 || x >= ANCHO_PANTALLA || y < 0 || y >= ALTO_PANTALLA) return;
  if (encendido) imagen[x] |= (1 << y);
  else           imagen[x] &= ~(1 << y);
}

bool leer(int x, int y) {
  if (x < 0 || x >= ANCHO_PANTALLA || y < 0 || y >= ALTO_PANTALLA) return false;
  return imagen[x] & (1 << y);
}

void invertir() { for (auto& c : imagen) c = ~c; }

void guardar(uint8_t* copia)          { memcpy(copia, imagen, sizeof(imagen)); }
void restaurar(const uint8_t* copia)  { memcpy(imagen, copia, sizeof(imagen)); }

void mostrar() {
  // La configuración viaja con CADA imagen: si un bit llegó mal y desconfiguró
  // un módulo (decodificar, número de filas, modo prueba, apagado...), se corrige
  // en la imagen siguiente, antes de que se note. Son 5 órdenes más por imagen.
  reconfigurar();
  // salida[m][r]: byte del registro r (fila 0..7) del módulo m de la cadena
  uint8_t salida[NUM_MODULOS][8];
  memset(salida, 0, sizeof(salida));
  for (int x = 0; x < ANCHO_PANTALLA; x++) {
    for (int y = 0; y < 8; y++) {
      if (!(imagen[x] & (1 << y))) continue;
      int modulo = x / 8;
      int lx = x % 8, ly = y;
      if (espejoModulo) lx = 7 - lx;
      int u, v;
      switch (rotacionModulo) {
        case 1:  u = ly;     v = 7 - lx; break;
        case 2:  u = 7 - lx; v = 7 - ly; break;
        case 3:  u = 7 - ly; v = lx;     break;
        default: u = lx;     v = ly;     break;
      }
      int cadena = ordenInvertido ? (NUM_MODULOS - 1 - modulo) : modulo;
      salida[cadena][v] |= (0x80 >> u);
    }
  }
  // Lo primero que se envía acaba en el módulo más alejado de la entrada
  for (uint8_t r = 0; r < 8; r++) {
    digitalWrite(gCs, LOW);
    retardo();
    for (int m = NUM_MODULOS - 1; m >= 0; m--) {
      byteSalida(r + 1);
      byteSalida(salida[m][r]);
    }
    digitalWrite(gCs, HIGH);
    retardo();
  }
}

void sprite(const Sprite& s, int x, int y, bool espejoH) {
  for (uint8_t i = 0; i < s.ancho; i++) {
    uint16_t col = pgm_read_word(&s.columnas[espejoH ? (s.ancho - 1 - i) : i]);
    for (uint8_t f = 0; f < s.alto; f++)
      if (col & (1 << f)) pixel(x + i, y + f, true);
  }
}

void lineaH(int x0, int x1, int y, bool encendido) {
  for (int x = x0; x <= x1; x++) pixel(x, y, encendido);
}

void rectangulo(int x, int y, int ancho, int alto, bool relleno) {
  for (int i = 0; i < ancho; i++)
    for (int j = 0; j < alto; j++)
      if (relleno || i == 0 || j == 0 || i == ancho - 1 || j == alto - 1)
        pixel(x + i, y + j, true);
}

void mini(const uint8_t* letra, int x, int y) {
  for (uint8_t i = 0; i < 3; i++) {
    uint8_t col = pgm_read_byte(&letra[i]);
    for (uint8_t f = 0; f < 5; f++)
      if (col & (1 << f)) pixel(x + i, y + f, true);
  }
}

int anchoTexto(const char* t) {
  int ancho = 0;
  const char* p = t;
  while (*p) {
    ancho += anchoGlifo(siguienteCaracter(p));
    if (*p) ancho += 1;          // espacio entre letras
  }
  return ancho;
}

int texto(const char* t, int x, int y) {
  const char* p = t;
  while (*p) {
    x += dibujarGlifo(siguienteCaracter(p), x, y);
    if (*p) x += 1;
  }
  return x;
}

int digito(uint8_t d, int x, int y) {
  // Las cifras de la fuente ocupan las filas 1..7: se suben una fila
  return dibujarGlifo('0' + (d % 10), x, y - 1);
}

void cortar(bool si) { cortarYa = si; }
bool cortado()        { return cortarYa; }

void desplazar(const char* t, uint16_t msPorColumna) {
  int ancho = anchoTexto(t);
  for (int x = ANCHO_PANTALLA; x >= -ancho && !cortarYa; x--) {
    limpiar();
    texto(t, x, 0);
    mostrar();
    esperar(msPorColumna);
  }
}

void cortinaSalida(uint16_t ms) {
  for (int x = 0; x < ANCHO_PANTALLA; x++) {
    imagen[x] = 0;
    if (x + 1 < ANCHO_PANTALLA) imagen[x + 1] = 0xFF;
    mostrar();
    esperar(ms);
  }
  limpiar();
  mostrar();
}

} // namespace Pantalla
