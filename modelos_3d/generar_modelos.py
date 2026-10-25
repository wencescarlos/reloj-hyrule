#!/usr/bin/env python3
# ===========================================================================
#  generar_modelos.py  -  Reloj de Hyrule v2.4 · "Bloques de piedra antigua"
#  Reloj de Hyrule · Proyecto de fans SIN ÁNIMO DE LUCRO · No oficial
#  CC BY-NC-SA 4.0 · https://github.com/wencescarlos/reloj-hyrule
#
#  Carcasa DISEÑADA DESDE CERO para:
#    - Wemos D1 mini (tumbado en el pedestal, USB hacia la tapa trasera)
#    - Matriz LED MAX7219 4 en 1 (128 x 32 mm) detrás de una ventana difusora
#
#  Lo único que viene de otro autor es el logo "Legend of Zelda Logo Text"
#  de geektoybox (CC BY-NC 4.0), que da forma a la cresta y al fondo verde:
#  https://www.printables.com/model/499505-legend-of-zelda-logo-text
#
#  Uso:
#     pip install trimesh manifold3d shapely numpy
#     python3 generar_modelos.py --logo LoZ_Logo_Text.stl
#
#  Genera, en la carpeta "piezas/":
#     cuerpo_piedra.stl              carcasa (losa + pedestal + patas + cresta)
#     tapa_trasera.stl               tapa con Trifuerza, puerto USB y 4 tornillos avellanados
#     tapa_trasera_zumbador.stl      la misma tapa con un aro para el zumbador opcional
#     barras_sujecion_matriz.stl     2 barras que aprietan la matriz contra la ventana
#     ventana_difusora.stl           placa blanca delante de los LED
#     trifuerza_dorada_trasera.stl   Trifuerza dorada de la tapa (30 mm)
#     trifuerza_dorada_frontal.stl   Trifuerza dorada del pedestal (15 mm)
#     soporte_wemos.stl              pinza que sujeta el Wemos (sin pegamento)
#     soporte_wemos_zumbador.stl     la misma pinza con un aro para el zumbador opcional
#     fondo_logo.stl                 fondo verde con el rebaje de las letras
#     logo_dos_colores.stl           opción B: fondo y letras en una sola pieza, para
#                                    imprimir en dos colores con una pausa a 2,0 mm
#
#  Tornillería: 10 tornillos iguales M3 x 6 autorroscantes, cabeza AVELLANADA
#  allen (hexágono de 2 mm), acero inoxidable. Sin insertos: roscan directamente
#  en agujeros guía de Ø2,5 mm. TODOS se atornillan desde atrás, en la misma
#  dirección (hacia el frente del reloj).
# ===========================================================================
import argparse, os, sys
import numpy as np
import trimesh
from shapely import affinity
from shapely.geometry import Point, Polygon, box, MultiPolygon, LineString
from shapely.ops import unary_union

# ------------------------------------------------------------------ medidas
# Marco del cuerpo: x horizontal, y hacia arriba, z hacia atrás (z=0 frente)
D        = 46.0      # fondo de la carcasa
PARED    = 3.0       # grosor de paredes
FRENTE   = 3.0       # grosor de la pared frontal
REBAJE   = 1.5       # escalón donde apoya la tapa trasera
TAPA     = 5.0       # grosor de la tapa trasera

LOSA_X, LOSA_Y0, LOSA_Y1 = 78.0, 28.0, 86.0      # bloque superior (±x)
PED_X,  PED_Y0,  PED_Y1  = 84.0, 0.0, 28.0       # pedestal (±x)
PATA_X0, PATA_X1, PATA_Y0 = 50.0, 80.0, -8.0     # patas (±x)
INCLINACION = 8.0                                # grados hacia atrás

VENT_C = (0.0, 57.0)          # centro de la ventana
VENT   = (127.0, 31.0)        # hueco visible (los LED ocupan 128 x 32)
# Alojamiento de la matriz: las placas 4 en 1 miden de 128 a 130 mm de largo
# (módulos de 32 a 32,5 mm) y de 32 a 32,5 de alto. Se deja holgura.
MATRIZ = (131.4, 33.4)
# La ventana difusora entra por el mismo hueco que la matriz y cae en un rebaje
# de 1 mm del frente; luego la matriz la aprieta contra la pared.
DIFUSOR = (MATRIZ[0] - 0.4, MATRIZ[1] - 0.4, 1.0)   # 131,0 x 33,0 x 1
MARCO_EXT = (MATRIZ[0] + 6.0, 41.0)                  # contorno exterior del marco
CHAFLAN = 2.4                 # chaflán del frente (borde de piedra)

# Wemos D1 mini (34,2 x 25,6 mm), tumbado, USB hacia atrás
D1_ANCHO, D1_LARGO = 25.6, 34.2
D1_PCB_Y = 9.0                # cara inferior de la placa (sobre las guías)
D1_PCB_GROSOR = 1.2
USB_SOBRESALE = 1.0
Z_TAPA_INT = D - TAPA         # cara interior de la tapa (41)

