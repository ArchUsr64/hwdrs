#pragma once

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

u_int32_t u8tou32(u_int8_t *element) {
  u_int32_t return_int = 0;
  for (int i = 0; i < 4; i++) {
    return_int += *(element + i) * pow(256, (3 - i));
  }
  return return_int;
};
