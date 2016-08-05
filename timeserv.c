#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <time.h>
#include <string.h>
#include <myerr.h>

#define PORTNUM 15000   /* our time service phone number */
#define HOSTLEN 256
#define oops(msg)       {perror(msg); exit(1);}

int main(int argc, char *argv[])
{
    struct    sockaddr_in  saddr;      /* build our address here */
    struct    hostent      *hp;    
    char      hostname[HOSTLEN];       /* host address */
    int       sock_id, sock_fd;
    FILE      *sock_fp;                /* use socket as stream */
    timer_t   thetime;                 /* the time server report */

    /*
     * STEP1: ask kernel for a socket
     */
    if ((sock_id = socket(PF_INET, SOCK_STREAM, 0)) < 0)     /* get a socket */
        oops("socket");
    
    /*
     * STEP: bind address to socket.Address is host, port
     */
    bzero((void *)&saddr, sizeof(saddr));   /* clear out struct */
    //memset(&saddr, 0, sizeof(saddr));

    gethostname(hostname, HOSTLEN);         /* fill in socket port */
    hp = gethostbyname(hostname);           /* fill in addr family */

    bcopy((void *)hp->h_addr, (void *)&saddr.sin_addr,
                hp->h_length);

    saddr.sin_port = htons(PORTNUM);        /* fill in socket port */
    saddr.sin_family = AF_INET;             /* fill in addr family */
    
    if (bind(sock_id, (struct sockaddr *)&saddr, sizeof(saddr)) != 0)
        //err_sys("bind error");
        oops("bind");

    /*
     * STEP 3: allow incoming calls with Qsize = 1 on socket
     */

    if (listen(sock_id, 1) != 0)
        err_sys("listen error");

    /*
     * main loop: accept(),write(),close()
     */

    while (1) {
        if ((sock_fd = accept(sock_id, NULL, NULL)) < 0)      /* wait for call */
            oops("accept");
        printf("Hah,call got!\n");

        sock_fp = fdopen(sock_fd, "w");
        if (sock_fp == NULL)
            err_sys("fdopen error");

        thetime = time(NULL);

        fprintf(sock_fp, "The time here is ..");
        fprintf(sock_fp, "%s", ctime(&thetime));
        fclose(sock_fp);
    }
}




