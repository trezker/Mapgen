#ifndef COLOURSYSTEM
#define COLOURSYSTEM

#include <allegro.h>

class ColourPoint
{
   public:
   unsigned char r;
   unsigned char g;
   unsigned char b;
   float p;
   ColourPoint *next;
   ColourPoint(unsigned char r,
               unsigned char g,
               unsigned char b,
               float pin);
};

class ColourSystem
{               
   ColourPoint *points;
   static int Interpolate(float c1, float p1,
                          float c2, float p2,
                          float pin);
   static void Hsv(int *r, int *g, int *b, 
                   float *hf, float *sf, float *vf);
   static void Rgb(int *r, int *g, int *b, 
                   int *h, int *s, int *v);
   static void HsvInt(float *hf, float *sf, float *vf, 
                      int *h, int *s, int *v);
   public:
   ColourSystem(unsigned char r1,
                unsigned char g1,
                unsigned char b1,
                unsigned char r2,
                unsigned char g2,
                unsigned char b2);
   void AddPoint(unsigned char r,
                 unsigned char g,
                 unsigned char b,
                 float pin);
   int GetColour(float cp);
   static int Pick(void);
   static int Pick(int start);
   static int RSlide(void *dp3, int d2);
   static int GSlide(void *dp3, int d2);
   static int BSlide(void *dp3, int d2);
   static int HSlide(void *dp3, int d2);
   static int SSlide(void *dp3, int d2);
   static int VSlide(void *dp3, int d2);
   static void Fill(void *istr);
   static void UpdateCField(int h, int s, int v, BITMAP *cfield);

   
   static void Delete(ColourPoint *pnt);
   ~ColourSystem();
};

#endif
