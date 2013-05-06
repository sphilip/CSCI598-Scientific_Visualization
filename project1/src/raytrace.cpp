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
RGB ka,kd,ks; // coefficients for calculating illumination
double I_lightsource; // initial intensity
double alpha_v; // initial opacity
Vector lightsource; // location of light source
double sigma; // slab of samples that light must travel through
double isovalue;

Vector gradient;
RGB intensity;
/** End global variables **/


/** Calculate illumination due to diffuse material **/
// RGB diffuse_term(const Vector& pt, Vector& gradient)
// {
//   RGB diffuse(0.0,0.0,0.0);
//   double magn = gradient.magnitude();
//   if (magn == 0)
//     return diffuse;
//
//   Vector pg = cross(pt,gradient);
//   pg = pg.normalize();
//
//   //   return kd*I_lightsource*abs(dot(pg,light));
//   diffuse.r = 255.0*abs(dot(pg,light));
//   diffuse.g = 255.0*abs(dot(pg,light));
//   diffuse.b = 255.0*abs(dot(pg,light));
//
//   return diffuse;
// }


/** Find illumination due to specular **/
/*double specular_term(const Vector& pt, const Vector& gradient)
{
  Vector LV = light + pt;
  double LV_magn = LV.magnitude();
  Vector halfway = LV*pow(LV_magn,-1);*/
// cout << LV_magn << endl;

//   return ks*I_lightsource*dot(cross(gradient,pt).normalize(), halfway);
// return 1;
// }


int lookup(float x, float y, float z)
{
//   int result;
//    result = (z*inZ*inX) + y*inZ + x;
//  result = pow((x-inX),2) + pow((y-inY),2) + pow((z-inZ),2);
//   return result;
return ((z*inZ*inX) + y*inZ + x);
// return (sqrt(pow((x-inX),2) + pow((y-inY),2) + pow((z-inZ),2)));
}

/** Find trilinear interpolation based off of given values
 **/
double trilinear_interpolation(double x_val, double y_val, double z_val)
{
  if ( floor(x_val) >= inX || ceil(x_val) >= inX)
    x_val = inX-1;
  
  if ( floor(y_val) >= inY || ceil(y_val) >= inY)
    y_val = inY-1;
    
  if ( floor(z_val) >= inZ || ceil(z_val) >= inZ)
    z_val = inZ-1;
  
  
  float weight_x = ceil(x_val) - x_val;

  int index_a = lookup(floor(x_val),ceil(y_val),floor(z_val));
  int index_b = lookup(ceil(x_val), ceil(y_val), floor(z_val));
  float x_inter_upper_front = weight_x*map[index_a] + (1-weight_x)*map[index_b];
  //float x_inter_upper_front = weight_x*map[index_b] + (1-weight_x)*map[index_a];

  index_a = lookup(floor(x_val), floor(y_val), floor(z_val));
  index_b = lookup(ceil(x_val), floor(y_val), floor(z_val));
  float x_inter_lower_front = weight_x*map[index_a] + (1-weight_x)*map[index_b];
  //float x_inter_lower_front = weight_x*map[index_b] + (1-weight_x)*map[index_a];

  index_a = lookup(floor(x_val), ceil(y_val), ceil(z_val));
  index_b = lookup(ceil(x_val), ceil(y_val), ceil(z_val));
  float x_inter_upper_back = weight_x*map[index_a] + (1-weight_x)*map[index_b];
  //float x_inter_upper_back = weight_x*index_b + (1-weight_x)*map[index_a];

  index_a = lookup(floor(x_val), floor(y_val), ceil(z_val));
  index_b = lookup(ceil(x_val), floor(y_val), ceil(z_val));
  float x_inter_lower_back = weight_x*map[index_a] + (1-weight_x)*map[index_b];
  //float x_inter_lower_back = weight_x*index_b + (1-weight_x)*map[index_a];

  float weight_y = ceil(y_val) - y_val;
  float y_inter_front = weight_y*x_inter_upper_front + (1-weight_y)*x_inter_lower_front;
  float y_inter_back = weight_y*x_inter_upper_back + (1-weight_y)*x_inter_lower_back;

  float weight_z = ceil(z_val) - z_val;

  return weight_z*y_inter_front + (1-weight_z)*y_inter_back;
}


