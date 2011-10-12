
#include <stdlib.h>

#include "game.h"
#include "vec3.h"
#include "util.h"


int world_width = 0;
int world_height = 0;
static int _size = 0;

static float _maxworld_height = 0.0f;

static float * _rock_level = 0;
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
	_water_level_0 = malloc(sizeof(float) * _size);
	_water_level_1 = malloc(sizeof(float) * _size);
	
	_water_level_new = _water_level_0;
	_water_level_old = _water_level_1;
	
	int x, y;
	for (x = 0; x < world_width; ++x)
		for (y = 0; y < world_height; ++y)
		{
			int index = _index(x, y);
			
			_rock_level[index] = (simplex_noise(1, x * 0.04f, y * 0.04f, 1.0f) * 10.0f) + 
			                     (simplex_noise(4, x * 0.1f, y * 0.1f, 2.0f) * 1.0f);
			_water_level_0[index] = _water_level_1[index] = 0.0f;
			
			_maxworld_height = MAX(_maxworld_height, _rock_level[index]);
		}
}

void world_free()
{
}

void world_tick(float dt)
{
	static int offset_x[9] = { -1, 0, 1, -1, 0, 1, -1, 0, 1 };
	static int offset_y[9] = { -1, -1, -1, 0, 0, 0, 1, 1, 1 };
	int x, y, i, step;
	int offsets[9];
	
	// Add some water
	if (glfwGetKey('Q') == GLFW_PRESS)
		_water_level_old[_index(23, 21)] += 15.0f * dt;
		
	if (glfwGetKey('E') == GLFW_PRESS)
	{
		_water_level_old[_index(23, 21)] = 0.0f;
	}
	
	solve_water_flow(dt * 8.0f, _rock_level, _water_level_old, _water_level_new, world_width, world_height);
	
	update_ground_heightmap(_rock_level, 0);
	update_water_heightmap(_rock_level, 0, _water_level_new);

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
