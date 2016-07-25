#include <myerr.h>
#include "apue.h"
#include <errno.h>

int
main(int argc, char *argv[])
{
    fprintf(stderr, "EACCES: %s\n", strerror(EACCES));
    errno = EFAULT;

    perror(argv[0]);
    exit(0);
}

