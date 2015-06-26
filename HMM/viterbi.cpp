#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <vector>
#include <sstream>
#include <algorithm>
#include <list>
#include "trans.h"

float trans_prob[50][50];
float emiss_prob[48];
Trans trans;

void load_trans_prob()
{
  std::ifstream transin("train.lab");
  std::string line;
  int pre_id = 1, pre_ans=-1;
  while(getline(transin, line))
  {
    std::string speaker_id, sequence_id, ans; 
    int now_id;
    size_t pre = 0, next;
    next = line.find('_');
    speaker_id = line.substr(pre, next-pre);
    pre = next+1;

    next = line.find('_', pre);
    sequence_id = line.substr(pre, next-pre);
    pre = next+1;

    next = line.find(',', pre);
    now_id = std::stoi(line.substr(pre, next-pre));
    pre = next+1;

    ans = line.substr(pre);
    int now_ans = trans.b2index[ans];
    emiss_prob[now_ans] ++;
    if (now_id == 1)
    {
      trans_prob[48][now_ans] ++;
      if (pre_id != -1)
        trans_prob[pre_ans][49] ++;
    } 
    if (now_id == pre_id+1)
      trans_prob[pre_ans][now_ans] ++;

    pre_id = now_id;
    pre_ans = now_ans;
  }
  for (int i=0; i<50; i++)
  {
    float sum = 0;
    for (int j=0; j<50; j++)
      sum += trans_prob[i][j];
    for (int j=0; j<50; j++)
      trans_prob[i][j] = std::log(trans_prob[i][j]/sum);
  }
  float sum = 0;
  for (int i=0; i<48; i++)
    sum += emiss_prob[i];
  for (int i=0; i<48; i++)
    emiss_prob[i] = std::log(emiss_prob[i] / sum);
}

struct Phone
{
  float features[48];
};

struct State
{
  int now_value;
  State *pre_ptr;
  float score;
};

const int SIZE = 50000000;
State memory[SIZE];
int top = 0;
std::vector<int> mid;

std::vector<int> viterbi1(std::vector<Phone> &seq)
{
  State *pre[48], *now[48];
  for (int j=0; j<48; j++)
  {
    pre[j] = memory+top;
    if (++top == SIZE)
      top = 0;
    pre[j]->now_value = j;
    pre[j]->pre_ptr = NULL;
    pre[j]->score = seq.front().features[j]*5 + emiss_prob[j] + trans_prob[48][j]*15;
    seq[0].features[j] = pre[j]->score;
  }
  for (auto it = seq.begin()+1; it != seq.end(); it++)
  {
    for (int j=0; j<48; j++)
    {
      now[j] = memory+top++;
      if (++top == SIZE)
        top = 0;
      now[j]->score = -1e50;
      for (int i=0; i<48; i++)
      {
        float new_score = pre[i]->score + it->features[j]*5 + emiss_prob[j] + trans_prob[i][j]*15;
        if (new_score > now[j]->score)
        {
          now[j]->now_value = j;
          now[j]->pre_ptr = pre[i];
          now[j]->score = new_score;
        }
      }
      it->features[j] = now[j]->score;
    }
    for (int j=0; j<48; j++)
      pre[j] = now[j];
  }
  State *end = pre[trans.b2index["sil"]];
  std::vector<int> ans(seq.size(), 0);
  int i = seq.size()-1;
  while(end != NULL)
  {
    ans[i--] = end->now_value;
    end = end->pre_ptr;
  }
  return ans;
}

void print_seq(std::vector<Phone> &seq, std::ostream &oo)
{
  for (int i=0; i<seq.size(); i++)
  {
    int max = 0;
    for (int j=0; j<48; j++)
    {
      if (seq[i].features[j] > seq[i].features[max]) max = j;
    }
    oo << trans.index2b[max] << ' ';
  }
  oo << std::endl << std::endl;
}

void print_ans(std::vector<int> &seq, std::ostream &oo)
{
  if (&oo == &std::cout) { for (int i=0; i<seq.size(); i++)
    {
      oo << trans.index2b[seq[i]] << ' ';
    }
    oo << std::endl << std::endl;
    return;
  }
  int start, end;
  for (start=0; start<seq.size(); start++)
    if (seq[start] != trans.b2index["sil"])
      break;
  for (end = seq.size(); end>0; end--)
    if (seq[end-1] != trans.b2index["sil"])
      break;
  for (int i=start; i<end; i++)
    if (seq[i] != seq[i-1])
      oo << trans.index2chr[seq[i]];
  oo << std::endl;
}

int main()
{
  load_trans_prob();
  std::ifstream emissin("prob.dat");
  std::ofstream fout("vit.dat");
  std::string line;
  fout << "id,phone_sequence" << std::endl;
  while (getline(emissin, line))
  {
    std::vector<Phone> seq;
    size_t pre = 0, next;
    std::string seq_id;
    next = line.find(' ', pre);
    seq_id = line.substr(pre, next - pre);
    pre = next+1;

    int cnt = std::stoi(line.substr(pre));
    std::cout << seq_id << std::endl;
    fout << seq_id << ",";
    while(cnt--)
    {
      Phone tmp;
      getline(emissin, line);
      std::istringstream ss(line);

      for (int i=0; i<48; i++)
        ss >> tmp.features[i];
      seq.push_back(tmp);
    }
    std::vector<int> ans;
    print_seq(seq, std::cout);
    ans = viterbi1(seq);
    //print_ans(ans, std::cout);
    print_ans(ans, std::cout);
    print_ans(ans, fout);
    /*
    ans = viterbi1(seq);
    print_ans(ans, std::cout);
    */
  }
}