# ---- Tornillería: M3 x 6 autorroscante, cabeza avellanada allen (dk 6,5 · k 2,1 · llave 2)
# Regla común para los 10 tornillos:
#   0,3 mm de rebaje + 3,0 mm de pieza atravesada + ~3,3 mm de rosca en la torreta
TORN_L      = 6.0     # largo total (en los avellanados se mide con la cabeza)
AVELL_D     = 6.9     # boca del avellanado (cabeza Ø6,5 + holgura)
AVELL_REB   = 0.3     # la cabeza queda 0,3 mm hundida: nunca sobresale
PASO_D      = 3.3     # agujero de paso en la pieza que se atornilla
PIEZA_T     = 3.0     # grosor de pieza en cada tornillo
PILOTO_D    = 2.5     # agujero guía en la torreta: el tornillo talla su rosca
PILOTO_PROF = 6.0     # rosca 3,3 + 2,7 de margen para la punta
TORNILLOS = [(-69.0, 77.0), (69.0, 77.0), (-75.0, 9.0), (75.0, 9.0)]
BOSS_D, BOSS_H = 8.0, 1.85                # la torreta entra 1,85 mm en un bolsillo de la tapa
BOLSILLO_D, BOLSILLO_H = 8.6, TAPA - PIEZA_T   # bolsillo de Ø8,6 x 2 en la cara interior de la tapa
# Barras de sujeción de la matriz: se atornillan FIRMES sobre torretas altas y
# llevan lengüetas elásticas que empujan la matriz contra la ventana.
# Valen para matrices de 9 a 14,5 mm (cara de los LED -> trasera de la placa).
BARRAS_Y = (46.0, 68.0)                   # alturas de las dos barras
BARRA_X = 71.0                            # tornillos de las barras (±x)
MARCO_Z = 10.0                            # altura del marco que centra la matriz
H_MIN, H_MAX = 9.0, 14.5                  # grosores de matriz admitidos
ASIENTO_Z = FRENTE + H_MAX + 2.0          # las barras se apoyan aquí (19,5): nunca tocan la placa
BARRA_GROSOR = 3.0
LENGUETA_GROSOR = 1.4                     # lengüeta elástica
PUNTA_Z = FRENTE + H_MIN - 0.5            # hasta dónde llega la punta en reposo (11,5)

# Soporte del Wemos: pinza atornillada DESDE ATRÁS (igual que la tapa y las barras)
# a dos torretas a los lados de la cuna. Una viga elástica con una zapata aprieta
# el blindaje metálico del módulo ESP (zona plana, sin pines ni componentes).
SOP_X, SOP_Y = 19.0, 15.5                 # tornillos del soporte (±x, altura)
SOP_Z = 15.0                              # cara trasera de las torretas = apoyo del soporte
ESP_ARRIBA = 9.0 + 1.2 + 3.2              # cara de arriba del blindaje del ESP (13,4)
SOP_PRECARGA = 0.4                        # la zapata baja 0,4 mm más: aprieta al atornillar

def ext(poly, h, z0=0.0):
    geoms = [g for g in getattr(poly, "geoms", [poly]) if g.area > 1e-6]
    ms = [trimesh.creation.extrude_polygon(g, h) for g in geoms]
    m = trimesh.util.concatenate(ms) if len(ms) > 1 else ms[0]
    m.apply_translation([0, 0, z0])
    return m

def U(ms):  return trimesh.boolean.union(ms, engine="manifold")
def R(a, b):
    b = b if isinstance(b, list) else [b]
    return trimesh.boolean.difference([a] + b, engine="manifold")
def I(a, b): return trimesh.boolean.intersection([a, b], engine="manifold")

def rrect(cx, cy, w, h, r):
    return box(cx - w/2 + r, cy - h/2 + r, cx + w/2 - r, cy + h/2 - r).buffer(r, resolution=16)

def piedra(x0, y0, x1, y1, muescas):
    """Rectángulo con esquinas y bordes desgastados (muescas = lista de polígonos)."""
    p = box(x0, y0, x1, y1)
    for m in muescas:
        p = p.difference(Polygon(m))
    return p

def con_chaflan(contorno, alto, z0=0.0, chaflan=CHAFLAN, pasos=6):
    """Extruye con el borde delantero achaflanado (escalones de 0,4 mm)."""
    partes = []
    dz = chaflan / pasos
    for i in range(pasos):
        off = -chaflan + i * dz + dz / 2
        partes.append(ext(contorno.buffer(off, join_style=2), dz, z0 + i * dz))
    partes.append(ext(contorno, alto - chaflan, z0 + chaflan))
    return U(partes)

def prisma_yz(pts_yz, x0, x1):
    m = trimesh.creation.extrude_polygon(Polygon(pts_yz), x1 - x0)
    v = m.vertices.copy(); m.vertices = np.c_[v[:, 2] + x0, v[:, 0], v[:, 1]]; m.fix_normals()
    return m

