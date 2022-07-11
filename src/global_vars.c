#pragma once

#include <SDL2/SDL.h>
#include <stdbool.h>

SDL_Texture *DEBUGGING_TEXTURE;
const char *FONT_PATH = "res/font/FiraCode-Medium.ttf";
const int INITIAL_WINDOW_WIDTH = 600;
const int INITIAL_WINDOW_HEIGHT = 600;
SDL_Renderer *RENDERER_PTR;
SDL_Window *WINDOW_PTR;
int GRID_SIZE;
float LEARNING_RATE;
float brush_size = 0.05;
