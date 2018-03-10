#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "ej2.h"


int main(int argc, char * argv[])
{
	int pid;
	int pipeToChild[2];
	int pipeToParent[2];
	int buffersize;
	
	if(pipe(pipeToChild) < 0 || pipe(pipeToParent) < 0)
	{
		return PIPE_FAIL;
	}

	if(!validateParameters(argc, argv))
	{
		return INVALID_PARAMETERS;
	}

	pid = fork();

	if(pid > 0)
	{

		close(pipeToParent[WRITE]);
		close(pipeToChild[READ]);

		buffersize = atoi(argv[BUFFER_SIZE]) + 1;

		parent(pipeToParent, pipeToChild, buffersize);

		wait(NULL);

	}
	else if(pid == 0)
	{
		
		dup2(pipeToChild[READ], STDIN_FILENO);
		dup2(pipeToParent[WRITE], STDOUT_FILENO);

		close(pipeToParent[READ]);
		close(pipeToChild[WRITE]);

		system(argv[COMMAND]);

		close(pipeToParent[WRITE]);

	}
	else
	{
		return FORK_FAIL;
	}

	return 0;

}


void parent(int pipeToParent[], int pipeToChild[], int buffSize)
{
	int inXor = 0;
	int outXor = 0;
	char buff[buffSize];
	int bytesRead = 0;
	int readCount = 0;
	

	while((bytesRead = read(STDIN_FILENO, buff, buffSize - 1)) > 0)
	{
		buff[bytesRead] = 0;
		inXor ^= calculateParity(buff);
		write(pipeToChild[WRITE], buff, bytesRead);
	}
	
	close(pipeToChild[WRITE]);

	while((bytesRead = read(pipeToParent[READ], buff, buffSize - 1)) > 0)
	{
		buff[bytesRead] = 0;
		outXor ^= calculateParity(buff);
		readCount++;
		printf("%s", buff);
	}
	putchar('\n');

	//We take the last new line char
	outXor = outXor ^ '\n';

	fprintf(stderr, "in parity: 0x%02X\n", inXor);
  	fprintf(stderr, "out parity: 0x%02X\n", outXor);
  	fprintf(stderr, "%d %s needed\n", readCount, readCount == 1? "read was" : "reads were");
}

int validateParameters(int argc, char * argv[])
{
	if(argc != ARG_QTY)
	{
		printf("%s\n", "Invalid number of parameters");
		printf("Usage: %s buffer_lenght command\n", argv[PROGRAM]);
		return 0;
	}

	return 1;
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

