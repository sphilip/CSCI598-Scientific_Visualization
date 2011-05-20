/**
Shanley Philip
Project 1: Volume Rendering
CSCI 598
**/

#include <iostream>
#include <fstream>
#include <cstring>
#include <stdlib.h>

#include "image.h"
#include "Vector.h"

using namespace std;

/** Global Variables **/
string binaryFile; // name of raw file
double x,y,z; // dimensions of raw file
int dimX, dimY;  // dimensions of output file
int* map = NULL; // holds the pixel values of image
Vector camera;

struct Square
{
  Vector p,q,r,s;
};

struct BoundingBox
{
  Square front,back,left,right,top,bottom;
} bbox;


/** Setup the dimensions of the bounding box **/
void defineBox()
{
  // front face
  Square front;
  front.p = Vector(0,0,0); // origin
  front.q = Vector(x,0,0); // along x
  front.r = Vector(0,y,0); // along y
  front.s = Vector(x,y,0); // along x,y
  bbox.front = front;

  // back face --
  //   same thing as front but translated along z
  Square back;
  back.p = Vector(0,0,z); // origin
  back.q = Vector(x,0,z); // along x
  back.r = Vector(0,y,z); // along y
  back.s = Vector(x,y,z); // along x,y
  bbox.back = back;

  // left face --
  //   p of left face = q of front;
  //   s of left = s of back
  Square left;
  left.p = Vector(x,0,0); // origin
  left.q = Vector(x,0,z); // along z
  left.r = Vector(x,y,0); // along y
  left.s = Vector(x,y,z); // along z,y
  bbox.left = left;

  // right face --
  //   p of right face = q of back;
  //   q of right = p of front
  //   r of right = s of back
  //   s of right = r of front
  Square right;
  right.p = Vector(0,0,z); // origin
  right.q = Vector(0,0,0); // along z
  right.r = Vector(0,y,z); // along y
  right.s = Vector(0,y,0); // along z,y
  bbox.right = right;

  // top face --
  //   p of top face = r of front;
  //   q of top = s of front
  //   r of top = r of back
  //   s of top = s of back
  Square top;
  top.p = Vector(0,y,0); // origin
  top.q = Vector(x,y,0); // along z
  top.r = Vector(0,y,z); // along y
  top.s = Vector(x,y,z); // along z,y
  bbox.top = top;

  // bottom face --
  //   p of bottom face = p of front;
  //   q of bottom = q of front
  //   r of bottom = p of back
  //   s of bottom = q of back
  Square bottom;
  bottom.p = Vector(0,0,0); // origin
  bottom.q = Vector(x,0,0); // along z
  bottom.r = Vector(0,0,z); // along y
  bottom.s = Vector(x,0,z); // along z,y
  bbox.bottom = bottom;

}

/** Read & translate binary file **/
void readBinaryFile(image* img)
{
  FILE* file = fopen(binaryFile.c_str(), "rb");

  if (file==NULL)
  {
    cout << "Couldn't open file " << binaryFile << endl;
  }

  else
  {
    unsigned char* buffer;
    int length;

    // get length of file:
    fseek(file, 0, ios::end);
    length = ftell(file);
    rewind(file);

    // allocate memory:
    buffer = new unsigned char [length];
    map = new int [length];

    // read data as a block:
    fread(map,sizeof(char),length,file);
    fclose(file);

    //     cout << length << "\t" << length/sizeof(int) << endl;
   // RGB pixel[length];
    for(int i=0; i<length; i++)
    {
      if (map[i] > 255)
	map[i] = 255;

      if (map[i] < 0)
	map[i] = 0;

      //       cout << map[i] << "\t";
    /*  if (i%3 == 0)
	pixel[i].r = map[i];

      else if (i%3 == 1)
	pixel[i].g = map[i];

      else
	pixel[i].b = map[i];
*/
    }

    //img->rgb = pixel;
    delete[] buffer;
  }

  return;
}


/** Open txt file and save the descriptors **/
void parseFile(char* filename)
{

  ifstream infile(filename, ios::in);
  if (!infile)
  {
    cout << "Couldn't open file " << filename << endl;

  }

  else
  {
    infile >> binaryFile;
    infile >> x >> y >> z;

    infile.close();
  }

  return;
}


/** Extract the name of the output image from the input name **/
string extractOutputName(char* input)
{
  string temp = string(input);
  string output;
  for (int i=0; i<temp.length(); i++)
  {
    if (temp[i] == '.')
      output = temp.substr(0,i);
  }
  /*int length(0), i(0);
  while (input != NULL)
  {
    if (input[i] != ".")
      length++;

    else break;
  }
*/
  output = output + ".ppm";
  return output;
}

/** Driver **/
int main(int argc, char** argv)
{
  if (argc < 3)
  {
    cout << "To execute, must include the image description file\n\t" << argv[0] << "<image description (must be .txt format)> <dimensions of output image>\n";
  }

  else
  {
    cout << "Reading " << argv[1] << endl;
    dimX = atoi(argv[2]);
    dimY = atoi(argv[3]);
    image img = image(dimX,dimY);

    string outputFile = extractOutputName(argv[1]);
    parseFile(argv[1]);
    readBinaryFile(&img);
    defineBox();

    img.save_to_ppm_file(outputFile.c_str());
  }

  return 0;
}