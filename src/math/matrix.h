#pragma once

#include <stdlib.h>

#include "../logger.c"
#include "../math/math.h"
#include "../print.c"

typedef float Scalar;

typedef struct _MatrixSize {
  int row;
  int col;
} MatrixSize;

typedef struct _Matrix {
  Scalar *element_ptr;
  MatrixSize size;
} Matrix;

Matrix new_matrix(int row_count, int col_count);
Matrix matrix_product(Matrix *matrix_1, Matrix *matrix_2);
Matrix matrix_add(Matrix *matrix_1, Matrix *matrix_2);
Matrix matrix_subtract(Matrix *matrix_1, Matrix *matrix_2);
void matrix_scale(Matrix *matrix, Scalar scalar);
Matrix matrix_transpose(Matrix *matrix);
void matrix_fill_random(Matrix *matrix, float range_min, float range_max);
void set_matrix_element(Matrix *matrix, Scalar value, int row_index,
                        int col_index);
Scalar get_matrix_element(Matrix *matrix, int row_index, int col_index);
void print_matrix(Matrix *matrix);
Matrix matrix_duplicate(Matrix *matrix);
void matrix_merge(Matrix *matrix_src, Matrix *matrix_dest);
void matrix_copy(Matrix *matrix_src, Matrix *matrix_dest);
bool matrix_equivalent(Matrix *matrix_1, Matrix *matrix_2);
Matrix row_matrix_from(Matrix *matrix);
void matrix_sigmoid(Matrix *matrix);
void free_matrix(Matrix *matrix);
void matrix_normalise(Matrix *matrix);
float matrix_element_sum(Matrix *matrix);
