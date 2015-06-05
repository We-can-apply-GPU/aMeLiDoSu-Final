from __future__ import print_function
import sys

fin = open("input", "r")
line = fin.readline().split(" ")

ans = ""
if sys.argv[1] == "60":
    for word in line:
        if word in ["bcl", "dcl", "epi", "gcl", "h#", "kcl", "pau", "pcl", "tcl"]:
            ans += "sil "
        else:
            ans += "{} ".format(word)

elif sys.argv[1] == "48":
    for word in line:
        if word in ["vcl", "epi", "cl"]:
            ans += "sil "
        else:
            ans += "{} ".format(word)

fout = open("input", "w")
print(ans, file=fout)
