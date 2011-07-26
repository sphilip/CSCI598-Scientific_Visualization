/** RGB class handles the colors **/

class RGB
{
  public:
    float r,g,b;

    RGB();
    RGB(float x, float y, float z);

    float max() {
      return (r<g ? (g<b ? b : g) : (r<b ? b : r));
    }
    void scale ( float s ) {
      r*=s;
      g*=s;
      b*=s;
    }

    const RGB operator+ (const RGB& a) const;
    RGB operator= (const RGB& a);
};
