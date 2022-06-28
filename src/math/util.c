#pragma once

#define abs_(x) ((x) > 0) ? ((x)) : (-1 * (x))
#define round_(x) (((x) - (int)(x)) > 0.5) ? ((int)((x) + 1)) : ((int)(x))
#define reciprocal_(x) (1.0 / (x))
#define PI 3.14159
#define E 2.71828

#include "util.h"

float map_to_range(float num_to_map, float input_range_min,
                   float input_range_max, float output_range_min,
                   float output_range_max) {
  num_to_map -= input_range_min;
  num_to_map /= input_range_max - input_range_min;
  num_to_map *= output_range_max - output_range_min;
  num_to_map += output_range_min;
  return num_to_map;
}

float sigmoid(float value) {
  float return_value;
  return_value = reciprocal_(1 + pow(E, -value));
	return return_value;
}
