
#include "world.h"
#include <stdlib.h>

struct cell
{
	short rock;
	short sand;
	short water;
};

static int _width = 0;
static int _height = 0;
static int _size = 0;

static struct cell * _world = 0;

void world_initialize(int width, int height)
{
	_width = width;
	_height = height;
	_size = width * height;
	
	_world = malloc(sizeof(struct cell) * _size);
}

void world_free()
{
	free(_world);
	_world = 0;
}