#!/usr/bin python3
import numpy as np
import sys
import numpy as np
def gls():
    d = open('vitdat')
    m = open('map')
    ls ={}
    mp = {}
    al = {}
    for i in m:
        tmp = i.rstrip().split('\t')
        mp[tmp[1]] = tmp
    #To 39 form
    while True:
        i = d.readline()
        if i=='':
            break
        x = i.rstrip().split()
        al[x[0]]=[]
        for j in range(int(x[1])):
            y = d.readline()
            y = y.rstrip().split()[:-1]
            al[x[0]].append([])
            for k in y:
                if mp[k][3]!='sil':
                    al[x[0]][-1].append(mp[k][3])
    return al
def gdic(fn = 'dicown'):
    d = open(fn)
    dic = {}
    for i in d:
        s = i.replace('/','').replace('1','').replace('2','').rstrip()
        c = s.split(',')
        dic[c[0]]=c[1].split()
    return dic
def grade(lx,e,f):
    return ((2*(lx-e)/(lx+0.0))**(lx))*f**0.3
def freq(fn):
    ls = []
    d = open(fn)
    for i in d:
        x = i.rstrip().split(',')
        x[1] = float(x[1])
        ls.append(x)
    return ls
def error(x,y):
    lx = x.__len__()
    ly = y.__len__()
    d = np.zeros((lx+1,ly+1))
    for i in range(lx+1) : 
        d[i][0] = i
    for j in range(ly+1) : 
        d[0][j] = j
    for j in range(1,ly+1):
        for i in range(1,lx+1):
            if x[i-1]== y[j-1]:
                d[i][j] = d[i-1][j-1]
            else:
                d[i][j] = min([d[i-1][j],d[i][j-1],d[i-1][j-1]])+1
    return int(d[lx][ly])
if len(sys.argv) == 3:
#    print(read(sys.argv[1],sys.argv[2]))    
    print(error(sys.argv[1],sys.argv[2]))   
else:
    #s = "p r k s z aa r n l t axr n ih t eh v".split()
    #q = open('n48sx143')
    se = []
    d = open('n39')
    w = open('testtt','w+')
    o = open('testto','w+')
    for id,i in enumerate(d):
        t = i.rstrip().split(',')
        se.append(t)
    ls = []
    dic = gdic()
    al = gls()
    ls.extend(freq('mstb'))
    ls.extend(freq('hfreq'))
    for z in se:
      for s in al[z[0]]:
        ts = []
        sen = [[] for i in range(len(s)+1)]
        scr = [0 for i in range(len(s)+1)]
        for of in range(1,len(s)+1):
            for i in ls:
                x = dic[i[0]]
                if len(x)<=of:
                    err = error(x,s[of-len(x):of])
                    if err*2.0<len(x)+1:
                      v = grade(len(x),err,i[1])
                      if len(x)<of and scr[of-len(x)]+v>scr[of]:
                        #idx = np.argmin(scr[:of-len(x)])
                        #min(scr[:of-len(x)])+err<scr[of]:
                        scr[of] = scr[of-len(x)]+v
                        sen[of] = sen[of-len(x)]+[i[0]]
                      elif len(x)==of and v>max(scr[of],of):
                        scr[of] = v
                        sen[of] = [i[0]]
            if scr[of-1]+2>scr[of]:
                scr[of] = scr[of-1]+2
                sen[of]=sen[of-1]
        for i in sen[-1]:
            w.write(i+' ')
            o.write(i+' ')
        w.write('\n')
        o.write('\n')
        for i in scr:
            w.write(str(i)+' ')
        w.write('\n')
        print sen[-1],scr[-1]
        #for id,i in enumerate(ts):
         #   w.write(i[1]+' ')
        #w.write('\n')
