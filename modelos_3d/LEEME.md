# Modelos 3D v2 · "Bloques de piedra antigua"

> ⚠️ Proyecto de fans **sin ánimo de lucro** y no oficial. *The Legend of Zelda* es una marca de Nintendo. Prohibida la venta de piezas o relojes. Ver [AVISO_LEGAL.md](../AVISO_LEGAL.md).

![Reloj de Hyrule v2](../imagenes/reloj_v2.png)

Carcasa **diseñada desde cero** como dos bloques de piedra antigua de Hyrule:

- **Losa superior:** la pantalla está en una ventana con una ranura tallada alrededor y rombos en las esquinas. Encima va la cresta con el logo.
- **Pedestal inferior:** más ancho, con juntas de piedra, una **Trifuerza dorada incrustada** y el **Wemos D1 mini dentro**.
- **Detalles de piedra:** esquinas desgastadas, grietas talladas y bordes achaflanados.
- **Dos patas de piedra:** inclinan el reloj **8° hacia atrás** para leerlo mejor desde la mesa.
- **Tapa trasera con 4 tornillos allen:** las cabezas quedan encajadas a ras. Lleva la Trifuerza dorada grande, el **puerto USB centrado justo debajo**, "RELOJ DE HYRULE" grabado con la fuente de píxeles del reloj y rejillas de ventilación.
- **La matriz LED, bien sujeta:** va en un marco que la centra y dos barras atornilladas la aprietan contra la ventana. Sirve para cualquier grosor de matriz.

**Medidas:** 168 mm de ancho, unos 157 mm de alto (con la cresta) y 46 mm de fondo. Peso aproximado: 185 g con la electrónica. Es muy estable: el centro de masas cae en mitad del apoyo.

---

## 1. Piezas

| # | Pieza | Archivo | Color recomendado | Cantidad |
|---|-------|---------|-------------------|----------|
| 1 | Carcasa (losa, pedestal, patas y cresta) | `cuerpo_piedra.stl` | **Gris piedra** (PLA gris, "piedra" o mármol) | 1 |
| 2 | Tapa trasera | `tapa_trasera.stl` | Gris piedra | 1 |
| 3 | Ventana difusora | `ventana_difusora.stl` | **Blanco** (deja pasar la luz de los LED) | 1 |
| 4 | Trifuerza dorada de la tapa | `trifuerza_dorada_trasera.stl` | Dorado silk | 1 |
| 5 | Trifuerza dorada del pedestal | `trifuerza_dorada_frontal.stl` | Dorado silk | 1 |
| 6 | Barras de sujeción de la matriz | `barras_sujecion_matriz.stl` | Cualquiera (no se ven) | 1 (2 barras) |
| 7 | Fondo del logo | `fondo_logo.stl` | Verde | 1 |
| 8 | Letras del logo | `logo_letras_geektoybox.stl` | Dorado silk | 1 (14 piezas) |

Ya no hace falta que la carcasa sea de color claro: la luz sale por la ventana difusora blanca, así que la piedra puede ser gris u oscura.

### Autores y licencias

