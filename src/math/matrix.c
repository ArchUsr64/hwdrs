#pragma once

#include "matrix.h"

Matrix new_matrix(int row_count, int col_count) {
  Matrix return_matrix = {.size = {.row = row_count, .col = col_count}};
  return_matrix.element_ptr =
      (Scalar *)calloc(sizeof(Scalar), row_count * col_count);
  return return_matrix;
}

Matrix matrix_product(Matrix *matrix_1, Matrix *matrix_2) {
  if (matrix_1->size.row == 1 && matrix_1->size.col == 16 &&
      matrix_2->size.col == 10) {
  }
  Matrix return_matrix = new_matrix(matrix_1->size.row, matrix_2->size.col);
  if (matrix_1->size.col == matrix_2->size.row) {
    for (int i = 0; i < return_matrix.size.row; i++) {
      for (int j = 0; j < return_matrix.size.col; j++) {
        Scalar element = 0;
        for (int k = 0; k < matrix_1->size.col; k++) {
          element += get_matrix_element(matrix_1, i, k) *
                     get_matrix_element(matrix_2, k, j);
        }
        set_matrix_element(&return_matrix, element, i, j);
      }
    }
  }
  return return_matrix;
}

Matrix matrix_element_wise_product(Matrix *matrix_1, Matrix *matrix_2) {
  Matrix return_matrix = new_matrix(matrix_1->size.row, matrix_1->size.col);
  if (matrix_equivalent(matrix_1, matrix_2)) {
    for (int i = 0; i < return_matrix.size.row; i++) {
      for (int j = 0; j < return_matrix.size.col; j++) {
        Scalar element = 0;
        element = get_matrix_element(matrix_1, i, j) *
                  get_matrix_element(matrix_2, i, j);
        set_matrix_element(&return_matrix, element, i, j);
      }
    }
  } else {
    log_("ERROR: Matrix specified for element wise product have different size "
         "(%d x %d) vs (%d x %d)",
         matrix_1->size.row, matrix_1->size.col, matrix_2->size.row,
         matrix_2->size.col);
  }

  return return_matrix;
}

Matrix matrix_add(Matrix *matrix_1, Matrix *matrix_2) {
  Matrix return_matrix = new_matrix(matrix_1->size.row, matrix_1->size.col);
  if (matrix_equivalent(matrix_1, matrix_2)) {
    for (int i = 0; i < return_matrix.size.row; i++) {
      for (int j = 0; j < return_matrix.size.col; j++) {
        Scalar element = 0;
        element = get_matrix_element(matrix_1, i, j) +
                  get_matrix_element(matrix_2, i, j);
        set_matrix_element(&return_matrix, element, i, j);
      }
    }
  } else {
    log_(
        "ERROR: Matrix specified for element wise addition have different size "
        "(%d x %d) vs (%d x %d)",
        matrix_1->size.row, matrix_1->size.col, matrix_2->size.row,
        matrix_2->size.col);
  }
  return return_matrix;
}

void matrix_add_assign(Matrix *matrix_1, Matrix *matrix_2) {
  Matrix temp_matrix = matrix_duplicate(matrix_1);
  free_matrix(matrix_1);
  *matrix_1 = matrix_add(&temp_matrix, matrix_2);
  free_matrix(&temp_matrix);
}

Matrix matrix_subtract(Matrix *matrix_1, Matrix *matrix_2) {
  Matrix return_matrix = new_matrix(matrix_1->size.row, matrix_1->size.col);
  if (matrix_equivalent(matrix_1, matrix_2)) {
    for (int i = 0; i < return_matrix.size.row; i++) {
      for (int j = 0; j < return_matrix.size.col; j++) {
        Scalar element = 0;
        element = get_matrix_element(matrix_1, i, j) -
                  get_matrix_element(matrix_2, i, j);
        set_matrix_element(&return_matrix, element, i, j);
      }
    }
  } else {
    log_("ERROR: Matrix specified for element wise subtraction have different "
         "size "
         "(%d x %d) vs (%d x %d)",
         matrix_1->size.row, matrix_1->size.col, matrix_2->size.row,
         matrix_2->size.col);
  }
  return return_matrix;
}

