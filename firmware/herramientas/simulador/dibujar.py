import sys, os
from PIL import Image, ImageDraw, ImageFont
S=10
def cuadro(cols, escala=S, brillo=2):
    im=Image.new('RGB',(32*escala+escala,8*escala+escala),(18,10,8)); d=ImageDraw.Draw(im)
    for x in range(32):
        for y in range(8):
            on=(cols[x]>>y)&1
            c=(255,70,40) if on else (48,22,18)
            o=escala//2
            d.ellipse([o+x*escala+1,o+y*escala+1,o+x*escala+escala-1,o+y*escala+escala-1],fill=c)
    return im
def leer(p):
    fr=[]
    for l in open(p):
        v=l.split(); fr.append((int(v[0]),int(v[1]),[int(h,16) for h in v[2:]]))
    return fr
def hoja(nombre, n=18, escala=6):
    fr=leer(f'out/{nombre}.txt'); idx=[int(i*(len(fr)-1)/(n-1)) for i in range(n)] if len(fr)>1 else [0]
    ims=[cuadro(fr[i][2],escala) for i in idx]; w,h=ims[0].size; cols=3
    out=Image.new('RGB',(cols*(w+8),((len(ims)+cols-1)//cols)*(h+8)+20),(0,0,0)); d=ImageDraw.Draw(out); d.text((4,4),nombre,fill=(255,255,255))
    for k,im in enumerate(ims): out.paste(im,((k%cols)*(w+8),20+(k//cols)*(h+8)))
    return out
def gif(nombre, destino, escala=12, maxdur=None):
    fr=leer(f'out/{nombre}.txt'); ims=[];durs=[]
    for i,(t,b,c) in enumerate(fr):
        nt=fr[i+1][0] if i+1<len(fr) else t+1500
        ims.append(cuadro(c,escala).convert('P',palette=Image.ADAPTIVE,colors=8)); durs.append(max(20,nt-t))
    ims[0].save(destino,save_all=True,append_images=ims[1:],duration=durs,loop=0,optimize=True)
if __name__=='__main__':
    if sys.argv[1]=='hojas':
        for grupo,nombres in [('a',['reloj','reloj12','fecha','arranque','0','1']),('b',['2','3','4','5','6','7']),('c',['8','9','10','cumple','noches','hora'])]:
            hs=[hoja(n) for n in nombres]; W=max(h.size[0] for h in hs); H=sum(h.size[1] for h in hs)
            # two columns
            half=(len(hs)+1)//2; col1=hs[:half]; col2=hs[half:]
            H=max(sum(h.size[1] for h in col1),sum(h.size[1] for h in col2))
            out=Image.new('RGB',(2*W+20,H),(0,0,0)); y=0
            for h in col1: out.paste(h,(0,y)); y+=h.size[1]
            y=0
            for h in col2: out.paste(h,(W+20,y)); y+=h.size[1]
            out.save(f'hoja_{grupo}.png')
