#include <iostream>
#include <cstring>
#include <fstream>
#include <cstdlib>
#include <vector>

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
  ifstream infile(file, ios::in | ios::binary);
  if (!infile)
  {
    cout << "Unable to open file " << file << endl;
    return;
  }

  char* buffer = new char[256];

  // skip code
  infile.getline(buffer,256);

  // skip over comments
  while ((char) infile.peek() == '#')
    infile.getline(buffer,256);

  delete[] buffer;

  infile >> inX >> inY; // grab image dimensions
  infile >> maxColor;  // grab max color

  source = image(inX,inY); // construct image

  // find size of image
  int start = infile.tellg();
  infile.seekg(0, ios_base::end);
  int end = infile.tellg();
  int size = end-start;

  // make sure that the number of pixels in file matches the dimensions
  if (inX*inY != size/3)
  {
    cout << "Dimensions of image doesn't match the number of pixels available" << endl;
    return;
  }


  RGB pixel;
  pixel.r = 0;
  pixel.g = 0;
  pixel.b = 0;

  vector<RGB> src;
  src.resize(inX*inY,pixel);

//   RGB* img = new RGB[size/3];
  char r,g,b;
  int j=0;

  infile.seekg(start+1,ios_base::beg);
  for (int i=0; i<size; i=i+3)
  {
    // read RGB values
    infile.readsome(&r,sizeof(char));
    infile.readsome(&g,sizeof(char));
    infile.readsome(&b,sizeof(char));

    // add 255 to pixel values below 0
    if ((double) r < 0)
      pixel.r = (double) r + maxColor;

    else
      pixel.r = (double) r;

    if ((double) g < 0)
      pixel.g = (double) g + maxColor;

    else
      pixel.g = (double) g;

    if ((double) b < 0)
      pixel.b = (double) b + maxColor;

    else
      pixel.b = (double) b;

    //     source.rgb[j] = pixel;
    src[i/3] = pixel;

    pixel.r=0;
    pixel.g=0;
    pixel.b=0;
    j++;
    //infile.sync();
  }

  for (unsigned int i=0; i<src.size(); i++)
  {
    cout << "at " << i << "\t" << src[i].r << "\t" << src[i].g << "\t" << src[i].b << endl;
  }

  infile.close();
//   delete[] img;

  return;
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
  source.save_to_ppm_file(outputFile.c_str());

  return 0;
}