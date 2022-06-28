#include <SDL2/SDL.h>
#include <pthread.h>
#include <stdlib.h>

#include "global_vars.c"
#include "gui/brush.c"
#include "gui/grid.c"
#include "gui/gui.h"
#include "gui/paint.c"
#include "input/input.c"
#include "logger.c"
#include "math.h"
#include "math/math.h"
#include "math/matrix.c"
#include "math/vector.c"
#include "window/map.c"

int main() {
  if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
    log_("Failed to initialise SDL: %s", SDL_GetError());
    exit(-1);
  }
  pthread_t input_thread_id;
  pthread_create(&input_thread_id, NULL, input_thread, NULL);
  WINDOW_PTR = SDL_CreateWindow("Test", SDL_WINDOWPOS_CENTERED,
                                SDL_WINDOWPOS_CENTERED, INITIAL_WINDOW_WIDTH,
                                INITIAL_WINDOW_HEIGHT, SDL_WINDOW_RESIZABLE);
  if (!WINDOW_PTR) {
    log_("Failed to create window: %s", SDL_GetError());
    exit(-1);
  }
  RENDERER_PTR = SDL_CreateRenderer(
      WINDOW_PTR, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  if (!RENDERER_PTR) {
    log_("Failed to create renderer: %s", SDL_GetError());
    exit(-1);
  }
  float cells[GRID_SIZE * GRID_SIZE];
  cells[11 * GRID_SIZE + 2] = 0.5;
  Grid grid = new_grid(cells);
  while (!user_quit) {
    SDL_SetRenderDrawColor(RENDERER_PTR, 0, 0, 0, 255);
    SDL_RenderClear(RENDERER_PTR);
    SDL_SetRenderDrawColor(RENDERER_PTR, 255, 255, 255, 255);
    paint_grid(&grid);
    render_grid(&grid);
    render_brush();
    SDL_RenderPresent(RENDERER_PTR);
  }
}
