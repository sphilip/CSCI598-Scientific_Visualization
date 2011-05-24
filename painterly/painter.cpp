#include <iostream>
#include <cstring>
#include <fstream>
#include <cstdlib>

#include "image.h"

using namespace std;

/** Global variables **/
image source;
image dest;
string outputFile;
int dimX, dimY;
int maxColor;
void read_input(char* file)
{
  ifstream infile(file, ios::in);

  if (!infile.is_open())
  {
    cout << "Unable to open file " << file << endl;
    return;
  }

  string comment;
  char code[2];
  infile >> code >> comment;
  infile >> dimX >> dimY;
  infile >> maxColor;
  infile.close();

  source = image(dimX,dimY);
  for (int j=0; j<dimY; j++)
  {
    RGB pixel;
    for (int i=0; i<dimX; i++)
    {
      infile >> pixel.r >> pixel.g >> pixel.b;
      source.rgb[j*dimX+i] = pixel;
    }
  }

for (int j=0; j<dimY; j++)
{
  for (int i=0; i<dimX; i++)
  {
    cout << source.rgb[j*dimX+i].r << "\t" << source.rgb[j*dimX+i].g << "\t" << source.rgb[j*dimX+i].b << endl;
  }
}
}

int main(int argc, char* argv[])
{


  if (argc == 4)
  {
    string filename;
    filename = (string) argv[1];
    int dirEnd(0);
    for (int i=0; i<filename.size(); i++)
    {
      if (filename[i] == '/')
      {
	dirEnd = i+1;
	break;
      }
    }

    outputFile = filename.substr(0,dirEnd)  + "out_" + filename.substr(dirEnd, filename.size());

    dimX = atoi(argv[2]);
    dimY = atoi(argv[3]);
    read_input(argv[1]);
  }

  else
  {
    cout << "Execute the Painterly Rendering program as follows\n\t" << argv[0] << "<input image as .ppm> <height of output image> <width of output image>" << endl;
    return -1;
  }
  return 0;
}