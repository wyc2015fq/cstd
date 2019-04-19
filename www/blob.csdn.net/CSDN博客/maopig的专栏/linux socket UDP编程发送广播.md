# linux socket UDP编程发送广播 - maopig的专栏 - CSDN博客
2012年03月08日 10:55:50[maopig](https://me.csdn.net/maopig)阅读数：14769
#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<string.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<sys/socket.h>
#include<sys/wait.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#include <arpa/inet.h>
#include<netdb.h>
#define PORT 7773
#define MAXDATASIZE 256
int main(int argc,char *argv[])
{
    int socket_fd;
    struct sockaddr_in my_addr,user_addr;
    char buf[MAXDATASIZE];
    int so_broadcast=1;
    socklen_t size;
    char my_ip[12];
    my_addr.sin_family=AF_INET;
    my_addr.sin_port=htons(PORT);
    my_addr.sin_addr.s_addr=inet_addr("255.255.255.255");
    bzero(&(my_addr.sin_zero),8);
    user_addr.sin_family=AF_INET;
    user_addr.sin_port=htons(PORT);
    user_addr.sin_addr.s_addr=htonl(INADDR_ANY);
    bzero(&(user_addr.sin_zero),8);
    if((socket_fd=(socket(AF_INET,SOCK_DGRAM,0)))==-1) {
        perror("socket");
        exit(1);
    }
   setsockopt(socket_fd,SOL_SOCKET,SO_BROADCAST,&so_broadcast,sizeof(so_broadcast));
    if((bind(socket_fd,(struct sockaddr *)&user_addr,
                        sizeof(struct sockaddr)))==-1) {
        perror("bind");
        exit(1);
    }
    strcpy(buf,"Hello,I'm on line!");
    sendto(socket_fd,buf,strlen(buf),0,(struct sockaddr *)&my_addr,sizeof(my_addr));
    size=sizeof(user_addr);
    recvfrom(socket_fd,buf,MAXDATASIZE,0,(struct sockaddr *)&user_addr,&size);
    strcpy(my_ip,inet_ntoa(user_addr.sin_addr));
    printf("my_ip:%sn",inet_ntoa(user_addr.sin_addr));
    while(1) {
        bzero(buf, sizeof(buf));          
        size=sizeof(user_addr);
        recvfrom(socket_fd,buf,MAXDATASIZE,0,(struct sockaddr *)&user_addr,&size);
        printf("%s:%sn",inet_ntoa(user_addr.sin_addr),buf);
        bzero(buf, sizeof(buf));
        if(strcmp(buf,"I'm off line,bye!")==0)
            strcpy(buf,"ok,I know,bye!");
        else {
            strcpy(buf,"send,Hello,I get you!");
                sleep(1);
        }    
        if((sendto(socket_fd,buf,strlen(buf),0,
                (struct sockaddr *)&user_addr,sizeof(user_addr)))==-1)
            perror("sendto");
    }
    return 0;
}
执行：
gcc -o udp udp.c
./udp
### [UDP协议发送、接收广播包](http://blog.csdn.net/sky1415/article/details/5528420)
# 网络通信基础
如果网络中两个主机上的应用程序要相互通信，其一要知道彼此的IP，其二要知道程序可监听的端口。因为同一主机上的程序使用网络是通过端口号来区分的。
# UDP Socket的使用过程：
1.       初始化网络库
2.       创建SOCK_DGRAM类型的Socket。
3.       绑定套接字。
4.       发送、接收数据。
5.       销毁套接字。
6.       释放网络库。
# 广播数据包的原理：
专门用于同时向网络中所有工作站进行发送的一个地址叫做广播地址。在使用TCP/IP 协议的网络中，主机标识段host ID 为全1 的IP 地址为广播地址。如果你的IP为：192.168.1.39，子网掩码为：255.255.255.0，则广播地址为：192.168.1.255；如果IP为192.168.1.39，子网掩码为：255.255.255.192，则广播地址为：192.168.1.63。
如果只想在本网络内广播数据，只要向广播地址发送数据包即可，这种数据包可以被路由，它会经由路由器到达本网段内的所有主机，此种广播也叫直接广播；如果想在整个网络中广播数据，要向255.255.255.255发送数据包，这种数据包不会被路由，它只能到达本物理网络中的所有主机，此种广播叫有限广播。
# 使用UDP协议发送、接收广播包的过程。
假如我们要向192.168.0.X，子网掩码为：255.255.255.0的子网中发送广播包。
其步骤如下：
1.       初始化Winsock库。
2.       创建SOCK_DIRAM类型的Socket。
3.       设置Socket的属性允许其广播。
4.       发送数据包到192.168.0.255
5.       接收自己广播的广播包。
6.       关闭Socket
7.       释放网络库。
注意事项如下：
1.       接收方一定要知道广播方的口号，然后绑定此端口号才能正确接收。
2.       接收方的Socket不需要设置成广播属性。
3.       绑定的IP不可以使用“127.0.0.1”，可以使用真实IP地址或者INADDR_ANY。否则接收失败。