/** Calculate the gradient of a pt.  The gradient is a vector pointing in the direction of greater values at the
    fastest rate given the pts around it (in x,y,z directions)   **/
void calculate_gradient(Vector& pt)
{
  double h=1.0;

  /* out of bounds possibility 
  
  scenario          soln        
  pt.x+h < inX      pt.x+h
  pt.x+h = inX      pt.x
  pt.x+h > inX      inX-1
  
  scenario          soln
  pt.x-h < 0        0
  pt.x-h = 0        0
  pt.x-h > 0        pt.x-h
  
  (0,0,0)          (1,1,1)
  (1,0,0)          (1,1,1) = (2,1,1)
     (0,0,0)          (1,1,1) --> (0,1,1)
  (0,1,0)          (1,1,1) = (1,2,1)
     (0,0,0)          (1,1,1) --> (1,0,1)
  (0,0,1)          (1,1,1) = (1,1,2)
     (0,0,0)          (1,1,1) --> (1,1,0)
     
  (.5,0,0)        (0,-.5,-.5) is good
  is good         
     
    0 ---- 1
   /      /| 
  0 ---- 1 |
  |      | 1
  |      |/
  0 ---- 1   
  
  */
    
  // test if new point if out of bounds in x-direction
  if ( (pt.x-h) >= 0 && (pt.x+h) < inX)
    gradient.x = (trilinear_interpolation(pt.x+h,pt.y,pt.z) - trilinear_interpolation(pt.x-h,pt.y,pt.z))/(2*h);

  else if ((pt.x-h) < 0)
    gradient.x = (trilinear_interpolation(pt.x+h,pt.y,pt.z) - trilinear_interpolation(0,pt.y,pt.z))/(2*h);

  else
    gradient.x = (trilinear_interpolation(inX,pt.y,pt.z) - trilinear_interpolation(pt.x-h,pt.y,pt.z))/(2*h);

  // test if new point if out of bounds in y-direction
  if ( (pt.y-h) >= 0 && (pt.y+h) < inY)
    gradient.y = (trilinear_interpolation(pt.x, pt.y+h, pt.z) - trilinear_interpolation(pt.x,pt.y-h, pt.z))/(2*h);

  else if ((pt.y-h) < 0)
    gradient.y = (trilinear_interpolation(pt.x, pt.y+h, pt.z) - trilinear_interpolation(pt.x,0, pt.z))/(2*h);

  else
    gradient.y = (trilinear_interpolation(pt.x,inY, pt.z) - trilinear_interpolation(pt.x,pt.y-h, pt.z))/(2*h);

  // test if new point if out of bounds in z-direction
  if ((pt.z-h) >= 0 && (pt.z+h) < inZ)
    gradient.z = (trilinear_interpolation(pt.x, pt.y, pt.z+h) - trilinear_interpolation(pt.x, pt.y, pt.z-h))/(2*h);

  else if ((pt.z-h) < 0)
    gradient.z = (trilinear_interpolation(pt.x, pt.y, pt.z+h) - trilinear_interpolation(pt.x, pt.y, 0))/(2*h);

  else
    gradient.z = (trilinear_interpolation(pt.x, pt.y, inZ) - trilinear_interpolation(pt.x, pt.y, pt.z-h))/(2*h);

  // make sure components of gradient >= 0
//   if (gradient.x < 0)
//     gradient.x = 0;
// 
//   if (gradient.y < 0)
//     gradient.y = 0;
// 
//   if (gradient.z < 0)
//     gradient.z = 0;

//   gradient = gradient.normalize();
}

/** Calculate transfer function (ie. pts on surface containing similar values) **/
double transfer_function( Vector& pt)
{
  calculate_gradient(pt);

  double fv = trilinear_interpolation(pt.x,pt.y,pt.z);
  double num = abs(fv - isovalue);

  double frac;
  double denom;
  if (gradient == Vector(0,0,0))
  {
    denom = 0;
    frac = 0;
  }

  else
  {
    denom = abs(gradient.magnitude());
    frac = num/denom;
  }

  double current_alpha;

  if (denom == 0 || fv == isovalue)
    current_alpha = 1.0;

  else if (denom != 0 && frac < sigma)
    current_alpha = 1.0 - (frac/sigma);

  else
    current_alpha = 0.0;

//   return_gradient = gradient;
  return current_alpha;
}


