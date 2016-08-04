#include <iostream>
#include <unistd.h>
#include <string>

#define MAXLINE 4096

int main()
{
    int     n;
    int     fd[2];
    pid_t   pid;
    //char    line[MAXLINE];
    std::string  str_i,str_buf;
    std::getline(std::cin, str_i);
    const char *c_in = str_i.c_str();
    char *c_buf = const_cast<char*>(str_buf.c_str());

    if (pipe(fd) < 0)
        std::cerr<<"pipe error"<<std::cout;
    if ((pid = fork()) < 0) {
        std::cerr<<"pipe error"<<std::cout;
    } else if (pid > 0) {
        close(fd[0]);
        write(fd[1], c_in, 50);
    } else {
        close(fd[1]);
        n = read(fd[0], c_buf, MAXLINE);
        write(STDOUT_FILENO, c_buf, n);
    }

    return 0;
}
