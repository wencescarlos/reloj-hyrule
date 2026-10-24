// Simulador en PC: imitación mínima del entorno Arduino/ESP8266
#pragma once
#include <cstdint>
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <string>
#include <algorithm>
#include <time.h>
#define PROGMEM
#define F(x) x
#define D6 12
#define OUTPUT 1
#define LOW 0
#define HIGH 1
#define MSBFIRST 1
#define SPI_MODE0 0
inline uint8_t pgm_read_byte(const void* p){return *(const uint8_t*)p;}
inline uint16_t pgm_read_word(const void* p){return *(const uint16_t*)p;}
extern uint32_t sim_ms;
inline uint32_t millis(){return sim_ms;}
inline void delay(uint32_t ms){sim_ms+=ms;}
inline long random(long a){return a>0? rand()%a:0;}
inline long random(long a,long b){return b>a? a+rand()%(b-a):a;}
void pinMode(int,int); void digitalWrite(int,int); inline void delayMicroseconds(unsigned){}
template<class T> T max(T a,T b){return a>b?a:b;}
class String {
 public: std::string s;
  String(){} String(const char*c):s(c?c:""){} String(const std::string&x):s(x){}
  String(char c):s(1,c){} String(int n):s(std::to_string(n)){} String(long n):s(std::to_string(n)){}
  String(unsigned n):s(std::to_string(n)){} String(unsigned long n):s(std::to_string(n)){}
  const char* c_str()const{return s.c_str();} size_t length()const{return s.size();}
  void replace(const String&a,const String&b){size_t p=0;while((p=s.find(a.s,p))!=std::string::npos){s.replace(p,a.s.size(),b.s);p+=b.s.size();}}
  void trim(){size_t a=s.find_first_not_of(" \t\r\n"),b=s.find_last_not_of(" \t\r\n");s=a==std::string::npos?"":s.substr(a,b-a+1);}
  bool startsWith(const String&x)const{return s.rfind(x.s,0)==0;}
  bool endsWith(const String&x)const{return s.size()>=x.s.size()&&s.compare(s.size()-x.s.size(),x.s.size(),x.s)==0;}
  String substring(size_t a,size_t b=std::string::npos)const{return s.substr(a,b==std::string::npos?b:b-a);}
  int indexOf(char c)const{auto p=s.find(c);return p==std::string::npos?-1:(int)p;}
  long toInt()const{return atol(s.c_str());}
  char operator[](size_t i)const{return s[i];}
  String& operator+=(const String&o){s+=o.s;return *this;} String& operator+=(const char*o){s+=o;return *this;}
  String& operator+=(char c){s+=c;return *this;} String& operator+=(int n){s+=std::to_string(n);return *this;}
  bool operator!=(const char*o)const{return s!=o;} bool operator==(const char*o)const{return s==o;}
  friend String operator+(const String&a,const String&b){return String(a.s+b.s);}
  friend String operator+(const String&a,const char*b){return String(a.s+b);}
  friend String operator+(const String&a,int b){return String(a.s+std::to_string(b));}
};
struct SerialSim{template<class...A>void printf(A...){} void println(...){} void print(...){} void begin(int){}};
extern SerialSim Serial;
void configTime(const char* tz,const char*,const char*,const char*);
