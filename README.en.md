# ▲ Hyrule Clock

[Español](README.md) · **English**

**A clock with adventures for little heroes.** Made with 3D printing, a Wemos D1 mini and 4 LED matrices, as a tribute to *The Legend of Zelda*.

> ## ⚠️ NON-PROFIT fan project
> - Made at home as a gift for a child and shared **for free**.
> - **Unofficial** and not affiliated with **Nintendo**. *The Legend of Zelda* is a trademark of Nintendo.
> - **Not for sale**: neither the code, nor the models, nor clocks built with them ([CC BY-NC-SA 4.0](LICENSE) license).
> - The code, the 3D case, the font and the pixel art are **original**. The logo letters are by geektoybox and are used **under their CC BY-NC 4.0 license, with credit**.
> - More information in the [legal notice](AVISO_LEGAL.md) (in Spanish).

<p align="center">
  <img src="imagenes/en/DEMO.gif" alt="Clock animations" width="420"><br>
  <i>Real clock animations (recorded with the included simulator)</i>
</p>

<p align="center">
  <img src="imagenes/reloj_v2.png" alt="Hyrule Clock v2: ancient stone blocks" width="480">
</p>

---

## ✨ What it does

| | |
|---|---|
| 🕐 **Exact time from the internet** | With automatic daylight saving time |
| 🔤 **24 h or 12 h with AM/PM** | In 12-hour mode an **A** (morning) or a **P** (afternoon) shows up |
| 📅 **Date** | "18/10" every half minute or "SUNDAY, OCTOBER 18" now and then |
| 🧚 **The seconds fairy** | Flies along the bottom row, from side to side |
| 🗡️ **11 random adventures** | Treasure chest, monster battle, Triforce, fairy, bomb, sword... |
| 🧒 **With the child's name** | "ALEJANDRO HAS ASSEMBLED THE TRIFORCE!" |
| 🎂 **Birthday** | Countdown and a party with cake and fireworks |
| 🌙 **Good morning and good night** | Plus night mode: minimum brightness or display off |
| 📱 **Setup from your phone** | At http://reloj-hyrule.local, no app to install |
| 🌍 **English or Spanish** | Chosen on the clock's page: it changes the clock's texts and the page itself |
| 🔄 **Updates itself** | Downloads new versions from this repository |

<p align="center">
  <img src="imagenes/en/02_cofre_del_tesoro.gif" width="270">
  <img src="imagenes/en/03_combate.gif" width="270">
  <img src="imagenes/en/04_trifuerza.gif" width="270">
</p>

---

## 🧰 Materials

| Item | Quantity |
|------|----------|
| Wemos D1 mini (ESP8266) | 1 |
| MAX7219 4-in-1 LED matrix (32 × 8, 32 × 32 mm modules) | 1 |
| M3 × 6 self-tapping countersunk hex socket screw, stainless (all the same, no inserts) | 10 |
| Dupont wires or thin wire | 5 |
| 5 V USB charger and micro-USB cable | 1 |
| PLA filament: stone grey, white, green and gold | — |

### Wiring

<p align="center"><img src="imagenes/esquema_conexiones.svg" alt="Wiring diagram of the Wemos D1 mini and the MAX7219 matrix" width="760"></p>

| MAX7219 matrix | Wemos D1 mini |
|---|---|
| VCC | 5V |
| GND | G |
| CLK | D5 |

> ⚠️ **Solder the wires to the matrix INPUT**, the side marked **DIN** (or IN). If you use the output (OUT or DOUT), the display stays dark or shows random dots.
| CS | D6 |
| DIN | D7 |

---

## 🖨️ 3D printing: version 2 "ancient stone blocks"

A case **designed from scratch** as two Hyrule stone blocks: a slab holding the display and the logo crest, on top of a pedestal with an inlaid gold Triforce, worn corners, carved cracks and two feet that tilt the clock 8° backwards. The Wemos sits **inside the pedestal**, held by a **screwed-in holder, no glue**, with the **USB port in the middle of the back cover, right under the Triforce**. The matrix is held by **two bars with spring tongues**. All screws are the same, **M3 × 6 self-tapping countersunk hex socket**, with no inserts, and **all of them go in from the back** ([screw diagram](imagenes/esquema_tornilleria.svg), in Spanish).

<p align="center">
  <img src="imagenes/reloj_v2_trasera.png" alt="Back cover with the Triforce and the USB port" width="380">
  <img src="imagenes/reloj_v2_interior.png" alt="Inside: matrix and Wemos" width="380">
</p>

All parts are in [`modelos_3d/`](modelos_3d/):

| Part | File | Color |
|------|------|-------|
| Stone case (slab, pedestal, feet and crest) | `cuerpo_piedra.stl` | Stone grey |
| Back cover (Triforce, USB and 4 screws) | `tapa_trasera.stl` | Stone grey |
| Diffuser window | `ventana_difusora.stl` | **White** (lets the light through) |
| Gold Triforce for the cover | `trifuerza_dorada_trasera.stl` | Silk gold |
| Gold Triforce for the pedestal | `trifuerza_dorada_frontal.stl` | Silk gold |
| Matrix holding bars | `barras_sujecion_matriz.stl` | Any |
| Wemos holder | `soporte_wemos.stl` | Any |
| Logo background | `fondo_logo.stl` | Green |
| Logo letters | `logo_letras_geektoybox.stl` | Silk gold |
| **Or:** one-piece logo | `logo_dos_colores.stl` | Green and gold, with a pause at 2.0 mm to swap filament |

