#ifndef WANDERERHPP
#define WANDERERHPP

#include "vektor.hpp"

class Wanderer
{
   char ux, uy, u0x, u0y;
   float dx, dy;
   float ditherer;
   float smaller;
   float bigger;
   char sgn(float x);
   float abs(float x);
   
   public:
   int x, y, steps;
   Wanderer(int xin, int yin, float dxin, float dyin);
   Wanderer(int x1in, int y1in, int x2in, int y2in);
   Wanderer(int xin, int yin, Vektor *v);
   void Step(void);
   char Done(void);
};

#endif
