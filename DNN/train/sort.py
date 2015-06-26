from __future__ import print_function
class Phone(object):
    def __init__(self, line):
        self.line = line[:-1]
        line = line.split(',')
        line[0] = line[0].split("_")
        self.a = line[0][0]
        self.b = line[0][1]
        self.c = int(line[0][2])
    def __lt__(self, other):
        if self.a != other.a:
            return self.a < other.a
        if self.b != other.b:
            return self.b < other.b
        if self.c != other.c:
            return self.c < other.c

fin = open("train.1943", "r")
fout = open("out.1943", "w")

a = []
for line in fin:
    a.append(Phone(line))

a.sort()

for aa in a:
    print(aa.line, file=fout)

