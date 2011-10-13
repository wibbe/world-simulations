
#include "game.h"
#include "util.h"
#include <stdlib.h>

#include "gfx/environment_walls.h"
#include "gfx/environment_celling.h"

static float _radius;

void environment_initialize(float radius)
{
   _radius = radius;
}

void render_environment_first()
{
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
   
   glDisable(GL_CULL_FACE);
   
   // Draw walls
   glPushMatrix();
   glScalef(_radius, 12.0f, _radius);
   
   enable_env_walls_material();
   
   glVertexPointer(3, GL_FLOAT, 0, environment_wallsVerts);
   glNormalPointer(GL_FLOAT, 0, environment_wallsNormals);
   glDrawArrays(GL_TRIANGLES, 0, environment_wallsNumVerts);
   
   disable_materials();
   
   glPopMatrix();
   glEnable(GL_CULL_FACE);
   
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
}

void render_environment_last()
{
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
   
   glDisable(GL_CULL_FACE);
   
   // Draw celling
   glPushMatrix();
   glTranslatef(0.0f, 12.0f, 0.0f);
   glScalef(_radius + 1.0f, _radius + 1.0f, _radius + 1.0f);
   
   glColor4f(0.3f, 0.6f, 1.0f, 0.6f);
   enable_blend();
   
   glVertexPointer(3, GL_FLOAT, 0, environment_cellingVerts);
   glNormalPointer(GL_FLOAT, 0, environment_cellingNormals);
   glDrawArrays(GL_TRIANGLES, 0, environment_cellingNumVerts);
   
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	
   glPopMatrix();
   disable_blend();
   glEnable(GL_CULL_FACE);
}

