#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <myerr.h>

int main(void)
{
    pid_t pid;
    int glob = 11;
    
    if ((pid = vfork()) < 0){
        err_sys("vfork error!");
        exit(-1);
    }else if (pid == 0){
        ++glob;
        int var = 1;
        return 0;
    }else {
        ++glob;
    }

    printf("glob=%d, pid=%d",glob, getpid());
    exit(0);
}
