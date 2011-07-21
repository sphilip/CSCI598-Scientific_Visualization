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
int inX,inY,inZ; // dimensions of raw file
int dimX, dimY;  // dimensions of output file
int* map = NULL; // holds the pixel values of image

Vector lower_left; // the lower left corner is always the origin;
Vector horizontal; // which direction is the horizontal pointing
Vector vertical; // which direction is the vertical pointing
Ray eye; // eye ray has origin @ center of cube and scans the entire cube (hence, direction changes)

double step_size; //  the distance between voxels to render the output image
double ka,kd,ks; // coefficients for calculating illumination
double I_lightsource; // initial intensity
double alpha_i; // initial opacity
Vector light; // location of light source
double sigma; // slab of samples that light must travel through
double isovalue;


/** Calculate illumination due to diffuse material **/
RGB diffuse_term(const Vector& pt, Vector& gradient)
{
  RGB diffuse(0.0,0.0,0.0);
  double magn = gradient.magnitude();
  if (magn == 0)
    return diffuse;

  Vector pg = cross(pt,gradient);
  pg = pg.normalize();

  //   return kd*I_lightsource*abs(dot(pg,light));
  diffuse.r = 255.0*abs(dot(pg,light));
  diffuse.g = 255.0*abs(dot(pg,light));
  diffuse.b = 255.0*abs(dot(pg,light));

  return diffuse;
}


/** Find illumination due to specular **/
double specular_term(const Vector& pt, const Vector& gradient)
{
  Vector LV = light + pt;
  double LV_magn = LV.magnitude();
  Vector halfway = LV*pow(LV_magn,-1);
// cout << LV_magn << endl;

//   return ks*I_lightsource*dot(cross(gradient,pt).normalize(), halfway);
return 1;
}

/** Find trilinear interpolation based off of given values
   e=> x-----x <=f
      /|    /|
 a=> x-----x <=b
  g=>| x---|-x <=h
     |/    |/
 c=> x-----x <=d **/
double trilinear_interpolation(double x_val, double y_val, double z_val)
{
  int index  = floor(x_val)*inX*inZ + ceil(y_val)*inY + floor(z_val);
  float a_val = map[index];

  index = ceil(x_val)*inX*inZ + ceil(y_val)*inY + floor(z_val);
  float b_val = map[index];

  index = floor(x_val)*inX*inZ + floor(y_val)*inY + floor(z_val);
  float c_val = map[index];

  index = ceil(x_val)*inX*inZ + floor(y_val)*inY + floor(z_val);
  float d_val = map[index];

  index = floor(x_val)*inX*inZ + ceil(y_val)*inY + ceil(z_val);
  float e_val = map[index];

  index = ceil(x_val)*inX*inZ + ceil(y_val)*inY + ceil(z_val);
  float f_val = map[index];

  index = floor(x_val)*inX*inZ + floor(y_val)*inY + ceil(z_val);
  float g_val = map[index];

  index = ceil(x_val)*inX*inZ + floor(y_val)*inY + ceil(z_val);
  float h_val = map[index];

  double weight_x = ceil(x_val) - x_val;
  double ab_contrib = weight_x*a_val + (1-weight_x)*b_val;
  double cd_contrib = weight_x*c_val + (1-weight_x)*d_val;
  double ef_contrib = weight_x*e_val + (1-weight_x)*f_val;
  double gh_contrib = weight_x*g_val + (1-weight_x)*h_val;

  double weight_y = ceil(y_val) - y_val;
  double abcd_contrib = weight_y*ab_contrib + (1-weight_y)*cd_contrib;

  double efgh_contrib = weight_y*ef_contrib + (1-weight_y)*gh_contrib;

  double weight_z = ceil(z_val) - z_val;
  return weight_z*abcd_contrib + (1-weight_z)*efgh_contrib;

}

void test_trilinear()
{
  double value = trilinear_interpolation(0.5,0.5,0.5);

  cout << "(0.5,0.5,0.5)= " << value << endl << endl;
  cout << "(0,0,0)= " << map[0] << endl;
  cout << "(1,0,0)= " << map[1*inX*inZ] << endl;
  cout << "(1,1,0)= " << map[1*inX*inZ+1*inY] << endl;
  cout << "(0,1,0)= " << map[1*inY] << endl;

  cout << "(0,0,1)= " << map[1] << endl;
  cout << "(1,0,1)= " << map[1*inX*inZ+1] << endl;
  cout << "(1,1,1)= " << map[1*inX*inZ+1*inY+1] << endl;
  cout << "(0,1,1)= " << map[1*inY+1] << endl;
}

