# 用C语言制作Web服务器 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2016年05月23日 09:30:15[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：10417
### *阅读经典——《深入理解计算机系统》09*
本文，我们将使用C语言从零开始实现一个支持静态/动态网页的Web服务器。我们把这个服务器叫做Tiny。
> - 背景知识
- 客户端-服务器编程模型
- 使用socket处理请求与响应
- HTTP协议与静/动态网页
- 关键代码解析
- 实验效果与源码
### 背景知识
Web服务器使用HTTP协议与客户端（即浏览器）通信，而HTTP协议又基于TCP/IP协议。因此我们要做的工作就是利用Linux系统提供的TCP通信接口来实现HTTP协议。
而Linux为我们提供了哪些网络编程接口呢？没错，就是socket（套接字），我们会在后面详细介绍该接口的使用方式。
另外我们应该清楚Linux的系统I/O和文件系统的关系。在Linux中，所有I/O设备都被看作一个个文件，I/O设备的输入输出被认做读写文件。网络作为一种I/O设备，同样被看作文件，而且是一类特殊的文件，即套接字文件。
我们还要对网络通信协议TCP/IP有一个大致的了解，知道IP地址和端口的作用。
接下来我们讲解客户端-服务器编程模型。
### 客户端-服务器编程模型
客户端-服务器编程模型是一个典型的进程间通信模型。客户端进程和服务器进程通常分处两个不同的主机，如下图所示，客户端发送请求给服务器，服务器从本地资源库中查找需要的资源，然后发送响应给客户端，最后客户端（通常是浏览器）处理这个响应，把结果显示在浏览器上。
![](http://upload-images.jianshu.io/upload_images/1186132-dd29ac148eaccd84.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
client-server transaction
这个过程看起来很简单，但是我们需要深入具体的实现细节。我们知道，TCP是基于连接的，需要先建立连接才能互相通信。在Linux中，socket为我们提供了方便的解决方案。
每一对网络连接称为一个socket对，包括两个端点的socket地址，表示如下
`(cliaddr : cliport, servaddr : servport)`
其中， `cliaddr`和`cliport`分别是客户端IP地址和客户端端口，`servaddr`和`servport`分别是服务器IP地址和服务器端口。举例说明如下：
![](http://upload-images.jianshu.io/upload_images/1186132-ae06657e637400ed.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
connection socket pair
这对地址和端口唯一确定了连接的双方，在TCP/IP协议网络中就能轻松地找到对方。
### 使用socket处理请求与响应
熟悉TCP协议的朋友们应该很容易理解下面的流程图。
![](http://upload-images.jianshu.io/upload_images/1186132-4cab0f440489fb06.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
socket overview
服务器调用`socket`函数获取一个socket，然后调用`bind`函数绑定本机的IP地址和端口，再调用`listen`函数开启监听，最后调用`accept`函数等待直到有客户端发起连接。
另一方面，客户端调用`socket`函数获取一个socket，然后调用`connect`函数向指定服务器发起连接请求，当连接成功或出现错误后返回。若连接成功，服务器端的`accept`函数也会成功返回，返回另一个已连接的socket（不是最初调用`socket`函数得到的socket），该socket可以直接用于与客户端通信。而服务器最初的那个socket可以继续循环调用`accept`函数，等待下一次连接的到来。
连接成功后，无论是客户端还是服务器，只要向socket读写数据就可以实现与对方socket的通信。图中`rio_readlineb`和`rio_written`是作者封装的I/O读写函数，与Linux系统提供的`read`和`write`作用基本相同，详细介绍见参考资料。
客户端关闭连接时会发送一个EOF到服务器，服务器读取后关闭连接，进入下一个循环。
这里面用到的所有Linux网络编程接口都定义在`<sys/socket.h>`头文件中，为了更清晰地帮助大家理解每个函数的使用方法，我们列出它们的函数声明。
```cpp
```cpp
#include <sys/types.h>
#include <sys/socket.h>
/**
获取一个socket descriptor
@params:
    domain: 此处固定使用AF_INET
    type: 此处固定使用SOCK_STREAM
    protocol: 此处固定使用0
@returns:
    nonnegative descriptor if OK, -1 on error.
*/
int socket(int domain, int type, int protocol);
/**
客户端socket向服务器发起连接
@params:
    sockfd: 发起连接的socket descriptor
    serv_addr: 连接的目标地址和端口
    addrlen: sizeof(*serv_addr)
@returns:
    0 if OK, -1 on error
*/
int connect(int sockfd, struct sockaddr *serv_addr, int addrlen);
/**
服务器socket绑定地址和端口
@params:
    sockfd: 当前socket descriptor
    my_addr: 指定绑定的本机地址和端口
    addrlen: sizeof(*my_addr)
@returns:
    0 if OK, -1 on error
*/
int bind(int sockfd, struct sockaddr *my_addr, int addrlen);
/**
将当前socket转变为可以监听外部连接请求的socket
@params:
    sockfd: 当前socket descriptor
    backlog: 请求队列的最大长度
@returns:
    0 if OK, -1 on error
*/
int listen(int sockfd, int backlog);
/**
等待客户端请求到达，注意，成功返回得到的是一个新的socket descriptor，
而不是输入参数listenfd。
@params:
    listenfd: 当前正在用于监听的socket descriptor
    addr: 客户端请求地址（输出参数）
    addrlen: 客户端请求地址的长度（输出参数）
@returns:
    成功则返回一个非负的connected descriptor，出错则返回-1
*/
int accept(int listenfd, struct sockaddr *addr, int *addrlen);
```
```
### HTTP协议与静/动态网页
HTTP协议的具体内容在此不再讲述，不熟悉的朋友们可以查看参考资料中的第二篇文章。
现在我们有必要说明一下所谓的静态网页和动态网页。静态网页是指内容固定的网页，通常是事先写好的html文档，每次访问得到的都是相同的内容。而动态网页是指多次访问可以得到不同内容的网页，现在流行的动态网页技术有PHP、JSP、ASP等。我们将要实现的服务器同时支持静态网页和动态网页，但动态网页并不采用上述几种技术实现，而是使用早期流行的CGI（Common Gateway Interface）。CGI是一种动态网页标准，规定了外部应用程序（CGI程序）如何与Web服务器交换信息，但由于有许多缺点，现在几乎已经被淘汰。关于CGI的更多信息，可以查看参考资料。
### 关键代码解析
Web服务器主进程从main函数开始，代码如下。
```cpp
```cpp
int main(int argc, char **argv) 
{
    int listenfd, connfd;
    socklen_t clientlen;
    struct sockaddr_storage clientaddr;
    /* Check command line args */
    if (argc != 2) {
        fprintf(stderr, "usage: %s <port>\n", argv[0]);
        exit(1);
    }
    listenfd = Open_listenfd(argv[1]);
    while (1) {
        clientlen = sizeof(clientaddr);
        connfd = Accept(listenfd, (SA *)&clientaddr, &clientlen);
        doit(connfd);
        Close(connfd);
    }
}
```
```
主函数参数需要传入服务器绑定的端口号码，得到这个号码后，调用`Open_listenfd`函数，该函数完成`socket`、`bind`、`listen`等一系列操作。接着调用`accept`函数等待客户端请求。注意，`Accept`是`accept`的包装函数，用来自动处理可能发生的异常，我们只需把它们当成一样的就行了。当`accept`成功返回后，我们拿到了connected
 socket descriptor，然后调用`doit`函数处理请求。
`doit`函数定义如下。
```cpp
```cpp
void doit(int fd) 
{
    int is_static;
    struct stat sbuf;
    char buf[MAXLINE], method[MAXLINE], uri[MAXLINE], version[MAXLINE];
    char filename[MAXLINE], cgiargs[MAXLINE];
    rio_t rio;
    /* Read request line and headers */
    Rio_readinitb(&rio, fd);
    if (!Rio_readlineb(&rio, buf, MAXLINE))
        return;
    printf("%s", buf);
    sscanf(buf, "%s %s %s", method, uri, version);
    if (strcasecmp(method, "GET")) {
        clienterror(fd, method, "501", "Not Implemented",
                    "Tiny does not implement this method");
        return;
    }
    read_requesthdrs(&rio);
    /* Parse URI from GET request */
    is_static = parse_uri(uri, filename, cgiargs);
    if (stat(filename, &sbuf) < 0) {
    clienterror(fd, filename, "404", "Not found",
            "Tiny couldn't find this file");
    return;
    }
    if (is_static) { /* Serve static content */          
    if (!(S_ISREG(sbuf.st_mode)) || !(S_IRUSR & sbuf.st_mode)) {
        clienterror(fd, filename, "403", "Forbidden",
            "Tiny couldn't read the file");
        return;
    }
    serve_static(fd, filename, sbuf.st_size);
    }
    else { /* Serve dynamic content */
    if (!(S_ISREG(sbuf.st_mode)) || !(S_IXUSR & sbuf.st_mode)) { 
        clienterror(fd, filename, "403", "Forbidden",
            "Tiny couldn't run the CGI program");
        return;
    }
    serve_dynamic(fd, filename, cgiargs);
    }
}
```
```
为了更接近现实，假设现在接收到的HTTP请求如下。该请求的请求头是空的。
```cpp
```cpp
GET /cgi-bin/adder?15000&213 HTTP/1.0
```
```
代码中，`Rio_readlineb`和`sscanf`负责读入请求行并解析出请求方法、请求URI和版本号。接下来调用`parse_uri`函数，该函数利用请求uri得到访问的文件名、CGI参数，并返回是否按照静态网页处理。如果是，则调用`serve_static`函数处理，否则调用`serve_dynamic`函数处理。
`serve_static`函数定义如下。
```cpp
```cpp
void serve_static(int fd, char *filename, int filesize) 
{
    int srcfd;
    char *srcp, filetype[MAXLINE], buf[MAXBUF];
    /* Send response headers to client */
    get_filetype(filename, filetype);
    sprintf(buf, "HTTP/1.0 200 OK\r\n");
    sprintf(buf, "%sServer: Tiny Web Server\r\n", buf);
    sprintf(buf, "%sConnection: close\r\n", buf);
    sprintf(buf, "%sContent-length: %d\r\n", buf, filesize);
    sprintf(buf, "%sContent-type: %s\r\n\r\n", buf, filetype);
    Rio_writen(fd, buf, strlen(buf));
    printf("Response headers:\n");
    printf("%s", buf);
    /* Send response body to client */
    srcfd = Open(filename, O_RDONLY, 0);
    srcp = Mmap(0, filesize, PROT_READ, MAP_PRIVATE, srcfd, 0);
    Close(srcfd);
    Rio_writen(fd, srcp, filesize);
    Munmap(srcp, filesize);
}
```
```
直接看最后几行代码。`Open`以只读方式打开请求的文件，`Mmap`将该文件直接读取到虚拟地址空间中的任意位置，然后关闭文件。接下来`Rio_written`把内存中的文件写入`fd`指定的connected socket descriptor，静态页面响应完成。`Munmap`删除刚才在虚拟地址空间申请的内存。关于`mmap`函数的更多介绍见参考资料。
`serve_dynamic`函数定义如下。
```cpp
```cpp
void serve_dynamic(int fd, char *filename, char *cgiargs) 
{
    char buf[MAXLINE], *emptylist[] = { NULL };
    /* Return first part of HTTP response */
    sprintf(buf, "HTTP/1.0 200 OK\r\n"); 
    Rio_writen(fd, buf, strlen(buf));
    sprintf(buf, "Server: Tiny Web Server\r\n");
    Rio_writen(fd, buf, strlen(buf));
    if (Fork() == 0) { /* Child */
    /* Real server would set all CGI vars here */
    setenv("QUERY_STRING", cgiargs, 1);
    Dup2(fd, STDOUT_FILENO);         /* Redirect stdout to client */
    Execve(filename, emptylist, environ); /* Run CGI program */
    }
    Wait(NULL); /* Parent waits for and reaps child */
}
```
```
对于动态网页请求，我们的方法是创建一个子进程，在子进程中执行CGI程序。看代码，`Fork`函数创建子进程，熟悉Linux进程的朋友们应该知道，该函数会返回两次，一次在父进程中返回，返回值不等于0，另一次在子进程中返回，返回值为0，因此`if`判断内部是子进程执行的代码。首先设置环境变量，用于把请求参数传递给CGI程序。接下来调用`Dup2`函数将标准输出重定向到connected socket descriptor，这样一来使用标准输出输出的内容将会直接发送给客户端。然后调用`Execve`函数在子进程中执行`filename`指定的CGI程序。最后在父进程中调用了`Wait`函数用于收割子进程，当子进程终止后该函数才会返回。因此该Web服务器不能同时处理多个访问，只能一个一个处理。
我们给出了一个CGI程序的实例adder，用于计算两个参数之和。代码如下。
```cpp
```cpp
/*
 * adder.c - a minimal CGI program that adds two numbers together
 */
int main(void) {
    char *buf, *p;
    char arg1[MAXLINE], arg2[MAXLINE], content[MAXLINE];
    int n1=0, n2=0;
    /* Extract the two arguments */
    if ((buf = getenv("QUERY_STRING")) != NULL) {
    p = strchr(buf, '&');
    *p = '\0';
    strcpy(arg1, buf);
    strcpy(arg2, p+1);
    n1 = atoi(arg1);
    n2 = atoi(arg2);
    }
    /* Make the response body */
    sprintf(content, "Welcome to add.com: ");
    sprintf(content, "%sTHE Internet addition portal.\r\n<p>", content);
    sprintf(content, "%sThe answer is: %d + %d = %d\r\n<p>", 
        content, n1, n2, n1 + n2);
    sprintf(content, "%sThanks for visiting!\r\n", content);
    /* Generate the HTTP response */
    printf("Connection: close\r\n");
    printf("Content-length: %d\r\n", (int)strlen(content));
    printf("Content-type: text/html\r\n\r\n");
    printf("%s", content);
    fflush(stdout);
    exit(0);
}
```
```
这段代码就非常简单了，从环境变量中取出请求参数，得到两个加数的值，相加后输出。需要注意的是，由于刚才已经重定向标准输出，因此使用`printf`就可以把内容输出给客户端。输出内容需要遵照HTTP协议的格式，才能在浏览器中正确显示出来。
### 实验效果与源码
输入如下命令启动Web服务器,并绑定8000端口:
> 
./tiny 8000
**静态网页效果：**访问[http://localhost:8000](http://localhost:8000)
![](http://upload-images.jianshu.io/upload_images/1186132-8bf7410516433d2a.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
静态网页效果
**动态网页效果：**访问[http://localhost:8000/cgi-bin/adder?1&2](http://localhost:8000/cgi-bin/adder?1&2)
![](http://upload-images.jianshu.io/upload_images/1186132-70e690610b6d07f9.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
动态网页效果
至此，我们的Web服务器终于大功告成。大家可以[下载源码](https://github.com/jingedawang/Tiny-WebServer.git)，并在自己的计算机上部署测试。
关注[作者](http://www.jianshu.com/users/8a84489c965c/latest_articles)或[文集《深入理解计算机系统》](http://www.jianshu.com/notebooks/2951875/latest)，第一时间获取最新发布文章。
### 参考资料
[Linux IO操作详解——RIO包](http://www.2cto.com/os/201603/496491.html) 金樽对月的成长脚步
[深入理解HTTP协议](http://www.360doc.com/content/10/0930/17/3668821_57590979.shtml) micro36
[CGI与Servlet的比较](http://blog.csdn.net/yttcjj/article/details/6975907) YTTCJJ
[我所了解的cgi](http://www.cnblogs.com/liuzhang/p/3929198.html) 掸尘
[Linux内存管理之mmap详解](http://blog.chinaunix.net/uid-26669729-id-3077015.html) heavent2010
文／金戈大王（简书作者）
原文链接：http://www.jianshu.com/p/dd580395bf11
著作权归作者所有，转载请联系作者获得授权，并标注“简书作者”。
