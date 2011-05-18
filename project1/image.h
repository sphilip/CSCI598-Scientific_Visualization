/** Image class stores the pixel values as an array and later writes the image to ppm file **/

#include "RGB.h"

using namespace std;

class image
{
  int xsize,ysize; // resolution
  RGB *rgb;        // pixel intensities

  public:
    image (int m, int n);// allocates image of specified size
    RGB &pixel ( int i, int j );  // access to a specific pixel
    void save_to_ppm_file ( char *filename );
    void scale();
};

