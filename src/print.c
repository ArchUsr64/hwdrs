#pragma once

#define pln() printf("\n")

#include <stdio.h>

void print_char(char char_to_print, int char_count) {
  for (int i = 0; i < char_count; i++) {
    printf("%c", char_to_print);
  }
}

void print_dashes(int dash_count) { print_char('-', dash_count); }

void print_hashes(int hash_count) { print_char('#', hash_count); }

void print_spaces(int space_count) { print_char(' ', space_count); }

void set_cursor_origin() { printf("\e[1;1H\e[2J"); }
