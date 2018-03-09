#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

char * readInput();
int calculateParity(char * s);


int main(int argc, char ** argv)
{
	int pid;
	int parentpipe[2];
	int childpipe[2];
	
	pipe(parentpipe);
	pipe(childpipe);


	pid = fork();

	if(pid == 0)
	{
		int stdin_copy = dup(0);

		dup2(parentpipe[0], STDIN_FILENO);
		dup2(childpipe[1], STDOUT_FILENO);

		close(childpipe[0]);
		close(parentpipe[1]);

		//Como hacer sin semaforo?

		system(argv[1]);

		close(childpipe[1]);

	}
	else
	{


		close(childpipe[1]);
		close(parentpipe[0]);


		int c;
		int inXor = 0;

		while( (c = getchar()) != EOF)
		{
			inXor ^= c;
			write(parentpipe[1], (char*)&c, sizeof(char));
		}
		
		close(parentpipe[1]);

		char a;
		
		int outXor = 0;

		while(read(childpipe[0], &a, sizeof(char)) > 0)
		{
			outXor ^= a;
			putchar(a);
		}
		putchar('\n');

		fprintf(stderr, "in parity: 0x%x\n", inXor);
      	fprintf(stderr, "out parity: 0x%x\n", outXor);





	}

	return 0;

}

int calculateParity(char * s)
{
  int ret = 0;
  while(*s)
  {
    ret = ret ^ *s;
    s++;
  }
  return ret;
}



char * readInput()
{
  int c;
  char * ret = malloc(10);
  int index = 0;

  while((c = getchar()) != EOF && index < 10)
  {
      ret[index++] = c;
  }

  ret[index] = 0;
  return ret;
}