The logo letters are by **geektoybox** ([Printables](https://www.printables.com/model/499505-legend-of-zelda-logo-text), CC BY-NC 4.0). Everything else is an original design from this project. The [`modelos_3d/generar_modelos.py`](modelos_3d/generar_modelos.py) script builds every part from the original logo.

**Quick print notes:** PLA 1.75 mm, no supports and no multicolor printer needed. Print the case **face down** (as the STL comes) with 0.2 mm layers and 4 perimeters. The screws cut their own thread in 2.5 mm pilot holes: tighten gently. For the one-piece logo, print with 0.2 mm layers and add a filament change (M600) or pause at the layer that ends at **2.2 mm**: green below, gold above. The full print and assembly guide is in [`modelos_3d/LEEME.md`](modelos_3d/LEEME.md) (in Spanish).

---

## 💾 Installing the firmware

1. Install the [Arduino IDE](https://www.arduino.cc/en/software).
2. Go to **File → Preferences → Additional boards manager URLs** and add:
   `https://arduino.esp8266.com/stable/package_esp8266com_index.json`
3. In the **Boards Manager**, install **esp8266** (version 3.1.2 or later). **No libraries needed.**
4. Open `firmware/RelojHyrule/RelojHyrule.ino` and choose:
   - **Board:** LOLIN(WEMOS) D1 R2 & mini
   - **Flash Size:** 4MB (FS:1MB OTA:~1019KB)
5. Press **Upload**.

### With Visual Studio Code and PlatformIO

1. Install [Visual Studio Code](https://code.visualstudio.com/) and the **PlatformIO IDE** extension (VS Code suggests it when you open the folder).
2. Open the repository folder. The [`platformio.ini`](platformio.ini) file is already set up: board, memory and LittleFS.
3. In the PlatformIO bottom bar: **✓** builds, **→** uploads over USB and **🔌** opens the serial monitor.
4. To upload over WiFi to a clock that is already installed, choose the **d1_mini_wifi** environment (it uses `reloj-hyrule.local` and your admin password).

**Without building:** download `firmware.bin` from the [project website](https://wencescarlos.github.io/reloj-hyrule/) and flash it with `esptool.py write_flash 0x0 firmware.bin`.

### First time

1. Connect your phone to the **Reloj-Hyrule** WiFi (password: **trifuerza**).
2. The clock's page opens (if it doesn't, go to http://192.168.4.1).
3. Tap **English** at the top of the page to switch the clock and the page to English.
4. Choose your home WiFi, type the password and press **Connect**.
5. Done: the clock is at **http://reloj-hyrule.local**. Type the child's name and birthday there.

> If the letters look rotated or jumbled, go to **Display** on the clock's page, change the rotation or the order of the modules and press **Test display**: you should see 1 2 3 4.
>
> If the display stays dark or does odd things, use **Display diagnostics** on the same page. It has three steps:
> - **Light the LEDs:** lights every LED, or one module at a time, using the matrix's own test mode.
> - **Measure the pins:** holds each pin at 3.3 V so you can check it with a multimeter.
> - **Change pins:** if a Wemos pin is damaged, you can move that wire to another pin.

<p align="center"><img src="imagenes/en/portal_configuracion.png" alt="Settings page" width="700"></p>

---

## 🔄 Automatic updates

Every time a change is pushed to the `firmware/` folder of this repository, **GitHub Actions** builds the firmware and publishes it on **GitHub Pages**:

- `https://wencescarlos.github.io/reloj-hyrule/version.txt`
- `https://wencescarlos.github.io/reloj-hyrule/firmware.bin`

Clocks check that address once a day. If the version is newer than theirs, **they update themselves**.

To publish a new version:
1. Change the number in `firmware/RelojHyrule/configuracion.h`, for example `#define VERSION_RELOJ "1.0.1"`.
2. Push the change to GitHub (*commit* and *push*).
3. Wait for the **"Compilar y publicar el firmware"** action to finish (*Actions* tab).

---

## 📁 Structure

```
reloj-hyrule/
├── firmware/
│   ├── RelojHyrule/        ← clock firmware (Arduino IDE)
│   └── herramientas/       ← font/sprite drawing, PC simulator and animation recording
├── modelos_3d/             ← v2 case (STL), generator script and instructions
├── docs/                   ← project website (GitHub Pages)
├── imagenes/               ← screenshots and animations (en/ = in English)
├── .github/workflows/      ← automatic build and publishing
├── README.md               ← this document in Spanish
├── AVISO_LEGAL.md          ← non-profit project, trademarks and credits
└── LICENSE                 ← CC BY-NC-SA 4.0
```

The code and file names are in Spanish, as the project was born as a gift for a Spanish-speaking child.

---

## 🙏 Credits

- ***Legend of Zelda Logo Text*:** **geektoybox**, [Printables](https://www.printables.com/model/499505-legend-of-zelda-logo-text), CC BY-NC 4.0 license. The letters are included unchanged and their outline shapes the background and the crest.
- **Original idea of a clock with a Wemos D1 mini and a MAX7219 matrix:** **Qrome (David Payne)**, [Marquee Scroller](https://github.com/Qrome/marquee-scroller). This project uses the same hardware, but the firmware and the case are new.
- **v2 case, firmware, font, pixel art, texts, web page and gold Triforces:** original work by [wencescarlos](https://github.com/wencescarlos), made from scratch.
- ***The Legend of Zelda*:** trademark of **Nintendo**. Thanks for so many adventures. 💚

---

<p align="center"><b>Made with love, not for profit</b><br>
<sub>Unofficial project, not affiliated with Nintendo · Not for sale · CC BY-NC-SA 4.0</sub></p>
