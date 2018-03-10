
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <unistd.h>


#define MAX_METHOD_SIZE 10

//STATES

#define FIRST_LINE 10
#define HEADERS 11
#define BODY 12
#define TERMINAL 13
#define HOST_LENGTH 4
#define INIT_SIZE 100
#define BUFFER_SIZE 20


typedef struct 
{
	char method[MAX_METHOD_SIZE];
	char * host;
	char * port;
	char * requestTarget;
	int bodyBytes;
	int parity;

} HttpValuesStruct;

char * readLine(int * endOFMessage);
void parseFirstLine(HttpValuesStruct * httpValues , char * line);
int isLetter(char c);
void parseHeader(HttpValuesStruct * httpValues , char * line);
int parityBuffer(char * s, int size);
void analyzeBody(HttpValuesStruct * httpValues);

int main(int argc, char * argv[])
{
	int bufferSize = 20;
	char buffer[bufferSize];
	int c;
	int state = FIRST_LINE;

	int endOFMessage = 0;

	HttpValuesStruct httpValues;

	httpValues.host = NULL;

	char * message;

	while(state != TERMINAL)
	{
		message = readLine(&endOFMessage);
		printf("%s\n", message);

		switch(state)
		{
			case FIRST_LINE:
			{
				parseFirstLine(&httpValues, message);
				state = HEADERS;
			}
			break;

			case HEADERS:
			{
				if(message[0] == 0)
				{
					state = TERMINAL;
					break;
				}
				else
				{
					if(httpValues.host == NULL)
					{
						parseHeader(&httpValues , message);
					}
				}
			}
			break;

			default:;
		}
	}

	analyzeBody(&httpValues);

	printf("The method is %s\n", httpValues.method);
	printf("The Target is %s\n", httpValues.requestTarget );
	printf("The Host is %s\n", httpValues.host);
	printf("The Port is %s\n", httpValues.port );
	printf("The Body size  is %d\n", httpValues.bodyBytes);
	printf("The parity of the body is %02x\n", httpValues.parity);

	printf("\n\nThe answer is : %s %s %s  %s %d %02x\n", httpValues.method, httpValues.host, httpValues.port, httpValues.requestTarget, httpValues.bodyBytes, httpValues.parity );

}

void analyzeBody(HttpValuesStruct * httpValues)
{
	int counter = 0;
	int parity = 0;
	int c;
	char buffer[BUFFER_SIZE];
	int n;

	while((c = getchar()) != EOF )
	{
		parity ^= c;
		counter += 1;
	}


	httpValues->bodyBytes = counter;
	httpValues->parity = parity;
}

void parseHeader(HttpValuesStruct * httpValues , char * line)
{
	char * host = "HOST";
	int i = 0;

	while(line[i] != ' ' && toupper((line[i]))== host[i] && i < HOST_LENGTH)
	{
		i++;
	}

	if(i != HOST_LENGTH)
	{
		return;
	}

	while(!isLetter(line[i]))
	{
		i++;
	}

	httpValues->host = line + i;

	while(!(line[i] == ':' || line[i] == 0))
	{
		i++;
	}
	


	if(line[i] == 0)
	{
		httpValues->port = "80";
	}
	else
	{
		line[i] = 0;
		httpValues->port = line + i + 1;

		while(line[i] != 0)
		{
			i++;
		}
		line[i] = 0;
	}



}

void parseFirstLine(HttpValuesStruct * httpValues , char * line)
{
	int i = 0;

	int startTarget;
	int endTarget;

	while(line[i] != ' ')
	{
		httpValues->method[i] = line[i];
		i++;
	}

	httpValues->method[i] = 0;

	while(!(line[i - 1] != '/' && line[i] == '/' && line[i + 1] != '/'))
	{
		i++;
	}

	httpValues->requestTarget = line + i;

	while(line[i] != ' ')
	{
		i++;
	}

	line[i] = 0;
}



char * readLine(int * endOFMessage)
{
	char * line = malloc(INIT_SIZE * sizeof(char));
	int i = 0;
	int currentSize = INIT_SIZE;
	int c;

	while((c = getchar()) != '\n' && c != -1)
	{
		if(i >= currentSize - 1)
		{
			line = realloc(line, currentSize + INIT_SIZE);
			//check for NULL
		}

		line[i++] = c;
	}
	line[i] = 0;

	if(c == -1)
	{
		*endOFMessage = 1;
	}

	return line;
}

int parityBuffer(char * s, int size)
{
  int ret = 0;
  int i = 0;
  while(*s && i < size)
  {
    ret = ret ^ *s;
    s++;
    i++;
  }
  return ret;
}

int isLetter(char c)
{
	if( (c <= 'z' && c >= 'a') || (c <= 'Z' && c >= 'A') )
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

