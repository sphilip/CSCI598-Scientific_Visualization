#include <cmath>

class Vector
{
  public:
    double x,y,z;

    Vector() : x(0.0), y(0.0), z(0.0) {};
    Vector(double a, double b, double c) : 
        x(a), y(b), z(c) {};
    Vector operator+ (const Vector& b) ;
    Vector operator- ( Vector& b) ;
    Vector operator- () ;
    Vector operator* (const double& x) ;
    Vector operator= (const Vector& b);
    double magnitude ();
    Vector normalize ();
    bool operator< ( Vector& b);
    bool operator<= ( Vector& b);
    bool operator> ( Vector& b);
    bool operator>= ( Vector& b);
    bool operator== (const Vector& b);

    double dot(const Vector& b);
    Vector cross(const Vector& b);
};
