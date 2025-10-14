#ifndef INIT_GAME
#define INIT_GAME
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#define SDL_FLAGS SDL_INIT_VIDEO

struct Game
{
  SDL_Window *window;
  SDL_Renderer *render;
  SDL_Surface *surface;
  uint32_t *pixels;
  SDL_PixelFormatDetails *format;
  char *name;
  int width;
  int height;
  
};

bool initialize_SDL(struct Game *g);
void clean(struct Game *g);
uint32_t rgb(uint8_t red, uint8_t green, uint8_t blue, struct Game *g);
void lock_to_draw(struct Game *g);
void update_draw(struct Game *g);
void delay(size_t delay_ms);
#endif
