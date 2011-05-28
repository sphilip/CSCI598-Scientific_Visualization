#include <cmath>

class Vector
{
  public:
    double x,y,z;

    Vector();
    Vector(double a, double b, double c);
    const Vector operator+ (const Vector& b) const;
    const Vector operator- (const Vector& b) const;
    const Vector operator- () const;
    const Vector operator* (double& x) const;
    Vector operator= (const Vector& b);
    double magnitude ();
    const Vector normalize ();
    bool operator< (const Vector& b);
    bool operator<= (const Vector& b);
    bool operator> (const Vector& b);
    bool operator>= (const Vector& b);
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
const Vector Vector::operator+ (const Vector& b) const
{
  Vector c;
  c.x = (this->x + b.x);
  c.y = (this->y + b.y);
  c.z = (this->z + b.z);

  return c;
}

/** Overloaded operator to subtract 2 vectors **/
const Vector Vector::operator- (const Vector& b) const
{
  Vector c;
  c.x = (this->x - b.x);
  c.y = (this->y - b.y);
  c.z = (this->z - b.z);

  return c;
}

/** Overloaded operator to negate **/
const Vector Vector::operator- () const
{
  return Vector(-this->x,-this->y,-this->z);
}

/** Overloaded operator to scalar mult **/
const Vector Vector::operator* (double& x) const
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
  return (sqrt(power));
}

/** Find normal vector **/
const Vector Vector::normalize()
{
  Vector result;
  double magnitude = this->magnitude();
  result.x = this->x*(1/magnitude);
  result.y = this->y*(1/magnitude);
  result.z = this->z*(1/magnitude);

  return result;
}

/** Overloaded inequalities **/
bool Vector::operator< (const Vector& b)
{
  if (this->x < b.x || this->y < b.y || this->z < b.z)
    return true;

  else return false;
}

bool Vector::operator<= (const Vector& b)
{
  if (this->x <= b.x || this->y <= b.y || this->z <= b.z)
    return true;

  else return false;
}


bool Vector::operator> (const Vector& b)
{
  if (this->x > b.x || this->y > b.y || this->z > b.z)
    return true;

  else return false;
}

bool Vector::operator>= (const Vector& b)
{
  if (this->x >= b.x || this->y >= b.y || this->z >= b.z)
    return true;

  else return false;
}

/** Calculate cross product from 2 given vectors) **/
Vector cross(const Vector& a, const Vector& b)
{
  Vector result;
  result.x = (a.y * b.z) - (a.z * b.y);
  result.y = (a.z * b.x) - (a.x * b.z);
  result.z = (a.x * b.y) - (a.y * b.x);

  return result;
}

/** Find dot product **/
double dot(const Vector& a, const Vector& b)
{
  return (a.x*b.x) + (a.y*b.y) + (a.z*b.z);
}

