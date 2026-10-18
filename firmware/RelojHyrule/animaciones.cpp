// ===========================================================================
//  animaciones.cpp  -  Las aventuras del héroe (dibujos y guiones propios)
//  Reloj de Hyrule · Proyecto de fans SIN ÁNIMO DE LUCRO · No oficial
//  CC BY-NC-SA 4.0 · https://github.com/wencescarlos/reloj-hyrule
//  Homenaje sin ánimo de lucro a The Legend of Zelda.
// ===========================================================================
#include "animaciones.h"
#include "pantalla.h"
#include "sprites.h"
#include "frases.h"
#include "ajustes.h"
#include "tiempo.h"
#include "sistema.h"
#include "configuracion.h"

using namespace Pantalla;
using Anim::personalizar;

namespace {

  // Muestra el cuadro actual y espera
  void cuadro(uint16_t ms) { mostrar(); esperar(ms); }

  void decir(const String& t) { desplazar(t.c_str(), ajustes.velocidad); }

  // Destellos al azar dentro de un rectángulo
  void destellos(int x0, int y0, int x1, int y1, uint8_t cuantos) {
    for (uint8_t i = 0; i < cuantos; i++)
      pixel(random(x0, x1 + 1), random(y0, y1 + 1));
  }

  // El héroe caminando de x0 a x1 (paso = +1 o -1)
  void caminar(int x0, int x1, uint16_t ms, bool haciaIzquierda = false) {
    int paso = x1 >= x0 ? 1 : -1;
    for (int x = x0; x != x1 + paso; x += paso) {
      limpiar();
      sprite(((x / 2) % 2) ? SPR_HEROE_A : SPR_HEROE_B, x, 0, haciaIzquierda);
      cuadro(ms);
    }
  }

  // ------------------------------------------------------------------
  //  1. Paseo por Hyrule
  // ------------------------------------------------------------------
  void paseo() {
    caminar(-8, ANCHO_PANTALLA, 70);
  }

  // ------------------------------------------------------------------
  //  2. El cofre del tesoro
  // ------------------------------------------------------------------
  void cofre() {
    for (int x = -8; x <= 9; x++) {
      limpiar();
      sprite(SPR_COFRE, 21, 1);
      sprite(((x / 2) % 2) ? SPR_HEROE_A : SPR_HEROE_B, x, 0);
      cuadro(80);
    }
    esperar(400);
    // El cofre tiembla un poco...
    for (uint8_t i = 0; i < 6; i++) {
      limpiar();
      sprite(SPR_HEROE_B, 9, 0);
      sprite(SPR_COFRE, 21 + (i % 2), 1);
      cuadro(90);
    }
    // ...y se abre con destellos
    for (uint8_t i = 0; i < 10; i++) {
      limpiar();
      sprite(SPR_HEROE_B, 9, 0);
      sprite(SPR_COFRE_ABIERTO, 21, 1);
      destellos(20, 0, 30, 1, 2);
      cuadro(80);
    }
    // ¡Tesoro conseguido! Pose de héroe con el tesoro brillando
    for (uint8_t i = 0; i < 16; i++) {
      limpiar();
      sprite(SPR_HEROE_TESORO, 6, 0);
      if (i % 4 != 3) sprite(SPR_RUPIA, 18, 0);
      destellos(14, 0, 27, 7, 3);
      cuadro(110);
    }
    decir(TESOROS[random(NUM_TESOROS)]);
  }

