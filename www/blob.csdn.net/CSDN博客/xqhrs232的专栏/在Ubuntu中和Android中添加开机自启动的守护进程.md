# 在Ubuntu中和Android中添加开机自启动的守护进程 - xqhrs232的专栏 - CSDN博客
2011年03月15日 14:17:00[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：946标签：[android																[ubuntu																[service																[linux内核																[终端																[linux](https://so.csdn.net/so/search/s.do?q=linux&t=blog)](https://so.csdn.net/so/search/s.do?q=终端&t=blog)](https://so.csdn.net/so/search/s.do?q=linux内核&t=blog)](https://so.csdn.net/so/search/s.do?q=service&t=blog)](https://so.csdn.net/so/search/s.do?q=ubuntu&t=blog)](https://so.csdn.net/so/search/s.do?q=android&t=blog)
个人分类：[Android/Java](https://blog.csdn.net/xqhrs232/article/category/906925)
原文地址::[http://blog.csdn.net/vrix/archive/2009/09/05/4522675.aspx](http://blog.csdn.net/vrix/archive/2009/09/05/4522675.aspx)
昨天和今天实验了向Android中添加一个守护进程，鼓捣了2天，小有点收获，自己编写的进程添加进这两个操作系统的开机启动中了。但离完全成功似乎还有些距离。另外今天还看了下解压、修改Android的ramdisk.img的方法。
先把我的守护进程（daemon09.c）发到这里
/************************程序开始**************************/
#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/time.h>
#include<sys/resource.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
int main()
{
    int i=0,fd;
    pid_t pid;
    struct rlimit rl;
    char str[]="Writing Test is going.../n";
    umask(0);
    if(getrlimit(RLIMIT_NOFILE,&rl)<0)
        printf("getrlimit函数调用出现错误！/n");
    if((pid=fork())<0)
    {
        printf("fork出现错误！/n");
        exit(1);
    }
    else if(pid>0)//父进程退出
    {
        printf("父进程退出，它的ID为%d，它的子进程ID为%d/n",getpid(),pid);
        exit(0);
    }
    //子进程
    sleep(2);
    printf("子进程ID为%d/n",getpid());
    setsid();
    if((pid=fork())<0)
    {
        printf("fork出现错误！/n");
        return -1;
    }
    else if(pid>0)//第一个子进程退出
    {
        printf("第一个子进程退出，它的ID为%d，它的子进程ID为%d/n",getpid(),pid); /*这个printf的内容可以被输出，貌似是因为它所在的进程虽然失去了终端，但它是一个会话组的首进程，因此看到有printf后，自己又申请了一个终端？*/
        exit(0);
    }
    //第二个子进程
    printf("不会输出这一行。/n");/*这个printf的内容将不会在屏幕上输出，原因可能是因为它所在的进程此时已经不是一个会话组的首进程，无法重新申请获得终端？*/
    chdir("/");
    if(rl.rlim_max==RLIM_INFINITY)
        rl.rlim_max=1024;
    printf("%d",(int)rl.rlim_max);
    for(i=0;i<rl.rlim_max;i++)
        close(i);
    open("/dev/null",O_RDWR);
    dup(0);
    dup(0);
    //每隔5s向文件内写入一次数据
    while(1)
    {
        fd=open("/data/deamon.txt ",O_WRONLY|O_CREAT|O_APPEND,0766);/*这里的/data指的是android系统上的/data目录*/
        write(fd,str,sizeof(str));
        sleep(5);
    }
    close(fd);
    exit(0);
}
/************************程序结束**************************/
因为andorid也是基于LINUX内核的，因此我想一开始先试验一下是否可以把这个进程在Ubuntu中开机自启动。
通过在/etc/rc.local文件中加入了一行“/usr/myfiles/test/daemon09”，成功让daemon09在Ubuntu开机时启动了。（注意，这里在LINUX下启动的话，上面的C程序中红字部分可能需要改变一下，因为Ubuntu下没有/data这样一个路径）
在Ubuntu Linux下成功之后，就转而看看在android下是否能行。android启动后的根目录下的init.rc似乎是管这个的（源码目录/system /core/rootdir/init.rc，make后就是它），于是我试着对源文件做修改再make运行，但一开始总是不得要领，不知道 init.rc中的语法是什么。后来在google论坛上找了下，原来init.rc的内容在“android源码路径/system/core /init/readme.txt“内有说明。于是根据那个搞了几个小时（没办法，本人是菜鸟），基本上弄出来了，android模拟器启动后，自己写的进程也成功加入了。方法如下：
在init.rc的那一堆以service开头的语句中插入一段新的service语句。格式如下：
# my_deamon_test是服务名称，后面跟的是你编写的要开机启动的程序及路径，这里的路径是android系统中的路径 
service my_deamon_test /data/deamon_test 
     oneshot(不加这一句似乎不行，少了它可能你的程序启动不起来) 
oneshot选项的含义（在android源码路径/system/core/init/readme.txt内有说明）：
Do not restart the service when it exits. 
＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
另外，这两天在网上看了一些android下的调试技巧，贴到这里备忘
1、解压、修改Android的ramdisk.img的方法
将ramdisk.img复制一份到任何其他目录下，将其名称改为ramdisk.img.gz，并使用命令
gunzip ramdisk.img.gz
然后新建一个文件夹，叫ramdisk吧，进入，输入命令
cpio -i -F ../ramdisk.img 
这下，你就能看见并操作ramdisk里面的内容了。
根据自己的需要对里面的内容修改之后，可以使用下列命令重新打包成镜像
cpio -i -t -F ../ramdisk.img > list
cpio -o -H newc -O lk.img < list　　
当前目录下生成的lk.img就是我们的新镜像了。
本文来自CSDN博客，转载请标明出处：[http://blog.csdn.net/vrix/archive/2009/09/05/4522675.aspx](http://blog.csdn.net/vrix/archive/2009/09/05/4522675.aspx)

