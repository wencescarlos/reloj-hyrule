# ▲ Reloj de Hyrule

**Español** · [English](README.en.md)

**Un reloj con aventuras para pequeños héroes.** Hecho con impresión 3D, un Wemos D1 mini y 4 matrices LED, como homenaje a *The Legend of Zelda*.

> ## ⚠️ Proyecto de fans SIN ÁNIMO DE LUCRO
> - Hecho en casa como regalo para un niño y compartido **gratis**.
> - **No es oficial** ni tiene relación con **Nintendo**. *The Legend of Zelda* es una marca de Nintendo.
> - **Prohibido venderlo**: ni el código, ni los modelos, ni relojes hechos con él (licencia [CC BY-NC-SA 4.0](LICENSE)).
> - El código, la fuente de letras y los dibujos en píxeles son **originales**. Los modelos 3D de otros autores se usan **con su licencia CC BY-NC 4.0 y citándolos**.
> - Más información en el [aviso legal](AVISO_LEGAL.md).

<p align="center">
  <img src="imagenes/DEMO.gif" alt="Animaciones del reloj" width="420"><br>
  <i>Animaciones reales del reloj (grabadas con el simulador incluido)</i>
</p>

<p align="center">
  <img src="imagenes/render_3d.png" alt="Reloj impreso en 3D" width="420">
</p>

---

## ✨ Qué hace

| | |
|---|---|
| 🕐 **Hora exacta por internet** | Con el cambio de hora de verano e invierno automático |
| 🔤 **24 h o 12 h con AM/PM** | En 12 h sale una **A** (mañana) o una **P** (tarde) |
| 📅 **Fecha** | "18/10" cada medio minuto o "DOMINGO 18 DE OCTUBRE" de vez en cuando |
| 🧚 **El hada de los segundos** | Vuela por la fila de abajo, de lado a lado |
| 🗡️ **11 aventuras al azar** | Cofre del tesoro, combate, Trifuerza, hada, bomba, espada... |
| 🧒 **Con el nombre del niño** | "¡ALEJANDRO HA REUNIDO LA TRIFUERZA!" |
| 🎂 **Cumpleaños** | Cuenta atrás y fiesta con tarta y fuegos artificiales |
| 🌙 **Buenos días y buenas noches** | Y modo noche: brillo mínimo o pantalla apagada |
| 📱 **Configuración desde el móvil** | En http://reloj-hyrule.local, sin instalar ninguna app |
| 🌍 **En español o en inglés** | Se elige en la página del reloj: cambia los textos del reloj y de la página |
| 🔄 **Se actualiza solo** | Descarga las versiones nuevas desde este repositorio |

<p align="center">
  <img src="imagenes/02_cofre_del_tesoro.gif" width="270">
  <img src="imagenes/03_combate.gif" width="270">
  <img src="imagenes/04_trifuerza.gif" width="270">
</p>

---

## 🧰 Material

| Material | Cantidad |
|----------|----------|
| Wemos D1 mini (ESP8266) | 1 |
| Matriz LED MAX7219 4 en 1 (32 × 8, módulos de 32 × 32 mm) | 1 |
| Cables dupont o cable fino | 5 |
| Cargador USB de 5 V y cable micro-USB | 1 |
| Filamento PLA: claro para la caja, verde y dorado para el logo y la Trifuerza | — |

### Conexiones

<p align="center"><img src="imagenes/esquema_conexiones.svg" alt="Esquema de conexiones del Wemos D1 mini con la matriz MAX7219" width="760"></p>

| Matriz MAX7219 | Wemos D1 mini |
|---|---|
| VCC | 5V |
| GND | G |
| CLK | D5 |
| CS | D6 |
| DIN | D7 |

---

## 🖨️ Impresión 3D

Todas las piezas están listas para imprimir en [`modelos_3d/`](modelos_3d/):

| Pieza | Archivo | Color | Autoría |
|-------|---------|-------|---------|
| Base con cresta | `base_reloj_zelda.stl` | **Claro** (blanco o marfil), para que se vea la pantalla | Remezcla de la caja de **Qrome** |
| Tapa trasera con Trifuerza | `tapa_trifuerza.stl` | Como la base | Remezcla de la caja de **Qrome** |
| Fondo del logo | `fondo_logo.stl` | Verde | Hecho a partir del logo de **geektoybox** |
| Letras del logo | `logo_letras_geektoybox.stl` | Dorado silk | Original de **geektoybox** (sin cambios) |
| Trifuerza dorada | `trifuerza_dorada.stl` | Dorado silk | Original de este proyecto |

