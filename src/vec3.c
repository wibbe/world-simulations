
#include "vec3.h"

Vec3 vec3(float x, float y, float z)
{
	Vec3 v = { x, y, z };
	return v;
}

Vec3 vzero()
{
	Vec3 v = { 0, 0, 0 };
	return v;	
}

Vec3 vadd(Vec3 a, Vec3 b)
{
	return vec3(a.x + b.x, a.y + b.y, a.z + b.z);
}

Vec3 vsub(Vec3 a, Vec3 b)
{
	return vec3(a.x - b.x, a.y - b.y, a.z - b.z);
}

float vdot(Vec3 v)
{
	return v.x * v.x + v.y * v.y + v.z * v.z;
}

float vlength(Vec3 v)
{
	return sqrt(vdot(v));
}

Vec3 vscale(Vec3 v, float s)
{
	return vec3(v.x * s, v.y * s, v.z * s);
}

Vec3 vnormalize(Vec3 v)
{
	float l = 1.0f / vlength(v);
	
	return vscale(v, l);
}

Vec3 vcross(Vec3 a, Vec3 b)
{    
  return vec3(a.y * b.z - a.z * b.y,
              a.z * b.x - a.x * b.z,
              a.x * b.y - a.y * b.x);
}

