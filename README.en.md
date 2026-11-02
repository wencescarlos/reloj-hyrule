# ▲ Hyrule Clock

[Español](README.md) · **English**

[![Firmware](https://img.shields.io/github/v/release/wencescarlos/reloj-hyrule?label=firmware&color=2e7d32)](https://github.com/wencescarlos/reloj-hyrule/releases/latest) [![Build](https://github.com/wencescarlos/reloj-hyrule/actions/workflows/publicar-firmware.yml/badge.svg)](https://github.com/wencescarlos/reloj-hyrule/actions/workflows/publicar-firmware.yml) [![License: CC BY-NC-SA 4.0](https://img.shields.io/badge/license-CC%20BY--NC--SA%204.0-lightgrey.svg)](LICENSE) ![ESP8266](https://img.shields.io/badge/board-Wemos%20D1%20mini-blue)

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
| 🎓 **Focus mode** | For studying: just the time, still, no date or adventures. By hand or on a schedule |
| ⏰ **Morning alarm** | At the time and on the days you choose, with its good-morning message |
| 🔔 **Sound (optional, in development)** | With a buzzer: alerts and alarm with original tunes and adjustable volume |
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
| *Optional, in development:* 12 mm passive buzzer (a PC motherboard speaker works) + NPN transistor, 1 kΩ resistor and 1N4148 diode | 1 |
| PLA filament: stone grey, white, green and gold | — |

### Wiring

<p align="center"><img src="imagenes/esquema_conexiones.svg" alt="Wiring diagram of the Wemos D1 mini and the MAX7219 matrix" width="760"></p>

| MAX7219 matrix | Wemos D1 mini |
|---|---|
| VCC | 5V |
| GND | G |
| CLK | D5 |
| CS | D6 |
| DIN | D7 |

> ⚠️ **Solder the wires to the matrix INPUT**, the side marked **DIN** (or IN). If you use the output (OUT or DOUT), the display stays dark or shows random dots.
>
> 💡 **For a stable display:**
> - Solder a **100–470 µF electrolytic capacitor** (10 V or higher) between VCC and GND at the matrix input. Mind the polarity: the **−** leg goes to GND.
> - Keep the wires **under 15 cm**.
> - Power the clock with a **5 V, 1 A** charger or better.
>
> On top of that, the clock resends its setup to the matrix every 2 seconds, so a module that gets scrambled fixes itself.
>
> 🛠️ **If you still see stray pixels while text scrolls**, the cause is that the Wemos sends 3.3 V signals and the matrix, powered at 5 V, wants at least 3.5 V. From easiest to best:
> 1. Turn on **Slow signals** in the *Test menu* of the clock's page and save.
> 2. Solder the 100–470 µF **capacitor** at the matrix input and shorten the wires.
> 3. **Definitive fix:** a **74AHCT125** (or 74HCT125) level shifter powered at 5 V between the Wemos and the matrix. It turns the 3.3 V signals into 5 V.
>
> | 74AHCT125 | Connection |
> |---|---|
> | 14 (VCC) | 5V |
> | 7 (GND), 1, 4, 10 (1OE, 2OE, 3OE) and 12 (4A) | G |
> | 13 (4OE) | 5V |
> | 2 (1A) → 3 (1Y) | D7 → DIN |
> | 5 (2A) → 6 (2Y) | D5 → CLK |
> | 9 (3A) → 8 (3Y) | D6 → CS |

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
| **Or:** back cover with a ring for the optional buzzer | `tapa_trasera_zumbador.stl` | Stone grey |
| Diffuser window | `ventana_difusora.stl` | **White** (lets the light through) |
| Gold Triforce for the cover | `trifuerza_dorada_trasera.stl` | Silk gold |
| Gold Triforce for the pedestal | `trifuerza_dorada_frontal.stl` | Silk gold |
| Matrix holding bars | `barras_sujecion_matriz.stl` | Any |
| Wemos holder | `soporte_wemos.stl` | Any |
| **Or:** holder for a Wemos **with soldered pin headers** plus a buzzer ring | `soporte_wemos_zumbador.stl` | Any |
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
   - **MMU:** 16KB cache + 48KB IRAM and 2nd Heap (shared) — *needed for internet updates*
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
4. Choose your home WiFi, type the password and press **Connect and restart**.
5. Done: the clock is at **http://reloj-hyrule.local**. Type the child's name and birthday there.

> If the letters look rotated or jumbled, go to **Display** on the clock's page, change the rotation or the order of the modules and press **Test display**: you should see 1 2 3 4.
>
> If the display stays dark or does odd things, open the **🔧 Test menu (advanced)** at the end of the page. It is closed so it doesn't confuse people who only want to use the clock. Its **Display diagnostics** has three steps:
> - **Light the LEDs:** lights every LED, or one module at a time, using the matrix's own test mode.
> - **Measure the pins:** holds each pin at 3.3 V so you can check it with a multimeter.
> - **Change pins:** if a Wemos pin is damaged, you can move that wire to another pin.
>
> That menu also shows the system status (reason for the last restart, time on and free memory) and, if you have a buzzer, its tests.

<p align="center"><img src="imagenes/en/portal_configuracion.png" alt="Settings page" width="700"></p>

---

## 🎓 Focus mode

For homework without distractions, the clock shows **just the time, still**: no blinking, no seconds, no date, no adventures or greetings. Messages you send still show up.
- **By hand:** from the clock's page or the phone page, for 25 min, 45 min, 1 h, 2 h or no limit.
- **On a schedule:** for example, Monday to Friday from 17:00 to 18:00 it turns on by itself.
- **When it ends:** optionally, "WELL DONE, {name}! TIME FOR A BREAK!" scrolls by (and a tune plays, if you have a buzzer).

## ⏰ Morning alarm

At the time and on the weekdays you choose, the clock wakes up and shows a good-morning message for the number of seconds you set. You can write your own message. If you have a buzzer, the tune you pick plays too.

## 🔔 Sound (optional, in development)

> 🚧 **This part is in development.** It works, but it is still being tested with different buzzers, and the sound or the assembly may change in future versions. The clock works perfectly without a buzzer.

It comes **turned off from the factory**. Until you turn it on, the sound options don't appear on any page. To use it, fit the buzzer and, on the clock's page, go to **Sound**, tick **I have a buzzer connected** and press **Save settings**. Then you get:
- **Volume** from 1 to 10 and buttons to try each tune.
- **Alert sound** on the phone message page.
- **Tune** for the morning alarm.
- **Buzzer tests** in the test menu: the pin (D1 from the factory) and a continuous beep to check it with a multimeter.

With a **12 mm passive buzzer**, like a PC motherboard speaker, the clock plays an alert when you send a message (if you pick a tune) and sounds with the alarm.

The clock **only makes sound when asked**: adventures and the time are silent. The 8 tunes (Adventure, Little bells, Sunrise, Victory, Alert, Mystery, March and Lullaby) are **original compositions** made for this project; the clock does not play music from the games.

The buzzer is **optional** and there are two places for it:
- **In the back cover:** print `tapa_trasera_zumbador.stl` instead of `tapa_trasera.stl`. The buzzer sits behind a vent and is louder.
- **Next to the Wemos:** print `soporte_wemos_zumbador.stl` instead of `soporte_wemos.stl`. It works with a Wemos that has **pin headers and dupont connectors**: it only passes over the antenna tip, where there are no pins. On one side it has a **loose ring for the buzzer** (it must not squeeze it: a squeezed buzzer barely sounds); if the buzzer is soldered on a small board, the board hangs from it.

It connects to **D1** (diagram in Spanish):

<p align="center"><img src="imagenes/esquema_zumbador.svg" alt="Buzzer wiring" width="760"></p>

## 📱 Messages from your phone

At **http://reloj-hyrule.local/mensaje** there is a page made for phones:
- **Your message:** type it yourself or pick a quick one (dinner time, bedtime...). The **+ name** button adds the child's name.
- **Who brings it:** the hero, the fairy, a chest, hearts, the Triforce, a bomb, a surprise or a full adventure.
- **Time on the display:** once, or repeated for 30 s, 1, 2, 5 or 10 minutes.
- **Speed:** slow is ideal for early readers.
- **Alert sound:** only shows up if sound is turned on.
- **Focus mode:** turn it on or off from your phone.
- **Preview:** shows how it will look on the clock's display, and the ■ button stops the message early.

**To use it like an app on an iPhone:** open the page in **Safari**, tap **Share** (the square with the arrow) and choose **Add to Home Screen**. The Triforce icon appears and the page opens full screen. On Android, in Chrome: menu ⋮ → **Add to Home screen**.

> The phone must be on the same WiFi as the clock.

<p align="center"><img src="imagenes/pagina_mensaje.png" alt="Message page on a phone" width="320"></p>

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

You can also download each version's `firmware.bin` from [Releases](https://github.com/wencescarlos/reloj-hyrule/releases) and read what changed in the [changelog](CHANGELOG.md).

---

## 📁 Structure

```
reloj-hyrule/
├── firmware/
│   ├── RelojHyrule/        ← clock firmware (Arduino IDE)
│   └── herramientas/       ← font/sprite drawing, PC simulator and animation recording
├── modelos_3d/             ← v2 case (STL), generator script and instructions
├── platformio.ini          ← build with Visual Studio Code and PlatformIO
├── docs/                   ← project website (GitHub Pages)
├── imagenes/               ← screenshots and animations (en/ = in English)
├── .github/               ← automatic build and issue templates
├── README.md               ← this document in Spanish
├── CHANGELOG.md            ← changes by version
├── CONTRIBUTING.md         ← how to contribute
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
