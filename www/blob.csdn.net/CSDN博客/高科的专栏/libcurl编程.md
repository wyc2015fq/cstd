
# libcurl编程 - 高科的专栏 - CSDN博客

2011年08月10日 14:39:53[高科](https://me.csdn.net/pbymw8iwm)阅读数：1619标签：[编程																](https://so.csdn.net/so/search/s.do?q=编程&t=blog)[gtk																](https://so.csdn.net/so/search/s.do?q=gtk&t=blog)[stream																](https://so.csdn.net/so/search/s.do?q=stream&t=blog)[ftp服务器																](https://so.csdn.net/so/search/s.do?q=ftp服务器&t=blog)[文件传输工具																](https://so.csdn.net/so/search/s.do?q=文件传输工具&t=blog)[file																](https://so.csdn.net/so/search/s.do?q=file&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=文件传输工具&t=blog)[
																					](https://so.csdn.net/so/search/s.do?q=ftp服务器&t=blog)个人分类：[linux																](https://blog.csdn.net/pbymw8iwm/article/category/863753)[网络																](https://blog.csdn.net/pbymw8iwm/article/category/893044)[
							](https://blog.csdn.net/pbymw8iwm/article/category/863753)
[
				](https://so.csdn.net/so/search/s.do?q=ftp服务器&t=blog)
[
			](https://so.csdn.net/so/search/s.do?q=ftp服务器&t=blog)
[
		](https://so.csdn.net/so/search/s.do?q=stream&t=blog)
[
	](https://so.csdn.net/so/search/s.do?q=gtk&t=blog)
[
	](https://so.csdn.net/so/search/s.do?q=编程&t=blog)
### 一、curl简介
curl是一个利用URL语法在命令行方式下工作的文件传输工具。它支持的协议有：FTP, FTPS, HTTP, HTTPS, GOPHER, TELNET, DICT, FILE 以及 LDAP。curl同样支持HTTPS认证，HTTP POST方法, HTTP PUT方法, FTP上传, kerberos认证, HTTP上传, 代理服务器, cookies, 用户名/密码认证, 下载文件断点续传, 上载文件断点续传, http代理服务器管道（ proxy tunneling）, 甚至它还支持IPv6,
 socks5代理服务器, 通过http代理服务器上传文件到FTP服务器等等，功能十分强大。
### 二、curl安装
下载地址：[http://curl.haxx.se/download.html](http://curl.haxx.se/download.html)
ubuntu安装：sudo apt-get install curl
libcurl静态库：./configure & make & install
windows:下载curl.exe，cmd进入curl.exe所在目录可以直接使用
### 三、curl使用
curl的用法为：curl [options] [URL...]，其中options是下载需要的参数，大约有80多个，curl的各个功能完全是依靠这些参数完成的。这里只介绍几种简单的用法，详细的curl的参数在[http://curl.haxx.se/docs/](http://curl.haxx.se/docs/)说明。
**1、读取网页**
curl[http://www.linuxidc.com](http://www.linuxidc.com/)
**2、保存网页、下载文件**
以page.html命名下载网页：curl –o page.html[http://www.linuxidc.com](http://www.linuxidc.com/)
以服务器上的文件名命名网页：curl –O http://cgi2.tky.3web.ne.jp/~zzh/screen1.JPG
批量下载文件：curl -O http://cgi2.tky.3web.ne.jp/~zzh/screen[1-10].JPG
**3、使用代理服务器及其端口**
curl –x 127.0.0.1:8580 –o page.html[http://www.linuxidc.com](http://www.linuxidc.com/)
**4、使用cookie来记录session信息**
cookie 信息存到cookie1.txt中：curl –o page.html –D cookie1.txt[
http://www.linuxidc.com](http://www.linuxidc.com/)
使用上次的cookie并生成新的cookie：curl –o page.html –D cookie2.txt  -b cookie2.txt[
http://www.linuxidc.com](http://www.linuxidc.com/)
**5、断点续传**
比如下载screen1.JPG中，突然掉线了，可以这样开始续传：curl -c -O[
http://cgi2.tky.3wb.ne.jp/~zzh/screen1.JPG](http://cgi2.tky.3wb.ne.jp/~zzh/screen1.JPG)
另外可以用-r选项进行分块下载
**6、上传文件**
比如我们向ftp传一个文件：curl -T localfile -u name:passwd ftp://upload_site:port/path/
PS：对于ftp服务器用-u name:passwd选项
**7、http提交一个表单GET与POST模式**
GET模式什么option都不用，只需要把变量写在url里面就可以了比如：
$curl http://www.linuxidc.com/login.cgi?user=nickwolfe&password=12345
POST模式的选项是 -d
比如：curl -d "user=nickwolfe&password=12345"[
http://www.linuxidc.com/login.cgi](http://www.linuxidc.com/login.cgi)
### 四、curl编程
**1、curl编程流程**
LibCurl编程流程在基于LibCurl的程序里，主要采用callback function （回调函数）的形式完成传输任务，用户在启动传输前设置好各类参数和回调函数，当满足条件时libcurl将调用用户的回调函数实现特定功能。下面是利用libcurl完成传输任务的流程：
（1）调用curl_global_init()初始化libcurl
（2）调用 curl_easy_init()函数得到 easy interface型指针
（3）调用curl_easy_setopt设置传输选项
（4）根据curl_easy_setopt设置的传输选项，实现回调函数以完成用户特定任务
（5）调用curl_easy_perform（）函数完成传输任务
（6）调用curl_easy_cleanup（）释放内存
在整过过程中设置curl_easy_setopt()参数是最关键的，几乎所有的libcurl程序都要使用它。
**2、重要函数**
（1）CURLcode curl_global_init(long flags);
描述：
这个函数只能用一次。(其实在调用curl_global_cleanup 函数后仍然可再用)
如果这个函数在curl_easy_init函数调用时还没调用，它讲由libcurl库自动完成。
参数：flags
CURL_GLOBAL_ALL                        //初始化所有的可能的调用。
CURL_GLOBAL_SSL                       //初始化支持 安全套接字层。
CURL_GLOBAL_WIN32                  //初始化win32套接字库。
CURL_GLOBAL_NOTHING              //没有额外的初始化。
（2）void curl_global_cleanup(void);
描述：在结束libcurl使用的时候，用来对curl_global_init做的工作清理。类似于close的函数。
（3）char *curl_version( );
描述: 打印当前libcurl库的版本。
（4）CURL *curl_easy_init( );
描述：curl_easy_init用来初始化一个CURL的指针(有些像返回FILE类型的指针一样). 相应的在调用结束时要用curl_easy_cleanup函数清理.
一般curl_easy_init意味着一个会话的开始. 它的返回值一般都用在easy系列的函数中.
（5）void curl_easy_cleanup(CURL *handle);
描述:
这个调用用来结束一个会话.与curl_easy_init配合着用.
参数:
CURL类型的指针.
（6）CURLcode curl_easy_setopt(CURL *handle, CURLoption option, parameter);
描述: 这个函数最重要了.几乎所有的curl 程序都要频繁的使用它.它告诉curl库.程序将有如何的行为. 比如要查看一个网页的html代码等.(这个函数有些像ioctl函数)参数:
1 CURL类型的指针
2 各种CURLoption类型的选项.(都在curl.h库里有定义,man 也可以查看到)
3 parameter 这个参数 既可以是个函数的指针,也可以是某个对象的指针,也可以是个long型的变量.它用什么这取决于第二个参数.
CURLoption 这个参数的取值很多.具体的可以查看man手册.
（7） CURLcode curl_easy_perform(CURL *handle);描述:这个函数在初始化CURL类型的指针 以及curl_easy_setopt完成后调用. 就像字面的意思所说perform就像是个舞台.让我们设置的
option 运作起来.参数:
CURL类型的指针.
**3、curl_easy_setopt函数介绍**
本节主要介绍curl_easy_setopt中跟http相关的参数。注意本节的阐述都是以libcurl作为主体，其它为客体来阐述的。
1.     CURLOPT_URL
设置访问URL
2.       CURLOPT_WRITEFUNCTION，CURLOPT_WRITEDATA
回调函数原型为：size_t function( void *ptr, size_t size, size_t nmemb, void *stream); 函数将在libcurl接收到数据后被调用，因此函数多做数据保存的功能，如处理下载文件。CURLOPT_WRITEDATA 用于表明CURLOPT_WRITEFUNCTION函数中的stream指针的来源。
3.      CURLOPT_HEADERFUNCTION，CURLOPT_HEADERDATA
回调函数原型为 size_t function( void *ptr, size_t size,size_t nmemb, void *stream); libcurl一旦接收到http 头部数据后将调用该函数。CURLOPT_WRITEDATA 传递指针给libcurl，该指针表明CURLOPT_HEADERFUNCTION 函数的stream指针的来源。
4.       CURLOPT_READFUNCTION CURLOPT_READDATA
libCurl需要读取数据传递给远程主机时将调用CURLOPT_READFUNCTION指定的函数，函数原型是：size_t function(void *ptr, size_t size, size_t nmemb,void *stream). CURLOPT_READDATA 表明CURLOPT_READFUNCTION函数原型中的stream指针来源。
5.       CURLOPT_NOPROGRESS，CURLOPT_PROGRESSFUNCTION，CURLOPT_PROGRESSDATA
跟数据传输进度相关的参数。CURLOPT_PROGRESSFUNCTION 指定的函数正常情况下每秒被libcurl调用一次，为了使CURLOPT_PROGRESSFUNCTION被调用，CURLOPT_NOPROGRESS必须被设置为false，CURLOPT_PROGRESSDATA指定的参数将作为CURLOPT_PROGRESSFUNCTION指定函数的第一个参数
6.       CURLOPT_TIMEOUT，CURLOPT_CONNECTIONTIMEOUT:
CURLOPT_TIMEOUT 由于设置传输时间，CURLOPT_CONNECTIONTIMEOUT 设置连接等待时间
7.       CURLOPT_FOLLOWLOCATION
设置重定位URL
CURLOPT_RANGE: CURLOPT_RESUME_FROM:
断点续传相关设置。CURLOPT_RANGE 指定char *参数传递给libcurl，用于指明http域的RANGE头域，例如：
表示头500个字节：bytes=0-499
表示第二个500字节：bytes=500-999
表示最后500个字节：bytes=-500
表示500字节以后的范围：bytes=500-
第一个和最后一个字节：bytes=0-0,-1
同时指定几个范围：bytes=500-600,601-999
CURLOPT_RESUME_FROM 传递一个long参数给libcurl，指定你希望开始传递的
偏移量。
**4、curl_easy_perform 函数说明（error 状态码）**
该函数完成curl_easy_setopt指定的所有选项，本节重点介绍curl_easy_perform的返回值。返回0意味一切ok，非0代表错误发生。主要错误码说明：
1.    CURLE_OK
任务完成一切都好
2     CURLE_UNSUPPORTED_PROTOCOL
不支持的协议，由URL的头部指定
3     CURLE_COULDNT_CONNECT
不能连接到remote 主机或者代理
4     CURLE_REMOTE_ACCESS_DENIED
访问被拒绝
5     CURLE_HTTP_RETURNED_ERROR
Http返回错误
6     CURLE_READ_ERROR
读本地文件错误
### 五、curl编程例子
**1、获取html网页**
[view plain](http://blog.csdn.net/s030702614/article/details/6198641#)
\#include <stdio.h>
\#include <curl/curl.h>
\#include <stdlib.h>
int main(int argc, char *argv[])
{
CURL *curl;             //定义CURL类型的指针
CURLcode res;           //定义CURLcode类型的变量，保存返回状态码
if(argc!=2)
{
printf("Usage : file <url>;/n");
exit(1);
}
curl = curl_easy_init();        //初始化一个CURL类型的指针
if(curl!=NULL)
{
//设置curl选项. 其中CURLOPT_URL是让用户指定url. argv[1]中存放的命令行传进来的网址
curl_easy_setopt(curl, CURLOPT_URL, argv[1]);
//调用curl_easy_perform 执行我们的设置.并进行相关的操作. 在这里只在屏幕上显示出来.
res = curl_easy_perform(curl);
//清除curl操作.
curl_easy_cleanup(curl);
}
return 0;
}

编译gcc  get_http.c  -o get_http –lcurl
./ get_http www.baidu.com
**2、网页下载保存**
[view plain](http://blog.csdn.net/s030702614/article/details/6198641#)
// 采用CURLOPT_WRITEFUNCTION 实现网页下载保存功能
\#include <stdio.h>;
\#include <stdlib.h>;
\#include <unistd.h>;
\#include <curl/curl.h>;
\#include <curl/types.h>;
\#include <curl/easy.h>;
FILE *fp;  //定义FILE类型指针
//这个函数是为了符合CURLOPT_WRITEFUNCTION而构造的
//完成数据保存功能
size_t write_data(void *ptr, size_t size, size_t nmemb, void *stream)
{
int written = fwrite(ptr, size, nmemb, (FILE *)fp);
return written;
}
int main(int argc, char *argv[])
{
CURL *curl;
curl_global_init(CURL_GLOBAL_ALL);
curl=curl_easy_init();
curl_easy_setopt(curl, CURLOPT_URL, argv[1]);
if((fp=fopen(argv[2],"w"))==NULL)
{
curl_easy_cleanup(curl);
exit(1);
}
////CURLOPT_WRITEFUNCTION 将后继的动作交给write_data函数处理
curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
curl_easy_perform(curl);
curl_easy_cleanup(curl);
exit(0);
}

编译gcc  save_http.c  -o save_http –lcurl
./ save_http www.baidu.com  /tmp/baidu
**3、进度条实例（显示文件下载进度）**
[view plain](http://blog.csdn.net/s030702614/article/details/6198641#)
// 采用CURLOPT_NOPROGRESS， CURLOPT_PROGRESSFUNCTION    CURLOPT_PROGRESSDATA 实现文件传输进度提示功能
//函数采用了gtk库，故编译时需指定gtk库
//函数启动专门的线程用于显示gtk 进度条bar
\#include <stdio.h>
\#include <gtk/gtk.h>
\#include <curl/curl.h>
\#include <curl/types.h> /* new for v7 */
\#include <curl/easy.h> /* new for v7 */
GtkWidget *Bar;
////这个函数是为了符合CURLOPT_WRITEFUNCTION而构造的
//完成数据保存功能
size_t my_write_func(void *ptr, size_t size, size_t nmemb, FILE *stream)
{
return fwrite(ptr, size, nmemb, stream);
}
//这个函数是为了符合CURLOPT_READFUNCTION而构造的
//数据上传时使用
size_t my_read_func(void *ptr, size_t size, size_t nmemb, FILE *stream)
{
return fread(ptr, size, nmemb, stream);
}
//这个函数是为了符合CURLOPT_PROGRESSFUNCTION而构造的
//显示文件传输进度，t代表文件大小，d代表传输已经完成部分
int my_progress_func(GtkWidget *bar,
double t, /* dltotal */
double d, /* dlnow */
double ultotal,
double ulnow)
{
/*  printf("%d / %d (%g %%)/n", d, t, d*100.0/t);*/
gdk_threads_enter();
gtk_progress_set_value(GTK_PROGRESS(bar), d*100.0/t);
gdk_threads_leave();
return 0;
}

void *my_thread(void *ptr)
{
CURL *curl;
CURLcode res;
FILE *outfile;
gchar *url = ptr;
curl = curl_easy_init();
if(curl)
{
outfile = fopen("test.curl", "w");
curl_easy_setopt(curl, CURLOPT_URL, url);
curl_easy_setopt(curl, CURLOPT_WRITEDATA, outfile);
curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, my_write_func);
curl_easy_setopt(curl, CURLOPT_READFUNCTION, my_read_func);
curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 0L);
curl_easy_setopt(curl, CURLOPT_PROGRESSFUNCTION, my_progress_func);
curl_easy_setopt(curl, CURLOPT_PROGRESSDATA, Bar);
res = curl_easy_perform(curl);
fclose(outfile);
/* always cleanup */
curl_easy_cleanup(curl);
}
return NULL;
}
int main(int argc, char **argv)
{
GtkWidget *Window, *Frame, *Frame2;
GtkAdjustment *adj;
/* Must initialize libcurl before any threads are started */
curl_global_init(CURL_GLOBAL_ALL);
/* Init thread */
g_thread_init(NULL);
gtk_init(&argc, &argv);
Window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
Frame = gtk_frame_new(NULL);
gtk_frame_set_shadow_type(GTK_FRAME(Frame), GTK_SHADOW_OUT);
gtk_container_add(GTK_CONTAINER(Window), Frame);
Frame2 = gtk_frame_new(NULL);
gtk_frame_set_shadow_type(GTK_FRAME(Frame2), GTK_SHADOW_IN);
gtk_container_add(GTK_CONTAINER(Frame), Frame2);
gtk_container_set_border_width(GTK_CONTAINER(Frame2), 5);
adj = (GtkAdjustment*)gtk_adjustment_new(0, 0, 100, 0, 0, 0);
Bar = gtk_progress_bar_new_with_adjustment(adj);
gtk_container_add(GTK_CONTAINER(Frame2), Bar);
gtk_widget_show_all(Window);
if (!g_thread_create(&my_thread, argv[1], FALSE, NULL) != 0)
g_warning("can't create the thread");
gdk_threads_enter();
gtk_main();
gdk_threads_leave();
return 0;
}

编译export PKG_CONFIG_PATH=/usr/lib/pkgconfig/
gcc progress.c –o progress ` pkg-config --libs –cflags gtk+-2..0` -lcurl –lgthread-2.0
./ progress  http://software.sky-union.cn/index.asp
**4、断点续传实例**
[view plain](http://blog.csdn.net/s030702614/article/details/6198641#)
//采用CURLOPT_RESUME_FROM_LARGE 实现文件断点续传功能
\#include <stdlib.h>
\#include <stdio.h>
\#include <sys/stat.h>
\#include <curl/curl.h>
//这个函数为CURLOPT_HEADERFUNCTION参数构造
/* 从http头部获取文件size*/
size_t getcontentlengthfunc(void *ptr, size_t size, size_t nmemb, void *stream) {
int r;
long len = 0;
/* _snscanf() is Win32 specific */
// r = _snscanf(ptr, size * nmemb, "Content-Length: %ld/n", &len);
r = sscanf(ptr, "Content-Length: %ld/n", &len);
if (r) /* Microsoft: we don't read the specs */
*((long *) stream) = len;
return size * nmemb;
}
/* 保存下载文件 */
size_t wirtefunc(void *ptr, size_t size, size_t nmemb, void *stream)
{
return fwrite(ptr, size, nmemb, stream);
}
/*读取上传文件 */
size_t readfunc(void *ptr, size_t size, size_t nmemb, void *stream)
{
FILE *f = stream;
size_t n;
if (ferror(f))
return CURL_READFUNC_ABORT;
n = fread(ptr, size, nmemb, f) * size;
return n;
}
// 下载 或者上传文件函数
int download(CURL *curlhandle, const char * remotepath, const char * localpath,
long timeout, long tries)
{
FILE *f;
curl_off_t local_file_len = -1 ;
long filesize =0 ;
CURLcode r = CURLE_GOT_NOTHING;
int c;
struct stat file_info;
int use_resume = 0;
/* 得到本地文件大小 */
//if(access(localpath,F_OK) ==0)
if(stat(localpath, &file_info) == 0)
{
local_file_len =  file_info.st_size;
use_resume  = 1;
}
//采用追加方式打开文件，便于实现文件断点续传工作
f = fopen(localpath, "ab+");
if (f == NULL) {
perror(NULL);
return 0;
}
//curl_easy_setopt(curlhandle, CURLOPT_UPLOAD, 1L);
curl_easy_setopt(curlhandle, CURLOPT_URL, remotepath);
curl_easy_setopt(curlhandle, CURLOPT_CONNECTTIMEOUT, timeout);  // 设置连接超时，单位秒
//设置http 头部处理函数
curl_easy_setopt(curlhandle, CURLOPT_HEADERFUNCTION, getcontentlengthfunc);
curl_easy_setopt(curlhandle, CURLOPT_HEADERDATA, &filesize);
// 设置文件续传的位置给libcurl
curl_easy_setopt(curlhandle, CURLOPT_RESUME_FROM_LARGE, use_resume?local_file_len:0);
curl_easy_setopt(curlhandle, CURLOPT_WRITEDATA, f);
curl_easy_setopt(curlhandle, CURLOPT_WRITEFUNCTION, wirtefunc);
//curl_easy_setopt(curlhandle, CURLOPT_READFUNCTION, readfunc);
//curl_easy_setopt(curlhandle, CURLOPT_READDATA, f);
curl_easy_setopt(curlhandle, CURLOPT_NOPROGRESS, 1L);
curl_easy_setopt(curlhandle, CURLOPT_VERBOSE, 1L);
r = curl_easy_perform(curlhandle);
fclose(f);
if (r == CURLE_OK)
return 1;
else {
fprintf(stderr, "%s/n", curl_easy_strerror(r));
return 0;
}
}
int main(int c, char **argv) {
CURL *curlhandle = NULL;
curl_global_init(CURL_GLOBAL_ALL);
curlhandle = curl_easy_init();
//download(curlhandle, "ftp://user:pass@host/path/file", "C://file", 0, 3);
download(curlhandle , "http://software.sky-union.cn/index.asp","/work/index.asp",1,3);
curl_easy_cleanup(curlhandle);
curl_global_cleanup();
return 0;
}

编译gcc  resume.c  -o resume –lcurl
./ resume


[
](https://so.csdn.net/so/search/s.do?q=编程&t=blog)
