# Genera RelojHyrule/fuente.h y RelojHyrule/sprites.h a partir de arte.py
# Uso: python3 generar.py
import sys; import os; AQUI=os.path.dirname(os.path.abspath(__file__)); sys.path.insert(0,AQUI)
from arte import *
OUT=os.path.join(AQUI,'..','RelojHyrule','')
def cols(rows, h):
    w=len(rows[0]); out=[]
    for x in range(w):
        v=0
        for y in range(h):
            if y < len(rows) and rows[y][x]=='#': v|=1<<y
        out.append(v)
    return out
# ---- fuente ----
glifos=[]  # (codigo, filas8)
for ch,rows in FUENTE.items(): glifos.append((ord(ch), ["."*len(rows[0])]+rows))
CODIGOS={'Á':128,'É':129,'Í':130,'Ó':131,'Ú':132,'Ü':133,'Ñ':134,'¡':135,'¿':136,'°':137}
for ch,(b,a) in ACENTOS.items():
    w=len(FUENTE[b][0]); a=a[:w].ljust(w,'.')
    if w==3: a=".#."
    glifos.append((CODIGOS[ch],[a]+FUENTE[b]))
for ch,rows in EXTRA.items():
    code=CODIGOS.get(ch, ord(ch))
    glifos.append((code,["."*len(rows[0])]+rows))
glifos.sort()
datos=[];idx=[];anch=[];mapa=[255]*256
for i,(code,rows) in enumerate(glifos):
    c=cols(rows,8); mapa[code]=i; idx.append(len(datos)); anch.append(len(c)); datos+=c
def arr(v,per=16,fmt='0x%02X'):
    return ',\n  '.join(', '.join(fmt%x for x in v[i:i+per]) for i in range(0,len(v),per))
f=open(OUT+'fuente.h','w')
f.write('''// ===========================================================================
//  fuente.h  -  Fuente propia del Reloj de Hyrule (5x7 proporcional + tildes)
//  Reloj de Hyrule · Proyecto de fans SIN ÁNIMO DE LUCRO · No oficial
//  CC BY-NC-SA 4.0 · https://github.com/wencescarlos/reloj-hyrule
//  Cada glifo se guarda por columnas: bit 0 = fila de arriba (tildes),
//  bits 1..7 = cuerpo de la letra. Archivo generado a partir del dibujo
//  de las letras; no hace falta tocarlo a mano.
// ===========================================================================
#pragma once
#include <Arduino.h>

// Códigos internos de los caracteres especiales (además del ASCII)
#define CAR_CORAZON   0x01
#define CAR_RUPIA     0x02
#define CAR_TRIFUERZA 0x03
#define CAR_DESTELLO  0x04
#define CAR_A_TILDE   128
#define CAR_E_TILDE   129
#define CAR_I_TILDE   130
#define CAR_O_TILDE   131
#define CAR_U_TILDE   132
#define CAR_U_DIERESIS 133
#define CAR_ENIE      134
#define CAR_ABRE_EXCL 135
#define CAR_ABRE_INTERR 136
#define CAR_GRADO     137

''')
f.write('const uint8_t FUENTE_DATOS[] PROGMEM = {\n  %s\n};\n\n'%arr(datos))
f.write('const uint16_t FUENTE_INICIO[] PROGMEM = {\n  %s\n};\n\n'%arr(idx,12,'%d'))
f.write('const uint8_t FUENTE_ANCHO[] PROGMEM = {\n  %s\n};\n\n'%arr(anch,16,'%d'))
f.write('// Tabla código -> número de glifo (255 = no existe)\nconst uint8_t FUENTE_MAPA[256] PROGMEM = {\n  %s\n};\n\n'%arr(mapa,16,'%d'))
pa=[]
for k in ['A','P']: pa+=cols(PEQUE[k],5)
f.write('// Letras pequeñas 3x5 para el indicador AM / PM\nconst uint8_t MINI_A[3] PROGMEM = {%s};\nconst uint8_t MINI_P[3] PROGMEM = {%s};\n'%(', '.join('0x%02X'%x for x in pa[:3]),', '.join('0x%02X'%x for x in pa[3:])))
f.close()
# ---- sprites ----
f=open(OUT+'sprites.h','w')
f.write('''// ===========================================================================
//  sprites.h  -  Dibujos originales en píxeles del Reloj de Hyrule
//  Reloj de Hyrule · Proyecto de fans SIN ÁNIMO DE LUCRO · No oficial
//  CC BY-NC-SA 4.0 · https://github.com/wencescarlos/reloj-hyrule
//  Cada sprite se guarda por columnas (bit 0 = fila de arriba), hasta 16 filas.
//  Homenaje sin ánimo de lucro: dibujos propios, hechos píxel a píxel.
// ===========================================================================
#pragma once
#include <Arduino.h>

struct Sprite {
  uint8_t ancho;
  uint8_t alto;
  const uint16_t* columnas;   // en PROGMEM
};

''')
for k,rows in SPRITES.items():
    c=cols(rows,len(rows))
    f.write('const uint16_t SPR_%s_DATOS[] PROGMEM = { %s };\n'%(k.upper(),', '.join('0x%04X'%x for x in c)))
    f.write('const Sprite SPR_%s = { %d, %d, SPR_%s_DATOS };\n\n'%(k.upper(),len(rows[0]),len(rows),k.upper()))
f.close()
print('glifos',len(glifos),'bytes',len(datos))
