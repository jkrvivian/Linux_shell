# Linux_shell
* A simple implementation of Linux shell
* A good practice for Linux system call
  (e.g. pipe(), dup2(), execvp()...)

## Build
* `$ make` to build
* `$ ./shell` to run

## Function
* Support all the commands in `$PATH`
* Support I/O redirection(`<` `>`) and pipe(`|`)

### Self-implement commands
* `exit`
  * terminate the shell program 
* `cd`
  * change directory
* `history`
  * show all the history since you start this program
* recall history command by pressing up or down arrow
  * implement with Non-blocking I/O
  
## TODO
* I/O redirection bug fix
* Exception handling 
