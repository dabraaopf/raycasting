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
const char GAME_WINDOW_NAME[100] = "Raycasting";
const double STEP_SIZE = 2.5;
const double PI = 3.1415;
const double TURN_ANGLE = PI / 1.2;
const double MS_TO_SEC = 1000.0;
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
  for (i=0;i<g->map.cols;++i)
  {
    for (j=0;j<g->map.rows;++j)
    {
      index = j*g->map.cols + i;
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
  pxp = SQUARE_SIZE+(g->player.position.x*SQUARE_SIZE)+(g->player.position.x*BORDER_SIZE*2)-1;
  pyp = SQUARE_SIZE+(g->player.position.y*SQUARE_SIZE)+(g->player.position.y*BORDER_SIZE*2)-1;
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

  double nxp, nyp;
  nxp = PLAYER_SIZE * g->player.direction.x;
  nyp = PLAYER_SIZE * g->player.direction.y;
  double pxn, pyn;
  pxn = pxp + (PLAYER_SIZE / 2);
  pyn = pyp + (PLAYER_SIZE / 2);
  color = rgb(0,0,255, &g->game);
  for(w = 0; w < PLAYER_SIZE * 1.5; ++w)
  {
      pixel(
          (int) pxn + (w*nxp),
          (int) pyn + (w*nyp),
          color,
          g->game.surface->pixels
        );
  }
  
}

void draw_background(struct Game *g)
{
  int i = 0;
  int j = 0;
  uint32_t color_top, color_bottom;
  color_top = rgb(0,255,0, g);
  color_bottom = rgb(0,0,255, g);
  for(i=0;i<g->height;++i)
  {
    for(j=0;j<g->width;++j)
    {
      if (i<g->height/2)
      { pixel(j, i,color_top, g->surface->pixels); }
      else
      {
        pixel(j, i,color_bottom, g->surface->pixels);
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


/* The code goes like:                                                          */
/*  - from the player position + direction, trace the camera plane              */
/*    this plane goes to -1 to +1 with the 0 on the end of the                  */
/*    player + position vector                                                  */
/*  - the numeber of rays casted is the same of pixel columns for the screen    */
/*    which means -1 is the column 0 and +1 is the column (GAME_WIDTH - 1)      */
/*  - for this camera plane, mesure the step size needed to go to the next      */
/*    cell in the map, both in X and Y                                          */
/*  - calculate how much you need to walk to reach the next cell, both in X and */
/*    Y                                                                         */
/*  - interate over it until you find the next non empty cell, always adding to */
/*    the smaller walking vector, one step unit at time                         */
/*  - if the ray is longer than the map, or if you hit a wall, break out from   */
/*    the loop and calculate the correct distance to avoid the fish eye         */
/*    effect                                                                    */
/*  - set the distance to the Z-Buffer, you will need it latter.                */
/*  - draw the line, apply texture and maybe a fog effect.                      */

void cast_rays(struct GameEngine *g)
{
  size_t x;
  double camera_pos;
  Vec2f camera_plane = {0.0f};
  Vec2f step = {0.0f};
  Vec2f walked = {0.0f};
  Vec2f hit_spot = {0};
  bool hit_x;
  bool hit_y;
  bool exited = false;
  for (x = 0; x < g->game.width; ++x)
  {
    hit_x = false;
    hit_y = false;
    exited = false;
    camera_pos = ((double)x / (g->game.width/2.0)) - 1;
    camera_plane.x = camera_pos * g->player.direction.y + g->player.direction.x; 
    camera_plane.y = camera_pos * g->player.direction.x + g->player.direction.y;
    step.x = 1.0;
    if (camera_plane.y != 0.0)
    {
      step.x = camera_plane.x / camera_plane.y;
    }
    step.y = 1.0 - step.x; 
    if (step.x == 0.0)
    {
      step.x = 1e10f;
    }
    if (step.y == 0.0)
    {
      step.y = 1e10f;
    }
    if (camera_plane.x > 0)
    {
      walked.x = floor(g->player.position.x + 1) - g->player.position.x;
    }
    else
    {
      walked.x = g->player.position.x - floor(g->player.position.x);
      walked.x *= -1;
      step.x *= -1;
    }
    if (camera_plane.y > 0)
    {
      walked.y = floor(g->player.position.y + 1) - g->player.position.y;
    }
    else
    {
      walked.y = g->player.position.y - floor(g->player.position.y);
      walked.y *= -1;
      step.y *= -1;
    }
    hit_spot.x = g->player.position.x;
    hit_spot.y = g->player.position.y;
    while (!hit_x && !hit_y && !exited)
    {
      if (abs(walked.y) > abs(walked.x))
      {
        hit_spot.x = g->player.position.x + walked.x;
        hit_spot.y = hit_spot.y;
        walked.x += step.x;
        if (hit_spot.x < 0 || hit_spot.x >= g->map.cols)
        {
          exited = true;
        }
        else if (g->map.cells[(int)(floor(hit_spot.y) * g->map.cols + floor(hit_spot.x))] > 9)
        {
          hit_x = true;
        }
      }
      else
      {
        hit_spot.y = g->player.position.y + walked.y;
        hit_spot.x = hit_spot.x;
        walked.y += step.y;
        if (hit_spot.y < 0 || hit_spot.y >= g->map.rows)
        {
          exited = true;
        }
        else if (g->map.cells[(int)(floor(hit_spot.y) * g->map.cols + floor(hit_spot.x))] > 9)
        {
          hit_y = true;
        }
      }
    }
    if (g->inputs.p)
    {
      printf("Hit_x: %b, hit_y: %b, hit_pos.x: %lf, hit_pos.y: %lf\n", 
          hit_x, hit_y, hit_spot.x, hit_spot.y);
      printf("Walked_x: %lf, Walked_y: %lf, step.x: %lf, step.y: %lf\n", 
          walked.x, walked.y, step.x, step.y);
    }
  }
  g->inputs.p = false;
}

/* add code to bound the player to the map and slide */
/* on the walls */
void update_player(struct GameEngine *g, double delta_ms)
{
  double proportion = STEP_SIZE / MS_TO_SEC * delta_ms;
  size_t x_map, y_map;
  if (g->inputs.w)
  {
    x_map = (int)(g->player.position.x+proportion*g->player.direction.x);
    y_map = (int)(g->player.position.y);
    if (x_map >= 0 && x_map < g->map.cols &&
        y_map >= 0 && y_map < g->map.rows && 
        g->map.cells[y_map*g->map.cols+x_map] < 10
      )
    {
      g->player.position.x += proportion  * g->player.direction.x;
    }
    x_map = (int)(g->player.position.x);
    y_map = (int)(g->player.position.y+proportion*g->player.direction.y);
    if (x_map >= 0 && x_map < g->map.cols &&
        y_map >= 0 && y_map < g->map.rows && 
        g->map.cells[y_map*g->map.cols+x_map] < 10
      )
    {
      g->player.position.y +=  proportion * g->player.direction.y;
    }
  }
  if (g->inputs.s)
  {
    x_map = (int)(g->player.position.x-proportion*g->player.direction.x);
    y_map = (int)(g->player.position.y);
    if (x_map >= 0 && x_map < g->map.cols &&
        y_map >= 0 && y_map < g->map.rows && 
        g->map.cells[y_map*g->map.cols+x_map] < 10
      )
    {
      g->player.position.x -= proportion * g->player.direction.x;
    }

    x_map = (int)(g->player.position.x);
    y_map = (int)(g->player.position.y-proportion*g->player.direction.y);
    if (x_map >= 0 && x_map < g->map.cols &&
        y_map >= 0 && y_map < g->map.rows && 
        g->map.cells[y_map*g->map.cols+x_map] < 10
      )
    {
      g->player.position.y -= proportion * g->player.direction.y;
    }
  }

  proportion = TURN_ANGLE * delta_ms / MS_TO_SEC;
  if (g->inputs.d)
  {
    rotateV2f(&g->player.direction, proportion);
  }

  if (g->inputs.a)
  {
    rotateV2f(&g->player.direction, proportion * (-1));
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
    double delta_ms, acum;
    int frames = 0;
    acum = 0.0;
    gettimeofday(&last, NULL);
    g.player.position.x = 3.0;
    g.player.position.y = 3.0;
    g.player.direction.x = 0.0;
    g.player.direction.y = 1.0;
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
      delta_ms = (current.tv_usec - last.tv_usec) / MS_TO_SEC;
      gettimeofday(&last, NULL);
      if (delta_ms < 0)
      {
        delta_ms = 0;
      }
      acum += delta_ms;
      frames++;
      if (acum > MS_TO_SEC)
      {
        printf("FPS: %i in %f\n",frames, acum);
        acum = 0.0f;
        frames =  0;
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
