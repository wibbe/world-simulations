
#include <stdio.h>
#include <GL/glfw.h>
#include "game.h"

void init()
{
	world_initialize(256, 256);
	
	glClearColor(0, 0, 0, 0);
}

void destroy()
{
	world_free();
}

void loop()
{
	int running = 1;
	
	while (running)
	{
		glClear(GL_COLOR_BUFFER_BIT || GL_DEPTH_BUFFER_BIT);
		
		player_tick();
		
		glfwSwapBuffers();
	
		running = !glfwGetKey(GLFW_KEY_ESC) && glfwGetWindowParam(GLFW_OPENED);
	}
}

int main(int argc, char * argv[])
{
	glfwInit();
	
	if (glfwOpenWindow(800, 500, 8, 8, 8, 0, 24, 0, GLFW_WINDOW) == GL_TRUE)
	{
		init();		
		loop();
		destroy();
	}
	
	glfwTerminate();
	return 0;
}