def avellanado(x, y, z0=0.0, largo=PIEZA_T):
    """Agujero para tornillo de cabeza avellanada (90°). z0 = cara por la que entra la cabeza;
    el agujero avanza hacia +z."""
    r0, r1 = AVELL_D / 2, PASO_D / 2
    perfil = [(0, -1), (r0, -1), (r0, AVELL_REB), (r1, AVELL_REB + (r0 - r1)), (r1, largo + 1), (0, largo + 1)]
    m = trimesh.creation.revolve(perfil, sections=48)
    m.apply_translation([x, y, z0]); return m

def piloto(x, y, z_boca):
    """Agujero guía de Ø2,5 que entra en la torreta desde z_boca hacia el frente (-z),
    con un pequeño chaflán de entrada para que el tornillo arranque recto."""
    r = PILOTO_D / 2
    perfil = [(0, -PILOTO_PROF), (r, -PILOTO_PROF), (r, -0.4), (r + 0.4, 0), (r + 0.4, 1), (0, 1)]
    m = trimesh.creation.revolve(perfil, sections=32)
    m.apply_translation([x, y, z_boca]); return m

def triforce(cx, cy, alto, hueco):
    """Tres triángulos (punta hacia +y) separados por 'hueco'."""
    m = alto / np.sqrt(3)
    a = (cx, cy + alto/2); bi = (cx - m, cy - alto/2); bd = (cx + m, cy - alto/2)
    mi = ((a[0]+bi[0])/2, (a[1]+bi[1])/2); md = ((a[0]+bd[0])/2, (a[1]+bd[1])/2); mb = (cx, cy - alto/2)
    tri = [Polygon([a, mi, md]), Polygon([mi, bi, mb]), Polygon([md, mb, bd])]
    return unary_union([t.buffer(-hueco/2, join_style=2) for t in tri]), Polygon([a, bi, bd])

# ------------------------------------------------------------ formas 2D
LOSA = piedra(-LOSA_X, LOSA_Y0, LOSA_X, LOSA_Y1, [
    [(-78.1, 86.1), (-71, 86.1), (-78.1, 81)],                 # esquina sup. izq.
    [(78.1, 86.1), (74, 86.1), (78.1, 83.5)],                  # esquina sup. der.
    [(-30, 86.1), (-26.5, 84.6), (-23, 86.1)],                 # mella arriba
    [(78.1, 47), (76.6, 44), (78.1, 41)],                      # mella lateral der.
    [(-78.1, 36), (-76.2, 33.5), (-78.1, 31)],                 # mella lateral izq.
    [(-78.1, 28.0), (-74.5, 28.0), (-78.1, 30.5)],             # esquina inf. izq.
    [(78.1, 28.0), (76.0, 28.0), (78.1, 29.8)],                # esquina inf. der.
    [(38, 86.1), (40.5, 84.9), (44, 85.4), (46, 86.1)],        # mella arriba der.
    [(-78.1, 70), (-76.8, 66), (-77.2, 62), (-78.1, 60)],      # erosión lateral
])
PED = piedra(-PED_X, PED_Y0, PED_X, PED_Y1, [
    [(-84.1, 28.1), (-80, 28.1), (-84.1, 24)],
    [(84.1, 28.1), (81.5, 28.1), (84.1, 25.5)],
    [(-84.1, -0.1), (-81, -0.1), (-84.1, 2.5)],
    [(84.1, -0.1), (79.5, -0.1), (84.1, 4.0)],
    [(40, -0.1), (43, 1.6), (46, -0.1)],
    [(84.1, 14), (82.6, 12), (84.1, 9.5)],
    [(-84.1, 17), (-82.8, 14.5), (-84.1, 12)],
])
# Grietas talladas en la piedra (vistas de frente; se espejan al construir)
GRIETAS = [
    [(-74, 80), (-68, 76.5), (-66, 72), (-61, 69.5)],
    [(71, 37), (66, 40.5), (64.5, 44)],
    [(52, 84), (55, 80), (59.5, 78.5)],
    [(-70, 36), (-73, 33), (-72.5, 30)],
    [(-80, 20), (-74, 17), (-70, 18.5), (-66, 14), (-62, 13)],
    [(78, 6), (72, 9.5), (69, 8), (64, 11)],
    [(14, 3.5), (18, 6), (17, 9)],
]
def pata(lado):
    s = 1 if lado > 0 else -1
    x0, x1 = sorted([s * PATA_X0, s * PATA_X1])
    m = [[(x0 - .1, PATA_Y0 - .1), (x0 + 3, PATA_Y0 - .1), (x0 - .1, PATA_Y0 + 3)],
         [(x1 + .1, PATA_Y0 - .1), (x1 - 3, PATA_Y0 - .1), (x1 + .1, PATA_Y0 + 3)]]
    return piedra(x0, PATA_Y0, x1, 1.0, m)
