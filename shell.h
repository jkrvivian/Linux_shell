#ifndef _SHELL_
#define _SHELL_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>

#define LENGTH_OF_COMMAND 50

#define ADDCMDLIST(node, str) {\
			node->next = malloc(sizeof(command)); \
			node->next->next = NULL; \
			node->next->prev = node; \
			memset(node->next->cmd, '\0', LENGTH_OF_COMMAND * sizeof(char)); \
			strcpy(node->next->cmd, str); \
			node = node->next; }

typedef struct CMD{
	char cmd[LENGTH_OF_COMMAND];
	struct CMD *next;
	struct CMD *prev;
} command;

char input[50];
int find_history;
command *head, *node, *now_cmd;

void terminal_nonblocking(int enabled);
int append(char c);
int cut_cmd(char *tmp);
int execute_cmd(char *parse_cmd[], int pipe_count);
void print_history();

#endif
