#pragma once

#include "../math/math.h"
#include "../window/map.h"

typedef enum _KeyState {
  KEYDOWN,
  KEYUP,
} KeyState;

typedef struct _MouseState {
  ScreenCoordinate position;
	Pixel position_px;
  KeyState left_button;
  KeyState right_button;
	void (*mouse_up_function)(void);
	void (*mouse_down_function)(void);
} MouseState;