CONTORNO = unary_union([LOSA, PED])              # silueta del cuerpo (sin patas)
LIMPIO = unary_union([box(-LOSA_X, LOSA_Y0, LOSA_X, LOSA_Y1), box(-PED_X, PED_Y0, PED_X, PED_Y1)])
# El hueco interior y el rebaje de la tapa respetan las mellas: pared mínima de 2 mm
INTERIOR = LIMPIO.buffer(-PARED, join_style=2).intersection(CONTORNO.buffer(-2.0, join_style=2))
TAPA_2D = LIMPIO.buffer(-REBAJE, join_style=2).intersection(CONTORNO.buffer(-1.2, join_style=2))
TAPA_2D = TAPA_2D.buffer(-0.2, join_style=2)      # holgura

# ------------------------------------------------------------ logo / cresta
def contorno_logo(logo):
    arriba = logo.face_normals[:, 2] > 0.5
    tris = logo.vertices[logo.faces[arriba]][:, :, :2]
    L = unary_union([Polygon(t) for t in tris]).buffer(0.02).buffer(-0.02).simplify(0.02)
    minx, miny, maxx, _ = L.bounds
    return affinity.translate(L, -(minx + maxx) / 2, -miny)

def cresta_y_fondo(L0, sep=6.0):
    fondo = L0.buffer(3.0, join_style=1).buffer(4, join_style=1).buffer(-4, join_style=1)
    cresta = L0.buffer(7.0, join_style=1).buffer(6, join_style=1).buffer(-6, join_style=1)
    banda = box(-70, -sep - 4, 70, -sep + 6).buffer(-2.5, join_style=1).buffer(2.5, join_style=1)
    cresta = unary_union([cresta, banda]).buffer(10, join_style=1).buffer(-10, join_style=1)
    cresta = cresta.intersection(box(-80, -sep - 3.0, 80, 300))
    libre = fondo.buffer(-2.2).difference(L0.buffer(1.6))
    cands = [p for p in (Point(x, y) for x in np.arange(-70, 70, .5) for y in np.arange(-5, 60, .5)) if libre.contains(p)]
    guias = [(p.x, p.y) for p in (min(cands, key=lambda p: p.x), max(cands, key=lambda p: p.x))]
    return cresta, fondo, guias

# ------------------------------------------------------------ texto en píxeles
def texto_pixeles(texto, cx, cy, px):
    """Texto con la fuente de píxeles del firmware (para grabar en la tapa)."""
    sys.path.insert(0, os.path.join(os.path.dirname(os.path.abspath(__file__)), "..", "firmware", "herramientas"))
    from arte import FUENTE
    filas = []
    for ch in texto.upper():
        g = FUENTE.get(ch, FUENTE[' ']) if ch != ' ' else ["..."] * 7
        filas.append(g)
    ancho = sum(len(g[0]) + 1 for g in filas) - 1
    x = cx - ancho * px / 2
    cuadros = []
    for g in filas:
        for fy, fila in enumerate(g):
            for fx, c in enumerate(fila):
                if c == '#':
                    X = x + fx * px; Y = cy + (3 - fy) * px
                    cuadros.append(box(X - 0.06, Y - px/2 - 0.06, X + px + 0.06, Y + px/2 + 0.06))
        x += (len(g[0]) + 1) * px
    return unary_union(cuadros).buffer(0.01).buffer(-0.01)

