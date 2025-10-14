#include "init.h"

bool initialize_SDL(struct Game *g)
{
  if (!SDL_Init(SDL_FLAGS))
  {
    printf("Failed to initialize SDL: %s", SDL_GetError());
    return false;
  }
 
  g->window = SDL_CreateWindow(g->name, g->width, g->height, 0);
  if (!g->window)
  {
    printf("Failed to create the window: %s", SDL_GetError());
    return false;
  }

  g->render = SDL_CreateRenderer(g->window, NULL);
  if (!g->render)
  {
    printf("Failed to create the renderer: %s", SDL_GetError());
    return false;
  }

  g->surface = SDL_GetWindowSurface(g->window);
  g->format = SDL_GetPixelFormatDetails(g->surface->format);
  memset(g->surface->pixels, 0, sizeof(uint32_t) * g->width * g->height);
  return true;
}

void clean(struct Game *g)
{
  if (g->render)
  {
    SDL_DestroyRenderer(g->render);
    g->render = NULL;
  }

  if(g->surface)
  {
    SDL_DestroySurface(g->surface);
    g->surface = NULL;
  }

  if(g->window)
  {
    SDL_DestroyWindow(g->window);
    g->window = NULL;
  }

  SDL_Quit();
}

uint32_t rgb(uint8_t red, uint8_t green, uint8_t blue, struct Game *g)
{
  return (uint32_t)SDL_MapRGB(g->format, NULL, red, green, blue);
}

void lock_to_draw(struct Game *g)
{
  SDL_LockSurface(g->surface);
}

void update_draw(struct Game *g)
{
  SDL_UnlockSurface(g->surface);
  SDL_UpdateWindowSurface(g->window);
}

void delay(size_t delay_ms)
{
    SDL_Delay(delay_ms);
}
