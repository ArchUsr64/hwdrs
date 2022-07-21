#pragma once

#include <SDL2/SDL.h>
#include <stdbool.h>

#include "../logger.c"
#include "../math/math.h"
#include "../window/map.h"
#include "input_state.h"

bool user_quit = false;
const int INPUT_UPDATE_FREQUENCY = 200;
void mouse_up_function() {
  if (brush_size < 1)
    brush_size += 0.01;
};
void mouse_down_function() {
  if (brush_size > 0.05)
    brush_size -= 0.01;
};
MouseState mouse;
KeyboardState keyboard;

void *input_thread(void *var) {
  mouse.mouse_up_function = &mouse_up_function;
  mouse.mouse_down_function = &mouse_down_function;
  int update_interval_in_ms = 1000 / INPUT_UPDATE_FREQUENCY;
  SDL_Event event;
  while (!user_quit) {
    int start_time = SDL_GetTicks();
    SDL_PumpEvents();
    Uint32 mouse_buttons =
        SDL_GetMouseState(&mouse.position_px.x, &mouse.position_px.y);
    mouse.position = map_to_world(&mouse.position_px);
    mouse.left_button =
        ((mouse_buttons & SDL_BUTTON_LMASK) != 0) ? KEYDOWN : KEYUP;
    mouse.right_button =
        ((mouse_buttons & SDL_BUTTON_RMASK) != 0) ? KEYDOWN : KEYUP;
    mouse.middle_button =
        ((mouse_buttons & SDL_BUTTON_MMASK) != 0) ? KEYDOWN : KEYUP;
    while (SDL_PollEvent(&event)) {
      switch (event.key.type) {
      case SDL_QUIT:
        user_quit = true;
        break;
      case SDL_MOUSEWHEEL:
        if (event.wheel.y > 0) {
          (*mouse.mouse_up_function)();
        }
        if (event.wheel.y < 0) {
          (*mouse.mouse_down_function)();
        }
        break;
      case SDL_KEYDOWN:
      case SDL_KEYUP:
        switch (event.key.keysym.scancode) {
        case SDL_SCANCODE_UP:
        case SDL_SCANCODE_W:
          keyboard.up_key = (event.key.type == SDL_KEYDOWN);
          break;
        case SDL_SCANCODE_DOWN:
        case SDL_SCANCODE_S:
          keyboard.down_key = (event.key.type == SDL_KEYDOWN);
          break;
        case SDL_SCANCODE_LEFT:
        case SDL_SCANCODE_A:
          keyboard.left_key = (event.key.type == SDL_KEYDOWN);
          break;
        case SDL_SCANCODE_RIGHT:
        case SDL_SCANCODE_D:
          keyboard.right_key = (event.key.type == SDL_KEYDOWN);
          break;
        case SDL_SCANCODE_RETURN:
          keyboard.enter_key = (event.key.type == SDL_KEYDOWN);
          break;
        case SDL_SCANCODE_BACKSPACE:
          keyboard.backspace_key = (event.key.type == SDL_KEYDOWN);
          break;
        case SDL_SCANCODE_SPACE:
          keyboard.space_key = (event.key.type == SDL_KEYDOWN);
          break;
        default:
          break;
        }
        break;
      }
    }
    while (SDL_GetTicks() < start_time + update_interval_in_ms)
      SDL_Delay(1);
  }
  return NULL;
}

void mouse_wheel_up() {}
void mouse_wheel_down() {}
