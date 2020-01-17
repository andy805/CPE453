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
		fputs(str, stderr);
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
node * insert_node(node * current, node * inserted_node)
{
	if(current->alloc_mem_flag == 1)
	{
		char str[50];
		snprintf(str, sizeof(char) * 40,"%s\n",
					 "error inserted node is not free");	
		fputs(str, stderr);
		return;	
	}	

	if((inserted_node->size) > current->size)
	{
		
		char str[100];
		snprintf(str, sizeof(char) * 70,"%s\n",
		"error inserted node is bigger than the space available");	
		fputs(str, stderr);
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
	fputs(str, stderr);
	void * ptr;      	
	return ptr;
}

void * malloc(size_t size)
{
	char str[50];
	snprintf(str, sizeof(char) * 40, "%s\n", "using my library for malloc");	
	fputs( str, stderr);
	node *current_node = glob_ptr;
	/* writing to stdoutput just to check that my malloc is 
	is being called */
	
	if(size < 1)
	{
		char str2[50]; 
		snprintf(str2, sizeof(char) * 40, "%s\n", "error calling malloc with 0");    
		fputs(str2, stderr);
		return;
	}

	int num = num_breaks(size); /* num now tells me how many
									times i need to call sbreak */
	int true_size = find_size(size);

	char str4[50]; 
	snprintf(str4, sizeof(char) * 40, "%s\n", "before outer while loop");    
	fputs(str4, stderr);
	
	while(glob_ptr != NULL)
	{
		char str5[50]; 
		snprintf(str5, sizeof(char) * 40, "%s\n", "before inner while loop");    
		fputs(str5, stderr);

		while(current_node->next != NULL)
		{
			char str6[50]; 
			snprintf(str6, sizeof(char) * 40, "%s\n", "inside inner while loop");    
			fputs(str6, stderr);

			if(current_node->alloc_mem_flag == 0)
			{
				char str7[50]; 
				snprintf(str7, sizeof(char) * 40, "%s\n", "have found some free space");    
				fputs(str7, stderr);

				//have found some free space 
				if(current_node->size < true_size)
				{
					/*the free space found is not big enough to allocate to */
					char str7[50]; 
					snprintf(str7, sizeof(char) * 40, "%s\n", 
					"have found some free space not big enough");    
					fputs(str7, stderr);
					
				}
				else
				{
					/*the free space found is big enough. time to carve out the  size the user is asking for*/

					char str2[50]; 
					snprintf(str2, sizeof(char) * 40, "%s\n", "free space found and suff");    
					fputs(str2, stderr);


					intptr_t curr_pos = (intptr_t)current_node;
					curr_pos = curr_pos + current_node->size;
					node *new = create_node(true_size, (void *)curr_pos);
					new = insert_node(current_node, new);
					intptr_t data_start = (intptr_t)new + sizeof(node);
					return (void *) data_start;
					
				}
			}
			current_node = current_node->next;
		}

		/*only get here after you have reached the end of the list
		 */
		void *top_pos = sbrk(0);
		top_pos = (intptr_t)top_pos;
		if(true_size > ((int)top_pos - current_node->size))
		{
			/*node im trying to add will not fit in current block
			 *need to call sbreak*/
			void *ptr = sbrk(num * 64000);
			current_node->size = true_size;
			current_node->alloc_mem_flag = 1;
			intptr_t last_header_pos = (intptr_t)current_node + current_node->size;
			node *last = (node*)last_header_pos;
			last->next = NULL;
			last->prev = current_node;
			last->size = (int)(((intptr_t) sbrk(0)) -
last_header_pos);
			last->alloc_mem_flag = 0;
		/*	
			node *new = create_node(true_size, ptr);
			new->prev = current_node;
			current_node->next = new;
			new->alloc_mem_flag = 1;	*/
			intptr_t data_start = (intptr_t)current_node + sizeof(node);
			return (void *) data_start;
		}
		else
		{
			/* node im trying to add will fit in current block
			 * i need to carve out some space */
			char str2[50]; 
			snprintf(str2, sizeof(char) * 40, "%s\n", "end of list carving");    
			fputs( str2, stderr);
			

			intptr_t curr_pos = (intptr_t)current_node;
			curr_pos = curr_pos + current_node->size;
			node *new = create_node(true_size, (void *)curr_pos);
			new->prev = current_node;
			current_node->next = new;
			new->alloc_mem_flag = 1;	
			intptr_t data_start = (intptr_t)new + sizeof(node);
			return (void *) data_start;
		}
		
		return;
	}
	/*	only true is first call to malloc
	 * so should only get to this part of the code when 
	 * first calling malloc */

	/* testing */
	char str3[50];
	snprintf(str3, sizeof(char) * 40, "%s\n", "first malloc call ever");	
	fputs(str3, stderr);
	/*end of testing */

	void * dummy = sbrk(0); /*getting address of the break*/

	/*careful about sbrk come back to this if having issues*/
	intptr_t div16= (16-((intptr_t)dummy % 16)); /* what i need to
add to first header to make sure its address is divisible by 16*/

	void * ptr = sbrk((num * 64000)); /*getting my memory*/

   intptr_t start = div16 + (intptr_t)ptr; /*start now points to an address that is divisible my 16 */

	glob_ptr = (node *)start; /*glob_ptr is pointing at the beginning of the	*the header not data */

	/*true size = sizeof(node) + size requested*/
	glob_ptr->size = true_size;


	node * dummy_header = (intptr_t)(((intptr_t)glob_ptr+glob_ptr->size));/* this is true_size + a little more from the potential of the first sbrk call not being on an address divisible by 16 */
	
	dummy_header->prev = glob_ptr;
	dummy_header->next = NULL;
	dummy_header->alloc_mem_flag = 0;
	intptr_t top_break = (intptr_t)sbrk(0);
	
	dummy_header->size = (int)(top_break - (((intptr_t) glob_ptr) +
glob_ptr->size));

	glob_ptr->alloc_mem_flag = 1;
	glob_ptr->next = dummy_header;
	glob_ptr->prev = NULL;

	char str8[50];
	snprintf(str8, sizeof(char) * 40, "%s\n", "made it here 1");	
	fputs(str8, stderr);

	start = (intptr_t)glob_ptr + sizeof(node); /*start is pointing at data*/
	return (void *)(start);
	
}

void free(void *ptr)
{
	char str[50];
	snprintf(str, sizeof(char) * 40, "%s\n", "using my library for free");	
	fputs(str, stderr);

	if(ptr == NULL)
	{
		exit(0);
	}
}
void * realloc(void * ptr, size_t size)
{

	char str[50];
	snprintf(str, sizeof(char) * 40, "%s\n", "using my library for realloc");
	fputs(str, stderr);
	return ptr;
}
