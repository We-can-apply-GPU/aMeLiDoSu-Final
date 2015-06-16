#include <sstream>
#include <fstream>
#include <string>
#include <map>
#include "../include/trans.h"

Trans::Trans()
{
  std::ifstream mapin("data/conf/state_48_39.map");

  std::string line;
  std::string tmp[1943];

  while (getline(mapin, line))
  {
    std::istringstream stream(line);
    int index;
    std::string phone;
    stream >> index >> phone;
    tmp[index] = phone;
    b2index[phone] = 0;
  }

  auto iter = b2index.begin();
  for (int z=0; iter != b2index.end(); iter++, z++)
  {
    iter->second = z;
    index2b[z] = iter->first;
  }
  for (int i=0; i<1943; i++) c2b[i] = b2index[tmp[i]];
}
