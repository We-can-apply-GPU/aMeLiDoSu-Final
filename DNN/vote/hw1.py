from __future__ import print_function
import sys
import numpy as np
import collections

index2b = [0]*48

def load_trans():
    f = open("../../conf/phones.60-48-39.map")
    lines = []
    dic = collections.OrderedDict()
    for line in f:
        line = line[:-1]
        line = line.split("\t")
        dic[line[1]] = line[2]
    dic = collections.OrderedDict(sorted(dic.iteritems()))
    print(dic)
    return dic

if len(sys.argv) != 2:
    sys.exit(-1)

fin = open(sys.argv[1], "r")
fout = open(sys.argv[1]+".hw1", "w")

dic = load_trans()
print("Id,Prediction", file=fout)
while True:
    line = fin.readline()
    if not line:
        break
    line = line.split(" ")
    speaker = line[0]
    cnt = int(line[1])
    print("{}_{},sil".format(speaker, 1), file=fout)
    print("{}_{},sil".format(speaker, 2), file=fout)
    index = 3
    for i in range(cnt):
        line = fin.readline()
        line = line.split(" ")
        line = line[:-2]
        line = [float(a) for a in line]
        print("{}_{},{}".format(speaker, index, dic.items()[np.argmax(line)][1]), file=fout)
        index+=1
    print("{}_{},sil".format(speaker, index), file=fout)
    index += 1
    print("{}_{},sil".format(speaker, index), file=fout)
