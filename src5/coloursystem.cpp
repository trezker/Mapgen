#include "coloursystem.h"
#include <iostream>

Colourpoint::Colourpoint()
:p(0)
,r(0)
,g(0)
,b(0)
{
}

Colourpoint::Colourpoint(int ir, int ig, int ib, float ip)
:p(ip)
,r(ir/256.f)
,g(ig/256.f)
,b(ib/256.f)
{
}

void Colourpoint::Set_position(float ip)
{
	p = ip;
}

void Colourpoint::Set_colour(int ir, int ig, int ib)
{
	r = ir/256.f;
	g = ig/256.f;
	b = ib/256.f;
}

void Colourpoint::Set_colour_f(float ir, float ig, float ib)
{
	r = ir;
	g = ig;
	b = ib;
}

float Colourpoint::Get_r() const
{
	return r;
}

float Colourpoint::Get_g() const
{
	return g;
}

float Colourpoint::Get_b() const
{
	return b;
}

float Colourpoint::Get_p() const
{
	return p;
}

Colourpoint Colourpoint::Interpolate(const Colourpoint& c, float pin) const
{
	float nr = (c.r - r)/(c.p-p)*(pin-p) + r;
	float ng = (c.g - g)/(c.p-p)*(pin-p) + g;
	float nb = (c.b - b)/(c.p-p)*(pin-p) + b;
//	return int((c2 - c1)/(p2 - p1)*(pin - p1) + c1);
	Colourpoint n;
	n.Set_colour_f(nr, ng, nb);
	n.Set_position(pin);
	return n;
}


void Coloursystem::Add_colourpoint(const Colourpoint& c)
{
	for(Colourpoints::iterator i=colourpoints.begin(); i!= colourpoints.end(); ++i) {
		if(i->Get_p() > c.Get_p()) {
			colourpoints.insert(i, c);
			return;
		}
	}
	colourpoints.push_back(c);
}

Colourpoint& Coloursystem::Get_colourpoint(float pin)
{
	Colourpoints::iterator j = colourpoints.begin();
	for(Colourpoints::iterator i=colourpoints.begin(); i!= colourpoints.end(); ++i) {
		if(i->Get_p() > pin) {
			if(i->Get_p() - pin < pin - j->Get_p())
			{
				return *i;
			} else {
				return *j;
			}
		}
		j = i;
	}
	return colourpoints.back();
}

const Colourpoint& Coloursystem::Get_colourpoint(float pin) const
{
	Colourpoints::const_iterator j = colourpoints.begin();
	for(Colourpoints::const_iterator i=colourpoints.begin(); i!= colourpoints.end(); ++i) {
		if(i->Get_p() > pin) {
			if(i->Get_p() - pin < pin - j->Get_p())
			{
				return *i;
			} else {
				return *j;
			}
		}
		j = i;
	}
	return colourpoints.back();
}

void Coloursystem::Remove_colourpoint(float pin)
{
	Colourpoints::iterator j = colourpoints.begin();
	for(Colourpoints::iterator i=colourpoints.begin(); i!= colourpoints.end(); ++i) {
		if(i->Get_p() > pin) {
			if(i->Get_p() - pin < pin - j->Get_p())
			{
				colourpoints.erase(i);
				return;
			} else {
				colourpoints.erase(j);
				return;
			}
		}
		j = i;
	}
	colourpoints.pop_back();
}

Colourpoint Coloursystem::Interpolate(float pin) const
{
	Colourpoints::const_iterator j = colourpoints.begin();
	for(Colourpoints::const_iterator i=colourpoints.begin(); i!= colourpoints.end(); ++i) {
		if(i->Get_p() >= pin) {
			if(i==j) {
				return *i;
			}
			return j->Interpolate(*i, pin);
		}
		j = i;
	}
	Colourpoint n = colourpoints.back();
	return n;
}
