#ifndef PROGRESSINDICATOR
#define PROGRESSINDICATOR

#include <allegro.h>

class ProgressIndicator{
	int x,y,w,h;
	bool showp;
	int bg,fg;
	char text[80];
public:
	ProgressIndicator(int ix, int iy, int iw, int ih,bool showp);
	ProgressIndicator(int ix, int iy, int iw, int ih,bool showp,int ibg, int ifg);
	void set_text(const char *itext);
	void Draw(float p);
};

#endif
   
