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
#include "sonido.h"

namespace {
  int      ultimoMinuto = -1;
  uint32_t proximaAventura = 0;     // millis() de la próxima aventura (0 = nunca)
  int      estadoPantalla = -1;     // -1 = desconocido, 0 = apagada, 1..16 = brillo + 1
  bool     enDiagnostico = false;   // la pantalla está en modo diagnóstico
  uint32_t finDiagnostico = 0;

  const char* NOMBRE_PATA[3] = {"DIN", "CLK", "CS"};

  // ---------------------------------------------------------- concentración
  bool     concManual = false;      // puesta desde la web
  bool     concSinFin = false;
  uint32_t concHasta = 0;           // millis() del final
  bool     horarioCancelado = false;// se quitó a mano durante el horario
  bool     concAntes = false;       // para notar cuándo termina
  bool     concParadaAMano = false;

  bool diaMarcado(uint8_t dias, const struct tm& t) { return (dias >> t.tm_wday) & 1; }

  bool dentroDe(int iniH, int iniM, int finH, int finM, const struct tm& t) {
    int ahora = t.tm_hour * 60 + t.tm_min, ini = iniH * 60 + iniM, fin = finH * 60 + finM;
    if (ini == fin) return false;
    if (ini < fin) return ahora >= ini && ahora < fin;
    return ahora >= ini || ahora < fin;
  }

  bool enHorarioEstudio() {
    if (!ajustes.estudioActivo) return false;
    struct tm t;
    if (!Tiempo::ahora(t)) return false;
    return diaMarcado(ajustes.estudioDias, t) &&
           dentroDe(ajustes.estudioIniH, ajustes.estudioIniM, ajustes.estudioFinH, ajustes.estudioFinM, t);
  }

  // ---------------------------------------------------------- alarma
  bool revisarAlarma(const struct tm& t);

  // Órdenes de diagnóstico que llegan desde la página web
  void atenderDiagnostico() {
    int8_t d = peticiones.diagnostico;
    if (d < 0) {
      if (enDiagnostico && (int32_t)(millis() - finDiagnostico) >= 0) d = 99;   // se cierra solo
      else return;
    }
    peticiones.diagnostico = -1;
    if (d == 99) {
      Pantalla::pruebaLeds(-1);
      enDiagnostico = false;
      estadoPantalla = -1;
      estadoDiagnostico = "";
      Serial.println(F("[diagnostico] Fin"));
      return;
    }
    if (!enDiagnostico) Pantalla::iniciar();        // vuelve a configurar la matriz desde cero
    enDiagnostico = true;
    finDiagnostico = millis() + MINUTOS_DIAGNOSTICO * 60000UL;
    if (d == 0) {
      Pantalla::pruebaLeds(0);
      estadoDiagnostico = tr("Todos los LED encendidos", "All LEDs on");
    } else if (d >= 1 && d <= 4) {
      Pantalla::pruebaLeds(d);
      estadoDiagnostico = String(tr("Encendido solo el módulo ", "Only module ")) + d + tr(" (1 = el más cercano a los cables)", " on (1 = nearest the wires)");
    } else if (d >= 10 && d <= 12) {
      Pantalla::pataFija(d - 10);
      const char* p = NOMBRE_PATA[d - 10];
      estadoDiagnostico = String(p) + tr(" a 3,3 V; las otras dos a 0 V", " at 3.3 V; the other two at 0 V");
    }
    Serial.printf("[diagnostico] %s\n", estadoDiagnostico.c_str());
  }

  // Aplica orientación, zona horaria, etc. tras guardar ajustes
  void aplicarAjustes() {
    Sonido::iniciar();
    Pantalla::pines(ajustes.pinDin, ajustes.pinClk, ajustes.pinCs, ajustes.spiLento);
    Pantalla::iniciar();
    enDiagnostico = false;
    estadoDiagnostico = "";
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
      bool corta = (peticiones.prueba == 2);
      peticiones.prueba = 0;
      despertar();
      Anim::patronPrueba(corta);
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
    // Mientras dura un mensaje puede llegar otro: se atiende en cuanto acaba
    while (peticiones.mensaje.length()) {
      String   m   = peticiones.mensaje;
      int16_t  in  = peticiones.mensajeIntro;
      uint16_t seg = peticiones.mensajeSegundos;
      uint8_t  vel = peticiones.mensajeVelocidad;
      uint8_t  snd = peticiones.mensajeSonido;
      peticiones.mensaje = "";
      peticiones.mensajeIntro = 1; peticiones.mensajeSegundos = 0; peticiones.mensajeVelocidad = 0;
      peticiones.mensajeSonido = 0;
      despertar();
      if (snd) Sonido::tocar(snd - 1);               // aviso con melodía (solo si se pide)
      Anim::mensajeEspecial(m.c_str(), in, seg, vel);
    }
  }

  bool revisarAlarma(const struct tm& t) {
    if (!ajustes.alarmaActiva || t.tm_hour != ajustes.alarmaH || t.tm_min != ajustes.alarmaM) return false;
    if (!diaMarcado(ajustes.alarmaDias, t)) return false;
    Serial.println(F("[alarma] ¡Arriba!"));
    despertar();
    Sonido::tocar(ajustes.alarmaMelodia, ajustes.alarmaSegundos);
    const char* texto = ajustes.alarmaTexto[0] ? ajustes.alarmaTexto : tr("¡BUENOS DÍAS, {N}! \x03", "GOOD MORNING, {N}! \x03");
    Anim::mensajeEspecial(texto, 1, ajustes.alarmaSegundos, 0);
    Sonido::parar();
    if (Tiempo::esCumple(t)) Anim::cumple();
    return true;
  }

