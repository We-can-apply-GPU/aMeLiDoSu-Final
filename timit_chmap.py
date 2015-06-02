#!/usr/bin/python
# -*- coding: utf-8 -*-
"""
File: timit_chmap.py
Description: map
"""
import sys

out = open('timit_out','w')
out.write('id,sequence')
def infile():
    sen = open(sys.argv[1],'r')
    for line in sen:
        _id = line[0:line.find(",")]
        out.write('\n' + _id + ',')
        line = line[line.find(",")+1:-2]
        l = line.rstrip().split(',')
        for i in range (0,len(l)):
            _str = l[i].split()
            for i in range (0,len(_str)):
                chmap(_str[i].lower())

def chmap(_str):
    tim = open('data/conf/timit.chmap','r')
    for line in tim:
        l = line.split()
        if l[0] == _str:
            out.write(l[1])
            break


if __name__=="__main__":
    infile()
