# Modelos 3D: impresión y montaje

> ⚠️ Proyecto de fans **sin ánimo de lucro** y no oficial. *The Legend of Zelda* es una marca de Nintendo. Prohibida la venta de piezas o relojes. Ver [AVISO_LEGAL.md](../AVISO_LEGAL.md).

Reloj basado en la caja *LED Marquee Scroller* de David Payne (Qrome) (Wemos D1 mini + 4 matrices LED MAX7219) con una cresta del logo de *The Legend of Zelda* integrada en la caja.

- **Medidas finales:** 148 × 116 mm de frente, unos 42 mm de fondo.
- **Inclinación:** 14° hacia atrás, apoyado en sus patas.
- **Estabilidad:** comprobada con la electrónica dentro (el centro de masas queda a unos 17 mm del borde delantero del apoyo).

---

## 1. Piezas

Todas las piezas están en esta carpeta, listas para imprimir:

| # | Pieza | Archivo | Color recomendado |
|---|-------|---------|-------------------|
| 1 | Base con cresta | `base_reloj_zelda.stl` | Blanco hueso / marfil (**color claro obligatorio**) |
| 2 | Fondo del logo | `fondo_logo.stl` | Verde (tipo túnica de Link) |
| 3 | Letras del logo | `logo_letras_geektoybox.stl` | Dorado silk |
| 4 | Tapa trasera con Trifuerza | `tapa_trifuerza.stl` | Como la base |
| 5 | Trifuerza dorada | `trifuerza_dorada.stl` | Dorado silk |

La base es la caja de Qrome con una cresta añadida para el logo. El fondo tiene un rebaje de 0,5 mm con la forma de cada letra, para colocarlas fácilmente. La tapa trasera es la original de Qrome: la rejilla central se ha cambiado por una **Trifuerza recortada** de 30 mm (también ventila) y las rejillas de los lados se mantienen para el D1 mini.

### Autores y licencias

