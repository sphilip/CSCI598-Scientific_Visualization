Shanley Philip
Project 1: Volume Rendering

The purpose of this README is to describe the minor nuances associated with executing this program sucessfully.

To execute the program, type the following commands into the terminal:
  make
  ./volume <input file> <height of output image> <width of output image>

The input file contains the following information in this order:
  RAW image filename -- from http://tc18.liris.cnrs.fr/code_data_set/3D_images.html
  x y z -- dimensions of the data (ie. 41 41 41)
  step size -- the distance between voxels to render the output image
  ka -- ambient coefficient (surface property)
  kd -- diffuse coefficient
  ks -- specular coefficient (amount of light being scattered)
  I -- intensity of lightsource
  Lx Ly Lz -- location of light source
  alpha0 -- initial opacity
  sigma -- slab of width that light must goes through
  isovalue - points on the surface which the contain this value