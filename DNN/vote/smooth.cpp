#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <vector>
#include <sstream>
#include <algorithm>
#include <list>

struct Phone
{
  double features[48];
};

void flatten(std::vector<Phone> &seq)
{
  std::vector<int> ans(seq.size(), 0);
  double max = -1e50;
  int pre =  0, dif = 0, now = 0;
  int i, j;
  for (i=0; i<seq.size(); i++)
  {
    max = -1e50, now = 0;
    for (j=0; j<48; j++)
    {
      if (seq[i].features[j] > max)
      {
        max = seq[i].features[j];
        now = j;
      }
    }
    ans[i] = now;
  }
  for (i=0; i<seq.size()-4; i++)
  {
    if (ans[i] == ans[i+4])
    {
      std::swap(seq[i+1].features[ans[i+1]], seq[i+1].features[ans[i]]);
      std::swap(seq[i+2].features[ans[i+2]], seq[i+2].features[ans[i]]);
      std::swap(seq[i+3].features[ans[i+3]], seq[i+3].features[ans[i]]);
      ans[i+1] = ans[i];
      ans[i+2] = ans[i];
      ans[i+3] = ans[i];
    }
  }
  for (i=0; i<seq.size()-3; i++)
  {
    if (ans[i] != ans[i+1] && ans[i+2] != ans[i+3])
    {
      std::swap(seq[i+1].features[ans[i+1]], seq[i+1].features[ans[i]]);
      std::swap(seq[i+2].features[ans[i+2]], seq[i+2].features[ans[i+3]]);
      ans[i+1] = ans[i];
      ans[i+2] = ans[i+3];
    }
  }
  for (i=0; i<seq.size()-2; i++)
  {
    if (ans[i] != ans[i+1] && ans[i+1] != ans[i+2])
    {
      std::swap(seq[i+1].features[ans[i+1]], seq[i+1].features[ans[i+(i%2)*2-1]]);
      ans[i+1] = ans[i+(i%2)*2-1];
    }
  }
}

int main(int argc, char *argv[])
{
  if (argc != 3) exit(-1);
  std::ifstream emissin(argv[1]);
  std::ofstream fout(argv[2]);
  std::string line;
  while (getline(emissin, line))
  {
    fout << line << std::endl;
    std::vector<Phone> seq;
    size_t pre = 0, next;
    std::string seq_id;
    next = line.find(' ', pre);
    seq_id = line.substr(pre, next - pre);
    pre = next+1;

    int cnt = std::stoi(line.substr(pre));
    std::cout << seq_id << "\t" << cnt << std::endl;

    while(cnt--)
    {
      Phone tmp;
      getline(emissin, line);
      std::istringstream ss(line);

      for (int i=0; i<48; i++)
        ss >> tmp.features[i];
      seq.push_back(tmp);
    }
    flatten(seq);
    for (int i=0; i<seq.size(); i++)
    {
      for (int j=0; j<48; j++)
        fout << seq[i].features[j] << " ";
      fout << std::endl;
    }
  }
}
