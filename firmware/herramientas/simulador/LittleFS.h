#pragma once
#include "Arduino.h"
struct File{operator bool()const{return false;} bool available(){return false;} String readStringUntil(char){return "";}
 template<class T>void print(T){} template<class T>void println(T){} void close(){}};
struct FSSim{bool begin(){return true;} void format(){} File open(const char*,const char*){return File();} bool remove(const char*){return true;}};
extern FSSim LittleFS;
