#pragma once

#include "../math/math.h"
#include "../window/map.h"

typedef enum _KeyState {
  KEYDOWN,
  KEYUP,
} KeyState;

typedef struct _KeyboardState {
  KeyState up_key;
  KeyState down_key;
  KeyState left_key;
  KeyState right_key;
  KeyState enter_key;
  KeyState backspace_key;
  KeyState space_key;
} KeyboardState;

typedef struct _MouseState {
  ScreenCoordinate position;
  Pixel position_px;
  KeyState left_button;
  KeyState right_button;
  KeyState middle_button;
  void (*mouse_up_function)(void);
  void (*mouse_down_function)(void);
} MouseState;
