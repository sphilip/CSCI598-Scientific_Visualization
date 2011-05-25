#include <cmath>

class Vector
{
  public:
    double x,y,z;

    Vector();
    Vector(double a, double b, double c);
    Vector& operator+ (const Vector& b);
    Vector& operator- (const Vector& b);
    Vector& operator- ();
    Vector& operator* (double& x);
    Vector& operator= (const Vector& b);
    double magnitude ();
    Vector& normalize ();
    bool operator< (const Vector& b);
    bool operator<= (const Vector& b);
};

/** Default construct, initialize to 0 **/
Vector::Vector()
{
  x = 0;
  y = 0;
  z = 0;
}

/** Construct w/  parameter list **/
Vector::Vector(double a, double b, double c)
{
  x = a;
  y = b;
  z = c;
}

/** Overloaded operator to add 2 vectors **/
Vector& Vector::operator+ (const Vector& b)
{
  Vector c;
  c.x = this->x + b.x;
  c.y = this->y + b.y;
  c.z = this->z + b.z;

  this->x = c.x;
  this->y = c.y;
  this->z = c.z;
  return *this;
}

/** Overloaded operator to subtract 2 vectors **/
Vector& Vector::operator- (const Vector& b)
{
  Vector c;
  c.x = this->x - b.x;
  c.y = this->y - b.y;
  c.z = this->z - b.z;

  this->x = c.x;
  this->y = c.y;
  this->z = c.z;
  return *this;
}

/** Overloaded operator to negate **/
Vector& Vector::operator- ()
{
  this->x = -this->x;
  this->y = -this->y;
  this->z = -this->z;
  return *this;
}

/** Overloaded operator to scalar mult **/
Vector& Vector::operator* (double& x)
{
  Vector c;
  c.x = this->x * x;
  c.y = this->y * x;
  c.z = this->z * x;

  this->x = c.x;
  this->y = c.y;
  this->z = c.z;

  return *this;
}

/** Overloaded operator to assign vector b to this vector **/
Vector& Vector::operator= (const Vector& b)
{
  this->x = b.x;
  this->y = b.y;
  this->z = b.z;

  return *this;
}

/** Find magnitude of this vector **/
double Vector::magnitude()
{
  return (sqrt(pow(this->x,2) + pow(this->y,2) + pow(this->z,2)));
}

/** Find normal vector **/
Vector& Vector::normalize()
{
  double magnitude = this->magnitude();
  this->x = this->x*(1/magnitude);
  this->y = this->y*(1/magnitude);
  this->z = this->z*(1/magnitude);
}

bool Vector::operator< (const Vector& b)
{
  if (this->x < b.x && this->y < b.y && this->z < b.z)
    return true;

  else return false;
}

bool Vector::operator<= (const Vector& b)
{
  if (this->x <= b.x && this->y <= b.y && this->z <= b.z)
    return true;

  else return false;
}