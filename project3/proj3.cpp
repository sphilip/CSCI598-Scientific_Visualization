#include "image.h"
#include <iostream>
#include <fstream>
#include <cmath>
#include <stdlib.h>
#include <vector>

using namespace std;

// Global Variables //
int resx, resy;


struct pixel_struct
{
  RGB color;
  float x;
  float y;
};

vector<pixel_struct> original_set;
vector<pixel_struct> forward;
vector<pixel_struct> backward;

int size, height,width,max_color;

void read_inputfile(char *file)
{
  ifstream infile(file);

  if (!infile.is_open())
  {
    cout << "Unable to open " << file << endl;
    return;
  }

  string code;
  infile >> code >> height >> width >> max_color;

  int start = infile.tellg();
  infile.seekg(0,ios::end);
  int end = infile.tellg();
  size = end - start;

  char *data = new char[size];

  infile.seekg(start+1,ios::beg);
  infile.read(data,size);
  infile.close();

  int *map = new int[size];
  for (int b=0; b<size; b++)
  {
    if ((int)data[b] < 0)
      map[b] = 255 + (int)data[b];

    else
      map[b] = (int) data[b];
  }

  delete[] data;

  for (int i=2; i<width*height*3; i=i+3)
  {
    pixel_struct temp;
    temp.color.r = map[i-2];
    temp.color.g = map[i-1];
    temp.color.b = map[i];

    int x = ((i-2)/3) % width;
    int y = (i-2)/(3*width);

    temp.x = (float) (2*x - width)/(width);
    temp.y = (float) (2*y - height)/(height);

    original_set.insert(original_set.end(), 1, temp);
  }

  delete []map;
}


RGB generate_noise()
{
  RGB color;
  color.r = rand() % 255;
  color.g = color.r;
  color.b = color.r;

  return color;
}


int make_1D(int x, int y)
{
  return x*width + y;
}


float bilinear_interpolation(float x, float y)
{
  float weight_x = ceil(x) - x;

  int index_a = make_1D(floor(x), ceil(y));
  int index_b = make_1D(ceil(x), ceil(y));
  float x_inter_upper = weight_x*original_set[index_a].color.r + (1-weight_x)*original_set[index_b].color.r;

  index_a = make_1D(floor(x), floor(y));
  index_b = make_1D(ceil(x), floor(y));
  float x_inter_lower = weight_x*original_set[index_a].color.r + (1-weight_x)*original_set[index_b].color.r;

  float weight_y = ceil(y) - y;
  return ((1-weight_y)*x_inter_upper) + (weight_y*x_inter_lower);
}


void follow_trajectory(int oldx, int oldy, float x, float y)
{
  float xn(x);
  float yn(y);
  float h = 0.025;

  float fx,fy;

  // constant
  //fx = 1.0/(abs(1.0)+1.0);
  //fy = 3.0/(abs(3.0)+1.0);

  // circular
  //fx = y/(abs(y)+1.0);
  //fy = -x/(abs(x)+1.0);

  //???
  //fx = sin(15*x+y)/(abs(sin(15*x+y))+1.0);
  //fy = cos(4*x+11*y)/(abs(cos(4*x+11*y))+1.0);

  fx = (0.4*sin(5*x*x+5*y*y)+y)/(abs(0.4*sin(5*x*x+5*y*y)+y)+1.0);
  fy = (0.4*sin(5*x*x+5*y*y)-x)/(abs(0.4*sin(5*x*x+5*y*y)-x)+1.0);
  int n=10;

  for (int i=0; i<n; i++)
  {
    pixel_struct temp;
    temp.x = xn + h*fx;
    temp.y = yn + h*fy;

    float orig_x = (float)(temp.x*width + width)/(2.0);
    float orig_y = (float)(-width*temp.y + width)/(2.0);

    if (orig_x >=0.0 && orig_y >=0.0 && orig_x < width-1 && orig_y < height-1)
    {
      temp.color.r = bilinear_interpolation(orig_x, orig_y);
      temp.color.b = temp.color.r;
      temp.color.g = temp.color.r;

      forward.insert(forward.end(),1,temp);

      xn = temp.x;
      yn = temp.y;
    }

    else break;
  }

  xn = x;
  yn = y;

  for (int i=0; i>(-n); i--)
  {
    pixel_struct temp;
    temp.x = xn - h*fx;
    temp.y = yn - h*fy;

    float orig_x = (float)(temp.x*width + width)/(2.0);
    float orig_y = (float)(-width*temp.y + width)/(2.0);

    if (orig_x >= 0.0 && orig_y >= 0.0 && orig_x < width-1 && orig_y < height-1)
    {
      temp.color.r = bilinear_interpolation(orig_x, orig_y);
      temp.color.b = temp.color.r;
      temp.color.g = temp.color.r;
      backward.insert(backward.end(),1,temp);

      xn = temp.x;
      yn = temp.y;
    }

    else break;
  }
}


RGB draw_image(int x, int y)
{
  float avg(0.0);
  float count(0.0);
  float sum = original_set[make_1D(x,y)].color.r;
  if (!forward.empty())
  {
    count = forward.size();

    for (int i=0; i < count; i++)
      sum += forward[i].color.r;
  }

  if (!backward.empty())
  {
    count += backward.size();

    for (int i=0; i < backward.size(); i++)
      sum += backward[i].color.r;
  }

  RGB color;
  if (count != 0.0)
  {
    avg = sum/count;
    color = RGB(avg,avg,avg);
  }

  else color = RGB(sum,sum,sum);

  return color;
}


int main(int argc, char* argv[])
{
  if (argc != 2)
  {
    cout << "Proper usage: ./<executable> <noise texture>\n";
    return 1;
  }

  read_inputfile(argv[1]);

  int resx = 1024;
  int resy = 1024;
  image img(resx,resy);
  RGB img_color;
  for (int x=0; x<resx; x++)
  {
    for (int y=0; y<resy; y++)
    {
      RGB &pix = img.pixel(x,y);

      //img_color = generate_noise();

      float scaled_x = (float)(2.0*x - width)/(width);
      float scaled_y =  (float) (2.0*y - height)/(-height);

      follow_trajectory(x,y,scaled_x, scaled_y);
      img_color = draw_image(x,y);

      forward.clear();
      backward.clear();


      pix.r = img_color.r;
      pix.g = img_color.g;
      pix.b = img_color.b;
    }
  }

  img.scale();
  img.save_to_ppm_file((char*)"output.ppm");
  return 0;
}