Los modelos de otros autores se publican con su licencia **CC BY-NC 4.0** (Reconocimiento-NoComercial), que permite remezclas sin fines comerciales:
- Caja *ESP8266 Wemos Clock News Weather Scrolling Marquee*, de **Qrome**: [Printables](https://www.printables.com/model/25374-esp8266-wemos-clock-news-weather-scrolling-marquee)
- *Legend of Zelda Logo Text*, de **geektoybox**: [Printables](https://www.printables.com/model/499505-legend-of-zelda-logo-text)

Si prefieres partir de los originales, [`modelos_3d/generar_modelos.py`](modelos_3d/generar_modelos.py) genera las mismas piezas a partir de ellos.

<p align="center"><img src="imagenes/render_trasera_trifuerza.png" alt="Tapa trasera con la Trifuerza" width="420"></p>

Las instrucciones completas de impresión y montaje están en [`modelos_3d/LEEME.md`](modelos_3d/LEEME.md).

---

## 💾 Instalar el programa

1. Instala el [Arduino IDE](https://www.arduino.cc/en/software).
2. Ve a **Archivo → Preferencias → URLs adicionales** y añade:
   `https://arduino.esp8266.com/stable/package_esp8266com_index.json`
3. En **Gestor de tarjetas**, instala **esp8266** (versión 3.1.2 o superior). **No hace falta ninguna librería.**
4. Abre `firmware/RelojHyrule/RelojHyrule.ino` y elige:
   - **Placa:** LOLIN(WEMOS) D1 R2 & mini
   - **Flash Size:** 4MB (FS:1MB OTA:~1019KB)
5. Pulsa **Subir**.

### Con Visual Studio Code y PlatformIO

1. Instala [Visual Studio Code](https://code.visualstudio.com/) y la extensión **PlatformIO IDE** (VS Code te la propone al abrir la carpeta).
2. Abre la carpeta del repositorio. El archivo [`platformio.ini`](platformio.ini) ya está configurado: placa, memoria y LittleFS.
3. En la barra inferior de PlatformIO: **✓** compila, **→** sube por USB y **🔌** abre el monitor serie.
4. Para subir por WiFi a un reloj ya instalado, elige el entorno **d1_mini_wifi** (usa `reloj-hyrule.local` y tu clave de administrador).

**Sin compilar:** descarga `firmware.bin` de la [web del proyecto](https://wencescarlos.github.io/reloj-hyrule/) y súbelo con `esptool.py write_flash 0x0 firmware.bin`.

### Primera vez

1. Conéctate con el móvil al WiFi **Reloj-Hyrule** (clave: **trifuerza**).
2. Se abre la página del reloj (si no se abre, entra en http://192.168.4.1).
3. Elige el WiFi de casa, escribe la clave y pulsa **Conectar**.
4. Ya está: el reloj está en **http://reloj-hyrule.local**. Escribe ahí el nombre del niño y su cumpleaños.
5. Arriba de la página puedes elegir el idioma: **Español** o **English**.

> Si las letras salen giradas o desordenadas, ve a **Pantalla** en la página del reloj, cambia el giro o el orden de los módulos y pulsa **Probar pantalla**: debe verse 1 2 3 4.

<p align="center"><img src="imagenes/portal_configuracion.png" alt="Portal de configuración" width="700"></p>

---

## 🔄 Actualizaciones automáticas

Cada vez que se sube un cambio a la carpeta `firmware/` de este repositorio, **GitHub Actions** compila el programa y lo publica en **GitHub Pages**:

- `https://wencescarlos.github.io/reloj-hyrule/version.txt`
- `https://wencescarlos.github.io/reloj-hyrule/firmware.bin`

Los relojes miran esa dirección una vez al día. Si la versión es más nueva que la suya, **se actualizan solos**.

Para publicar una versión nueva:
1. Cambia el número en `firmware/RelojHyrule/configuracion.h`, por ejemplo `#define VERSION_RELOJ "1.0.1"`.
2. Sube el cambio a GitHub (*commit* y *push*).
3. Espera a que termine la acción **"Compilar y publicar el firmware"** (pestaña *Actions*).

---

## 📁 Estructura

```
reloj-hyrule/
├── firmware/
│   ├── RelojHyrule/        ← programa del reloj (Arduino IDE)
│   └── herramientas/       ← dibujar letras/sprites, simulador en PC y grabación de animaciones
├── modelos_3d/             ← piezas STL listas para imprimir + instrucciones
├── docs/                   ← web del proyecto (GitHub Pages)
├── imagenes/               ← capturas y animaciones (en/ = en inglés)
├── .github/workflows/      ← compilación y publicación automática
├── README.en.md            ← este documento en inglés
├── AVISO_LEGAL.md          ← proyecto sin ánimo de lucro, marcas y créditos
└── LICENSE                 ← CC BY-NC-SA 4.0
```

---

## 🙏 Créditos

- **Caja *ESP8266 Wemos Clock News Weather Scrolling Marquee*:** **Qrome (David Payne)**, [Printables](https://www.printables.com/model/25374-esp8266-wemos-clock-news-weather-scrolling-marquee), licencia CC BY-NC 4.0. Remezclada: se ha añadido la cresta del logo a la base y la Trifuerza a la tapa.
- ***Legend of Zelda Logo Text*:** **geektoybox**, [Printables](https://www.printables.com/model/499505-legend-of-zelda-logo-text), licencia CC BY-NC 4.0. Las letras se incluyen sin cambios y su contorno se usa para el fondo y la cresta.
- **Firmware, fuente de letras, dibujos en píxeles, textos, página web, script de modelos y Trifuerza dorada:** originales de [wencescarlos](https://github.com/wencescarlos), hechos desde cero.
- ***The Legend of Zelda*:** marca de **Nintendo**. Gracias por tantas aventuras. 💚

---

<p align="center"><b>Hecho con cariño y sin ánimo de lucro</b><br>
<sub>Proyecto no oficial y sin relación con Nintendo · Prohibida su venta · CC BY-NC-SA 4.0</sub></p>