RGB illumination(Vector& pt)
{
  // calculate diffuse
  RGB diffuse;
  /*double magn = gradient.magnitude();
  if (magn == 0)
    return diffuse;

  Vector pg = cross(pt,gradient);
  pg = pg.normalize();

  //   return kd*I_lightsource*abs(dot(pg,light));
  diffuse.r = 255.0*abs(dot(pg,light));
  diffuse.g = 255.0*abs(dot(pg,light));
  diffuse.b = 255.0*abs(dot(pg,light))*/;

  Vector LVector = lightsource - pt;
  LVector = LVector.normalize();

  Vector normal;

  if (gradient == Vector(0,0,0))
    normal = Vector(0,0,0);

  else
    normal = gradient.normalize();


//   diffuse.r = I_lightsource * kd.r * abs(dot(normal, LVector));
//   diffuse.g = I_lightsource * kd.g * abs(dot(normal, LVector));
//   diffuse.b = I_lightsource * kd.b * abs(dot(normal, LVector));

  diffuse.r = I_lightsource * abs(dot(normal, LVector));
  diffuse.g = I_lightsource * abs(dot(normal, LVector));
  diffuse.b = I_lightsource * abs(dot(normal, LVector));

  // calculate specular
  //   RGB specular = specular_term(pt, gradient);

  return diffuse;
}


