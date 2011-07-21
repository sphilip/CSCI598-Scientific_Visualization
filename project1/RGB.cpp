#include "RGB.h"

RGB::RGB()
{
  r=0.0;
  g=0.0;
  b=0.0;
}

RGB::RGB(float x, float y, float z)
{
  r=x;
  b=y;
  g=z;
}

const RGB RGB::operator+ (const RGB& a) const
{
  RGB result;
  result.r = (this->r + a.r);
  result.g = (this->g + a.g);
  result.b = (this->b + a.b);

  return result;
}