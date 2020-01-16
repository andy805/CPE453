/* Andy Velazquez
 * avelaz02@calpoly.edu
 * assignment 1
 */

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "header_my_malloc.h"

static node *glob_ptr = NULL;

int find_size(size_t size)
{
	size = size + sizeof(node) ; 
	size = size + (16-(size % 16));
	return size;	
}

int num_breaks(size_t size)
{
	int actual_size = find_size(size);
	if(actual_size < 64000)
	{
		return 1;
	}
	
	return actual_size/64000;
}


void * calloc(size_t nmeb, size_t size)
{
	char str[50];
	snprintf(str, sizeof(char) * 40,"%s\n", "using my library for calloc");	
	write(1, str, 50);
	void * ptr;      	
	return ptr;
}

void * malloc(size_t size)
{
	/* writing to stdoutput just to check that my malloc is 
	is being called */
	char str[50];
	snprintf(str, sizeof(char) * 40, "%s\n", "using my library for malloc");	
	write(1, str, 30);
	
	if(size < 1)
	{
		char str2[50]; 
		snprintf(str2, sizeof(char) * 40, "%s\n", "error calling malloc with 0");    
		write(1, str2, 40);
		return;
	}

	int num = num_breaks(size); /* num now tells me how many
									times i need to call sbreak */
	int true_size = find_size(size);
	
	while(glob_ptr != NULL)
	{
		intptr_t pos = (intptr_t)glob_ptr;
		pos = pos + find_size(size);
		
		return;
	}
	/*	only true is first call to malloc
	 * so should only get to this part of the code when 
	 * first calling malloc */

	/* testing */
	char str3[50];
	snprintf(str3, sizeof(char) * 40, "%s\n", "first malloc call");	
	write(1, str3, 35);
	/*end of testing */

	void * ptr = sbrk(num * 64000);
	glob_ptr = (node *)ptr;
	glob_ptr->size = size;
	glob_ptr->alloc_mem_flag = 1;
	glob_ptr->next = NULL;
	glob_ptr->prev = NULL;
	return (void *)glob_ptr;
	
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
