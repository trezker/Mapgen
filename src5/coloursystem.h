#ifndef mapgen_coloursystem_h
#define mapgen_coloursystem_h

#include <vector>

class Colourpoint {
public:
	Colourpoint();
	Colourpoint(int r, int g, int b, float p);
	void Set_position(float p);
	void Set_colour(int r, int g, int b);
	void Set_colour_f(float r, float g, float b);
	float Get_r() const;
	float Get_g() const;
	float Get_b() const;
	float Get_p() const;
	Colourpoint Interpolate(const Colourpoint& c, float pin) const;
private:
	float p;
	float r;
	float g;
	float b;
};

class Coloursystem {
public:
	void Add_colourpoint(const Colourpoint& c);
	Colourpoint& Get_colourpoint(float pin);
	const Colourpoint& Get_colourpoint(float pin) const;
	void Remove_colourpoint(float pin);
	Colourpoint Interpolate(float pin) const;
private:
	typedef std::vector<Colourpoint> Colourpoints;
	Colourpoints colourpoints;
};

#endif
