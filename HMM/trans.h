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
  int c2a[1943];
  int c2b[1943];
  std::map<std::string, int> a2index;
  std::map<std::string, int> b2index;
  std::string index2a[39];
  std::string index2b[48];
  char index2chr[48];
};
