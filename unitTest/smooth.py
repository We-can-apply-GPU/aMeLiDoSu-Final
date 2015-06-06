#!/usr/bin/python
# -*- coding: utf-8 -*-
"""
File: smooth.py
Description: smooth pulse label
"""

import sys

ID = []
Prediction = []

def infile():
    lab = open(sys.argv[1],'r')
    next(lab)
    for line in lab:
        l = line.rstrip().split(',')
        ID.append(l[0])
        Prediction.append(l[1])

def FourToOne():   #xyyx->xxxx
    for i in range (0, len(Prediction)-4):
        if Prediction[i] == Prediction[i+4]:
            Prediction[i+1] = Prediction[i]
            Prediction[i+2] = Prediction[i]
            Prediction[i+3] = Prediction[i]

def border():      #...xyyz... or ...xyuz... -> ...xxxz... or ...xzzz... 
    for i in range (0, len(Prediction)-3):
        if ((Prediction[i] != Prediction[i+3]) and 
            (Prediction[i] != Prediction[i+1]) and 
            (Prediction[i+3] != Prediction[i+1])):
            for j in range (0, len(Prediction[i+1])):
                if (Prediction[i+1][j] in Prediction[i]):
                    Prediction[i+1] = Prediction[i]
                    break
                elif (Prediction[i+1][j] in Prediction[i+3]):
                    Prediction[i+1] = Prediction[i+3]
                    break
            if ((Prediction[i] != Prediction[i+1]) and 
                (Prediction[i+3] != Prediction[i+1])):
                Prediction[i+1] = Prediction[i]

def outfile():
    f=open('smooth.csv','w')
    f.write('ID,Prediction\n')
    f=open('smooth.csv','a')
    for i in range (0, len(ID)):
        f.write(ID[i]+','+Prediction[i]+'\n')

if __name__ == "__main__":
    infile()
    FourToOne()
    border()
    FourToOne()
    border()
    FourToOne()
    outfile()