  // ------------------------------------------------------------------
  //  3. Combate contra un monstruo
  // ------------------------------------------------------------------
  void combate() {
    // Entran los dos
    for (int i = 0; i <= 16; i++) {
      limpiar();
      int hx = -8 + i;               // héroe: -8 -> 8
      int mx = 32 - (i * 12) / 16;   // monstruo: 32 -> 20
      sprite(((i / 2) % 2) ? SPR_HEROE_A : SPR_HEROE_B, hx, 0);
      sprite((i % 2) ? SPR_MONSTRUO_A : SPR_MONSTRUO_B, mx, 1);
      cuadro(90);
    }
    // ¡Espadazo!
    for (uint8_t golpe = 0; golpe < 2; golpe++) {
      limpiar();
      sprite(SPR_HEROE_ATAQUE, 8, 0);
      sprite(SPR_MONSTRUO_A, 21, 1);
      invertir();
      cuadro(60);
      limpiar();
      sprite(SPR_HEROE_ATAQUE, 8, 0);
      sprite(SPR_MONSTRUO_B, 22, 1);
      cuadro(160);
      limpiar();
      sprite(SPR_HEROE_A, 8, 0);
      sprite(SPR_MONSTRUO_A, 21, 1);
      cuadro(200);
    }
    // El monstruo parpadea y desaparece en una nube de humo
    for (uint8_t i = 0; i < 6; i++) {
      limpiar();
      sprite(SPR_HEROE_A, 8, 0);
      if (i % 2) sprite(SPR_MONSTRUO_A, 21, 1);
      cuadro(110);
    }
    const Sprite* humo[] = { &SPR_HUMO_1, &SPR_HUMO_2, &SPR_HUMO_1 };
    for (auto h : humo) {
      limpiar();
      sprite(SPR_HEROE_A, 8, 0);
      sprite(*h, 21, 0);
      cuadro(150);
    }
    // Deja caer un corazón que late
    for (uint8_t i = 0; i < 10; i++) {
      limpiar();
      sprite(SPR_HEROE_A, 8, 0);
      if (i % 3 != 2) sprite(SPR_CORAZON, 21, 1);
      cuadro(120);
    }
    // El héroe lo recoge
    for (int x = 8; x <= 15; x++) {
      limpiar();
      sprite(SPR_CORAZON, 21, 1);
      sprite(((x / 2) % 2) ? SPR_HEROE_A : SPR_HEROE_B, x, 0);
      cuadro(80);
    }
    decir(personalizar("¡MONSTRUO DERROTADO! {N} RECUPERA UN \x01"));
  }

  // ------------------------------------------------------------------
  //  4. La Trifuerza se une
  // ------------------------------------------------------------------
  void dibujarTrifuerza(int arribaY, int izqX, int derX) {
    sprite(SPR_TRIANGULO, 12, arribaY);
    sprite(SPR_TRIANGULO, izqX, 4);
    sprite(SPR_TRIANGULO, derX, 4);
  }

  void trifuerza() {
    // Cae el triángulo de arriba
    for (int y = -4; y <= 0; y++) { limpiar(); sprite(SPR_TRIANGULO, 12, y); cuadro(140); }
    // Llegan los otros dos desde los lados
    for (int i = 0; i <= 15; i++) {
      limpiar();
      int izq = -7 + (i * 15) / 15;  // -7 -> 8
      int der = 32 - (i * 16) / 15;  // 32 -> 16
      dibujarTrifuerza(0, izq, der);
      cuadro(70);
    }
    // Brilla
    for (uint8_t i = 0; i < 6; i++) {
      limpiar(); dibujarTrifuerza(0, 8, 16);
      if (i % 2) invertir();
      cuadro(150);
    }
    for (uint8_t i = 0; i < 14; i++) {
      limpiar(); dibujarTrifuerza(0, 8, 16);
      destellos(0, 0, 6, 7, 2);
      destellos(25, 0, 31, 7, 2);
      cuadro(100);
    }
    decir(personalizar("¡{N} HA REUNIDO LA TRIFUERZA! \x03"));
  }

  // ------------------------------------------------------------------
  //  5. El hada
  // ------------------------------------------------------------------
  void hada() {
    const uint8_t ESTELA = 6;
    int ex[ESTELA], ey[ESTELA];
    for (uint8_t i = 0; i < ESTELA; i++) { ex[i] = -10; ey[i] = 0; }
    for (int x = -5; x <= ANCHO_PANTALLA + 8; x++) {
      int y = 2 + (int)(2.0f * sinf(x / 2.5f));
      for (uint8_t i = ESTELA - 1; i > 0; i--) { ex[i] = ex[i - 1]; ey[i] = ey[i - 1]; }
      ex[0] = x + 2; ey[0] = y + 2;
      limpiar();
      for (uint8_t i = 2; i < ESTELA; i++)
        if (random(3)) pixel(ex[i] - i, ey[i] + random(-1, 2));
      sprite((x % 2) ? SPR_HADA_A : SPR_HADA_B, x, y);
      cuadro(75);
    }
    decir(personalizar("¡ESCUCHA! ¡ESCUCHA! "));
    decir(personalizar(FRASES[random(NUM_FRASES)]));
  }

