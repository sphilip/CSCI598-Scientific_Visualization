#include <iostream>
#include <cmath>
#include <vector>

using namespace std;

int lookup(float, float, float);
double trilinear_interpolation(double,double,double);
void create_map();

// int map[8];
vector<int> map(8,0);
int inX, inY, inZ;

int main(int argc, char **argv)
{
  cout << "Testing interpolation!\n";
  create_map();
//   cout << "\tat (0,0,0) = " << trilinear_interpolation(0,0,0) << endl;
//   cout << "\tat (1,0,0) = " << trilinear_interpolation(1,0,0) << endl;
//   cout << "\tat (0,1,0) = " << trilinear_interpolation(0,1,0) << endl;
//   cout << "\tat (1,1,0) = " << trilinear_interpolation(1,1,0) << endl;
//   cout << "\tat (0,0,1) = " << trilinear_interpolation(0,0,1) << endl;
//   cout << "\tat (1,0,1) = " << trilinear_interpolation(1,0,1) << endl;
//   cout << "\tat (0,1,1) = " << trilinear_interpolation(0,1,1) << endl;
//   cout << "\tat (1,1,1) = " << trilinear_interpolation(1,1,1) << endl;

// cout << "\tat (.5,.5,.5) = " << trilinear_interpolation(.5,.5,.5) << endl;
// cout << "\tat (.25,.25,.25) = " << trilinear_interpolation(.25,.25,.25) << endl;
cout << "\tat (.75,.75,.75) = " << trilinear_interpolation(.75,.75,.75) << endl;

/*
    12--13--14
   /       /|
  0---1---2 |
  |       | 17
  3---4---5/|
  |       | 20
  6---7---8 |
  |       | 23
  |       |/
  9--10--11

  x len = 3
  y len = 4
  z len = 2

  14 = (2,3,1) = 1*2*3 + 3*2 + 2 = 6+6+2
  */
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

int lookup(float x, float y, float z)
{
  int result;
   result = (z*inX*inZ) + y*inZ+ x;
   cout << "lookup = " << result << endl;
//  result = pow((x-inX),2) + pow((y-inY),2) + pow((z-inZ),2);
  return result;
}

/** Find trilinear interpolation based off of given values **/
double trilinear_interpolation(double x_val, double y_val, double z_val)
{
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