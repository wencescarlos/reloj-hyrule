// ===========================================================================
//  principal.cpp  -  Lo que hace el reloj en cada momento (arranque y bucle)
//  Reloj de Hyrule · Proyecto de fans SIN ÁNIMO DE LUCRO · No oficial
//  CC BY-NC-SA 4.0 · https://github.com/wencescarlos/reloj-hyrule
// ===========================================================================
#include <ESP8266WiFi.h>
#include <sys/time.h>
#include "configuracion.h"
#include "ajustes.h"
#include "pantalla.h"
#include "tiempo.h"
#include "esfera.h"
#include "animaciones.h"
#include "red.h"
#include "actualizacion.h"
#include "sistema.h"
#include "principal.h"

namespace {
  int      ultimoMinuto = -1;
  uint32_t proximaAventura = 0;     // millis() de la próxima aventura (0 = nunca)
  int      estadoPantalla = -1;     // -1 = desconocido, 0 = apagada, 1..16 = brillo + 1

  // Aplica orientación, zona horaria, etc. tras guardar ajustes
  void aplicarAjustes() {
    Pantalla::orientar(ajustes.rotacion, ajustes.invertirModulos, ajustes.espejo);
    Tiempo::iniciar();
    estadoPantalla = -1;
  }

  // Brillo según sea de día o de noche (solo envía cambios)
  void ajustarBrillo(bool noche) {
    int objetivo;
    if (noche && ajustes.nocheApagar) objetivo = 0;
    else objetivo = (noche ? ajustes.brilloNoche : ajustes.brillo) + 1;
    if (objetivo == estadoPantalla) return;
    estadoPantalla = objetivo;
    if (objetivo == 0) {
      Pantalla::encender(false);
    } else {
      Pantalla::brillo(objetivo - 1);
      Pantalla::encender(true);
    }
  }

  // Para enseñar algo aunque sea de noche (lo pide la web o es un saludo)
  void despertar() {
    Pantalla::encender(true);
    Pantalla::brillo(max(ajustes.brillo, (uint8_t)1));
    estadoPantalla = -1;
  }

  // La próxima aventura llega a los X minutos, con un poco de azar (±30 %)
  void programarAventura() {
    if (ajustes.animCadaMin == 0) { proximaAventura = 0; return; }
    uint32_t base = ajustes.animCadaMin * 60000UL;
    proximaAventura = millis() + base * (70 + random(61)) / 100;
    if (proximaAventura == 0) proximaAventura = 1;
  }

  uint16_t milisegundosDelSegundo() {
    struct timeval tv;
    gettimeofday(&tv, nullptr);
    return tv.tv_usec / 1000;
  }

  // Atiende lo que se ha pedido desde la página web
  void atenderPeticiones(const struct tm& t, bool hayHora) {
    if (peticiones.ajustesCambiados) {
      peticiones.ajustesCambiados = false;
      aplicarAjustes();
      programarAventura();
    }
    if (peticiones.buscarActualizacion) {
      peticiones.buscarActualizacion = false;
      Actualizacion::comprobar();
    }
    if (peticiones.prueba) {
      peticiones.prueba = false;
      despertar();
      Anim::patronPrueba();
    }
    if (peticiones.animacion >= 0) {
      int n = peticiones.animacion;
      peticiones.animacion = -1;
      despertar();
      if (n == 255) {
        if (hayHora) Anim::alAzar(t);
        else Anim::lanzar(random(Anim::total()));
      } else {
        Anim::lanzar(n);
      }
    }
    if (peticiones.mensaje.length()) {
      String m = peticiones.mensaje;
      peticiones.mensaje = "";
      despertar();
      Anim::mensaje(m.c_str());
    }
  }

  // Cosas que pasan al empezar cada minuto
  void eventosDelMinuto(const struct tm& t, bool noche) {
    int ahora  = t.tm_hour * 60 + t.tm_min;
    int inicio = ajustes.nocheIniH * 60 + ajustes.nocheIniM;
    int fin    = ajustes.nocheFinH * 60 + ajustes.nocheFinM;
    bool saludos = ajustes.saludos && ajustes.nocheActiva;

    if (saludos && ahora == fin) {                 // ¡buenos días!
      despertar();
      Anim::buenosDias(t);
      if (Tiempo::esCumple(t)) Anim::cumple();
      return;
    }
    if (saludos && ahora == inicio) {              // ¡buenas noches!
      despertar();
      Anim::buenasNoches();
      return;
    }
    if (noche) return;                             // de noche, silencio

    if (Tiempo::esCumple(t) && (t.tm_min == 0 || t.tm_min == 30)) {
      Anim::cumple();                              // el día del cumple, ¡fiesta!
    } else if (t.tm_min == 0 && ajustes.horaEnPunto) {
      Anim::horaEnPunto(t);
    } else if (ajustes.modoFecha == FECHA_DESPLAZAR && ajustes.fechaCadaMin &&
               ahora % ajustes.fechaCadaMin == 0) {
      Anim::fecha(t);
    }
  }
}

// ---------------------------------------------------------------------------
namespace Principal {

void iniciar() {
  Serial.begin(115200);
  Serial.println(F("\n\n=== RELOJ DE HYRULE v" VERSION_RELOJ " ==="));
  randomSeed(ESP.random());

  Config::cargar();
  Pantalla::iniciar();
  Pantalla::orientar(ajustes.rotacion, ajustes.invertirModulos, ajustes.espejo);
  Pantalla::brillo(max(ajustes.brillo, (uint8_t)1));

  Anim::arranque();
  Red::iniciar();
  Tiempo::iniciar();

  if (Red::conectado()) {
    String aviso = String(tr("CONECTADO \x04 WEB: ", "CONNECTED \x04 WEB: ")) + Red::ip();
    Pantalla::desplazar(aviso.c_str(), ajustes.velocidad);
  }
  programarAventura();
}

void bucle() {
  Red::servicio();
  Actualizacion::servicio();

  struct tm t;
  bool hayHora = Tiempo::ahora(t);
  bool noche = hayHora && Tiempo::esNoche(t);

  atenderPeticiones(t, hayHora);

  // Sin hora todavía: explicar cómo configurar o esperar a internet
  if (!hayHora) {
    ajustarBrillo(false);
    if (Red::modoPortal()) Anim::instruccionesWifi();
    else Anim::buscandoHora();
    return;
  }

  ajustarBrillo(noche);

  if (t.tm_min != ultimoMinuto) {
    bool primeraVez = (ultimoMinuto < 0);
    ultimoMinuto = t.tm_min;
    if (!primeraVez) eventosDelMinuto(t, noche);
  }

  // Aventura al azar (nunca de noche)
  if (!noche && proximaAventura && (int32_t)(millis() - proximaAventura) >= 0) {
    Anim::alAzar(t);
    programarAventura();
  }

  // Dibujar la hora (se vuelve a leer por si una animación ha tardado)
  Tiempo::ahora(t);
  noche = Tiempo::esNoche(t);
  ajustarBrillo(noche);
  Pantalla::limpiar();
  if (ajustes.modoFecha == FECHA_ALTERNAR && t.tm_sec >= 30 && t.tm_sec < 34)
    Esfera::fechaCorta(t);
  else
    Esfera::hora(t, milisegundosDelSegundo());
  Pantalla::mostrar();
  esperar(40);
}

} // namespace Principal