/** Calculate transfer function (ie. pts on surface containing similar values) **/
double transfer_function(const Vector& pt, Vector& return_gradient)
{
  double h=0.005;
  Vector gradient;

  gradient.x = (trilinear_interpolation(pt.x+h,pt.y,pt.z) - trilinear_interpolation(pt.x-h,pt.y,pt.z))/(2*h);
  gradient.y = (trilinear_interpolation(pt.x, pt.y+h, pt.z) - trilinear_interpolation(pt.x,pt.y-h, pt.z))/(2*h);
  gradient.z = (trilinear_interpolation(pt.x, pt.y, pt.z+h) - trilinear_interpolation(pt.x, pt.y, pt.z-h))/(2*h);

  double current_alpha;
  double value = abs( trilinear_interpolation(pt.x,pt.y,pt.z) - isovalue)/gradient.magnitude();

  if (value < sigma)
  {
    current_alpha = 1 - (1/sigma) * value;
  }

  else
    current_alpha = 0;

  return_gradient = gradient;
  return current_alpha;
}


void test_transfer()
{
  Vector test(1,1,1);
  Vector test_gradient;
  double value = transfer_function(test, test_gradient);
  cout << value << endl;
  return;
}

RGB illumination(const Vector& pt, Vector& gradient)
{
  // calculate diffuse
  RGB diffuse(0.0,0.0,0.0);
  double magn = gradient.magnitude();
  if (magn == 0)
    return diffuse;

  Vector pg = cross(pt,gradient);
  pg = pg.normalize();

  //   return kd*I_lightsource*abs(dot(pg,light));
  diffuse.r = 255.0*abs(dot(pg,light));
  diffuse.g = 255.0*abs(dot(pg,light));
  diffuse.b = 255.0*abs(dot(pg,light));

  // calculate specular
  //   RGB specular = specular_term(pt, gradient);

  return diffuse;
}


/** Take samples from min to max t (ie. front to back) and return the total intensity. **/
RGB front_to_back_compositing(double mint, double maxt)
{
  double alpha = alpha_i;
//   double final_I = 0.0;

//   int steps = (maxt - mint)/step_size;
  Vector pt, gradient;

  //   double alpha_i;
  double current_step = mint;
  RGB intensity = RGB(0.0,0.0,0.0);

  while (current_step < floor(maxt))
//   for (int step=0; step<steps; step++)
  {
    if (alpha <= 0.00001)
      break;

    pt = eye.origin + (eye.direction*current_step);
    alpha_i = transfer_function(pt, gradient);

    //     final_I = final_I + (alpha*init_I);
    RGB illuminate = illumination(pt, gradient);

    intensity.r = intensity.r + (current_step*alpha_i*alpha*illuminate.r);
    intensity.g = intensity.g + (current_step*alpha_i*alpha*illuminate.g);
    intensity.b = intensity.b + (current_step*alpha_i*alpha*illuminate.b);

//     final_I = (exp(-sigma*alpha_i)*init_I) + final_I;

    //     alpha = alpha*(1-alpha_i);
    alpha = alpha * exp(-current_step*alpha_i);

    current_step +=step_size;
    //     cout << current_step << endl;
  }

//   cout << alpha << "\t" << final_I << "\t" << steps << endl;
// RGB illumin;
// illumin.r = final_I;
// illumin.g = final_I;
// illumin.b = final_I;

  return intensity;
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

  if (denom <= 0)
    return -1;

  double t = -(num/denom);

  if (t < 0)
    return -1;

  else
  {
    Vector pt = eye.origin + (eye.direction*t);
    if (pt >= face.p && pt <= face.q && pt >= face.r && pt <= face.s)
    {
      return t;
    }

    else return -1;
  }
}


