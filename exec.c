#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#define BUFF_SIZE 30

char * readInput();
int calculateParity(char * s);
void parent(int childpipe[] , int parentpipe[]);


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

		dup2(parentpipe[0], STDIN_FILENO);
		dup2(childpipe[1], STDOUT_FILENO);

		close(childpipe[0]);
		close(parentpipe[1]);

		system(argv[1]);

		close(childpipe[1]);

	}
	else
	{
		close(childpipe[1]);
		close(parentpipe[0]);

		parent(childpipe , parentpipe);

	}

	return 0;

}


void parent(int childpipe[] , int parentpipe[])
{
	int c;
	int inXor = 0;
	char buff[BUFF_SIZE];
	int bytesRead;
	char a;
	int outXor = 0;

	while((bytesRead = read(STDIN_FILENO, buff, BUFF_SIZE - 1)) > 0)
	{
		buff[bytesRead] = 0;
		inXor ^= calculateParity(buff);
		write(parentpipe[1], buff, bytesRead);
	}
	
	close(parentpipe[1]);

	while((bytesRead = read(childpipe[0], buff, BUFF_SIZE - 1)) > 0)
	{
		buff[bytesRead] = 0;
		outXor ^= calculateParity(buff);
		printf("%s", buff);
	}

	//We take the last nl char
	outXor = outXor ^ '\n';

	fprintf(stderr, "in parity: 0x%02x\n", inXor);
  	fprintf(stderr, "out parity: 0x%02x\n", outXor);
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

