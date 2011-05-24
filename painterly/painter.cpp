#include <iostream>
#include <cstring>
#include <fstream>
#include "image.h"

using namespace std;

/** Global variables **/
image source;
image dest;
int dimX, dimY;

void read_input(char* file)
{
  ifstream infile(file);

  if (!infile.is_open())
  {
    cout << "Unable to open file " << file << endl;
    return;
  }

  string ppm;
  infile >> ppm;
  infile >> dimX >> dimY;
  infile.close();

  ifstream ppmFile(ppm.c_str());
  if (!ppmFile.is_open())
  {
    cout << "Unable to open ppm file " << ppm << endl;
    return;
  }

  source = image(dimX,dimY);
  for (int j=0; j<dimY; j++)
  {
    RGB pixel;
    for (int i=0; i<dimX; i++)
    {
      ppmFile >> pixel.r >> pixel.g >> pixel.b;
      source.rgb[j*dimX+i] = pixel;
    }
  }
}

int main(int argc, char* argv[])
{
  string filename;

  if (argc == 2)
  {
    filename = argv[1];
  }

  else
  {
    cout << "Execute the Painterly Rendering program as follows\n\t" << argv[0] << "<input image as .ppm>" << endl;
  }
  return 0;
}