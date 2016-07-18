#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const int n = 9;
int suduko[n][n];
int check_array[3*n];

typedef struct 
{
	int rows;
	int columns;
	int ordinal;	
} parameters;

void parse_input_line(int *array, char *buf);
void *check(void *param);

int main(int argc, char const *argv[])
{
	/* code */
	if (argc != 2)
	{
		fprintf(stderr, "Usage: ./suduko <soduko_file>\n");
		exit(1);
	}

	char buf[18];

	FILE *input_fd;
	input_fd = fopen(argv[1], "r");

	for (int i=0; i<n; i++)
	{
		if (fgets(buf, 19, input_fd) == NULL)
		{
			break;
		}
		parse_input_line(suduko[i], buf);
	}

	pthread_t tid[3*n];
	pthread_attr_t attr;

	pthread_attr_init(&attr);

	// rows
 	for (int i=0; i<n; i++) 
 	{
 		parameters *data = (parameters *) malloc(sizeof(parameters));
 		data->rows = i;
 		data->columns = -1;
 		data->ordinal = i;
 		pthread_create(&tid[i], &attr, check, data);
 	} 
 	// columns
 	for (int i=0; i<n; i++) 
 	{
 		parameters *data = (parameters *) malloc(sizeof(parameters));
 		data->rows = -1;
 		data->columns = i;
 		data->ordinal = i + n;
 		pthread_create(&tid[i], &attr, check, data);
 	} 
 	// blocks 
 	int k = 0;
 	for (int i=0; i<3; i++) 
 	{
 		for (int j=0; j<3; ++j)
 		{
 			parameters *data = (parameters *) malloc(sizeof(parameters));
	 		data->rows = i;
	 		data->columns = j;
	 		data->ordinal = k++ + 2*n;
	 		pthread_create(&tid[i], &attr, check, data);
 		}
 	} 

 	for (int i=0; i<3*n; i++) 
 	{
 		pthread_join(tid[i], NULL);
 	}

 	// Check result
 	int final_result = 1;
 	for (int i=0; i<3*n; i++) 
 	{
 		if (check_array[i] == 0)
 		{
 			final_result = 0;
 		}
 		printf("%d ", check_array[i]);
 	}
 	printf("\n");

 	if (final_result == 1) 
 	{
 		printf("This is a valid suduko\n");
 	}
 	else 
 	{
 		printf("This is Not a valid suduko\n");
 	}

	fclose(input_fd);
	return 0;
}

void *check(void *param) 
{
	parameters *data = (parameters*) param;
	int all_nums[n];
	for (int i = 0; i < n; ++i)
	{
		all_nums[i] = 0;
	}

	int check_result = 1;
	// to chcek a row 
	if (data->columns == -1) {
		for (int i=0; i<n; i++)
		{
			int content = suduko[data->rows][i];
			if (content < 10 && content >= 0 && all_nums[content] == 0) 
			{
				all_nums[content] = 1; 
			}
			else 
			{
				check_result = 0;
				break;
			}
		}
	}
	// to check a column
	else if (data->rows == -1) {
		for (int i=0; i<n; i++)
		{
			int content = suduko[i][data->columns];
			if (content < 10 && content >= 0 && all_nums[content] == 0) 
			{
				all_nums[content] = 1; 
			}
			else 
			{
				check_result = 0;
				break;
			}
		}
	}
	// check a block 
	else 
	{
		printf("%d %d\n", data->rows, data->columns);
		for (int i=0; i<3; i++)
		{
			for (int j=0; j<3; j++) 
			{
				int content = suduko[3*(data->columns) + i][3*(data->rows) + j];
				if (content < 10 && content >= 0 && all_nums[content] == 0)
				{
					all_nums[content] = 1; 
				}
				else 
				{
					check_result = 0;
					break;
				}
			}
		}
	}

	check_array[data->ordinal] = check_result;
	pthread_exit(0);
} 

void parse_input_line(int *array, char *buf)
{
	for (int i = 0; i < n; ++i)
	{
		array[i] = atoi(&buf[2*i]);
	}
}