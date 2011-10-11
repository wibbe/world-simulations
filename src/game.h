
#ifndef GAME_H
#define GAME_H

#include <GL/glfw.h>

/* Enums */
enum FontTypes
{
	FONT_HELVETICA_17,
	FONT_MONACO_12,
	FONT_COUNT
};

/* Defines */
#define WORLD_SCALE 4.0f

/* Macros */
#define PI 3.1415f
#define DEG_TO_RAD(angle)	((angle) * (PI / 180.0f))
#define MAX(x, y) ((x) > (y) ? (x) : (y))
#define MIN(x, y) ((x) < (y) ? (x) : (y))
#define CLAMP(x, min, max) (MAX(min, MIN(max, x)))

/* Globals */
extern int world_width;
extern int world_height;

/* Textures */
enum Textures
{
	TEXTURE_DIRT,
	TEXTURE_STONE,
	TEXTURE_WATER
};

/* Helper functions */

static inline int _index(int x, int y)
{
	float h = world_height - 1;
	float w = world_width - 1;
	return CLAMP(y, 0, h) * world_width + CLAMP(x, 0, w);
}

/* World */
void world_free();
void world_tick(float dt);
void world_draw();

/* Initialization */
void world_initialize(int width, int height);
void player_initialize();
void heightmap_initialize();
void textures_initialize();
void font_initialize();
void materials_initialize();

void solve_water_flow(float dt, float * base, float * in, float * out, int width, int height);


void draw_frame();

void render_heightmaps();
void update_heightmaps(float * rock_height_data, float * sand_height_data, float * water_height_data);

/* Textures */
void enable_terrain_texture(GLenum unit);
void disable_texture(GLenum unit);
void get_texture_uv(int tex, float * u, float * v, float * width, float * height);

/* Materials */
void reload_materials();

void enable_terrain_material();
void disable_materials();

/* Noise */
float noise(float xin, float yin, float zin);
float simplex_noise(int octaves, float x, float y, float z);

/* Font drawing */
int	font_get_height(int font);
int	font_get_width(int font, char * msg, ...);
void font_print(int font, int x, int y, char * msg, ...);


void enable_blend();
void disable_blend();

#endif