  // ------------------------------------------------------------------
  //  6. Contenedores de corazón
  // ------------------------------------------------------------------
  void corazones() {
    bool lleno[4] = { false, false, false, false };
    auto pintar = [&]() {
      limpiar();
      for (uint8_t i = 0; i < 4; i++)
        sprite(lleno[i] ? SPR_CORAZON : SPR_CORAZON_VACIO, i * 8, 1);
    };
    pintar(); cuadro(500);
    for (uint8_t c = 0; c < 4; c++) {
      for (uint8_t p = 0; p < 6; p++) {
        lleno[c] = p % 2;
        pintar(); cuadro(110);
      }
      lleno[c] = true;
      pintar(); cuadro(250);
    }
    for (uint8_t i = 0; i < 4; i++) {           // latido final
      pintar(); if (i % 2) invertir();
      cuadro(180);
    }
    decir(personalizar("¡VIDA LLENA! {N} ESTÁ LISTO PARA LA AVENTURA \x01"));
  }

  // ------------------------------------------------------------------
  //  7. ¡Bomba!
  // ------------------------------------------------------------------
  void bomba() {
    // La mecha chisporrotea
    for (uint8_t i = 0; i < 24; i++) {
      limpiar();
      bool visible = i < 14 || (i % 2);        // al final parpadea deprisa
      if (visible) sprite(SPR_BOMBA, 12, 0);
      if (random(2)) pixel(19 + random(0, 2), random(0, 2));
      cuadro(i < 14 ? 110 : 60);
    }
    // ¡BUM! anillos que crecen
    for (int r = 0; r < 20; r++) {
      limpiar();
      for (int x = 0; x < ANCHO_PANTALLA; x++)
        for (int y = 0; y < 8; y++) {
          float d = sqrtf((x - 15.5f) * (x - 15.5f) + (y - 3.5f) * (y - 3.5f) * 4.0f);
          if (fabsf(d - r) < 1.0f || fabsf(d - r * 0.6f) < 0.6f) pixel(x, y);
        }
      if (r == 2) invertir();
      cuadro(40);
    }
    // Restos que se apagan
    for (uint8_t i = 0; i < 10; i++) {
      limpiar(); destellos(0, 0, 31, 7, 14 - i);
      cuadro(70);
    }
    limpiar();
    texto("¡BUM!", (ANCHO_PANTALLA - anchoTexto("¡BUM!")) / 2, 0);
    cuadro(900);
  }

  // ------------------------------------------------------------------
  //  8. Lluvia de rupias
  // ------------------------------------------------------------------
  void lluviaRupias() {
    const uint8_t N = 9;
    int rx[N], ry[N];
    for (uint8_t i = 0; i < N; i++) { rx[i] = random(0, 29); ry[i] = -4 - random(0, 24); }
    for (uint8_t f = 0; f < 60; f++) {
      limpiar();
      for (uint8_t i = 0; i < N; i++) {
        ry[i]++;
        if (ry[i] > 8 && f < 40) { ry[i] = -4 - random(0, 6); rx[i] = random(0, 29); }
        sprite(SPR_RUPIA_MINI, rx[i], ry[i]);
      }
      cuadro(70);
    }
    decir(personalizar("¡LLUVIA DE RUPIAS! {N} ES EL HÉROE MÁS RICO DE HYRULE \x02"));
  }

  // ------------------------------------------------------------------
  //  9. La espada legendaria
  // ------------------------------------------------------------------
  void espada() {
    for (int y = 8; y >= -5; y--) {
      limpiar();
      sprite(SPR_ESPADA, 13, y);
      destellos(6, 0, 11, 7, 1);
      destellos(20, 0, 25, 7, 1);
      cuadro(140);
    }
    for (uint8_t i = 0; i < 8; i++) {
      limpiar();
      sprite(SPR_ESPADA, 13, -5);
      destellos(4, 0, 27, 7, 4);
      if (i == 2 || i == 4) invertir();
      cuadro(110);
    }
    decir(personalizar("¡{N} HA CONSEGUIDO LA ESPADA LEGENDARIA!"));
  }

