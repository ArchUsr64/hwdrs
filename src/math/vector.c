#pragma once

#include "vector.h"

Vector2 vec2_new(Scalar x, Scalar y) {
  Vector2 return_vec = {.x = x, .y = y};
  return return_vec;
}
Vector2 vec2_add(Vector2 *vec_1, Vector2 *vec_2) {
  return vec2_new(vec_1->x + vec_2->x, vec_1->y + vec_2->y);
}
Vector2 vec2_subtract(Vector2 *vec_1, Vector2 *vec_2) {
  return vec2_new(vec_1->x - vec_2->x, vec_1->y - vec_2->y);
}
Vector2 vec2_scale(Vector2 *vec, Scalar scl) {
  return vec2_new(vec->x * scl, vec->y * scl);
}
Vector2 vec2_normalise(Vector2 *vec) {
  return vec2_scale(vec, 1.0 / vec2_length(vec));
}
Scalar vec2_dist(Vector2 *vec_1, Vector2 *vec_2) {
  Vector2 vec_1_to_vec_2 = vec2_subtract(vec_1, vec_2);
  return vec2_length(&vec_1_to_vec_2);
}
Scalar vec2_dot(Vector2 *vec_1, Vector2 *vec_2) {
  return ((vec_1->x * vec_2->x) + (vec_1->y * vec_2->y));
}
Scalar vec2_length(Vector2 *vec) {
  return sqrt(pow(vec->y, 2) + pow(vec->x, 2));
}
