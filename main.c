#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define BUF_LEN 256
#define MAX_ARGC 10
#define DELIMS " \t\r\n"
#define STDIN 0
#define STDOUT 1
#define SINGLE_PROC 111
#define TWO_PROC_PIPE 222
#define IO_REDIR 333
#define BACK_GND 444
#define READ_ERROR 999

int main()
{
	while(1)
	{
		char buf[BUF_LEN];// command line input
		char *arg[MAX_ARGC];// processed command line
		int pid;
		printf("$> ");// prompt
		fgets(buf, BUF_LEN, stdin);// input read
		int k, i, type = 0;
		char *p = buf;// set type
		if(strchr(p, '|') != 0)
			type = TWO_PROC_PIPE;
		if(strchr(p, '>') != 0 || strchr(p, '<') != 0)
			type = IO_REDIR;
		if(strchr(p, '&') != 0)
			type = BACK_GND;
		if(type == 0)
			type = SINGLE_PROC;		
		arg[0] = strtok(buf, DELIMS);// input spilt
		for(k = 1; k < MAX_ARGC; k++)
			arg[k] = strtok(0, DELIMS);
		if(type == SINGLE_PROC)// single process
		{
			pid = fork();
			if(pid > 0)// parent
				wait(NULL);
			else if(pid == 0)// child
				execvp(arg[0], arg);
			else if(pid < 0)// fork fail	
			{
				printf("fork fail!\n");
				break;
			}
		}				
		/*else if(type == TWO_PROC_PIPE)
		{
			int pipe_fd[2];
			int tmp;
			pipe(pipe_fd);// create pipe
			pid = fork();
			if(pid == 0)// child
			{
				close(pipe_fd[0]);// close read, don't need
				close(STDOUT);
				pipe_fd[1] = open(arg[0], O_WRONLY);
				execvp(arg[0], arg[0]);
			}
			else if(pid > 0)// parent
			{
				close(pipe_fd[1]);// close write, don't need
				close(STDIN);
				pipe_fd[0] = open(arg[1], O_RDONLY);
				execvp(arg[1], arg[1]);
			}
			else if(pid < 0)
			{
				printf("fork fail!\n");
				break;
			}
		}*/
	}
	return 0;
}