# ======================================================================
def cuerpo(L0):
    piezas = [con_chaflan(LOSA, D), con_chaflan(PED, D), con_chaflan(pata(1), D), con_chaflan(pata(-1), D)]
    c = U(piezas)
    # Hueco interior y escalón de la tapa
    c = R(c, [ext(INTERIOR, D - FRENTE + 1, FRENTE),
              ext(TAPA_2D.buffer(0.2, join_style=2), TAPA + 1, Z_TAPA_INT)])
    # Ventana, rebaje del difusor y alojamiento de la matriz
    vx, vy = VENT_C
    c = R(c, [ext(rrect(vx, vy, *VENT, 1.0), FRENTE + 2, -1),
              ext(rrect(vx, vy, MATRIZ[0], MATRIZ[1], 0.5), DIFUSOR[2] + 0.01, FRENTE - DIFUSOR[2])])   # rebaje del difusor
    mw, mh = MATRIZ
    marco = rrect(vx, vy, *MARCO_EXT, 1).difference(rrect(vx, vy, mw, mh, 0.5))
    marco = marco.difference(box(vx - mw/2 - 8, vy - 7, vx - mw/2 + 1, vy + 7))   # paso de pines y cables
    marco = marco.difference(box(vx + mw/2 - 1, vy - 7, vx + mw/2 + 8, vy + 7))
    rebaje = rrect(vx, vy, MATRIZ[0], MATRIZ[1], 0.5)
    piezas = [c, ext(marco, MARCO_Z - FRENTE, FRENTE), ext(marco.difference(rebaje.buffer(0.01)), 0.8, FRENTE - 0.5)]   # el marco se apoya en la pared
    # Cresta del logo
    if L0 is not None:
        cresta, _, guias = cresta_y_fondo(L0)
        cr = affinity.affine_transform(cresta, [-1, 0, 0, 1, 0, LOSA_Y1 + 6.0])
        piezas.append(ext(cr, 4.0))
        piezas.append(prisma_yz([(LOSA_Y1 - 1, 3.5), (LOSA_Y1 - 1, 9), (LOSA_Y1 + 6, 3.5)], -60, 60))
        for xc in (-48, 0, 48):
            piezas.append(prisma_yz([(LOSA_Y1 - 1, 3.5), (LOSA_Y1 - 1, 24), (LOSA_Y1 + 22, 3.5)], xc - 1.5, xc + 1.5))
    # Torretas para los insertos de los tornillos
    for (x, y) in TORNILLOS:
        sx = np.sign(x)
        lado_x = sx * (LOSA_X if y > 40 else PED_X)
        lado_y = LOSA_Y1 if y > 40 else PED_Y0
        torre = unary_union([Point(x, y).buffer(4.8, resolution=24),
                             box(min(x, lado_x), min(y, lado_y), max(x, lado_x), max(y, lado_y))]).intersection(CONTORNO)
        piezas.append(ext(torre, Z_TAPA_INT - FRENTE, FRENTE))     # desde el frente: se imprime sin soportes
        piezas.append(ext(Point(x, y).buffer(BOSS_D / 2, resolution=32), BOSS_H, Z_TAPA_INT))   # centra la tapa
    # Torretas para las barras que sujetan la matriz
    for yb_ in BARRAS_Y:
        for sx in (1, -1):
            t2 = unary_union([Point(sx * BARRA_X, yb_).buffer(4.2, resolution=24),
                              box(min(sx * BARRA_X, sx * (LOSA_X - 1)), yb_ - 4.2, max(sx * BARRA_X, sx * (LOSA_X - 1)), yb_ + 4.2)])
            piezas.append(ext(t2.intersection(CONTORNO), ASIENTO_Z - FRENTE + 0.5, FRENTE - 0.5))
    # Cuna del Wemos D1 mini (guías, apoyos y tope delantero)
    a2 = D1_ANCHO / 2
    z_fin = Z_TAPA_INT - USB_SOBRESALE                       # borde trasero de la placa
    z_ini = z_fin - D1_LARGO                                 # borde delantero
    yb = PED_Y0 + PARED
    largo = z_fin - z_ini + 0.5
    for s in (1, -1):
        apoyo = box(min(s*(a2-0.8), s*(a2+0.3)), yb, max(s*(a2-0.8), s*(a2+0.3)), D1_PCB_Y)       # apoyo bajo el borde
        guia  = box(min(s*(a2+0.3), s*(a2+1.9)), yb, max(s*(a2+0.3), s*(a2+1.9)), D1_PCB_Y + 4.0) # pared lateral
        piezas += [ext(apoyo, largo, z_ini), ext(guia, largo, z_ini)]
    for s_ in (1, -1):   # tope delantero: dos postes en las esquinas de la placa
        piezas.append(ext(box(min(s_*10.6, s_*(a2+0.3)), yb, max(s_*10.6, s_*(a2+0.3)), D1_PCB_Y + D1_PCB_GROSOR + 1.0),
                          z_ini - 0.6 - FRENTE + 0.01, FRENTE))
    for s_ in (1, -1):   # torretas del soporte del Wemos
        t3 = unary_union([Point(s_ * SOP_X, SOP_Y).buffer(4.5, resolution=32),
                          box(s_ * SOP_X - 4.5, PED_Y0 + 1, s_ * SOP_X + 4.5, SOP_Y)])
        piezas.append(ext(t3, SOP_Z - FRENTE + 0.5, FRENTE - 0.5))
    c = U(piezas)
    if L0 is not None:
        for (u, v) in guias:   # agujeros para las guías de filamento del fondo del logo
            c = R(c, ext(Point(-u, LOSA_Y1 + 6.0 + v).buffer(0.95, resolution=24), 2.6 + 1, -1))
    # Agujeros guía de Ø2,5 para los tornillos autorroscantes (todos en la misma dirección)
    guias_t = [(sx * BARRA_X, yb_, ASIENTO_Z) for yb_ in BARRAS_Y for sx in (1, -1)]
    guias_t += [(x, y, Z_TAPA_INT + BOSS_H) for (x, y) in TORNILLOS]
    guias_t += [(sx * SOP_X, SOP_Y, SOP_Z) for sx in (1, -1)]
    c = R(c, [piloto(x, y, z) for (x, y, z) in guias_t])
    # Trifuerza dorada del frente (en el pedestal)
    hueco, _ = triforce(0, 14.0, 15.0, 0.9)
    c = R(c, ext(hueco, FRENTE + 2, -1))
    # Ranura tallada alrededor de la ventana y juntas del pedestal (grabadas 0,8 mm)
    ranura = rrect(vx, vy, VENT[0] + 12, VENT[1] + 12, 3).difference(rrect(vx, vy, VENT[0] + 9.6, VENT[1] + 9.6, 2))
    for sx in (-1, 1):
        for sy in (-1, 1):
            ranura = ranura.union(Polygon([(vx + sx*(VENT[0]/2+7.5), vy + sy*(VENT[1]/2+3)), (vx + sx*(VENT[0]/2+10.5), vy + sy*(VENT[1]/2+6)),
                                           (vx + sx*(VENT[0]/2+7.5), vy + sy*(VENT[1]/2+9)), (vx + sx*(VENT[0]/2+4.5), vy + sy*(VENT[1]/2+6))]))
    juntas = unary_union([box(-30.6, PED_Y0, -29.4, PED_Y1), box(29.4, PED_Y0, 30.6, PED_Y1)])
    grietas = unary_union([LineString([(-x, y) for (x, y) in g]).buffer(0.45, cap_style=1, join_style=1) for g in GRIETAS])
    c = R(c, ext(unary_union([ranura, juntas, grietas]), 0.8 + 1, -1))
    # Inclinación: las patas se cortan para que el reloj mire 8° hacia arriba
    t = np.tan(np.radians(INCLINACION))
    corte = prisma_yz([(PATA_Y0 - 50, -10), (PATA_Y0 - 50, D + 10), (PATA_Y0 + t * (D + 10), D + 10), (PATA_Y0 - t * 10, -10)], -200, 200)
    c = R(c, corte)
    return c

