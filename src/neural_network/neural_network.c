#pragma once

#include "neural_network.h"

void neural_network_forward_propagation(NeuralNetwork *network) {
  if (!network->nodes_updated) {
    if (network->hidden_neuron_count > 0) {
      layer_feed_input(&network->hidden_neuron_layer_array[0],
                       &network->input_neuron_layer.node_value_matrix);
      for (int i = 1; i < network->hidden_neuron_count; i++) {
        layer_feed_input(
            &network->hidden_neuron_layer_array[i],
            &network->hidden_neuron_layer_array[i - 1].node_value_out_matrix);
      }
      layer_feed_input(
          &network->output_neuron_layer,
          &network->hidden_neuron_layer_array[network->hidden_neuron_count - 1]
               .node_value_out_matrix);
    } else {
      layer_feed_input(&network->output_neuron_layer,
                       &network->input_neuron_layer.node_value_matrix);
    }
    matrix_normalise(&network->output_neuron_layer.node_value_out_matrix);
    network->nodes_updated = true;
  }
}

void neural_network_fill_random(NeuralNetwork *network, float range_min,
                                float range_max) {
  network->nodes_updated = false;
  for (int i = 0; i < network->hidden_neuron_count; i++) {
    hidden_neuron_layer_fill_random(&network->hidden_neuron_layer_array[i],
                                    range_min, range_max);
  }
  output_neuron_layer_fill_random(&network->output_neuron_layer, range_min,
                                  range_max);
};

void neural_network_print_output(NeuralNetwork *network) {
  Matrix output_matrix =
      matrix_duplicate(&network->output_neuron_layer.node_value_out_matrix);
  matrix_normalise(&output_matrix);
  log_("Network PDF:");
  printf("| N | => | PV  | |");
  print_dashes(40);
  printf("|");
  pln();
  for (int i = 0; i < output_matrix.size.col; i++) {
    float p_value = get_matrix_element(&output_matrix, 0, i);
    printf("| %d | => | %3d%% | ", i, (int)(p_value * 100));
    print_dashes(p_value * 40);
    pln();
  }
  NeuralNetworkBestChoice choice = neural_network_digit_choice(network);
  printf("Best guess is '%d' at '%d%%' confidence", choice.digit,
         (int)(choice.p_value * 100));
  pln();
  free_matrix(&output_matrix);
};

Matrix desired_output_if_number_is(int activation_number) {
  Matrix return_matrix = new_matrix(1, 10);
  set_matrix_element(&return_matrix, 1, 0, activation_number);
  return return_matrix;
}

float cost_for_matrix(Matrix *output_matrix, Matrix *desired_output_matrix) {
  float cost = 0;
  Matrix diff_matrix = matrix_subtract(desired_output_matrix, output_matrix);
  for (int i = 0; i < diff_matrix.size.row; i++) {
    for (int j = 0; j < diff_matrix.size.col; j++) {
      cost += pow(get_matrix_element(&diff_matrix, i, j), 2);
    }
  }
  return cost;
}

float neural_network_evaluate_fitness(NeuralNetwork *network,
                                      DatasetImage *test_image,
                                      DatasetLabel *test_label) {
  float correctly_identified = 0;
  int total_test = min(test_label->count, test_image->image_count);
  Matrix input_matrix =
      new_matrix(test_image->row_count, test_image->col_count);
  for (int i = 0; i < min(test_label->count, test_image->image_count); i++) {
    free_matrix(&input_matrix);
    input_matrix = get_image_data_as_matrix(test_image, i);
    to_row_matrix(&input_matrix);
    neural_network_set_input_matrix(network, &input_matrix);
    neural_network_forward_propagation(network);
    if (neural_network_digit_choice(network).digit ==
        get_label_data(test_label, i)) {
      correctly_identified++;
    }
  }
  free_matrix(&input_matrix);
  return correctly_identified / total_test;
};

Matrix neural_network_error_matrix(NeuralNetwork *network,
                                   Matrix *desired_output_matrix) {
  neural_network_forward_propagation(network);
  Matrix output_matrix =
      matrix_duplicate(&network->output_neuron_layer.node_value_out_matrix);
  Matrix error_matrix = matrix_subtract(&output_matrix, desired_output_matrix);
  free_matrix(&output_matrix);
  return error_matrix;
}

