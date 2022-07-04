#pragma once

#include "../datset_parser/datset_parser.h"
#include "../math/math.h"
#include "../print.c"
#include "neuron_layer.h"

typedef struct _NeuralNetwork {
  InputNeuronLayer input_neuron_layer;
  HiddenNeuronLayer *hidden_neuron_layer_array;
  OutputNeuronLayer output_neuron_layer;
  int hidden_neuron_count;
} NeuralNetwork;

typedef struct _NeuralNetworkBestChoice {
  int digit;
  float p_value;
} NeuralNetworkBestChoice;

void neural_network_forward_propogation(NeuralNetwork *network);
void neural_network_fill_random(NeuralNetwork *network, float range_min,
                                float range_max);
void neural_network_print_output(NeuralNetwork *network);
float neural_network_evaluate_cost(NeuralNetwork *network,
                                   DatasetImage *training_image,
                                   DatasetLabel *training_label);
float neural_network_evaluate_fitness(NeuralNetwork *network,
                                      DatasetImage *test_image,
                                      DatasetLabel *test_label);
NeuralNetworkBestChoice neural_network_digit_choice(NeuralNetwork *network);
