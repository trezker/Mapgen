#include "ColourSystem.hpp"
//#include "punkter.hpp"
#include <allegro.h>
#include <cstdio>
#include <sstream>

ColourSystem::ColourSystem(unsigned char r1,
                           unsigned char g1,
                           unsigned char b1,
                           unsigned char r2,
                           unsigned char g2,
                           unsigned char b2)
{
   ColourPoint *z;
   points = new ColourPoint(r1, g1, b1, 0);
   z = new ColourPoint(r2, g2, b2, 1);
   points->next = z;
   z->next = NULL;
}

void ColourSystem::AddPoint(unsigned char r,
                            unsigned char g,
                            unsigned char b,
                            float pin)
{
   ColourPoint *z, *i;
   if (pin <= 0 || pin >= 1) return;
   for (i = points; i != NULL; i = i->next)
   {
      if (i->p == pin)
      {
          i->r = r;
          i->g = g;
          i->b = b;
          return;
      }
      if (i->p > pin)
      {
         z = new ColourPoint(i->r, i->g, i->b, i->p);
         i->r = r;
         i->g = g;
         i->b = b;
         i->p = pin;
         z->next = i->next;
         i->next = z;
         return;
      }
   }
}

int ColourSystem::GetColour(float fin)
{
   ColourPoint *i, *j;
   if (fin < 0 || fin > 1) return 0;
   for (i = points, j = points; i != NULL; i = i->next) 
   {
      if (fin == i->p) return makecol(i->r, i->g, i->b);
      if (fin < i->p)
          return makecol(Interpolate(j->r, j->p, 
                                     i->r, i->p,
                                     fin),
                         Interpolate(j->g, j->p, 
                                     i->g, i->p,
                                     fin),
                         Interpolate(j->b, j->p, 
                                     i->b, i->p,
                                     fin));
      j = i;
   }
   return 0xff8040;
}
                                       
int ColourSystem::Interpolate(float c1, float p1,
                              float c2, float p2,
                              float pin)
{
   return int((c2 - c1)/(p2 - p1)*(pin - p1) + c1);
}
  
ColourPoint::ColourPoint(unsigned char rin,
                         unsigned char gin,
                         unsigned char bin,
                         float pin)
: r(rin), g(gin), b(bin), p(pin)
{
}

ColourSystem::~ColourSystem()
{
    Delete(points);
}

void ColourSystem::Fill(void *istr)
{
   char *str = (char*)istr;
   int i = atoi(str);
   if (i < 100)
   {
      str[3] = '\0';
      str[2] = ' ';
   }
   if (i < 10)
      str[1] = ' ';
}

void SetString(char* s, int v)
{
	sprintf(s, "%i", v);
}

int ColourSystem::RSlide(void *dp3, int )
{
   /**********************************\ 
   | Skapa skilda RSlide, GSlide...   |
   | Läs RSlide, Gtxt, Btxt           |
   | skapa HSVslide&txt               |
   \**********************************/
    int r, g, b, h, s, v;
    float hf, sf, vf;
    DIALOG *d;
    d = (DIALOG*)dp3;
    
    r = d[2].d2; r *= 5;
	SetString((char*)d[8].dp, r);
    Fill(d[8].dp);
    g = atoi((char*)d[9].dp);
    b = atoi((char*)d[10].dp);
    Hsv(&r, &g, &b, &hf, &sf, &vf);
    HsvInt(&hf, &sf, &vf, &h, &s, &v);
	SetString((char*)d[11].dp, h);
    Fill(d[11].dp);
	SetString((char*)d[12].dp, s);
    Fill(d[12].dp);
	SetString((char*)d[13].dp, v);
    Fill(d[13].dp);
    d[5].d2 = h / 5;
    d[6].d2 = s / 5;
    d[7].d2 = v / 5;
    object_message(&d[8], MSG_DRAW, 0);
    object_message(&d[5], MSG_DRAW, 0);
    object_message(&d[6], MSG_DRAW, 0);
    object_message(&d[7], MSG_DRAW, 0);
    
    object_message(&d[11], MSG_DRAW, 0);
    object_message(&d[12], MSG_DRAW, 0);
    object_message(&d[13], MSG_DRAW, 0);

    return 0;
    
}

