#include <sstream>
#include <fstream>
#include <string>
#include <map>
#include "trans.h"

Trans::Trans()
{
  std::ifstream mapin("../conf/state_48_39.map");

  std::string line;
  std::string tmp39[1943];
  std::string tmp48[1943];

  while (getline(mapin, line))
  {
    std::istringstream stream(line);
    int index;
    std::string phone39, phone48;
    stream >> index >> phone48 >> phone39;
    tmp39[index] = phone39;
    tmp48[index] = phone48;
    a2index[phone39] = 0;
    b2index[phone48] = 0;
  }

  auto iter = a2index.begin();
  for (int z=0; iter != a2index.end(); iter++, z++)
  {
    iter->second = z;
    index2a[z] = iter->first;
  }
  for (int i=0; i<1943; i++) c2a[i] = a2index[tmp39[i]];

  iter = b2index.begin();
  for (int z=0; iter != b2index.end(); iter++, z++)
  {
    iter->second = z;
    index2b[z] = iter->first;
  }
  for (int i=0; i<1943; i++) c2b[i] = a2index[tmp48[i]];

  std::ifstream chrin("../conf/48_idx_chr.map");
  while (getline(chrin, line))
  {
    std::istringstream stream(line);
    int index;
    std::string phone48, chr;
    stream >> phone48 >> index >> chr;
    index2chr[b2index[phone48]] = chr[0];
  }
}
