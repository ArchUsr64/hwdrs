#pragma once

#include "../global_vars.c"
#include "../math/math.h"
#include "../window/map.h"

typedef struct _Grid {
  float *cell;
} Grid;

Grid new_grid(float *grid_cell);
void render_grid(Grid *grid);
float get_cell_value(Grid *grid, int index_x, int index_y);
void set_cell_value(Grid *grid, float value, int index_x, int index_y);
Matrix grid_cells_to_matrix(Grid *grid);
