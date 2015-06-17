#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <vector>
#include <sstream>
#include <algorithm>
#include <list>
#include "../include/trans.h"

float trans_prob[39][39];
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
    int now_ans = trans.a2index[ans];
    if (phone_id == pre_id+1)
      trans_prob[pre_ans][now_ans] ++;

    pre_id = phone_id;
    pre_ans = now_ans;
  }
  for (int i=0; i<39; i++)
  {
    float sum = 0;
    for (int j=0; j<39; j++)
      sum += trans_prob[i][j];
    for (int j=0; j<39; j++)
      trans_prob[i][j] = std::log(trans_prob[i][j]/sum);
  }
}

struct Phone
{
  float features[39];
};

struct State
{
  int now_value;
  State *pre_ptr;
  float score;
  bool operator < (const State &t) const
  {
    return score > t.score;
  }
};

bool PointerCompare(const State *a, const State *b)
{
  return *a < *b;
}

bool OrderCompare(const State *a, const State *b)
{
  while(a != NULL && b != NULL)
  {
    if (a->now_value != b->now_value)
      return a->now_value < b->now_value;
    a = a->pre_ptr;
    b = b->pre_ptr;
  }
  return 0;
}

bool OrderEqual(const State *a, const State *b)
{
  while(a != NULL && b != NULL)
  {
    if (a->now_value != b->now_value)
      return 0;
    a = a->pre_ptr;
    b = b->pre_ptr;
  }
  return a == b;
}

const int SIZE = 50000000;
State memory[SIZE];
int top = 0;
std::vector<int> mid;

void viterbi1(std::vector<Phone> &seq)
{
  std::vector<State*> pre[39], now[39];
  for (int j=0; j<39; j++)
  {
    State *tmp = memory+top;
    if (++top == SIZE)
      top = 0;
    tmp->now_value = j;
    tmp->pre_ptr = NULL;
    tmp->score = seq.front().features[j];
    pre[j].push_back(tmp);
  }
  for (auto it = seq.begin()+1; it != seq.end(); it++)
  {
    for (int j=0; j<39; j++)
    {
      now[j].clear();
      for (int i=0; i<39; i++)
      {
        State *tmp = memory+top;
        if (++top == SIZE)
          top = 0;
        tmp->now_value = j;
        tmp->pre_ptr = pre[i].front();
        tmp->score = pre[i].front()->score + it->features[j] + trans_prob[i][j];
        now[j].push_back(tmp);
      }
      std::sort(now[j].begin(), now[j].end(), PointerCompare);
    }
    for (int j=0; j<39; j++)
      pre[j] = now[j];
  }
  now[0].clear();
  for (int i=0; i<39; i++)
    now[0].push_back(pre[i].front());
  std::sort(now[0].begin(), now[0].end(), PointerCompare);
  State *ptr = now[0].front();
  mid.clear();
  while(ptr != NULL)
  {
    mid.push_back(ptr->now_value);
    ptr = ptr->pre_ptr;
  }
}

void viterbi2(std::vector<Phone> &seq, int N, std::ofstream &fout)
{
  std::vector<State*> pre[39], now[39];
  for (int j=0; j<39; j++)
  {
    State *tmp = memory+top;
    if (++top == SIZE)
      top = 0;
    tmp->now_value = j;
    tmp->pre_ptr = NULL;
    tmp->score = seq.front().features[j];
    pre[j].push_back(tmp);
  }
  int phone = 1;
  for (auto it = seq.begin()+1; it != seq.end(); it++, phone++)
  {
    for (int j=0; j<39; j++)
    {
      now[j].clear();
      for (int i=0; i<39; i++)
      {
        if (i == j) continue;
        int z = 0;
        for (auto p = pre[i].begin(); p != pre[i].end() && z < N; p++)
        {
          State *tmp = memory+top;
          if (++top == SIZE)
            top = 0;
          tmp->now_value = j;
          tmp->pre_ptr = *p;
          tmp->score = (*p)->score + it->features[j] + trans_prob[i][j];
          now[j].push_back(tmp);
          z++;
        }
      }
      std::sort(now[j].begin(), now[j].end(), PointerCompare);
    }
    for (int j=0; j<39; j++)
      pre[j] = now[j];
  }
  now[0].clear();
  for (int i=0; i<39; i++)
  {
    int z = 0;
    for (auto p = pre[i].begin(); p != pre[i].end(), z < N; p++, z++)
      now[0].push_back(*p);
  }
  std::sort(now[0].begin(), now[0].end(), PointerCompare);
  int z = 0;
  for (auto p = now[0].begin(); p != now[0].end() && z < N; p++, z++)
  {
    State *ptr = *p;
    std::vector<int> to_reverse;
    while(ptr != NULL)
    {
      to_reverse.push_back(ptr->now_value);
      ptr = ptr->pre_ptr;
    }
    for (int i=to_reverse.size()-1; i>=0; i--)
      fout << trans.index2a[to_reverse[i]] << " ";
    fout << (*p)->score << std::endl;
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
    int N = 5;
    std::vector<Phone> seq;
    size_t pre = 0, next;
    std::string seq_id;
    next = line.find(' ', pre);
    seq_id = line.substr(pre, next - pre);
    fout << seq_id << " " << N << std::endl;
    pre = next+1;

    int cnt = std::stoi(line.substr(pre));
    std::cout << seq_id << "\t" << cnt << std::endl;
    while(cnt--)
    {
      Phone tmp;
      getline(emissin, line);
      std::istringstream ss(line);

      for (int i=0; i<39; i++)
        ss >> tmp.features[i];

      seq.push_back(tmp);
    }
    viterbi1(seq);
    /*
    for (int z=mid.size()-1; z>=0; z--)
      std::cout << trans.index2a[mid[z]] << " ";
    std::cout << std::endl;
    */
    std::vector<Phone> newseq;
    Phone tmp;
    for (int i=0; i<39; i++) tmp.features[i] = 0;
    int index = 0;
    cnt = 0;
    for (int z=mid.size()-1; z>=0; z--, index++)
    {
      if (z == mid.size()-1 || mid[z] == mid[z+1])
      {
        cnt ++;
        for (int i=0; i<39; i++) tmp.features[i] += std::exp(seq[index].features[i]);
      }
      else
      {
        for (int i=0; i<39; i++) tmp.features[i] = std::log(tmp.features[i] / cnt);
        newseq.push_back(tmp);
        for (int i=0; i<39; i++) tmp.features[i] = std::exp(seq[index].features[i]);
        cnt = 1;
      }
    }
    for (int i=0; i<39; i++) tmp.features[i] = std::log(tmp.features[i] / cnt);
    newseq.push_back(tmp);
    /*
    for (int i=0; i<newseq.size(); i++)
    {
      for (int j=0; j<39; j++)
      {
        fout << newseq[i].features[j] << " ";
      }
      fout << std::endl;
    }
    */
    viterbi2(newseq, N, fout);
  }
}
