#include "diamond_square.h"

void Diamond_square::Generate(Map& map, int seed, float fractalfactor)
{
	mtrand.seed(seed);
	
	float amount = 1;
	int w = map.Get_width();
	int h = map.Get_height();
	// Do the corners
	map.Get_point(0, 0).elevation = 0;
	for (int i = w/2; i > 0; i /= 2){
		// This is the square phase
		for (int j = i; j < w; j += 2 * i)
			for (int k = i; k < h; k += 2 * i){
				Map_point* p[4] = {
					&map.Get_point(j-i, k-i),
					&map.Get_point(j-i, k+i),
					&map.Get_point(j+i, k+i),
					&map.Get_point(j+i, k-i)
				};
				Calc_midpoint(amount*1.41, map, p, i, j, k);
			}
		amount /= fractalfactor;

		// This is the diamond phase
		for (int j = 0; j < w; j += i)
			for (int k = 0; k < h; k += i)
				if ((((j + k) / i) % 2) == 1){
					Map_point* p[4] = {
						&map.Get_point(j-i, k),
						&map.Get_point(j, k-i),
						&map.Get_point(j+i, k),
						&map.Get_point(j, k+i)
					};
					Calc_midpoint(amount, map, p, i, j, k);
				}
		amount /= fractalfactor;
	}
	for(int x = 0; x<map.Get_width(); ++x) {
		for(int y = 0; y<map.Get_height(); ++y) {
			float average = 0;
			for(int ix=-1; ix<=1;++ix) {
				for(int iy=-1; iy<=1;++iy) {
					float h = map.Get_point(x+ix, y+iy).elevation;
					average += h/9;
				}
			}
			map.Get_point(x, y).elevation = average;
		}
	}
	for(int x = 0; x<map.Get_width(); ++x) {
		for(int y = 0; y<map.Get_height(); ++y) {
			map.Get_point(x, y).elevation *= map.Get_width()/2;
		}
	}
}

void Diamond_square::Calc_midpoint(float amount, Map& map, Map_point* p[4], int i, int j, int k)
{
	float r = FRandom(amount);
	map.Get_point(j, k).elevation = (
		p[0]->elevation +
		p[1]->elevation +
		p[2]->elevation +
		p[3]->elevation
		) / 4. + r;
}

float Diamond_square::FRandom(float amount)
{
	float h;
	float r;
	r = mtrand();
	h = amount * (r - 0.5);
	return h;
}
