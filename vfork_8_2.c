#include <myerr.h>
#include "apue.h"

int glob = 6;

int main(void)
{
    int var = 10;
    pid_t fpid;

    if((fpid = vfork()) < 0){
        err_sys("vfork error");
    }else if(fpid == 0){
        ++glob;
        ++var;
        printf("child pid = %d ppid = %d glob = %d var = %d fpid = %d\n",getpid(), getppid(), glob, var, fpid);
        exit(0);
    }else
        printf("parent pid = %d ppid = %d glob = %d var = %d fpid = %d\n",getpid(), getppid(), glob, var, fpid);
    exit(0);
}
