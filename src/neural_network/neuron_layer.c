#pragma once

#include "neuron_layer.h"

InputNeuronLayer new_input_layer(int node_count) {
  InputNeuronLayer return_neuron_layer = {
      .node_count = node_count, .node_value_matrix = new_matrix(1, node_count)};
  return return_neuron_layer;
};

HiddenNeuronLayer new_hidden_layer(int input_node_count, int node_count) {
  HiddenNeuronLayer return_neuron_layer = {
      .node_count = node_count,
      .node_value_matrix = new_matrix(1, node_count),
      .node_bias_matrix = new_matrix(1, node_count),
      .node_weight_matrix = new_matrix(input_node_count, node_count)};
  return return_neuron_layer;
};

OutputNeuronLayer new_output_layer(int input_node_count, int node_count) {
  OutputNeuronLayer return_neuron_layer =
      new_hidden_layer(input_node_count, node_count);
  return return_neuron_layer;
};

void layer_feed_input(HiddenNeuronLayer *layer, Matrix *input_matrix) {
  layer->node_value_matrix =
      matrix_product(input_matrix, &layer->node_weight_matrix);
  layer->node_value_matrix =
      matrix_add(&layer->node_weight_matrix, &layer->node_bias_matrix);
}
