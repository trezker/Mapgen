#include<math.h>
#include "vektor.hpp"

Vektor::Vektor(){i=j=k=0;}

Vektor::Vektor(float ii, float ij, float ik)
: i(ii), j(ij), k(ik)
{
}

Vektor Vektor::operator+(Vektor &annan)
{
   Vektor hj = *this;
   hj.i += annan.i;
   hj.j += annan.j;
   hj.k += annan.k;
   return hj;
}

Vektor Vektor::operator-(Vektor &annan)
{
   Vektor hj = *this;
   hj.i -= annan.i;
   hj.j -= annan.j;
   hj.k -= annan.k;
   return hj;
}

Vektor Vektor::operator*(float k)
{
   Vektor hj = *this;
   hj.i *= k;
   hj.j *= k;
   hj.k *= k;
   return hj;
}

Vektor Vektor::operator/(float k)
{
   Vektor hj = *this;
   hj.i /= k;
   hj.j /= k;
   hj.k /= k;
   return hj;
}

float Vektor::operator*(Vektor &annan)
{
   return i * annan.i + j * annan.j + k * annan.k;
}

float Vektor::operator^(Vektor &annan)
{
   float hj;
   hj = (*this * annan)/Langd()/annan.Langd();
   // hj = cos(angle between the vectors)
   hj /= 2.0;
   hj += 0.5;
   // 0 <= hj <= 1, easier to use as a parameter 
   return hj;
}

Vektor Vektor::operator%(Vektor &annan)
{
   Vektor hj(j * annan.k - annan.j * k,
             annan.i * k - i * annan.k,
             i * annan.j - annan.i * j);
   return hj;
}

float Vektor::Langd(void)
{
   return sqrt(i * i + j * j + k * k);
}

   
