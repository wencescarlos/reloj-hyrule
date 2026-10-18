# Hace la captura del portal de configuración (en español y en inglés)
# Reloj de Hyrule · Proyecto de fans SIN ÁNIMO DE LUCRO · No oficial
# CC BY-NC-SA 4.0 · https://github.com/wencescarlos/reloj-hyrule
#
# Abre la página del reloj (pagina_web.h) en un navegador sin pantalla, con
# datos de ejemplo en lugar de un reloj de verdad, y la parte en 3 columnas.
# Uso (necesita Playwright y Pillow):   python3 captura_portal.py
#   imagenes/portal_configuracion.png      (español)
#   imagenes/en/portal_configuracion.png   (inglés)
import io, json, os, re
from PIL import Image
from playwright.sync_api import sync_playwright

AQUI = os.path.dirname(os.path.abspath(__file__))
RAIZ = os.path.normpath(os.path.join(AQUI, '..', '..', '..'))
FUENTE = os.path.join(RAIZ, 'firmware', 'RelojHyrule', 'pagina_web.h')

ANCHO = 400          # ancho de un móvil
COLUMNAS = 3
SEPARACION = 20
FONDO = (11, 26, 17)

AVENTURAS = {
    'es': ["Paseo por Hyrule", "El cofre del tesoro", "Combate contra monstruo", "La Trifuerza", "El hada",
           "Contenedores de corazón", "¡Bomba!", "Lluvia de rupias", "La espada legendaria", "Persecución",
           "Frase de ánimo"],
    'en': ["A walk in Hyrule", "The treasure chest", "Monster battle", "The Triforce", "The fairy",
           "Heart containers", "Bomb!", "Rupee rain", "The legendary sword", "The chase", "Words of courage"],
}
ESTADO = {
    'es': {"fecha": "DOMINGO 18 DE OCTUBRE", "actualizacion": "Tienes la última versión (1.1.0)."},
    'en': {"fecha": "SUNDAY, OCTOBER 18", "actualizacion": "You have the latest version (1.1.0)."},
}


def pagina():
    s = open(FUENTE, encoding='utf-8').read()
    return re.search(r'R"PAGINA\((.*)\)PAGINA"', s, re.S).group(1)


def ajustes(idioma):
    return {
        "idioma": 1 if idioma == 'en' else 0, "nombre": "Alejandro", "cumpleDia": 25, "cumpleMes": 10,
        "mensaje": "¡Te queremos mucho, {N}!" if idioma == 'es' else "We love you so much, {N}!",
        "saludos": 1, "formato24": 1, "ceroIzquierda": 1, "modoFecha": 2, "fechaCadaMin": 10,
        "modoSegundos": 2, "parpadeo": 1, "horaEnPunto": 1, "animCadaMin": 10, "frases": 1,
        "nocheActiva": 1, "nocheIniH": 21, "nocheIniM": 30, "nocheFinH": 7, "nocheFinM": 30,
        "nocheApagar": 0, "brilloNoche": 0, "brillo": 2, "velocidad": 35, "rotacion": 0,
        "invertirModulos": 0, "espejo": 0, "zona": "CET-1CEST,M3.5.0,M10.5.0/3",
        "servidorHora": "es.pool.ntp.org", "wifiNombre": "MiCasa",
        "urlActualizacion": "https://wencescarlos.github.io/reloj-hyrule/", "autoActualizar": 1,
    }


def estado(idioma):
    e = {"version": "1.1.0", "hora": "20:45:12", "sincronizado": True, "portal": False, "wifi": "MiCasa",
         "conectado": True, "senal": -58, "ip": "192.168.1.42", "diasCumple": 7}
    e.update(ESTADO[idioma])
    return e


def capturar(navegador, idioma):
    html = pagina()
    datos = {'/api/ajustes': ajustes(idioma), '/api/estado': estado(idioma), '/api/aventuras': AVENTURAS[idioma]}
    p = navegador.new_page(viewport={'width': ANCHO, 'height': 900})

    def responder(ruta):
        url = ruta.request.url
        camino = '/' + url.split('/', 3)[3].split('?')[0]
        if camino == '/':
            ruta.fulfill(status=200, content_type='text/html; charset=utf-8', body=html)
        elif camino in datos and ruta.request.method == 'GET':
            ruta.fulfill(status=200, content_type='application/json', body=json.dumps(datos[camino]))
        else:
            ruta.fulfill(status=200, content_type='application/json', body='{"ok":true}')

    p.route('**/*', responder)
    p.goto('http://reloj-hyrule.local/')
    p.wait_for_timeout(800)
    imagen = Image.open(io.BytesIO(p.screenshot(full_page=True))).convert('RGB')
    p.close()
    return imagen


def columnas(imagen):
    alto = -(-imagen.height // COLUMNAS)
    salida = Image.new('RGB', (COLUMNAS * ANCHO + (COLUMNAS - 1) * SEPARACION, alto), FONDO)
    for c in range(COLUMNAS):
        trozo = imagen.crop((0, c * alto, ANCHO, min(imagen.height, (c + 1) * alto)))
        salida.paste(trozo, (c * (ANCHO + SEPARACION), 0))
    return salida


def main():
    with sync_playwright() as pw:
        navegador = pw.chromium.launch()
        for idioma, carpeta in [('es', 'imagenes'), ('en', os.path.join('imagenes', 'en'))]:
            os.makedirs(os.path.join(RAIZ, carpeta), exist_ok=True)
            destino = os.path.join(RAIZ, carpeta, 'portal_configuracion.png')
            columnas(capturar(navegador, idioma)).save(destino, optimize=True)
            print(destino)
        navegador.close()


if __name__ == '__main__':
    main()
