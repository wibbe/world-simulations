
#ifndef VEC3_H
#define VEC3_H

#include <math.h>

typedef struct Vec3
{
	float x, y, z;
} Vec3;

static inline Vec3 vec3(float x, float y, float z)
{
	Vec3 v = { x, y, z };
	return v;
}

#define zero() { 0.0f, 0.0f, 0.0f }

static inline Vec3 add(Vec3 a, Vec3 b)
{
	return vec3(a.x + b.x, a.y + b.y, a.z + b.z);
}

static inline Vec3 sub(Vec3 a, Vec3 b)
{
	return vec3(a.x - b.x, a.y - b.y, a.z - b.z);
}

static inline float dot(Vec3 v)
{
	return v.x * v.x + v.y * v.y + v.z * v.z;
}

static inline float length(Vec3 v)
{
	sqrt(dot(v));
}

static inline Vec3 scale(Vec3 v, float s)
{
	return vec3(v.x * s, v.y * s, v.z * s);
}

#endif