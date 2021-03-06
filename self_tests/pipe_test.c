#include <sys/types.h> 
#include <stdio.h> 
#include <string.h> 
#include <unistd.h>
#include <sys/wait.h>

#define BUFFER_SIZE 25 
#define READ_END 0 
#define WRITE_END 1

int main(void)
{
	char *arrays[] = {"This", "is", "a", "message"};
	char write_msg[BUFFER_SIZE]; 
	char read_msg[BUFFER_SIZE];
	int fd[2];
	pid_t pid;

	/* create the pipe */ 
	if (pipe(fd) == -1) {
	  	fprintf(stderr,"Pipe failed");
		return 1;
	}
	/* fork a child process */
	pid = fork();
	if (pid < 0) { 
		/* error occurred */ 
		fprintf(stderr, "Fork Failed"); return 1;
	}

	if (pid > 0) { /* parent process */
		/* close the unused end of the pipe */ 
		close(fd[READ_END]);
		/* write to the pipe */

		for (int i = 0; i < 4; i++)
		{
			printf("sending: %s\n", arrays[i]);
			if (i != 3) {
				write(fd[WRITE_END], arrays[i], strlen(arrays[i]));
			}
			else {
				write(fd[WRITE_END], arrays[i], strlen(arrays[i]) + 1);
			}
		}
		/* close the write end of the pipe */ 
		close(fd[WRITE_END]);
		wait(NULL);
	} else { /* child process */
		/* close the unused end of the pipe */ close(fd[WRITE_END]);
		/* read from the pipe */
		while (read(fd[READ_END], read_msg, BUFFER_SIZE) != 0) {
			printf("read: %s\n",read_msg);
			
		}
		/* close the write end of the pipe */
		close(fd[READ_END]);
	}
	printf("ending: %d\n", pid);
	return 0;
}
