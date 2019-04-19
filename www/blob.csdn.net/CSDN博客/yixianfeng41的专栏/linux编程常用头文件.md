# linux编程常用头文件 - yixianfeng41的专栏 - CSDN博客
2017年02月17日 12:20:25[第2梦](https://me.csdn.net/yixianfeng41)阅读数：668
所属专栏：[Linux/Unix 网络编程](https://blog.csdn.net/column/details/14513.html)
转自：[socket网络编程中的头文件](http://blog.csdn.net/legion8169/article/details/1635510)
### Socket编程中需要用到的头文件
stdlib.h   ： //某些结构体定义和宏定义，如EXIT_FAILURE、EXIT_SUCCESS等
sys/types.h：包含很多类型重定义，如pid_t、int8_t等
sys/socket.h：与套接字相关的函数声明和结构体定义，如socket()、bind()、connect()及struct sockaddr的定义等
netinet/in.h：定义数据结构，如sockaddr_in，PROTO_ICMP、INADDR_ANY等
arpa/inet.h：提供IP地址转换函数，如inet_ntop()、inet_ntoa()等
netdb.h：提供设置及获取域名的结构、宏和函数，如struct hostent、struct servent、gethostbyname()、gethostbyaddr()、herror()等
sys/ioctl.h：提供对I/O控制的函数，如ioctl()
sys/poll.h：提供socket等待测试机制的函数
unistd.h：提供通用的文件、目录、程序及进程操作的函数
errno.h：提供错误号errno的定义，用于错误处理
fcntl.h：提供对文件控制的函数
time.h：提供有关时间的函数
crypt.h：提供使用DES加密算法的加密函数
pwd.h：提供对/etc/passwd文件访问的函数
shadow.h：提供对/etc/shadow文件访问的函数
pthread.h：提供多线程操作的函数
signal.h：提供对信号操作的函数
sys/wait.h、sys/ipc.h、sys/shm.h：提供进程等待、进程间通讯（IPC）及共享内存的函数
建议：在编写网络程序时，可以直接使用下面这段头文件代码
```
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <ctype.h>
#include <errno.h>
#include <malloc.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <stdarg.h>
#include <fcntl.h>
#include <fcntl.h>
```
### 涉及到用户权限及密码验证问题时加入如下语句：
```
#include <shadow.h>
#include <crypt.h>
#include <pwd.h>
```
需要注意的是，应该在编译时链接加密算法库，即增加编译选项：-lcrypt
### 涉及到文件及时间操作加入如下语句：
```
#include <sys/time.h>
#include <utime.h>
#include <time.h>
#include <sys/stat.h>
#include <sys/file.h>
```
### 涉及到多进程操作时加入如下语句：
```
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <signal.h>
```
### 涉及到多线程操作时加入如下语句：
```
#include <pthread.h>
#include <sys/poll.h>
```
需要注意的是，应该在编译时链接线程库，即增加编译选项：-pthread
