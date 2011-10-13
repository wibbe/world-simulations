
#include "game.h"
#include "util.h"
#include "vec3.h"

static float * _water_pos_vertex_array = 0;
static float * _ground_pos_vertex_array = 0;
static float * _ground_normal_vertex_array = 0;
static float * _ground_uv_vertex_array = 0;

static unsigned short * _terrain_indicies = 0;

void heightmap_initialize()
{
	int size = world_width * world_height;
	_water_pos_vertex_array = malloc(sizeof(float) * size * 3);
	_ground_pos_vertex_array = malloc(sizeof(float) * size * 3);
	_ground_normal_vertex_array = malloc(sizeof(float) * size * 3);
	_ground_uv_vertex_array = malloc(sizeof(float) * size * 4);
	
	int x, y;
	for (x = 0; x < world_width; ++x)
		for (y = 0; y < world_height; ++y)
		{
			int index = _index(x, y);
			
			_water_pos_vertex_array[(index * 3) + 0] = x * WORLD_SCALE;
			_water_pos_vertex_array[(index * 3) + 1] = 0.0f;
			_water_pos_vertex_array[(index * 3) + 2] = y * WORLD_SCALE;

			_ground_pos_vertex_array[(index * 3) + 0] = x * WORLD_SCALE;
			_ground_pos_vertex_array[(index * 3) + 1] = 0.0f;
			_ground_pos_vertex_array[(index * 3) + 2] = y * WORLD_SCALE;

			_ground_normal_vertex_array[(index * 3) + 0] = 0.0f;
			_ground_normal_vertex_array[(index * 3) + 1] = 0.0f;
			_ground_normal_vertex_array[(index * 3) + 2] = 0.0f;
			
			_ground_uv_vertex_array[(index * 4) + 0] = x / (world_width - 1.0f);
			_ground_uv_vertex_array[(index * 4) + 1] = y / (world_height - 1.0f);
			_ground_uv_vertex_array[(index * 4) + 2] = 0.0f;
			_ground_uv_vertex_array[(index * 4) + 3] = 0.0f;
			
			if (x < (world_width - 1) && y < (world_height - 1))
			{
				sbpush(_terrain_indicies, _index(x, y));
				sbpush(_terrain_indicies, _index(x, y + 1));
				sbpush(_terrain_indicies, _index(x + 1, y + 1));
				sbpush(_terrain_indicies, _index(x + 1, y));
			}
		}
}

void update_ground_heightmap(float * rock_height_data, float * sand_height_data)
{
	const int offset[4] = { -world_width, -1, world_width, 1 };
	const float dir[4][2] = {{ 0.0f, -WORLD_SCALE }, { -WORLD_SCALE, 0.0f }, { 0.0f, WORLD_SCALE }, { WORLD_SCALE, 0.0f }};
	float * rock_it = rock_height_data;
	float * sand_it = sand_height_data;
	float * rock_end = rock_height_data + (world_width * world_height);
	float * ground_vertex_it = _ground_pos_vertex_array + 1;
	float * ground_uv_it = _ground_uv_vertex_array + 2;

  // Fill in heights
	for (; rock_it != rock_end; rock_it++, sand_it++, ground_vertex_it += 3, ground_uv_it += 4)
  {
		*ground_vertex_it = *rock_it + *sand_it;
		*ground_uv_it = *sand_it > 0.0f ? 1.0f : 0.0f;
	}

  // Calculate normals
	ground_vertex_it = _ground_pos_vertex_array;
	float * ground_vertex_end = _ground_pos_vertex_array + (world_width * world_height * 3);
	float * ground_normal_it = _ground_normal_vertex_array;

  for (; ground_vertex_it != ground_vertex_end; ground_vertex_it += 3, ground_normal_it += 3)
  {
	  Vec3 normal = vec3(0, 1, 0);
    Vec3 v0 = vec3(0.0f, *(ground_vertex_it + 1), 0.0f);

		int i;
		for (i = 0; i < 4; ++i)
    {
      float * offset_1 = ground_vertex_it + (offset[i] * 3);
      float * offset_2 = ground_vertex_it + (offset[(i + 1) %4] * 3);

      if (offset_1 >= _ground_pos_vertex_array &&
          offset_2 >= _ground_pos_vertex_array&&
          offset_1 < ground_vertex_end &&
          offset_2 < ground_vertex_end)
      {
        Vec3 v1 = vec3(dir[i][0], *(offset_1 + 1), dir[i][1]);
        Vec3 v2 = vec3(dir[(i + 1) % 4][0], *(offset_2 + 1), dir[(i + 1) %4][1]);

        normal = vadd(normal, vcross(vsub(v1, v0), vsub(v2, v0)));
      }
    }

    *(ground_normal_it + 0) = normal.x;
    *(ground_normal_it + 1) = normal.y;
    *(ground_normal_it + 2) = normal.z;
  }
}

