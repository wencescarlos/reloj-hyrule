#!/usr/bin/env python3
# ===========================================================================
#  generar_modelos.py  -  Genera las piezas 3D del Reloj de Hyrule
#  Reloj de Hyrule · Proyecto de fans SIN ÁNIMO DE LUCRO · No oficial
#  CC BY-NC-SA 4.0 · https://github.com/wencescarlos/reloj-hyrule
#
#  Las piezas ya generadas están en esta carpeta. Este script sirve para
#  crearlas de nuevo a partir de los originales de sus autores
#  (ambos con licencia CC BY-NC 4.0):
#
#   - Caja "LED Marquee Scroller" de David Payne (Qrome):
#       https://www.printables.com/model/25374-esp8266-wemos-clock-news-weather-scrolling-marquee
#       (también: https://www.thingiverse.com/thing:2867294)
#       (archivos led_marque_base_1.stl y led_marque_plate_1.stl)
#   - "Legend of Zelda Logo Text" de geektoybox:
#       https://www.printables.com/model/499505-legend-of-zelda-logo-text
#       (archivo LoZ_Logo_Text.stl)
#
#  Uso:
#     pip install trimesh manifold3d shapely numpy
#     python3 generar_modelos.py --base led_marque_base_1.stl \
#         --tapa led_marque_plate_1.stl --logo LoZ_Logo_Text.stl
#
#  Genera, en la carpeta "piezas/":
#     base_reloj_zelda.stl   base con la cresta para el logo
#     fondo_logo.stl         fondo del logo con el rebaje de las letras
#     tapa_trifuerza.stl     tapa trasera con la Trifuerza recortada
#     trifuerza_dorada.stl   los tres triángulos dorados (pieza original)
#  Las letras se imprimen con el archivo original del logo, tal cual.
#
#  Sin ánimo de lucro. Cita siempre a Qrome y a geektoybox.
#  Prohibido el uso comercial de los modelos y de sus remezclas.
# ===========================================================================
import argparse
import os
import sys

import numpy as np
import trimesh
from shapely import affinity
from shapely.geometry import Point, Polygon, box
from shapely.ops import unary_union

# --- Medidas de la caja original (base) ------------------------------------
CX = (-65.4 + 83.0) / 2      # centro de la base en x
TOP = -33.62                 # canto superior de la base (con el reloj de pie)
GAP = 5.0                    # separación entre el logo y el canto
T = 3.2                      # grosor de la cresta
BASE_MEDIDAS = (148.4, 54.4, 40.0)
TAPA_MEDIDAS = (145.0, 39.9, 8.0)
LOGO_MEDIDAS = (126.2, 50.0, 1.3)

# --- Trifuerza de la tapa ---------------------------------------------------
TRI_CX, TRI_CY, TRI_ALTO = 8.5, -10.95, 30.0
TRI_SEPARACION = 0.9         # mitad del hueco entre triángulos
TRI_HOLGURA = 0.15           # holgura de la pieza dorada
TAPA_GROSOR = 2.0
PESTANA = 0.6                # pestaña interior de la pieza dorada


def extruir(poligono, alto, z0=0.0):
    partes = [trimesh.creation.extrude_polygon(p, alto)
              for p in getattr(poligono, "geoms", [poligono])]
    m = trimesh.util.concatenate(partes) if len(partes) > 1 else partes[0]
    m.apply_translation([0, 0, z0])
    return m


def union(mallas):
    return trimesh.boolean.union(mallas, engine="manifold")


def resta(a, b):
    return trimesh.boolean.difference([a, b], engine="manifold")


def prisma_yz(puntos_yz, x0, x1):
    """Perfil en el plano (y, z) extruido a lo largo de x."""
    m = trimesh.creation.extrude_polygon(Polygon(puntos_yz), x1 - x0)
    v = m.vertices.copy()
    m.vertices = np.c_[v[:, 2] + x0, v[:, 0], v[:, 1]]
    m.fix_normals()
    return m


def comprobar(nombre, malla, esperado, tolerancia=1.0):
    ext = malla.extents
    if np.any(np.abs(np.sort(ext) - np.sort(esperado)) > tolerancia):
        print(f"  ⚠️  {nombre}: medidas {np.round(ext, 1)} distintas de las esperadas "
              f"{esperado}. ¿Es el archivo correcto?")
    else:
        print(f"  ✓ {nombre}: medidas correctas {np.round(ext, 1)}")


def contorno_logo(logo):
    arriba = logo.face_normals[:, 2] > 0.5
    triangulos = logo.vertices[logo.faces[arriba]][:, :, :2]
    L = unary_union([Polygon(t) for t in triangulos]).buffer(0.02).buffer(-0.02)
    L = L.simplify(0.02)
    minx, miny, maxx, _ = L.bounds
    return affinity.translate(L, -(minx + maxx) / 2, -miny)