  // Cosas que pasan al empezar cada minuto
  void eventosDelMinuto(const struct tm& t, bool noche) {
    if (revisarAlarma(t)) return;                  // la alarma va primero
    if (Concentracion::activa()) return;           // concentración: nada que distraiga

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
String estadoDiagnostico;

namespace Concentracion {
  void empezar(int minutos) {
    concManual = true;
    concSinFin = (minutos < 0);
    concHasta = millis() + (uint32_t)(minutos > 0 ? minutos : 0) * 60000UL;
    horarioCancelado = false;
    concParadaAMano = false;
  }
  void terminar() {
    concManual = false;
    if (enHorarioEstudio()) horarioCancelado = true;
    concParadaAMano = true;
  }
  bool porHorario() { return !concManual && enHorarioEstudio() && !horarioCancelado; }
  bool activa() {
    if (concManual && !concSinFin && (int32_t)(millis() - concHasta) >= 0) concManual = false;
    if (!enHorarioEstudio()) horarioCancelado = false;
    return concManual || (enHorarioEstudio() && !horarioCancelado);
  }
  int32_t segundosQuedan() {
    if (!activa()) return 0;
    if (concManual) return concSinFin ? -1 : (int32_t)((concHasta - millis()) / 1000);
    // por horario: hasta el final del horario de hoy
    struct tm t;
    if (!Tiempo::ahora(t)) return -1;
    int ahora = t.tm_hour * 3600 + t.tm_min * 60 + t.tm_sec;
    int fin = ajustes.estudioFinH * 3600 + ajustes.estudioFinM * 60;
    int q = fin - ahora; if (q < 0) q += 86400;
    return q;
  }
}

namespace Principal {

void iniciar() {
  Serial.begin(115200);
  Serial.println(F("\n\n=== RELOJ DE HYRULE v" VERSION_RELOJ " ==="));
  randomSeed(ESP.random());

  Config::cargar();
  Pantalla::pines(ajustes.pinDin, ajustes.pinClk, ajustes.pinCs, ajustes.spiLento);
  Pantalla::iniciar();
  Pantalla::orientar(ajustes.rotacion, ajustes.invertirModulos, ajustes.espejo);
  Pantalla::brillo(max(ajustes.brillo, (uint8_t)1));
  Sonido::iniciar();

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

  atenderDiagnostico();
  if (enDiagnostico) {          // la pantalla es para el diagnóstico: no se dibuja nada
    peticiones.prueba = 0;
    peticiones.animacion = -1;
    peticiones.mensaje = "";
    if (peticiones.ajustesCambiados) atenderPeticiones(t, hayHora);
    esperar(20);
    return;
  }
  atenderPeticiones(t, hayHora);

  // Sin hora todavía: explicar cómo configurar o esperar a internet
  if (!hayHora) {
    ajustarBrillo(false);
    if (Red::modoPortal()) Anim::instruccionesWifi();
    else Anim::buscandoHora();
    return;
  }

  ajustarBrillo(noche);

  // ¿Acaba de terminar la concentración? Aviso suave (si se quiere)
  bool conc = Concentracion::activa();
  if (concAntes && !conc) {
    if (ajustes.estudioAviso && !concParadaAMano) {
      despertar();
      if (Sonido::disponible()) Sonido::tocar(3);             // Victoria
      Anim::mensajeEspecial(tr("¡BUEN TRABAJO, {N}! ¡A DESCANSAR! \x03",
                               "WELL DONE, {N}! TIME FOR A BREAK! \x03"), 1, 0, 0);
    }
    concParadaAMano = false;
    programarAventura();
  }
  if (!concAntes && conc) concParadaAMano = false;
  concAntes = conc;

  if (t.tm_min != ultimoMinuto) {
    bool primeraVez = (ultimoMinuto < 0);
    ultimoMinuto = t.tm_min;
    if (!primeraVez) eventosDelMinuto(t, noche);
  }

  // Aventura al azar (nunca de noche ni en concentración)
  if (Concentracion::activa()) {
    if (proximaAventura && (int32_t)(millis() - proximaAventura) >= 0) programarAventura();
  } else if (!noche && proximaAventura && (int32_t)(millis() - proximaAventura) >= 0) {
    Anim::alAzar(t);
    programarAventura();
  }

  // Dibujar la hora (se vuelve a leer por si una animación ha tardado)
  Tiempo::ahora(t);
  noche = Tiempo::esNoche(t);
  ajustarBrillo(noche);
  Pantalla::limpiar();
  if (Concentracion::activa())
    Esfera::hora(t, 0, true);                       // solo la hora, quieta
  else if (ajustes.modoFecha == FECHA_ALTERNAR && t.tm_sec >= 30 && t.tm_sec < 34)
    Esfera::fechaCorta(t);
  else
    Esfera::hora(t, milisegundosDelSegundo());
  Pantalla::mostrar();
  esperar(40);
}

} // namespace Principal
