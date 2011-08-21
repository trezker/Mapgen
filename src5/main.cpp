#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_opengl.h>
#include <allegro5/allegro_primitives.h>
#include "map.h"
#include <iostream>
#include <cmath>
#include "mtrand.h"
#include "diamond_square.h"
#include "coloursystem.h"

#include "view3d.h"


int main() {
	MTRand_int32 seedrand;

	Map map;
	map.Set_size(256, 256);
	Diamond_square diamond_square;
	diamond_square.Generate(map, 1337, 1.6);

	Coloursystem coloursystem;
	Colourpoint colourpoint;
	colourpoint.Set_position(0);
	colourpoint.Set_colour_f(1, 0, 0);
	coloursystem.Add_colourpoint(colourpoint);
	colourpoint.Set_position(1);
	colourpoint.Set_colour_f(0, 1, 0);
	coloursystem.Add_colourpoint(colourpoint);

	for(int x = 0; x<map.Get_width(); ++x) {
		for(int y = 0; y<map.Get_height(); ++y) {
			map.Get_point(x, y).elevation *= map.Get_width()/2;
		}
	}

	al_init();
	al_install_mouse();
	al_install_keyboard();
	al_init_font_addon();
	al_init_ttf_addon();
	al_init_primitives_addon();

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
				if(ALLEGRO_KEY_N == event.keyboard.keycode)
				{
					diamond_square.Generate(map, seedrand(), 1.6);
					view3d.Build_map(map);
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
		
		for(int x = 0; x<800; ++x) {
			Colourpoint c = coloursystem.Interpolate(x/800.f);
			ALLEGRO_COLOR color;
			color = al_map_rgb_f(c.Get_r(), c.Get_g(), c.Get_b());
			al_draw_line(x, 0, x, 10, color, 0);
		}
		
		al_flip_display();

		al_rest(0.001);
	}

	al_destroy_event_queue(event_queue);
	al_destroy_display(display);
	return 0;
}