void matrix_scale(Matrix *matrix, Scalar scalar) {
  for (int i = 0; i < matrix->size.row; i++) {
    for (int j = 0; j < matrix->size.col; j++) {
      float element = get_matrix_element(matrix, i, j);
      element *= scalar;
      set_matrix_element(matrix, element, i, j);
    }
  }
}

void matrix_transpose(Matrix *matrix) {
  Matrix temp_matrix = matrix_duplicate(matrix);
  *matrix = new_matrix(matrix->size.col, matrix->size.row);
  for (int i = 0; i < matrix->size.row; i++) {
    for (int j = 0; j < matrix->size.col; j++) {
      set_matrix_element(matrix, get_matrix_element(&temp_matrix, j, i), i, j);
    }
  }
  free_matrix(&temp_matrix);
}

void set_matrix_element(Matrix *matrix, Scalar value, int row_index,
                        int col_index) {
  if (row_index >= matrix->size.row || col_index >= matrix->size.col) {
    log_("Error: Trying to access out of bounds matrix element");
  } else
    *(matrix->element_ptr + row_index * matrix->size.col + col_index) = value;
}

Scalar get_matrix_element(Matrix *matrix, int row_index, int col_index) {
  if (row_index >= matrix->size.row || col_index >= matrix->size.col) {
    log_("Error: Trying to access out of bounds matrix element");
    return 0;
  } else
    return *(matrix->element_ptr + row_index * matrix->size.col + col_index);
}

void print_matrix(Matrix *matrix) {
  int value_percision = 2;
  log_("Matrix data: ");
  print_dashes((value_percision + 3) * matrix->size.col + 2);
  printf("\n");
  for (int i = 0; i < matrix->size.row; i++) {
    printf("| ");
    for (int j = 0; j < matrix->size.col; j++) {
      printf("%.*f", value_percision, get_matrix_element(matrix, i, j));
      printf(" ");
    }
    printf("|\n");
  }
  print_dashes((value_percision + 3) * matrix->size.col + 2);
  printf("\n");
}

Matrix matrix_duplicate(Matrix *matrix) {
  Matrix return_matrix = new_matrix(matrix->size.row, matrix->size.col);
  for (int i = 0; i < matrix->size.row; i++)
    for (int j = 0; j < matrix->size.col; j++)
      set_matrix_element(&return_matrix, get_matrix_element(matrix, i, j), i,
                         j);
  return return_matrix;
}

void matrix_fill_random(Matrix *matrix, float range_min, float range_max) {
  for (int i = 0; i < matrix->size.row; i++) {
    for (int j = 0; j < matrix->size.col; j++) {
      set_matrix_element(matrix, random_in_range(range_min, range_max), i, j);
    }
  }
}

void matrix_merge(Matrix *matrix_src, Matrix *matrix_dest) {
  matrix_dest->size.row = matrix_src->size.row;
  matrix_dest->size.col = matrix_src->size.col;
  for (int i = 0; i < matrix_dest->size.row; i++)
    for (int j = 0; j < matrix_dest->size.col; j++)
      set_matrix_element(matrix_dest, get_matrix_element(matrix_src, i, j), i,
                         j);
  free_matrix(matrix_dest);
}

bool matrix_equivalent(Matrix *matrix_1, Matrix *matrix_2) {
  if (matrix_1->size.row == matrix_2->size.row &&
      matrix_1->size.col == matrix_2->size.col) {
    return true;
  }
  return false;
}

void to_row_matrix(Matrix *matrix) {
  Matrix temp_matrix = new_matrix(1, (matrix->size.row * matrix->size.col));
  for (int i = 0; i < matrix->size.row; i++) {
    for (int j = 0; j < matrix->size.col; j++) {
      Scalar value = get_matrix_element(matrix, i, j);
      set_matrix_element(&temp_matrix, value, 0, i * matrix->size.col + j);
    }
  }
  free_matrix(matrix);
  *matrix = matrix_duplicate(&temp_matrix);
  free_matrix(&temp_matrix);
}

