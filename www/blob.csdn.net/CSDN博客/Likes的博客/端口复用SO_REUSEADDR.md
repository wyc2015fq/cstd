# 端口复用SO_REUSEADDR - Likes的博客 - CSDN博客
2018年10月23日 11:39:32[Lailikes](https://me.csdn.net/songchuwang1868)阅读数：184
版权声明：本博客文章，大多是本人整理编写，或在网络中收集，转载请注明出处！    https://blog.csdn.net/tennysonsky/article/details/44062173
在《绑定( bind )端口需要注意的问题》提到：一个网络应用程序只能绑定一个端口( 一个套接字只能绑定一个端口 )。
实际上，默认的情况下，如果一个网络应用程序的一个套接字 绑定了一个端口( 占用了 8000 )，这时候，别的套接字就无法使用这个端口( 8000 ), 验证例子如下：
```cpp
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
 
int main(int argc, char *argv[])
{
    int sockfd_one;
    int err_log;
    sockfd_one = socket(AF_INET, SOCK_DGRAM, 0); //创建UDP套接字one
    if(sockfd_one < 0)
    {
    perror("sockfd_one");
    exit(-1);
    }
 
    // 设置本地网络信息
    struct sockaddr_in my_addr;
    bzero(&my_addr, sizeof(my_addr));
    my_addr.sin_family = AF_INET;
    my_addr.sin_port = htons(8000);        // 端口为8000
    my_addr.sin_addr.s_addr = htonl(INADDR_ANY);
 
    // 绑定，端口为8000
    err_log = bind(sockfd_one, (struct sockaddr*)&my_addr, sizeof(my_addr));
    if(err_log != 0)
    {
        perror("bind sockfd_one");
        close(sockfd_one);        
        exit(-1);
    }
 
    int sockfd_two;
    sockfd_two = socket(AF_INET, SOCK_DGRAM, 0);  //创建UDP套接字two
    if(sockfd_two < 0)
    {
        perror("sockfd_two");
        exit(-1);
    }
 
    // 新套接字sockfd_two，继续绑定8000端口，绑定失败
    // 因为8000端口已被占用，默认情况下，端口没有释放，无法绑定
    err_log = bind(sockfd_two, (struct sockaddr*)&my_addr, sizeof(my_addr));
    if(err_log != 0)
    {
        perror("bind sockfd_two");
        close(sockfd_two);        
        exit(-1);
    }
 
    close(sockfd_one);
    close(sockfd_two);
 
    return 0;
}
```
程序编译运行后结果如下：
那如何让sockfd_one, sockfd_two两个套接字都能成功绑定8000端口呢？这时候就需要要到端口复用了。端口复用允许在一个应用程序可以把 n 个套接字绑在一个端口上而不出错。
设置socket的SO_REUSEADDR选项，即可实现端口复用：
```cpp
int opt = 1;
// sockfd为需要端口复用的套接字
setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (const void *)&opt, sizeof(opt));
```
SO_REUSEADDR可以用在以下四种情况下。 (摘自《Unix网络编程》卷一，即UNPv1)
1、当有一个有相同本地地址和端口的socket1处于TIME_WAIT状态时，而你启动的程序的socket2要占用该地址和端口，你的程序就要用到该选项。
2、SO_REUSEADDR允许同一port上启动同一服务器的多个实例(多个进程)。但每个实例绑定的IP地址是不能相同的。在有多块网卡或用IP Alias技术的机器可以测试这种情况。
3、SO_REUSEADDR允许单个进程绑定相同的端口到多个socket上，但每个socket绑定的ip地址不同。这和2很相似，区别请看UNPv1。
4、SO_REUSEADDR允许完全相同的地址和端口的重复绑定。但这只用于UDP的多播，不用于TCP。
需要注意的是，设置端口复用函数要在绑定之前调用，而且只要绑定到同一个端口的所有套接字都得设置复用：
```cpp
// sockfd_one, sockfd_two都要设置端口复用
// 在sockfd_one绑定bind之前，设置其端口复用
int opt = 1;
setsockopt( sockfd_one, SOL_SOCKET,SO_REUSEADDR, (const void *)&opt, sizeof(opt) );
err_log = bind(sockfd_one, (struct sockaddr*)&my_addr, sizeof(my_addr));
 
// 在sockfd_two绑定bind之前，设置其端口复用
opt = 1;
setsockopt( sockfd_two, SOL_SOCKET,SO_REUSEADDR,(const void *)&opt, sizeof(opt) );
err_log = bind(sockfd_two, (struct sockaddr*)&my_addr, sizeof(my_addr));
```
端口复用完整代码如下：
```cpp
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
 
int main(int argc, char *argv[])
{
    int sockfd_one;
    int err_log;
    sockfd_one = socket(AF_INET, SOCK_DGRAM, 0); //创建UDP套接字one
    if(sockfd_one < 0)
    {
    perror("sockfd_one");
    exit(-1);
    }
 
    // 设置本地网络信息
    struct sockaddr_in my_addr;
    bzero(&my_addr, sizeof(my_addr));
    my_addr.sin_family = AF_INET;
    my_addr.sin_port = htons(8000);        // 端口为8000
    my_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    
    // 在sockfd_one绑定bind之前，设置其端口复用
    int opt = 1;
    setsockopt( sockfd_one, SOL_SOCKET,SO_REUSEADDR, 
                    (const void *)&opt, sizeof(opt) );
 
    // 绑定，端口为8000
    err_log = bind(sockfd_one, (struct sockaddr*)&my_addr, sizeof(my_addr));
    if(err_log != 0)
    {
        perror("bind sockfd_one");
        close(sockfd_one);        
        exit(-1);
    }
 
    int sockfd_two;
    sockfd_two = socket(AF_INET, SOCK_DGRAM, 0);  //创建UDP套接字two
    if(sockfd_two < 0)
    {
        perror("sockfd_two");
        exit(-1);
    }
 
    // 在sockfd_two绑定bind之前，设置其端口复用
    opt = 1;
    setsockopt( sockfd_two, SOL_SOCKET,SO_REUSEADDR, 
                    (const void *)&opt, sizeof(opt) );
    
    // 新套接字sockfd_two，继续绑定8000端口，成功
    err_log = bind(sockfd_two, (struct sockaddr*)&my_addr, sizeof(my_addr));
    if(err_log != 0)
    {
        perror("bind sockfd_two");
        close(sockfd_two);        
        exit(-1);
    }
 
    close(sockfd_one);
    close(sockfd_two);
 
    return 0;
}
```
端口复用允许在一个应用程序可以把 n 个套接字绑在一个端口上而不出错。同时，这 n 个套接字发送信息都正常，没有问题。但是，这些套接字并不是所有都能读取信息，只有最后一个套接字会正常接收数据。
下面，我们在之前的代码上，添加两个线程，分别负责接收sockfd_one，sockfd_two的信息：
```cpp
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
 
// 线程1的回调函数
void *recv_one(void *arg)
{
    printf("===========recv_one==============\n");
    int sockfd = (int )arg;
    while(1){
        int recv_len;
        char recv_buf[512] = "";
        struct sockaddr_in client_addr;
        char cli_ip[INET_ADDRSTRLEN] = "";//INET_ADDRSTRLEN=16
        socklen_t cliaddr_len = sizeof(client_addr);
        
        recv_len = recvfrom(sockfd, recv_buf, sizeof(recv_buf), 0, (struct sockaddr*)&client_addr, &cliaddr_len);
        inet_ntop(AF_INET, &client_addr.sin_addr, cli_ip, INET_ADDRSTRLEN);
        printf("\nip:%s ,port:%d\n",cli_ip, ntohs(client_addr.sin_port));
        printf("sockfd_one =========== data(%d):%s\n",recv_len,recv_buf);
    
    }
 
    return NULL;
}
 
// 线程2的回调函数
void *recv_two(void *arg)
{
    printf("+++++++++recv_two++++++++++++++\n");
    int sockfd = (int )arg;
    while(1){
        int recv_len;
        char recv_buf[512] = "";
        struct sockaddr_in client_addr;
        char cli_ip[INET_ADDRSTRLEN] = "";//INET_ADDRSTRLEN=16
        socklen_t cliaddr_len = sizeof(client_addr);
        
        recv_len = recvfrom(sockfd, recv_buf, sizeof(recv_buf), 0, (struct sockaddr*)&client_addr, &cliaddr_len);
        inet_ntop(AF_INET, &client_addr.sin_addr, cli_ip, INET_ADDRSTRLEN);
        printf("\nip:%s ,port:%d\n",cli_ip, ntohs(client_addr.sin_port));
        printf("sockfd_two @@@@@@@@@@@@@@@ data(%d):%s\n",recv_len,recv_buf);
    
    }
 
    return NULL;
}
 
int main(int argc, char *argv[])
{
    int err_log;
    
    /////////////////////////sockfd_one
    int sockfd_one;
    sockfd_one = socket(AF_INET, SOCK_DGRAM, 0); //创建UDP套接字one
    if(sockfd_one < 0)
    {
    perror("sockfd_one");
    exit(-1);
    }
 
    // 设置本地网络信息
    struct sockaddr_in my_addr;
    bzero(&my_addr, sizeof(my_addr));
    my_addr.sin_family = AF_INET;
    my_addr.sin_port = htons(8000);        // 端口为8000
    my_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    
    // 在sockfd_one绑定bind之前，设置其端口复用
    int opt = 1;
    setsockopt( sockfd_one, SOL_SOCKET,SO_REUSEADDR, 
                    (const void *)&opt, sizeof(opt) );
 
    // 绑定，端口为8000
    err_log = bind(sockfd_one, (struct sockaddr*)&my_addr, sizeof(my_addr));
    if(err_log != 0)
    {
        perror("bind sockfd_one");
        close(sockfd_one);        
        exit(-1);
    }
    
    //接收信息线程1
    pthread_t tid_one;
    pthread_create(&tid_one, NULL, recv_one, (void *)sockfd_one);
    
    /////////////////////////sockfd_two
    int sockfd_two;
    sockfd_two = socket(AF_INET, SOCK_DGRAM, 0);  //创建UDP套接字two
    if(sockfd_two < 0)
    {
        perror("sockfd_two");
        exit(-1);
    }
 
    // 在sockfd_two绑定bind之前，设置其端口复用
    opt = 1;
    setsockopt( sockfd_two, SOL_SOCKET,SO_REUSEADDR, 
                    (const void *)&opt, sizeof(opt) );
    
    // 新套接字sockfd_two，继续绑定8000端口，成功
    err_log = bind(sockfd_two, (struct sockaddr*)&my_addr, sizeof(my_addr));
    if(err_log != 0)
    {
        perror("bind sockfd_two");
        close(sockfd_two);        
        exit(-1);
    }
    //接收信息线程2
    pthread_t tid_two;
    pthread_create(&tid_two, NULL, recv_two, (void *)sockfd_two);
    
    
    while(1){    // 让程序阻塞在这，不结束
        NULL;
    }
 
    close(sockfd_one);
    close(sockfd_two);
 
    return 0;
}
```
接着，通过网络调试助手给这个服务器发送数据，结果显示，只有最后一个套接字sockfd_two会正常接收数据：
![](https://img-blog.csdn.net/20181023113628685?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NvbmdjaHV3YW5nMTg2OA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
我们上面的用法，实际上没有太大的意义。端口复用最常用的用途应该是防止服务器重启时之前绑定的端口还未释放或者程序突然退出而系统没有释放端口。这种情况下如果设定了端口复用，则新启动的服务器进程可以直接绑定端口。如果没有设定端口复用，绑定会失败，提示ADDR已经在使用中——那只好等等再重试了，麻烦！
源代码下载请点此处。
--------------------- 
作者：Mike__Jiang 
来源：CSDN 
原文：https://blog.csdn.net/tennysonsky/article/details/44062173 
版权声明：本文为博主原创文章，转载请附上博文链接！
