#include"mapgen.h"

#define farlimit 20000.
#define tilesize 100

class MAP{
	int size;
	V3D_f **map;
public:
	MAP(MAPGEN &mapgen,float hflat,float lflat);
	void remake(MAPGEN &mapgen);
	void render(BITMAP *bmp, MATRIX_f *camera, int ox, int oy);
};
