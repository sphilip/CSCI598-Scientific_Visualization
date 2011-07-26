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
  ka -- ambient coefficient (surface property) for each (RGB) component
  kd -- diffuse coefficient for each (RGB) component
  ks -- specular coefficient (amount of light being scattered) for each (RGB) component
  I -- intensity of lightsource (0-255)
  Lx Ly Lz -- location of light source
  alpha0 -- initial opacity (0-1)
  sigma -- max distance between pt and isosurface
???slab of width that light must goes through???
  isovalue - points on the surface which the contain this value