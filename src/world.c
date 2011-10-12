
#include <stdlib.h>

#include "game.h"
#include "vec3.h"
#include "util.h"


int world_width = 0;
int world_height = 0;
static int _size = 0;

static float _maxworld_height = 0.0f;

static float * _rock_level = 0;
static float * _sand_level = 0;
static float * _water_level_0 = 0;
static float * _water_level_1 = 0;
static float * _water_level_new = 0;
static float * _water_level_old = 0;

void world_initialize(int width, int height)
{
	world_width = width;
	world_height = height;
	_size = width * height;
	
	_rock_level = malloc(sizeof(float) * _size);
	_sand_level = malloc(sizeof(float) * _size);
	_water_level_0 = malloc(sizeof(float) * _size);
	_water_level_1 = malloc(sizeof(float) * _size);
	
	_water_level_new = _water_level_0;
	_water_level_old = _water_level_1;
	
	int x, y;
	for (x = 0; x < world_width; ++x)
		for (y = 0; y < world_height; ++y)
		{
			int index = _index(x, y);
			float dist = MIN(1.0f, vlength(vsub(vec3(x, 0, y), vec3(world_width / 2.0f, 0, world_height / 2.0f))) / (world_width / 2.0f));
			
			_rock_level[index] = (simplex_noise(1, x * 0.04f, y * 0.04f, 1.0f) * 5.0f * (1.0f + cos(dist * PI))) + 
			                     (simplex_noise(4, x * 0.1f, y * 0.1f, 2.0f) * 1.0f);
      _sand_level[index] = 0.0f;
			_water_level_0[index] = _water_level_1[index] = 0.0f;
			
			_maxworld_height = MAX(_maxworld_height, _rock_level[index]);
		}
}

void world_free()
{
}

void world_tick(float dt)
{
	int i;
	
  // River...
	_water_level_old[_index(world_width / 2, world_height / 2 - 10)] += 5.0f * dt;

  // Make sure we have water at the borders
	for (i = 0; i < world_width; ++i)
	{
	  int idx1 = _index(i, 0);
	  int idx2 = _index(i, world_height - 1);

	  _water_level_old[idx1] = MAX(WATER_LEVEL - _rock_level[idx1], 0.0f);
	  _water_level_old[idx2] = MAX(WATER_LEVEL - _rock_level[idx2], 0.0f);
  }

	for (i = 0; i < world_height; ++i)
	{
	  int idx1 = _index(0, i);
	  int idx2 = _index(world_height - 1, i);

	  _water_level_old[idx1] = MAX(WATER_LEVEL - _rock_level[idx1], 0.0f);
	  _water_level_old[idx2] = MAX(WATER_LEVEL - _rock_level[idx2], 0.0f);
  }
	
	// Step water simulation
	solve_water_flow(dt * 8.0f, _rock_level, _sand_level, _water_level_old, _water_level_new, world_width, world_height);
	
	// Update heightmaps
	update_ground_heightmap(_rock_level, _sand_level);
	update_water_heightmap(_rock_level, _sand_level, _water_level_new);

	{ // Swap water level buffers
		float * tmp = _water_level_old;
		_water_level_old = _water_level_new;
		_water_level_new = tmp;
	}
}

void world_draw()
{
	int x, y, i;
	
	render_heightmaps();
}

void world_add_sand(float dt)
{
  int x = floor(mouse_world_position.x / WORLD_SCALE);
  int y = floor(mouse_world_position.z / WORLD_SCALE);

  if (x >= 0 && x < world_width &&
      y >= 0 && y < world_height)
  {
    _sand_level[_index(x, y)] += 2.0f * dt;
    _sand_level[_index(x + 1, y)] += 2.0f * dt;
    _sand_level[_index(x, y + 1)] += 2.0f * dt;
    _sand_level[_index(x + 1, y + 1)] += 2.0f * dt;
  }
}

void world_remove_sand(float dt)
{
  int x = floor(mouse_world_position.x / WORLD_SCALE);
  int y = floor(mouse_world_position.z / WORLD_SCALE);

  if (x >= 0 && x < world_width &&
      y >= 0 && y < world_height)
  {
    _sand_level[_index(x, y)] = MAX(0.0f, _sand_level[_index(x, y)] - 2.0f * dt);
    _sand_level[_index(x + 1, y)] = MAX(0.0f, _sand_level[_index(x + 1, y)] - 2.0f * dt);
    _sand_level[_index(x, y + 1)] = MAX(0.0f, _sand_level[_index(x, y + 1)] - 2.0f * dt);
    _sand_level[_index(x + 1, y + 1)] = MAX(0.0f, _sand_level[_index(x + 1, y + 1)] - 2.0f * dt);
  }
}

