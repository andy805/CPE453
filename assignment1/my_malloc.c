/* Andy Velazquez
 * avelaz02@calpoly.edu
 * assignment 1
 */

#include <stdio.h>
#include <unistd.h>
#include "header_my_malloc.h"
#include "helper.h"


static node *glob_ptr = NULL;

node * create_node(size_t size, void * ptr)
{
	if((size % 16) != 0)
	{
		char str[100];
		snprintf(str, sizeof(char) * 60,"%s\n", 
		"error creating node not divisible 16");	
		write(2, str, 60);
		return;
		
	}
	node *new = (node *)ptr;
	new->size = size;
	new->next = NULL;
	new->prev = NULL;
	new->alloc_mem_flag = 0;
	return new;
}

/*insert requires current node
 * assumes that inserted node
 * has enough space to fit */
node * insert(node * current, node * inserted_node)
{
	if(current->alloc_mem_flag == 1)
	{
		char str[50];
		snprintf(str, sizeof(char) * 40,"%s\n",
					 "error inserted node is not free");	
		write(2, str, 50);
		return;	
	}	

	if((inserted_node->size) > current->size)
	{
		
		char str[100];
		snprintf(str, sizeof(char) * 70,"%s\n",
		"error inserted node is bigger than the space available");	
		write(2, str, 60);
		return;
	}

	current->prev->next = inserted_node;
	inserted_node->prev = current->prev;
	current->prev = inserted_node;
	inserted_node->next = current;
	current->size = current->size - inserted_node->size;
	inserted_node->alloc_mem_flag = 1;
	return inserted_node;
	
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
	node *current_node = glob_ptr;
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
		while(current_node->next != NULL)
		{
			if(current_node->alloc_mem_flag == 0)
			{
				//have found some free space 
			}
			
		}



		/*only get here after you have reached the end of the list
		 */
		
		if(true_size > current_node->size)
		{
			/*node im trying to add will not fit in current block
			 *need to call sbreak*/
			void *ptr = sbrk(num * 64000);
			node *new = create_node(true_size, ptr);
			new->prev = current_node;
			current_node->next = new;
			new->alloc_mem_flag = 1;	
			return (void *) new;
		}
		else
		{
			/* node im trying to add will fit in current block
			 * i need to carve out some space */
			intptr_t curr_pos = (intptr_t)current_node;
			curr_pos = curr_pos + current_node->size;
			node *new = create_node(true_size, (void *)curr_pos);
			new->prev = current_node;
			current_node->next = new;
			new->alloc_mem_flag = 1;	
			return (void *) new;
		}
		
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
	void * dummy = sbrk(0);
	/*careful about sbrk come back to this if having issues*/
	intptr_t start= (16-((intptr_t)dummy % 16));
	void * ptr = sbrk((num * 64000));
   start = start + (intptr_t)ptr;
	glob_ptr = (node *)start;
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
