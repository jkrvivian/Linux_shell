# Linux_shell
* A simple implementation of Linux shell
* A good practice for Linux system call
  (e.g. pipe(), dup2(), execvp()...)

## Build
* `$ make` to build
* `$ ./shell` to run

## Function
* Can perform all the commands in `$PATH`
* Can perform I/O redirection(`<` `>`) and pipe(`|`)
### Self-implement commands
* `exit`
  * terminate the shell program 
 * `cd`
  * change directory
 * `history`
  * show all the history since you start this program

## TODO
* I/O redirection bug fix
