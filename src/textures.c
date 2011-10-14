
#include <stdio.h>

#include "game.h"

#define STBI_HEADER_FILE_ONLY
#include "stb_image.c"


static unsigned int _terrain_texture;


static int load_texture(const char * filename)
{
	int width, height, components;
	
	unsigned char * data = stbi_load(filename, &width, &height, &components, 4);
	
	if (data == NULL)
		return -1;
	
	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	glBindTexture(GL_TEXTURE_2D, 0);
	
	stbi_image_free(data);
	
	return texture;
}

void textures_initialize()
{
	const char * terrain_files[3] = {
		"gfx/terrain.png",
		"../gfx/terrain.png",
		"../../gfx/terrain.png"
	};
	int i;
	
	// Load terrain texture
	for (i = 0; i < 3; ++i)
		if ((_terrain_texture = load_texture(terrain_files[i])) != -1)
			break;
			
	if (_terrain_texture == -1)
		fprintf(stderr, "Could not load texture \"terrain.png\"\n");
}

void enable_terrain_texture(GLenum unit)
{
	glActiveTexture(unit);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, _terrain_texture);
	glActiveTexture(GL_TEXTURE0);
}

void disable_texture(GLenum unit)
{
	glActiveTexture(unit);
	glBindTexture(GL_TEXTURE_2D, 0);
	glActiveTexture(GL_TEXTURE0);
}

void get_texture_uv(int tex, float * u, float * v, float * width, float * height)
{
	*width = *height = 16.0f / 256.0f;
	
	switch (tex)
	{
		case TEXTURE_DIRT:
			*u = 2.0f / 16.0f;
			*v = 0.0f; //15.0f / 16.0f;
			break;
		case TEXTURE_STONE:
			*u = 0.0f;
			*v = 0.0f; //15.0f / 16.0f;
			break;
		case TEXTURE_SAND:
			*u = 2.0f / 16.0f;
			*v = 1.0f / 16.0f;
		  break;
		case TEXTURE_WATER:
			*u = 13.0f / 16.0f;
			*v = 12.0f / 16.0f;
			break;
		case TEXTURE_STONE_SLAB:
		  *u = 6.0f / 16.0f;
		  *v = 3.0f / 16.0f;
		  break;
	}
}

void set_uv_offset_uniform(int tex, GLuint uniform)
{
	float u, v, w, h;
	get_texture_uv(tex, &u, &v, &w, &h);
	glUniform4f(uniform, u, v, w, h);
}

