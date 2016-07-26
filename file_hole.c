#include <myerr.h>
#include <fcntl.h>
#include "apue.h"

char    buf1[] = "abcdefghij";
char    buf2[] = "AbCDEFGHIJ";

int
main(void)
{
    int     fd;
//    off_t   currpos;

    if ((fd = creat("file.hole", FILE_MODE)) < 0)
        err_sys("creat error");
    
    if (write(fd, buf1, 10) != 10)
        err_sys("buf1 write error");
    /* offset now is 10 */
    
    int newfd;
    if((newfd = dup2(fd,3)) == -1)
        err_sys("dup error");

//    if(lseek(fd, 16384, SEEK_SET) == -1)
//        err_sys("lseek error");
    /* offset now = 16384 */
    
    if(pwrite(3, buf2, 10, 64) != 10)
        err_sys("buf2 write error");

//    if((currpos = lseek(fd, 0, SEEK_CUR)) == -1)
//        err_sys("currpos get error");
//    printf("%d/n",currpos);

//    if (write(fd, buf2, 10) != 10)
//        err_sys("buf2 write error");
    /* offset now = 16394 */

    exit(0);
}
