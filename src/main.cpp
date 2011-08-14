#include<math.h>
#include<allegro.h>
#include"mapgen.h"
#include"ColourSystem.hpp"
#include"view3d.h"
#include <cstdio>
#include <iostream>

// Global variables

// Initial value of user defined fractal factor  
char fractalFactor[16] = "1.6";

// Initial random seed
char seedString[32] = "6040";
int seed;

// Initial filename for saved map,
// number will be incremented after each saving
char filename[80] = "MyMap00.bmp";


char highflat[16] = ".9";
char lowflat[16] = ".5";


// Humbly borrowed from ex14.c
DIALOG dlg[] =
{
   /* (proc)        (x)  (y)  (w)  (h) (fg) (bg)      (key) (flags)     (d1) (d2) (dp)                               (dp2) (dp3) */
   { d_text_proc,   20,  13,  200, 16, 0,   0xeeeeee, 0,    0,          0,   0,   (void*)"Map Generator",            0,    0    },
   { d_button_proc, 20,  38,  200, 16, 0,   0xeeeeee, 'r',  D_EXIT,     0,   0,   (void*)"Create &Random",           0,    0    },
   { d_button_proc, 20,  58,  200, 16, 0,   0xeeeeee, 'k',  D_EXIT,     0,   0,   (void*)"Create from Current &Key", 0,    0    },
   { d_text_proc,   20,  83,  200, 16, 0,   0xeeeeee, 0,    0,          0,   0,   (void*)"Current key:",             0,    0    },
   { d_edit_proc,   125, 83,  140, 16, 0,   0xeeeeee, 0,    0,          15,  0,   seedString,                        0,    0    },
   { d_text_proc,   20,  103, 140, 16, 0,   0xeeeeee, 0,    0,          0,   0,   (void*)"Fractal Factor",           0,    0    },
   { d_radio_proc,  30,  118, 140, 16, 0,   0xeeeeee, 0,    D_SELECTED, 1,   0,   (void*)"Square Root of 2",         0,    0    },
   { d_radio_proc,  30,  138, 140, 16, 0,   0xeeeeee, 0,    0,          1,   0,   (void*)"",                         0,    0    },
   { d_edit_proc,   55,  143, 140, 16, 0,   0xeeeeee, 0,    0,          15,  0,   fractalFactor,                     0,    0    },
   { d_text_proc,   20,  163, 140, 16, 0,   0xeeeeee, 0,    0,          0,   0,   (void*)"Palette:",                 0,    0    },
   { d_radio_proc,  30,  178, 140, 16, 0,   0xeeeeee, 0,    0,          2,   0,   (void*)"green/blue palette",       0,    0    },
   { d_radio_proc,  30,  198, 140, 16, 0,   0xeeeeee, 0,    0,          2,   0,   (void*)"no sea palette",           0,    0    },
   { d_radio_proc,  30,  218, 140, 16, 0,   0xeeeeee, 0,    0,	2,   0,   (void*)"grey palette",             0,    0    },
   { d_radio_proc,  30,  238, 140, 16, 0,   0xeeeeee, 0,    D_SELECTED,          2,   0,   (void*)"terrain palette",          0,    0    },
   { d_button_proc, 16,  420, 92, 16, 0,   0xeeeeee, 's',  D_EXIT,     0,   0,   (void*)"&Save", 0,    0    },
   { d_button_proc, 124,  420, 92, 16, 0,   0xeeeeee, 'q',  D_EXIT,     0,   0,   (void*)"&Quit",                    0,    0    },
   { d_slider_proc, 208, 108, 16,  90, 0,   0xeeeeee, 0,    0,          3,   2,   NULL,                              NULL, NULL },
   { d_text_proc,   208, 98,  64,  8,  0,   0xeeeeee, 0,    0,          0,   0,   (void*)"size",                     NULL, NULL },
   { d_text_proc,   224, 190, 8,   8,  0,   0xeeeeee, 0,    0,          0,   0,   (void*)"64",                       NULL, NULL },
   { d_text_proc,   224, 163, 16,  8,  0,   0xeeeeee, 0,    0,          0,   0,   (void*)"128",                      NULL, NULL },
   { d_text_proc,   224, 136, 16,  8,  0,   0xeeeeee, 0,    0,          0,   0,   (void*)"256",                      NULL, NULL },
   { d_text_proc,   224, 108, 16,  8,  0,   0xeeeeee, 0,    0,          0,   0,   (void*)"512",                      NULL, NULL },
   { d_slider_proc, 208, 248, 16,  90, 0,   0xeeeeee, 0,    0,          90,  22,  NULL,                              NULL, NULL },
   { d_text_proc,   208, 238, 64,  8,  0,   0xeeeeee, 0,    0,          0,   0,   (void*)"altitude",                 NULL, NULL },
   { d_text_proc,   224, 330, 8,   8,  0,   0xeeeeee, 0,    0,          0,   0,   (void*)"0",                        NULL, NULL },
   { d_text_proc,   224, 290, 16,  8,  0,   0xeeeeee, 0,    0,          0,   0,   (void*)"45",                       NULL, NULL },
   { d_text_proc,   224, 248, 16,  8,  0,   0xeeeeee, 0,    0,          0,   0,   (void*)"90",                       NULL, NULL },
   { d_slider_proc, 16,  320, 180, 16, 0,   0xeeeeee, 0,    0,          180, 22,  NULL,                              NULL, NULL },
   { d_text_proc,   16,  302, 56,  8,  0,   0xeeeeee, 0,    0,          0,   0,   (void*)"azimuth",                  NULL, NULL },
   { d_text_proc,   16,  312, 8,   8,  0,   0xeeeeee, 0,    0,          0,   0,   (void*)"0",                        NULL, NULL },
   { d_text_proc,   56,  312, 16,  8,  0,   0xeeeeee, 0,    0,          0,   0,   (void*)"90",                       NULL, NULL },
   { d_text_proc,   96,  312, 24,  8,  0,   0xeeeeee, 0,    0,          0,   0,   (void*)"180",                      NULL, NULL },
   { d_text_proc,   136, 312, 24,  8,  0,   0xeeeeee, 0,    0,          0,   0,   (void*)"270",                      NULL, NULL },
   { d_text_proc,   176, 312, 24,  8,  0,   0xeeeeee, 0,    0,          0,   0,   (void*)"360",                      NULL, NULL },
   { d_text_proc,   16,  264, 48,  8,  0,   0xeeeeee, 0,    0,          0,   0,   (void*)"Light:",                   NULL, NULL },
   { d_check_proc,  16,  280, 72,  16, 0,   0xeeeeee,        0,    0,          1,   0,   (void*)"Shadow",                   NULL, NULL },
   { d_button_proc, 16,  350, 200, 16, 0,   0xeeeeee, 'v',  D_EXIT,     0,   0,   (void*)"&View in 3D",                    0,    0    },
   { d_check_proc,  16,  370, 72,  16, 0,   0xeeeeee,        0,    0,          1,   0,   (void*)"High flat:",                   NULL, NULL },
   { d_check_proc,  16,  390, 72,  16, 0,   0xeeeeee,        0,    D_SELECTED,          1,   0,   (void*)"Low flat:",                   NULL, NULL },
   { d_edit_proc,   120,  374, 140, 16, 0,   0xeeeeee, 0,    0,          15,  0,   highflat,                     0,    0    },
   { d_edit_proc,   120,  394, 140, 16, 0,   0xeeeeee, 0,    0,          15,  0,   lowflat,                     0,    0    },
   { NULL,          0,   0,   0,   0,  0,   0,        0,    0,          0,   0,   NULL,                              NULL, NULL }
};



