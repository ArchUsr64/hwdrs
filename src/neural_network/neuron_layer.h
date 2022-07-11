#pragma once

#include "../math/math.h"

typedef struct _InputNeuronLayer {
  int node_count;
  Matrix node_value_matrix;
} InputNeuronLayer;

typedef struct _HiddenNeuronLayer {
  int node_count;
  Matrix node_value_out_matrix;
  Matrix node_value_net_matrix;
  Matrix node_weight_matrix;
  Matrix node_bias_matrix;
} HiddenNeuronLayer;

typedef HiddenNeuronLayer OutputNeuronLayer;

InputNeuronLayer new_input_layer(int node_count);
HiddenNeuronLayer new_hidden_layer(int input_node_count, int node_count);
void hidden_neuron_layer_fill_random(HiddenNeuronLayer *layer, float range_min,
                                     float range_max);
OutputNeuronLayer new_output_layer(int input_node_count, int node_count);
void output_neuron_layer_fill_random(OutputNeuronLayer *layer, float range_min,
                                     float range_max);

void layer_feed_input(HiddenNeuronLayer *layer, Matrix *input_matrix);
Matrix layer_backward_propogate_error(HiddenNeuronLayer *layer,
                                      Matrix *previous_layer_output,
                                      Matrix *error_matrix,
                                      float learning_rate) ;
