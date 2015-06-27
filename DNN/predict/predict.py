#!/usr/bin/env python

from __future__ import print_function
import sys
import numpy as np
import theano
import theano.tensor as T
import lasagne
import time
import random
import pickle
from settings import *

def load_transition(to=39):
    file = open("../../conf/state_48_39.map");
    trans = np.zeros((1943, to), dtype=np.float32)
    transmap = np.zeros((1943,), dtype=np.int32)
    lines = file.readlines()
    phones = dict()
    for i, line in enumerate(lines):
        lines[i] = line.split("\t")
        if to == 48:
            phones[lines[i][1]]=0
        else:
            phones[lines[i][2]]=0
    phones = collections.OrderedDict(sorted(phones.items()))
    for i, k in enumerate(phones):
        phones[k] = i
    for line in lines:
        if to == 48:
            trans[int(line[0]), phones[line[1]]] = 1
            transmap[int(line[0])] = phones[line[1]]
        else:
            trans[int(line[0]), phones[line[2]]] = 1
            transmap[int(line[0])] = phones[line[2]]
    return trans, transmap
def load_data():
    fin = open("test.dat")
    data = []
    seq = []
    index = 0
    for line in fin:
        if len(line) > 50:
            data.append(line.split(" "))
            index += 1
        else:
            if seq:
                seq[-1][2] = index
            seq.append([line[:-1], index, 0])
    seq[-1][2] = index
    X = np.asarray(data).astype(np.float32)
    return {"X": X,
            "seq": seq}

def create_iter_functions(data, output_layer):
    X = T.matrix('x')
    trans = T.matrix('trans')

    pred = T.log(T.dot(lasagne.layers.get_output(output_layer, X, deterministic=True), trans))

    return theano.function(
        [X], pred,
        givens={
            trans: data['trans'],
        }
    )

def main():
    print("Loading data...")
    if len(sys.argv) == 3 and sys.argv[2] == "39":
        trans, transmap = load_transition(39);
    else:
        trans, transmap = load_transition(48);
    data = load_data()
    data['trans'] = theano.shared(trans, borrow=True)
    print("Building model and compile theano...")
    X_num = len(data['X'])
    print("train data rows: {}".format(X_num))
    output_layer = build_model(input_dim = 350, output_dim = 1943, batch_size = None)
    fin = open("../model/"+sys.argv[1])
    lasagne.layers.set_all_param_values(output_layer, pickle.load(fin))
    predict = create_iter_functions(data, output_layer)

    index = 0
    fout = open("../vote/"+sys.argv[1], "w")
    for key, x, y in data['seq']:
        pred = predict(data['X'][x:y])
        print(key, x, y, sep="\t")
        print("{} {}".format(key, y-x), file=fout)
        for row in pred:
            for value in row:
                print("{} ".format(value), end="", file=fout)
            print(file=fout)

if __name__ == '__main__':
    main()
