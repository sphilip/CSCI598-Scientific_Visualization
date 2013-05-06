#include <iostream>
#include <vector>
#include <math.h>
using namespace std;

struct Vector
{
  double x,y,z;
};

void calculate_gradient(Vector&);
int lookup(double x, double y, double z);
double trilinear_interpolation(double, double, double);
void create_map();

vector<int> map(8,0);
int inX, inY, inZ;
Vector gradient;

int main()
{
  cout << "Testing gradient!\n";
  create_map();
  Vector pt;

  pt.x = 0;
  pt.y = 0;
  pt.z = 0;

// pt.x = 1;
// pt.y = 1;
// pt.z = 1;

  calculate_gradient(pt);
  cout << "gradient= (" << gradient.x << ", " << gradient.y << ", " << gradient.z << ")\n";
  return 0;
}



void create_map()
{
  inX = 2;
  inY = 2;
  inZ = 2;

  map[0] = 0;
  map[1] = 1;
  map[2] = 0;
  map[3] = 1;
  map[4] = 0;
  map[5] = 1;
  map[6] = 0;
  map[7] = 1;
}

int lookup(double x, double y, double z)
{
  int result;
  result = (z*inX*inZ) + y*inZ+ x;
//   cout << "lookup = " << result << endl;
  //  result = pow((x-inX),2) + pow((y-inY),2) + pow((z-inZ),2);
  return result;
}

double trilinear_interpolation(double x_val, double y_val, double z_val)
{
  float weight_x = ceil(x_val) - x_val;

  int index_a = lookup(floor(x_val),ceil(y_val),floor(z_val));
  int index_b = lookup(ceil(x_val), ceil(y_val), floor(z_val));
  float x_inter_upper_front = weight_x*map[index_a] + (1-weight_x)*map[index_b];

  index_a = lookup(floor(x_val), floor(y_val), floor(z_val));
  index_b = lookup(ceil(x_val), floor(y_val), floor(z_val));
  float x_inter_lower_front = weight_x*map[index_a] + (1-weight_x)*map[index_b];

  index_a = lookup(floor(x_val), ceil(y_val), ceil(z_val));
  index_b = lookup(ceil(x_val), ceil(y_val), ceil(z_val));
  float x_inter_upper_back = weight_x*map[index_a] + (1-weight_x)*map[index_b];

  index_a = lookup(floor(x_val), floor(y_val), ceil(z_val));
  index_b = lookup(ceil(x_val), floor(y_val), ceil(z_val));
  float x_inter_lower_back = weight_x*map[index_a] + (1-weight_x)*map[index_b];

  float weight_y = ceil(y_val) - y_val;
  float y_inter_front = weight_y*x_inter_upper_front + (1-weight_y)*x_inter_lower_front;
  float y_inter_back = weight_y*x_inter_upper_back + (1-weight_y)*x_inter_lower_back;

  float weight_z = ceil(z_val) - z_val;

  return weight_z*y_inter_front + (1-weight_z)*y_inter_back;
}

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
//   if ( (pt.x+h) < inX )
//   {
//     if ( (pt.x-h) <= 0 )
//       gradient.x = (trilinear_interpolation(pt.x+h,pt.y,pt.z) - trilinear_interpolation(0,pt.y,pt.z))/(2*h);
//     
//     else
//       gradient.x = (trilinear_interpolation(pt.x+h,pt.y,pt.z) - trilinear_interpolation(pt.x-h,pt.y,pt.z))/(2*h);
//   }
//   
//   else if ( (pt.x+h) == inX )
//   {
//     if ( (pt.x-h) <= 0 )
//       gradient.x = (trilinear_interpolation(pt.x,pt.y,pt.z) - trilinear_interpolation(0,pt.y,pt.z))/(2*h);
//     
//     else
//       gradient.x = (trilinear_interpolation(pt.x,pt.y,pt.z) - trilinear_interpolation(pt.x-h,pt.y,pt.z))/(2*h);
//   }
//   
//   else
//   {
//     if ( (pt.x-h) <= 0 )
//       gradient.x = (trilinear_interpolation(inX-1,pt.y,pt.z) - trilinear_interpolation(0,pt.y,pt.z))/(2*h);
//     
//     else
//       gradient.x = (trilinear_interpolation(inX-1,pt.y,pt.z) - trilinear_interpolation(pt.x-h,pt.y,pt.z))/(2*h);
//   }
//   
//   // test if new point if out of bounds in y-direction
//   if ( (pt.y+h) < inY )
//   {
//     if ( (pt.y-h) <= 0 )
//       gradient.y = (trilinear_interpolation(pt.x,pt.y+h,pt.z) - trilinear_interpolation(pt.x,0,pt.z))/(2*h);
//     
//     else
//       gradient.y = (trilinear_interpolation(pt.x,pt.y+h,pt.z) - trilinear_interpolation(pt.x,pt.y-h,pt.z))/(2*h);
//   }
//   
//   else if ( (pt.y+h) == inY )
//   {
//     if ( (pt.y-h) <= 0 )
//       gradient.y = (trilinear_interpolation(pt.x,pt.y+h,pt.z) - trilinear_interpolation(pt.x,0,pt.z))/(2*h);
//     
//     else
//       gradient.y = (trilinear_interpolation(pt.x,pt.y+h,pt.z) - trilinear_interpolation(pt.x,pt.y-h,pt.z))/(2*h);
//   }
//   
//   else
//   {
//     if ( (pt.y-h) <= 0 )
//       gradient.y = (trilinear_interpolation(pt.x,inY-1,pt.z) - trilinear_interpolation(pt.x,0,pt.z))/(2*h);
//     
//     else
//       gradient.y = (trilinear_interpolation(pt.x,inY-1,pt.z) - trilinear_interpolation(pt.x,pt.y-h,pt.z))/(2*h);
//   }
//   
//   // test if new point if out of bounds in z-direction
//   if ( (pt.z+h) < inZ )
//   {
//     if ( (pt.z-h) <= 0 )
//       gradient.z = (trilinear_interpolation(pt.x,pt.y,pt.z+h) - trilinear_interpolation(pt.x,pt.y,0))/(2*h);
//     
//     else
//       gradient.z = (trilinear_interpolation(pt.x,pt.y,pt.z+h) - trilinear_interpolation(pt.x,pt.y,pt.z-h))/(2*h);
//   }
//   
//   else if ( (pt.z+h) == inZ )
//   {
//     if ( (pt.z-h) <= 0 )
//       gradient.z = (trilinear_interpolation(pt.x,pt.y+h,pt.z+h) - trilinear_interpolation(pt.x,pt.y,0))/(2*h);
//     
//     else
//       gradient.z = (trilinear_interpolation(pt.x,pt.y+h,pt.z+h) - trilinear_interpolation(pt.x,pt.y,pt.z-h))/(2*h);
//   }
//   
//   else
//   {
//     if ( (pt.z-h) <= 0 )
//       gradient.z = (trilinear_interpolation(pt.x,pt.y,inZ-1) - trilinear_interpolation(pt.x,pt.y,0))/(2*h);
//     
//     else
//       gradient.z = (trilinear_interpolation(pt.x,pt.y,inZ-1) - trilinear_interpolation(pt.x,pt.y,pt.z-h))/(2*h);
//   }
  
