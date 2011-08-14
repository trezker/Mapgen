#ifndef VEKTORHPP
#define VEKTORHPP

class Vektor
{
   
   
   public:
   float i, j, k;
   Vektor();
   Vektor(float ii, float ij, float ik);
   Vektor operator+(Vektor &annan);
   Vektor operator-(Vektor &annan);
   float operator*(Vektor &annan);   // dot product
   Vektor operator%(Vektor &annan);  // cross product
   float operator^(Vektor &annan);   // see vektor.cpp
   Vektor operator*(float k);
   Vektor operator/(float k);
   float Langd(void);  // Length
};

#endif
  
