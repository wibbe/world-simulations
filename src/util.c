
#include "util.h"
#include <assert.h>
#include <stdio.h>

void stb__sbgrowf(void **arr, int increment, int itemsize)
{
   int m = *arr ? 2*stb__sbm(*arr)+increment : increment+1;
   void *p = realloc(*arr ? stb__sbraw(*arr) : 0, itemsize * m + sizeof(int)*2);
   assert(p);
   if (p) {
      if (!*arr) ((int *) p)[1] = 0;
      *arr = (void *) ((int *) p + 2);
      stb__sbm(*arr) = m;
   }
}

char * file_content(const char * filename, int * length)
{
	assert(filename && length);
	FILE * file = fopen(filename, "r");
	
	if (!file)
		return 0;
	
	fseek(file, 0, SEEK_END);
	*length = ftell(file);
	rewind(file);
	
	char * data = malloc(sizeof(char) * *length);
	fread(data, sizeof(char), *length, file);
	fclose(file);
	
	return data;
}
