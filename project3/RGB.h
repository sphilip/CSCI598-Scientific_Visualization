#ifndef RGB_H
#define RGB_H

class RGB
{
public:
  int r,g,b;
  
  RGB(){r = 0; g=0; b=0;}
  
  RGB(int rr, int gg, int bb) {r = rr; g = gg; b = bb;}
  
  float max() {
    return (r<g ? (g<b ? b : g) : (r<b ? b : r));
  }
  
  void scale ( float s ) {
    r*=s;
    g*=s;
    b*=s;
  }
};

#endif
