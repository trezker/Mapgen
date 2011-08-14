#ifndef mapgen_h
#define mapgen_h

#include"vektor.hpp"
#include"ColourSystem.hpp"

#define SHADOW 1

class MAPGEN{
	int msize;
	float **map;
	BITMAP *cmap;

	Vektor ljus;

	//Helpers for wrapping the edges.
	int Minus(int j, int i);
	int Plus(int j, int i);
	// Returns something between -amount/2 and amount/2
	float FRandom(float amount);
public:
	ColourSystem *pal;
	int flags;

	//Creates a map generator with initial map size.
	//Currently only square 2^n sizes are possible.
	MAPGEN(int isize);
	//Keep the memory clean.
	~MAPGEN();
	//Change the size of the map, any generated map disappears.
	void resize(int isize);
	
	//Access to map info.
	int size();
	float **heightmap();
	BITMAP *colormap();

	//Creates a heightmap from your seed.
	void create(float fractalfactor, int seed);

	//Rendering functions.
	void set_light(float az, float alt);
	void render();
	void render_point(int x,int y);
};

#endif
