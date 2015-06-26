import lasagne
import numpy as np
import collections
BATCH_SIZE = 1024
MOMENTUM = 0.9
NUM_HIDDEN_UNITS = [1024, 1024, 1024, 1024]
LEARNING_RATE = 0.0001
def build_model(input_dim, output_dim, batch_size=BATCH_SIZE, num_hidden_units=NUM_HIDDEN_UNITS):
    l_in = lasagne.layers.InputLayer(shape=(BATCH_SIZE, input_dim))
    l_dp0 = lasagne.layers.DropoutLayer(l_in, p=0.5)
    l_hidden1 = lasagne.layers.DenseLayer(
            l_dp0,
            num_units=num_hidden_units[0],
            nonlinearity=lasagne.nonlinearities.rectify
            )
    l_dp1 = lasagne.layers.DropoutLayer(l_hidden1, p=0.6)
    l_hidden2 = lasagne.layers.DenseLayer(
            l_dp1,
            num_units=num_hidden_units[1],
            nonlinearity=lasagne.nonlinearities.rectify
            )
    l_dp2 = lasagne.layers.DropoutLayer(l_hidden2, p=0.6)
    l_hidden3 = lasagne.layers.DenseLayer(
            l_dp2,
            num_units=num_hidden_units[2],
            nonlinearity=lasagne.nonlinearities.rectify
            )
    l_dp3 = lasagne.layers.DropoutLayer(l_hidden3, p=0.6)
    l_hidden4 = lasagne.layers.DenseLayer(
            l_dp3,
            num_units=num_hidden_units[3],
            nonlinearity=lasagne.nonlinearities.rectify
            )
    l_dp4 = lasagne.layers.DropoutLayer(l_hidden4, p=0.7)
    l_out = lasagne.layers.DenseLayer(
            l_dp4,
            num_units=output_dim,
            nonlinearity=lasagne.nonlinearities.softmax
            )
    return l_out

