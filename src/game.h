
#ifndef GAME_H
#define GAME_H

/* Enums */
enum FontTypes
{
	FONT_HELVETICA_17,
	FONT_MONACO_12,
	FONT_COUNT
};

/* Macros */
#define PI 3.1415f
#define DEG_TO_RAD(angle)	((angle) * (PI / 180.0f))
#define MAX(x, y) ((x) > (y) ? (x) : (y))
#define MIN(x, y) ((x) < (y) ? (x) : (y))
#define CLAMP(x, min, max) (MAX(min, MIN(max, x)))

/* World */
void world_initialize(int width, int height);
void world_free();
void world_tick(float dt);
void world_draw();

void player_initialize();

void solve(float dt, float * base, float * in, float * out, int width, int height);


void draw_frame();

/* Noise */
float noise(float xin, float yin, float zin);
float simplex_noise(int octaves, float x, float y, float z);

/* Font drawing */
void font_initialize();

int	font_get_height(int font);
int	font_get_width(int font, char * msg, ...);
void font_print(int font, int x, int y, char * msg, ...);


void enable_blend();
void disable_blend();

#endif