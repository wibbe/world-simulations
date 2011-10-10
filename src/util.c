
#include "util.h"
#include <assert.h>

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