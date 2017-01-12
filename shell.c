#include "shell.h"

int main()
{
	char tmp;
    find_history = 0;
	head = malloc(sizeof(command));
	head->next = NULL;
	head->prev = NULL;
	node = head;

	while(1) {
		printf("\033[1;33m0\033[0m\033[1;34m$\033[0m ");
		memset(input, '\0', sizeof(input));

		terminal_nonblocking(1);

		/* wait for input */
		while(1) {
			tmp = getchar();		
			if(append(tmp)) 
				break;
		}

		terminal_nonblocking(0);
		
		/* get command and add to command linked list */
		ADDCMDLIST(node, input)

		if(cut_cmd(input) == -1) 
			break;
	}

	return 0;
}

int cut_cmd(char *tmp)
{
	char *str, *parse_cmd[20];
	int i, len = 0, pipe_count = 0;
	memset(parse_cmd, '\0', sizeof(parse_cmd));

	/* parse command */
	str = strtok(tmp, " ");
	while(str) {
		parse_cmd[len++] = str;
		if(strcmp(str, "|") == 0)
			++pipe_count;
		str = strtok(NULL, " ");
	}
	parse_cmd[len] = NULL;
	
	/* execute command */
	return execute_cmd(parse_cmd, pipe_count);
}

int execute_cmd(char *parse_cmd[], int pipe_count)
{
	int i, k, j = 0, ret, in = 0, fd[2];
	int child_status, small, big, redirect_in, redirect_out;
	char *pipe_cmd[30];
	pid_t pid;
	
	for(i = 0; i <= pipe_count; ++i) {
		ret = 0, small = 0, big = 0;
		pipe(fd);
		
		/* get pipe command */
	    k = 0;
		while(parse_cmd[j] && strcmp(parse_cmd[j], "|") != 0) {
			if(strcmp(parse_cmd[j], "<") == 0) {
				++small;
				redirect_in = open(parse_cmd[++j], O_RDONLY);
				if(redirect_in == -1) {
					printf("File not found\n");
					return ret;
				} 
				++j;
				continue;
			} else if(strcmp(parse_cmd[j], ">") == 0) {
				++big;
				redirect_out = open(parse_cmd[++j], O_CREAT | O_WRONLY, 0666);
				if(redirect_out == -1) {
					printf("Create file failed\n");
					return ret;
				}
				++j;
				continue;
			}
			pipe_cmd[k++] = parse_cmd[j++];
		}		
		pipe_cmd[k] = NULL;
		++j;
		
		/* start execution */
        if ((pid = fork()) == -1)
            exit(EXIT_FAILURE);
        else if (pid == 0) {
			if(small) { 
				dup2(redirect_in, in);
				close(redirect_in);
			}
			if(big) { 
				dup2(redirect_out, 1);
				close(redirect_out);
				close(fd[1]);
			}

            dup2(in, 0); 			//change the input according to the old one 
            if (i != pipe_count && !big)
              dup2(fd[1], 1);
            close(fd[0]);    
                
            if(execvp(pipe_cmd[0], pipe_cmd) == -1)
          	  exit(5);
            else
          	  exit(EXIT_SUCCESS);
          	
        } else {
          	waitpid(pid, &child_status, 0);
          	
			/* self-implement command */          	
          	if(WIFEXITED(child_status)) {  
				if(WEXITSTATUS(child_status) == 5) {
					if(strcmp(parse_cmd[0], "exit") == 0)
						ret = -1;
					else if(strcmp(parse_cmd[0], "history") == 0) {
						print_history();
					} else if(strcmp(parse_cmd[0], "cd") == 0) {
						if(chdir(parse_cmd[1]) != 0)
							printf("change directory failed\n");
					} else 
						printf("can't find \"%s\" command\n", parse_cmd[0]);
				}
			}
          	close(fd[1]);
          	in = fd[0]; //save the input for the next command
        }	
	}
	return ret;
}

void print_history()
{
	command *i = head->next;
	int j = 0;
	for(; i && i->next; i = i->next) {
		printf(" %2d  %s\n", j, i->cmd);
		++j;
	}
}

