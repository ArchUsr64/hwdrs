#pragma once

#include <math.h>

typedef float Scalar;

typedef struct _Vec2 {
  Scalar x;
  Scalar y;
} Vector2;
Vector2 vec2_new(Scalar x, Scalar y);
Vector2 vec2_add(Vector2 *vec_1, Vector2 *vec_2);
Vector2 vec2_subtract(Vector2 *vec_1, Vector2 *vec_2);
Vector2 vec2_scale(Vector2 *vec_1, Scalar scl);
Vector2 vec2_normalise(Vector2 *vec);
Scalar vec2_dist(Vector2 *vec_1, Vector2 *vec_2);
Scalar vec2_dot_product(Vector2 *vec_1, Vector2 *vec_2);
Scalar vec2_length(Vector2 *vec);
