
#include <stdlib.h>
#include <GL/glfw.h>

#include "game.h"
#include "vec3.h"
#include "util.h"

#define WORLD_SCALE 4.0f

struct cell
{
	float rock;
	float sand;
	float water;
};

static int _width = 0;
static int _height = 0;
static int _size = 0;

static struct cell * _world = 0;
static float _max_height = 0.0f;

static float * _rock_level = 0;
static float * _water_level_0 = 0;
static float * _water_level_1 = 0;
static float * _water_level_new = 0;
static float * _water_level_old = 0;

static float * _water_vertex_array = 0;

static unsigned short * _terrain_indicies = 0;

static inline int _index(int x, int y)
{
	float h = _height - 1;
	float w = _width - 1;
	return CLAMP(y, 0, h) * _width + CLAMP(x, 0, w);
	//return y * _width + x;
}

static inline float _rock_h(int x, int y)
{
	//return _world[_index(x, y)].rock;
	return _rock_level[_index(x, y)];
}
/*
static inline float _water_h(int x, int y)
{
	int index = _index(x, y);
	return _world[_index(x, y)].rock + _world[index].water;
}
*/
static inline float _water_h(int idx)
{
	//return _world[idx].rock + _world[idx].water;
	//return _rock_level[idx] + _water_level[idx];
	
	return _rock_level[idx] + _water_vertex_array[(idx * 3) + 1];
}

void world_initialize(int width, int height)
{
	_width = width;
	_height = height;
	_size = width * height;
	
	//_world = malloc(sizeof(struct cell) * _size);
	_rock_level = malloc(sizeof(float) * _size);
	_water_level_0 = malloc(sizeof(float) * _size);
	_water_level_1 = malloc(sizeof(float) * _size);
	_water_vertex_array = malloc(sizeof(float) * _size * 3);
	
	_water_level_new = _water_level_0;
	_water_level_old = _water_level_1;
	
	int x, y;
	for (x = 0; x < _width; ++x)
		for (y = 0; y < _height; ++y)
		{
			int index = _index(x, y);
			
			_rock_level[index] = simplex_noise(2, x * 0.1f, y * 0.1f, 1.0f) * 6.0f;
			_water_level_0[index] = _water_level_1[index] = 0.0f;
			
			_water_vertex_array[(index * 3) + 0] = x * WORLD_SCALE;
			_water_vertex_array[(index * 3) + 1] = 0.0f;
			_water_vertex_array[(index * 3) + 2] = y * WORLD_SCALE;
			
			//_world[index].rock = simplex_noise(2, x * 0.1f, y * 0.1f, 1.0f) * 6.0f;
			//_world[index].water = 0.0f;
			//_world[index].sand = 0.0f;
			
			_max_height = MAX(_max_height, _rock_level[index]);
		}
		
	// Build quads
	for (x = 0; x < _width - 1; ++x)
		for (y = 0; y < _height - 1; ++y)
		{
			sbpush(_terrain_indicies, _index(x, y));
			sbpush(_terrain_indicies, _index(x, y + 1));
			sbpush(_terrain_indicies, _index(x + 1, y + 1));
			sbpush(_terrain_indicies, _index(x + 1, y));
		}
		
	//_world[_index(1, 1)].water = 1;
}

void world_free()
{
	free(_world);
	_world = 0;
}

