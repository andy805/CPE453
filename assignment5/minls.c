/* Andy Velazquez , avelaz02@calpoly.edu
 * Caleb Rabbon
 * Assignment 5
 */

#include <stdio.h>
#include <unistd.h>

int flagP = 0;
int argc_counter = 0;

int main(int argc, char * argv[])
{
	int opt;
	char * path = NULL;
	while((opt = getopt(argc, argv, "vp:s:")) != -1)
	{
		switch(opt)
		{
			case 'v':
				printf("v\n");
				break;
			case 'p':
				printf("p\n");
				printf("option in p: %s\n", optarg);
				flagP = 1;
				break;
			case 's':
				printf("option in s: %s\n", optarg);
				if(flagP == 0)
				{
					printf("error didnt get a p option\n");
					return 0;
				}
				break;
			case '?':
				break;	
		}
	}
	
	
	for(; optind < argc; optind++)
	{
		argc_counter++;
		printf("left over args: %s\n", argv[optind]);
		if(argc_counter == 2)
		{
			path = argv[optind];
		}
		
		if(argc_counter == 3)
		{
			printf("too many args extra things\n");
			return 0;
		}
	}
	return 0;
}
