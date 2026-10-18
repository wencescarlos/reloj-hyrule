# Graba las animaciones de imagenes/ con el simulador (en español y en inglés)
# Reloj de Hyrule · Proyecto de fans SIN ÁNIMO DE LUCRO · No oficial
# CC BY-NC-SA 4.0 · https://github.com/wencescarlos/reloj-hyrule
#
# Uso (necesita g++ y Pillow):   python3 grabar_gifs.py
#   imagenes/      -> animaciones en español
#   imagenes/en/   -> animaciones en inglés (README.en.md)
import os, subprocess, sys, tempfile
from PIL import Image

AQUI = os.path.dirname(os.path.abspath(__file__))
RAIZ = os.path.normpath(os.path.join(AQUI, '..', '..', '..'))
sys.path.insert(0, AQUI)
from dibujar import cuadro

ESCALA = 10      # 32 x 8 LED -> 330 x 90 píxeles
SEMILLA = 7      # misma semilla = mismas animaciones cada vez
FUENTES = ['ajustes', 'pantalla', 'animaciones', 'esfera', 'tiempo']

# Archivo -> escenas del simulador ("reloj", "cumple" o el número de la aventura)
GIFS = {
    'DEMO.gif':               ['relojcorto', '1', 'relojcorto', '2', '3', 'relojcorto', '6'],
    '02_cofre_del_tesoro.gif':['1'],
    '03_combate.gif':         ['2'],
    '04_trifuerza.gif':       ['3'],
    'cumpleanos.gif':         ['cumple'],
    'reloj_24h.gif':          ['reloj'],
    'reloj_12h_ampm.gif':     ['reloj12'],
}
SOLO_ESPANOL = {'reloj_24h.gif', 'reloj_12h_ampm.gif'}   # no tienen texto


def compilar(destino):
    rel = os.path.join(RAIZ, 'firmware', 'RelojHyrule')
    cmd = ['g++', '-O1', '-std=c++17', '-I', AQUI, '-include', 'Arduino.h',
           os.path.join(AQUI, 'sim.cpp')] + [os.path.join(rel, f + '.cpp') for f in FUENTES] + ['-o', destino]
    subprocess.run(cmd, check=True)


def escena(sim, nombre, idioma, carpeta, semilla):
    salida = os.path.join(carpeta, f'{nombre}_{idioma}_{semilla}.txt')
    subprocess.run([sim, nombre, str(semilla), salida, idioma], check=True, stdout=subprocess.DEVNULL)
    cuadros = []
    for linea in open(salida):
        v = linea.split()
        cuadros.append((int(v[0]), [int(h, 16) for h in v[2:]]))
    # Duración de cada cuadro hasta el siguiente (el último, 100 ms en el reloj o 1,5 s)
    final = 100 if nombre.startswith('reloj') else 1500
    return [(c, max(20, (cuadros[i + 1][0] if i + 1 < len(cuadros) else t + final) - t))
            for i, (t, c) in enumerate(cuadros)]


def main():
    with tempfile.TemporaryDirectory() as tmp:
        sim = os.path.join(tmp, 'sim')
        compilar(sim)
        for idioma, carpeta in [('es', 'imagenes'), ('en', os.path.join('imagenes', 'en'))]:
            os.makedirs(os.path.join(RAIZ, carpeta), exist_ok=True)
            for archivo, escenas in GIFS.items():
                if idioma == 'en' and archivo in SOLO_ESPANOL:
                    continue
                cuadros = []
                for k, nombre in enumerate(escenas):
                    cuadros += escena(sim, nombre, idioma, tmp, SEMILLA + k)
                # Cuadros iguales seguidos se juntan en uno (GIF más ligero)
                juntos = []
                for c, d in cuadros:
                    if juntos and juntos[-1][0] == c: juntos[-1] = (c, juntos[-1][1] + d)
                    else: juntos.append((c, d))
                cuadros = juntos
                imagenes = [cuadro(c, ESCALA).convert('P', palette=Image.ADAPTIVE, colors=8) for c, _ in cuadros]
                destino = os.path.join(RAIZ, carpeta, archivo)
                imagenes[0].save(destino, save_all=True, append_images=imagenes[1:],
                                 duration=[d for _, d in cuadros], loop=0, optimize=True)
                print(f'{carpeta}/{archivo}: {len(cuadros)} cuadros')


if __name__ == '__main__':
    main()
