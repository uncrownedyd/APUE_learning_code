#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>

#define oops(msg) { perror(msg); exit(1); }

int main(int argc, char *argv[])
{
    struct sockaddr_in  servadd;    
    struct hostent      *hp;
    int    sock_id;
    char   buffer[BUFSIZ];
    int    n_read;                  /* length of message */

    if (argc != 4)    
        oops("address error");
    
    /*
     * Step 1 : get a socket 
     */

    if ((sock_id = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        oops("socket");

    /*
     * Step 2 : connect to server 
     */

    bzero( &servadd, sizeof(servadd));

    if((hp = gethostbyname(argv[1])) == NULL)       /* lookup host's ip# */
        oops(argv[1]);
    bcopy(hp->h_addr, (struct sockaddr *)&servadd.sin_addr,
                hp->h_length);

    servadd.sin_port = htons(atoi(argv[2]));          /* fill in port number */
    servadd.sin_family = AF_INET;               /* fill in socket type */

    if (connect(sock_id, (struct sockaddr *)&servadd, sizeof(servadd)) != 0)
        oops("connect");

    /*
     * Step 3 : send directory name, then reaad back result
     */
    if (write(sock_id, argv[3],strlen(argv[3])) == -1)
        oops("write");
    if (write(sock_id, "\n", 1) == -1)
        oops("write");

    while ((n_read = read(sock_id, buffer, BUFSIZ)) > 0)
        if (write(1, buffer, n_read) == -1)
            oops("write");
    close(sock_id);
    return 0;

}
