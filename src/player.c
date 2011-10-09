
#include <GL/glfw.h>

#include "game.h"
#include "vec3.h"

static Vec3 _player_pos = zero();
static float _player_yaw = 0.0f;
static float _player_pitch = 0.0f;

static void _apply_camera()
{
	glTranslatef(-_player_pos.x, -_player_pos.y, -_player_pos.z);
}

void player_tick()
{
	_apply_camera();
}