# Zumbador opcional (pasivo de 12 mm, como el "speaker" de las placas base):
# va en un aro por dentro de la tapa, justo detrás de una rejilla, para que se oiga.
ZUMB_X, ZUMB_Y = 49.0, 40.0          # en coordenadas de la tapa (impresa boca abajo)
ZUMB_D, ZUMB_ALTO = 12.4, 6.0         # hueco para el zumbador y altura del aro

def aro_zumbador():
    exterior = Point(ZUMB_X, ZUMB_Y).buffer(ZUMB_D / 2 + 1.6, resolution=64)
    hueco = Point(ZUMB_X, ZUMB_Y).buffer(ZUMB_D / 2, resolution=64)
    nervios = unary_union([Point(ZUMB_X + (ZUMB_D / 2) * np.cos(a), ZUMB_Y + (ZUMB_D / 2) * np.sin(a)).buffer(0.45, resolution=16)
                           for a in np.radians([90, 210, 330])])          # tres nervios: entra a presión
    aro = exterior.difference(hueco).union(nervios.intersection(exterior))
    ranura = box(ZUMB_X - ZUMB_D / 2 - 3, ZUMB_Y - 1.6, ZUMB_X - ZUMB_D / 2 + 0.5, ZUMB_Y + 1.6)  # paso de los cables
    base = ext(aro, 2.0, TAPA)
    alto = ext(aro.difference(ranura), ZUMB_ALTO - 2.0, TAPA + 2.0)
    return U([base, alto])

def tapa(con_zumbador=False):
    # La tapa se imprime boca abajo y luego se le da la vuelta (giro sobre el eje vertical):
    # por eso su silueta y su texto se dibujan en espejo respecto al cuerpo.
    espejo = lambda g: affinity.scale(g, xfact=-1, yfact=1, origin=(0, 0))
    p = con_chaflan(espejo(TAPA_2D), TAPA, 0.0, chaflan=0.6, pasos=2)        # z=0: cara exterior
    agujeros = []
    for (x, y) in TORNILLOS:
        xm = -x   # coordenadas en espejo (la tapa se imprime boca abajo)
        agujeros.append(avellanado(xm, y, 0.0, TAPA))                        # cabeza avellanada a ras
        agujeros.append(ext(Point(xm, y).buffer(BOLSILLO_D / 2, resolution=48), BOLSILLO_H + 1, PIEZA_T))  # bolsillo de la torreta
    # Puerto USB (rebaje exterior + hueco) justo debajo de la Trifuerza
    yusb = D1_PCB_Y + D1_PCB_GROSOR / 2 + 1.5
    agujeros.append(ext(rrect(0, yusb, 12.0, 10.0, 2.0), TAPA + 2, -1))
    agujeros.append(ext(rrect(0, yusb, 14.5, 12.5, 2.5), 3.5 + 1, -1))   # el conector entra 3,5 mm
    # Trifuerza grande
    hueco, _ = triforce(0, 40.0, 30.0, 1.6)
    agujeros.append(ext(hueco, TAPA + 2, -1))
    # Texto grabado (0,6 mm) y pequeñas rejillas de ventilación
    # La tapa se imprime boca abajo y luego se gira: el texto se graba en espejo
    txt = affinity.scale(texto_pixeles("RELOJ DE HYRULE", 0, 70.0, 1.15), xfact=-1, yfact=1, origin=(0, 0))
    agujeros.append(ext(txt, 0.6 + 1, -1))
    for s in (-1, 1):
        for k in range(4):
            agujeros.append(ext(rrect(s * (40 + k * 6), 40, 2.2, 16, 1.1), TAPA + 2, -1))
    p = R(p, agujeros)
    if con_zumbador:
        p = U([p, aro_zumbador()])
    return p      # z=0 es la cara exterior: se imprime así, boca abajo

