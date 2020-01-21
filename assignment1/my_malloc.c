/* Andy Velazquez
 * avelaz02@calpoly.edu
 * assignment 1
 */

//#include <stdio.h>
#include <unistd.h>
#include "header_my_malloc.h"
#include "helper.h"
#include <string.h>

static node *glob_ptr = NULL;
static int counter = 0;

void print_node(node * node)
{
      char str[300];
      snprintf(str, sizeof(char) * 250,
	"%d\n%s%p\n%s%p\n%s%p\n%s%d\n%s%d\n\n",counter,
	"pointer previous -->", (void *)node->prev,
    "pointer current-->", (void *) node, "pointer next at-->",
      (uintptr_t)node->next, "alloc_mem_flag:",
		node->alloc_mem_flag, "size: ",
		node->size);
      fputs(str, stdout);
		
}
void print_list()
{
   
   if(glob_ptr == NULL)
   {  
      return;
   }
   node *current = glob_ptr;
   
   while(current != NULL)
   {
		print_node(current);
      current = current->next;
   }
	counter++;
}

int find_size(size_t size)                                                      
{      
                                                                         
   	size_t node_size = sizeof(node) + 
	(16-(sizeof(node) % 16));                                              
   size = size + node_size + node_size;
   return size;
}                                                                               
                                                                                
int num_breaks(size_t size)                                                     
{                                                                               
	uint32_t tmp = 0;
   int actual_size = find_size(size);                                           
	while(tmp < actual_size)
	{
		tmp+=64000;
	} 
	return tmp;
}  

