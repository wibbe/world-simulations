
#include "game.h"
#include "util.h"
#include <stdlib.h>

static float * _border_pos_vertex_array = 0;
static unsigned short * _border_indicies = 0;

#define RESOLUTION 32

void environment_initialize(float radius)
{
  int i, count = RESOLUTION * 3;
  _border_pos_vertex_array = malloc(sizeof(float) * 3 * count);
  float * pos_it = _border_pos_vertex_array;

  float rad, step = (PI * 2.0f) / RESOLUTION;
  for (rad = 0.0f; rad < (PI * 2.0f); rad += step, pos_it += 9)
  {
    float x = sin(rad);
    float z = cos(rad);

    *(pos_it + 0) = x * radius;
    *(pos_it + 1) = 0.0f;
    *(pos_it + 2) = z * radius;

    *(pos_it + 3) = x * radius;
    *(pos_it + 4) = 10.0f;
    *(pos_it + 5) = z * radius;

    *(pos_it + 3) = x * (radius + 0.2f);
    *(pos_it + 4) = 10.0f;
    *(pos_it + 5) = z * (radius + 0.2f);
  }

  for (i = 0; i < RESOLUTION; ++i)
  {
    sbpush(_border_indicies, (i * 3));
    sbpush(_border_indicies, (i * 3) + 1);
  }
}

