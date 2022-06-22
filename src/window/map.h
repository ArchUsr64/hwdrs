#pragma once

#include <SDL2/SDL.h>

#include "../global_vars.c"
#include "../math/math.h"

typedef Vector2 ScreenCoordinate;

typedef struct _Pixel {
	int x;
	int y;
} Pixel;

ScreenCoordinate map_to_world();
Pixel map_to_viewport(ScreenCoordinate *);
Pixel window_size();
float window_aspect();