node * create_node(size_t size, void * ptr)
{
	if((size % 16) != 0)
	{
		
		char str[100];
		snprintf(str, sizeof(char) * 60,"%s\n", 
		"update error creating node not divisible 16");	
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

		int cp_size = inserted_node->size;	
		inserted_node->prev = current;
		inserted_node->next = current->next;
		 
		inserted_node->alloc_mem_flag = 0;
		current->next = inserted_node;
		current->size = current->size - cp_size;
		current->alloc_mem_flag = 1; 
	
	return inserted_node;
	
}

void * calloc(size_t nmeb, size_t size)
{
	char str[200];
	if(nmeb == 0 || size == 0)
	{
		return NULL;
	}
	size_t tot_size = nmeb * size;
	void * ptr = malloc(tot_size);
	memset(ptr, 0, tot_size);
	if(getenv("DEBUG_MALLOC"))
	{
		snprintf(str, sizeof(char) * 100,
		"%s%d%s%d%s%8s%p%s%d%s\n", 
		"MALLOC: calloc(",nmeb,",",size,
		")", " =>  (ptr=",
		ptr,
		", size=", tot_size,")");	
		fputs(str, stderr);	
	}
	return ptr;
}

void * malloc(size_t size)
{
	char str[350];
	size_t copy_size;
	//print_list();	
   	int node_size = sizeof(node) + 
	(16-(sizeof(node) % 16));                                              
	if (size % 16)
	{
		size = size + (16-(size % 16));
	}
	
	node *current_node = glob_ptr;
	node *prev_node = NULL;
	if(size == 0)
	{
	/*	
		char str2[50]; 
		snprintf(str2, sizeof(char) * 40, "%s\n", 
		"error calling malloc with 0");    
		fputs(str2, stderr);
	*/	
		return NULL;
	}

	int true_size = find_size(size);
	  
	int num = num_breaks(size);
	
	while(current_node != NULL)
	{
		if((current_node->alloc_mem_flag == 0) &&
			(current_node->size >= (node_size + size)))
		{
			//have found some free space 
			if(current_node->size < (size + 2*node_size))
			{
	/*the free space big enough but not for header */
				current_node->alloc_mem_flag=1;

				if(getenv("DEBUG_MALLOC"))
				{
					snprintf(str, sizeof(char) * 60,
					"%s%d%s%8s%p%s%d%s\n", 
					"MALLOC: malloc(",copy_size,
					")", " =>  (ptr=",
					(uintptr_t)current_node+node_size,
					", size=", size,")");	
					fputs(str, stderr);	
				}
				return (void*) 
		((uintptr_t)current_node+node_size);
			}
			else
			{
				/*the free space found is big enough.	
				* time to carve out the  size the	
				* user is asking for*/
				uintptr_t curr_pos = (uintptr_t)current_node;
				//print_node(current_node);
				uintptr_t top_node = curr_pos + size +node_size;
				node *new = create_node(size, (void *)top_node);
				new->next = current_node->next;
				new->prev = current_node;
				new->size = 
				current_node->size - size - node_size;
				new->alloc_mem_flag = 0;
				current_node->next = new;
				current_node->size= size+node_size;
				current_node->alloc_mem_flag = 1;
				//new = insert_node(current_node, new);
				uintptr_t data_start = (uintptr_t)current_node
				 + node_size;
				if(getenv("DEBUG_MALLOC"))
				{
					snprintf(str, sizeof(char) * 100,
					"%s%d%s%8s%p%s%d%s\n", 
					"MALLOC: malloc(",copy_size,
					")", " =>  (ptr=",
					data_start,
					", size=", current_node->size,")");	
					fputs(str, stderr);	
				}
				return (void *) data_start;
				
			}
		}
		prev_node = current_node;
		current_node = current_node->next;
	}

	/*only get here after you have reached the end of the list
	 */
	void * top_pos;
	if(glob_ptr == NULL)
	{
		
		void * dummy = sbrk(0); /*getting address of the break*/

		/*careful about sbrk come back 
* 			to this if having issues*/
		uintptr_t div16= (16-((uintptr_t)dummy % 16));
		 /* what i need to
		add to first header to make sure 
		its address is divisible by 16*/
		sbrk(div16);
		
	}
	top_pos = sbrk(num);
	top_pos = (uintptr_t)top_pos;
	node * new_node1 = (node*)top_pos;
	new_node1->size = size+node_size;
	new_node1->prev = prev_node;
	new_node1->alloc_mem_flag = 1;
	uintptr_t top_pos2 = top_pos + size+node_size;
	node *new_node2 = (node*) top_pos2;
	new_node2->alloc_mem_flag = 0;
	new_node2->prev = new_node1;
	new_node2->next = NULL;
	new_node2->size = num - new_node1->size;
	new_node1->next = new_node2;
	
	if(glob_ptr == NULL)
	{
		glob_ptr = new_node1;
	}
	else
	{
		new_node1->prev->next = new_node1;
	}
	if(getenv("DEBUG_MALLOC"))
	{
		snprintf(str, sizeof(char) * 60,
		"%s%d%s%8s%p%s%d%s\n", 
		"MALLOC: malloc(",copy_size,
		")", " =>  (ptr=",
		(void *)((intptr_t)new_node1 + node_size),
		", size=", new_node1->size,")");	
		fputs(str, stderr);	
	}
	return (void *)((intptr_t)new_node1 + node_size);	
}

void free(void *ptr)
{
	char str[200];
   	uint32_t node_size = sizeof(node) + 
	(16-(sizeof(node) % 16));
	if(ptr == NULL)
	{
		return NULL;
	}
	node *current_node = glob_ptr;
	while(current_node)
	{
		uintptr_t low = (uintptr_t)current_node+node_size;
		uintptr_t upper = (uintptr_t)current_node
		+current_node->size;
		if((upper > (uintptr_t)ptr) && ((uintptr_t)ptr >= low))
		{
			/*i have found the pointer */
			if(current_node->next == NULL)
			{
				current_node->alloc_mem_flag = 0;

			if(getenv("DEBUG_MALLOC"))
			{
				snprintf(str, sizeof(char) * 100,
				"%s%p%s\n", 
				"MALLOC: free(",ptr,
				")"); 
				
				fputs(str, stderr);	
			}

			return;
			}
			if(current_node->next->alloc_mem_flag == 0)
			{
				/*merge blocks*/
				size_t tot_size = current_node->size 
				+current_node->next->size;

				if(current_node->next->next != NULL)
				{
					node * hold = current_node->next->next;
					hold->prev = current_node;
					current_node->next = hold;
				
					current_node->size = tot_size;
					current_node->alloc_mem_flag = 0;
						
				}
				else
				{
					current_node->next = NULL;
					current_node->size = tot_size;
					current_node->alloc_mem_flag = 0;
				}
			}
			if(current_node->prev == NULL)
			{
				current_node->alloc_mem_flag = 0;
				if(getenv("DEBUG_MALLOC"))
				{
					snprintf(str, sizeof(char) * 100,
					"%s%p%s\n", 
					"MALLOC: free(",ptr,
					")") ;
					
					fputs(str, stderr);	
				}
				return;
			}
			if(current_node->prev->alloc_mem_flag == 0)
			{
				/*merge blocks*/
				size_t tot_size = current_node->size +
				current_node->prev->size;
				
				if(current_node->next == NULL)
				{
					current_node->prev->size = tot_size;
					current_node->prev->next = NULL;
					current_node->prev->alloc_mem_flag = 0;
					current_node->alloc_mem_flag = 0;
					if(getenv("DEBUG_MALLOC"))
					{
						snprintf(str,
                     sizeof(char) * 100,
						"%s%p%s\n", 
						"MALLOC: free(",ptr,
						")") ;
						
						fputs(str, stderr);	
					}
					return;
				}
				else
				{
				
					current_node->prev->size = tot_size;
					current_node->prev->next
					 = current_node->next;
					current_node->next->prev
					 = current_node->prev;
					
					current_node->alloc_mem_flag = 0;
					current_node->prev->alloc_mem_flag = 0;
					if(getenv("DEBUG_MALLOC"))
					{
						snprintf(str,
                         sizeof(char) * 100,
						"%s%p%s\n", 
						"MALLOC: free(",ptr,
						")") ;
						
						fputs(str, stderr);	
					}
					return;
				}
				
				
			}
			current_node->alloc_mem_flag = 0;
			if(getenv("DEBUG_MALLOC"))
			{
				snprintf(str, sizeof(char) * 100,
				"%s%p%s\n", 
				"MALLOC: free(",ptr,
				")") ;
				
				fputs(str, stderr);	
			}
			return;
			
		//low++;
		}
		current_node = current_node->next;
	}
	/*this far means that im at the end
 * of the list so ptr does not exit*/
	return NULL;	

}
void * realloc(void * ptr, size_t size)
{
	char str[200];
	size_t copy_size =size;
	if(ptr == NULL)
	{
		return malloc(size);
	}
	if((size == 0) && (ptr !=NULL))
	{
		free(ptr);
	}
	if(size % 16)
	{
		size = size + (16 -(size % 16));
	}
   	uint32_t node_size = sizeof(node) + 
	(16-(sizeof(node) % 16));
	node *current_node = glob_ptr;
	node *prev_node = NULL;
	uintptr_t input = (uintptr_t)ptr;
	while(current_node)
	{
		uintptr_t start = (uintptr_t)current_node + node_size;
		uintptr_t end = (uintptr_t)current_node + current_node->size;
		if(input >= start && input < end)
		{
			if(current_node->size > (size + node_size))
			{
				if(getenv("DEBUG_MALLOC"))
				{
					snprintf(str, sizeof(char) * 60,
					"%s%p%s%d%s%8s%p%s%d%s\n", 
					"MALLOC: realloc(",ptr,",",copy_size,
					")", " =>  (ptr=",
					start,
					", size=", current_node->size,")");	
					fputs(str, stderr);	
				}
				return start;
			}
			else
			{
				if(current_node->next && (current_node->size 
					+ current_node->next->size 
				>= node_size + size))
				{
					if(current_node->next->next)
					{
						current_node->next->next->prev
							 = current_node;
					}
					current_node->size 
					+= current_node->next->size;
					current_node->next 
					= current_node->next->next;	
					if(getenv("DEBUG_MALLOC"))
					{
						snprintf(str, sizeof(char) * 60,
						"%s%p%s%d%s%8s%p%s%d%s\n", 
						"MALLOC: realloc(",
                        ptr,",",copy_size,
						")", " =>  (ptr=",
						start,", size=",
                         current_node->size,")");	
						fputs(str, stderr);	
					}
					return (void *)start;
							
				}
				else
				{
					void *new = malloc(size);
					memcpy(new, start, 
					current_node->size - node_size);
					if(getenv("DEBUG_MALLOC"))
					{
						snprintf(str, sizeof(char) * 60,
						"%s%p%s%d%s%8s%p%s%d%s\n", 
						"MALLOC: realloc(",
                        ptr,",",copy_size,
						")", " =>  (ptr=",
						new,
						", size=", 
                        current_node->size,")");	
						fputs(str, stderr);	
					}
					free(start);
					return new;
				}
			}
		}
	prev_node = current_node;
	current_node = current_node->next;
	}
}
