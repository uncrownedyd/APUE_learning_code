#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <myerr.h>

//#define oops(m, x)      {perror(m); exit(x);}
void be_dc(int in[2], int out[2]);
void be_bc(int in[2], int out[2]);
void fatal(const char *mess[]);

int main(void)
{
    pid_t   pid;
    int     todc[2], fromdc[2];     /* filedes for two pipe */

    /* make two pipes */

    if (pipe(todc) < 0 || pipe(fromdc) < 0)
        err_sys("pipe error");

    if ((pid = fork()) < 0) {
        err_sys("fork error");
    } else if (pid == 0) {      /* child process - dc */
        be_dc(todc, fromdc);
    } else {
        be_bc(todc, fromdc);
        wait(NULL);
    }
    exit(0);
}

void be_dc(int in[2], int out[2])
{   /* set up stdin&stdout, then execl dc */

    /* setup stdin from pipein */
        if (dup2(in[0], STDIN_FILENO) != STDIN_FILENO)      /* copy read end to stdin */
            err_sys("dc:cannot redirect stdin");
        close(in[0]);       /* redirect into stdin */
        close(in[1]);       /* won't write here */

    /* setup stdout to pipeout */
        if (dup2(out[1], STDOUT_FILENO) != STDOUT_FILENO)   /* copy write end to stdout */
            err_sys("dc:cannot redirect stdout");
        close(out[1]);      /* redirect into stdout */
        close(out[0]);      /* won't read from here */

    /* now execl dc with the - option */
        execlp("dc", "dc", "-", NULL);
        err_sys("cannot run dc");
}

void be_bc(int todc[2], int fromdc[2])
{   /* read from stdin and convert into RPN,then send down pipe 
     * then read from other pipe and print it to user 
     * use fdopen() to convert a filedes to a stream
     */
    int     num1, num2;
    char    operation[BUFSIZ], message[BUFSIZ];
    FILE    *fpout, *fpin;

    /* setup */
    close(todc[0]);         /* won't read from pipe to dc */
    close(fromdc[1]);       /* won't write to pipe from dc */

    fpout = fdopen(todc[1], "w");   /* convert file descript */
    fpin = fdopen(fromdc[0], "r");  /* riptors to stream */
    if (fpout == NULL || fpin == NULL)
        fatal("Error convering pipes to streams");

    /* main loop */
    while (printf("tinybc: "), fgets(message, BUFSIZ, stdin) != NULL) {
    /* parse input */
        if (sscanf(message, "%d %[- + * / ^] %d", &num1, operation, &num2) != 3) {
            printf("syntax error\n");
            continue;
        }

        if (fprintf(fpout, "%d\n %d\n %c\n p \n", num1, num2, *operation) == EOF )
            fatal("Error writing");

        fflush(fpout);
        if (fgets(message, BUFSIZ, fpin) == NULL)
            break;

        printf("%d %c %d = %s", num1, *operation, num2, message);
    }
    fclose(fpout);      /* close pipe */
    fclose(fpin);       /* dc will see EOF */
}

void fatal(const char *mess[])
{
    fprintf(stderr,"Error: %s\n", mess);
    exit(1);
}
