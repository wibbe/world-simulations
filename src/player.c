
#include <GL/glfw.h>

#include "game.h"
#include "vec3.h"

static Vec3 _player_pos = { 0, 0, 0 };
static float _player_yaw = 0.0f;
static float _player_pitch = 0.0f;
static double _time_stamp = -1.0f;
static int _mouse_x = 0, _mouse_y = 0;

static void _apply_camera()
{
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	glRotatef(_player_pitch, 1, 0, 0);
	glRotatef(_player_yaw, 0, 1, 0);
	glTranslatef(-_player_pos.x, -_player_pos.y, -_player_pos.z);
}

static void _apply_projection(int width, int height)
{
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	
	gluPerspective(60, (float)width / (float)height, 1, 1000);
}

static void _apply_gui(int width, int height)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, width, height, 0);
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();	
}

static void _camera_control(float dt)
{
	#define PLAYER_SPEED 10.0f
	#define PLAYER_SENSITIVITY 0.4f
	
	Vec3 movement = vzero();
	
	// Move forward/backwards
	if (glfwGetKey('W') == GLFW_PRESS)
		movement.z = -1;
	else if (glfwGetKey('S') == GLFW_PRESS)
		movement.z = 1;
		
	// Straifing
	if (glfwGetKey('A') == GLFW_PRESS)
		movement.x = -1;
	else if (glfwGetKey('D') == GLFW_PRESS)
		movement.x = 1;
	
	// Flying
	if (glfwGetKey('R') == GLFW_PRESS)
		movement.y = 1;
	else if (glfwGetKey('F') == GLFW_PRESS)
		movement.y = -1;
	
	if (vlength(movement) > 0.0f)
	{
		movement = vscale(vnormalize(movement), PLAYER_SPEED * dt);

    _player_pos.x += (float)cos(DEG_TO_RAD(_player_yaw + 90)) * movement.z +
    							   (float)cos(DEG_TO_RAD(_player_yaw)) * movement.x;
    _player_pos.z += (float)sin(DEG_TO_RAD(_player_yaw + 90)) * movement.z +
    							   (float)sin(DEG_TO_RAD(_player_yaw)) * movement.x;
    _player_pos.y += movement.y;
	}
	
	// Orientation
	int x, y;
	glfwGetMousePos(&x, &y);
	
	int diff_x = x - _mouse_x;
	int diff_y = y - _mouse_y;
	_mouse_x = x;
	_mouse_y = y;
	
	if (glfwGetMouseButton(GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
	{
		_player_yaw += diff_x * PLAYER_SENSITIVITY;
		_player_pitch += diff_y * PLAYER_SENSITIVITY;
		
		if (_player_yaw < 0.0f)
			_player_yaw += 360.0f;
		if (_player_yaw > 360.0f)
			_player_yaw -= 360.0f;
			
		_player_pitch = _player_pitch > 85.0f ? 85.0f : _player_pitch < -85.0f ? -85.0f : _player_pitch;
	}
}

static void draw_world()
{
	world_draw();
}

static void draw_gui(int width, int height)
{
	enable_blend();
	
	glColor3f(1, 1, 1);
	font_print(FONT_MONACO_12, 10, 10, "Player Pos: (%3.1f, %3.1f, %3.1f)", _player_pos.x, _player_pos.y, _player_pos.z);
	
	disable_blend();
}

void player_initialize()
{
	_time_stamp = glfwGetTime();
	
	glfwGetMousePos(&_mouse_x, &_mouse_y);
}

void tick_frame()
{
	double time_stamp = glfwGetTime();
	double dt = time_stamp - _time_stamp;
	_time_stamp = time_stamp;
	
	_camera_control(dt);
	world_tick(dt);
}


void draw_frame(int width, int height)
{
	_apply_projection(width, height);	
	_apply_camera();
	
	draw_world();
	
	_apply_gui(width, height);
	draw_gui(width, height);
}