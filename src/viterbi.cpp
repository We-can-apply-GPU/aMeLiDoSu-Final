#include <iostream>
#include "../include/trans.h"

int main()
{
  Trans trans;
  for (int i=0; i<48; i++)
    std::cout << i << trans.index2b[i] << std::endl;
}
