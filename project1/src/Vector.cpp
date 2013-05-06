#include "Vector.h"

/** Overloaded operator to add 2 vectors **/
Vector Vector::operator+ (const Vector& b)
{
  Vector c;
  c.x = (this->x + b.x);
  c.y = (this->y + b.y);
  c.z = (this->z + b.z);

  return c;
}

/** Overloaded operator to subtract 2 vectors **/
 Vector Vector::operator- ( Vector& b)
{
  Vector c;
  c.x = (this->x - b.x);
  c.y = (this->y - b.y);
  c.z = (this->z - b.z);

  return c;
}

/** Overloaded operator to negate **/
 Vector Vector::operator- ()
{
  return Vector(-this->x,-this->y,-this->z);
}

/** Overloaded operator to scalar mult **/
 Vector Vector::operator* (const double& x)
{
  Vector c;
  c.x = (this->x * x);
  c.y = (this->y * x);
  c.z = (this->z * x);

  return c;
}

/** Overloaded operator to assign vector b to this vector **/
Vector Vector::operator= (const Vector& b)
{
  this->x = b.x;
  this->y = b.y;
  this->z = b.z;

  return *this;
}

/** Find magnitude of this vector **/
double Vector::magnitude()
{
  double power = pow(this->x,2) + pow(this->y,2) + pow(this->z,2);

  return sqrt(power);
}

/** Find normal vector **/
 Vector Vector::normalize()
{
  Vector result;
  double magnitude = this->magnitude();
  result.x = this->x/magnitude;
  result.y = this->y/magnitude;
  result.z = this->z/magnitude;

  return result;
}

/** Overloaded inequalities **/
bool Vector::operator< ( Vector& b)
{
  if (this->x < b.x || this->y < b.y || this->z < b.z)
    return true;

  else return false;
}

bool Vector::operator<= ( Vector& b)
{
  if (this->x <= b.x && this->y <= b.y && this->z <= b.z)
    return true;

  else return false;
}


bool Vector::operator> ( Vector& b)
{
  if (this->x > b.x || this->y > b.y || this->z > b.z)
    return true;

  else return false;
}

bool Vector::operator>= ( Vector& b)
{
  if (this->x >= b.x && this->y >= b.y && this->z >= b.z)
    return true;

  else return false;
}

bool Vector::operator== (const Vector& b)
{
  if (this->x == b.x && this->y == b.y && this->z == b.z)
    return true;

  else return false;
}

/** Calculate cross product from 2 given vectors **/
Vector Vector::cross(const Vector& b)
{
  Vector result;
  result.x = (this->y * b.z) - (this->z * b.y);
  result.y = (this->z * b.x) - (this->x * b.z);
  result.z = (this->x * b.y) - (this->y * b.x);

  return result;
}

/** Find dot product **/
double Vector::dot(const Vector& b)
{
  return (this->x*b.x) + (this->y*b.y) + (this->z*b.z);
}