def generar_base_y_fondo(base, logo, salida):
    L0 = contorno_logo(logo)

    fondo = L0.buffer(3.0, join_style=1).buffer(4, join_style=1).buffer(-4, join_style=1)
    cresta = L0.buffer(7.0, join_style=1).buffer(6, join_style=1).buffer(-6, join_style=1)
    banda = box(-74.2, -GAP - 3, 74.2, -GAP + 6).buffer(-2.5, join_style=1).buffer(2.5, join_style=1)
    cresta = unary_union([cresta, banda]).buffer(10, join_style=1).buffer(-10, join_style=1)
    cresta = cresta.intersection(box(-74.2, -GAP - 1.0, 74.2, 200))

    # Guías (trozos de filamento de 1,75 mm): dentro del fondo y lejos de las letras
    libre = fondo.buffer(-2.2).difference(L0.buffer(1.6))
    candidatos = [p for p in (Point(x, y) for x in np.arange(-70, 70, 0.5)
                              for y in np.arange(-5, 60, 0.5)) if libre.contains(p)]
    guias = [(p.x, p.y) for p in (min(candidatos, key=lambda p: p.x),
                                  max(candidatos, key=lambda p: p.x))]

    # Base + cresta (vista frontal -> marco de la base: x espejado)
    cresta_b = affinity.affine_transform(cresta, [-1, 0, 0, 1, CX, TOP + GAP])
    partes = [base, extruir(cresta_b, T),
              prisma_yz([(TOP - 1, T - 0.5), (TOP - 1, T + 3), (TOP + 5, T - 0.5)], -63.0, 81.0)]
    for xc in (-48, CX, 66):   # escuadras de refuerzo
        partes.append(prisma_yz([(TOP - 1, T - 0.5), (TOP - 1, T + 12), (TOP + 16, T - 0.5)],
                                xc - 1.5, xc + 1.5))
    resultado = union(partes)
    for (u, v) in guias:
        c = trimesh.creation.cylinder(radius=0.95, height=5.2, sections=32)
        c.apply_translation([CX - u, TOP + GAP + v, 0])
        resultado = resta(resultado, c)
    resultado.export(os.path.join(salida, "base_reloj_zelda.stl"))
    print("  → base_reloj_zelda.stl")

    # Fondo: 1,6 mm, rebaje de las letras de 0,5 mm y agujeros de las guías
    pieza = extruir(fondo, 1.6)
    pieza = resta(pieza, extruir(L0.buffer(0.12, join_style=2), 1.5, 1.1))
    for (u, v) in guias:
        c = trimesh.creation.cylinder(radius=0.95, height=2.2, sections=32)
        c.apply_translation([u, v, 0])
        pieza = resta(pieza, c)
    pieza.export(os.path.join(salida, "fondo_logo.stl"))
    print("  → fondo_logo.stl")


def triangulos_trifuerza():
    media = TRI_ALTO / np.sqrt(3)
    a = (TRI_CX, TRI_CY - TRI_ALTO / 2)            # punta (arriba con el reloj de pie)
    bi = (TRI_CX - media, TRI_CY + TRI_ALTO / 2)
    bd = (TRI_CX + media, TRI_CY + TRI_ALTO / 2)
    mi = ((a[0] + bi[0]) / 2, (a[1] + bi[1]) / 2)
    md = ((a[0] + bd[0]) / 2, (a[1] + bd[1]) / 2)
    mb = (TRI_CX, TRI_CY + TRI_ALTO / 2)
    tri = [Polygon([a, mi, md]), Polygon([mi, bi, mb]), Polygon([md, mb, bd])]
    huecos = unary_union([t.buffer(-TRI_SEPARACION, join_style=2) for t in tri])
    return huecos, Polygon([a, bi, bd])


def generar_tapa(tapa, salida):
    huecos, _ = triangulos_trifuerza()
    relleno = extruir(box(-6, -22.5, 21.5, 2.0), TAPA_GROSOR)     # tapa la rejilla central
    resultado = resta(union([tapa, relleno]), extruir(huecos, 4, -1))
    resultado.export(os.path.join(salida, "tapa_trifuerza.stl"))
    print("  → tapa_trifuerza.stl")


def generar_trifuerza_dorada(salida):
    huecos, grande = triangulos_trifuerza()
    pestana = grande.buffer(1.6, join_style=1)
    pieza = union([extruir(pestana, PESTANA),
                   extruir(huecos.buffer(-TRI_HOLGURA, join_style=2), TAPA_GROSOR, PESTANA)])
    pieza.apply_translation([-TRI_CX, -TRI_CY, 0])
    pieza.export(os.path.join(salida, "trifuerza_dorada.stl"))
    print("  → trifuerza_dorada.stl")


def main():
    p = argparse.ArgumentParser(description="Genera las piezas del Reloj de Hyrule")
    p.add_argument("--base", help="led_marque_base_1.stl (caja de Qrome)")
    p.add_argument("--tapa", help="led_marque_plate_1.stl (caja de Qrome)")
    p.add_argument("--logo", help="LoZ_Logo_Text.stl (logo de geektoybox)")
    p.add_argument("--salida", default="piezas", help="carpeta de salida (por defecto: piezas)")
    a = p.parse_args()
    os.makedirs(a.salida, exist_ok=True)

    print("Reloj de Hyrule · generador de piezas (uso personal, sin ánimo de lucro)")
    generar_trifuerza_dorada(a.salida)

    if a.tapa:
        tapa = trimesh.load(a.tapa)
        comprobar("tapa", tapa, TAPA_MEDIDAS)
        generar_tapa(tapa, a.salida)
    if a.base and a.logo:
        base, logo = trimesh.load(a.base), trimesh.load(a.logo)
        comprobar("base", base, BASE_MEDIDAS)
        comprobar("logo", logo, LOGO_MEDIDAS, tolerancia=0.5)
        generar_base_y_fondo(base, logo, a.salida)
    elif a.base or a.logo:
        print("  ⚠️  Para la base con cresta hacen falta --base y --logo a la vez.")
    if not (a.base or a.tapa or a.logo):
        print("  (Sin archivos originales solo se genera la Trifuerza dorada. Usa --help.)")
    print(f"Listo. Piezas en: {os.path.abspath(a.salida)}")


if __name__ == "__main__":
    sys.exit(main())
