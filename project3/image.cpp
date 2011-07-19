#include "image.h"  
#include <cmath>
#include <fstream>
#include <cassert>

/* ----------- image class: methods ---------- */
image::image ( int m, int n ) : xsize(m), ysize(n)
{
  rgb = new RGB[m*n];
}

/* ----------------------- */

RGB &image::pixel ( int i, int j )
{
  return rgb[i+xsize*j];
}

/* ----------------------- */

static unsigned char clampnround ( float x )
{
  if (x>255)
    x = 255;
  if (x<0)
    x = 0;
  return (unsigned char)floor(x+.5);
}

/* ----------------------- */

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

//  for ( i=0; i<xsize*ysize; i++ )
  //  rgb[i].scale(m0);
}

/* ----------------------- */

void image::save_to_ppm_file ( char *filename )
{
  std::ofstream ofs(filename,std::ios::binary);
  assert(ofs);
  ofs << "P6" << std::endl;
  ofs << xsize << " " << ysize << std::endl << 255 << std::endl;
  for ( int i=0; i<xsize*ysize; i++ ) {
    unsigned char r = clampnround(rgb[i].r);
    unsigned char g = clampnround(rgb[i].g);
    unsigned char b = clampnround(rgb[i].b);
    
    ofs.write((char*)&r,sizeof(char));
    ofs.write((char*)&g,sizeof(char));
    ofs.write((char*)&b,sizeof(char));
  }
}

