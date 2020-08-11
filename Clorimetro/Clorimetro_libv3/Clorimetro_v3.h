/*
  Clorimetro_v1.h - Arduino clorimetro
Creada por Ramsés Marrero, 8/7/15
Lanzado bajo licencia --- Open Software
*/
#ifndef Clorimetro_h
#define Clorimetro_h
#include "Arduino.h"
class Clorimetro {
  public:
   Clorimetro(int emisor, int receptor, int time);
   void Calibrar();
   double Read();
  private:  
   int _emisor;
   int _receptor;
   int _time;
};
#endif
