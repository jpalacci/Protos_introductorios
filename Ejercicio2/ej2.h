#ifndef EJ2_H
#define EJ2_H

#define ARG_QTY 3

enum parameters
{
	PROGRAM, BUFFER_SIZE, COMMAND
};

enum pipe_ends
{
	READ, WRITE
};

enum errors
{
	PIPE_FAIL, FORK_FAIL, INVALID_PARAMETERS
};


void parent(int childpipe[], int parentpipe[], int buffSize);
int validateParameters(int argc, char * argv[]);
int calculateParity(char * s);


#endif
