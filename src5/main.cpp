#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_opengl.h>
#include "map.h"
#include <iostream>
#include <cmath>
#include "mtrand.h"

#include "view3d.h"

MTRand mtrand;

float FRandom(float amount){
	float h;
	float r;
	r = mtrand();
	h = amount * (r - 0.5);
	return h;
}

void Calc_midpoint(float amount, Map& map, Map_point* p[4], int i, int j, int k) {
	float r = FRandom(amount);
	map.Get_point(j, k).elevation = (
		p[0]->elevation +
		p[1]->elevation +
		p[2]->elevation +
		p[3]->elevation
		) / 4. + r;
}

int main() {
	mtrand.seed(1337);
	Map map;
	map.Set_size(256, 256);
	
	srand(1390891825);
	//float fractalfactor = sqrt(2);
	float fractalfactor = 1.6;
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
			map.Get_point(x, y).elevation *= map.Get_width()/2;//rand()/float(RAND_MAX);
		}
	}

	
	al_init();
	al_install_mouse();
	al_install_keyboard();
	al_init_font_addon();
	al_init_ttf_addon();

	ALLEGRO_DISPLAY *display;
	al_set_new_display_flags(ALLEGRO_WINDOWED | ALLEGRO_OPENGL);
	al_set_new_display_option(ALLEGRO_DEPTH_SIZE, 24, ALLEGRO_REQUIRE);
 	display = al_create_display(800, 600);
 	if(!display)
 	{
 		std::cout<<"Failed to create display"<<std::endl;
 		return 0;
	}

	ALLEGRO_EVENT_QUEUE *event_queue = al_create_event_queue();
	al_register_event_source(event_queue, (ALLEGRO_EVENT_SOURCE *)display);
	al_register_event_source(event_queue, al_get_keyboard_event_source());
	al_register_event_source(event_queue, al_get_mouse_event_source());

	View3D view3d;

	if(!view3d.Init())
		return 0;

	view3d.Build_map(map);

	double last_time = al_current_time();

	bool quit = false;
	while(1)
	{
		ALLEGRO_EVENT event;
		while (al_get_next_event(event_queue, &event))
		{
		  	if (ALLEGRO_EVENT_KEY_DOWN == event.type)
			{
				if (ALLEGRO_KEY_ESCAPE == event.keyboard.keycode)
				{
					quit = true;
				}
			}
			if (ALLEGRO_EVENT_DISPLAY_CLOSE == event.type)
			{
				quit = true;
			}
			view3d.Event(event);
		}
		if (quit)
			break;

		double current_time = al_current_time();
		double dt = current_time - last_time;
		last_time = current_time;
		view3d.Update(dt);

		al_clear_to_color(al_map_rgb(0, 0, 0));
		view3d.Render();
		al_flip_display();

		al_rest(0.001);
	}

	al_destroy_event_queue(event_queue);
	al_destroy_display(display);
	return 0;
}
