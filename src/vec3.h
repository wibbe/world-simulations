
#ifndef VEC3_H
#define VEC3_H

#include <math.h>

typedef struct Vec3
{
	float x, y, z;
} Vec3;

Vec3 vec3(float x, float y, float z);
Vec3 vzero();

Vec3 vadd(Vec3 a, Vec3 b);

Vec3 vsub(Vec3 a, Vec3 b);

float vdot(Vec3 v);
float vlength(Vec3 v);
Vec3 vscale(Vec3 v, float s);
Vec3 vcross(Vec3 a, Vec3 b);

Vec3 vnormalize(Vec3 v);

#endif
