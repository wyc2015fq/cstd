# TCP的socket连接示例 - ljx0305的专栏 - CSDN博客
2010年02月05日 15:26:00[ljx0305](https://me.csdn.net/ljx0305)阅读数：623
用C实现的TCP socket连接/读/写操作。采用fcntl设置非阻塞式连接以实现connect超时处理；采用select方法来设置socket读写超时。此示例可被编译运行于Windows/unix系统。
源文件connector.c
原来的代码在windows下编译不通过，今天qzj问起才发现。因为加了异步的处理，没有对这部分代码进行兼容性处理。本着做学问一丝不苟嘀精神，重新修改了一下源代码。以下代码在VC++6和linux下编译执行通过 :)
/*
 * on Unix:
 *    cc -c connector.c
 *    cc -o connector connector.o
 *
 * on Windows NT:
 *    open connector.c in Visual Studio
 *    press 'F7' to link -- a project to be created
 *    add wsock32.lib to the link section under project setting
 *    press 'F7' again
 *
 * running:
 *    type 'connector' for usage
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <errno.h>
#include <fcntl.h>
#ifdef WIN32
#include <winsock2.h>
#else
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#endif
#ifndef INADDR_NONE
#define INADDR_NONE     0xffffffff
#endif
#define MAX_STRING_LEN  1024
#define BUFSIZE  2048
#ifndef WIN32
#define SOCKET int
#else
#define errno WSAGetLastError()
#define close(a) closesocket(a)
#define write(a, b, c) send(a, b, c, 0)
#define read(a, b, c) recv(a, b, c, 0)
#endif
char buf[BUFSIZE];
static char i_host[MAX_STRING_LEN];  /* site name */
static char i_port[MAX_STRING_LEN];  /* port number */
void err_doit(int errnoflag, const char *fmt, va_list ap);
void err_quit(const char *fmt, ...);
int tcp_connect(const char *host, const unsigned short port);
void print_usage();
//xnet_select x defines
#define READ_STATUS  0
#define WRITE_STATUS 1
#define EXCPT_STATUS 2
/*
s    - SOCKET
sec  - timeout seconds
usec - timeout microseconds
x    - select status
*/
SOCKET xnet_select(SOCKET s, int sec, int usec, short x)
{
 int st = errno;
 struct timeval to;
 fd_set fs;
 to.tv_sec = sec;
 to.tv_usec = usec;
 FD_ZERO(&fs);
 FD_SET(s, &fs);
 switch(x){
  case READ_STATUS:
  st = select(s+1, &fs, 0, 0, &to);
  break;
  case WRITE_STATUS:
  st = select(s+1, 0, &fs, 0, &to);
  break;
  case EXCPT_STATUS:
  st = select(s+1, 0, 0, &fs, &to);
  break;
 }
 return(st);
}
int tcp_connect(const char *host, const unsigned short port)
{
    unsigned long non_blocking = 1;
    unsigned long blocking = 0;
    int ret = 0;
    char * transport = "tcp";
    struct hostent      *phe;   /* pointer to host information entry    */
    struct protoent *ppe;       /* pointer to protocol information entry*/
    struct sockaddr_in sin;     /* an Internet endpoint address  */
    SOCKET s;                    /* socket descriptor and socket type    */
    int error;
#ifdef WIN32
    {
        WORD wVersionRequested;
        WSADATA wsaData;
        int err;
 
        wVersionRequested = MAKEWORD( 2, 0 );
 
        err = WSAStartup( wVersionRequested, &wsaData );
        if ( err != 0 ) {
            /* Tell the user that we couldn't find a usable */
            /* WinSock DLL.                               */
            printf("can't initialize socket library/n");
            exit(0);
        }
    }
#endif    
    
    memset(&sin, 0, sizeof(sin));
    sin.sin_family = AF_INET;
    
    if ((sin.sin_port = htons(port)) == 0)
        err_quit("invalid port /"%d/"/n", port);
    
    /* Map host name to IP address, allowing for dotted decimal */
    if ( phe = gethostbyname(host) )
        memcpy(&sin.sin_addr, phe->h_addr, phe->h_length);
    else if ( (sin.sin_addr.s_addr = inet_addr(host)) == INADDR_NONE )
        err_quit("can't get /"%s/" host entry/n", host);
    
    /* Map transport protocol name to protocol number */
    if ( (ppe = getprotobyname(transport)) == 0)
        err_quit("can't get /"%s/" protocol entry/n", transport);
    
    /* Allocate a socket */
    s = socket(PF_INET, SOCK_STREAM, ppe->p_proto);
    if (s < 0)
        err_quit("can't create socket: %s/n", strerror(errno));
    
    /* Connect the socket with timeout */
#ifdef WIN32
    ioctlsocket(s,FIONBIO,&non_blocking);
#else
    ioctl(s,FIONBIO,&non_blocking);
#endif
    //fcntl(s,F_SETFL, O_NONBLOCK);
    if (connect(s, (struct sockaddr *)&sin, sizeof(sin)) == -1){
        struct timeval tv; 
        fd_set writefds;
        // 设置连接超时时间
        tv.tv_sec = 10; // 秒数
        tv.tv_usec = 0; // 毫秒
        FD_ZERO(&writefds); 
        FD_SET(s, &writefds); 
        if(select(s+1,NULL,&writefds,NULL,&tv) != 0){ 
            if(FD_ISSET(s,&writefds)){
                int len=sizeof(error); 
                //下面的一句一定要，主要针对防火墙 
                if(getsockopt(s, SOL_SOCKET, SO_ERROR,  (char *)&error, &len) < 0)
                    goto error_ret; 
                if(error != 0) 
                    goto error_ret; 
            }
            else
                goto error_ret; //timeout or error happen 
        }
        else goto error_ret; ; 
#ifdef WIN32
        ioctlsocket(s,FIONBIO,&blocking);
#else
        ioctl(s,FIONBIO,&blocking);
#endif
    }
    else{
error_ret:
        close(s);
        err_quit("can't connect to %s:%d/n", host, port);
    }
    return s;
}
void err_doit(int errnoflag, const char *fmt, va_list ap)
{
    int errno_save;
    char buf[MAX_STRING_LEN];
    errno_save = errno; 
    vsprintf(buf, fmt, ap);
    if (errnoflag)
        sprintf(buf + strlen(buf), ": %s", strerror(errno_save));
    strcat(buf, "/n");
    fflush(stdout);
    fputs(buf, stderr);
    fflush(NULL);
    return;
}
/* Print a message and terminate. */
void err_quit(const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    err_doit(0, fmt, ap);
    va_end(ap);
    exit(1);
}
#ifdef WIN32
char *optarg;
char getopt(int c, char *v[], char *opts)
{
    static int now = 1;
    char *p;
    if (now >= c) return EOF;
    if (v[now][0] == '-' && (p = strchr(opts, v[now][1]))) {
        optarg = v[now+1];
        now +=2;
        return *p;
    }
    return EOF;
}
#else
extern char *optarg;
#endif
#define required(a) if (!a) { return -1; }
int init(int argc, char *argv[])
{
    char c;
    //int i,optlen;
    //int slashcnt;
    i_host[0]  =  '/0';
    i_port[0]  =  '/0';
    while ((c = getopt(argc, argv, "h:p:?")) != EOF) {
        if (c == '?')
            return -1;
        switch (c) { 
        case 'h':
            required(optarg);
            strcpy(i_host, optarg);
            break;
        case 'p':
            required(optarg);
            strcpy(i_port, optarg);
            break;
        default:
            return -1;
        }
    }
    /* 
     * there is no default value for hostname, port number, 
     * password or uri
     */
    if (i_host[0] == '/0' || i_port[0] == '/0')
        return -1;
    return 1;
}
void print_usage()
{
    char *usage[] =
    {
        "Usage:",
        "    -h    host name",
        "    -p    port",
        "example:",
        "    -h 127.0.0.1 -p 4001",
    };   
    int i;
    for (i = 0; i < sizeof(usage) / sizeof(char*); i++)
        printf("%s/n", usage[i]);
    
    return;
}
int main(int argc, char *argv[])
{
    SOCKET fd;
    int n;
    /* parse command line etc ... */
    if (init(argc, argv) < 0) {
        print_usage();
        exit(1);
    }
    buf[0] = '/0';
    /* pack the info into the buffer */     
    strcpy(buf, "HelloWorld");
    /* make connection to the server */
    fd = tcp_connect(i_host, (unsigned short)atoi(i_port));
    if(xnet_select(fd, 0, 500, WRITE_STATUS)>0){
        /* send off the message */
        write(fd, buf, strlen(buf));
    }
    else{
        err_quit("Socket I/O Write Timeout %s:%s/n", i_host, i_port);
    }
    if(xnet_select(fd, 3, 0, READ_STATUS)>0){
        /* display the server response */
        printf("Server response:/n");
        n = read(fd, buf, BUFSIZE);
        buf[n] = '/0';
        printf("%s/n", buf);
    }
    else{
        err_quit("Socket I/O Read Timeout %s:%s/n", i_host, i_port);
    }
    close(fd);
#ifdef WIN32
    WSACleanup();
#endif
    return 0;
}
同时mark几个有用的url:
[http://people.web.psi.ch/rohrer_u/sample1.htm](http://people.web.psi.ch/rohrer_u/sample1.htm) Demo program for remote CAMAC access via TCP/IP
[http://www.lsword.net/code/list.asp?id=1383](http://www.lsword.net/code/list.asp?id=1383)**将Socket应用程序从Unix向Windows移植中应注意的几点问题**
**引用:http://www.zeali.net/entry/13**
