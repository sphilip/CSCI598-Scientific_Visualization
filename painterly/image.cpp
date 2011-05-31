#include <fstream>
#include <cassert>
#include <cmath>

#include "image.h"
/* ----------- image class: methods ---------- */
// Constructor for image
image::image ()
{
  xsize = 0;
  ysize = 0;
  printf("WARNING: no size for rgb array\n");
}

image::image ( int m, int n ) : xsize(m), ysize(n)
{
  rgb = new RGB[m*n];
}

/* ----------------------- */
// Get color
RGB &image::pixel ( int i, int j )
{
  return rgb[i+xsize*j];
}

/* ----------------------- */
// Round pixel elements > 255 & < 0
static unsigned char clampnround ( float x )
{
  if (x>255)
    x = 255;
  if (x<0)
    x = 0;
  return (unsigned char)floor(x+.5);
}

/* ----------------------- */
// Scale the image
void image::scale()
{
  int i;
  float m0 = 0;
  for ( i=0; i<xsize*ysize; i++ ) {
    float m = rgb[i].max();
    if (m>m0)
      m0 = m;
  }
  if (m0==0)
    m0 = 1;
  else
    m0 = 1/m0;

  for ( i=0; i<xsize*ysize; i++ )
    rgb[i].scale(m0);
}

/* ----------------------- */
// Save image to ppm
void image::save_to_ppm_file (const char *filename )
{
  ofstream ofs(filename,ios::binary);
  assert(ofs);
  ofs << "P6" << endl;
  ofs << xsize << " " << ysize << endl << 255 << endl;
  for ( int i=0; i<xsize*ysize; i++ ) {
    unsigned char r = clampnround(255*rgb[i].r);
    unsigned char g = clampnround(255*rgb[i].g);
    unsigned char b = clampnround(255*rgb[i].b);
    ofs.write((char*)&r,sizeof(char));
    ofs.write((char*)&g,sizeof(char));
    ofs.write((char*)&b,sizeof(char));
  }
}