Matrix neural_network_cost_matrix(NeuralNetwork *network,
                                  Matrix *desired_output_matrix) {
  Matrix error_matrix =
      neural_network_error_matrix(network, desired_output_matrix);
  matrix_square(&error_matrix);
  return error_matrix;
}

void neural_network_backward_propogation(NeuralNetwork *network,
                                         Matrix *desired_output_matrix,
                                         float learning_rate) {
  neural_network_forward_propagation(network);
  int hidden_neuron_count = network->hidden_neuron_count;
  Matrix error_matrix =
      matrix_subtract(&network->output_neuron_layer.node_value_out_matrix,
                      desired_output_matrix);
  Matrix previous_layer_error;
  if (hidden_neuron_count == 0) {
    Matrix dump = layer_backward_propogate_error(
        &network->output_neuron_layer,
        &network->input_neuron_layer.node_value_matrix, &error_matrix,
        learning_rate);
    free_matrix(&dump);
  } else {
    Matrix previous_layer_error = layer_backward_propogate_error(
        &network->output_neuron_layer,
        &network->hidden_neuron_layer_array[hidden_neuron_count - 1]
             .node_value_out_matrix,
        desired_output_matrix, learning_rate);
    for (int l = network->hidden_neuron_count - 1 && false; l > 0; l++) {
      Matrix temp_error = matrix_duplicate(&previous_layer_error);
      free_matrix(&previous_layer_error);
      previous_layer_error = layer_backward_propogate_error(
          &network->hidden_neuron_layer_array[l],
          &network->hidden_neuron_layer_array[l - 1].node_value_out_matrix,
          &temp_error, learning_rate);
      free_matrix(&temp_error);
    }
    Matrix dump = layer_backward_propogate_error(
        &network->hidden_neuron_layer_array[0],
        &network->input_neuron_layer.node_value_matrix, &previous_layer_error,
        learning_rate);
    free_matrix(&dump);
    free_matrix(&previous_layer_error);
  }
  free_matrix(&error_matrix);
}

float neural_network_evaluate_cost(NeuralNetwork *network,
                                   DatasetImage *training_image,
                                   DatasetLabel *training_label) {
  float cost_total;
  Matrix input_matrix =
      new_matrix(training_image->row_count, training_image->col_count);
  Matrix desired_output_matrix = new_matrix(1, 10);
  Matrix output_matrix = new_matrix(1, 10);
  for (int i = 0; i < min(training_label->count, training_image->image_count);
       i++) {
    free_matrix(&input_matrix);
    input_matrix = get_image_data_as_matrix(training_image, i);
    to_row_matrix(&input_matrix);
    network->input_neuron_layer.node_value_matrix = input_matrix;
    neural_network_forward_propagation(network);
    output_matrix = network->output_neuron_layer.node_value_out_matrix;
    cost_total += cost_for_matrix(&output_matrix, &desired_output_matrix);
  }
  return cost_total / min(training_label->count, training_image->image_count);
}

NeuralNetworkBestChoice neural_network_digit_choice(NeuralNetwork *network) {
  NeuralNetworkBestChoice choice = {.p_value = 0};
  Matrix *output_matrix = &network->output_neuron_layer.node_value_out_matrix;
  for (int i = 0; i < output_matrix->size.col; i++) {
    float p_value = get_matrix_element(output_matrix, 0, i);
    if (p_value > choice.p_value) {
      choice.p_value = p_value;
      choice.digit = i;
    }
  }
  return choice;
};

void neural_network_set_input_matrix(NeuralNetwork *network, Matrix *matrix) {
  network->nodes_updated = false;
  if (matrix->size.row != 1 &&
      matrix->size.col != network->input_neuron_layer.node_count) {
    log_("ERROR: Invalid size for input matrix for neural network");
    log_("Expected size: (%d x %d), Got: (%d x %d)", 1,
         network->input_neuron_layer.node_count, matrix->size.row,
         matrix->size.col)
  } else {
    free_matrix(&network->input_neuron_layer.node_value_matrix);
    network->input_neuron_layer.node_value_matrix = matrix_duplicate(matrix);
  }
};
