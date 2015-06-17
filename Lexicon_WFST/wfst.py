#!/usr/bin/env python

from __future__ import print_function
import os

def main():
    f = open("../data/vit.dat", "r")
    fout = open("../data/wfst.dat", "w")
    while True:
        line = f.readline()
        print(line[:-1])
        print(line[:-1], file=fout)
        line = line.split(" ")
        sequence_id = line[0]
        cnt = int(line[1])
        for i in range(cnt):
            line = f.readline()
            line = line[:line.rindex(" ")]
            os.system("echo {} > example".format(line))
            ans = os.popen("./run.sh example").read()
            print(ans[:-1])
            print(ans[:-1], file=fout)
    pass

if __name__ == "__main__":
    main()
