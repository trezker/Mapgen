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

	Coloursystem coloursystem;
/*
	coloursystem.Add_colourpoint(Colourpoint(255, 0, 0, 0));
	coloursystem.Add_colourpoint(Colourpoint(0, 255, 0, 1));
*/
/*	Blue/green
	coloursystem.Add_colourpoint(Colourpoint(0, 0, 255, 0));
	coloursystem.Add_colourpoint(Colourpoint(0, 240, 0, 1));
	coloursystem.Add_colourpoint(Colourpoint(0, 0, 255, 0.49));
	coloursystem.Add_colourpoint(Colourpoint(0, 240, 0, 0.51));
*/
/*	//Land only
	coloursystem.Add_colourpoint(Colourpoint(0, 220, 0, 0));
	coloursystem.Add_colourpoint(Colourpoint(255, 255, 255, 1));
	coloursystem.Add_colourpoint(Colourpoint(10, 120, 5, 0.1));
	coloursystem.Add_colourpoint(Colourpoint(192, 140, 70, 0.3));
	coloursystem.Add_colourpoint(Colourpoint(160, 80, 10, 0.5));
	coloursystem.Add_colourpoint(Colourpoint(50, 40, 30, 0.65));
	coloursystem.Add_colourpoint(Colourpoint(99, 99, 99, 0.8));
	coloursystem.Add_colourpoint(Colourpoint(255, 255, 244, 0.9));
*/
//	Grey
/*
	coloursystem.Add_colourpoint(Colourpoint(0, 0, 0, 0));
	coloursystem.Add_colourpoint(Colourpoint(255, 255, 255, 1));
	coloursystem.Add_colourpoint(Colourpoint(20, 20, 20, 0.45));
	coloursystem.Add_colourpoint(Colourpoint(190, 190, 190, 0.5));
	coloursystem.Add_colourpoint(Colourpoint(190, 190, 190, 0.55));
	coloursystem.Add_colourpoint(Colourpoint(100, 100, 100, 0.551));
	coloursystem.Add_colourpoint(Colourpoint(30, 30, 30, 0.75));
	coloursystem.Add_colourpoint(Colourpoint(60, 60, 60, 0.9));
	coloursystem.Add_colourpoint(Colourpoint(255, 255, 255, 0.95));
*/
// Natural
	coloursystem.Add_colourpoint(Colourpoint(0, 0, 200, 0));
	coloursystem.Add_colourpoint(Colourpoint(255, 255, 255, 1));
	coloursystem.Add_colourpoint(Colourpoint(0, 0, 255, 0.45));
	coloursystem.Add_colourpoint(Colourpoint(0, 255, 255, 0.5));
	coloursystem.Add_colourpoint(Colourpoint(255, 159, 31, 0.51));
	coloursystem.Add_colourpoint(Colourpoint(255, 159, 31, 0.55));
	coloursystem.Add_colourpoint(Colourpoint(19, 179, 11, 0.555));
	coloursystem.Add_colourpoint(Colourpoint(0, 119, 0, 0.6));
	coloursystem.Add_colourpoint(Colourpoint(219, 186, 140, 0.7));
	coloursystem.Add_colourpoint(Colourpoint(125, 77, 0, 0.85));
	coloursystem.Add_colourpoint(Colourpoint(250, 250, 250, 0.9));

/*
	ColourSystem pal4(0, 0, 200, 255, 255, 255);
	pal4.AddPoint(0, 0, 255, 0.45);
	pal4.AddPoint(0, 255, 255, 0.5);
	pal4.AddPoint(255, 159, 31, 0.51);
	pal4.AddPoint(255, 159, 31, 0.55);
	pal4.AddPoint(19, 179, 11, 0.555);
	pal4.AddPoint(0, 119, 0, 0.6);
	pal4.AddPoint(219, 186, 140, 0.7);
	pal4.AddPoint(125, 77, 0, 0.85);
	pal4.AddPoint(250, 250, 250, 0.9);
*/
	Map map;
	map.Set_size(256, 256);

	Diamond_square diamond_square;

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
					map.Normalize();
					
					shared_ptr<Bitmap> texture = view3d.Get_texture();
					texture->Create(map.Get_width(), map.Get_height());
					texture->Set_target();
					al_clear_to_color(al_map_rgba_f(1, 1, 0, 1));
					for(int x = 0; x<map.Get_width(); ++x) {
						for(int y = 0; y<map.Get_height(); ++y) {
				//			std::cout<<map.Get_point(x, y).elevation<<std::endl;
							Colourpoint colourpoint = coloursystem.Interpolate(map.Get_point(x, y).elevation);
							ALLEGRO_COLOR color = al_map_rgb_f(colourpoint.Get_r(), colourpoint.Get_g(), colourpoint.Get_b());
							al_draw_pixel(x+0.5f, map.Get_height()-y-0.5f, color);
						}
					}
					al_set_target_backbuffer(al_get_current_display());

					for(int x = 0; x<map.Get_width(); ++x) {
						for(int y = 0; y<map.Get_height(); ++y) {
							map.Get_point(x, y).elevation *= map.Get_width()/2;
						}
					}

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
