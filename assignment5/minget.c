/* Andy Velazquez , avelaz02@calpoly.edu
 * Caleb Rabbon
 * Assignment 5
 */

#include <stdio.h>
#include <unistd.h>

int flagP = 0;
int argc_counter = 0;
int flagV = 0;

int main(int argc, char * argv[])
{
	int opt;
	char * srcPath = NULL;
    char * dstPath = NULL;
	/*command line parsing 
	 * "vp:s:" means that v,p,s are options. 
	 * the : means that the option requires an 	
     * argument to be present after it */
	while((opt = getopt(argc, argv, "vp:s:")) != -1)
	{
		switch(opt)
		{
			case 'v':
				printf("v\n");
                flagV = 1;
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
	
	if(flagV == 0 && flagP == 0 && argc < 2)
    {
        printf("usage: minget [ -v ] [ -p num [ -s num ] ]");
        printf(" imagefile srcpath [ dstpath ]\n");
        printf("Options:\n");
        printf("-p  part    --- select partition for filesystem");
        printf(" (default: none)\n");
        printf("-s  sub     --- select subpartition for filesystem");
        printf(" (default: none)\n");
        printf("-h  help    --- print usage information and exit\n");
        printf("-v  verbose --- increase verbosity level\n");
        return 0;
        
    }

	for(; optind < argc; optind++)
	{
		argc_counter++;
		printf("left over args: %s\n", argv[optind]);
		if(argc_counter == 2)
		{
			srcPath = argv[optind];
		}
        else if(argc_counter == 3)
        {
            dstPath = argv[optind];
        }
		
		else if(argc_counter == 4)
		{
			printf("too many args extra things\n");
			return 0;
		}
	}
	return 0;
}
