#include "apue.h"
#include <myerr.h>

int main(void)
{
    int     n;
    int     fd[2];
    pid_t   pid;
    char    line[MAXLINE];

    if (pipe(fd) < 0)
        err_sys("pipe error");
    if ((pid = fork()) < 0)
        err_sys("fork error");
    else if (pid > 0) {     /* parent process */
        close(fd[0]);
        //if ((dup2(fd[1], STDOUT_FILENO)) != STDOUT_FILENO)
        //    err_sys("dup error"); 
        //write(STDOUT_FILENO, "hello world\n", 12);
    } else {                /* child process */
        close(fd[1]);
        if ((dup2(fd[0], STDIN_FILENO)) != STDIN_FILENO)
            err_sys("dup error");        
        n = read(STDIN_FILENO, line, MAXLINE);
        write(STDOUT_FILENO, line, n);
    }
    exit(0)
}

