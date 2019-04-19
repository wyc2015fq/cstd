# Linux C++程序解决文件句柄（包括socket句柄等）耗尽问题 - xqhrs232的专栏 - CSDN博客
2016年07月08日 14:30:21[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1491
原文地址::[http://www.2cto.com/kf/201208/145170.html](http://www.2cto.com/kf/201208/145170.html)
相关文章
1、socket资源耗尽问题的追查 - wangsili - 博客园----[http://www.tuicool.com/articles/aMBZjy](http://www.tuicool.com/articles/aMBZjy)
2、服务器出现socket耗尽怎么解决----[http://bbs.csdn.net/topics/390196768](http://bbs.csdn.net/topics/390196768)

最近在线上发现一个问题，程序在服务器上跑了3周后，会出现新日志文件创建不出来的问题，另外，该程序需要使用curl访问后端的HTTP服务调用总是失败。日志文件创建不出来由于不是关键业务，一开始没有做太多关注。但是访问后端HTTP服务失败的问题却是很要命。监控报警一响，我们首先尝试解决问题，就重启了所有程序，问题解决。之后，就立马排查这个问题发生的原因。
在没有重启程序前，首先怀疑的自然是后端HTTP服务有问题（虽然该服务的报警没有响起），一个同事在服务器上命令行尝试curl调用，没有问题（这也排除了DNS问题、防火墙问题，无需再ping和telnet了）。
查询出现问题时服务器使用的CPU、内存量和磁盘剩余空间，都是正常的。
我突然将新日志文件创建不出来的问题和curl调用后端HTTP服务失败的问题联系起来，这两者都需要创建文件句柄（curl需要创建socket句柄），会不会是文件句柄耗尽的问题呢？
测试环境大压力压程序，命令行使用“lsof -c 程序名 | wc -l”，确实发现打开的文件句柄越来越多。问题找到了。接下来就发现是一个函数内socket句柄创建后没有关闭。添加close(fd)后，再次在测试环境测试，打开的文件句柄数量没有出现增长，问题解决。
由于程序中加了设置可以打开文件句柄最大数量为65535，所以，这个问题事前根本没有想到，而且在系统频繁发布（一周两次发布）的情况，根本不会出现。但是当发布频率放缓后，问题就出现了。
因此，Linux程序开发，除了需要关注CPU使用率、内存使用率和硬盘空间外，也需要特别关注一下程序打开的文件句柄释放的问题。
PS：更新打开文件句柄的最大数量程序如下：
[cpp] 
#include <sys/time.h> 
#include <sys/resource.h> 
void max_fd() 
{ 
    struct rlimit rlim, rlim_new; 
    if (getrlimit(RLIMIT_NOFILE, &rlim) == 0) { 
        printf("RLIMIT_NOFILE Old Soft(%d), Hard(%d)\n", rlim.rlim_cur, rlim.rlim_max); 
        if(rlim.rlim_max < 65535 && rlim.rlim_max != RLIM_INFINITY) 
        { 
            rlim_new.rlim_max = 65535; 
        } 
        if(rlim.rlim_cur < 65535 && rlim.rlim_max != RLIM_INFINITY) 
        { 
            rlim_new.rlim_cur = 65535; 
        } 
        if (setrlimit(RLIMIT_NOFILE, &rlim_new) != 0) { 
            rlim_new.rlim_cur = rlim_new.rlim_max = rlim.rlim_max; 
            (void) setrlimit(RLIMIT_NOFILE, &rlim_new); 
            printf("RLIMIT_NOFILE New Soft(%d), Hard(%d)\n", rlim_new.rlim_cur, rlim_new.rlim_max); 
        } 
        else [www.2cto.com](http://www.2cto.com/kf/201208/145170.html)
        { 
            printf("RLIMIT_NOFILE New Soft(%d), Hard(%d)\n", rlim_new.rlim_cur, rlim_new.rlim_max); 
        } 
    } 
} 
作者：huyiyang2010
