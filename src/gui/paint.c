#pragma once

#include "../input/input.c"
#include "grid.h"

void paint_grid(Grid *grid) {
  for (int i = 0; i < GRID_SIZE; i++) {
    for (int j = 0; j < GRID_SIZE; j++) {
      Vector2 cell_mid_point;
      cell_mid_point.x = map_to_range(i + 0.5, 0, GRID_SIZE, -1, 1);
      cell_mid_point.y = map_to_range(j + 0.5, 0, GRID_SIZE, 1, -1);
      Scalar cell_mid_point_to_mouse_distance =
          vec2_dist(&cell_mid_point, &mouse.position);
      float cell_value_to_write =
          (brush_size - 0.8 * cell_mid_point_to_mouse_distance) / brush_size;
      if (mouse.left_button == KEYDOWN &&
          cell_value_to_write > get_cell_value(grid, i, j)) {
        set_cell_value(grid, cell_value_to_write, i, j);
      }
      if (mouse.right_button == KEYDOWN &&
          cell_mid_point_to_mouse_distance < brush_size) {
        set_cell_value(grid, 0.0, i, j);
      }
    }
  }
}
