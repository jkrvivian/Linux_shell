#include "shell.h"

void terminal_nonblocking(int enabled) 
{
	/*set nonblocking IO(reference: network)*/
    struct termios tty_state;
    int fd_state;

    tcgetattr(0, &tty_state);
    fd_state = fcntl(0, F_GETFL);

    if (enabled) {
        fcntl(0, F_SETFL, fd_state | O_NONBLOCK);
        tty_state.c_lflag &= ~ICANON;
        tty_state.c_cc[VMIN] = 1;
    } else {
        fcntl(0, F_SETFL, fd_state & ~O_NONBLOCK);
        tty_state.c_lflag |= ICANON;
    }

    tcsetattr(0, TCSANOW, &tty_state);
}

int append(char c)
{
	/*judge when to send message*/
	if(c != EOF) {
		switch(c) {
			case '\n':
				if(find_history) {
					now_cmd = NULL;
					find_history = 0;
				}
				return 1;
				break;
			case 127:
				/* backspace delete character */ 
				printf("\b \b");
				printf("\b \b");
				printf("\b \b");
				input[strlen(input) - 1] = '\0';
				break;
			case '\033':
				getchar(); 
				char dir;
				switch(dir = getchar()) {
					case 'A': /* up */
						printf("\033[2K\r");
						if(!find_history) {
							find_history = 1;
							now_cmd = node;
						} else if(now_cmd->prev && now_cmd->prev != head)  
							now_cmd = now_cmd->prev;
					
						memset(input, '\0', sizeof(input));
						strcpy(input, now_cmd->cmd);
						printf("\033[1;33m0\033[0m\033[1;34m$\033[0m %s", input);
					break;

					case 'B': 	/* down */
						printf("\033[2K\r");
						if(find_history && now_cmd->next) 
							now_cmd = now_cmd->next;
						else {
							printf("\033[1;33m0\033[0m\033[1;34m$\033[0m ");
							break;
						}

						memset(input, '\0', sizeof(input));
						strcpy(input, now_cmd->cmd);
						printf("\033[1;33m0\033[0m\033[1;34m$\033[0m %s", input);
						break;
					
					default:
						break;
				}
				break;
			default:
				/* append input */
				input[strlen(input)] = c;
				break;
		}
	}
	return 0;
}
