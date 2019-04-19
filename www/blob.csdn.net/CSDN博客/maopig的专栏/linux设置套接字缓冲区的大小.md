# linux设置套接字缓冲区的大小 - maopig的专栏 - CSDN博客
2011年11月17日 16:14:22[maopig](https://me.csdn.net/maopig)阅读数：19676
                
系统提供的socket缓冲区大小为8K，你可以将之设置为64K，尤其在传输实时视频时。 
socket发送数据时候先把数据发送到socket缓冲区中，之后接受函数再从缓冲区中取数据，如果发送端特别快的时候，缓冲区很快就被填满（socket默认的是1024×8=8192字节），这时候我们应该根据情况设置缓冲区的大小，可以通过setsockopt函数实现
#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <string.h> 
#include <errno.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <assert.h> 
int main(int argc,char **argv) 
{ 
    int err = -1;        /* 返回值 */ 
    int s = -1;            /* socket描述符 */ 
    int snd_size = 0;   /* 发送缓冲区大小 */ 
    int rcv_size = 0;    /* 接收缓冲区大小 */ 
    socklen_t optlen;    /* 选项值长度 */ 
    /* 
     * 建立一个TCP套接字 
     */ 
    s = socket(PF_INET,SOCK_STREAM,0); 
    if( s == -1){ 
        printf("建立套接字错误\n"); 
        return -1;     
    } 
    /* 
     * 先读取缓冲区设置的情况 
     * 获得原始发送缓冲区大小
     */ 
    optlen = sizeof(snd_size); 
    err = getsockopt(s, SOL_SOCKET, SO_SNDBUF,&snd_size, &optlen); 
    if(err<0){ 
        printf("获取发送缓冲区大小错误\n"); 
    }   
    /* 
     * 打印原始缓冲区设置情况 
     */ 
    /* 
     * 获得原始接收缓冲区大小
     */ 
    optlen = sizeof(rcv_size); 
    err = getsockopt(s, SOL_SOCKET, SO_RCVBUF, &rcv_size, &optlen); 
    if(err<0){ 
        printf("获取接收缓冲区大小错误\n"); 
    } 
   printf(" 发送缓冲区原始大小为: %d 字节\n",snd_size); 
   printf(" 接收缓冲区原始大小为: %d 字节\n",rcv_size); 
    /* 
     * 设置发送缓冲区大小 
     */ 
    snd_size = 10*1024;    /* 发送缓冲区大小为8K */ 
    optlen = sizeof(snd_size); 
    err = setsockopt(s, SOL_SOCKET, SO_SNDBUF, &snd_size, optlen); 
    if(err<0){ 
        printf("设置发送缓冲区大小错误\n"); 
    } 
    /* 
     * 设置接收缓冲区大小 
     */ 
    rcv_size = 10*1024;    /* 接收缓冲区大小为8K */ 
    optlen = sizeof(rcv_size); 
    err = setsockopt(s,SOL_SOCKET,SO_RCVBUF, (char *)&rcv_size, optlen); 
    if(err<0){ 
        printf("设置接收缓冲区大小错误\n"); 
    } 
    /* 
     * 检查上述缓冲区设置的情况 
     * 获得修改后发送缓冲区大小 
     */ 
    optlen = sizeof(snd_size); 
    err = getsockopt(s, SOL_SOCKET, SO_SNDBUF,&snd_size, &optlen); 
    if(err<0){ 
        printf("获取发送缓冲区大小错误\n"); 
    }   
    /* 
     * 获得修改后接收缓冲区大小 
     */ 
    optlen = sizeof(rcv_size); 
    err = getsockopt(s, SOL_SOCKET, SO_RCVBUF,(char *)&rcv_size, &optlen); 
    if(err<0){ 
        printf("获取接收缓冲区大小错误\n"); 
    } 
    /* 
     * 打印结果 
     */ 
    printf(" 发送缓冲区大小为: %d 字节\n",snd_size); 
    printf(" 接收缓冲区大小为: %d 字节\n",rcv_size); 
    close(s); 
    return 0; 
} 
运行后的结果：
发送缓冲区原始大小为: 16384 字节
 接收缓冲区原始大小为: 87380 字节
 发送缓冲区大小为: 20480 字节
 接收缓冲区大小为: 20480 字节
从结果 看出ubuntu系统默认的发送缓冲区大小：16384字节，接收缓冲区87380字节
但是有个问题我设置的接收和发送缓冲区大小为：10*1024=10240字节，但是实际用getoptsock得到却是20480字节加了一倍。改成其他大小也是都是加倍。
这是由linux内核的算法决定的。
