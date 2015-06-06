#!/usr/bin/python
# -*- coding: utf-8 -*-
"""
File: timit_chmap.py
Description: map
"""
import sys

out = open(sys.argv[2],'w')
def infile():
    out.write('id,sequence\n')
    with open(sys.argv[1],'r') as sen ,open("nameList",'r') as nameList:
        for name,line in zip(nameList,sen):
            out.write(name.rstrip()+ ',')
            l = line.rstrip().split(',')
            for i in range (0,len(l)):
                _str = l[i].split()
                for i in range (0,len(_str)):
                    chmap(_str[i].lower())
            out.write('\n')
def chmap(_str):
    tim = open('../data/conf/timit.chmap','r')
    for line in tim:
        l = line.split()
        if l[0] == _str:
            out.write(l[1])
            break


if __name__=="__main__":
    infile()
