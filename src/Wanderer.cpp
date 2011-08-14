#include "Wanderer.hpp"
#include "vektor.hpp"
#include <math.h>

Wanderer::Wanderer(int xin, int yin, Vektor *v)
: x(xin), y(yin)
{
   steps = 0;
   if (abs(v->j) > abs(v->i))
   {
      u0x = 0;
      u0y = sgn(v->j);
      ux = sgn(v->i);
      uy = sgn(v->j);
      bigger = abs(v->j);
      smaller = abs(v->i);
      
   }
   else
   {
      u0y = 0;
      u0x = sgn(v->i);
      uy = sgn(v->j);
      ux = sgn(v->i);
      bigger = abs(v->i);
      smaller = abs(v->j);
      
   }
   ditherer = 0;
}
   

Wanderer::Wanderer(int xin, int yin, float dxin, float dyin)
: x(xin), y(yin)
{
   steps = 0;
   if (abs(dyin) > abs(dxin))
   {
      u0x = 0;
      u0y = sgn(dyin);
      ux = sgn(dxin);
      uy = sgn(dyin);
      bigger = abs(dyin);
      smaller = abs(dxin);
      
   }
   else
   {
      u0y = 0;
      u0x = sgn(dxin);
      uy = sgn(dyin);
      ux = sgn(dxin);
      bigger = abs(dxin);
      smaller = abs(dyin);
      
   }
   ditherer = 0;
}
/*
Wanderer::Wanderer(int x1in, int y1in, int x2in, int y2in)
: x(x1in), y(y1in)
{
   steps = 0;
    
}
*/
char Wanderer::sgn(float x)
{
   if (x > 0)
      return 1;
   if (x < 0)
      return -1;
   return 0;
}

float Wanderer::abs(float x)
{
   if (x < 0) return -x;
   return x;
}
   
void Wanderer::Step(void)
{
   steps++;
   ditherer += smaller;
   if (ditherer > bigger)
   {
      ditherer -= bigger;
      x += ux;
      y += uy;
   }
   else
   {
      x += u0x;
      y += u0y;
   }
}
/*
char Wanderer::Done(void)
{
}
*/