int ColourSystem::GSlide(void *dp3, int )
{
    int r, g, b, h, s, v;
    float hf, sf, vf;
    DIALOG *d;
    d = (DIALOG*)dp3;
    
    g = d[3].d2; g *= 5;
	SetString((char*)d[9].dp, g);
    Fill(d[9].dp);
    r = atoi((char*)d[8].dp);
    b = atoi((char*)d[10].dp);
    Hsv(&r, &g, &b, &hf, &sf, &vf);
    HsvInt(&hf, &sf, &vf, &h, &s, &v);
	SetString((char*)d[11].dp, h);
    Fill(d[11].dp);
	SetString((char*)d[12].dp, s);
    Fill(d[12].dp);
	SetString((char*)d[13].dp, v);
    Fill(d[13].dp);
    d[5].d2 = h / 5;
    d[6].d2 = s / 5;
    d[7].d2 = v / 5;
    object_message(&d[9], MSG_DRAW, 0);
    object_message(&d[5], MSG_DRAW, 0);
    object_message(&d[6], MSG_DRAW, 0);
    object_message(&d[7], MSG_DRAW, 0);
    
    object_message(&d[11], MSG_DRAW, 0);
    object_message(&d[12], MSG_DRAW, 0);
    object_message(&d[13], MSG_DRAW, 0);

    return 0;
    
}

int ColourSystem::BSlide(void *dp3, int )
{
    int r, g, b, h, s, v;
    float hf, sf, vf;
    DIALOG *d;
    d = (DIALOG*)dp3;
    
    b = d[4].d2; b *= 5;
	SetString((char*)d[10].dp, b);
    Fill(d[10].dp);
    r = atoi((char*)d[8].dp);
    g = atoi((char*)d[9].dp);
    Hsv(&r, &g, &b, &hf, &sf, &vf);
    HsvInt(&hf, &sf, &vf, &h, &s, &v);
	SetString((char*)d[11].dp, h);
    Fill(d[11].dp);
	SetString((char*)d[12].dp, s);
    Fill(d[12].dp);
	SetString((char*)d[13].dp, v);
    Fill(d[13].dp);
    d[5].d2 = h / 5;
    d[6].d2 = s / 5;
    d[7].d2 = v / 5;
    object_message(&d[10], MSG_DRAW, 0);
    object_message(&d[5], MSG_DRAW, 0);
    object_message(&d[6], MSG_DRAW, 0);
    object_message(&d[7], MSG_DRAW, 0);
    
    object_message(&d[11], MSG_DRAW, 0);
    object_message(&d[12], MSG_DRAW, 0);
    object_message(&d[13], MSG_DRAW, 0);

    return 0;
    

}

int ColourSystem::HSlide(void *dp3, int )
{
    int r, g, b, h, s, v;
    DIALOG *d;
    d = (DIALOG*)dp3;
    
    h = d[5].d2; h *= 5;
	SetString((char*)d[11].dp, h);
    Fill(d[11].dp);
    s = atoi((char*)d[12].dp);
    v = atoi((char*)d[13].dp);
    Rgb(&r, &g, &b, &h, &s, &v);
	SetString((char*)d[8].dp, r);
    Fill(d[8].dp);
	SetString((char*)d[9].dp, g);
    Fill(d[9].dp);
	SetString((char*)d[10].dp, b);
    Fill(d[10].dp);
    d[2].d2 = r / 5;
    d[3].d2 = g / 5;
    d[4].d2 = b / 5;
    object_message(&d[11], MSG_DRAW, 0);
    object_message(&d[2], MSG_DRAW, 0);
    object_message(&d[3], MSG_DRAW, 0);
    object_message(&d[4], MSG_DRAW, 0);
    
    object_message(&d[8], MSG_DRAW, 0);
    object_message(&d[9], MSG_DRAW, 0);
    object_message(&d[10], MSG_DRAW, 0);

    return 0;
}