  // ------------------------------------------------------------------
  //  10. Persecución
  // ------------------------------------------------------------------
  void persecucion() {
    // El monstruo persigue al héroe...
    for (int x = -20; x <= ANCHO_PANTALLA + 2; x++) {
      limpiar();
      sprite((x % 2) ? SPR_MONSTRUO_A : SPR_MONSTRUO_B, x, 1);
      sprite(((x / 2) % 2) ? SPR_HEROE_A : SPR_HEROE_B, x + 12, 0);
      cuadro(55);
    }
    esperar(500);
    // ...y vuelve el héroe con la espada, ¡ahora huye el monstruo!
    for (int x = ANCHO_PANTALLA + 2; x >= -24; x--) {
      limpiar();
      sprite((x % 2) ? SPR_MONSTRUO_A : SPR_MONSTRUO_B, x, 1, true);
      sprite((x % 4 < 2) ? SPR_HEROE_ATAQUE : SPR_HEROE_A, x + 11, 0, true);
      cuadro(55);
    }
    decir(personalizar("¡QUÉ VALIENTE ES {N}!"));
  }

  // ------------------------------------------------------------------
  //  11. Frase de ánimo
  // ------------------------------------------------------------------
  void frase() {
    caminar(-8, 2, 70);
    esperar(300);
    decir(personalizar(FRASES[random(NUM_FRASES)]));
  }

  // ------------------------------------------------------------------
  struct Aventura { const char* nombre; void (*funcion)(); };
  const Aventura AVENTURAS[] = {
    { "Paseo por Hyrule",       paseo },
    { "El cofre del tesoro",    cofre },
    { "Combate contra monstruo",combate },
    { "La Trifuerza",           trifuerza },
    { "El hada",                hada },
    { "Contenedores de corazón",corazones },
    { "¡Bomba!",                bomba },
    { "Lluvia de rupias",       lluviaRupias },
    { "La espada legendaria",   espada },
    { "Persecución",            persecucion },
    { "Frase de ánimo",         frase },
  };
  const uint8_t NUM_AVENTURAS = sizeof(AVENTURAS) / sizeof(AVENTURAS[0]);
  int8_t ultima = -1;
}

