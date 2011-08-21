#include "map.h"
#include <iostream>

float Map_point::Get_elevation() const
{
	return elevation;
}


void Map::Set_size(int width, int height)
{
	if(width<1 || height<1) {
		throw "Size less than 1.";
	}
	map_field.resize(width);
	for(Map_field::iterator i = map_field.begin(); i != map_field.end(); ++i) {
		i->resize(height);
	}
}

int Map::Get_width() const
{
	return map_field.size();
}

int Map::Get_height() const
{
	return map_field[0].size();
}

Map_point& Map::Get_point(int x, int y)
{
	while(x<0) x += Get_width();
	while(y<0) y += Get_height();
	while(x>=Get_width()) x -= Get_width();
	while(y>=Get_height()) y -= Get_height();
	return map_field[x][y];
}

const Map_point& Map::Get_point(int x, int y) const
{
	while(x<0) x += Get_width();
	while(y<0) y += Get_height();
	while(x>=Get_width()) x -= Get_width();
	while(y>=Get_height()) y -= Get_height();
	return map_field[x][y];
}