/*
   e=> x-----x <=f
      /|    /|
 a=> x-----x <=b
  g=>| x---|-x <=h
     |/    |/
 c=> x-----x <=d
 
 */

//   int index  = floor(x_val)*inX*inZ + ceil(y_val)*inY + floor(z_val);
//   float a_val = map[index];
//
//   index = ceil(x_val)*inX*inZ + ceil(y_val)*inY + floor(z_val);
//   float b_val = map[index];
//
//   index = floor(x_val)*inX*inZ + floor(y_val)*inY + floor(z_val);
//   float c_val = map[index];
//
//   index = ceil(x_val)*inX*inZ + floor(y_val)*inY + floor(z_val);
//   float d_val = map[index];
//
//   index = floor(x_val)*inX*inZ + ceil(y_val)*inY + ceil(z_val);
//   float e_val = map[index];
//
//   index = ceil(x_val)*inX*inZ + ceil(y_val)*inY + ceil(z_val);
//   float f_val = map[index];
//
//   index = floor(x_val)*inX*inZ + floor(y_val)*inY + ceil(z_val);
//   float g_val = map[index];
//
//   index = ceil(x_val)*inX*inZ + floor(y_val)*inY + ceil(z_val);
//   float h_val = map[index];
//
//   double weight_x = ceil(x_val) - x_val;
//   double ab_contrib = weight_x*a_val + (1-weight_x)*b_val;
//   double cd_contrib = weight_x*c_val + (1-weight_x)*d_val;
//   double ef_contrib = weight_x*e_val + (1-weight_x)*f_val;
//   double gh_contrib = weight_x*g_val + (1-weight_x)*h_val;
//
//   double weight_y = ceil(y_val) - y_val;
//   double abcd_contrib = weight_y*ab_contrib + (1-weight_y)*cd_contrib;
//
//   double efgh_contrib = weight_y*ef_contrib + (1-weight_y)*gh_contrib;
//
//   double weight_z = ceil(z_val) - z_val;
//   return weight_z*abcd_contrib + (1-weight_z)*efgh_contrib;


  // test if new point if out of bounds in x-direction
  if ( (pt.x+h) < inX && (pt.x-h) >= 0)
    gradient.x = (trilinear_interpolation(pt.x+h,pt.y,pt.z) - trilinear_interpolation(pt.x-h,pt.y,pt.z))/(2*h);
   
  
  else if ( (pt.x+h) < inX && (pt.x-h) < 0)
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

//   // make sure components of gradient >= 0
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
