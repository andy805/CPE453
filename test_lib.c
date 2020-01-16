/* testing if running my malloc
 */
#include <stdlib.h>

int main(int argc, char * argv[])
{
   char *ptr = (char *) malloc(sizeof(char));
	malloc(65000);
	malloc(0);
	malloc(800);
   return 0;
}
