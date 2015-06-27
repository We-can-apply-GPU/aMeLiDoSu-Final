#include <string>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <vector>
#include <map>
#include <sstream>
#include <cmath>

class Phone
{
public:
  std::string speaker_id;
  std::string sequence_id;
  int phone_id, ans;
  std::vector<float> value;

  bool operator<(const Phone &t) const
  {
    if (speaker_id != t.speaker_id)
      return speaker_id < t.speaker_id;
    if (sequence_id != t.sequence_id)
      return sequence_id < t.sequence_id;
    return phone_id < t.phone_id;
  }

  std::string get_string() const
  {
    std::string ans = std::to_string(*(value.begin()));
    for (auto it = value.begin()+1; it != value.end(); it++)
      ans += " " + std::to_string(*it);
    return ans;
  }
};

void get_fbank(std::ifstream& arkin, std::vector<Phone>& phones)
{
  std::string line;
  while (getline(arkin, line))
  {
    Phone now;
    if (arkin.eof()) break;
    size_t pre = 0, next;
    next = line.find('_');
    now.speaker_id = line.substr(pre, next-pre);
    pre = next+1;

    next = line.find('_', pre);
    now.sequence_id = line.substr(pre, next-pre);
    pre = next+1;

    next = line.find(' ', pre);
    now.phone_id = std::stoi(line.substr(pre, next-pre));
    pre = next+1;
    
    std::string vec = line.substr(pre);
    std::istringstream ss(vec);

    if (now.speaker_id[0] == 'm')
      now.value.push_back(-1);
    if (now.speaker_id[0] == 'f')
      now.value.push_back(1);

    for (int i=0; i<69; i++)
    {
      float tmp;
      ss >> tmp;
      now.value.push_back(tmp);
    }

    phones.push_back(now);
  }
}

int main()
{
  std::ifstream trainin("train.ark");
  std::ifstream labin("train.1943");
  std::ifstream testin("../predict/test.ark");

  std::ofstream trainout("train.dat");
  std::ofstream validout("valid.dat");

  std::string line;
  std::vector<Phone> train, test;

  std::map<std::string, int> id2index;

  get_fbank(trainin, train);
  get_fbank(testin, test);

  std::vector<float> mean(train[0].value.size(), 0), var(train[0].value.size(), 0);
  int rows = 0;
  for (int i=0; i<train.size(); i++, rows++)
    for (int j=0; j<train[i].value.size(); j++)
      mean[j] += train[i].value[j];

   for (int i=0; i<test.size(); i++, rows++)
    for (int j=0; j<test[i].value.size(); j++)
      mean[j] += test[i].value[j];

  for (int i=0; i<mean.size(); i++)
    mean[i] /= rows;

  for (int i=0; i<train.size(); i++)
    for (int j=0; j<train[i].value.size(); j++)
      var[j] += (train[i].value[j]-mean[j]) * (train[i].value[j]-mean[j]);

  for (int i=0; i<test.size(); i++)
    for (int j=0; j<test[i].value.size(); j++)
      var[j] += (test[i].value[j]-mean[j]) * (test[i].value[j]-mean[j]);

  for (int i=0; i<mean.size(); i++)
  {
    var[i] /= rows;
    var[i] = std::sqrt(var[i]);
  }

  for (int i=0; i<train.size(); i++)
    for (int j=0; j<train[i].value.size(); j++)
      train[i].value[j] = (train[i].value[j] - mean[j])/var[j];

  int cnt = 0;
  while (getline(labin, line))
  {
    size_t pre = 0, next;
    next = line.find('_');
    std::string speaker_id = line.substr(pre, next-pre);
    pre = next+1;

    next = line.find('_', pre);
    std::string sequence_id = line.substr(pre, next-pre);
    pre = next+1;
    
    next = line.find(',', pre);
    int phone_id = std::stoi(line.substr(pre, next-pre));
    pre = next+1;

    train[cnt].ans = std::stoi(line.substr(pre));
      
    cnt ++;
  }

  std::random_device rd;
  std::mt19937 g(rd());

  std::vector<int> mapping(train.size());
  for (int i=0; i<mapping.size(); i++)
    mapping[i] = i;

  std::shuffle(mapping.begin(), mapping.end(), g);

  int z=0;
  for (; z<train.size()*5/6; z++)
  {
    if(z%10000==0) std::cout << z << std::endl;
    int i = mapping[z];
    if (i+4 >= train.size()) continue;

    if (train[i].phone_id == train[i+4].phone_id - 4)
      trainout << std::to_string(train[i+2].ans)+" "+train[i].get_string()+" "+train[i+1].get_string()+" "+train[i+2].get_string()+" "+train[i+3].get_string()+" "+train[i+4].get_string() << std::endl;
  }

  for (; z<train.size(); z++)
  {
    if(z%10000==0) std::cout << z << std::endl;
    int i = mapping[z];
    if (i+4 >= train.size()) continue;

    if (train[i].phone_id == train[i+4].phone_id - 4)
      validout << std::to_string(train[i+2].ans)+" "+train[i].get_string()+" "+train[i+1].get_string()+" "+train[i+2].get_string()+" "+train[i+3].get_string()+" "+train[i+4].get_string() << std::endl;
  }
}

