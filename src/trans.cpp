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
    std::string phone, _;
    stream >> index >> _ >> phone;
    tmp[index] = phone;
    a2index[phone] = 0;
  }

  auto iter = a2index.begin();
  for (int z=0; iter != a2index.end(); iter++, z++)
  {
    iter->second = z;
    index2a[z] = iter->first;
  }
  for (int i=0; i<1943; i++) c2a[i] = a2index[tmp[i]];
}
