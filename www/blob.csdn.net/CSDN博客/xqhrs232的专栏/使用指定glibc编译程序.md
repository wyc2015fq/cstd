# 使用指定glibc编译程序 - xqhrs232的专栏 - CSDN博客
2017年02月22日 15:52:21[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：848
原文地址::[http://blog.csdn.net/jefbai/article/details/47859335](http://blog.csdn.net/jefbai/article/details/47859335)
相关文章
1、[glibc的编译和调试](http://blog.chinaunix.net/uid-20786208-id-4980168.html) ----[http://blog.chinaunix.net/uid-20786208-id-4980168.html](http://blog.chinaunix.net/uid-20786208-id-4980168.html)
有时候我们需要[测试](http://lib.csdn.net/base/softwaretest)不同的glibc的性能，此时可以使用如下命令来指定动态库加载器和glibc，
` g++ test_log.c -Wl,--rpath=/usr/local/lib -Wl,--dynamic-linker=/usr/local/lib/ld-linux-x86-64.so.2 `- 1
- 1
其中/usr/local/lib为glibc动态库的路径，linker为动态装载器 
以下test_log.c代码为例
```cpp
/*
 *  learn for syslog/openlog 
 *
 *
 * */
#include <syslog.h>
#include <stdio.h>
#include <stdlib.h>
int main(int argc,char **argv)
{
    //check the argc,usage e.g. ./a.out filename 
    if(argc != 2){
        perror("argc != 2");
        exit(1);    
    }
    openlog(argv[0],LOG_PID | LOG_NOWAIT | LOG_NDELAY,LOG_USER);
    syslog(LOG_INFO,"%s","hello,world");
    closelog();
}
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
- 16
- 17
- 18
- 19
- 20
- 21
- 22
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
- 16
- 17
- 18
- 19
- 20
- 21
- 22
使用上述命令编译后，可以用/usr/bin/ldd查看a.out依赖的动态库
```
[jefby@localhost work]$ /usr/bin/ldd a.out 
    linux-vdso.so.1 =>  (0x00007fff3d9ff000)
    libstdc++.so.6 => /usr/lib64/libstdc++.so.6 (0x0000003c67400000)
    libm.so.6 => /usr/local/lib/libm.so.6 (0x00007fdf9bc5c000)
    libgcc_s.so.1 => /lib64/libgcc_s.so.1 (0x0000003c67000000)
    libc.so.6 => /usr/local/lib/libc.so.6 (0x00007fdf9b8b8000)
    /usr/local/lib/ld-linux-x86-64.so.2 => /lib64/ld-linux-x86-64.so.2 (0x0000003c5a000000)
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 1
- 2
- 3
- 4
- 5
- 6
- 7
说明已经替换成功，运行
`./a.out jefby`- 1
- 1
然后使用tail来查看是否成功写入系统日志中
`sudo tail /var/log/messages`- 1
- 1
如下所示，可以看到已经成功写入：
```
[jefby@localhost work]$ ./a.out jefby
[jefby@localhost work]$ sudo tail /var/log/messages -n 1
Aug 22 14:30:02 localhost ./a.out[12965]: hello,world
```
