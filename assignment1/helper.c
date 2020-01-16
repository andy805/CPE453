#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <sys/stat.h>
#include "header_my_malloc.h"

node * find_tail(node *head)
{
	node *current = head;
	if(head == NULL)
	{
		return head;
	}

	while(current->next != NULL)
	{
		current = current->next;	
	}
	return current;
}

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

