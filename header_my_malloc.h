/* Andy Velazquez
 * avelaz02@calpoly.edu	
 * assignment 1
 * My library malloc
 */

typedef struct node
{
	struct node *next;
	int alloc_mem_flag;
	/* where is the buffer
	 * size?
	 * how to find the next one
	 */
}

void *calloc(size_t nmemb, size_t size);

void * malloc(size_t size);

void free(void *ptr);

void *realloc(void *ptr, size_t size);
