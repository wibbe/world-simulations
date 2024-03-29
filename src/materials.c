
#include "game.h"
#include "util.h"
#include "vec3.h"

#include <stdio.h>
#include <stdlib.h>

// -- Shaders -----------------------------------

static struct {
	GLuint vertex;
	GLuint fragment;
	GLuint program;
	
	struct {
		GLuint texture;
		GLuint uv_offset_rock;
		GLuint uv_offset_sand;
		GLuint repeat;
		GLuint sun_dir;
	} uniforms;
} _terrain_material;

static struct {
	GLuint vertex;
	GLuint fragment;
	GLuint program;
	
	struct {
		GLuint texture;
    GLuint uv_offset_water;
		GLuint repeat;
		GLuint sun_dir;
    GLuint camera_pos;
	} uniforms;
} _water_material;

static struct {
	GLuint vertex;
	GLuint fragment;
	GLuint program;
	
	struct {
	    GLuint texture;
      GLuint uv_offset;
      GLuint sun_dir;
      GLuint repeat;
   } uniforms;
} _env_wall_material;

static struct {
	GLuint vertex;
	GLuint fragment;
	GLuint program;   
} _env_celling_material;

static Vec3 _sun_direction;
static float _wall_repeat = 1.0f;

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
	_terrain_material.uniforms.uv_offset_rock = glGetUniformLocation(_terrain_material.program, "uv_offset_rock");
	_terrain_material.uniforms.uv_offset_sand = glGetUniformLocation(_terrain_material.program, "uv_offset_sand");
	_terrain_material.uniforms.repeat = glGetUniformLocation(_terrain_material.program, "repeat");
	_terrain_material.uniforms.sun_dir = glGetUniformLocation(_terrain_material.program, "sun_dir");
}

static void destroy_terrain_material()
{
	glDeleteProgram(_terrain_material.program);
	glDeleteShader(_terrain_material.vertex);
	glDeleteShader(_terrain_material.fragment);
}

static void create_water_material()
{
   
}

static void destroy_water_material()
{
   
}

static void create_env_walls_material()
{
	_env_wall_material.vertex = make_shader(GL_VERTEX_SHADER, "../../shaders/wall_vertex.glsl");
	_env_wall_material.fragment = make_shader(GL_FRAGMENT_SHADER, "../../shaders/wall_fragment.glsl");
	_env_wall_material.program = make_program(_env_wall_material.vertex, _env_wall_material.fragment);
	_env_wall_material.uniforms.texture = glGetUniformLocation(_env_wall_material.program, "texture");
	_env_wall_material.uniforms.sun_dir = glGetUniformLocation(_env_wall_material.program, "sun_dir");
	_env_wall_material.uniforms.uv_offset = glGetUniformLocation(_env_wall_material.program, "uv_offset");
	_env_wall_material.uniforms.repeat = glGetUniformLocation(_env_wall_material.program, "repeat");
}

static void destroy_env_walls_material()
{
	glDeleteProgram(_env_wall_material.program);
	glDeleteShader(_env_wall_material.vertex);
	glDeleteShader(_env_wall_material.fragment);
}

// -- API ---------------------------------------

void materials_initialize(float radius)
{
	create_terrain_material();
  create_water_material();
  create_env_walls_material();

	_sun_direction = vnormalize(vec3(0.5f, 0.7f, 0.4f));
	_wall_repeat = (2.0f * PI * radius) / 2.0f;
}

void reload_materials()
{
	destroy_terrain_material();
  destroy_water_material();
  destroy_env_walls_material();
   
	create_terrain_material();
  create_water_material();
  create_env_walls_material();
}

void enable_terrain_material()
{
	float u, v, w, h;
	get_texture_uv(TEXTURE_DIRT, &u, &v, &w, &h);
	
	glUseProgram(_terrain_material.program);
	glUniform1i(_terrain_material.uniforms.texture, 0);
	glUniform2f(_terrain_material.uniforms.repeat, (float)world_width, (float)world_height);
  set_uv_offset_uniform(TEXTURE_STONE, _terrain_material.uniforms.uv_offset_rock);
  set_uv_offset_uniform(TEXTURE_SAND, _terrain_material.uniforms.uv_offset_sand);
	glUniform3f(_terrain_material.uniforms.sun_dir, _sun_direction.x, _sun_direction.y, _sun_direction.z);
}

void enable_env_walls_material()
{
	glUseProgram(_env_wall_material.program);
	glUniform1i(_env_wall_material.uniforms.texture, 0);
	glUniform2f(_env_wall_material.uniforms.repeat, _wall_repeat, 5.0f);
	glUniform3f(_env_wall_material.uniforms.sun_dir, _sun_direction.x, _sun_direction.y, _sun_direction.z);
  set_uv_offset_uniform(TEXTURE_STONE_SLAB, _env_wall_material.uniforms.uv_offset);
}

void disable_materials()
{
	glUseProgram(0);
}

