#pragma once

#include "../global_vars.c"
#include "../input/input.c"
#include "../sdl/sdl_abstracted.h"

void render_brush() {
  set_draw_colour(1, 1, 1);
  draw_circle(mouse.position.x, mouse.position.y, brush_size);
}
