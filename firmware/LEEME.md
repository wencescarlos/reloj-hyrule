# Firmware del Reloj de Hyrule

> ⚠️ Proyecto de fans **sin ánimo de lucro** y no oficial. Código **original**, escrito desde cero. Licencia CC BY-NC-SA 4.0: prohibido el uso comercial.

Programa para el Wemos D1 mini (ESP8266) con 4 matrices MAX7219. El código está escrito en español, el reloj habla **español o inglés** (se elige en su página web) y **no necesita librerías externas**: solo usa lo que trae el núcleo ESP8266 para Arduino (3.1.2 o superior).

La instalación y la configuración se explican en el [README principal](../README.md).

**Compilación:** placa *LOLIN(WEMOS) D1 R2 & mini*, *Flash Size* 4MB (FS:1MB OTA:~1019KB) y **MMU "16KB cache + 48KB IRAM and 2nd Heap (shared)"**. Sin esa opción de memoria el reloj funciona, pero las actualizaciones por internet (HTTPS) no tienen memoria suficiente. `platformio.ini` y GitHub Actions ya la usan.

## Personalizarlo

- **Frases y tesoros:** edita `RelojHyrule/frases.h` (hay una lista en español y otra en inglés). `{N}` se cambia por el nombre del niño. `\x01` es un corazón, `\x02` una rupia, `\x03` una trifuerza y `\x04` un destello.
- **Dibujos y letras:** están en `firmware/herramientas/arte.py`, dibujados con `#` y `.`. Después de cambiarlos, ejecuta `python3 firmware/herramientas/generar.py` para regenerar `fuente.h` y `sprites.h`.
- **Simulador:** en `firmware/herramientas/simulador` puedes ver las animaciones en el ordenador sin el reloj. Compila con:
  `g++ -std=gnu++17 -I. -I../../RelojHyrule -include Arduino.h sim.cpp ../../RelojHyrule/{pantalla,animaciones,esfera,tiempo,ajustes}.cpp -o sim`.
  Después, `mkdir -p out && ./sim 1 7 out/1.txt` graba la aventura 1 (`./sim mensaje2 1 out/m.txt` graba un mensaje traído por el hada) (el segundo número es la semilla del azar; añade `en` al final para verla en inglés). Para generar el GIF necesitas Python con Pillow: `python3 -c "from dibujar import gif; gif('1','cofre.gif')"`.
- **Página de mensajes del móvil:** se edita en `firmware/herramientas/pagina_mensaje/mensaje.html`. Después, `python3 firmware/herramientas/pagina_mensaje/generar.py` crea `pagina_mensaje.h` con la fuente y los dibujos del reloj (para la vista previa) y el icono de la pantalla de inicio.
- **Animaciones y capturas del README:** `python3 grabar_gifs.py` vuelve a grabar todos los GIF de `imagenes/` (español) e `imagenes/en/` (inglés), y `python3 captura_portal.py` hace las capturas de la página de configuración (necesita Playwright).

## Archivos

| Archivo | Para qué sirve |
|---------|----------------|
| `RelojHyrule.ino` | Punto de entrada (`setup` y `loop`) |
| `principal.*` | Programa principal: decide qué se ve en cada momento |
| `configuracion.h` | Versión, pines y nombres de red |
| `pantalla.*` | Controlador propio de las matrices MAX7219, texto con tildes y Ñ |
| `esfera.*` | Cómo se dibujan la hora, AM/PM, la fecha y el hada de los segundos |
| `animaciones.*` | Las 11 aventuras, los personajes que traen los mensajes y los momentos especiales |
| `tiempo.*` | Hora por internet, zona horaria, fechas en español e inglés y cumpleaños |
| `ajustes.*` | Guarda la configuración en la memoria del reloj (y `tr()` para elegir el texto según el idioma) |
| `red.*` | WiFi, portal cautivo, páginas web y API |
| `pagina_web.h` | La página de configuración (HTML, CSS y JavaScript), en español e inglés, con el menú de pruebas |
| `pagina_mensaje.h` | La página de mensajes del móvil (`/mensaje`), su icono y su manifiesto (generado) |
| `sonido.*` | Zumbador opcional (en desarrollo): melodías originales sin bloquear, por PWM |
| `actualizacion.*` | Actualizaciones por internet, por navegador y por el IDE |
| `frases.h` | Frases de ánimo y tesoros, en español e inglés |
| `fuente.h`, `sprites.h` | Letras y dibujos (generados con `herramientas/generar.py`) |

Uso de memoria (versión 1.5.0): 43 KB de RAM (53 %) y 574 KB de flash (56 % del espacio para el programa).

