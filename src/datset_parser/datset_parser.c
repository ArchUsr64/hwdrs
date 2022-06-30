#pragma once

#include "datset_parser.h"

void set_image_data(DatasetImage *image_data_set, u_int8_t value,
                    int index_image, int index_row, int index_col);

void set_label_data(DatasetLabel *label_data_set, u_int8_t value, int index);

u_int32_t parse_u_int_32(FILE *file_ptr);

FILE *ffopen(const char *file_path);

DatasetLabel new_dataset_label(const char *file_path) {
  DatasetLabel return_dataset_label;
  FILE *file_ptr = ffopen(file_path);
  u_int32_t magic_number = parse_u_int_32(file_ptr);
  assert(magic_number == 2049);
  return_dataset_label.count = parse_u_int_32(file_ptr);
  return_dataset_label.data =
      (u_int8_t *)calloc(sizeof(u_int8_t), return_dataset_label.count);
  for (int i = 0; i < return_dataset_label.count; i++) {
    u_int8_t value = fgetc(file_ptr);
    set_label_data(&return_dataset_label, value, i);
  }
  fclose(file_ptr);
  return return_dataset_label;
};

DatasetImage new_dataset_image(const char *file_path) {
  DatasetImage return_dataset_image;
  FILE *file_ptr = ffopen(file_path);
  u_int32_t magic_number = parse_u_int_32(file_ptr);
  assert(magic_number == 2051);
  return_dataset_image.image_count = parse_u_int_32(file_ptr);
  return_dataset_image.row_count = parse_u_int_32(file_ptr);
  return_dataset_image.col_count = parse_u_int_32(file_ptr);
  return_dataset_image.data =
      (u_int8_t *)calloc(sizeof(u_int8_t), (return_dataset_image.image_count *
                                            return_dataset_image.col_count *
                                            return_dataset_image.row_count));
  for (int i = 0; i < return_dataset_image.image_count; i++) {
    for (int j = 0; j < return_dataset_image.row_count; j++) {
      for (int k = 0; k < return_dataset_image.col_count; k++) {
        u_int8_t value = fgetc(file_ptr);
        set_image_data(&return_dataset_image, value, i, j, k);
      }
    }
  }
  fclose(file_ptr);
  return return_dataset_image;
}

void print_dataset_image(DatasetImage *image_set, int image_index) {
  log_("Image data at index %d:", image_index);
  print_dashes(3 * image_set->col_count + 3);
  for (int j = 0; j < image_set->row_count; j++) {
    printf("| ");
    for (int k = 0; k < image_set->col_count; k++) {
      int value = get_image_data(image_set, image_index, j, k);
      if (value == 0) {
        printf("   ");
      } else {
        printf("%2x ", value);
      }
    }
    printf("|\n");
  }
  print_dashes(3 * image_set->col_count + 3);
};

u_int8_t get_label_data(DatasetLabel *label_data_set, int index) {
  return *(label_data_set->data + index);
};

void set_label_data(DatasetLabel *label_data_set, u_int8_t value, int index) {
  *(label_data_set->data + index) = value;
};

u_int8_t get_image_data(DatasetImage *image_data_set, int index_image,
                        int index_row, int index_col) {
  return *(image_data_set->data +
           index_image *
               (image_data_set->row_count * image_data_set->col_count) +
           (index_row * image_data_set->col_count) + index_col);
};

void set_image_data(DatasetImage *image_data_set, u_int8_t value,
                    int index_image, int index_row, int index_col) {
  *(image_data_set->data +
    index_image * (image_data_set->row_count * image_data_set->col_count) +
    (index_row * image_data_set->col_count) + index_col) = value;
};

u_int32_t parse_u_int_32(FILE *file_ptr) {
  u_int8_t return_number_digit[4];
  for (int i = 0; i < 4; i++) {
    return_number_digit[i] = fgetc(file_ptr);
  }
  u_int32_t return_number = u8tou32(return_number_digit);
  return return_number;
};

FILE *ffopen(const char *file_path) {
  FILE *file_ptr = fopen(file_path, "r");
  if (file_ptr == NULL) {
    log_("Failed to open '%s'", file_path);
  }
  return file_ptr;
}
