#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <time.h>
#include <string.h>
#include <ctype.h>

#define PORTNUM     16000
#define HOSTLEN     256
#define oops(msg)   { perror(msg), exit(1); }

void sanitize(char *);

int main(int argc, char *argv[])
{
    struct sockaddr_in  saddr;
    struct hostent      *hp;
    char    hostname[HOSTLEN];
    int     sock_id, sock_fd;
    FILE    *sock_fpi, *sock_fpo;   /*streams for in and out */
    FILE    *pipe_fp;               /* use popen to run cmd */
    char    dirname[BUFSIZ];        /* from client */
    char    command[BUFSIZ];        /* for popen() */
    int     c;

    /** Step 1 : ask kernel for a  socket **/

    if((sock_id = socket(PF_INET, SOCK_STREAM, 0)) < 0)
        oops("socket");

    /** Step 2 : bind address to socket,Address is host,port **/

    bzero((void *)&saddr, sizeof(saddr));
    //memset(&saddr, 0, sizeof(saddr));

    gethostname(hostname, HOSTLEN);
    hp = gethostbyname(hostname);

    bcopy((void *)hp->h_addr, (void *)&saddr.sin_addr, hp->h_length);
    saddr.sin_port = htons(PORTNUM);
    saddr.sin_family = AF_INET;
    if (bind(sock_id, (struct sockaddr *)&saddr, sizeof(saddr)) != 0)
        oops("bind");

    /** Step 3 : allow incoming calls with Qsize =1 on socket **/

    if (listen(sock_id, 1) != 0)
        oops("listen");

    /** main loop : accept(), write(), close() **/

    while(1) {
        if ((sock_fd = accept(sock_id, NULL, NULL)) < 0)
            oops("accept");

        printf("Aha, success accepted\n");
        
        //if ((sock_fpo = fdopen(sock_fd, "w")) == NULL)
        //    oops("fdopen");
        
        //fprintf(sock_fpo, "Connect it ?..");
        
        
        /* open reading direction as buffered stream */
        if ((sock_fpi = fdopen(sock_fd, "r")) == NULL)
            oops("fdopen reading");
        
        if (fgets(dirname, BUFSIZ - 5, sock_fpi) == NULL)
            oops("reading dirname");
        sanitize(dirname);

        /* open writing direction as buffered stream */
        if ((sock_fpo = fdopen(sock_fd, "w")) == NULL)
            oops("fdopen writing");

        sprintf(command, "ls %s", dirname);
        fprintf(sock_fpo, "command is :%s\n", command);
 
        if ((pipe_fp = popen(command,"r")) == NULL)
            oops("popen");

        /* transfer data from ls to socket */
        while ((c= getc(pipe_fp)) != EOF)
            putc(c, sock_fpo);
        pclose(pipe_fp);
        fclose(sock_fpo);
        fclose(sock_fpi);
    }
    return 0;
}

void sanitize(char *str)
{
    /*
     * it would be very bad if someone passed us an dirname like
     * "; rm *" and we naively created a command "ls ; rm *"
     * so we remove everything but slashes and alphanumerics
     */
    
    char *src, *dest;

    for (src = dest = str; *src; src++)
        if (*src == '/' || isalnum(*src))
            *dest++ = *src;
    *dest = '\0';
}
