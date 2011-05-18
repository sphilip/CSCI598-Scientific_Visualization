/** RGB class handles the colors **/

class RGB
{
  public:
    float r,g,b;

    float max() {
      return (r<g ? (g<b ? b : g) : (r<b ? b : r));
    }
    void scale ( float s ) {
      r*=s;
      g*=s;
      b*=s;
    }
};