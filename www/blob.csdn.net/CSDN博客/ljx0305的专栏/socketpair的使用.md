# socketpair的使用 - ljx0305的专栏 - CSDN博客
2008年07月23日 10:57:00[ljx0305](https://me.csdn.net/ljx0305)阅读数：1476
                 socketpair函数概要如下：
#include <sys/types.h>
#include <sys/socket.h>
int socketpair(int domain, int type, int protocol, int sv[2]);
sys/types.h文件需要用来定义一些C宏常量。sys/socket.h文件必须包含进来定义socketpair函数原型。
socketpair函数需要四个参数。他们是：
套接口的域
套接口类型
使用的协议
指向存储文件描述符的指针
类型参数声明了我们希望创建哪种类型的套接口。socketpair函数的选择如下：
SOCK_STREAM
SOCK_DGRAM
对于socketpair函数，protocol参数必须提供为0。
参数sv[2]是接收代表两个套接口的整数数组。每一个文件描述符代表一个套接口，并且与另一个并没有区别。
如果函数成功，将会返回0值。否则将会返回-1表明创建失败，并且errno来表明特定的错误号。
关于流程。socketpair()函数创建出两个进程，fork()之后这两个进程都会执行主程序中的代码，这个一定要注意！尤其是bind的时候，如果bind两次的话，那就会出错了。一般会在子进程里调用一个带死循环的函数，这样就好了。（这个情况的例子会在综合运用中讲解）
一下给出个简单的例子。
// 建立socket对
       #include <sys/types.h>
       #include <sys/socket.h>
       #include <stdlib.h>
       #include <stdio.h>
       int main ()
       {
         int fd[2];
        int r = socketpair( AF_UNIX, SOCK_STREAM, 0, fd );
        if ( r < 0 ) {
          perror( "socketpair()" );
          exit( 1 );
        }
        if ( fork() ) {
          /* Parent process: echo client */
          int val = 0;
          close( fd[1] );
          while ( 1 ) {
            sleep( 1 );
            ++val;
            printf( "Sending data: %d/n", val );
            write( fd[0], &val, sizeof(val) );
            read( fd[0], &val, sizeof(val) );
            printf( "Data received: %d/n", val );
          }
        }
        else {
          /* Child process: echo server */
          int val;
          close( fd[0] );
          while ( 1 ) {
            read( fd[1], &val, sizeof(val) );
            ++val;
            write( fd[1], &val, sizeof(val) );
          }
        }
      }
在给出一个用sendmsg来传递数据的例子
/***************************************** 
 *
 * Listing 1.2
 *
 * Example performing I/O on s socket pair:
 * 
 * ******************************************/ 
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
int main(int argc,char **argv)
{
    int z;        /* Status return code */
    int s[2];    /* Pair of sockets */
 struct msghdr msg;
    struct iovec iov[1];
 char send_buf[100] = "TEST";
 struct msghdr msgr;
    struct iovec iovr[1];
    char recv_buf[100];
    /*
     * Create a pair of local sockets:
     */
    z = socketpair(AF_LOCAL,SOCK_STREAM,0,s);
    if(z == -1)
    {
        fprintf(stderr,
                "%s:socketpair(AF_LOCAL,SOCK_STREAM,""0)/n",strerror(errno));
        return 1;    /* Failed */
    }
    /*
     * Sendmsg s[1]:
     */
         bzero(&msg, sizeof(msg));
         msg.msg_name = NULL;        /* attention this is a pointer to void* type */
         msg.msg_namelen = 0;
         iov[0].iov_base = send_buf;
         iov[0].iov_len = sizeof(send_buf);
         msg.msg_iov = iov;
         msg.msg_iovlen = 1;
    printf("sendmsg begin./n");
   z = sendmsg( s[1], &msg, 0 );
   if(z == -1 )
   {
    fprintf(stderr,"Sendmsg failed.  errno : %s/n",strerror(errno));
    return -1;
   }
    printf("Sendmsg Success!/n");
    /*
     * Read from socket s[0]:
     */
         bzero(&msg, sizeof(msg));
         msgr.msg_name = NULL;        /* attention this is a pointer to void* type */
         msgr.msg_namelen = 0;
         iovr[0].iov_base = &recv_buf;
         iovr[0].iov_len = sizeof(recv_buf);
         msgr.msg_iov = iovr;
         msgr.msg_iovlen = 1;
         z = recvmsg(  s[0], &msgr, 0);
   if(z == -1 )
   {
    fprintf(stderr,"Recvmsg failed.  errno : %s/n",strerror(errno));
    return -1;
   }
    printf("Recvmsg Success!/n");
 printf("recvmsg : %s/n", recv_buf);
    /*
     * Close the sockets:
     */
    close(s[0]);
    close(s[1]);
    puts("Done");
    return 0;
}
引用:http://blog.csdn.net/sunnyboychina/archive/2007/11/14/1884076.aspx
