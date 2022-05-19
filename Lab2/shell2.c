#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/types.h>
#include <signal.h>

#define MAXLINE 4096                    /* max line length */

void quit(char *msg);

int main(void)
{
	char	buf[MAXLINE];	/* from apue.h */
	pid_t	pid;
	int		status;
	char *arg[MAXLINE];
  
	printf("\n%% ");	/* print prompt (printf requires %% to print %) */
	while (fgets(buf, MAXLINE, stdin) != NULL) {
		if (buf[strlen(buf) - 1] == '\n')
			buf[strlen(buf) - 1] = 0; /* replace newline with null */
		arg[0] = strtok(buf, " ");
		int i = 0;
		while(arg[i] != NULL){
			i++;
			arg[i] = strtok(NULL, " ");
		}

		if ((pid = fork()) < 0) {
			quit("fork error");
		}
    else if (pid == 0) {		/* child */
      int i = 0;
      int fd;
        while (arg[i]) {   /* parse args for '<' or '>' and filename */
            if (*arg[i] == '>' && arg[i+1]) {
                if ((fd = open (arg[i+1], 
                            O_WRONLY | O_CREAT, 
                            S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)) == -1) {
                    perror (arg[i+1]);
                    exit (EXIT_FAILURE);
                }
                dup2 (fd, 1);
                dup2 (fd, 2);
                close (fd);
                while (arg[i]) {
                    arg[i] = arg[i+2];
                    i++; 
                }
                break;
            }
            else if (*arg[i] == '<' && arg[i+1]) {
                if ((fd = open (arg[i+1], O_RDONLY)) == -1) {
                    perror (arg[i+1]);
                    exit (EXIT_FAILURE);
                }
                dup2 (fd, 0);
                close (fd);
                while (arg[i]) {
                    arg[i] = arg[i+2];
                    i++; 
                }
                break;
            }
            i++;
        }
			execvp(arg[0], arg);
			quit("couldn't execute: ");
			exit(127);
		}
		/* parent */
		if ((pid = waitpid(pid, &status, 0)) < 0)
			quit("waitpid error");
		printf("%% ");
	}
	exit(0);
}

void quit(char *msg)
{
  printf("%s\n", msg);

  exit(1);
}