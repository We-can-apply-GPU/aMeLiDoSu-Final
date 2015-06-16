#!/usr/bin/env python

from __future__ import print_function
import sys
import numpy as np
import theano
import theano.tensor as T
import lasagne
import time
import random
import collections
from StringIO import StringIO
from load_data import load_data
from settings import *

def load_data(fin, data):
    OK = True
    lines = []
    for z in range(BATCH_SIZE):
        line = fin.readline()
        if not line:
            fin = open(data, "r")
            line = fin.readline()
            OK = False
        lines.append(np.loadtxt(StringIO(line), dtype=np.float32))

    lines = np.asarray(lines)
    Y = lines[:, 0].astype(np.int32)
    X = lines[:, 1:]

    return X, Y, OK, fin

def load_transition():
    file = open("data/conf/state_48_39.map");
    trans = np.zeros((1943, 48), dtype=np.float32)
    transmap = np.zeros((1943,), dtype=np.int32)
    lines = file.readlines()
    phones = dict()
    for i, line in enumerate(lines):
        lines[i] = line.split("\t")
        phones[lines[i][1]]=0
    phones = collections.OrderedDict(sorted(phones.items()))
    for i, k in enumerate(phones):
        phones[k] = i
    for line in lines:
        trans[int(line[0]), phones[line[1]]] = 1
        transmap[int(line[0])] = phones[line[1]]
    return trans, transmap

def create_iter_functions(data, output_layer):
    X_batch = T.matrix('x')
    Y_batch = T.ivector('y')
    trans = T.matrix('trans')
    transmap = T.ivector('transmap')

    objective = lasagne.objectives.Objective(output_layer, loss_function=lasagne.objectives.categorical_crossentropy)

    all_params = lasagne.layers.get_all_params(output_layer)
    param_rms = theano.shared(np.float32(0))
    for param in all_params:
        param_rms += T.mean(T.square(param))

    param_rms /= len(all_params)

    loss_train = objective.get_loss(X_batch, target=Y_batch) + param_rms * 100

    pred = T.argmax(T.dot(lasagne.layers.get_output(output_layer, X_batch, deterministic=True), trans), axis=1)
    accuracy = T.mean(T.eq(pred, transmap[Y_batch]), dtype=theano.config.floatX)

    updates = lasagne.updates.rmsprop(loss_train, all_params, LEARNING_RATE)

    iter_train = theano.function(
        [X_batch, Y_batch], accuracy, updates=updates,
        givens={
            trans: data['trans'],
            transmap: data['transmap']
        }
    )

    iter_valid = theano.function(
        [X_batch, Y_batch], accuracy,
        givens={
            trans: data['trans'],
            transmap: data['transmap']
        }
    )

    return {"train": iter_train, "valid": iter_valid}

def main():
    print("Loading data...")
    data = {}
    trans, transmap = load_transition();
    data['trans'] = theano.shared(trans, borrow=True)
    data['transmap'] = theano.shared(transmap, borrow=True)
    print("Building model and compile theano...")
    output_layer = build_model(input_dim = 350, output_dim = 1943)
    if len(sys.argv) == 2:
        print("loading model {}/{}".format(MODEL_NAME, sys.argv[1]))
        fin = open("model/{}/{}".format(MODEL_NAME, sys.argv[1]))
        import pickle
        lasagne.layers.set_all_param_values(output_layer, pickle.load(fin))
    iter_funcs = create_iter_functions(data, output_layer)
    print("Training")
    trainin = open("data/train.dat")
    validin = open("data/valid.dat")
    epoch = 0
    while True:

        accu = 0
        cnt = 0
        now = time.time()
        while True:
            X, Y, OK, trainin = load_data(trainin, "data/train.dat")
            if not OK:
                break
            accu += iter_funcs['train'](X, Y)
            cnt += 1
        accu = accu / cnt
        print("Epoch {} took {:.3f}s\t{:.2f}%".format(epoch+1, time.time() - now, accu * 100))

        accu = 0
        cnt = 0
        now = time.time()
        while True:
            X, Y, OK, validin = load_data(validin, "data/valid.dat")
            if not OK:
                break
            accu += iter_funcs['valid'](X, Y)
            cnt += 1
        accu = accu / cnt
        print("Valid {} took {:.3f}s\t{:.2f}%".format(epoch+1, time.time() - now, accu * 100))

        epoch += 1
        import pickle
        fout = open("model/{}/{}x{:.2f}".format(MODEL_NAME, epoch, accu * 100), "w")
        pickle.dump(lasagne.layers.get_all_param_values(output_layer), fout)

if __name__ == '__main__':
  main()