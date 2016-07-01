#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

void *fib_cal(void *param);
int fib(int i);

int *fib_arr;

int main(int argc, char *argv[])
{

	if (argc !=2) 
	{
		fprintf(stderr, "Usage: fib-seq <number>\n");
	}
	if (atoi(argv[1]) < 1) {
		fprintf(stderr, "Usage: Argument %d must be greater than one", atoi(argv[1]));
	}

	int num = atoi(argv[1]);

	fib_arr = malloc(sizeof(int) * num);

	

	pthread_t tid;
	pthread_attr_t attr;

	pthread_attr_init(&attr);
	pthread_create(&tid, &attr, fib_cal, argv[1]);

	pthread_join(tid, NULL);

	
	for (int i = 0; i < num; ++i)
	{
		printf("%d ", fib_arr[i]);
	}

	printf("\n");

	free(fib_arr);
	return 0;

}

void *fib_cal(void *param)
{
	int num = atoi(param);

	for (int i = 0; i < num; ++i)
	{
		fib_arr[i] = fib(i+1);
	}

	pthread_exit(0);

}

int fib(int i) {
	if (i <= 2) return 1;
	else return fib(i-1) + fib(i-2);
}	