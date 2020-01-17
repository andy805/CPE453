/* Andy Velazquez
 * CPE 453
 * NICO
 * afternoon section
 * lab 1
 */

#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
int main(int argc, char * argv[])
{
   pid_t child_a, child_b;
   int status_a, status_b;
   int fd[2];

   int outfile_descriptor;

   if(pipe(fd) == -1)
   {
      fprintf(stderr, "pipe failed");
      return 1;
   }
   

   if((child_a = fork()))
   {
      /*this is the parent*/
      if(child_a == -1)
      {
	      perror("fork failure child_a");
	      exit(EXIT_FAILURE);

      }
      if((child_b = fork()))
      {
         /*this is the parent*/
         if(child_b == -1)
	 {
            perror("fork failure child_b");
	    exit(EXIT_FAILURE);
	 }
      }
      else
      {
         /*this is child_b*/
         close(fd[1]); /*closing the write end of pipe*/
	      dup2(fd[0], STDIN_FILENO);/*fd[0] is read end of pipe*/
	      outfile_descriptor = open("outfile",
			  O_RDONLY  | O_CREAT | O_WRONLY | O_TRUNC, 777);
	      dup2(outfile_descriptor, STDOUT_FILENO);
	      execl("/usr/bin/sort", "usr/bin/sort", "-r", NULL);
	      close(outfile_descriptor);
	      close(fd[0]);
         exit(0);
      }
      waitpid(child_a, &status_a, WIFEXITED(status_a));
      waitpid(child_b, &status_b, WIFEXITED(status_b));
      close(fd[1]);/*closing write end*/
      close(fd[0]);/*closing read end*/
   }
   else
   {
      /*this is the child a*/
      dup2(fd[1], STDOUT_FILENO);/*fd[1] is write end of pipe*/
      close(fd[0]);/*closing read end of pipe, not using it*/
      execl("/bin/ls", "ls", NULL);
      close(fd[1]);
      exit(0);
   }


   return 0;
}