int lc=0;
void _lc(){lc++;}END_OF_FUNCTION(_lc)


void SetStringMain(char* s, int v)
{
	sprintf(s, "%i", v);
}

int main(void){
//	int i;
	int command=0;

	srand(time(NULL));
	allegro_init();

	// Change this to what ever you want, f.i. the gfx dialog
	set_color_depth(32);
	std::cout<<"HERE"<<std::endl;
	set_gfx_mode(GFX_AUTODETECT_WINDOWED, 800, 600, 0, 0);
	install_timer();
	install_mouse();
	install_keyboard();
	
	LOCK_VARIABLE(lc);
	LOCK_FUNCTION(_lc);
	install_int_ex(_lc,BPS_TO_TIMER(100));

	// Four predefined palettes
	ColourSystem pal1(0, 0, 255, 0, 240, 0);
	ColourSystem pal2(0, 220, 0, 255, 255, 255);
	ColourSystem pal3(0, 0, 0, 255, 255, 255);
	ColourSystem pal4(0, 0, 200, 255, 255, 255);

	pal1.AddPoint(0, 0, 255, 0.49);
	pal1.AddPoint(0, 240, 0, 0.51);

	pal2.AddPoint(10, 120, 5, 0.1);
	pal2.AddPoint(192, 140, 70, 0.3);
	pal2.AddPoint(160, 80, 10, 0.5);
	pal2.AddPoint(50, 40, 30, 0.65);
	pal2.AddPoint(99, 99, 99, 0.8);
	pal2.AddPoint(255, 255, 244, 0.9);

	pal3.AddPoint(20, 20, 20, 0.45);
	pal3.AddPoint(190, 190, 190, 0.5);
	pal3.AddPoint(190, 190, 190, 0.55);
	pal3.AddPoint(100, 100, 100, 0.551);
	pal3.AddPoint(30, 30, 30, 0.75);
	pal3.AddPoint(60, 60, 60, 0.9);
	pal3.AddPoint(255, 255, 255, 0.95);

	pal4.AddPoint(0, 0, 255, 0.45);
	pal4.AddPoint(0, 255, 255, 0.5);
	pal4.AddPoint(255, 159, 31, 0.51);
	pal4.AddPoint(255, 159, 31, 0.55);
	pal4.AddPoint(19, 179, 11, 0.555);
	pal4.AddPoint(0, 119, 0, 0.6);
	pal4.AddPoint(219, 186, 140, 0.7);
	pal4.AddPoint(125, 77, 0, 0.85);
	pal4.AddPoint(250, 250, 250, 0.9);

	clear_to_color(screen, 0xeeeeee);


	int size[]={64,128,256,512};
	MAPGEN mapgen(size[dlg[16].d2]);

	// Main loop
	while (command != 15 && !key[KEY_ESC]){
		//Test moving the light
/*		az+=10;az%=180;
		al+=5;al%=90;
		Show(currentImage,map,256,256,currentPal,az,al);
		blit(currentImage, screen, 0, 0, 0, 0, 256, 256);
*/
		command = do_dialog(dlg, -1);
/*		switch(dlg[16].d2){
		case 0: size=64;	break;
		case 1: size=128;	break;
		case 2: size=256;	break;
		case 3: size=512;	break;
		}
*/
		mapgen.resize(size[dlg[16].d2]);
		switch(command){
		// Random
		case 1:
			srand(time(NULL));
			seed = rand();
//			itoa(seed, seedString, 10);
			SetStringMain(seedString, seed);

		//Both cases uses the same code to create the map, the first just sets a random key.
		// From key
		case 2:
			seed = atoi(seedString);
			mapgen.create((dlg[7].flags & D_SELECTED)?atof(fractalFactor):1.4142136,seed);

			if (dlg[10].flags & D_SELECTED)
				mapgen.pal = &pal1;
			if (dlg[11].flags & D_SELECTED)
				mapgen.pal = &pal2;
			if (dlg[12].flags & D_SELECTED)
				mapgen.pal = &pal3;
			if (dlg[13].flags & D_SELECTED)  
				mapgen.pal = &pal4;  
			mapgen.flags=0;
			if(dlg[35].flags&D_SELECTED)	mapgen.flags|=SHADOW;

			mapgen.set_light(dlg[27].d2,dlg[22].d2);
			mapgen.render();
			clear_to_color(screen, 0xeeeeee);
			blit(mapgen.colormap(), screen, 0, 0, 280, 8, size[dlg[16].d2], size[dlg[16].d2]);
		break;

		// Save the image as BMP
		// Change this if you want more options
		case 14:
			if (file_select_ex("Save to file", filename, "BMP",80,320,200)){
//				save_bmp(filename, currentImage, NULL);
				save_bmp(filename, mapgen.colormap(), NULL);
				// Increment file name number, if used
				if (filename[5]>='0' && filename[5]<='9' && filename[6]>='0' && filename[6]<='9'){
					filename[6]++;
					if (filename[6]>'9')
					{
						filename[6] = '0';
						filename[5]++;
						if (filename[5]>'9')
							filename[5]='0';
					}
				} 
				clear_to_color(screen, 0xeeeeee);
//				blit(currentImage, screen, 0, 0, 0, 0, dlg[16].d2, dlg[16].d2);
				blit(mapgen.colormap(), screen, 0, 0, 0, 0, dlg[16].d2, dlg[16].d2);
			}
		break;
		case 36:
			view3d(mapgen,(dlg[37].flags & D_SELECTED)?atof(highflat):-1,(dlg[38].flags & D_SELECTED)?atof(lowflat):-1);
			clear_to_color(screen, 0xeeeeee);
			blit(mapgen.colormap(), screen, 0, 0, 0, 0, dlg[16].d2, dlg[16].d2);
		break;
		}
	}
/*
	for (i = 0; i < 256; i++)
		delete [] map[i];
	delete [] map;
*/
/*	for (i = 0; i < 512; i++)
		delete [] map[i];
	delete [] map;

	destroy_bitmap(currentImage);
*/
	return 0;
}END_OF_MAIN();
