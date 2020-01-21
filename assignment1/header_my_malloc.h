/* Andy Velazquez
 * avelaz02@calpoly.edu	
 * assignment 1
 * My library malloc
 */

#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <stdint.h>

typedef struct node
{
	struct node *next;
	struct node *prev;
	int alloc_mem_flag;
   size_t size; /*size_t is unsigned integer type representing 
                 * number of bytes*/

         
	/* where is the buffer
	 * size?
	 * how to find the next one
	 */
}node;

void *calloc(size_t nmemb, size_t size);

void * malloc(size_t size);

void free(void *ptr);

void *realloc(void *ptr, size_t size);

node * find_tail(node * head);

node * create_node(size_t size, void * ptr);

node *insert_node(node * current, node * new);

int find_size(size_t size);                                                     
                                                                                
int num_breaks(size_t size);  

void print_list();