| Archivo | Autor | Licencia |
|---------|-------|----------|
| `base_reloj_zelda.stl`, `tapa_trifuerza.stl` | Remezcla de la caja de **Qrome** ([Printables](https://www.printables.com/model/25374-esp8266-wemos-clock-news-weather-scrolling-marquee)) | CC BY-NC 4.0 |
| `logo_letras_geektoybox.stl` | **geektoybox** ([Printables](https://www.printables.com/model/499505-legend-of-zelda-logo-text)), sin cambios | CC BY-NC 4.0 |
| `fondo_logo.stl` | Hecho a partir del logo de **geektoybox** | CC BY-NC 4.0 |
| `trifuerza_dorada.stl` | Original de este proyecto | CC BY-NC-SA 4.0 |

**Prohibido el uso comercial** de todas las piezas. Si compartes tus impresiones (por ejemplo, un *make* en Printables), cita a Qrome, a geektoybox y este repositorio.

### Generarlas desde los originales (opcional)

Si prefieres partir de los archivos originales de sus autores, [`generar_modelos.py`](generar_modelos.py) crea las mismas piezas:
```
pip install trimesh manifold3d shapely numpy
python3 generar_modelos.py --base led_marque_base_1.stl --tapa led_marque_plate_1.stl --logo LoZ_Logo_Text.stl
```

> ⚠️ **La base tiene que ser de un color claro.** La pantalla se ve a través de una piel frontal de solo 0,6 mm. En negro o en otro color oscuro, los LED no se verán.

### Filamentos sugeridos (Amazon.es)

- Dorado: [Creality CR-Silk PLA Oro](https://www.amazon.es/Creality-CR-Silk-PLA-Silk-oro/dp/B0BZD26PLT) o [GIANTARM PLA Silk Dorado](https://www.amazon.es/GIANTARM-Filamento-1-75mm-Impresora-Carrete/dp/B07RZRJNJ7)
- Verde: [CC3D PLA MAX Classic Green](https://www.amazon.es/Filamento-filamento-impresora-impresi%C3%B3n-CC3D/dp/B09B3M2MTD) o [3D Solutech Camo Green](https://www.amazon.es/3D-Solutech-Filamento-Precisi%C3%B3n-dimensional/dp/B07BN8H2LQ)
- Claro: [GEEETECH PLA Bone White](https://www.amazon.es/GEEETECH-Filament-Filamento-Reliable-Printing/dp/B01MR5OCX5) o [eSUN PLA Cool White](https://www.amazon.es/eSUN-Filamento-Velocidad-Dimensional-Impresoras/dp/B0CR1DBBFM)

**Otras combinaciones** (con las letras siempre doradas): fondo negro (elegante), azul oscuro (Espada Maestra / Breath of the Wild) o rojo granate (Zelda clásico de NES/SNES).

---

## 2. Ajustes de impresión

Material: PLA, 1,75 mm. No hace falta multicolor ni cambiar de filamento a mitad de la impresión.

| Pieza | Orientación | Capa | Relleno | Soportes | Notas |
|-------|-------------|------|---------|----------|-------|
| Base | **Boca abajo**: cara de la pantalla y cresta sobre la cama (la orientación del STL) | 0,2 mm | 15–20 % | **No** | Cama: 150 × 117 mm. La primera capa tiene que salir perfecta: es la cara vista. Usa cama lisa (PEI liso o cristal) para un frontal brillante. |
| Fondo verde | Plana, con el rebaje de las letras **hacia arriba** | 0,2 mm (o 0,12 mm para más detalle) | 100 % | No | Pieza de 1,6 mm de grosor. |
| Letras del logo | Plana, tal como viene | 0,12–0,16 mm | 100 % | No | Son 14 piezas sueltas. Si alguna letra pequeña se despega, usa un *brim* fino. |
| Tapa trasera con Trifuerza | Plana, con el soporte del D1 mini hacia arriba (cara exterior sobre la cama) | 0,2 mm | 20 % | No | La Trifuerza queda en la cara lisa de la cama. |
| Trifuerza dorada | Con la pestaña plana sobre la cama y los triángulos hacia arriba | 0,12–0,16 mm | 100 % | No | La cara de arriba de los triángulos es la que se ve. |

**Consejos:**
- La piel de 0,6 mm son 3 capas de 0,2 mm. No cambies la altura de la primera capa en la base o la pantalla se verá peor (o no se verá).
- En la base, las 3 escuadras traseras de refuerzo de la cresta crecen hacia arriba y no necesitan soporte.
- Los dos agujeros pequeños del frontal de la cresta (Ø 1,9 mm) son para las guías: no los tapes.

---

## 3. Montaje del logo

Material: pegamento (cianoacrilato en gel o adhesivo de contacto) y un trozo de filamento de 1,75 mm.

1. **Letras en el fondo:** coloca las 14 piezas doradas en su rebaje del fondo verde. Cada una encaja solo en su sitio (holgura de 0,12 mm). Pon una gota pequeña de pegamento en cada una.
2. **Guías:** corta **2 trozos de filamento de 1,75 mm de unos 3,5 mm de largo**. Mételos en los agujeros de la parte trasera del fondo verde.
3. **Fondo en la cresta:** encaja las guías en los dos agujeros del frontal de la cresta y pega el fondo. Debe quedar un borde claro de unos 3 mm alrededor del verde.
4. Deja secar con algo de peso encima, sobre una superficie plana.

---

## 4. Trifuerza de la tapa trasera

1. Mete la pieza dorada **por dentro** de la tapa: los tres triángulos entran en sus huecos y la pestaña fina queda apoyada en la cara interior.
2. Por fuera, los triángulos dorados quedan enrasados con la tapa (holgura de 0,15 mm).
3. Fíjala con unas gotas de pegamento en la pestaña.

Con el reloj de pie, la Trifuerza queda con la punta hacia arriba. Si prefieres que ventile, no pongas la pieza dorada: los huecos de la Trifuerza hacen de rejilla.

## 5. Electrónica

![Esquema de conexiones](../imagenes/esquema_conexiones.svg)

| Componente | Cantidad |
|------------|----------|
| Wemos D1 mini (ESP8266) | 1 |
| Matriz LED MAX7219 8×8 (32 × 32 mm), en módulo de 4 en línea (32 × 8) | 1 (o 4 unidades) |
| Cable USB y cargador de 5 V | 1 |
| Cables dupont o cable fino | 5 |

### Conexiones

| Matriz MAX7219 | Wemos D1 mini |
|----------------|---------------|
| VCC | 5V |
| GND | G |
| CLK | D5 (SCK) |
| CS | D6 |
| DIN | D7 (MOSI) |

### Montaje

1. Coloca las matrices **boca abajo** en el bolsillo interior de la base (128,4 × 32,4 mm), apoyadas contra la piel frontal. La entrada (IN) va hacia el lado del D1 mini.
2. Encaja el D1 mini en el soporte de la tapa trasera, con el USB hacia la muesca lateral de la caja.
3. Conecta los cables según la tabla.
4. Cierra con la tapa trasera. El cable USB sale por la muesca lateral.

---

## 6. Programa

Las instrucciones para instalar el programa, configurarlo y actualizarlo están en el [README principal](../README.md).

## 7. Lista de comprobación final

- [ ] Base impresa en color claro y con la pantalla visible a través del frontal
- [ ] Letras pegadas en el fondo verde
- [ ] Fondo pegado y centrado en la cresta con las guías de filamento
- [ ] Trifuerza dorada pegada en la tapa trasera
- [ ] Matrices orientadas con la entrada (IN) hacia el D1 mini
- [ ] Cables según la tabla de conexiones
- [ ] Firmware cargado y WiFi configurada
- [ ] Brillo bajo y horario nocturno ajustados

¡Que la Trifuerza te acompañe! ▲

---
*Homenaje de fans sin ánimo de lucro. No oficial y sin relación con Nintendo.*
