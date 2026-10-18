# ▲ Reloj de Hyrule

**Un reloj con aventuras para pequeños héroes.** Hecho con impresión 3D, un Wemos D1 mini y 4 matrices LED, como homenaje a *The Legend of Zelda*.

> ## ⚠️ Proyecto de fans SIN ÁNIMO DE LUCRO
> - Hecho en casa como regalo para un niño y compartido **gratis**.
> - **No es oficial** ni tiene relación con **Nintendo**. *The Legend of Zelda* es una marca de Nintendo.
> - **Prohibido venderlo**: ni el código, ni los modelos, ni relojes hechos con él (licencia [CC BY-NC-SA 4.0](LICENSE)).
> - El código, la carcasa 3D, la fuente de letras y los dibujos en píxeles son **originales**. Las letras del logo son de geektoybox y se usan **con su licencia CC BY-NC 4.0, citándolo**.
> - Más información en el [aviso legal](AVISO_LEGAL.md).

<p align="center">
  <img src="imagenes/DEMO.gif" alt="Animaciones del reloj" width="420"><br>
  <i>Animaciones reales del reloj (grabadas con el simulador incluido)</i>
</p>

<p align="center">
  <img src="imagenes/reloj_v2.png" alt="Reloj de Hyrule v2: bloques de piedra antigua" width="480">
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
| 🧒 **Con el nombre del niño** | "¡LUCAS HA REUNIDO LA TRIFUERZA!" |
| 🎂 **Cumpleaños** | Cuenta atrás y fiesta con tarta y fuegos artificiales |
| 🌙 **Buenos días y buenas noches** | Y modo noche: brillo mínimo o pantalla apagada |
| 📱 **Configuración desde el móvil** | En http://reloj-hyrule.local, sin instalar ninguna app |
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
| Tornillo M3 × 8 allen de cabeza cilíndrica (DIN 912), todos iguales | 8 |
| Inserto roscado térmico M3 (largo 5,7 mm, Ø exterior 4,6 mm) | 8 |
| Cables dupont o cable fino | 5 |
| Cargador USB de 5 V y cable micro-USB | 1 |
| Filamento PLA: gris piedra, blanco, verde y dorado | — |

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

## 🖨️ Impresión 3D: versión 2 "bloques de piedra antigua"

Carcasa **diseñada desde cero** como dos bloques de piedra de Hyrule: una losa con la pantalla y la cresta del logo, sobre un pedestal con una Trifuerza dorada incrustada, esquinas desgastadas, grietas talladas y dos patas que inclinan el reloj 8° hacia atrás. El Wemos va **dentro del pedestal**, con el **USB en el centro de la tapa trasera, justo debajo de la Trifuerza**. La matriz queda sujeta con **dos barras con lengüetas elásticas** y la tapa se cierra con **tornillos allen** encajados a ras. Todos los tornillos son iguales: **M3 × 8 DIN 912** ([esquema de tornillería](imagenes/esquema_tornilleria.svg)).

<p align="center">
  <img src="imagenes/reloj_v2_trasera.png" alt="Tapa trasera con Trifuerza y USB" width="380">
  <img src="imagenes/reloj_v2_interior.png" alt="Interior: matriz y Wemos" width="380">
</p>

Todas las piezas están en [`modelos_3d/`](modelos_3d/):

| Pieza | Archivo | Color |
|-------|---------|-------|
| Carcasa de piedra (losa, pedestal, patas y cresta) | `cuerpo_piedra.stl` | Gris piedra |
| Tapa trasera (Trifuerza, USB y 4 tornillos) | `tapa_trasera.stl` | Gris piedra |
| Ventana difusora | `ventana_difusora.stl` | **Blanco** (deja pasar la luz) |
| Trifuerza dorada de la tapa | `trifuerza_dorada_trasera.stl` | Dorado silk |
| Trifuerza dorada del pedestal | `trifuerza_dorada_frontal.stl` | Dorado silk |
| Barras de sujeción de la matriz | `barras_sujecion_matriz.stl` | Cualquiera |
| Fondo del logo | `fondo_logo.stl` | Verde |
| Letras del logo | `logo_letras_geektoybox.stl` | Dorado silk |

Las letras del logo son de **geektoybox** ([Printables](https://www.printables.com/model/499505-legend-of-zelda-logo-text), CC BY-NC 4.0). Todo lo demás es diseño original de este proyecto. El script [`modelos_3d/generar_modelos.py`](modelos_3d/generar_modelos.py) genera todas las piezas a partir del logo original.

Las instrucciones completas de impresión y montaje, con la lista de tornillería, están en [`modelos_3d/LEEME.md`](modelos_3d/LEEME.md).

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
│   └── herramientas/       ← dibujar letras/sprites y simulador en PC
├── modelos_3d/             ← carcasa v2 (STL), generador e instrucciones
├── docs/                   ← web del proyecto (GitHub Pages)
├── imagenes/               ← capturas y animaciones
├── .github/workflows/      ← compilación y publicación automática
├── AVISO_LEGAL.md          ← proyecto sin ánimo de lucro, marcas y créditos
└── LICENSE                 ← CC BY-NC-SA 4.0
```

---

## 🙏 Créditos

- ***Legend of Zelda Logo Text*:** **geektoybox**, [Printables](https://www.printables.com/model/499505-legend-of-zelda-logo-text), licencia CC BY-NC 4.0. Las letras se incluyen sin cambios y su contorno da forma a la cresta y al fondo.
- **Idea original del reloj con Wemos D1 mini y matriz MAX7219:** **Qrome (David Payne)**, [Marquee Scroller](https://github.com/Qrome/marquee-scroller). Este proyecto usa el mismo hardware, pero el firmware y la carcasa son nuevos.
- **Carcasa v2, firmware, fuente de letras, dibujos en píxeles, textos, página web y Trifuerzas doradas:** originales de [wencescarlos](https://github.com/wencescarlos), hechos desde cero.
- ***The Legend of Zelda*:** marca de **Nintendo**. Gracias por tantas aventuras. 💚

---

<p align="center"><b>Hecho con cariño y sin ánimo de lucro</b><br>
<sub>Proyecto no oficial y sin relación con Nintendo · Prohibida su venta · CC BY-NC-SA 4.0</sub></p>
