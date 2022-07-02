#pragma once

#define log_(...)                                                              \
  print_timestamp();                                                           \
  printf(__VA_ARGS__);                                                         \
  printf("\n");

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

struct tm local;

void print_timestamp() {
  int hours, minutes, seconds, day, month, year;
  time_t now;
  time(&now);
  local = *localtime(&now);
  hours = local.tm_hour;
  minutes = local.tm_min;
  seconds = local.tm_sec;
  printf("[%02d:%02d:%02d] ", hours, minutes, seconds);
};
