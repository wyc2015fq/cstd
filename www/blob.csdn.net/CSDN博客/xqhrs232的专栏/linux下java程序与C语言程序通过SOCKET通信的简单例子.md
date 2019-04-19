# linux下java程序与C语言程序通过SOCKET通信的简单例子 - xqhrs232的专栏 - CSDN博客
2011年12月06日 15:18:49[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：802标签：[socket																[java																[语言																[linux																[c																[服务器](https://so.csdn.net/so/search/s.do?q=服务器&t=blog)](https://so.csdn.net/so/search/s.do?q=c&t=blog)](https://so.csdn.net/so/search/s.do?q=linux&t=blog)](https://so.csdn.net/so/search/s.do?q=语言&t=blog)](https://so.csdn.net/so/search/s.do?q=java&t=blog)](https://so.csdn.net/so/search/s.do?q=socket&t=blog)
个人分类：[Linux/Ubuntu/Fedora](https://blog.csdn.net/xqhrs232/article/category/906931)
原文地址::[http://blog.csdn.net/guan506713512/article/details/6867145](http://blog.csdn.net/guan506713512/article/details/6867145)
本例中C语言端作为服务器，java端作为客户端
代码如下：
/****************** **server program** *****************/
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <stdlib.h>
#include <sys/un.h>
#include <pthread.h>
#include <arpa/inet.h>
int sockfd,newfd;
void *read_socket();
int main()
{
    int ret;
    pthread_t read_tid,write_tid;
    struct sockaddr_in server_addr;
    server_addr.sin_family=AF_INET;/*设置域为IPV4*/
    server_addr.sin_addr.s_addr=INADDR_ANY;/*绑定到 INADDR_ANY 地址*/
    server_addr.sin_port=htons(5678);/*通信端口号为5678，注意这里必须要用htons函数处理一下，不能直接写5678，否则可能会连不上*/
    sockfd=socket(AF_INET,SOCK_STREAM,0);
    if (sockfd<0)
    {
        printf("调用socket函数建立socket描述符出错！\n");
         exit(1);
    }
    printf("调用socket函数建立socket描述符成功！\n");
    ret=bind(sockfd,(struct sockaddr *)(&server_addr),sizeof(server_addr));
    perror("server");
    if (ret<0)
    {
        printf("调用bind函数绑定套接字与地址出错！\n");
         exit(2);
    }
    printf("调用bind函数绑定套接字与地址成功！\n");
    ret=listen(sockfd,4);
    if (ret<0)
    {
        printf("调用listen函数出错，无法宣告服务器已经可以接受连接！\n");
         exit(3);
    }
    printf("调用listen函数成功，宣告服务器已经可以接受连接请求！\n");
    newfd=accept(sockfd,NULL,NULL);/*newfd连接到调用connect的客户端*/
    if (newfd<0)
    {
        printf("调用accept函数出错，无法接受连接请求，建立连接失败！\n");
         exit(4);
    }
    printf("调用accept函数成功，服务器与客户端建立连接成功！\n");
    /** 服务器端建立一个线程，负责从socket读取数据，当然此处并不一定非要建立一个新线程，直接在原线程中写也可以 **/
    pthread_create(&read_tid,NULL,read_socket,NULL);
    /********************* 令原线程睡眠 ************************/
    while(1)
    {
        sleep(10000);
    }
}
/****************** 从socket读取数据 ********************/
void *read_socket()
{
    int recv_num,recv_num_total=0;
    char recv_buf[50];
    while (1)
    {
        memset(recv_buf,0,sizeof(recv_buf));/*清空一下recv_buf缓存区*/
recv_num=recv(newfd,recv_buf,26,0);
        if (recv_num<0)
            printf("服务器端：调用recv接收失败！\n");
        else if(recv_num>0)
        {
            recv_num_total+=recv_num;
            printf("服务器端：调用recv接收成功！本次接收到%d个字节，内容为：\"%s\"。共收到%d个字节的数据。\n",recv_num,recv_buf,recv_num_total);
            sync();
        }
        else/*收到数据为0，表明服务器与客户端的连接已经中断*/
        {
            printf("服务器端：与客户端的连接已中断，当前共收到%d个字节的数据。服务器将再次等待客户端的连接。\n",recv_num_total);
            newfd=accept(sockfd,NULL,NULL);/*当客户端退出后，再次开始接收客户端的连接*/
        }
        sleep(1);
    }
}
/****************** **Client program** *****************/
import java.net.*;
import java.io.*;
public class Client {
static Socket server;
public static void main(String[] args) throws Exception {
   server = new Socket(InetAddress.getLocalHost(), 5678);
   BufferedReader in = new BufferedReader(new InputStreamReader(server.getInputStream()));
   PrintWriter out = new PrintWriter(server.getOutputStream());
   BufferedReader wt = new BufferedReader(new InputStreamReader(System.in));
   while (true) {
    String str = wt.readLine();
    out.println(str);
    out.flush();
    if (str.equals("end")) {
     break;
    }
   }
   server.close();
}
}
把服务器端程序存为server.c，客户端程序存为Client.java
编译：
gcc -o server -lpthread server.c
javac Client.java
在两个终端下分别运行服务端和客户端：
./server
java Client
///////////////////////// 客户端运行结果 ////////////////////////////////
hello,this is a socket test!（自己输入一些文字）
good
///////////////////////// 服务端运行结果 ////////////////////////////////
调用socket函数建立socket描述符成功！
server: Success
调用bind函数绑定套接字与地址成功！
调用listen函数成功，宣告服务器已经可以接受连接请求！
调用accept函数成功，服务器与客户端建立连接成功！
服务器端：调用recv接收成功！本次接收到26个字节，内容为："hello,this is a socket tes"。共收到26个字节的数据。
服务器端：调用recv接收成功！本次接收到3个字节，内容为："t!
"。共收到29个字节的数据。
服务器端：调用recv接收成功！本次接收到5个字节，内容为："good
"。共收到34个字节的数据。
＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
可见通信是成功的，其中客户端发送的第一条消息超过了服务端程序中规定每次读取的字节数（26个字节，在这一句里recv_num=recv(newfd,recv_buf,**26**,0);设定的），服务端分两次接收了该消息。
另外，客户端输入的换行符也被接收了进来，因此可以看到蓝色字体的这一条返回结果中，t!后面有一个换行，同时显示本次接收到了3个字符。
为了您的安全，请只打开来源可靠的网址
[打开网站](http://hi.baidu.com/mcu99/blog/item/dcb91a5c2bcc384bfaf2c0f5.html)[取消](http://apps.hi.baidu.com/share/detail/30686013#)
来自: [http://hi.baidu.com/mcu99/blog/item/dcb91a5c2bcc384bfaf2c0f5.html](http://hi.baidu.com/mcu99/blog/item/dcb91a5c2bcc384bfaf2c0f5.html)
