#!/usr/bin/env python3
# ===========================================================================
#  generar_modelos.py  -  Reloj de Hyrule v2 · "Bloques de piedra antigua"
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
#     tapa_trasera.stl               tapa con Trifuerza, puerto USB y 4 tornillos
#     ventana_difusora.stl           placa blanca delante de los LED
#     trifuerza_dorada_trasera.stl   Trifuerza dorada de la tapa (30 mm)
#     trifuerza_dorada_frontal.stl   Trifuerza dorada del pedestal (15 mm)
#     fondo_logo.stl                 fondo verde con el rebaje de las letras
#
#  Tornillería: 4 x M3 x 8 avellanados + 4 insertos térmicos M3 (Ø4,0 mm).
# ===========================================================================
import argparse, os, sys
import numpy as np
import trimesh
from shapely import affinity
from shapely.geometry import Point, Polygon, box, MultiPolygon, LineString
from shapely.ops import unary_union

# ------------------------------------------------------------------ medidas
# Marco del cuerpo: x horizontal, y hacia arriba, z hacia atrás (z=0 frente)
D        = 44.0      # fondo de la carcasa
PARED    = 3.0       # grosor de paredes
FRENTE   = 3.0       # grosor de la pared frontal
REBAJE   = 1.5       # escalón donde apoya la tapa trasera
TAPA     = 3.0       # grosor de la tapa trasera

LOSA_X, LOSA_Y0, LOSA_Y1 = 78.0, 28.0, 86.0      # bloque superior (±x)
PED_X,  PED_Y0,  PED_Y1  = 84.0, 0.0, 28.0       # pedestal (±x)
PATA_X0, PATA_X1, PATA_Y0 = 50.0, 80.0, -8.0     # patas (±x)
INCLINACION = 8.0                                # grados hacia atrás

VENT_C = (0.0, 57.0)          # centro de la ventana
VENT   = (127.0, 31.0)        # hueco visible (la matriz es 128 x 32)
DIFUSOR = (133.0, 37.0, 1.0)  # placa difusora
MATRIZ = (128.8, 32.8)        # alojamiento de la matriz
CHAFLAN = 2.4                 # chaflán del frente (borde de piedra)

# Wemos D1 mini (34,2 x 25,6 mm), tumbado, USB hacia atrás
D1_ANCHO, D1_LARGO = 25.6, 34.2
D1_PCB_Y = 9.0                # cara inferior de la placa (sobre las guías)
D1_PCB_GROSOR = 1.2
USB_SOBRESALE = 1.0
Z_TAPA_INT = D - TAPA         # cara interior de la tapa (41)

# Tornillos: 4 x M3 x 8 avellanados + insertos térmicos M3 (agujero 4,0)
TORNILLOS = [(-69.0, 77.0), (69.0, 77.0), (-75.0, 9.0), (75.0, 9.0)]
INSERTO_D, INSERTO_PROF = 4.0, 8.0

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
    [(-56, 28.1), (-52.5, 26.6), (-49, 28.1)],
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
INTERIOR = unary_union([box(-LOSA_X + PARED, LOSA_Y0 - 1, LOSA_X - PARED, LOSA_Y1 - PARED),
                        box(-PED_X + PARED, PED_Y0 + PARED, PED_X - PARED, PED_Y1 + 1)])
