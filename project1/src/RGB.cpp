#include "RGB.h"

/** Default constructor **/
RGB::RGB()
{
  r=0;
  g=0;
  b=0;
}

/** User-defined constructor **/
RGB::RGB(double x, double y, double z)
{
  r = x;
  b = y;
  g = z;
}

/** Addition operator to add colors together **/
RGB RGB::operator+ (  RGB& a)
{
  RGB result;
  result.r = (this->r + a.r);
  result.g = (this->g + a.g);
  result.b = (this->b + a.b);

  return result;
}

/** Assignment operator to save colors **/
void RGB::operator= (const RGB& a)
{
  this->r = a.r;
  this->g = a.g;
  this->b = a.b;

//   return *this;
}