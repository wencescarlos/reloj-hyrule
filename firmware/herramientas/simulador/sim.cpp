// Simulador: imita el MAX7219 a partir de los bytes que enviaría el SPI y guarda cada cuadro
#include "Arduino.h"
#include "SPI.h"
#include "LittleFS.h"
#include "../../RelojHyrule/ajustes.h"
#include "../../RelojHyrule/pantalla.h"
#include "../../RelojHyrule/animaciones.h"
#include "../../RelojHyrule/esfera.h"
#include "../../RelojHyrule/tiempo.h"
#include <vector>
uint32_t sim_ms=0; SerialSim Serial; SPISim SPI; FSSim LittleFS;
void configTime(const char* tz,const char*,const char*,const char*){setenv("TZ",tz,1);tzset();}
static int cs=1; static std::vector<uint8_t> bytes;
static uint8_t regs[4][8];   // [módulo desde la entrada][fila]
static bool apagado=false; static int brillo=0;
struct Cuadro{uint32_t t; uint8_t img[32]; int brillo;};
std::vector<Cuadro> cuadros;
void pinMode(int,int){}
void digitalWrite(int pin,int v){
  if(v==LOW){bytes.clear();}
  else if(cs==LOW){ // fin de transmisión: el primer par llega al módulo más lejano
    int n=bytes.size()/2;
    for(int i=0;i<n;i++){int m=n-1-i;uint8_t r=bytes[2*i],d=bytes[2*i+1];
      if(r>=1&&r<=8) regs[m][r-1]=d; else if(r==0x0A) brillo=d; else if(r==0x0C) apagado=(d==0);}
    if(bytes.size()>=2 && bytes[0]==8){  // última fila enviada: cuadro completo
      Cuadro c; c.t=sim_ms; c.brillo=apagado?-1:brillo;
      for(int x=0;x<32;x++){uint8_t col=0;for(int y=0;y<8;y++) if(regs[x/8][y]&(0x80>>(x%8))) col|=1<<y; c.img[x]=col;}
      cuadros.push_back(c);}
  }
  cs=v;
}
uint8_t SPISim::transfer(uint8_t b){bytes.push_back(b);return 0;}
void esperar(uint32_t ms){sim_ms+=ms;}
time_t sim_epoch=0;
extern "C" time_t time(time_t* t){time_t v=sim_epoch+sim_ms/1000; if(t)*t=v; return v;}
int main(int argc,char**argv){
  srand(atoi(argv[2]));
  Config::valoresPorDefecto(); strcpy(ajustes.nombre,"Alejandro"); ajustes.cumpleDia=25; ajustes.cumpleMes=10;
  if(argc>4 && std::string(argv[4])=="en") ajustes.idioma=IDIOMA_EN;   // idioma opcional: es / en
  configTime(ajustes.zona,"","","");
  sim_epoch=1792342980; // 2026-10-18 18:23 UTC aprox
  Pantalla::iniciar(); cuadros.clear();
  std::string que=argv[1];
  struct tm t; Tiempo::ahora(t);
  if(que=="reloj"||que=="relojcorto"){ int n=que=="reloj"?80:15; for(int i=0;i<n;i++){Tiempo::ahora(t);Pantalla::limpiar();Esfera::hora(t,(sim_ms%1000));Pantalla::mostrar();esperar(100);} }
  else if(que=="reloj12"){ ajustes.formato24=0; ajustes.modoSegundos=SEG_BARRA; for(int i=0;i<30;i++){Tiempo::ahora(t);Pantalla::limpiar();Esfera::hora(t,(sim_ms%1000));Pantalla::mostrar();esperar(100);} }
  else if(que=="fecha"){ Pantalla::limpiar(); Esfera::fechaCorta(t); Pantalla::mostrar(); esperar(1500);}
  else if(que=="arranque") Anim::arranque();
  else if(que=="cumple") Anim::cumple();
  else if(que=="noches") Anim::buenasNoches();
  else if(que=="hora"){ t.tm_hour=17;t.tm_min=0; Anim::horaEnPunto(t);}
  else if(que=="prueba") Anim::patronPrueba();
  else if(que.rfind("mensaje",0)==0) Anim::mensajeEspecial("¡A CENAR, {N}!", atoi(que.c_str()+7), 20, 40);   // mensaje0..mensaje6, 99, 100+n
  else Anim::lanzar(atoi(que.c_str()));
  // salida: t brillo 32 columnas en hex
  FILE*f=fopen(argv[3],"w");
  for(auto&c:cuadros){fprintf(f,"%u %d",c.t,c.brillo);for(int x=0;x<32;x++)fprintf(f," %02x",c.img[x]);fprintf(f,"\n");}
  fclose(f); printf("%s: %zu cuadros, %.1f s\n",argv[1],cuadros.size(),sim_ms/1000.0);
}