/** Calculate direction that the eye ray is pointing **/
void calculateDirection(int i, int j)
{
  eye.direction.x = (((i+0.5)*horizontal.x)/dimX) + (((j+0.5)*vertical.x)/dimY) - eye.origin.x;
  eye.direction.y = (((i+0.5)*horizontal.y)/dimX) + (((j+0.5)*vertical.y)/dimY) - eye.origin.y;
  eye.direction.z = (((i+0.5)*horizontal.z)/dimX) + (((j+0.5)*vertical.z)/dimY) - eye.origin.z;
}
/** Setup the dimensions of the bounding box **/
void defineBox()
{
  // front face
  Square front;
  front.name = "front";
  front.p = Vector(0,0,0); // origin
  front.q = Vector((inX-1),0,0); // along inX
  front.r = Vector(0,(inY-1),0); // along inY
  front.s = Vector((inX-1),(inY-1),0); // along inX,inY
  bbox.face[0] = front;

  // back face --
  //   same thing as front but translated along inZ
  Square back;
  back.name = "back";
  back.p = Vector(0,0,(inZ-1)); // origin
  back.q = Vector((inX-1),0,(inZ-1)); // along inX
  back.r = Vector(0,(inY-1),(inZ-1)); // along inY
  back.s = Vector((inX-1),(inY-1),(inZ-1)); // along inX,(inY-1)
  bbox.face[1] = back;

  // right face --
  //   p of right face = q of front;
  //   s of right = s of back
  Square right;
  right.name = "right";
  right.p = Vector((inX-1),0,0); // origin
  right.q = Vector((inX-1),0,(inZ-1)); // along inZ
  right.r = Vector((inX-1),(inY-1),0); // along inY
  right.s = Vector((inX-1),(inY-1),(inZ-1)); // along inZ,inY
  bbox.face[2] = right;

  // left face --
  //   p of left face = q of back;
  //   q of left = p of front
  //   r of left = s of back
  //   s of left = r of front
  Square left;
  left.name = "left";
  left.p = Vector(0,0,(inZ-1)); // origin
  left.q = Vector(0,0,0); // along inZ
  left.r = Vector(0,(inY-1),(inZ-1)); // along inY
  left.s = Vector(0,(inY-1),0); // along inZ,inY
  bbox.face[3] = left;

  // top face --
  //   p of top face = r of front;
  //   q of top = s of front
  //   r of top = r of back
  //   s of top = s of back
  Square top;
  top.name = "top";
  top.p = Vector(0,(inY-1),0); // origin
  top.q = Vector((inX-1),(inY-1),0); // along inZ
  top.r = Vector(0,(inY-1),(inZ-1)); // along inY
  top.s = Vector((inX-1),(inY-1),(inZ-1)); // along inZ,inY
  bbox.face[4] = top;

  // bottom face --
  //   p of bottom face = p of front;
  //   q of bottom = q of front
  //   r of bottom = p of back
  //   s of bottom = q of back
  Square bottom;
  bottom.name = "bottom";
  bottom.p = Vector(0,0,0); // origin
  bottom.q = Vector((inX-1),0,0); // along inZ
  bottom.r = Vector(0,0,(inZ-1)); // along inY
  bottom.s = Vector((inX-1),0,(inZ-1)); // along inZ,inY
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
//     unsigned char* buffer;
    int length;

    // get length of file:
    fseek(file, 0, ios::end);
    length = ftell(file);
    rewind(file);

    // allocate memory:
//     buffer = new unsigned char [length];
    map = new int [length];

    // read data as a block:
    fread(map,sizeof(char),length,file);
    fclose(file);

    for(int i=0; i<length; i++)
    {
      if (map[i] > 255)
	map[i] = 255;

      if (map[i] < 0)
	map[i] = 255 + map[i];
    }

//     delete[] buffer;
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
    infile >> inX >> inY >> inZ;
    infile >> step_size;
    infile >> ka;
    infile >> kd;
    infile >> ks;
    infile >> I_lightsource;
    infile >> light.x >> light.y >> light.z;
    infile >> alpha_i;
    infile >> sigma;
    infile >> isovalue;
    infile.close();

    // define screen orientation
    horizontal = Vector(inX-1,0,0);
    vertical = Vector(0,inY-1,0);

    // define camera position
    eye.origin = Vector(inX/2, inY/2, -inZ/2);

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
    cout << "To execute, must include the image description file\n\t" << argv[0] << " <image description (must be .txt format)> <dimensions of output image>\n";
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
      double min_t(-1), max_t(-1);

      for (int k=0; k<6; k++)
      {
	if ((t[k] < min_t || min_t == -1) && t[k] != -1)
	  min_t = t[k];

	if (t[k] > max_t && t[k] != -1)
	  max_t = t[k];
      }

      delete[] t;

      //       cout << "At (" << i << ", " << j << "):\t" << min_t << "\t" << max_t << endl;

      RGB color;
      if (min_t != -1 && max_t != -1)
      {
	color = front_to_back_compositing(min_t, max_t);
      }

      else
	color = RGB(255.0,255.0,255.0);

      img.rgb[j*dimX+i] = color;
    }
  }

  //   test_trilinear();
  //   test_transfer();

  img.save_to_ppm_file(outputFile.c_str());

  return 0;
}