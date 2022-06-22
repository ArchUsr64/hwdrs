#pragma once

#include "map.h"

Pixel window_size() {
  Pixel window_size;
  SDL_GetWindowSize(WINDOW_PTR, &window_size.x, &window_size.y);
  return window_size;
}

Pixel map_to_viewport(ScreenCoordinate *vec_view) {
  Pixel _window_size = window_size();
  Pixel return_pixel;
  return_pixel.x = round_(map_to_range(vec_view->x, -1, 1, 0, _window_size.x));
  return_pixel.y = round_(map_to_range(vec_view->y, 1, -1, 0, _window_size.y));
  return return_pixel;
}

ScreenCoordinate map_to_world(Pixel *pixel_view) {
  Pixel _window_size = window_size();
  ScreenCoordinate return_vec;
  return_vec.x = map_to_range(pixel_view->x, 0, _window_size.x, -1, 1);
  return_vec.y = map_to_range(pixel_view->y, 0, _window_size.y, 1, -1);
  return return_vec;
}

float window_aspect() {
  Pixel _window_size = window_size();
  return ((float)_window_size.x) / _window_size.y;
}
