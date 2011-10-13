
#include <stdio.h>
#include "game.h"

void init()
{
  glewInit();

	// Initialize GL
	glClearColor(0.6, 0.6, 0.6, 0);
	
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glShadeModel(GL_SMOOTH);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glActiveTexture(GL_TEXTURE0);
	glEnable(GL_TEXTURE_2D);
	
	// Initialize the rest of the app
	world_initialize(128, 128);
	heightmap_initialize();
	font_initialize();
	player_initialize();
	textures_initialize();
	materials_initialize();
	environment_initialize(32.0f * WORLD_SCALE);
}

void destroy()
{
	world_free();
}

void loop()
{
	int running = 1;
	int width, height;
	
	while (running)
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		glfwGetWindowSize(&width, &height);
		
		tick_frame();
		draw_frame(width, height);
		
		glfwSwapBuffers();
	
		running = !glfwGetKey(GLFW_KEY_ESC) && glfwGetWindowParam(GLFW_OPENED);
	}
}

int main(int argc, char * argv[])
{
	glfwInit();
	
	if (glfwOpenWindow(800, 500, 8, 8, 8, 8, 24, 0, GLFW_WINDOW) == GL_TRUE)
	{
		init();		
		loop();
		destroy();
	}
	
	glfwTerminate();
	return 0;
}