def trifuerza_dorada(alto, hueco, holgura=0.15, grosor=TAPA, pestana=0.6, margen=1.4):
    h, grande = triforce(0, 0, alto, hueco)
    pieza = U([ext(grande.buffer(margen, join_style=1), pestana),
               ext(h.buffer(-holgura, join_style=2), grosor, pestana)])
    return pieza

def barras():
    """Dos barras con 4 lengüetas elásticas cada una.
    Se imprimen con la cara trasera sobre la cama (z=0); las columnas apuntan a la matriz."""
    T, t = BARRA_GROSOR, LENGUETA_GROSOR
    alto_col = (ASIENTO_Z + T - PUNTA_Z) - t          # columna desde la lengüeta hasta la punta
    piezas = []
    for k in range(2):
        cuerpo2d = rrect(0, 0, 2 * (LOSA_X - PARED - 1.2), 7.0, 2.0)
        hueco2d, lengs, cols = [], [], []
        for cx in (-48, -16, 16, 48):
            s_ = 1 if cx > 0 else -1                      # raíz hacia fuera, punta hacia el centro
            x0, x1 = sorted([cx - s_ * 13.0, cx + s_ * 13.0])
            cuerpo2d = cuerpo2d.union(rrect(cx, 4.0, 30.0, 11.0, 1.5))      # ensanche hacia el centro de la matriz
            lengua = box(x0, 2.0, x1, 7.0)
            ranura = box(x0 - (0 if s_ > 0 else 1.0), 1.0, x1 + (1.0 if s_ > 0 else 0), 8.0).difference(
                     box(x0 - 2 if s_ > 0 else x0, 2.0, x1 if s_ > 0 else x1 + 2, 7.0))
            hueco2d.append(ranura)
            lengs.append(lengua)
            px = x1 - 2.5 if s_ > 0 else x0 + 2.5
            cols.append(box(px - 2.5, 2.0, px + 2.5, 7.0))
        b = ext(cuerpo2d, T)
        b = R(b, [ext(unary_union(hueco2d), T + 2, -1), ext(unary_union(lengs), T, t)])   # ranuras y lengüeta fina
        b = U([b, ext(unary_union(cols), alto_col, t)])
        b = R(b, [avellanado(sx * BARRA_X, 0, 0.0, T) for sx in (1, -1)])   # cabeza avellanada por detrás
        if k == 1:   # la barra de arriba: girada 180° (las lengüetas miran hacia abajo, al centro)
            b.apply_transform(np.array([[-1, 0, 0, 0], [0, -1, 0, 0], [0, 0, 1, 0], [0, 0, 0, 1.]]))
        b.apply_translation([0, k * 26.0, 0])
        piezas.append(b)
    return trimesh.util.concatenate(piezas)

def soporte_wemos(con_zumbador=False):
    """Pinza del Wemos. Se apoya en la cara trasera de sus dos torretas y se atornilla
    desde atrás con 2 tornillos M3 x 6 (igual que la tapa). Una viga fina de borde a borde
    lleva en el centro una zapata que aprieta el blindaje del ESP: al atornillar, la viga
    se curva 0,4 mm y deja la placa inmovilizada sin pegamento.
    Coordenadas: x, y como el cuerpo; z=0 es la cara trasera (la de las cabezas) y se
    imprime sobre la cama."""
    T = PIEZA_T
    orejas = unary_union([Point(s * SOP_X, SOP_Y).buffer(4.5, resolution=48) for s in (1, -1)])
    larguero = rrect(0, SOP_Y + 6.0, 2 * SOP_X + 4.0, 5.0, 1.5)                  # arriba, rígido
    viga = box(-SOP_X, SOP_Y - 1.2, SOP_X, SOP_Y)                                 # viga elástica (1,2 mm)
    y_zapata = ESP_ARRIBA - SOP_PRECARGA
    zapata = rrect(0, (y_zapata + SOP_Y - 0.6) / 2, 9.0, (SOP_Y - 0.6) - y_zapata, 0.5)   # zapata de 9 mm
    montantes = unary_union([box(s * SOP_X - 3.0, SOP_Y, s * SOP_X + 3.0, SOP_Y + 6.0) for s in (1, -1)])
    plano = unary_union([orejas, larguero, viga, zapata, montantes])
    # ventana con la Trifuerza en el larguero (detalle)
    tri, _ = triforce(0, SOP_Y + 6.2, 4.2, 0.5)
    p = ext(plano, T)
    p = R(p, [ext(tri, T + 2, -1)] + [avellanado(s * SOP_X, SOP_Y, 0.0, T) for s in (1, -1)])
    if con_zumbador:
        p = U([p, aro_soporte()])
        p = R(p, ext(Point(0, SZ_Y).buffer(ZUMB_D / 2 - 0.01, resolution=64), T + 2, -1))   # se mete por detrás
    return p