void update_water_heightmap(float * rock_height_data, float * sand_height_data, float * water_height_data)
{
	const int offset[8] = { -world_width - 1, -world_width, -world_width + 1, -1, 1, world_width - 1, world_width, world_width + 1 };
	int i;
	float * water_it = water_height_data;
	float * rock_it = rock_height_data;
	float * sand_it = sand_height_data;
	float * water_end = water_height_data + (world_width * world_height);
	float * water_vertex_it = _water_pos_vertex_array + 1;
	float * ground_uv_it = _ground_uv_vertex_array + 3;
	
	for (;water_it != water_end; water_it++, rock_it++, sand_it++, water_vertex_it += 3, ground_uv_it += 4)
	{
	  *ground_uv_it = *water_it;

		if (*water_it < 0.00001f)
		{
			float sum = 0.0; //*rock_it + *sand_it;
			int count = 0; //1;
			for (i = 0; i < 8; ++i)
			{
				float * offset_it = water_it + offset[i];
				if (offset_it >= water_height_data && offset_it < water_end && (*offset_it > 0.0f))
				{
					sum += *(rock_it + offset[i]) + *(sand_it + offset[i]) + *offset_it;
					count++;
				}
			}
			
			sum /= count;
			
			*water_vertex_it = sum;
		}
		else
		{
			*water_vertex_it = *rock_it + *sand_it + *water_it;
		}
	}
}

static void render_debug_normals()
{
  glBegin(GL_LINES);
  glColor4f(1, 0, 0, 1);

  int i;
	for (i = 0; i < (world_width * world_height); ++i)
	{
    glVertex3f(_ground_pos_vertex_array[i * 3 + 0],
               _ground_pos_vertex_array[i * 3 + 1],
               _ground_pos_vertex_array[i * 3 + 2]);
    glVertex3f(_ground_pos_vertex_array[i * 3 + 0] + _ground_normal_vertex_array[i * 3 + 0],
               _ground_pos_vertex_array[i * 3 + 1] + _ground_normal_vertex_array[i * 3 + 1],
               _ground_pos_vertex_array[i * 3 + 2] + _ground_normal_vertex_array[i * 3 + 2]);
  }

  glEnd();
}

void render_heightmaps()
{
   glPushMatrix();
   glTranslatef(world_width * WORLD_SCALE * -0.5f, 0.0f, world_height * WORLD_SCALE * -0.5f);
   
	enable_terrain_texture(GL_TEXTURE0);
	enable_terrain_material();
	
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	
	// Ground
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glVertexPointer(3, GL_FLOAT, 0, _ground_pos_vertex_array);
	glTexCoordPointer(4, GL_FLOAT, 0, _ground_uv_vertex_array);
	glNormalPointer(GL_FLOAT, 0, _ground_normal_vertex_array);
	glDrawElements(GL_QUADS, sbcount(_terrain_indicies), GL_UNSIGNED_SHORT, _terrain_indicies);
	
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	
	disable_materials();
	disable_texture(GL_TEXTURE0);

	//render_debug_normals();

	// Water
	enable_blend();
	glColor4f(0.2f, 0.4f, 0.8f, 0.4f);
	
	glVertexPointer(3, GL_FLOAT, 0, _water_pos_vertex_array);
	glDrawElements(GL_QUADS, sbcount(_terrain_indicies), GL_UNSIGNED_SHORT, _terrain_indicies);
	disable_blend();
		
	glDisableClientState(GL_VERTEX_ARRAY);
	
   glPopMatrix();
}

float heightmap_x_pos(float world_x)
{
   return world_x + (world_width * WORLD_SCALE * 0.5f);
}

float heightmap_z_pos(float world_z)
{
   return world_z + (world_height * WORLD_SCALE * 0.5f);
}