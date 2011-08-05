/** RGB class handles the colors **/

class RGB
{
  public:
    double r,g,b;

    RGB();
    RGB(double x, double y, double z);

    double max() {
      return (r<g ? (g<b ? b : g) : (r<b ? b : r));
    }

    void scale ( double s ) {
      r*=s;
      g*=s;
      b*=s;
    }

    RGB operator+ ( RGB& a) ;
    void operator= (const RGB& a);
};
