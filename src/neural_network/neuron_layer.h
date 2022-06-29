#pragma once

#include "../math/math.h"

typedef struct _InputNeuronLayer {
  int node_count;
  Matrix node_value_matrix;
} InputNeuronLayer;

typedef struct _HiddenNeuronLayer {
  int node_count;
  Matrix node_value_matrix;
  Matrix node_weight_matrix;
  Matrix node_bias_matrix;
} HiddenNeuronLayer;

typedef HiddenNeuronLayer OutputNeuronLayer;

OutputNeuronLayer new_output_layer(int input_node_count, int node_count);

void layer_feed_input(HiddenNeuronLayer *layer, Matrix *input_matrix);
