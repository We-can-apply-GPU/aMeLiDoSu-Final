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

def load_data():
    fin = open("data/hw2test.dat")
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
    if len(sys.argv) == 3 and sys.argv[2] == "48":
        trans, transmap = load_transition(48);
    else:
        trans, transmap = load_transition(39);
    data = load_data()
    data['trans'] = theano.shared(trans, borrow=True)
    print("Building model and compile theano...")
    X_num = len(data['X'])
    print("train data rows: {}".format(X_num))
    output_layer = build_model(input_dim = 350, output_dim = 1943, batch_size = None)
    fin = open("model/4d1024/"+sys.argv[1])
    lasagne.layers.set_all_param_values(output_layer, pickle.load(fin))
    predict = create_iter_functions(data, output_layer)

    index = 0
    fout = open("data/hw2prob.dat", "w")
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
