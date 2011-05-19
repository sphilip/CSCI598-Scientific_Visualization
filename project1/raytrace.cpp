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

using namespace std;

/** Global Variables **/
string binaryFile; // name of raw file
double x,y,z; // dimensions of raw file
int dimX, dimY;  // dimensions of output file
int* map = NULL; // holds the pixel values of image

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
    RGB pixel[length];
    for(int i=0; i<length; i++)
    {
      if (map[i] > 255)
	map[i] = 255;

      if (map[i] < 0)
	map[i] = 0;

      //       cout << map[i] << "\t";
      if (i%3 == 0)
	pixel[i].r = map[i];

      else if (i%3 == 1)
	pixel[i].g = map[i];

      else
	pixel[i].b = map[i];

    }

    img->rgb = pixel;
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

    img.save_to_ppm_file(outputFile.c_str());
  }

  return 0;
}