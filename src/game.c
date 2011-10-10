
#include "game.h"

#include "GL/glfw.h"


void enable_blend()
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void disable_blend()
{
	glDisable(GL_BLEND);
}