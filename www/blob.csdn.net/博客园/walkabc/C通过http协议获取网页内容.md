# C通过http协议获取网页内容 - walkabc - 博客园
# [C通过http协议获取网页内容](https://www.cnblogs.com/hitfire/articles/4596041.html)
```
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <netinet/in.h>
/*
*C通过http协议获取网页内容
*/
struct sockaddr_in* getSockaddrByName2(char* name)
{
    struct sockaddr_in* addr = (struct sockaddr_in*)malloc(sizeof(struct sockaddr_in));
    struct hostent* host = gethostbyname(name);
    addr->sin_family = host->h_addrtype;
    addr->sin_port = htons(80);
    addr->sin_addr = *((struct in_addr*)(host->h_addr));
    return addr;
}
int main(int argc, char* argv[])
{
    char* domain;
    if (argc > 1) {
        domain = argv[1];
    } else {
        printf("get domain name error !");
        return -1;
    }
    struct sockaddr_in* addr = getSockaddrByName2(domain);
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if(fd < 0) {
        printf("create socket error !\n");
        return -1;
    }
    int code = connect(fd, (struct sockaddr*)addr, sizeof(struct sockaddr));
    if(code != 0) {
        printf("connect error !");
        return -1;
    }
    
    char request[] = "GET / HTTP/1.1\r\nHost: www.danmu.com\r\nConnection: keep-alive\r\nPragma: no-cache\r\nCache-Control: no-cache\r\nAccept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8\r\n\r\n";
    write(fd, request, sizeof(request));
    char buf[256];
    while (1) {
        memset(buf, 0, 256);
        int len = read(fd, buf, 256);
        printf("buf is: %s\n", buf);
        if(len < 256) {
            break;
        }
    }
    return 0;
}
```

