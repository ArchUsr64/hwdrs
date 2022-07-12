#pragma once

#include <SDL2/SDL.h>

#include "../datset_parser/datset_parser.h"
#include "../math/math.h"
#include "../print.c"
#include "../sdl/sdl_abstracted.h"
#include "neuron_layer.h"

typedef struct _NeuralNetwork {
  InputNeuronLayer input_neuron_layer;
  HiddenNeuronLayer *hidden_neuron_layer_array;
  OutputNeuronLayer output_neuron_layer;
  int hidden_neuron_count;
  bool nodes_updated;
} NeuralNetwork;

typedef struct _NeuralNetworkBestChoice {
  int digit;
  float p_value;
} NeuralNetworkBestChoice;

void neural_network_set_input_matrix(NeuralNetwork *network, Matrix *matrix);
void neural_network_forward_propagation(NeuralNetwork *network);
void neural_network_backward_propogation(NeuralNetwork *network,
                                         Matrix *desired_output_matrix,
                                         float learning_rate);
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
void neural_network_render_structure(NeuralNetwork *network);
