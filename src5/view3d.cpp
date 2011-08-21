#include "view3d.h"
#include "map.h"
#include <allegro5/allegro_opengl.h>
#include <iostream>

shared_ptr<Bitmap> View3D::Get_texture()
{
	return texture;
}

bool View3D::Init()
{
	fov = 45.f;
	near = 1.f;
	far = 1000.f;
	width = 640;
	height = 480;

	move_forward = false;
	move_backward = false;
	move_left = false;
	move_right = false;
	move_up = false;
	move_down = false;
	lmb = false;
	rmb = false;
	mouse_x = 0;
	mouse_y = 0;

	camera = new Cameranode();
	camera->Set_position(Vector3(-20, 150, -20));
	camera->Set_rotation(Vector3(-30, -135, 0));
	root.Attach_node(camera);

	light = new Lightnode;
	light->Set_ambient(0.2, 0.2, 0.2, 1);
	light->Set_diffuse(0.8, 0.8, 0.8, 1);
	light->Set_specular(0.4, 0.4, 0.4, 1);
//	light->Set_position(Vector3(1, 1, 1), 0);
	light->Set_position(Vector3(100, 100, 100), 1);
	camera->Attach_node(light);
	
	transform = new Transformnode;
	light->Attach_node(transform);

	texture = new Bitmap;
	texture->Create(512, 512);
	texture->Set_target();
	al_clear_to_color(al_map_rgb_f(1, 1, 1));
	al_set_target_backbuffer(al_get_current_display());

	splat_texture = new Bitmap;
	splat_texture->Create(512, 512);
	splat_texture->Set_target();
	al_clear_to_color(al_map_rgba_f(1, 0, 0, 0));
	al_set_target_backbuffer(al_get_current_display());

	heightmap = new Heightmap;
	heightmap->Set_tilesize(1);
	heightmap->Resize(4, 4);
	heightmap->Set_texture(texture, 0);
	heightmap->Set_texture(texture, 1);
	heightmap->Set_splat_texture(splat_texture);
	transform->Attach_node(heightmap);
	return true;
}

void View3D::Build_map(const Map& map, float height_scale, float sealevel)
{
	int width = map.Get_width();
	int height = map.Get_height();
	float cw = -width;///2.0;
	float ch = -height;///2.0;
	heightmap->Resize(width, height);
	for(int x = 0; x<width; ++x){
		for(int y = 0; y<height; ++y){
			float h = map.Get_point(x, y).Get_elevation();
			heightmap->Set_height(x, y, h>sealevel? h * height_scale : sealevel*height_scale);
		}
	}
	heightmap->Recalc_normals();
}

void View3D::Update(float dt)
{
	camera_velocity = 50;
	if(move_forward)
	{
		camera->Set_position(camera->Get_position() + camera->Get_front() * dt*camera_velocity);
	}
	if(move_backward)
	{
		camera->Set_position(camera->Get_position() - camera->Get_front() * dt*camera_velocity);
	}
	if(move_left)
	{
		camera->Set_position(camera->Get_position() - camera->Get_right() * dt*camera_velocity);
	}
	if(move_right)
	{
		camera->Set_position(camera->Get_position() + camera->Get_right() * dt*camera_velocity);
	}
	if(move_up)
	{
		camera->Set_position(camera->Get_position() + camera->Get_up() * dt*camera_velocity);
	}
	if(move_down)
	{
		camera->Set_position(camera->Get_position() - camera->Get_up() * dt*camera_velocity);
	}

	if(rmb)
	{
		Init_perspective_view(fov, width/height, near, far);
		Vector3 oglpoint = Unproject(mouse_x, mouse_y, camera);
		Pop_view();

//		std::cout<<"oglpoint x, y, z: "<<oglpoint.x<<", "<<oglpoint.y<<", "<<oglpoint.z<<std::endl;
		float curve[5] = {-1, -.7, 0, .3, 0};
		heightmap->Apply_brush(oglpoint.x, oglpoint.z, 10, 1*dt, curve, 5);
	}
}

void View3D::Render()
{
	Init_perspective_view(fov, width/height, near, far);

	glEnable(GL_DEPTH_TEST);
 	glClear(GL_DEPTH_BUFFER_BIT);
	glEnable(GL_LIGHTING);

	root.Apply();

	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);

	Pop_view();
}


void View3D::Event(ALLEGRO_EVENT event)
{
	if (ALLEGRO_EVENT_KEY_DOWN == event.type)
	{
		if (ALLEGRO_KEY_W == event.keyboard.keycode)
		{
			move_forward = true;
		}
		if (ALLEGRO_KEY_S == event.keyboard.keycode)
		{
			move_backward = true;
		}
		if (ALLEGRO_KEY_A == event.keyboard.keycode)
		{
			move_left = true;
		}
		if (ALLEGRO_KEY_D == event.keyboard.keycode)
		{
			move_right = true;
		}
		if (ALLEGRO_KEY_R == event.keyboard.keycode)
		{
			move_up = true;
		}
		if (ALLEGRO_KEY_F == event.keyboard.keycode)
		{
			move_down = true;
		}
	}
	if (ALLEGRO_EVENT_KEY_UP == event.type)
	{
		if (ALLEGRO_KEY_W == event.keyboard.keycode)
		{
			move_forward = false;
		}
		if (ALLEGRO_KEY_S == event.keyboard.keycode)
		{
			move_backward = false;
		}
		if (ALLEGRO_KEY_A == event.keyboard.keycode)
		{
			move_left = false;
		}
		if (ALLEGRO_KEY_D == event.keyboard.keycode)
		{
			move_right = false;
		}
		if (ALLEGRO_KEY_R == event.keyboard.keycode)
		{
			move_up = false;
		}
		if (ALLEGRO_KEY_F == event.keyboard.keycode)
		{
			move_down = false;
		}
	}
	if (ALLEGRO_EVENT_MOUSE_BUTTON_DOWN == event.type)
	{
		if(event.mouse.button == 1)
		{
			lmb = true;
		}
		if(event.mouse.button == 2)
		{
			rmb = true;
		}
	}
	if (ALLEGRO_EVENT_MOUSE_BUTTON_UP == event.type)
	{
		if(event.mouse.button == 1)
		{
			lmb = false;
		}
		if(event.mouse.button == 2)
		{
			rmb = false;
		}
	}
	if (ALLEGRO_EVENT_MOUSE_AXES == event.type)
	{
		if(lmb)
		{
			camera->Set_rotation(camera->Get_rotation() + Vector3(-event.mouse.dy, -event.mouse.dx, 0));
		}
		mouse_x = event.mouse.x;
		mouse_y = event.mouse.y;
	}
}
