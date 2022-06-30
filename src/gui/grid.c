#pragma once

#include "grid.h"

Grid new_grid(float *grid_cell) {
  Grid return_grid;
  for (int i = 0; i < GRID_SIZE * GRID_SIZE; i++) {
    grid_cell[i] = 0;
  }
  return_grid.cell = grid_cell;
  return return_grid;
}

void render_grid(Grid *grid) {
  Vector2 cell_size;
  Pixel _window_size = window_size();
  cell_size.x = _window_size.x * 1.0 / GRID_SIZE;
  cell_size.y = _window_size.y * 1.0 / GRID_SIZE;
  SDL_Colour previous_colour;
  SDL_GetRenderDrawColor(RENDERER_PTR, &previous_colour.r, &previous_colour.g,
                         &previous_colour.b, &previous_colour.a);
  SDL_Rect draw_rect;
  draw_rect.w = cell_size.x;
  draw_rect.h = cell_size.y;
  for (int i = 0; i < GRID_SIZE; i++) {
    SDL_RenderDrawLine(RENDERER_PTR, cell_size.x * i, 0, cell_size.x * i,
                       _window_size.y);
    SDL_RenderDrawLine(RENDERER_PTR, 0, cell_size.y * i, _window_size.x,
                       cell_size.y * i);
    for (int j = 0; j < GRID_SIZE; j++) {
      draw_rect.x = cell_size.x * i;
      draw_rect.y = cell_size.y * j;
      float draw_colour = get_cell_value(grid, i, j);
      draw_colour = draw_colour * 255;
      SDL_SetRenderDrawColor(RENDERER_PTR, draw_colour, draw_colour,
                             draw_colour, 255);
      SDL_RenderFillRect(RENDERER_PTR, &draw_rect);
    }
    SDL_SetRenderDrawColor(RENDERER_PTR, previous_colour.r, previous_colour.g,
                           previous_colour.b, previous_colour.a);
  }
  for (int i = 0; i < GRID_SIZE; i++) {
    SDL_RenderDrawLine(RENDERER_PTR, cell_size.x * i, 0, cell_size.x * i,
                       _window_size.y);
    SDL_RenderDrawLine(RENDERER_PTR, 0, cell_size.y * i, _window_size.x,
                       cell_size.y * i);
  }
}

float get_cell_value(Grid *grid, int index_x, int index_y) {
  return *(grid->cell + (index_x * GRID_SIZE) + index_y);
}

void set_cell_value(Grid *grid, float value, int index_x, int index_y) {
  *(grid->cell + (index_x * GRID_SIZE) + index_y) = value;
}

Matrix grid_cells_to_matrix(Grid *grid) {
  Matrix return_matrix = new_matrix(GRID_SIZE, GRID_SIZE);
  for (int i = 0; i < return_matrix.size.row; i++) {
    for (int j = 0; j < return_matrix.size.col; j++) {
      set_matrix_element(&return_matrix, get_cell_value(grid, j, i), i, j);
    }
  }
  return return_matrix;
}
