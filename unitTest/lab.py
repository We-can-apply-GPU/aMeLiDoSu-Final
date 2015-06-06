#!/usr/bin/env python
# -*- coding: utf-8 -*-
import collections
import sys
sentences = collections.defaultdict(list)
phone48_to_39={}
mapping = open("../data/conf/phones.60-48-39.map",'r')
for line in mapping:
    line = line.split()
    if(len(line) == 3):
        phone48_to_39[line[1]] = line[2]


def read_label(filename):
    with open (filename,'r') as fin:
        curName = ''
        for line in fin:
            line = line.rstrip().split(',')
            tmp = line[0].split('_')
            name = tmp[0]+'_'+tmp[1]

            if name != curName:
                curPhone=''
                curName = name

            phone = phone48_to_39[line[1]]
            #phone=line[1]
            if phone != curPhone:
                curPhone = phone
                sentences[name].append(phone)

        for lab in sentences.values():
            if lab[0] == 'sil' or lab[0] == 'vcl' or lab[0] == 'cl':
                lab.pop(0)
            if lab[-1] == 'sil' or lab[0] == 'vcl' or lab[0] == 'cl':
                lab.pop(-1)

if __name__ == "__main__":
    filename = sys.argv[1]
    read_label(filename)
    
    with open(sys.argv[2] + "_48phones",'w') as fout ,open("nameList",'w') as nameList:
        for name,lab in sentences.items():
            nameList.write("{}\n".format(name))
            fout.write("{}".format((' '.join(lab) + '\n')))
