#include <myerr.h>
#include <sys/wait.h>
#include "apue.h"

int main()
{
    pid_t pid;

    if ((pid = fork()) < 0) {
        err_sys("fork error");
    }else if (pid == 0) {
        printf("first child before fork, parent pid = %d, pid = %d\n", getppid(), pid); 
        if ((pid = fork()) < 0)
            err_sys("fork error");
        else if (pid > 0){
            printf("first child after fork, parent pid = %d, pid = %d\n", getppid(), pid);
            exit(0);
        }
        //sleep(5);
        printf("second child, parent pid = %d\n", getppid());
        exit(0);
    }
    
    printf("parent, parent pid = %d, first child pid = %d\n", getppid(), pid);
    if (waitpid(pid, NULL, 0) != pid)
        err_sys("waitpid error");

    exit(0);
}
