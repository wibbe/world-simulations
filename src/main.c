
#include <stdio.h>

#include "world.h"

int main(int argc, char * argv[])
{
	glfwInit();
	world_initialize(256, 256);
	
	printf("Hello, World!\n");
	
	
	world_free();
	glfwTerminate();
	
	
	return 0;
}