int ColourSystem::SSlide(void *dp3, int )
{
    int r, g, b, h, s, v;
    DIALOG *d;
    d = (DIALOG*)dp3;
    
    s = d[6].d2; s *= 5;
	SetString((char*)d[12].dp, s);
    Fill(d[12].dp);
    h = atoi((char*)d[11].dp);
    v = atoi((char*)d[13].dp);
    Rgb(&r, &g, &b, &h, &s, &v);
	SetString((char*)d[8].dp, r);
    Fill(d[8].dp);
	SetString((char*)d[9].dp, g);
    Fill(d[9].dp);
	SetString((char*)d[10].dp, b);
    Fill(d[10].dp);
    d[2].d2 = r / 5;
    d[3].d2 = g / 5;
    d[4].d2 = b / 5;
    object_message(&d[12], MSG_DRAW, 0);
    object_message(&d[2], MSG_DRAW, 0);
    object_message(&d[3], MSG_DRAW, 0);
    object_message(&d[4], MSG_DRAW, 0);
    
    object_message(&d[8], MSG_DRAW, 0);
    object_message(&d[9], MSG_DRAW, 0);
    object_message(&d[10], MSG_DRAW, 0);

    return 0;

}

int ColourSystem::VSlide(void *dp3, int )
{
    int r, g, b, h, s, v;
    DIALOG *d;
    d = (DIALOG*)dp3;
    
    v = d[7].d2; v *= 5;
	SetString((char*)d[13].dp, v);
    Fill(d[13].dp);
    h = atoi((char*)d[11].dp);
    s = atoi((char*)d[12].dp);
    Rgb(&r, &g, &b, &h, &s, &v);
	SetString((char*)d[8].dp, r);
    Fill(d[8].dp);
	SetString((char*)d[9].dp, g);
    Fill(d[9].dp);
	SetString((char*)d[10].dp, b);
    Fill(d[10].dp);
    d[2].d2 = r / 5;
    d[3].d2 = g / 5;
    d[4].d2 = b / 5;
    object_message(&d[13], MSG_DRAW, 0);
    object_message(&d[2], MSG_DRAW, 0);
    object_message(&d[3], MSG_DRAW, 0);
    object_message(&d[4], MSG_DRAW, 0);
    
    object_message(&d[8], MSG_DRAW, 0);
    object_message(&d[9], MSG_DRAW, 0);
    object_message(&d[10], MSG_DRAW, 0);

    return 0;

}

/*
void ColourSystem::UpdateCField(int h, int s, int v, BITMAP *cfield)
{
    int i, j, r1, g1, b1, dx, dy;
    float h1, s1, rad;
    FloatCoord origo(64, 64);
    FloatCoord hobo;
    
    for (i = 0; i < 128; i++)
    {
       for (j = 0; j < 128; j++)
       {
           hobo.Set(j, i);
           rad = hobo - origo;
           if (rad > 64) continue;
           h1 = origo.Dir(&hobo) * 57.29578;
           if (rad > 54)
              s1 = 1;
           else
              s1 = rad / 54;
           hsv_to_rgb(h1, s1, float(v) / 256, &r1, &g1, &b1);
           putpixel(cfield, j, i, makecol(r1, g1, b1));
       }
    }
    dx = s * sin(float(h)*360/256/57.29578) / 256 * 64;
    dy = s * cos(float(h)*360/256/57.29578) / 256 * 64;
    circle(cfield, 64 + dx, 64 + dy, 7, 0xffffff);
    blit(cfield, screen, 0, 0, 120, 196, 160, 160);
}
*/
/*           
int d_colcir_proc(int msg, DIALOG *d, int c)   
{
   switch (msg)
   {
      case MSG_START :
      case MSG_END :
        return D_O_K;
      
      
        
*/           
           
