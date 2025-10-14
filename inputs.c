#include "inputs.h"

void process_input(struct Inputs *input)
{
  SDL_Event event;
  while (SDL_PollEvent(&event))
  {
    switch (event.type)
    {
      case SDL_EVENT_QUIT:
        input->esc = true;
        break;
      case SDL_EVENT_KEY_DOWN:
        switch (event.key.key)
        {
          case 27:
            input->esc = true;
            break;
          case 97:
            input->a = true;
            break;
          case 115:
            input->s = true;
            break;
          case 100:
            input->d = true;
            break;
          case 119:
            input->w = true;
            break;
          case 108:
            input->l = true;
            break;
          case 112:
            input->p = true;
            break;
          case 49:
            input->gun_1 = true;
            break;
          case 50:
            input->gun_2 = true;
            break;
          case 51:
            input->gun_3 = true;
            break;
          case 52:
            input->gun_4 = true;
            break;
          case 53:
            input->gun_5 = true;
            break;
          case 54:
            input->gun_6 = true;
            break;
          case 55:
            input->gun_7 = true;
            break;
          case 56:
            input->gun_8 = true;
            break;
          case 57:
            input->gun_9 = true;
            break;
          case 1073742049:
            input->shift = true;
            break;
          case 13:
            input->enter = true;
            break;
          default:
            break;
        }
        break;
      case SDL_EVENT_KEY_UP:
        switch (event.key.key)
        {
          case 27:
            input->esc = false;
            break;
          case 97:
            input->a = false;
            break;
          case 115:
            input->s = false;
            break;
          case 100:
            input->d = false;
            break;
          case 119:
            input->w = false;
            break;
          case 108:
            input->l = false;
            break;
          case 112:
            input->p = false;
            break;
          case 49:
            input->gun_1 = false;
            break;
          case 50:
            input->gun_2 = false;
            break;
          case 51:
            input->gun_3 = false;
            break;
          case 52:
            input->gun_4 = false;
            break;
          case 53:
            input->gun_5 = false;
            break;
          case 54:
            input->gun_6 = false;
            break;
          case 55:
            input->gun_7 = false;
            break;
          case 56:
            input->gun_8 = false;
            break;
          case 57:
            input->gun_9 = false;
            break;
          case 1073742049:
            input->shift = false;
            break;
          case 13:
            input->enter = false;
            break;
          default:
            break;
        }
      default:
        break;
    }
  }
}

void clear_inputs(struct Inputs *input)
{
  input->a = false;
  input->s = false;
  input->d = false;
  input->w = false;
  input->shift = false;
  input->space = false;
  input->gun_1 = false;
  input->gun_2 = false;
  input->gun_3 = false;
  input->gun_4 = false;
  input->gun_5 = false;
  input->gun_6 = false;
  input->gun_7 = false;
  input->gun_8 = false;
  input->gun_9 = false;
  input->p = false;
  input->l = false;
  input->enter = false;
  input->esc = false;
}

