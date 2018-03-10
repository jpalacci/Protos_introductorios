#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

char * readInput();
int calculateParity(char * s);
void parent(int childpipe[] , int parentpipe[], int buffSize);
int validateParameters(int argc , char * argv[]);


int main(int argc, char * argv[])
{
	int pid;
	int parentpipe[2];
	int childpipe[2];
	int buffersize;
	
	pipe(parentpipe);
	pipe(childpipe);

	if(!validateParameters(argc , argv))
	{
		return 0;
	}

	pid = fork();

	if(pid == 0)
	{

		dup2(parentpipe[0], STDIN_FILENO);
		dup2(childpipe[1], STDOUT_FILENO);

		close(childpipe[0]);
		close(parentpipe[1]);

		system(argv[2]);

		close(childpipe[1]);

	}
	else
	{
		close(childpipe[1]);
		close(parentpipe[0]);

		buffersize = atoi(argv[1]) + 1;

		parent(childpipe , parentpipe , buffersize);

	}

	return 0;

}


void parent(int childpipe[] , int parentpipe[], int buffSize)
{
	int c;
	int inXor = 0;
	char buff[buffSize];
	int bytesRead;
	char a;
	int readCount = 0;
	int outXor = 0;


	while((bytesRead = read(STDIN_FILENO, buff, buffSize- 1)) > 0)
	{
		buff[bytesRead] = 0;
		inXor ^= calculateParity(buff);
		write(parentpipe[1], buff, bytesRead);
	}
	
	close(parentpipe[1]);

	while((bytesRead = read(childpipe[0], buff, buffSize - 1)) > 0)
	{
		buff[bytesRead] = 0;
		outXor ^= calculateParity(buff);
		readCount++;
		//printf("%s", buff);
	}

	//We take the last nl char
	outXor = outXor ^ '\n';

	fprintf(stderr, "in parity: 0x%02x\n", inXor);
  	fprintf(stderr, "out parity: 0x%02x\n", outXor);
  	fprintf(stderr, "%d reads where needed\n", readCount);
}

int validateParameters(int argc , char * argv[])
{
	if(argc != 3)
	{
		printf("%s\n", "Invalid number of parameters" );
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

