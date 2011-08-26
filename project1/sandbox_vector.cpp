#include <iostream>
#include "Vector.h"

using namespace std;

int main ()
{
  Vector first(2,4,5);
  Vector second(3,8,9);
  
  Vector third = first + second;
  cout << "add: (" << third.x << ", " << third.y << ", " << third.z << ")\n";
  
  Vector fourth = first - second;
  cout << "sub: (" << fourth.x << ", " << fourth.y << ", " << fourth.z << ")\n";
  
  Vector fifth = first*3;  
  cout << "scalar mult: (" << fifth.x << ", " << fifth.y << ", " << fifth.z << ")\n";
  
  Vector sixth = -first;
  cout << "neg: (" << sixth.x << ", " << sixth.y << ", " << sixth.z << ")\n";
  
  Vector seven = first;
  cout << "assign: (" << seven.x << ", " << seven.y << ", " << seven.z << ")\n";
  
  double magn = first.magnitude();
  cout << "magn: (" << magn << ")\n";
  
  Vector mult(3,6,9);
  mult = mult.normalize();
  cout << "norm: (" << mult.x << ", " << mult.y << ", " << mult.z << ")\n";
  
  
  
  
  
  return 0;
}

