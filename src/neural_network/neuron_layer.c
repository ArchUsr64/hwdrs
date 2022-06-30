#pragma once

#include "neuron_layer.h"

InputNeuronLayer new_input_layer(int node_count) {
  InputNeuronLayer return_neuron_layer = {
      .node_count = node_count, .node_value_matrix = new_matrix(1, node_count)};
  return return_neuron_layer;
};

HiddenNeuronLayer new_hidden_layer(int input_node_count, int node_count) {
  Matrix value_matrix = new_matrix(1, node_count);
  Matrix bias_matrix = new_matrix(1, node_count);
  Matrix weight_matrix = new_matrix(input_node_count, node_count);
  HiddenNeuronLayer return_neuron_layer = {
      .node_count = node_count,
      .node_value_matrix = value_matrix,
      .node_bias_matrix = bias_matrix,
      .node_weight_matrix = weight_matrix,
  };
  return return_neuron_layer;
};

HiddenNeuronLayer hidden_neuron_layer_fill_random(HiddenNeuronLayer *layer,
                                                  float range_min,
                                                  float range_max) {
  HiddenNeuronLayer return_layer;
  return_layer.node_weight_matrix =
      matrix_fill_random(&layer->node_weight_matrix, -1, 1);
  return_layer.node_bias_matrix =
      matrix_fill_random(&layer->node_bias_matrix, -1, 1);
  return return_layer;
};

OutputNeuronLayer new_output_layer(int input_node_count, int node_count) {
  OutputNeuronLayer return_layer =
      new_hidden_layer(input_node_count, node_count);
  return return_layer;
};

OutputNeuronLayer output_neuron_layer_fill_random(OutputNeuronLayer *layer,
                                                  float range_min,
                                                  float range_max) {
  OutputNeuronLayer return_layer =
      hidden_neuron_layer_fill_random(layer, range_min, range_max);
  return return_layer;
};

void layer_feed_input(HiddenNeuronLayer *layer, Matrix *input_matrix) {
  layer->node_value_matrix =
      matrix_product(input_matrix, &layer->node_weight_matrix);
  layer->node_value_matrix =
      matrix_add(&layer->node_weight_matrix, &layer->node_bias_matrix);
}
