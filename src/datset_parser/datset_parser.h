#pragma once

#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "../logger.c"
#include "../math/math.h"
#include "../print.c"

typedef struct _DatasetLabel {
  int count;
  u_int8_t *data;
} DatasetLabel;

typedef struct _DatasetImage {
  int image_count;
  int row_count;
  int col_count;
  u_int8_t *data;
} DatasetImage;

DatasetLabel new_dataset_label(const char *file_path);
DatasetImage new_dataset_image(const char *file_path);
u_int8_t get_label_data(DatasetLabel *label_data_set, int index);
u_int8_t get_image_data(DatasetImage *image_data_set, int index_image,
                        int index_row, int index_col);
Matrix get_image_data_as_matrix(DatasetImage *image_data_set,
                              int image_index);
void print_dataset_image(DatasetImage *image_set, int image_index);
