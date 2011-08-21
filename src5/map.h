#ifndef mapgen_map_h
#define mapgen_map_h
#include <vector>

class Map_point {
public:
	float Get_elevation() const;
	float elevation;
};

class Map {
public:
	void Set_size(int width, int height);
	int Get_width() const;
	int Get_height() const;
	Map_point& Get_point(int x, int y);
	const Map_point& Get_point(int x, int y) const;
private:
	typedef std::vector< Map_point > Map_strip;
	typedef std::vector< Map_strip > Map_field;
	Map_field map_field;
};

#endif
