#pragma once

#include "neural_network.h"

void neural_network_forward_propogation(NeuralNetwork *network) {

  if (network->hidden_neuron_count > 0) {
    layer_feed_input(&network->hidden_neuron_layer_array[0],
                     &network->input_neuron_layer.node_value_matrix);
    for (int i = 1; i < network->hidden_neuron_count; i++) {
      layer_feed_input(
          &network->hidden_neuron_layer_array[i],
          &network->hidden_neuron_layer_array[i - 1].node_value_matrix);
    }
    layer_feed_input(
        &network->output_neuron_layer,
        &network->hidden_neuron_layer_array[network->hidden_neuron_count]
             .node_value_matrix);
  } else {
    layer_feed_input(&network->output_neuron_layer,
                     &network->input_neuron_layer.node_value_matrix);
  }
};

void neural_network_fill_random(NeuralNetwork *network, float range_min,
                                float range_max) {
  for (int i = 0; i < network->hidden_neuron_count; i++) {
    network->hidden_neuron_layer_array[i] = hidden_neuron_layer_fill_random(
        &network->hidden_neuron_layer_array[i], range_min, range_max);
  }
  network->output_neuron_layer = hidden_neuron_layer_fill_random(
      &network->output_neuron_layer, range_min, range_max);
};