TAPA_2D = unary_union([box(-LOSA_X + REBAJE, LOSA_Y0, LOSA_X - REBAJE, LOSA_Y1 - REBAJE),
                       box(-PED_X + REBAJE, PED_Y0 + REBAJE, PED_X - REBAJE, PED_Y1 + 0.01)])
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
              ext(rrect(vx, vy, DIFUSOR[0] + 0.4, DIFUSOR[1] + 0.4, 1.0), DIFUSOR[2] + 0.01, FRENTE - DIFUSOR[2])])
    mw, mh = MATRIZ
    marco = rrect(vx, vy, DIFUSOR[0] + 4.0, DIFUSOR[1] + 4.0, 1).difference(rrect(vx, vy, mw, mh, 0.5))
    marco = marco.difference(box(vx - mw/2 - 3, vy - 7, vx - mw/2 + 1, vy + 7))   # paso de cables
    marco = marco.difference(box(vx + mw/2 - 1, vy - 7, vx + mw/2 + 3, vy + 7))
    rebaje = rrect(vx, vy, DIFUSOR[0] + 0.4, DIFUSOR[1] + 0.4, 1.0)
    piezas = [c, ext(marco, 9.0, FRENTE), ext(marco.difference(rebaje.buffer(0.01)), 0.8, FRENTE - 0.5)]   # el marco se apoya en la pared
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
    piezas.append(ext(box(-a2 + 1, yb, a2 - 1, D1_PCB_Y + D1_PCB_GROSOR + 1.0), z_ini - 0.6 - FRENTE + 0.01, FRENTE))
    c = U(piezas)
    if L0 is not None:
        for (u, v) in guias:   # agujeros para las guías de filamento del fondo del logo
            c = R(c, ext(Point(-u, LOSA_Y1 + 6.0 + v).buffer(0.95, resolution=24), 2.6 + 1, -1))
    for (x, y) in TORNILLOS:   # agujeros para los insertos térmicos M3
        c = R(c, ext(Point(x, y).buffer(INSERTO_D / 2, resolution=32), INSERTO_PROF + 1, Z_TAPA_INT - INSERTO_PROF))
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

def tapa():
    p = con_chaflan(TAPA_2D, TAPA, 0.0, chaflan=0.6, pasos=2)        # z=0: cara exterior
    agujeros = []
    for (x, y) in TORNILLOS:
        # agujero avellanado (90°) para tornillo M3 de cabeza plana, en una sola pieza
        perfil = [(0, -1), (3.55, -1), (3.55, 0), (1.7, 1.85), (1.7, TAPA + 1), (0, TAPA + 1)]
        av = trimesh.creation.revolve(perfil, sections=48)
        av.apply_translation([x, y, 0]); agujeros.append(av)
    # Puerto USB (rebaje exterior + hueco) justo debajo de la Trifuerza
    yusb = D1_PCB_Y + D1_PCB_GROSOR / 2 + 1.5
    agujeros.append(ext(rrect(0, yusb, 12.0, 10.0, 2.0), TAPA + 2, -1))
    agujeros.append(ext(rrect(0, yusb, 14.5, 12.5, 2.5), 1.2 + 1, -1))
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
    return p      # z=0 es la cara exterior: se imprime así, boca abajo

def trifuerza_dorada(alto, hueco, holgura=0.15, grosor=TAPA, pestana=0.6, margen=1.4):
    h, grande = triforce(0, 0, alto, hueco)
    pieza = U([ext(grande.buffer(margen, join_style=1), pestana),
               ext(h.buffer(-holgura, join_style=2), grosor, pestana)])
    return pieza

def difusor():
    return ext(rrect(0, 0, *DIFUSOR[:2], 1.0), DIFUSOR[2])

def fondo_logo(L0):
    _, fondo, guias = cresta_y_fondo(L0)
    p = ext(fondo, 1.6)
    p = R(p, ext(L0.buffer(0.12, join_style=2), 1.5, 1.1))
    for (u, v) in guias:
        p = R(p, ext(Point(u, v).buffer(0.95, resolution=24), 1.1 + 1, -1))
    return p

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
    out(trifuerza_dorada(30.0, 1.6), "trifuerza_dorada_trasera.stl")
    out(trifuerza_dorada(15.0, 0.9, grosor=FRENTE), "trifuerza_dorada_frontal.stl")
    out(difusor(), "ventana_difusora.stl")
    out(fondo_logo(L0), "fondo_logo.stl")

if __name__ == "__main__":
    main()
