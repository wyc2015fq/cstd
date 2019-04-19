# 简单的Echo服务器 - walkabc - 博客园
# [简单的Echo服务器](https://www.cnblogs.com/hitfire/articles/4598174.html)
```
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <netinet/in.h>
int main(int argc, char* argv[])
{
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if(fd < 0) {
        printf("create socket error !");
        return -1;
    }
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(9090);
    struct in_addr inp;
    inet_aton("127.0.0.1", &inp);
    addr.sin_addr = inp;
    if(bind(fd, (struct sockaddr*)&addr, sizeof(struct sockaddr)) != 0) {
        printf("bind socket error ！");
        return -1;
    }
    if(listen(fd, 10) != 0) {
        printf("listen socket error !");
        return -1;
    }
    char welcome[] = "welcome link to server !\n";
    while(1) {
        struct sockaddr clientAddr;
        int clientLength;
        memset(&clientAddr, 0, sizeof(clientAddr));
        int client = accept(fd, &clientAddr, &clientLength);
        if(client < 0) {
            printf("accept socket error !");
            break;
        }
        write(client, welcome, sizeof(welcome));
        close(client);
    }
    return -1;
}
```