| Archivo | Autor | Licencia |
|---------|-------|----------|
| `cuerpo_piedra.stl`, `tapa_trasera.stl`, `ventana_difusora.stl`, `barras_sujecion_matriz.stl`, Trifuerzas | **Diseño original** de este proyecto | CC BY-NC-SA 4.0 |
| `logo_letras_geektoybox.stl` | **geektoybox** ([Printables](https://www.printables.com/model/499505-legend-of-zelda-logo-text)), sin cambios | CC BY-NC 4.0 |
| `fondo_logo.stl` y la cresta | Hechos a partir del contorno del logo de geektoybox | CC BY-NC 4.0 |

Si compartes tu impresión (por ejemplo, un *make* en Printables), cita a geektoybox y este repositorio. **Prohibido el uso comercial.**

---

## 2. Tornillería y material extra

**Todos los tornillos del reloj son iguales:** M3 × 8 allen de cabeza cilíndrica (**DIN 912**), los típicos de las impresoras 3D, en acero inoxidable. No hay que medir nada.

![Esquema de tornillería](../imagenes/esquema_tornilleria.svg)

| Qué | Cantidad | Para qué |
|-----|----------|----------|
| **Tornillo M3 × 8 DIN 912** (allen, cabeza cilíndrica) | **8** | 4 cierran la tapa (A) y 4 fijan las barras de la matriz (B) |
| **Inserto roscado térmico M3**, largo 5,7 mm, Ø exterior 4,6 mm (para agujero de 4,0 mm) | **8** | Uno en cada torreta: rosca de metal duradera |
| Llave allen de 2,5 mm | 1 | |
| Trozos de filamento de 1,75 mm de unos 3,5 mm | 2 | Guías para centrar el fondo del logo |
| Pegamento (cianoacrilato en gel) y cola caliente | — | Logo, Trifuerzas y Wemos |

**Cómo trabaja cada tornillo:**
- **Tapa (A):** el tornillo atraviesa 1,7 mm de tapa y rosca los 5,7 mm del inserto. La cabeza queda encajada a ras en un cajeado de Ø6,5 × 3,3 mm.
- **Barras (B):** la barra se atornilla firme sobre una torreta alta, siempre a la misma altura. El tornillo atraviesa los 3 mm de la barra y rosca 5 mm del inserto. La matriz la sujetan las **lengüetas elásticas** de la barra, que se doblan lo necesario y la empujan contra la ventana. Valen para matrices de **9 a 14,5 mm** de grueso, desde la cara de los LED hasta la parte de atrás de la placa: así sirven para todos los módulos habituales.

Los insertos se colocan con el **soldador** (el mismo de los cables), con la punta a unos 200–220 °C: apoya el inserto en el agujero y empuja suavemente hasta que quede a ras.

---

## 3. Ajustes de impresión

Material: PLA, 1,75 mm. Ninguna pieza necesita soportes.

| Pieza | Orientación | Capa | Relleno | Notas |
|-------|-------------|------|---------|-------|
| Carcasa | **Boca abajo**: el frente (con la cresta) sobre la cama, tal como viene el STL | 0,2 mm | 15 % | Cama de 170 × 160 mm como mínimo. Las grietas, la ranura de la ventana y los chaflanes salen en la primera capa: usa una cama lisa. |
| Tapa trasera | **Boca abajo**: la cara con el texto sobre la cama, tal como viene | 0,2 mm | 20 % | 5 mm de grosor. El texto grabado y los cajeados de los tornillos quedan en la cara de la cama. |
| Ventana difusora | Plana | 0,2 mm | 100 % | 1 mm de grosor: deja pasar la luz difuminada. Para más contraste, prueba con 0,8 mm. |
| Barras de sujeción | Tal como vienen: cara plana sobre la cama y columnas hacia arriba | 0,2 mm | 100 % | Dos barras en el mismo archivo. Mejor en **PETG** (más elástico); en PLA también funcionan. |
| Trifuerzas doradas | Con la pestaña plana sobre la cama y los triángulos hacia arriba | 0,12–0,16 mm | 100 % | La cara de arriba de los triángulos es la que se ve. |
| Fondo del logo | Plano, con el rebaje de las letras hacia arriba | 0,2 mm | 100 % | |
| Letras del logo | Planas, tal como vienen | 0,12–0,16 mm | 100 % | 14 piezas sueltas; usa un *brim* fino si alguna se despega. |

**Toque mágico opcional:** pasa un pincel seco con pintura gris clara o blanca sobre la piedra. Resaltará las grietas, las juntas y los bordes, y parecerá piedra de verdad.

---

## 4. Montaje

![Despiece](../imagenes/reloj_v2_despiece.png)

1. **Insertos:** coloca los 8 insertos térmicos: 4 en las torretas de las esquinas, por detrás, y 4 en las torretas que hay a los lados de la ventana, para las barras.
2. **Trifuerzas:** mete la Trifuerza dorada pequeña por dentro del frente del pedestal y la grande por dentro de la tapa. La pestaña queda apoyada por dentro y los triángulos a ras por fuera. Fíjalas con unas gotas de pegamento. Hazlo antes de colocar el Wemos.
3. **Logo:** coloca las 14 letras en el rebaje del fondo verde y pégalas. Pon los 2 trozos de filamento en los agujeros de detrás del fondo, encájalos en los agujeros de la cresta y pega el fondo.
4. **Ventana:** coloca la ventana difusora por dentro, en el rebaje del frente.
5. **Matriz:** métela en su marco, con los LED hacia la ventana y el conector de entrada hacia cualquiera de los dos lados. Los cortes laterales del marco dejan paso a los pines y los cables.
6. **Barras:** pon las dos barras por detrás de la matriz, con las columnas de las lengüetas hacia la placa, y atorníllalas **hasta el fondo** sobre sus torretas con 4 tornillos M3 × 8. Las lengüetas se doblan solas y dejan la matriz firme contra la ventana.
7. **Wemos D1 mini:** suelda los 5 cables (ver el [esquema](../imagenes/esquema_conexiones.svg)). Apoya la placa en la cuna del pedestal, con el chip ESP hacia arriba y el USB hacia atrás, y deslízala hasta los dos topes delanteros. Un punto de cola caliente en los apoyos evita que se mueva.
8. **Cables:** conecta los cables a la **entrada (IN)** de la matriz. Hay sitio de sobra entre el pedestal y la matriz.
9. **Tapa:** ciérrala con los otros 4 tornillos M3 × 8 DIN 912. El conector USB del Wemos queda justo detrás del hueco de la tapa, debajo de la Trifuerza.

Si las letras de la pantalla salen giradas o desordenadas, no hace falta abrir: en la página del reloj, ve a **Pantalla** y cambia el giro o el orden de los módulos.

---

## 5. Generar las piezas tú mismo (opcional)

[`generar_modelos.py`](generar_modelos.py) crea todas las piezas desde cero. Solo necesita el logo original de geektoybox:

```
pip install trimesh manifold3d shapely numpy
python3 generar_modelos.py --logo LoZ_Logo_Text.stl
```

Al principio del archivo están todas las medidas (fondo, tamaño de la ventana, posición del Wemos, tornillos, inclinación...) por si quieres adaptarlo a otra matriz o placa.

---

## 6. Lista de comprobación

- [ ] 8 insertos térmicos colocados a ras
- [ ] Trifuerzas doradas pegadas (frente y tapa)
- [ ] Logo pegado en el fondo verde y el fondo en la cresta
- [ ] Ventana difusora en su rebaje
- [ ] Matriz en su marco, LED hacia delante, sujeta con las 2 barras (4 × M3 × 8)
- [ ] Wemos en la cuna, USB hacia atrás, cables a la entrada (IN) de la matriz
- [ ] Tapa cerrada con 4 tornillos M3 × 8 DIN 912

¡Que la Trifuerza te acompañe! ▲

---
*Homenaje de fans sin ánimo de lucro. No oficial y sin relación con Nintendo.*
