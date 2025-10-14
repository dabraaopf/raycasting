#ifndef INPUTS_HANDLER
#define INPUTS_HANDLER
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3/SDL_events.h>

typedef struct Inputs
{
  bool a;
  bool s;
  bool d;
  bool w;
  bool p;
  bool l;
  bool shift;
  bool space;
  bool gun_1;
  bool gun_2;
  bool gun_3;
  bool gun_4;
  bool gun_5;
  bool gun_6;
  bool gun_7;
  bool gun_8;
  bool gun_9;
  bool esc;
  bool enter;
};


void process_input(struct Inputs *input);
void clear_inputs(struct Inputs *input);

#endif
