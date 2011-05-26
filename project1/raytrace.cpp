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

/** Struct defining the dimensions of 1 side of a box **/
struct Square
{
  Vector p,q,r,s;
  Vector normal;
  string name;
};

/** Details of the box enclosing the object **/
struct BoundingBox
{
  Square face[6];
  double t[6];
} bbox;

/** Ray -- self explanatory  **/
struct Ray
{
  Vector origin, direction;
};

/** Global Variables **/
string binaryFile; // name of raw file
double x,y,z; // dimensions of raw file
int dimX, dimY;  // dimensions of output file
int* map = NULL; // holds the pixel values of image

Vector lower_left = Vector(); // the lower left corner is always the origin;
Vector horizontal; // which direction is the horizontal pointing
Vector vertical; // which direction is the vertical pointing
Ray eye; // eye ray has origin @ center of cube and scans the entire cube (hence, direction changes)

int step_size; //  the distance between voxels to render the output image

/** Calculate cross product from 2 given vectors) **/
Vector cross(const Vector& a, const Vector& b)
{
  Vector result;
  result.x = (a.y * b.z) - (a.z * b.y);
  result.y = (a.z * b.x) - (a.x * b.z);
  result.z = (a.x * b.y) - (a.y * b.x);

  return result;
}

/** Find dot product **/
double dot(const Vector& a, const Vector& b)
{
  return (a.x*b.x) + (a.y*b.y) + (a.z*b.z);
}


/** Find the intersection between the eye ray & a given face of the bounding box **/
double ray_plane_intersection(Square face)
{
  // when ray and pt on plane intersects,
  //     p = o + td  pt = origin + t*direction
  //     hence: (p-o)/d = t
  Vector po = eye.origin - face.p; // p-o
  Vector pq = face.q - face.p;
  Vector pr = face.r - face.p;
  face.normal = cross(pq, pr).normalize(); // normal faces into cube

  double num = dot(po, face.normal);
  double denom = dot(eye.direction, face.normal);

  if (denom == 0)
    return -1;

  double t = -(num/denom);

  if (t < 0)
    return -1;

  else
  {
    Vector pt = eye.origin + (eye.direction*t);
    if (pt < face.p && pt < face.q && pt < face.r && pt < face.s)
    {
      return t;
    }

    else return -1;
  }
}


/** Calculate direction that the eye ray is pointing **/
void calculateDirection(int i, int j)
{
  eye.direction.x = (((i+0.5)*horizontal.x)/dimX) + (((j+0.5)*vertical.x)/dimY);
  eye.direction.y = (((i+0.5)*horizontal.y)/dimX) + (((j+0.5)*vertical.y)/dimY);
  eye.direction.z = (((i+0.5)*horizontal.z)/dimX) + (((j+0.5)*vertical.z)/dimY);
}
/** Setup the dimensions of the bounding box **/
void defineBox()
{
  // front face
  Square front;
  front.name = "front";
  front.p = Vector(0,0,0); // origin
  front.q = Vector((x-1),0,0); // along x
  front.r = Vector(0,(y-1),0); // along y
  front.s = Vector((x-1),(y-1),0); // along x,y
  bbox.face[0] = front;

  // back face --
  //   same thing as front but translated along z
  Square back;
  back.name = "back";
  back.p = Vector(0,0,(z-1)); // origin
  back.q = Vector((x-1),0,(z-1)); // along x
  back.r = Vector(0,(y-1),(z-1)); // along y
  back.s = Vector((x-1),(y-1),(z-1)); // along x,(y-1)
  bbox.face[1] = back;

  // right face --
  //   p of right face = q of front;
  //   s of right = s of back
  Square right;
  right.name = "right";
  right.p = Vector((x-1),0,0); // origin
  right.q = Vector((x-1),0,(z-1)); // along z
  right.r = Vector((x-1),(y-1),0); // along y
  right.s = Vector((x-1),(y-1),(z-1)); // along z,y
  bbox.face[2] = right;

  // left face --
  //   p of left face = q of back;
  //   q of left = p of front
  //   r of left = s of back
  //   s of left = r of front
  Square left;
  left.name = "left";
  left.p = Vector(0,0,(z-1)); // origin
  left.q = Vector(0,0,0); // along z
  left.r = Vector(0,(y-1),(z-1)); // along y
  left.s = Vector(0,(y-1),0); // along z,y
  bbox.face[3] = left;

  // top face --
  //   p of top face = r of front;
  //   q of top = s of front
  //   r of top = r of back
  //   s of top = s of back
  Square top;
  top.name = "top";
  top.p = Vector(0,(y-1),0); // origin
  top.q = Vector((x-1),(y-1),0); // along z
  top.r = Vector(0,(y-1),(z-1)); // along y
  top.s = Vector((x-1),(y-1),(z-1)); // along z,y
  bbox.face[4] = top;

  // bottom face --
  //   p of bottom face = p of front;
  //   q of bottom = q of front
  //   r of bottom = p of back
  //   s of bottom = q of back
  Square bottom;
  bottom.name = "bottom";
  bottom.p = Vector(0,0,0); // origin
  bottom.q = Vector((x-1),0,0); // along z
  bottom.r = Vector(0,0,(z-1)); // along y
  bottom.s = Vector((x-1),0,(z-1)); // along z,y
  bbox.face[5] = bottom;
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
	map[i] = 255 + map[i];

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
    infile >> step_size;

    horizontal = Vector(x-1,0,0);
    vertical = Vector(0,y-1,0);

    // define camera position
    eye.origin = Vector(x/2, y/2, -z/2);
    infile.close();
  }

  return;
}


/** Extract the name of the output image from the input name.  Save as dir/filename.ppm **/
string extractOutputName(char* input)
{
  string temp = string(input);
  string output;
  for (unsigned int i=0; i<temp.length(); i++)
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
    return -1;
  }

  cout << "Reading " << argv[1] << endl;
  dimX = atoi(argv[2]);
  dimY = atoi(argv[3]);
  image img = image(dimX,dimY);

  // Get name for output image from input name
  string temp = string(argv[1]);
  string outputFile;
  for (unsigned int i=0; i<temp.length(); i++)
  {
    // save everything except .txt
    if (temp[i] == '.')
      outputFile = temp.substr(0,i);
  }

  outputFile = outputFile + ".ppm";

  parseFile(argv[1]);
  readBinaryFile(&img);
  defineBox();


  // iterate through each pixel in output image
  for (int j=0; j<dimY; j++)
  {
    for (int i=0; i<dimX; i++)
    {
      // calculate direction vector of eye ray
      calculateDirection(i,j);

      // array of possible t-values
      double* t = new double[6];
      for (int k=0; k<6; k++)
	t[k] = ray_plane_intersection(bbox.face[k]);

      // min & max possible t for intersections between bounding box & eye ray
      double min_t(99999.09), max_t(-1);

      for (int k=0; k<6; k++)
      {
	if (t[k] < min_t)
	  min_t = t[k];

	if (max_t > t[k])
	  max_t = t[k];
      }

      delete[] t;

      cout << "At (" << i << ", " << j << "): " << min_t << "\t" << max_t << endl;
    }
  }

//   img.save_to_ppm_file(outputFile.c_str());

  return 0;
}