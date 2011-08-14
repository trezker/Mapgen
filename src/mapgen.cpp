#include <time.h>
#include <math.h>
#include <allegro.h>
#include "ColourSystem.hpp"
#include "vektor.hpp"
#include "Wanderer.hpp"
#include "ProgInd.hpp"
#include "mapgen.h"

//Helpers for wrapping the edges.
int MAPGEN::Minus(int j, int i){
	int r;

	r = j - i;
	if (r >= 0)
		return r;
	return r + msize;
}



int MAPGEN::Plus(int j, int i){
	int r;
	
	r = j + i;
	if (r < msize)
		return r;
	return r - msize;
}



// Returns something between -amount/2 and amount/2
float MAPGEN::FRandom(float amount){
	float h;
	h = amount * (float(rand()) / RAND_MAX - 0.5);
	return h;
}



//Creates a map generator with initial map size.
//Currently only square 2^n sizes are possible.
MAPGEN::MAPGEN(int isize):msize(isize){
	map = new float*[msize];
	for (int i = 0; i < msize; i++)
		map[i] = new float[msize];
	cmap=create_bitmap(msize,msize);
}



MAPGEN::~MAPGEN(){
	for (int i = 0; i < msize; i++)
		delete [] map[i];
	delete [] map;
	destroy_bitmap(cmap);
}



//Change the size of the map, any generated map disappears.
void MAPGEN::resize(int isize){
	if(isize!=msize){
		for (int i = 0; i < msize; i++)
			delete [] map[i];
		delete [] map;
	
		msize=isize;
		map = new float*[msize];
		for (int i = 0; i < msize; i++)
			map[i] = new float[msize];
	
		destroy_bitmap(cmap);
		cmap=create_bitmap(msize,msize);
	}
}



//Access to map info.
int MAPGEN::size(){
	return msize;
}



float **MAPGEN::heightmap(){
	return map;
}



BITMAP *MAPGEN::colormap(){
	return cmap;
}



//Creates a heightmap from your seed.
void MAPGEN::create(float fractalfactor, int seed){
	srand(seed);
	int w=msize,h=msize;
	int i, j, k;
	float amount = 256;//, l;

	// Do the corners
	map[0][0] = 0; //FRandom(amount);
	map[w-1][0] = 0; //FRandom(amount);
	map[w-1][h-1] = 0; //FRandom(amount);
	map[0][h-1] = 0; //FRandom(amount);
	amount /= fractalfactor;

	for (i = 128; i > 0; i /= 2){
		// This is the square phase
		for (j = i; j < w; j += 2 * i)
			for (k = i; k < h; k += 2 * i){
				map[j][k] = (map[Minus(j, i)][Minus(k, i)] +
					map[Minus(j, i)][Plus(k, i)] +
					map[Plus(j, i)][Plus(k, i)] +
					map[Plus(j, i)][Minus(k, i)]) / 4. +
					FRandom(amount);
			}
		amount /= fractalfactor;

		// This is the diamond phase
		for (j = 0; j < w; j += i)
			for (k = 0; k < h; k += i)
				if ((((j + k) / i) % 2) == 1){
					//  In this phase we might fall off the edge
					//  when we count the average of neighbours
					//  Minus and Plus take care of that
					map[j][k] = (map[Minus(j, i)][k] +
						map[j][Minus(k, i)] +
						map[Plus(j, i)][k] +
						map[j][Plus(k, i)]) / 4 +
						FRandom(amount);
				}
		amount /= fractalfactor;
	}
}



//Rendering functions.
void MAPGEN::set_light(float az, float alt){
	az *= 0.034906585;
	alt *= 0.0174532925;

	float lx, ly, lz;
	lx = 10*sin(az);
	ly = -10*cos(az);
	lz = 10*sin(alt);
	lx *= cos(alt);
	ly *= cos(alt);
	ljus=Vektor(lx, ly, lz);
}



void MAPGEN::render(){
	int w=msize,h=msize;
	ProgressIndicator myInd(20, 570, 236, 20,1);

	int startcol;

	float min = 256, max = -256, col;
	int i, j;

	myInd.set_text("Searching for min and max");
	for (i = 0; i < w; i++){
		myInd.Draw(float(i)/w);
		for (j = 0; j < h; j++){
			if (min > map[i][j]) min = map[i][j];
			if (max < map[i][j]) max = map[i][j];
		}
	}
	myInd.set_text("Rendering map");
	Wanderer *wander;
	for (i = 0; i < w; i++){
		myInd.Draw(float(i)/w);
		for (j = 0; j < h; j++){
			if(mouse_b)	return;
			wander = new Wanderer(i, j, &ljus);
			col = (map[i][j] - min)/(max - min);
			if (col > 1) col = 1;
			startcol = pal->GetColour(col);
			ColourSystem *shady = new ColourSystem(int(0.5 * getr(startcol)), 
                                                int(0.5 * getg(startcol)),
                                                int(0.5 * getb(startcol)),
						getr(startcol),
                                                getg(startcol),
                                                getb(startcol));
			shady->AddPoint(int(0.6 * getr(startcol)),
						int(0.6 * getg(startcol)),
						int(0.6 * getb(startcol)),
						int(0.4));
			shady->AddPoint(int(0.8 * getr(startcol)),
						int(0.8 * getg(startcol)),
						int(0.8 * getb(startcol)),
						int(0.6));
            
			Vektor *v = new Vektor(1, 0, map[Plus(i,1)][j]-map[i][j]);
			Vektor *u = new Vektor(0, 1, map[i][Plus(j, 1)]-map[i][j]);
			Vektor x = *v % *u;

			float facing = x ^ ljus;
			if(flags&SHADOW){
				float lightAlt = ljus.k / sqrt(ljus.i * ljus.i + ljus.j * ljus.j);
				wander->Step(); wander->Step();
				while(1){
					wander->Step();
					if (wander->steps > w)
						break;
					float deltAlt = map[wander->x & (w-1)][wander->y & (w-1)] - map[i][j];
					if (deltAlt / sqrt(pow(wander->x - i, 2) + pow(wander->y - j, 2)) > lightAlt){
						facing /= 2;
						break;
					}
				}
			}

			putpixel(cmap, i, j, shady->GetColour(facing));
			delete v;
			delete u;
			delete shady;
			delete wander;
		}
		blit(cmap, screen, i, 0, 280+i, 8, 1, h);
	}
}



void MAPGEN::render_point(int ,int ){

}
