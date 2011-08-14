#include<math.h>
#ifndef M_PI
   #define M_PI   3.14159
#endif
#include"main.h"
#include"map.h"
int polys_drawn;

void view3d(MAPGEN &mapgen, float hflat, float lflat){
	lc=0;
	MAP map(mapgen,hflat,lflat);

	BITMAP *buffer=create_bitmap(480,320);
	ZBUFFER *zbuf=create_zbuffer(buffer);
	set_zbuffer(zbuf);
	MATRIX_f roller,camera;

//	create_scene(4*(mapgen.size()*mapgen.size()), (mapgen.size()*mapgen.size()));
	set_projection_viewport(0, 0, buffer->w, buffer->h);

	float xfront, yfront, zfront;
	float xup, yup, zup;
	float heading=0,pitch=0,roll=0;
	int fov=32;
	float x=128,y=128,z=128;
	float xs=0,ys=0,zs=0;

	int fps=0,frames[101];
	for(int i=0;i<101;i++)
		frames[i]=0;

	int mx,my,tc;
	//To get rid of that annoying roll I reset mickeys.
	get_mouse_mickeys(&mx,&my);
	while(!(mouse_b || key[KEY_ESC])){
		tc=lc;
		lc=0;
		while(!mouse_b && tc){
			tc--;

			fps-=frames[100];
			for(int i=100;i>0;i--)
				frames[i]=frames[i-1];
			frames[0]=0;
			fps+=frames[1];

/*			if(key[KEY_Q])
				y++;
			if(key[KEY_Z])
				y--;
			if(key[KEY_W]){
				x+=sin(heading);
				z+=cos(heading);
			}
			if(key[KEY_S]){
				x-=sin(heading);
				z-=cos(heading);
			}
			if(key[KEY_A]){
				x+=sin(heading+M_PI/2);
				z+=cos(heading+M_PI/2);
			}
			if(key[KEY_D]){
				x-=sin(heading+M_PI/2);
				z-=cos(heading+M_PI/2);
			}
*/
			if(key[KEY_Q])
				ys++;
			if(key[KEY_Z])
				ys--;
			if(key[KEY_W]){
				xs+=sin(heading);
				zs+=cos(heading);
			}
			if(key[KEY_S]){
				xs-=sin(heading);
				zs-=cos(heading);
			}
			if(key[KEY_A]){
				xs+=sin(heading+M_PI/2);
				zs+=cos(heading+M_PI/2);
				roll+=.02;
			}
			if(key[KEY_D]){
				xs-=sin(heading+M_PI/2);
				zs-=cos(heading+M_PI/2);
				roll-=.02;
			}
	
			xs*=.99;
			ys*=.9;
			zs*=.99;
			x+=xs;
			y+=ys;
			z+=zs;

			get_mouse_mickeys(&mx,&my);
			position_mouse(SCREEN_W/2,SCREEN_H/2);
			heading-=mx/100.;
			pitch-=my/100.;
			if(pitch>M_PI/3) pitch=M_PI/3;
			if(pitch<-M_PI/3) pitch=-M_PI/3;
			
			roll-=mx/100.;
			roll*=.95;

/*			if(x>=0 && z>=0 && x/tilesize<mapgen.size()  && z/tilesize<mapgen.size())
				y=200+mapgen.heightmap()[int(x/tilesize)][int(z/tilesize)]*tilesize;
*/		}

		/* calculate the in-front vector */
		xfront = sin(heading) * cos(pitch);
		yfront = sin(pitch);
		zfront = cos(heading) * cos(pitch);

		/* rotate the up vector around the in-front vector by the roll angle */
		get_vector_rotation_matrix_f(&roller, xfront, yfront, zfront, roll*128.0/M_PI);
		apply_matrix_f(&roller, 0, 1, 0, &xup, &yup, &zup);

		/* build the camera matrix */
		get_camera_matrix_f(&camera, x, y, z, xfront, yfront, zfront, xup, yup, zup, fov, 1);

		polys_drawn=0;
		clear_bitmap(buffer);
//		clear_scene(buffer);
		clear_zbuffer(zbuf,0);
		map.render(buffer,&camera,int(x),int(y));
//		render_scene();
		textprintf_ex(buffer,font,8,8,-1,-1,"FPS: %i",fps);
		textprintf_ex(buffer,font,8,16,-1,-1,"Polygon count: %i",polys_drawn);
		blit(buffer, screen, 0, 0, 280, 8, buffer->w, buffer->h);
		frames[0]++;
	}
	destroy_bitmap(buffer);
	destroy_zbuffer(zbuf);
//	destroy_scene();
	while(mouse_b || key[KEY_ESC]);
	clear_keybuf();
}
