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
      .node_value_net_matrix = new_matrix(1, node_count),
      .node_value_out_matrix = new_matrix(1, node_count),
      .node_bias_matrix = new_matrix(1, node_count),
      .node_weight_matrix = new_matrix(input_node_count, node_count),
  };
  return return_neuron_layer;
};

void hidden_neuron_layer_fill_random(HiddenNeuronLayer *layer, float range_min,
                                     float range_max) {
  matrix_fill_random(&layer->node_weight_matrix, range_min, range_max);
  matrix_fill_random(&layer->node_bias_matrix, range_min, range_max);
};

OutputNeuronLayer new_output_layer(int input_node_count, int node_count) {
  OutputNeuronLayer return_layer =
      new_hidden_layer(input_node_count, node_count);
  return return_layer;
};

void output_neuron_layer_fill_random(OutputNeuronLayer *layer, float range_min,
                                     float range_max) {
  hidden_neuron_layer_fill_random(layer, range_min, range_max);
};

void layer_feed_input(HiddenNeuronLayer *layer, Matrix *input_matrix) {
  free_matrix(&layer->node_value_out_matrix);
  free_matrix(&layer->node_value_net_matrix);
  Matrix value_matrix =
      matrix_product(input_matrix, &layer->node_weight_matrix);
  layer->node_value_net_matrix =
      matrix_add(&value_matrix, &layer->node_bias_matrix);
  layer->node_value_out_matrix =
      matrix_duplicate(&layer->node_value_net_matrix);
  matrix_relu(&layer->node_value_out_matrix);
  free_matrix(&value_matrix);
}

void apply_gradient(HiddenNeuronLayer *layer, Matrix *weight_gradient,
                    Matrix *bias_gradient, float learning_rate) {
  matrix_scale(weight_gradient, -1 * learning_rate);
  matrix_scale(bias_gradient, -1 * learning_rate);
  matrix_add_assign(&layer->node_bias_matrix, bias_gradient);
  matrix_add_assign(&layer->node_weight_matrix, weight_gradient);
}

Matrix layer_backward_propogate_error(HiddenNeuronLayer *layer,
                                      Matrix *previous_layer_output,
                                      Matrix *error_matrix,
                                      float learning_rate) {
  int previous_layer_node_count = previous_layer_output->size.col;
  Matrix net_out_dirrivative_relu =
      matrix_duplicate(&layer->node_value_net_matrix);
  matrix_derivative_relu(&net_out_dirrivative_relu);
  Matrix bias_gradient =
      matrix_element_wise_product(error_matrix, &net_out_dirrivative_relu);
  Matrix weight_gradient =
      new_matrix(previous_layer_node_count, layer->node_count);
  for (int i = 0; i < weight_gradient.size.row; i++) {
    for (int j = 0; j < weight_gradient.size.col; j++) {
      float bias_gradient_val = get_matrix_element(&bias_gradient, 0, j);
      float previous_layer_neuron_out =
          get_matrix_element(previous_layer_output, 0, i);
      set_matrix_element(&weight_gradient,
                         bias_gradient_val * previous_layer_neuron_out, i, j);
    }
  }
  Matrix previous_layer_error = new_matrix(1, previous_layer_node_count);
  for (int i = 0; i < previous_layer_node_count; i++) {
    float element = 0;
    for (int j = 0; j < layer->node_count; j++) {
      element += get_matrix_element(&bias_gradient, 0, j) *
                 get_matrix_element(&layer->node_weight_matrix, i, j);
    }
    set_matrix_element(&previous_layer_error, element, 0, i);
  }
  apply_gradient(layer, &weight_gradient, &bias_gradient, learning_rate);
  if (previous_layer_error.size.col != 8 && false) {
    log_("Bias gradient: ");
    print_matrix(&bias_gradient);
    log_("Error propogated to previous layer: ");
    print_matrix(&previous_layer_error);
		SDL_Delay(1000);
  }
  free_matrix(&bias_gradient);
  free_matrix(&weight_gradient);
  free_matrix(&net_out_dirrivative_relu);
  return previous_layer_error;
}
