#pragma once

#include <stdlib.h>

#include "../logger.c"
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
Matrix matrix_scale(Matrix *matrix, Scalar scalar);
Matrix matrix_transpose(Matrix *matrix);
void set_matrix_element(Matrix *matrix, Scalar value, int row_index,
                        int col_index);
Scalar get_matrix_element(Matrix *matrix, int row_index, int col_index);
void print_matrix(Matrix *matrix);
bool matrix_equivalent(Matrix *matrix_1, Matrix *matrix_2);