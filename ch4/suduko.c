#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct 
{
	int *rows;
	int *columns;	
} parameters;

int main(int argc, char const *argv[])
{
	/* code */
	if (argc != 2)
	{
		fprintf(stderr, "Usage: suduko.c <soduko_file>\n");
	}

	FILE *input_fd;
	int suduko[9][9];
	
	input_fd = fopen(argv[1], "r");




	fclose(input_fd);

	return 0;
}