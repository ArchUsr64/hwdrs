#pragma once

#include "sdl_abstracted.h"

void DrawCircle(SDL_Renderer *renderer, int32_t centreX, int32_t centreY,
                int32_t radius) {
  if (radius == 0) {
    SDL_RenderDrawPoint(RENDERER_PTR, centreX, centreY);
  } else {
    const int32_t diameter = (radius * 2);
    int32_t x = (radius - 1);
    int32_t y = 0;
    int32_t tx = 1;
    int32_t ty = 1;
    int32_t error = (tx - diameter);
    while (x >= y) {
      //  Each of the following renders an octant of the circle
      SDL_RenderDrawPoint(renderer, centreX + x, centreY - y);
      SDL_RenderDrawPoint(renderer, centreX + x, centreY + y);
      SDL_RenderDrawPoint(renderer, centreX - x, centreY - y);
      SDL_RenderDrawPoint(renderer, centreX - x, centreY + y);
      SDL_RenderDrawPoint(renderer, centreX + y, centreY - x);
      SDL_RenderDrawPoint(renderer, centreX + y, centreY + x);
      SDL_RenderDrawPoint(renderer, centreX - y, centreY - x);
      SDL_RenderDrawPoint(renderer, centreX - y, centreY + x);
      if (error <= 0) {
        ++y;
        error += ty;
        ty += 2;
      }
      if (error > 0) {
        --x;
        tx += 2;
        error += (tx - diameter);
      }
    }
  }
}

void draw_circle(float x, float y, float r) {
  ScreenCoordinate circle_pos = {.x = x, .y = y};
  Pixel circle_pos_px = map_to_viewport(&circle_pos);
  int radius_px = window_size().x * r / 2;
  DrawCircle(RENDERER_PTR, circle_pos_px.x, circle_pos_px.y, radius_px);
};

void draw_line(float x1, float y1, float x2, float y2) {
  ScreenCoordinate point_1 = {.x = x1, .y = y1};
  ScreenCoordinate point_2 = {.x = x2, .y = y2};
  Pixel point_1_px = map_to_viewport(&point_1);
  Pixel point_2_px = map_to_viewport(&point_2);
  SDL_RenderDrawLine(RENDERER_PTR, point_1_px.x, point_1_px.y, point_2_px.x,
                     point_2_px.y);
};
void set_draw_colour(float r, float g, float b) {
  SDL_SetRenderDrawColor(RENDERER_PTR, map_to_range(r, 0, 1, 0, 255),
                         map_to_range(g, 0, 1, 0, 255),
                         map_to_range(b, 0, 1, 0, 255), 255);
};
void clear_window() { SDL_RenderClear(RENDERER_PTR); }
void present_to_window() { SDL_RenderPresent(RENDERER_PTR); }
