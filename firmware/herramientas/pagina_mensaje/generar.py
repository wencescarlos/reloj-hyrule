#!/usr/bin/env python3
# ===========================================================================
#  generar.py  -  Crea firmware/RelojHyrule/pagina_mensaje.h
#  Reloj de Hyrule · Proyecto de fans SIN ÁNIMO DE LUCRO · No oficial
#  CC BY-NC-SA 4.0 · https://github.com/wencescarlos/reloj-hyrule
#
#  Junta en un solo archivo para el firmware:
#    - la página del móvil (mensaje.html), con la fuente y los dibujos del
#      reloj copiados de fuente.h y sprites.h para la vista previa;
#    - el icono de la pantalla de inicio (PNG de 180 x 180);
#    - el manifiesto web.
#  Uso (necesita Pillow):   python3 generar.py
# ===========================================================================
import io, json, os, re
from PIL import Image, ImageDraw, ImageFilter

AQUI = os.path.dirname(os.path.abspath(__file__))
FW = os.path.join(AQUI, '..', '..', 'RelojHyrule')

def numeros(texto, nombre):
    m = re.search(nombre + r'\[[^\]]*\]\s*PROGMEM\s*=\s*\{(.*?)\};', texto, re.S)
    return [int(x, 0) for x in re.findall(r'0x[0-9A-Fa-f]+|\d+', m.group(1))]

def fuente():
    f = open(os.path.join(FW, 'fuente.h'), encoding='utf-8').read()
    datos, inicio, ancho, mapa = (numeros(f, n) for n in ('FUENTE_DATOS', 'FUENTE_INICIO', 'FUENTE_ANCHO', 'FUENTE_MAPA'))
    r = {}
    for codigo, g in enumerate(mapa):
        if g != 255:
            r[codigo] = datos[inicio[g]:inicio[g] + ancho[g]]
    return r

def sprites():
    f = open(os.path.join(FW, 'sprites.h'), encoding='utf-8').read()
    r = {}
    for nombre in ('HEROE_TESORO', 'HADA_A', 'COFRE_ABIERTO', 'CORAZON', 'TRIANGULO', 'BOMBA'):
        r[nombre] = numeros(f, 'SPR_' + nombre + '_DATOS')
    return r

def icono():
    """Trifuerza dorada hecha de LED sobre fondo verde oscuro (180 x 180)."""
    from PIL import ImageChops
    T = 180
    img = Image.new('RGB', (T, T), (12, 30, 20))
    d = ImageDraw.Draw(img)
    for r in range(150, 0, -2):                      # luz suave desde arriba
        c = int(40 * (1 - r / 150))
        d.ellipse([T / 2 - r, 50 - r, T / 2 + r, 50 + r], fill=(12 + c // 2, 30 + c, 20 + c // 2))
    # 12 filas de LED en triángulo: la fila f tiene f + 1 LED. Por debajo de la
    # fila 6 se quita el hueco central y quedan las tres piezas de la Trifuerza.
    paso, filas = 11.5, 12
    alto = (filas - 1) * paso * 0.866
    y0 = (T - alto) / 2 - 4
    brillo = Image.new('RGB', (T, T), (0, 0, 0))
    db = ImageDraw.Draw(brillo)
    for f in range(filas):
        for k in range(f + 1):
            if f >= filas // 2 and f - filas // 2 < k < filas // 2:
                continue                                  # hueco central
            x = T / 2 + (k - f / 2) * paso
            y = y0 + f * paso * 0.866
            db.ellipse([x - 7, y - 7, x + 7, y + 7], fill=(230, 150, 30))
            d.ellipse([x - 4.4, y - 4.4, x + 4.4, y + 4.4], fill=(255, 216, 96))
    img = ImageChops.add(img, brillo.filter(ImageFilter.GaussianBlur(5)), scale=1.5)
    buf = io.BytesIO()
    img.quantize(64).save(buf, 'PNG', optimize=True)
    return buf.getvalue()

def main():
    html = open(os.path.join(AQUI, 'mensaje.html'), encoding='utf-8').read()
    html = html.replace('/*FUENTE*/', json.dumps(fuente(), separators=(',', ':')))
    html = html.replace('/*SPRITES*/', json.dumps(sprites(), separators=(',', ':')))
    assert ')PAGINA"' not in html
    png = icono()
    open(os.path.join(AQUI, 'icono.png'), 'wb').write(png)
    manifiesto = json.dumps({
        "name": "Reloj de Hyrule", "short_name": "Hyrule", "start_url": "/mensaje", "display": "standalone",
        "background_color": "#0f2418", "theme_color": "#0f2418",
        "icons": [{"src": "/icono.png", "sizes": "180x180", "type": "image/png"}]}, ensure_ascii=False)
    filas = [', '.join(f'0x{b:02X}' for b in png[i:i + 20]) for i in range(0, len(png), 20)]
    salida = f'''// ===========================================================================
//  pagina_mensaje.h  -  Página del móvil para mandar mensajes (/mensaje)
//  Reloj de Hyrule · Proyecto de fans SIN ÁNIMO DE LUCRO · No oficial
//  CC BY-NC-SA 4.0 · https://github.com/wencescarlos/reloj-hyrule
//  ARCHIVO GENERADO por firmware/herramientas/pagina_mensaje/generar.py
//  a partir de mensaje.html, fuente.h y sprites.h. No lo edites a mano.
// ===========================================================================
#pragma once
#include <Arduino.h>

const char PAGINA_MENSAJE[] PROGMEM = R"PAGINA({html})PAGINA";

const char MANIFIESTO[] PROGMEM = R"JSON({manifiesto})JSON";

// Icono para la pantalla de inicio (PNG 180 x 180, {len(png)} bytes)
const uint8_t ICONO_PNG[] PROGMEM = {{
  {(',' + chr(10) + '  ').join(filas)}
}};
'''
    open(os.path.join(FW, 'pagina_mensaje.h'), 'w', encoding='utf-8').write(salida)
    print(f'pagina_mensaje.h: página {len(html)} bytes, icono {len(png)} bytes')

if __name__ == '__main__':
    main()
