#include <iostream>
#include <cstring>
#include <fstream>
#include <cstdlib>

#include "image.h"

using namespace std;

/** Global variables **/
image source; // input image
image dest;   // new image
string outputFile; // name of output image
int dimX, dimY; // dimensions of output image
int inX, inY;  // dimensions of input image
int maxColor; // max color used in image

void read_input(char* file)
{
  ifstream infile(file, ios::in);

  if (!infile)
  {
    cout << "Unable to open file " << file << endl;
    return;
  }

  char* code = new char[1024];
  char* comment = new char[1024];
  infile >> code;
  infile >> comment;
  infile >> inX >> inY;
  infile >> maxColor;


  source = image(inX, inY);
  for (int j=0; j<inY; j++)
  {
    RGB pixel;
    for (int i=0; i<inX; i++)
    {
      infile >> pixel.r >> pixel.g >> pixel.b;
      source.rgb[j*inX+i] = pixel;
    }
  }
  infile.close();

  for (int j=0; j<inY; j++)
  {
    for (int i=0; i<inX; i++)
    {
      cout << source.rgb[j*inX+i].r << "\t" << source.rgb[j*inX+i].g << "\t" << source.rgb[j*dimX+i].b << endl;
    }
  }
}

int main(int argc, char* argv[])
{
  if (argc != 2)
  {
    cout << "Execute the Painterly Rendering program as follows\n\t" << argv[0] << " <input image as .ppm> <height of output image> <width of output image>" << endl;
    return -1;
  }

  string filename;
  filename = (string) argv[1];
  int dirEnd(0);
  for (unsigned int i=0; i<filename.size(); i++)
  {
    if (filename[i] == '/')
    {
      dirEnd = i+1;
      break;
    }
  }

  outputFile = filename.substr(0,dirEnd)  + "out_" + filename.substr(dirEnd, filename.size());

  //dimX = atoi(argv[2]);
  //dimY = atoi(argv[3]);
  read_input(argv[1]);

  return 0;
}