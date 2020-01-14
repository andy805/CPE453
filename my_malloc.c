/* Andy Velazquez
 * avelaz02@calpoly.edu
 * assignment 1
 */

#include <stdlib.h>
#include <stdio.h>
int main( int argc, char * argv[])
{
	void * calloc(size_t nmeb, size_t size)
	{
	   printf("using my library for calloc");	
      void * ptr;      	
      return ptr;
	}
	void * malloc(size_t size)
	{

	   printf("using my library for malloc");	
      void * ptr;      	
      return ptr;
	}
	void free(void *ptr)
	{
	   printf("using my library for free");	
	}
	void * realloc(void * ptr, size_t size)
	{

	   printf("using my library for realloc");
      return ptr;
	}
   return 0;
}