# Variante del soporte con un aro para el zumbador encima del larguero.
# El aro sale hacia el frente del reloj (se imprime hacia arriba, sin soportes);
# el zumbador se mete a presión por detrás, a través del soporte.
SZ_Y = SOP_Y + 6.0 + 2.5 + ZUMB_D / 2 + 1.6 - 1.0     # centro del aro (justo encima del larguero)
SZ_FONDO = 7.5                                          # largo total del aro (soporte incluido): no llega al marco de la matriz

def aro_soporte():
    exterior = Point(0, SZ_Y).buffer(ZUMB_D / 2 + 1.6, resolution=64)
    hueco = Point(0, SZ_Y).buffer(ZUMB_D / 2, resolution=64)
    nervios = unary_union([Point((ZUMB_D / 2) * np.cos(a), SZ_Y + (ZUMB_D / 2) * np.sin(a)).buffer(0.45, resolution=16)
                           for a in np.radians([30, 150, 270])])
    aro = exterior.difference(hueco).union(nervios.intersection(exterior))
    union_larguero = box(-5.0, SOP_Y + 6.0, 5.0, SZ_Y)                 # pie que lo une al larguero
    ranura = box(ZUMB_D / 2 - 0.5, SZ_Y - 1.6, ZUMB_D / 2 + 3, SZ_Y + 1.6)   # salida de los cables
    base = ext(unary_union([aro, union_larguero.difference(hueco)]), PIEZA_T)
    resto = ext(aro.difference(ranura), SZ_FONDO - PIEZA_T, PIEZA_T)
    return U([base, resto])

def difusor():
    return ext(rrect(0, 0, *DIFUSOR[:2], 0.5), DIFUSOR[2])

def fondo_logo(L0):
    _, fondo, guias = cresta_y_fondo(L0)
    p = ext(fondo, 1.6)
    p = R(p, ext(L0.buffer(0.12, join_style=2), 1.5, 1.1))
    for (u, v) in guias:
        p = R(p, ext(Point(u, v).buffer(0.95, resolution=24), 1.1 + 1, -1))
    return p

LOGO_PAUSA_Z = 2.0     # altura del fondo: aquí se cambia del verde al dorado
LOGO_ALTO    = 3.5     # altura total (las letras sobresalen 1,5 mm)

def logo_dos_colores(L0, logo):
    """Opción B del logo: una sola pieza para imprimir en dos colores con una pausa.
    De 0 a 2,0 mm es el fondo verde (el mismo de fondo_logo.stl, más alto, con sus dos
    agujeros guía); de 2,0 a 3,5 mm sobresalen las letras de geektoybox, que se imprimen
    en dorado tras cambiar el filamento."""
    fondo = fondo_logo(L0)
    fondo.apply_transform(np.diag([1, 1, LOGO_PAUSA_Z / fondo.bounds[1][2], 1]))
    mn, mx = logo.bounds
    letras = logo.copy()
    letras.apply_translation([-(mn[0] + mx[0]) / 2, -mn[1], -mn[2]])
    letras.apply_transform(np.diag([1, 1, LOGO_ALTO / (mx[2] - mn[2]), 1]))
    return U([fondo, letras])

def main():
    a = argparse.ArgumentParser()
    a.add_argument("--logo", required=True, help="LoZ_Logo_Text.stl de geektoybox")
    a.add_argument("--salida", default="piezas")
    args = a.parse_args()
    os.makedirs(args.salida, exist_ok=True)
    logo = trimesh.load(args.logo)
    L0 = contorno_logo(logo)
    out = lambda m, n: (m.export(os.path.join(args.salida, n)), print(f"  → {n}  estanca={m.is_watertight}  {np.round(m.extents,1)}"))
    c = cuerpo(L0); out(c, "cuerpo_piedra.stl")
    out(tapa(), "tapa_trasera.stl")
    out(tapa(con_zumbador=True), "tapa_trasera_zumbador.stl")
    out(trifuerza_dorada(30.0, 1.6), "trifuerza_dorada_trasera.stl")
    out(trifuerza_dorada(15.0, 0.9, grosor=FRENTE), "trifuerza_dorada_frontal.stl")
    out(difusor(), "ventana_difusora.stl")
    out(barras(), "barras_sujecion_matriz.stl")
    out(soporte_wemos(), "soporte_wemos.stl")
    out(soporte_wemos(con_zumbador=True), "soporte_wemos_zumbador.stl")
    out(fondo_logo(L0), "fondo_logo.stl")
    out(logo_dos_colores(L0, logo), "logo_dos_colores.stl")

if __name__ == "__main__":
    main()
