#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <vector>
#include <sstream>
#include <algorithm>
#include "../include/trans.h"

float trans_prob[48][48];
Trans trans;

void load_trans_prob()
{
  std::ifstream transin("data/label/train.lab");
  std::string line;
  int pre_id = 1, pre_ans;
  while(getline(transin, line))
  {
    std::string speaker_id, sequence_id, ans; 
    int phone_id;
    size_t pre = 0, next;
    next = line.find('_');
    speaker_id = line.substr(pre, next-pre);
    pre = next+1;

    next = line.find('_', pre);
    sequence_id = line.substr(pre, next-pre);
    pre = next+1;

    next = line.find(',', pre);
    phone_id = std::stoi(line.substr(pre, next-pre));
    pre = next+1;

    ans = line.substr(pre);
    int now_ans = trans.b2index[ans];
    if (phone_id == pre_id+1)
      trans_prob[pre_ans][now_ans] ++;

    pre_id = phone_id;
    pre_ans = now_ans;
  }
  for (int i=0; i<48; i++)
  {
    float sum = 0;
    for (int j=0; j<48; j++)
      sum += trans_prob[i][j];
    for (int j=0; j<48; j++)
      trans_prob[i][j] = std::log(trans_prob[i][j]/sum);
  }
}

struct Phone
{
  float features[48];
};

struct State
{
  std::vector<int> route;
  float score;
  bool operator < (const State &t) const
  {
    return score > t.score;
  }
};

void viterbi(std::vector<Phone> &seq, int N, std::ofstream &fout)
{
  std::vector<State> pre[48], now[48];
  for (int j=0; j<48; j++)
  {
    std::vector<int> route {j};
    float score = seq.front().features[j];
    pre[j].push_back((State){route, score});
  }
  for (int i=0; i<pre[0].size(); i++)
  {
    std::cout << pre[0][i].score << std::endl;
  }
  for (auto it = seq.begin()+1; it != seq.begin()+2; it++)
  {
    for (int j=0; j<48; j++)
    {
      now[j].clear();
      for (int i=0; i<48; i++)
      {
        int z = 0;
        for (auto p = pre[i].begin(); p != pre[i].end(), z < N; p++, z++)
        {
          State tmp;
          tmp.route = p->route;
          tmp.route.push_back(j);
          tmp.score = p->score + it->features[j] + trans_prob[i][j];
          now[j].push_back(tmp);
        }
      }
      std::sort(now[j].begin(), now[j].end());
    }
    for (int j=0; j<48; j++)
      pre[j] = now[j];
  }
}

int main()
{
  load_trans_prob();
  std::ifstream emissin("data/prob.dat");
  std::ofstream fout("data/vit.dat");
  std::string line;
  while (getline(emissin, line))
  {
    int N = 10;
    std::vector<Phone> seq;
    size_t pre = 0, next;
    std::string seq_id;
    next = line.find(' ', pre);
    seq_id = line.substr(pre, next - pre);
    fout << seq_id << " " << N << std::endl;
    pre = next+1;

    int cnt = std::stoi(line.substr(pre));
    while(cnt--)
    {
      Phone tmp;
      getline(emissin, line);
      std::istringstream ss(line);

      for (int i=0; i<48; i++)
        ss >> tmp.features[i];

      seq.push_back(tmp);
    }
    viterbi(seq, 10, fout);
  }
}