/*
int ColourSystem::Pick(int start)
{
    BITMAP *cfield;   
    cfield = create_bitmap(160, 160);
    clear_to_color(cfield, 14869218);
    char rtxt[4], gtxt[4], btxt[4], htxt[4], stxt[4], vtxt[4], rettxt[7];
         
DIALOG cpick[] =
{
   // (proc)        (x)  (y)  (w)  (h)  (fg)      (bg)      (key) (flags) (d1) (d2) (dp)      (dp2) (dp3) 
   { d_clear_proc,  104, 80,  280, 216, 14869218, 14869218, 0,    0,      0,   0,   NULL,     NULL, NULL },
   { d_colcir_proc, 120, 96,  160, 160, 0,        0,        0,    0,      0,   0,   cfield,   NULL, NULL },
   { d_slider_proc, 288, 112, 20,  64,  11141120, 14869218, 0,    0,      51,  25,  NULL,     ColourSystem::RSlide, cpick},
   { d_slider_proc, 316, 112, 20,  64,  43520,    14869218, 0,    0,      51,  25,  NULL,     ColourSystem::GSlide, cpick},
   { d_slider_proc, 344, 112, 20,  64,  170,      14869218, 0,    0,      51,  25,  NULL,     ColourSystem::BSlide, cpick},
   { d_slider_proc, 288, 200, 20,  64,  0,        14869218, 0,    0,      51,  25,  NULL,     ColourSystem::HSlide, cpick},
   { d_slider_proc, 316, 200, 20,  64,  0,        14869218, 0,    0,      51,  25,  NULL,     ColourSystem::SSlide, cpick},
   { d_slider_proc, 344, 200, 20,  64,  0,        14869218, 0,    0,      51,  25,  NULL,     ColourSystem::VSlide, cpick},
   { d_text_proc,   288, 178, 24,  8,   0,        14869218, 0,    0,      0,   0,   rtxt,    NULL, NULL },
   { d_text_proc,   316, 178, 24,  8,   0,        14869218, 0,    0,      0,   0,   gtxt,    NULL, NULL },
   { d_text_proc,   344, 178, 24,  8,   0,        14869218, 0,    0,      0,   0,   btxt,    NULL, NULL },
   { d_text_proc,   288, 266, 24,  8,   0,        14869218, 0,    0,      0,   0,   htxt,    NULL, NULL },
   { d_text_proc,   316, 266, 24,  8,   0,        14869218, 0,    0,      0,   0,   stxt,    NULL, NULL },
   { d_text_proc,   344, 266, 24,  8,   0,        14869218, 0,    0,      0,   0,   vtxt,    NULL, NULL },
   { d_text_proc,   296, 103, 8,   8,   4202528,  14869218, 0,    0,      0,   0,   (void*)"r",      NULL, NULL },
   { d_text_proc,   324, 103, 8,   8,   4202528,  14869218, 0,    0,      0,   0,   (void*)"g",      NULL, NULL },
   { d_text_proc,   352, 103, 8,   8,   4202528,  14869218, 0,    0,      0,   0,   (void*)"b",      NULL, NULL },
   { d_text_proc,   296, 191, 8,   8,   4202528,  14869218, 0,    0,      0,   0,   (void*)"h",      NULL, NULL },
   { d_text_proc,   324, 191, 8,   8,   4202528,  14869218, 0,    0,      0,   0,   (void*)"s",      NULL, NULL },
   { d_text_proc,   352, 191, 8,   8,   4202528,  14869218, 0,    0,      0,   0,   (void*)"v",      NULL, NULL },
   { d_button_proc, 128, 264, 64,  16,  0,        14869218, 0,    D_EXIT, 0,   0,   (void*)"Ok",     NULL, NULL },
   { d_button_proc, 208, 264, 64,  16,  0,        14869218, 0,    0,      0,   0,   (void*)"Cancel", NULL, NULL },
   { NULL,          0,   0,   0,   0,   0,        0,        0,    0,      0,   0,   NULL,     NULL, NULL }
};

   int r, g, b, h, s, v;
   int dialogRet;
   float hf, sf, vf;
   b = start % 256;
   start /= 256;
   g = start % 256;
   r = start / 256;
   
   Hsv(&r, &g, &b, &hf, &sf, &vf);
   HsvInt(&hf, &sf, &vf, &h, &s, &v);
   itoa(r, rtxt, 10);
   itoa(g, gtxt, 10);
   itoa(b, btxt, 10);
   itoa(h, htxt, 10);
   itoa(s, stxt, 10);
   itoa(v, vtxt, 10);
   itoa(65536 * r + 256 * g + b, rettxt, 16);
   cpick[2].d2 = r / 5;
   cpick[3].d2 = g / 5;
   cpick[4].d2 = b / 5;
   cpick[5].d2 = h / 5;
   cpick[6].d2 = s / 5;
   cpick[7].d2 = v / 5;
   UpdateCField(h, s, v, cfield);
   
   do
   {
       
       dialogRet = do_dialog(cpick, -1);
       switch (dialogRet)
       {
          case 1 :
          case 2 :
          case 3 : 
             r = atoi(rtxt);
             g = atoi(gtxt);
             b = atoi(btxt);
             rgb_to_hsv(r, g, b, &hf, &sf, &vf);
             h = 256 * (hf / 360);
             s = 256 * sf;
             v = 256 * vf;
             UpdateCField(h, s, v, cfield);
             object_message(&cpick[1], MSG_DRAW, 0);
             itoa(h, htxt, 10);
             itoa(s, stxt, 10);
             itoa(v, vtxt, 10);
             // update slides
             break;
          case 4 :
          case 5 :
          case 6 : 
             h = atoi(htxt); hf = float(h)*360/256;
             s = atoi(stxt); sf = float(s) / 256;
             v = atoi(vtxt); vf = float(v) / 256;
             UpdateCField(h, s, v, cfield);
             object_message(&cpick[1], MSG_DRAW, 0);
             hsv_to_rgb(hf, sf, vf, &r, &g, &b);
             itoa(r, rtxt, 10);
             itoa(g, gtxt, 10);
             itoa(b, btxt, 10);
             
             break;
       }
           
             
              
       
   }
   while (dialogRet != 20 && dialogRet != 21);
   destroy_bitmap(cfield);
   if (dialogRet == 20)
      return 65536 * r + 256 * g + b;
   else
      return -1;
          
   // Fortsätt med att skapa en event handler av dialogen:
      
   //      sliders har en update UPP,
   //      övriga skall troligen uppdateras 
   //      efter varje slinga
   
}
*/
void ColourSystem::Hsv(int *r, int *g, int *b, float *hf, float *sf, float *vf)
{
   rgb_to_hsv(*r, *g, *b, hf, sf, vf);
}

void ColourSystem::Rgb(int *r, int *g, int *b, int *h, int *s, int *v)
{
   hsv_to_rgb(float(*h) / 256 * 360, float(*s) / 256, float(*v) / 256, r, g, b);
}

void ColourSystem::HsvInt(float *hf, float *sf, float *vf, int *h, int *s, int *v)
{
   *h = int(256 * (*hf / 360));
   *s = int(256 * *sf);
   *v = int(256 * *vf);
}


void ColourSystem::Delete(ColourPoint *pnt)
{
    if (pnt->next)
       Delete(pnt->next);
    delete pnt;
    pnt = NULL;
}

       
   
