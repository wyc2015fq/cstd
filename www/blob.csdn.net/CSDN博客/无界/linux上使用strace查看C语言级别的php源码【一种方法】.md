
# linux上使用strace查看C语言级别的php源码【一种方法】 - 无界 - CSDN博客

置顶2012年11月18日 00:06:04[21aspnet](https://me.csdn.net/21aspnet)阅读数：15198


如果你希望看到C语言级别的php代码就需要使用strace这个默认是安装了的，如果没有安装可以
\#yum install strace
查看httpd进程
\#ps auxw | grep httpd
![](https://img-my.csdn.net/uploads/201211/18/1353168384_5257.jpg)
有多个，必须停止apache
[root@localhost usr]\# /usr/local/webserver/apache2/bin/apachectl stop
启动单进程httpd
[root@localhost usr]\# /usr/local/webserver/apache2/bin/apachectl -X -k start
再使用\#ps auxw | grep httpd查看只有单经常，记下进程id
将strace绑定至apache
\#strace -p 28224

**算法**
快速排序php代码

```python
<?php
       function quickSort($arr)
       {
           $len    = count($arr);
           if($len <= 1) {
               return $arr;
           }
           $key = $arr[0];
           $left_arr    = array();
           $right_arr    = array();
           for($i=1; $i<$len; $i++){
               if($arr[$i] <= $key){
                   $left_arr[] = $arr[$i];
               } else {
                   $right_arr[] = $arr[$i];
               }
           }
           $left_arr    = quickSort($left_arr);
           $right_arr    = quickSort($right_arr);
           return array_merge($left_arr, array($key), $right_arr);
       }

       $arr = array(6,3,8,5,9,2,10);
       echo '<pre>';
       print_r(quickSort($arr));
       ?>
```

在浏览器请求php页面，得到追踪信息
![](https://img-my.csdn.net/uploads/201211/18/1353168650_1695.jpg)

**注意：如果看C语言生成的文件直接strace即可**
**\#strace**./a.out

如果你只要看php的opcode可以参考：
### [PHP安装与使用VLD查看opcode代码【一种方法】        ](http://blog.csdn.net/21aspnet/article/details/7002644)
最简单的php代码
$a='123';
echo $a;

追踪输出：
![](https://img-my.csdn.net/uploads/201211/18/1353207532_9844.jpg)
我们清晰的看到完整的解析流程
从eAccelerator中去读取缓存：
调用open()函数打开文件-->flock()函数锁定文件-->read()函数读取文件内容-->flock()解除锁定-->close()关闭文件
writev()输出http协议头http/1.1  200等等
关于eAccelerator参考[ PHP安装eAccelerator](http://blog.csdn.net/21aspnet/article/details/8194030)
----------------------------------------------------
**flock() 函数 锁定文件或解除锁定**

定义：
int flock(int fd,int operation);
表头文件：
\#include<sys/file.h>
说明：
flock()会依参数operation所指定的方式对参数fd所指的文件做各种锁定或解除锁定的动作。此函数只能锁定整个文件, 无法锁定文件的某一区域。
参数：
operation有下列四种情况:
LOCK_SH 建立共享锁定。多个进程可同时对同一个文件作共享锁定。
LOCK_EX 建立互斥锁定。一个文件同时只有一个互斥锁定。
LOCK_UN 解除文件锁定状态。
LOCK_NB 无法建立锁定时, 此操作可不被阻断, 马上返回进程。通常与LOCK_SH或LOCK_EX 做OR(|)组合。
单一文件无法同时建立共享锁定和互斥锁定, 而当使用dup()或fork()时文件描述词不会继承此种锁定。
返回值：
返回0表示成功, 若有错误则返回-1, 错误代码存于errno。
----------------------------------------------------------
**静态HTML**
我们请求一个html静态文件，追踪信息
![](https://img-my.csdn.net/uploads/201211/18/1353247339_1113.jpg)
可见直接是open()函数去读静态文件
==============================
**linux函数分析查询工具**
1.优先推荐linux 中man命令
2.一个不错的中文Linux手册：[http://cpp.ezbty.org/manpage](http://cpp.ezbty.org/manpage)
3.在线查英文Man手册：
[http://www.kernel.org/doc/man-pages/](http://www.kernel.org/doc/man-pages/)
[http://man7.org/linux/man-pages/dir_all_alphabetic.html](http://man7.org/linux/man-pages/dir_all_alphabetic.html)

[http://linux.about.com/od/commands/l/blcmdl.htm](http://linux.about.com/od/commands/l/blcmdl.htm)
[http://linux.die.net/man/](http://linux.die.net/man/)
[http://www.linuxmanpages.com/](http://www.linuxmanpages.com/)
==============================
**通用函数**
**gettimeofday****取得目前的时间**
定义：
int gettimeofday ( struct timeval * tv , struct timezone * tz )
表头文件：
\#include <sys/time.h>
\#include <unistd.h>

gettimeofday()会把目前的时间有tv所指的结构返回, 当地时区的信息则放到tz所指的结构中。
timeval结构定义为:
struct timeval{
long tv_sec; /*秒*/
long tv_usec; /*微秒*/
};
timezone 结构定义为:
struct timezone{
int tz_minuteswest; /*和Greenwich 时间差了多少分钟*/
int tz_dsttime; /*日光节约时间的状态*/
};

```python
#include<sys/time.h>
#include<unistd.h>
main(){
	struct timeval tv;
	struct timezone tz;
	gettimeofday (&tv , &tz);
	printf("tv_sec; %d\n", tv,.tv_sec) ;
	printf("tv_usec; %d\n",tv.tv_usec);
	printf("tz_minuteswest; %d\n", tz.tz_minuteswest);
	printf("tz_dsttime, %d\n",tz.tz_dsttime);
}
```
tv_sec: 974857339
tv_usec:136996
tz_minuteswest:-540
tz_dsttime:0
=======================================================================
**poll 函数**
http://linux.about.com/library/cmd/blcmdl2_poll.htm
Linux / Unix Command:**poll**

## NAME
poll - wait for some event on a file descriptor
## SYNOPSIS
**\#include <****sys/poll.h****>**
**int poll(struct pollfd ****ufds***, unsigned int***nfds***, int***timeout***);**
## DESCRIPTION
**poll**is a variation on the theme of**select**. It specifies an array of*nfds*structures of type
struct pollfd {
                int fd;           /* file descriptor */
                short events;     /* requested events */
                short revents;    /* returned events */
        };-------------------------**write****将数据写入已打开的文件内**
定义：
ssize_t write (int fd,const void * buf,size_t count);
表头文件：
\#include<unistd.h>
说明：
write()会把参数buf所指的内存写入count个字节到参数fd所指的文件内。当然, 文件读写位置也会随之移动。
------------------------------
Linux / Unix Command:**writev**
http://linux.about.com/library/cmd/blcmdl2_writev.htm

## NAME
readv, writev - read or write a vector
## SYNOPSIS
**\#include <****sys/uio.h****>**
**ssize_t readv(int***fd***, const struct iovec ****vector***, int***count***);**
**ssize_t writev(int***fd***, const struct iovec ****vector***, int***count***);**
**struct iovec {    void ****iov_base***;**/* Starting address */**size_t***iov_len***;**/* Length in bytes  */**};**----------------------------
使用man命令查**getsockname**
![](https://img-my.csdn.net/uploads/201211/18/1353211530_7842.jpg)

其他函数：
### [ACCEPT()和ACCEPT4()        ](http://blog.csdn.net/21aspnet/article/details/8196671)
getuid取得用户标识[getcwd](http://man7.org/linux/man-pages/man2/getcwd.2.html)获取当前目录[rt_sigaction(2)](http://man7.org/linux/man-pages/man2/rt_sigaction.2.html)检查和改变信号动作，**sigaction**() 系统调用被用于更改一个进程在接收一个特定信号后的动作。[setitimer](http://man7.org/linux/man-pages/man2/setitimer.2.html)获取设置定时器的值[poll(2)](http://man7.org/linux/man-pages/man2/poll.2.html)在文件描述符上等待事件[stat64](http://man7.org/linux/man-pages/man2/stat64.2.html)获取文件状态[chdir](http://man7.org/linux/man-pages/man2/chdir.2.html)改变工作目录



