#pragma once

#include <math.h>
#include <stdlib.h>

#define min(x, y) (((x) > (y)) ? (y) : (x))
#define max(x, y) (((x) < (y)) ? (y) : (x))
#define abs_(x) ((x) > 0) ? ((x)) : (-1 * (x))
#define round_(x) (((x) - (int)(x)) > 0.5) ? ((int)((x) + 1)) : ((int)(x))
#define reciprocal_(x) (1.0 / (x))
#define PI 3.14159
#define E 2.71828

float map_to_range(float num_to_map, float range_input_min,
                   float range_input_max, float range_output_min,
                   float range_output_max);

float sigmoid(float value);

float random_in_range(float range_min, float range_max);

u_int32_t u8tou32(u_int8_t *element);
