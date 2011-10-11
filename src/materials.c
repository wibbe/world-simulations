
#include "game.h"
#include "util.h"

#include <GL/glfw.h>
#include <stdio.h>
#include <stdlib.h>

// -- Shaders -----------------------------------

static struct {
	GLuint vertex;
	GLuint fragment;
	GLuint program;
	
	struct {
		GLuint texture;
		GLuint uv_offset;
		GLuint repeat;
	} uniforms;
} _terrain_material;

// -- Util functions ----------------------------

static void show_info_log(GLuint object, 
								  void (*glGet__iv)(GLuint, GLenum, GLint*),
								  void (*glGet__InfoLog)(GLuint, GLsizei, GLsizei*, GLchar*))
{
	int log_length;
	char * log;
	
	glGet__iv(object, GL_INFO_LOG_LENGTH, &log_length);
	log = malloc(sizeof(char) * log_length);
	glGet__InfoLog(object, log_length, NULL, log);
	fprintf(stderr, "%s", log);
	free(log);
}

static GLuint make_shader(GLenum type, const char * filename)
{
	int length;
	char * source = file_content(filename, &length);
	
	GLuint shader;
	int shader_ok;
	
	if (!source)
		return 0;
		
	shader = glCreateShader(type);
	glShaderSource(shader, 1, (const GLchar **)&source, &length);
	free(source);
	glCompileShader(shader);
	
	glGetShaderiv(shader, GL_COMPILE_STATUS, &shader_ok);
	if (!shader_ok)
	{
		fprintf(stderr, "Failed to compile shader: %s\n", filename);
		show_info_log(shader, glGetShaderiv, glGetShaderInfoLog);
		glDeleteShader(shader);
		return 0;
	}
	
	return shader;
}

static GLuint make_program(GLuint vertex_shader, GLuint fragment_shader)
{
	int program_ok;
	
	if (vertex_shader == 0 || fragment_shader == 0)
		return 0;
	
	GLuint program = glCreateProgram();
	glAttachShader(program, vertex_shader);
	glAttachShader(program, fragment_shader);
	glLinkProgram(program);
	
	glGetProgramiv(program, GL_LINK_STATUS, &program_ok);
	if (!program_ok)
	{
		fprintf(stderr, "Failed to link shader program:\n");
		show_info_log(program, glGetProgramiv, glGetProgramInfoLog);
		glDeleteProgram(program);
		return 0;
	}
	
	return program;
}

// -- Create materials --------------------------

static void create_terrain_material()
{
	_terrain_material.vertex = make_shader(GL_VERTEX_SHADER, "../../shaders/terrain_vertex.glsl");
	_terrain_material.fragment = make_shader(GL_FRAGMENT_SHADER, "../../shaders/terrain_fragment.glsl");
	_terrain_material.program = make_program(_terrain_material.vertex, _terrain_material.fragment);
	_terrain_material.uniforms.texture = glGetUniformLocation(_terrain_material.program, "texture");
	_terrain_material.uniforms.uv_offset = glGetUniformLocation(_terrain_material.program, "uv_offset");
	_terrain_material.uniforms.repeat = glGetUniformLocation(_terrain_material.program, "repeat");
}

static void destroy_terrain_material()
{
	glDeleteProgram(_terrain_material.program);
	glDeleteShader(_terrain_material.vertex);
	glDeleteShader(_terrain_material.fragment);
}

// -- API ---------------------------------------

void materials_initialize()
{
	create_terrain_material();
}

void reload_materials()
{
	destroy_terrain_material();
	create_terrain_material();
}

void enable_terrain_material()
{
	float u, v, w, h;
	get_texture_uv(TEXTURE_DIRT, &u, &v, &w, &h);
	
	glUseProgram(_terrain_material.program);
	glUniform1i(_terrain_material.uniforms.texture, 0);
	glUniform2f(_terrain_material.uniforms.repeat, (float)world_width, (float)world_height);
	glUniform4f(_terrain_material.uniforms.uv_offset, u, v, w, h);
}

void disable_materials()
{
	glUseProgram(0);
}