namespace Anim {

String personalizar(const char* t) {
  String r = t;
  String nombre = ajustes.nombre[0] ? String(ajustes.nombre) : String("HÉROE");
  r.replace("{N}", nombre);
  return r;
}

uint8_t total() { return NUM_AVENTURAS; }

const char* nombre(uint8_t n) { return n < NUM_AVENTURAS ? AVENTURAS[n].nombre : ""; }

void lanzar(uint8_t n) {
  if (n >= NUM_AVENTURAS) return;
  ultima = n;
  AVENTURAS[n].funcion();
}

void alAzar(const struct tm& t) {
  // De vez en cuando, en lugar de una aventura: cuenta atrás del cumpleaños
  int dias = Tiempo::diasParaCumple(t);
  if (dias > 0 && dias <= 30 && random(5) == 0) {
    caminar(-8, 2, 70);
    String s = dias == 1 ? String("¡MAÑANA ES TU CUMPLEAÑOS, {N}! \x01")
                         : String("¡FALTAN ") + dias + " DÍAS PARA TU CUMPLEAÑOS, {N}! \x01";
    decir(personalizar(s.c_str()));
    return;
  }
  // Mensaje personalizado de papá / mamá
  if (ajustes.mensaje[0] && random(4) == 0) { mensaje(ajustes.mensaje); return; }

  uint8_t n;
  do {
    n = random(NUM_AVENTURAS);
    if (!ajustes.frases && n == NUM_AVENTURAS - 1) continue;
  } while ((int8_t)n == ultima || (!ajustes.frases && n == NUM_AVENTURAS - 1));
  lanzar(n);
}

void arranque() {
  // La Trifuerza se forma y aparece el título
  for (int y = -4; y <= 0; y++) { limpiar(); sprite(SPR_TRIANGULO, 12, y); cuadro(90); }
  for (int i = 0; i <= 15; i++) {
    limpiar(); dibujarTrifuerza(0, -7 + i, 32 - (i * 16) / 15); cuadro(45);
  }
  for (uint8_t i = 0; i < 4; i++) { limpiar(); dibujarTrifuerza(0, 8, 16); if (i % 2) invertir(); cuadro(150); }
  decir("RELOJ DE HYRULE \x03 v" VERSION_RELOJ);
}

void conectando(uint8_t paso) {
  limpiar();
  sprite((paso % 2) ? SPR_HEROE_A : SPR_HEROE_B, 0, 0);
  for (uint8_t i = 0; i < 3; i++)
    if (paso % 4 > i) rectangulo(12 + i * 6, 3, 2, 2, true);
  mostrar();
}

void instruccionesWifi() {
  decir("¡HOLA! CONECTA EL MÓVIL AL WIFI \"" WIFI_PROPIO_NOMBRE "\" "
        "CON LA CLAVE \"" WIFI_PROPIO_CLAVE "\" Y ABRE 192.168.4.1");
}

void buscandoHora() {
  caminar(-8, ANCHO_PANTALLA, 60);
  decir("BUSCANDO LA HORA EN INTERNET...");
}

void horaEnPunto(const struct tm& t) {
  for (uint8_t i = 0; i < 6; i++) {
    limpiar(); dibujarTrifuerza(0, 8, 16);
    if (i % 2) invertir();
    destellos(0, 0, 31, 7, 3);
    cuadro(160);
  }
  decir(Tiempo::horaHablada(t));
}

void buenosDias(const struct tm& t) {
  caminar(-8, 2, 70);
  String s = String("¡BUENOS DÍAS, {N}! HOY ES ") + Tiempo::fechaLarga(t) + ". ¡A POR LA AVENTURA! \x04";
  decir(personalizar(s.c_str()));
}

void buenasNoches() {
  for (uint8_t i = 0; i < 24; i++) {          // luna y estrellas que titilan
    limpiar();
    sprite(SPR_LUNA, 2, 0);
    destellos(10, 0, 31, 5, 3);
    cuadro(140);
  }
  decir(personalizar("¡BUENAS NOCHES, {N}! A DESCANSAR, QUE MAÑANA HAY MÁS AVENTURAS. ZZZ..."));
}

void cumple() {
  for (uint8_t i = 0; i < 30; i++) {          // la tarta con las velas
    limpiar();
    sprite(SPR_TARTA, 9, 0);
    for (uint8_t v = 0; v < 3; v++)           // las llamas titilan
      if (random(3)) pixel(11 + v * 4, 0, false);
    if (i > 12) { destellos(0, 0, 7, 7, 2); destellos(23, 0, 31, 7, 2); }
    cuadro(120);
  }
  // Fuegos artificiales de corazones
  for (uint8_t i = 0; i < 6; i++) {
    int x = random(0, 25);
    for (uint8_t f = 0; f < 3; f++) {
      limpiar();
      if (f < 2) pixel(x + 3, 7 - f * 3);
      else sprite(SPR_CORAZON, x, 1);
      cuadro(110);
    }
  }
  decir(personalizar("¡FELIZ CUMPLEAÑOS, {N}! \x01 ¡TODO HYRULE LO CELEBRA CONTIGO! \x03"));
}

void fecha(const struct tm& t) {
  decir(Tiempo::fechaLarga(t));
}

void mensaje(const char* t) {
  caminar(-8, 2, 70);
  esperar(250);
  decir(personalizar(t));
}

void patronPrueba() {
  // Marco + "1234": así se ve si los módulos están en orden y bien girados
  limpiar();
  for (uint8_t m = 0; m < NUM_MODULOS; m++) {
    rectangulo(m * 8, 0, 8, 8);
    digito(m + 1, m * 8 + 2, 1);
  }
  cuadro(2500);
  // Flecha que recorre de izquierda a derecha
  for (int x = 0; x < ANCHO_PANTALLA; x++) {
    limpiar();
    lineaH(0, x, 3); lineaH(0, x, 4);
    pixel(x + 1, 2); pixel(x + 1, 5); pixel(x + 2, 3); pixel(x + 2, 4);
    cuadro(40);
  }
  decir("PRUEBA DE PANTALLA OK");
}

void progreso(uint8_t porcentaje) {
  limpiar();
  texto("ACT", 0, 0);
  int largo = (porcentaje * 12) / 100;
  rectangulo(18, 1, 14, 6);
  if (largo > 0) rectangulo(19, 2, largo, 4, true);
  mostrar();
}

} // namespace Anim
