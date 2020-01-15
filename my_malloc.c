/* Andy Velazquez
 * avelaz02@calpoly.edu
 * assignment 1
 */

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "header_my_malloc.h"

void * calloc(size_t nmeb, size_t size)
{
	char str[50];
	snprintf(str, sizeof(char) * 40,"%s\n", "using my library for malloc");	
	write(1, str, 50);
	void * ptr;      	
	return ptr;
}

void * malloc(size_t size)
{
	char str[50];

	snprintf(str, sizeof(char) * 40, "%s\n", "using my library for malloc");	
	write(1, str, 30);
	void * ptr;      	
	return ptr;
}

void free(void *ptr)
{
	char str[50];
	snprintf(str, sizeof(char) * 40, "%s\n", "using my library for free");	
	write(1, str, 30);
	if(ptr == NULL)
	{
		exit(0);
	}
}
void * realloc(void * ptr, size_t size)
{

	char str[50];
	snprintf(str, sizeof(char) * 40, "%s\n", "using my library for realloc");
	write(1, str, 30);
	return ptr;
}
