#ifndef mapgen_view3d_h
#define mapgen_view3d_h

#include <alledge/View.h>
#include <alledge/Scenenode.h>
#include <alledge/Cameranode.h>
#include <alledge/Lightnode.h>
#include <alledge/Quadnode.h>
#include <alledge/Transformnode.h>
#include <alledge/Heightmap.h>
#include <alledge/Bitmap.h>

class Map;

class View3D {
public:
	bool Init();
	
	void Build_map(const Map& map);
	shared_ptr<Bitmap> Get_texture();
	
	void Update(float dt);
	void Render();
	void Event(ALLEGRO_EVENT event);
private:
	Scenenode root;
	shared_ptr<Cameranode> camera;
	shared_ptr<Lightnode> light;
	shared_ptr<Transformnode> transform;
	shared_ptr<Heightmap> heightmap;
	shared_ptr<Bitmap> texture;
	shared_ptr<Bitmap> splat_texture;
	bool move_forward;
	bool move_backward;
	bool move_left;
	bool move_right;
	bool move_up;
	bool move_down;
	bool lmb;
	bool rmb;
	int mouse_x;
	int mouse_y;
	float fov;
	float near;
	float far;
	float width;
	float height;
	float camera_velocity;
};

#endif
