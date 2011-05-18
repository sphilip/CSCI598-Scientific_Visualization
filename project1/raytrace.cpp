/**
Shanley Philip
Project 1: Volume Rendering
CSCI 598
**/

#include <iostream>
#include <fstream>
#include <cstring>

#include "image.h"

using namespace std;

void parseFile(char* filename)
{

  ifstream infile(filename);
  if (!infile)
  {
    cout << "Couldn't open file " << filename << endl;

  }

  else
  {
  }

  return;
}

int main(int argc, char** argv)
{
  if (argc < 2)
  {
    cout << "To execute, must include images\n\t" << argv[0] << "list of images\n";
  }

else
{
  for (int i=1; i<argc; i++)
  {
    cout << "Reading " << argv[i] << endl;
    parseFile(argv[i]);
  }
}
  return 0;
}