#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <sys/time.h>
#include <math.h>
#include "init.h"
#include "inputs.h"
#include "vector.h"

const int GAME_WIDTH = 800;
const int GAME_HEIGHT = 600;
const char GAME_WINDOW_NAME[100] = "GameWindow";
const float STEP_SIZE = 2.0f;
const float PI = 3.1415;
const float TURN_ANGLE = PI / 1.2f;
const short SQUARE_SIZE = 10;
const short BORDER_SIZE = 1;
const short PLAYER_SIZE = 3;

struct Map
{
  short *cells;
  int rows;
  int cols;
};

struct Ray
{
  float height;
  float z_index;
  int texture;
};

struct Rays 
{
  struct Ray *rays;
  size_t length;
};

struct Player
{
  Vec2f position;
  Vec2f direction;
  float health;
  int current_gun;
};

struct GameEngine
{
  struct Game game;
  struct Map map;
  struct Rays rays;
  struct Player player;
  struct Inputs inputs;
};

void pixel(int x, int y, uint32_t color, uint32_t *p)
{
  if (x < 0 || x >= GAME_WIDTH || y < 0 || y >= GAME_HEIGHT)
  {
    return;
  }
  p[y*GAME_WIDTH + x] = color; 
}

void draw_minimap(struct GameEngine *g)
{
  size_t i, j, k, w, index, value;
  uint32_t color;
  for (i=0;i<g->map.rows;++i)
  {
    for (j=0;j<g->map.cols;++j)
    {
      index = i*g->map.cols + j;
      value = g->map.cells[index];
      if (value == 0)
      {
        color = rgb(240,240,240, &g->game);
      }
      else
      {
        color = rgb(40,40,40, &g->game);
      }
      for (w = 0; w < SQUARE_SIZE; ++w)
      {
        for (k=0; k< SQUARE_SIZE;++k)
        {

          pixel(
              SQUARE_SIZE + i*SQUARE_SIZE + (i * BORDER_SIZE * 2)  + k,
              SQUARE_SIZE + j*SQUARE_SIZE + (j * BORDER_SIZE * 2)  + w,
              color,
              g->game.surface->pixels
            );
            
        }
      } 
    }
  }
  size_t pxp, pyp;
  pxp = SQUARE_SIZE + g->player.position.x * SQUARE_SIZE - 1;
  pyp = SQUARE_SIZE + g->player.position.y * SQUARE_SIZE - 1;
  color = rgb(255,0,0, &g->game);
   
  for (w = 0; w < PLAYER_SIZE; ++w)
  {
    for (k=0; k< PLAYER_SIZE;++k)
    {
      pixel(
          pxp+w,
          pyp+k,
          color,
          g->game.surface->pixels
        );
        
    }
  } 
  g->inputs.p = false;
}

void draw_background(struct Game *g)
{
  int i = 0;
  int j = 0;
  for(i=0;i<g->height;++i)
  {
    for(j=0;j<g->width;++j)
    {
      if (i<g->height/2)
      { pixel(j, i,rgb(0,255,0, g), g->surface->pixels); }
      else
      {
        pixel(j, i,rgb(0,0,255, g), g->surface->pixels);
      }
    }
  }
}

void draw(struct GameEngine *g)
{
  lock_to_draw(&g->game);
  draw_background(&g->game);
  draw_minimap(g);
  update_draw(&g->game);
}

/* maybe in the future, join everything in the Game struct, so no need to */
/* pass lots of parameters */

void cast_rays(struct GameEngine *g)
{

}

void update_player(struct GameEngine *g, double delta_ms)
{
  double proportion = (double)(delta_ms / 1000);
  if (g->inputs.w)
  {
    g->player.position.x += STEP_SIZE * proportion  * g->player.direction.x;
    g->player.position.y += STEP_SIZE * proportion * g->player.direction.y;
  }
  if (g->inputs.s)
  {
    g->player.position.x -= STEP_SIZE * proportion * g->player.direction.x;
    g->player.position.y -= STEP_SIZE * proportion * g->player.direction.y;
  }
  if (g->inputs.a)
  {
    rotateV2f(&g->player.direction, TURN_ANGLE * proportion);
  }

  if (g->inputs.d)
  {
    rotateV2f(&g->player.direction, TURN_ANGLE * proportion * (-1));
  }
}

bool update(struct GameEngine *g, double delta_ms)
{
  process_input(&g->inputs);
  update_player(g, delta_ms);
  cast_rays(g);
  return !g->inputs.esc;
}

void load_map(struct Map *map)
{
  FILE *p = fopen("./assets/map_1.txt", "r");
  char dv;
  int cell, rows, cols, counter, i;
  if (!p)
  {
    printf("Failed to load the map file. \n");
    return;
  }
  if (fscanf(p, "%ix%i%c",&rows, &cols, &dv) != 3)
  {
    printf("Failed to find the map header, can't continue\n");
    fclose(p);
    return;
  }
  map->rows = rows;
  map->cols = cols;
  counter = rows * cols;
  map->cells = malloc(sizeof(short)*counter);
  i = 0;
  while(fscanf(p,"%i%c",&cell,&dv)==2)
  {
    map->cells[i++] = cell; 
    if (i >= counter)
    {
      break;
    }
  } 
  fclose(p);
}


int main(int argc, char *argv[])
{
    struct GameEngine g;
    /*struct Game game = {0};*/
    /*struct Inputs inp = {false};*/
    /*struct Rays rays;*/
    /*struct Player player;*/
    /*struct Map map;*/
    struct timeval current, last;
    double delta_ms;
    gettimeofday(&last, NULL);
    g.player.position.x = 3.0f;
    g.player.position.y = 3.0f;
    g.player.direction.x = 0.0f;
    g.player.direction.y = 1.0f;
    g.rays.rays = malloc(sizeof(struct Ray) * GAME_WIDTH);
    g.rays.length = GAME_WIDTH;
    g.game.name = &GAME_WINDOW_NAME;
    g.game.width = GAME_WIDTH;
    g.game.height = GAME_HEIGHT;

    if (!initialize_SDL(&g.game))
    {
      return 1;
    }
    load_map(&g.map);
    if (g.map.cells == NULL)
    {
      return 1;
    } 

    while(1)
    {
      gettimeofday(&current, NULL);
      delta_ms = (current.tv_usec - last.tv_usec) / 1000;
      gettimeofday(&last, NULL);
      if (delta_ms < 0)
      {
        delta_ms = 0;
      }
      if(!update(&g, delta_ms))
      {
        break;
      }
      draw(&g);
    }

    clean(&g.game);
    if (g.rays.rays != NULL) 
    {
      free(g.rays.rays);
      g.rays.rays = NULL;
    }
    if (g.map.cells != NULL)
    { 
      free(g.map.cells);
      g.map.cells= NULL;
    }
    printf("Leaving the game now...\n");
    return 0;
}
