# C 语言开发 Linux 下 web 服务器(支持 GET/POST,SSL,目录显示等) - 文章 - 伯乐在线
原文出处： [yueguanghaidao](http://blog.csdn.net/yueguanghaidao/article/details/8450938)
这个主要是在CSAPP基础上做的，添加了POST，SSL，目录显示等功能。
一、 实现功能：
1.支持GET/POST方法
2.支持SSL安全连接即HTTPS
3.支持CGI
4.基于IP地址和掩码的认证
5.目录显示
6.日志功能
7.错误提示页面
github地址:**[https://github.com/Skycrab/Linux-C-Web-Server](https://github.com/Skycrab/Linux-C-Web-Server)**
源代码下载地址：[点击打开链接](http://download.csdn.net/detail/yueguanghaidao/5137322)
二、设计原理
首先介绍一些HTTP协议基本知识。
#1.GET/POST
本实现支持GET/POST方法，都是HTTP协议需要支持的标准方法。
GET方法主要是通过URL发送请求和传送数据，而POST方法在请求头空一格之后传送数据，所以POST方法比GET方法安全性高，因为GET方法可以直接看到传送的数据。另外一个区别就是GET方法传输的数据较小，而POST方法很大。所以一般表单，登陆页面等都是通过POST方法。
#2.MIME类型
当服务器获取客户端的请求的文件名，将分析文件的MIME类型，然后告诉浏览器改文件的MIME类型，浏览器通过MIME类型解析传送过来的数据。具体来说，浏览器请求一个主页面，该页面是一个HTML文件，那么服务器将”text/html”类型发给浏览器，浏览器通过HTML解析器识别发送过来的内容并显示。
下面将描述一个具体情景。
客户端使用浏览器通过URL发送请求，服务器获取请求。
如浏览器URL为：127.0.0.1/postAuth.html,
那么服务器获取到的请求为：GET  /postAuth.html  HTTP/1.1
意思是需要根目录下postAuth.html文件的内容，通过GET方法，使用HTTP/1.1协议(1.1是HTTP的版本号)。这是服务器将分析文件名，得知postAuth.html是一个HTML文件，所以将”text/html”发送给浏览器，然后读取postAuth.html内容发给浏览器。
实现简单的MIME类型识别代码如下：
主要就是通过文件后缀获取文件类型。


```
static void get_filetype(const char *filename, char *filetype)   
{  
    if (strstr(filename, ".html"))  
        strcpy(filetype, "text/html");  
    else if (strstr(filename, ".gif"))  
        strcpy(filetype, "image/gif");  
    else if (strstr(filename, ".jpg"))  
        strcpy(filetype, "image/jpeg");  
    else if (strstr(filename, ".png"))  
        strcpy(filetype, "image/png");  
    else  
    strcpy(filetype, "text/plain");  
}
```
如果支持HTTPS的话，那么我们就#define HTTPS，这主要通过gcc 的D选项实现的，具体细节可参考man手册。
静态内容显示实现如下：


```
static void serve_static(int fd, char *filename, int filesize) 
{
    int srcfd;
    char *srcp, filetype[MAXLINE], buf[MAXBUF];
 
    /* Send response headers to client */
    get_filetype(filename, filetype);
    sprintf(buf, "HTTP/1.0 200 OK\r\n");
    sprintf(buf, "%sServer: Tiny Web Server\r\n", buf);
    sprintf(buf, "%sContent-length: %d\r\n", buf, filesize);
    sprintf(buf, "%sContent-type: %s\r\n\r\n", buf, filetype);
    /* Send response body to client */
    srcfd = Open(filename, O_RDONLY, 0);
    srcp = Mmap(0, filesize, PROT_READ, MAP_PRIVATE, srcfd, 0);
    Close(srcfd);
    #ifdef HTTPS 
    if(ishttps)
    {
    	SSL_write(ssl, buf, strlen(buf));
	SSL_write(ssl, srcp, filesize);
    }
    else
    #endif
    {
	Rio_writen(fd, buf, strlen(buf));
	Rio_writen(fd, srcp, filesize);
    }
    Munmap(srcp, filesize);
}
```
#3.CGI规范
如果只能显示页面那么无疑缺少动态交互能力，于是CGI产生了。CGI是公共网关接口(Common Gateway Interface),是在CGI程序和Web服务器之间传递信息的规则。CGI允许Web服务器执行外部程序，并将它们的输出发送给浏览器。这样就提供了动态交互能力。
那么服务器是如何分开处理静态页面和动态CGI程序的呢？这主要是通过解析URL的方式。我们可以定义CGI程序的目录，如cgi-bin,那么如果URL包含”cgi-bin”字符串则这是动态程序，且将URL的参数给cgiargs。如果是静态页面，parse_uri返回1，反正返回0。所以我们可以通过返回值区别不同的服务类型。
具体解析URL方式如下：


```
static void serve_static(int fd, char *filename, int filesize) 
{
    int srcfd;
    char *srcp, filetype[MAXLINE], buf[MAXBUF];
 
    /* Send response headers to client */
    get_filetype(filename, filetype);
    sprintf(buf, "HTTP/1.0 200 OK\r\n");
    sprintf(buf, "%sServer: Tiny Web Server\r\n", buf);
    sprintf(buf, "%sContent-length: %d\r\n", buf, filesize);
    sprintf(buf, "%sContent-type: %s\r\n\r\n", buf, filetype);
    /* Send response body to client */
    srcfd = Open(filename, O_RDONLY, 0);
    srcp = Mmap(0, filesize, PROT_READ, MAP_PRIVATE, srcfd, 0);
    Close(srcfd);
    #ifdef HTTPS 
    if(ishttps)
    {
    	SSL_write(ssl, buf, strlen(buf));
	SSL_write(ssl, srcp, filesize);
    }
    else
    #endif
    {
	Rio_writen(fd, buf, strlen(buf));
	Rio_writen(fd, srcp, filesize);
    }
    Munmap(srcp, filesize);
}
```
GET方式的CGI规范实现原理:
服务器通过URL获取传给CGI程序的参数，设置环境变量QUERY_STRING，并将标准输出重定向到文件描述符，然后通过EXEC函数簇执行外部CGI程序。外部CGI程序获取QUERY_STRING并处理，处理完后输出结果。由于此时标准输出已重定向到文件描述符，即发送给了浏览器。
实现细节如下：由于涉及到HTTPS，所以稍微有点复杂。


```
static void serve_static(int fd, char *filename, int filesize) 
{
    int srcfd;
    char *srcp, filetype[MAXLINE], buf[MAXBUF];
 
    /* Send response headers to client */
    get_filetype(filename, filetype);
    sprintf(buf, "HTTP/1.0 200 OK\r\n");
    sprintf(buf, "%sServer: Tiny Web Server\r\n", buf);
    sprintf(buf, "%sContent-length: %d\r\n", buf, filesize);
    sprintf(buf, "%sContent-type: %s\r\n\r\n", buf, filetype);
    /* Send response body to client */
    srcfd = Open(filename, O_RDONLY, 0);
    srcp = Mmap(0, filesize, PROT_READ, MAP_PRIVATE, srcfd, 0);
    Close(srcfd);
    #ifdef HTTPS 
    if(ishttps)
    {
    	SSL_write(ssl, buf, strlen(buf));
	SSL_write(ssl, srcp, filesize);
    }
    else
    #endif
    {
	Rio_writen(fd, buf, strlen(buf));
	Rio_writen(fd, srcp, filesize);
    }
    Munmap(srcp, filesize);
}
```
POST方式的CGI规范实现原理：
由于POST方式不是通过URL传递参数，所以实现方式与GET方式不一样。
POST方式获取浏览器发送过来的参数长度设置为环境变量CONTENT-LENGTH。并将参数重定向到CGI的标准输入，这主要通过pipe管道实现的。CGI程序从标准输入读取CONTENT-LENGTH个字符就获取了浏览器传送的参数，并将处理结果输出到标准输出，同理标准输出已重定向到文件描述符，所以浏览器就能收到处理的响应。
具体实现细节如下：


```
static void serve_static(int fd, char *filename, int filesize) 
{
    int srcfd;
    char *srcp, filetype[MAXLINE], buf[MAXBUF];
 
    /* Send response headers to client */
    get_filetype(filename, filetype);
    sprintf(buf, "HTTP/1.0 200 OK\r\n");
    sprintf(buf, "%sServer: Tiny Web Server\r\n", buf);
    sprintf(buf, "%sContent-length: %d\r\n", buf, filesize);
    sprintf(buf, "%sContent-type: %s\r\n\r\n", buf, filetype);
    /* Send response body to client */
    srcfd = Open(filename, O_RDONLY, 0);
    srcp = Mmap(0, filesize, PROT_READ, MAP_PRIVATE, srcfd, 0);
    Close(srcfd);
    #ifdef HTTPS 
    if(ishttps)
    {
    	SSL_write(ssl, buf, strlen(buf));
	SSL_write(ssl, srcp, filesize);
    }
    else
    #endif
    {
	Rio_writen(fd, buf, strlen(buf));
	Rio_writen(fd, srcp, filesize);
    }
    Munmap(srcp, filesize);
}
```
目录显示功能原理：
主要是通过URL获取所需目录，然后获取该目录下所有文件，并发送相应信息，包括文件格式对应图片，文件名，文件大小，最后修改时间等。由于我们发送的文件名是通过超链接的形式，所以我们可以点击文件名继续浏览信息。
具体实现细节如下：


```
static void serve_static(int fd, char *filename, int filesize) 
{
    int srcfd;
    char *srcp, filetype[MAXLINE], buf[MAXBUF];
 
    /* Send response headers to client */
    get_filetype(filename, filetype);
    sprintf(buf, "HTTP/1.0 200 OK\r\n");
    sprintf(buf, "%sServer: Tiny Web Server\r\n", buf);
    sprintf(buf, "%sContent-length: %d\r\n", buf, filesize);
    sprintf(buf, "%sContent-type: %s\r\n\r\n", buf, filetype);
    /* Send response body to client */
    srcfd = Open(filename, O_RDONLY, 0);
    srcp = Mmap(0, filesize, PROT_READ, MAP_PRIVATE, srcfd, 0);
    Close(srcfd);
    #ifdef HTTPS 
    if(ishttps)
    {
    	SSL_write(ssl, buf, strlen(buf));
	SSL_write(ssl, srcp, filesize);
    }
    else
    #endif
    {
	Rio_writen(fd, buf, strlen(buf));
	Rio_writen(fd, srcp, filesize);
    }
    Munmap(srcp, filesize);
}
```
HTTPS的实现：
HTTPS主要基于openssl的开源库实现。如果没有安装，那么我们就不#define HTTPS。
HTTPS的功能主要就是提供安全的连接，服务器和浏览器之间传送的数据是通过加密的，加密方式可以自己选定。
开始连接时，服务器需要发送CA，由于我们的CA是自己签发的，所以需要我们自己添加为可信。
访问控制功能：
主要是通过获取客户端IP地址，并转换为整数，与上配置文件中定义的掩码，如果符合配置文件中允许的网段，那么可以访问，否则不可以。
具体实现如下。


```
static void serve_static(int fd, char *filename, int filesize) 
{
    int srcfd;
    char *srcp, filetype[MAXLINE], buf[MAXBUF];
 
    /* Send response headers to client */
    get_filetype(filename, filetype);
    sprintf(buf, "HTTP/1.0 200 OK\r\n");
    sprintf(buf, "%sServer: Tiny Web Server\r\n", buf);
    sprintf(buf, "%sContent-length: %d\r\n", buf, filesize);
    sprintf(buf, "%sContent-type: %s\r\n\r\n", buf, filetype);
    /* Send response body to client */
    srcfd = Open(filename, O_RDONLY, 0);
    srcp = Mmap(0, filesize, PROT_READ, MAP_PRIVATE, srcfd, 0);
    Close(srcfd);
    #ifdef HTTPS 
    if(ishttps)
    {
    	SSL_write(ssl, buf, strlen(buf));
	SSL_write(ssl, srcp, filesize);
    }
    else
    #endif
    {
	Rio_writen(fd, buf, strlen(buf));
	Rio_writen(fd, srcp, filesize);
    }
    Munmap(srcp, filesize);
}
```
配置文件的读取：
主要选项信息都定义与配置文件中。
格式举例如下;
#HTTP PORT
PORT = 8888
所以读取配置文件函数具体如下：


```
static char* getconfig(char* name)  
{  
/* 
pointer meaning: 
 
...port...=...8000... 
   |  |   |   |  | 
  *fs |   |   |  *be    f->forward  b-> back 
      *fe |   *bs       s->start    e-> end 
          *equal 
*/  
    static char info[64];  
    int find=0;  
    char tmp[256],fore[64],back[64],tmpcwd[MAXLINE];  
    char *fs,*fe,*equal,*bs,*be,*start;  
  
    strcpy(tmpcwd,cwd);  
    strcat(tmpcwd,"/");  
    FILE *fp=getfp(strcat(tmpcwd,"config.ini"));  
    while(fgets(tmp,255,fp)!=NULL)  
    {  
        start=tmp;  
        equal=strchr(tmp,'=');  
  
        while(isblank(*start))  
            ++start;  
        fs=start;  
  
        if(*fs=='#')  
            continue;  
        while(isalpha(*start))  
            ++start;  
        fe=start-1;  
  
        strncpy(fore,fs,fe-fs+1);  
        fore[fe-fs+1]='\0';  
        if(strcmp(fore,name)!=0)  
            continue;  
        find=1;  
  
        start=equal+1;  
        while(isblank(*start))  
            ++start;  
        bs=start;  
  
        while(!isblank(*start)&&*start!='\n')  
            ++start;  
        be=start-1;  
  
        strncpy(back,bs,be-bs+1);  
        back[be-bs+1]='\0';  
        strcpy(info,back);  
        break;  
    }  
    if(find)  
        return info;  
    else  
        return NULL;  
}
```
二、 测试
本次测试使用了两台机器。一台Ubuntu的浏览器作为客户端，一台Redhat作为服务器端，其中Redhat是Ubuntu上基于VirtualBox的一台虚拟机。
IP地址信息如下：Ubuntu的vboxnet0：


```
<img src="http://jbcdn2.b0.upaiyun.com/2016/11/73d81eee84e5fe07de8e076ee6b5d25a.jpg" alt="" />
```
RedHateth0:
![](http://jbcdn2.b0.upaiyun.com/2016/11/96ac3370d7967dc81945e7685b470ffe.jpg)
RedHat主机编译项目：
![](http://jbcdn2.b0.upaiyun.com/2016/11/95c404967072b86ebe659b31c3b1c660.jpg)
由于我们同事监听了8000和4444，所以有两个进程启动。
HTTP的首页：
![](http://jbcdn2.b0.upaiyun.com/2016/11/073ecbf2836bd9c81112ca8308a72fec.jpg)
目录显示功能：
![](http://jbcdn2.b0.upaiyun.com/2016/11/66db39dd38a6f5f066438a271d7f47f0.jpg)
HTTP GET页面：
![](http://jbcdn2.b0.upaiyun.com/2016/11/9f83486800d40564723cab72ccc54802.jpg)
HTTPGET响应：
![](http://jbcdn2.b0.upaiyun.com/2016/11/e3081b3d57031fb76e4c80a86c5d3b65.jpg)
从HTTP GET响应中我们观察URL，参数的确是通过URL传送过去的。
其中getAuth.c如下：


```
#include "wrap.h"  
#include "parse.h"  
  
int main(void) {  
    char *buf, *p;  
    char name[MAXLINE], passwd[MAXLINE],content[MAXLINE];  
  
    /* Extract the two arguments */  
    if ((buf = getenv("QUERY_STRING")) != NULL) {  
    p = strchr(buf, '&');  
    *p = '\0';  
    strcpy(name, buf);  
    strcpy(passwd, p+1);  
    }  
  
  
    /* Make the response body */  
    sprintf(content, "Welcome to auth.com:%s and %s\r\n<p>",name,passwd);  
    sprintf(content, "%s\r\n", content);  
  
    sprintf(content, "%sThanks for visiting!\r\n", content);  
    
    /* Generate the HTTP response */  
    printf("Content-length: %d\r\n", strlen(content));  
    printf("Content-type: text/html\r\n\r\n");  
    printf("%s", content);  
    fflush(stdout);  
    exit(0);  
}
```
HTTPS的首页：由于我们的CA不可信，所以需要我们认可
![](http://jbcdn2.b0.upaiyun.com/2016/11/cdb1bff994394e70a40f4d9f06eb9a9e.jpg)
认可后HTTPS首页：
![](http://jbcdn2.b0.upaiyun.com/2016/11/8c5e9570bbff938c29e74d5cef4103c7.jpg)
HTTPS POST页面：
![](http://jbcdn2.b0.upaiyun.com/2016/11/85edb1402dfae1aaf300fe6ce6c9fe90.jpg)
HTTPS POST响应：
![](http://jbcdn2.b0.upaiyun.com/2016/11/9f0aa042438830879b24439a098a19ef.jpg)
从上我们可以看出，POST提交的参数的确不是通过URL传送的。
