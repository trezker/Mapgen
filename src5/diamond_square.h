#ifndef mapgen_diamond_square_h
#define mapgen_diamond_square_h

#include "mtrand.h"
#include "map.h"

class Diamond_square {
public:
	void Generate(Map& map, int seed, float fractalfactor);
private:
	void Calc_midpoint(float amount, Map& map, Map_point* p[4], int i, int j, int k);
	float FRandom(float amount);
	MTRand mtrand;
};

#endif
