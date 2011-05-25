Shanley Philip
Project 1: Volume Rendering

The purpose of this README is to describe the minor nuances associated with executing this program sucessfully.

To execute the program, type the following commands into the terminal:
  make
  ./volume <input file> <height of output image> <width of output image>

The input file contains the following information in this order:
  RAW image filename
  x y z -- dimensions of the data (ie. 41 41 41)
  step size -- the distance between voxels to render the output image