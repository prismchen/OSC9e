/**
 * Simple shell interface program.
 *
 * Operating System Concepts - Ninth Edition
 * Copyright John Wiley & Sons - 2013
 */

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <stdlib.h>



#define MAX_LINE 80 /* 80 chars per line, per command */

int main(void)
{
	char *args[MAX_LINE/2 + 1];	/* command line (of 80) has max of 40 arguments */
    int should_run = 1;


    char *history[10];
    int newest = -1;
    int oldest = 0;
	
		
    while (should_run){   

        printf("osh>");
        fflush(stdout);

        // get usr input
        char input_str[MAX_LINE];
        fgets(input_str, MAX_LINE, stdin);

        // remvoe newline
        char *pos;
        if ((pos=strchr(input_str, '\n')) != NULL)
            *pos = '\0';

        // repeat last cmd
        if (strcmp(input_str, "!!") == 0) {
            if (history[newest] == NULL) {
                fprintf(stderr, "No commands in history\n");
                continue;
            }
            strcpy(input_str, history[newest]);
        }
        // repeat cmd with number followed by '!'
        else if (input_str[0] == '!') {
            int j = 1;
            int index = 0;
            while (j < strlen(input_str) && input_str[j] >= '0' && input_str[j] <= '9') {
                index = index*10 + (int) (input_str[j] - '0');
                j++;
            }
            if (index-1 <= newest && index-1 >= oldest) { // if number is within range
                memset(input_str, 0, strlen(input_str));
                strncpy(input_str, history[(index-1) % 10], sizeof input_str - 1);
            }
            else {
                fprintf(stderr, "No such command in history\n");
                continue;
            }
        }


        // add new cmd to history, update two pointers
        newest++;
        history[newest % 10] = (char *)malloc(strlen(input_str)+1);
        strcpy(history[newest % 10], input_str);
        if (newest - oldest  == 10) oldest++;

        // tokenize input string, stored resutls in args
        int i=0;
        char *ptr;
        ptr = strtok(input_str, " ");
        while (ptr != NULL) {
            args[i++] = ptr;
            ptr = strtok(NULL, " ");
        }
        args[i] = NULL;

        
        // find if cmd ends with '&'
        int str_cmp = strcmp(args[i-1], "&");

        // create child
        pid_t pid;
        pid = fork();

        if (pid < 0) { // fork() fails
            fprintf(stderr, "Fork Failed\n");
            return 1;
        }

        if (pid > 0) { // parent

            if (str_cmp != 0) { // parent waits for child
                waitpid(pid, NULL, WUNTRACED);
            }
            // clear args[]
            for (int k=0; k<MAX_LINE/2 + 1; k++) args[k] = NULL;
            
        }
        else if (pid == 0) { // child
            if (strcmp(args[0], "history") != 0) { // if cmd != "history"
                if (str_cmp == 0) { // if cmd ends with '&', remove it for clean execution
                    args[i-1] = NULL; 
                }
                int indicator = execvp(args[0], args); // execution in child
                if (indicator == -1) {
                    fprintf(stderr, "Child Failed\n");
                    return 1;
                }
            }
            else { // if cmd == "history"
                for (int i = newest; i >= oldest; i--)
                {
                    printf("%d %s\n", i+1, history[i % 10]);
                }
            }
            // child exit while loop
            should_run = 0;
        }
        
    }
    
	return 0;
}
