#pragma once

#include "neuron_layer.h"

typedef struct _NeuralNetwork {
  InputNeuronLayer input_neuron_layer;
  HiddenNeuronLayer *hidden_neuron_layer_array;
  OutputNeuronLayer output_neuron_layer;
  int hidden_neuron_count;
} NeuralNetwork;

void neural_network_forward_propogation(NeuralNetwork *network);
void neural_network_fill_random(NeuralNetwork *network, float range_min,
                                float range_max);