/** Take samples from min to max t (ie. front to back) and return the total intensity. **/
RGB front_to_back_compositing(double mint, double maxt)
{
  double alpha = alpha_v;
//   double final_I = 0.0;

//   int steps = (maxt - mint)/step_size;
  Vector pt;

  //   double alpha_i;
  double current_step = mint;
  //   RGB intensity(0,0,0);
  intensity.r = 0;
  intensity.g = 0;
  intensity.b = 0;

  pt = eye.origin + (eye.direction*current_step);

  //   while (pt >= Vector(0,0,0) && pt < Vector(inX,inY,inZ) && alpha > 0.0001 && current_step < floor(maxt))
  while ((pt.x >= 0 && pt.x < inX) && (pt.y >= 0 && pt.y < inY) && (pt.z >= 0 && pt.z < inZ) && (alpha > 0.00001) && ceil(current_step) <= maxt)
  {
    float alpha_i = transfer_function(pt);

    //     final_I = final_I + (alpha*init_I);
    RGB illuminate = illumination(pt);

    intensity.r = intensity.r + (current_step*alpha_i*alpha*illuminate.r);
    intensity.g = intensity.g + (current_step*alpha_i*alpha*illuminate.g);
    intensity.b = intensity.b + (current_step*alpha_i*alpha*illuminate.b);

// intensity.r = current_step*alpha_i*alpha* illuminate.r;
// intensity.g =  current_step*alpha_i*alpha* illuminate.g;
// intensity.b =  current_step*alpha_i*alpha* illuminate.b;



    //     final_I = (exp(-sigma*alpha_i)*init_I) + final_I;

    //     alpha = alpha*(1-alpha_i);
    alpha = alpha * exp(-current_step*alpha_i);

    current_step +=step_size;
    pt = eye.origin + (eye.direction*current_step);
//     cout << "alpha= " << alpha << "\t current_step= " << current_step << endl;
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

  if (denom == 0)
    return -1;

  double t = -(num/denom);

  if (t < 0)
    return -1;

  else
  {
    Vector pt = eye.origin + (eye.direction*t);
    //     if (pt >= face.p && pt <= face.q && pt >= face.r && pt <= face.s)
    //     if (pt.x >= 0 && pt.x < inX && pt.y >= 0 && pt.y < inY && pt.z >= 0 && pt.z < inZ)
    if (((pt.z == 0) && (pt.x >= 0 && pt.x < inX) && (pt.y >= 0 && pt.y < inY)) ||
      ((pt.z == inZ-1) && (pt.x >= 0 && pt.x < inX) && (pt.y >= 0 && pt.y < inY)) ||
      ((pt.x == inX-1) && (pt.y >= 0 && pt.y < inY) && (pt.z >= 0 && pt.z < inZ)) ||
      ((pt.x == 0) && (pt.y >= 0 && pt.y < inY) && (pt.z >= 0 && pt.z < inZ)) ||
      ((pt.y == inY-1) && (pt.x >= 0 && pt.x < inX) && (pt.z >= 0 && pt.z < inZ)) ||
      ((pt.y == 0) && (pt.x >= 0 && pt.x < inX) && (pt.z >= 0 && pt.z < inZ)))

    {
      return t;
    }

    else return -1;
  }
}


/** Calculate direction that the eye ray is pointing **/
void find_eyeray(int i, int j)
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
  front.p = Vector(0,(inY-1),0); // along inY
  front.q = Vector((inX-1),(inY-1),0); // along inX,inY
  front.r = Vector(0,0,0); // origin
  front.s = Vector((inX-1),0,0); // along inX
  bbox.face[0] = front;

  // back face --
  //   same thing as front but translated along inZ
  Square back;
  back.name = "back";
  back.p = Vector(0,(inY-1),(inZ-1)); // along inY
  back.q = Vector((inX-1),(inY-1),(inZ-1)); // along inX,(inY-1)
  back.r =  Vector(0,0,(inZ-1)); // origin
  back.s =  Vector((inX-1),0,(inZ-1)); // along inX
  bbox.face[1] = back;

  // right face --
  Square right;
  right.name = "right";
  right.p = Vector((inX-1),(inY-1),0); // along inY
  right.q = Vector((inX-1),(inY-1),(inZ-1)); // along inZ,inY
  right.r = Vector((inX-1),0,0); // origin
  right.s = Vector((inX-1),0,(inZ-1)); // along inZ
  bbox.face[2] = right;

  // left face --
  Square left;
  left.name = "left";
  left.p = Vector(0,(inY-1),(inZ-1)); // along inY
  left.q = Vector(0,(inY-1),0); // along inZ,inY
  left.r = Vector(0,0,(inZ-1)); // origin
  left.s = Vector(0,0,0); // along inZ
  bbox.face[3] = left;

  // top face --
  Square top;
  top.name = "top";
  top.p = Vector(0,(inY-1),(inZ-1)); // along inY
  top.q = Vector((inX-1),(inY-1),(inZ-1)); // along inZ,inY
  top.r = Vector(0,(inY-1),0); // origin
  top.s =  Vector((inX-1),(inY-1),0); // along inZ

  bbox.face[4] = top;

  // bottom face --
  Square bottom;
  bottom.name = "bottom";
  bottom.p = Vector(0,0,(inZ-1)); // along inY
  bottom.q = Vector((inX-1),0,(inZ-1)); // along inZ,inY
  bottom.r = Vector(0,0,0); // origin
  bottom.s = Vector((inX-1),0,0); // along inZ
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
    infile >> ka.r >> ka.g >> ka.b;
    infile >> kd.r >> kd.g >> kd.b;
    infile >> ks.r >> ks.g >> ks.b;
    infile >> I_lightsource;
    infile >> lightsource.x >> lightsource.y >> lightsource.z;
    infile >> alpha_v;
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
      RGB &pix = img.pixel(i,j);

      // calculate direction vector of eye ray
      find_eyeray(i,j);

      // array of possible t-values
      double t[6];
      for (int k=0; k<6; k++)
	t[k] = ray_plane_intersection(bbox.face[k]);

      // min & max possible t for intersections between bounding box & eye ray
      double min_t(-1), max_t(-1);

      for (int k=0; k<6; k++)
      {
	if ((t[k] < min_t || min_t == -1) && t[k] != -1)
	  min_t = t[k];

	if ((t[k] > max_t || max_t == -1) && t[k] != -1)
	  max_t = t[k];
      }

//       delete[] t;

      RGB color;
      if (min_t != -1 && max_t != -1 && min_t <= max_t)
      {
	color = front_to_back_compositing(min_t, max_t);
      }

      else
      {
	color.r = 255;
	color.g = 255;
	color.b = 255;
      }


      pix.r = color.r;
      pix.g = color.g;
      pix.b = color.b;
    }
  }

  //   test_trilinear();
  //   test_transfer();
//   img.scale();
  img.save_to_ppm_file(outputFile.c_str());

  return 0;
}