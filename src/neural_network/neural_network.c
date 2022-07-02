#pragma once

#include "neural_network.h"

void neural_network_forward_propogation(NeuralNetwork *network) {
  if (network->hidden_neuron_count > 0) {
    layer_feed_input(&network->hidden_neuron_layer_array[0],
                     &network->input_neuron_layer.node_value_matrix);
    matrix_sigmoid(&network->hidden_neuron_layer_array[0].node_value_matrix);
    for (int i = 1; i < network->hidden_neuron_count; i++) {
      layer_feed_input(
          &network->hidden_neuron_layer_array[i],
          &network->hidden_neuron_layer_array[i - 1].node_value_matrix);
      matrix_sigmoid(&network->hidden_neuron_layer_array[i].node_value_matrix);
    }
    layer_feed_input(
        &network->output_neuron_layer,
        &network->hidden_neuron_layer_array[network->hidden_neuron_count - 1]
             .node_value_matrix);
  } else {
    layer_feed_input(&network->output_neuron_layer,
                     &network->input_neuron_layer.node_value_matrix);
  }
  matrix_sigmoid(&network->output_neuron_layer.node_value_matrix);
  matrix_normalise(&network->output_neuron_layer.node_value_matrix);
};

void neural_network_fill_random(NeuralNetwork *network, float range_min,
                                float range_max) {
  for (int i = 0; i < network->hidden_neuron_count; i++) {
    hidden_neuron_layer_fill_random(&network->hidden_neuron_layer_array[i],
                                    range_min, range_max);
  }
  output_neuron_layer_fill_random(&network->output_neuron_layer, range_min,
                                  range_max);
};

void neural_network_print_output(NeuralNetwork *network) {
  clear_screen();
  Matrix *output_matrix = &network->output_neuron_layer.node_value_matrix;
  log_("Network PDF:");
  printf("| N | => | PV  | |");
  print_dashes(40);
  printf("|");
  pln();
  float p_value_max = 0;
  int p_value_max_i;
  for (int i = 0; i < output_matrix->size.col; i++) {
    float p_value = get_matrix_element(output_matrix, 0, i);
    printf("| %d | => | %3d%% | ", i, (int)(p_value * 100));
    print_dashes(p_value * 40);
    pln();
    if (p_value > p_value_max) {
      p_value_max = p_value;
      p_value_max_i = i;
    }
  }
  printf("Best guess is '%d' at '%d%%' confidence", p_value_max_i,
         (int)(p_value_max * 100));
  pln();
};
