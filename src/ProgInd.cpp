#include "ProgInd.hpp"
#include <allegro.h>
#include <string.h>
#include <cstdio>

ProgressIndicator::ProgressIndicator(int ix, int iy, int iw, int ih,bool ishowp)
:x(ix),y(iy),w(iw),h(ih),showp(ishowp){
	bg = 0x4DB3FB;
	fg = 0x2171A9;
	text[0]=0;
}



ProgressIndicator::ProgressIndicator(int ix, int iy, int iw, int ih,bool ishowp,int ibg, int ifg)
:x(ix),y(iy),w(iw),h(ih),showp(ishowp),bg(ibg),fg(ifg){
	text[0]=0;
}



void ProgressIndicator::Draw(float p){
	int pl=int(w*p);
	rectfill(screen,x,y,x+pl,y+h-1,fg);
	if(pl!=w)
		rectfill(screen,x+pl,y,x+w-1,y+h-1,bg);
	char tstr[80];
	if(showp)
		sprintf(tstr,"%s %i%%",text,int(p*100));
	else
		strcpy(tstr,text);
	textout_centre_ex(screen,font,tstr,x+w/2,y+h/2-4,-1,-1);
}



void ProgressIndicator::set_text(const char *itext){
	strcpy(text,itext);
}