void to_col_matrix(Matrix *matrix) {
  Matrix temp_matrix = new_matrix(1, (matrix->size.row * matrix->size.col));
  for (int i = 0; i < matrix->size.row; i++) {
    for (int j = 0; j < matrix->size.col; j++) {
      Scalar value = get_matrix_element(matrix, i, j);
      set_matrix_element(&temp_matrix, value, i * matrix->size.col + j, 0);
    }
  }
  free_matrix(matrix);
  *matrix = matrix_duplicate(&temp_matrix);
  free_matrix(&temp_matrix);
}

Matrix row_matrix_from(Matrix *matrix, int col_index) {
  Matrix return_matrix = new_matrix(1, matrix->size.col);
  for (int i = 0; i < matrix->size.col; i++) {
    Scalar element = get_matrix_element(matrix, 0, i);
    set_matrix_element(matrix, element, 0, i);
  }
  return return_matrix;
}

Matrix col_matrix_from(Matrix *matrix, int row_index) {
  Matrix return_matrix = new_matrix(matrix->size.row, 1);
  for (int i = 0; i < matrix->size.row; i++) {
    Scalar element = get_matrix_element(matrix, i, 0);
    set_matrix_element(matrix, element, i, 0);
  }
  return return_matrix;
}

void free_matrix(Matrix *matrix) { free(matrix->element_ptr); }

void matrix_relu(Matrix *matrix) {
  for (int i = 0; i < matrix->size.row; i++) {
    for (int j = 0; j < matrix->size.col; j++) {
      float value = get_matrix_element(matrix, i, j);
      value = relu(value);
      set_matrix_element(matrix, value, i, j);
    }
  }
}

void matrix_derivative_relu(Matrix *matrix) {
  for (int i = 0; i < matrix->size.row; i++) {
    for (int j = 0; j < matrix->size.col; j++) {
      float value = get_matrix_element(matrix, i, j);
      value = derivative_relu(value);
      set_matrix_element(matrix, value, i, j);
    }
  }
}

void matrix_normal_map(Matrix *matrix) {
  float smallest_element = matrix_smallest_element(matrix);
  float largest_element = matrix_largest_element(matrix);
  for (int i = 0; i < matrix->size.row; i++) {
    for (int j = 0; j < matrix->size.col; j++) {
      float element = get_matrix_element(matrix, i, j);
      if (element > 0)
        element = map_to_range(element, 0, largest_element, 0, 1);
      else
        element = map_to_range(element, smallest_element, 0, -1, 0);
      set_matrix_element(matrix, element, i, j);
    }
  }
}

float matrix_smallest_element(Matrix *matrix) {
  float smallest_element = get_matrix_element(matrix, 0, 0);
  for (int i = 0; i < matrix->size.row; i++) {
    for (int j = 0; j < matrix->size.col; j++) {
      float element = get_matrix_element(matrix, i, j);
      if (element < smallest_element) {
        smallest_element = element;
      }
    }
  }
  return smallest_element;
}

float matrix_largest_element(Matrix *matrix) {
  float largest_element = get_matrix_element(matrix, 0, 0);
  for (int i = 0; i < matrix->size.row; i++) {
    for (int j = 0; j < matrix->size.col; j++) {
      float element = get_matrix_element(matrix, i, j);
      if (element > largest_element) {
        largest_element = element;
      }
    }
  }
  return largest_element;
}

void matrix_normalise(Matrix *matrix) {
  float element_sum = matrix_element_sum(matrix);
  for (int i = 0; i < matrix->size.row; i++) {
    for (int j = 0; j < matrix->size.col; j++) {
      float value = get_matrix_element(matrix, i, j);
      value /= (element_sum != 0) ? element_sum : 1;
      set_matrix_element(matrix, value, i, j);
    }
  }
}

float matrix_element_sum(Matrix *matrix) {
  float element_sum = 0;
  for (int i = 0; i < matrix->size.row; i++) {
    for (int j = 0; j < matrix->size.col; j++) {
      element_sum += get_matrix_element(matrix, i, j);
    }
  }
  return element_sum;
}

void matrix_square(Matrix *matrix) {
  for (int i = 0; i < matrix->size.row; i++) {
    for (int j = 0; j < matrix->size.col; j++) {
      float element = get_matrix_element(matrix, i, j);
      set_matrix_element(matrix, square(element), i, j);
    }
  }
}
