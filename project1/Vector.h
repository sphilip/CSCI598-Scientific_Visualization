class Vector
{
  public:
    double x,y,z;

    Vector();
    Vector(double a, double b, double c);
    Vector& operator+ (const Vector b);
    Vector& operator- (const Vector b);
    Vector& operator* (double x);
    Vector& operator= (const Vector b);
};

Vector::Vector()
{
  x = 0;
  y = 0;
  z = 0;
}

Vector::Vector(double a, double b, double c)
{
  x = a;
  y = b;
  z = c;
}

Vector& Vector::operator+ (const Vector b)
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

Vector& Vector::operator- (const Vector b)
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

Vector& Vector::operator* (double x)
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

Vector& Vector::operator= (const Vector b)
{
  this->x = b.x;
  this->y = b.y;
  this->z = b.z;

  return *this;
}