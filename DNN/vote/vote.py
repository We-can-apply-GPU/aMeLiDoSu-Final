from __future__ import print_function
import sys
import numpy as np

cnt = len(sys.argv) - 1
file = []
rate = []

sum = 0
for i in range(cnt):
    file.append(open(sys.argv[i+1], "r"))
    rate.append(int(sys.argv[i+1]))
    sum += rate[i]

for i in range(cnt):
    rate[i] = float(rate[i])/sum

fout = open("vote", "w")

line = ""
while True:
    for i in range(cnt):
        line = file[i].readline()
    if not line:
        break
    print(line.rstrip(), file=fout)
    seq = int(line.rstrip().split(" ")[1])
    for i in range(seq):
        ans = np.zeros(48, dtype=np.float32)
        for k in range(cnt):
            line = file[k].readline()
            line = line.rstrip().split(" ")
            ans += np.exp(np.array([float(a) for a in line])) * rate[k]
        ans = np.log(ans)
        for k in range(48):
            print("{} ".format(ans[k]), file=fout, end="")
        print(file=fout)
