#include <cstring>
#include <map>

class Trans
{
  /*
   * a : 39
   * b : 48
   * c : 1943
   */
public:
  Trans();
  int c2b[1943];
  std::map<std::string, int> b2index;
  std::string index2b[48];
};
