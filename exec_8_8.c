#include "apue.h"
#include <myerr.h>
#include <sys/wait.h>

char *env_init[] = { "USER=dingo", "PATH=/tmp", NULL };

int main(void)
{
    pid_t   pid;

    if ((pid = fork()) < 0) {
        err_sys("fork error");
    }else if (pid == 0) {   /* specify pathname, specify enviornment */
        if (execle("/home/dingo/yd/apue/echoall", "echoall", "myarg1",
                        "MY ARG2", (char *)0, env_init) < 0)
            err_sys("execle error");
    }

    if (waitpid(pid, NULL, 0) < 0)
        err_sys("wait error");
    
    if ((pid = fork()) < 0) {
        err_sys("fork error");
    }else if (pid == 0){
        if (execlp("./echoall", "echoall", "only 1 arg", (char *)0) < 0)
            err_sys("execlp error");
    }
    exit(0);
}