void world_tick(float dt)
{
	#define FLOW 0.01f
	
	static int offset_x[9] = { -1, 0, 1, -1, 0, 1, -1, 0, 1 };
	static int offset_y[9] = { -1, -1, -1, 0, 0, 0, 1, 1, 1 };
	int x, y, i, step;
	int offsets[9];
	
	// Add some water
	if (glfwGetKey('Q') == GLFW_PRESS)
		_water_level_old[_index(6, 6)] += 10.0f * dt;
		
	if (glfwGetKey('E') == GLFW_PRESS)
	{
		_water_level_old[0] = 0.0f; //FLOW * 20.0f * dt;
		_water_level_old[1] = 0.0f; //FLOW * 20.0f * dt;
	}
	
	//void solve(float dt, float * base, float * in, float * out, int width, int height);
	
	solve(dt * 2.0f, _rock_level, _water_level_old, _water_level_new, _width, _height);
	
	for (i = 0; i < _size; ++i)
		_water_vertex_array[i * 3 + 1] = _rock_level[i] + _water_level_new[i];

	//printf("h = %3.1f\n", _water_level_new[_index(6, 6)]);

	{ // Swap water level buffers
		float * tmp = _water_level_old;
		_water_level_old = _water_level_new;
		_water_level_new = tmp;
	}
	

#if 0
	dt /= 4.0f;
	static int dir_x[4] = { 1, 0, -1, 0 };
	static int dir_y[4] = { 0, 1, 0, -1 };
	
	for (step = 0; step < 4; ++step)
	{
		for (x = 0; x < _width; ++x)
			for (y = 0; y < _height; ++y)
			{
				int idx1 = _index(x, y);
				int idx2 = _index(x + dir_x[step], y + dir_y[step]);
				
				float h1 = _water_h(idx1);
				float h2 = _water_h(idx2);			
				
				if (h1 > h2)
				{
					if (_water_level[idx1] > 0.0f)
					{
						float amount = MIN(FLOW, _water_level[idx1]) * dt;
						_water_level[idx1] -= amount;
						_water_level[idx2] += amount;
					}/*
					else
					{
						_water_level[idx1] = h2 - h1;
					}*/
				}
				else if (h2 > h1)
				{
					if (_water_level[idx2] > 0.0f)
					{
						float amount = MIN(FLOW, _water_level[idx2]) * dt;
						_water_level[idx1] += amount;
						_water_level[idx2] -= amount;
					}/*
					else
					{
						_water_level[idx2] = h1 - h2;
					}*/
				}
			}
	}
#endif
}

void world_draw()
{
	int x, y, i;
	
	//glPolygonMode(GL_FRONT, GL_LINE);
	
	// Rocks
	glBegin(GL_QUADS);
	
	for (x = 0; x < _width - 1; ++x)
		for (y = 0; y < _height - 1; ++y)
		{
			float h[4] = {
				_rock_h(x, y), _rock_h(x + 1, y),
				_rock_h(x, y + 1), _rock_h(x + 1, y + 1)
			};
			float c = h[0] / _max_height;
			
			glColor3f(c, 0, c);
			glVertex3f(x * WORLD_SCALE, h[0], y * WORLD_SCALE);
			glVertex3f(x * WORLD_SCALE, h[2], (y + 1) * WORLD_SCALE);
			glVertex3f((x + 1) * WORLD_SCALE, h[3], (y + 1) * WORLD_SCALE);
			glVertex3f((x + 1) * WORLD_SCALE, h[1], y * WORLD_SCALE);
		}

	glEnd();
	
	// Water
	enable_blend();
	glColor4f(0.2f, 0.4f, 0.8f, 0.4f);
	
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, _water_vertex_array);
	glDrawElements(GL_QUADS, sbcount(_terrain_indicies), GL_UNSIGNED_SHORT, _terrain_indicies);
	glDisableClientState(GL_VERTEX_ARRAY);
	
	/*
	glBegin(GL_QUADS);
	
	for (x = 0; x < _width - 1; ++x)
	{
		for (y = 0; y < _height - 1; ++y)
		{
			const int idx[4] = {
				_index(x, y), _index(x + 1, y), _index(x, y + 1), _index(x + 1, y + 1)
			};
			const float h[4] = {
				_water_h(idx[0]), _water_h(idx[1]),
				_water_h(idx[2]), _water_h(idx[3])
			};
			
			glColor4f(0.2f, 0.4f, 0.8f, 0.4f);
			glVertex3f(x * WORLD_SCALE, h[0], y * WORLD_SCALE);
			glVertex3f(x * WORLD_SCALE, h[2], (y + 1) * WORLD_SCALE);
			glVertex3f((x + 1) * WORLD_SCALE, h[3], (y + 1) * WORLD_SCALE);
			glVertex3f((x + 1) * WORLD_SCALE, h[1], y * WORLD_SCALE);
		}
	}
		
	glEnd();
	*/
	disable_blend();
}
