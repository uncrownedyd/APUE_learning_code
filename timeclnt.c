#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <myerr.h>

#define oops(msg)   {perror(msg); exit(1); }

int main(int argc, char *argv[])
{
    struct sockaddr_in  servadd;
    struct hostent      *hp;
    int    sock_id;
    char   message[BUFSIZ];
    int    messlen;

    /*
     * Step 1 :Get a socket
     */

    if ((sock_id = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        oops("socket");

    /*
     * Step 2 :connect to server
     *      need to build address (host, port) of server first
     */

    bzero(&servadd, sizeof(servadd));

    hp = gethostbyname(argv[1]);
    if (hp == NULL)
        oops(argv[1]);

    bcopy(hp->h_addr, (struct sockaddr *)&servadd.sin_addr,
        hp->h_length);

    servadd.sin_port = htons(atoi(argv[2]));    /* fill in port number */

    servadd.sin_family = AF_INET;               /* fill in socket type */

    if (connect(sock_id, (struct sockaddr *)&servadd,
        sizeof(servadd)) != 0)
        oops("connect");

    /*
     * Step 3 : transfer data from server,then hangup
     */

    if ((messlen = read(sock_id, message, BUFSIZ)) < 0)
        oops("read");

    if (write(1, message, messlen) != messlen)
        oops("write");
    close(sock_id);
    return 0;
}
