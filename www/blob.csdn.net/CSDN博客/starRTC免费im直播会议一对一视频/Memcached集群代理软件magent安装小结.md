# Memcached集群代理软件magent安装小结 - starRTC免费im直播会议一对一视频 - CSDN博客
2016年08月26日 18:27:11[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：521
[magent](https://code.google.com/p/memagent/)是一个memcached代理软件(memcached agent)，又叫memagent。（magent
 is a simple but useful proxy program for memcached servers. ）
主页：[https://code.google.com/p/memagent/](https://code.google.com/p/memagent/)
[https://yunpan.cn/cB3NBjiCWEQQf](https://yunpan.cn/cB3NBjiCWEQQf)  访问密码 7869
开始安装
yum install libevent-devel libevent
**mkdir** magent
**cd** magent**/****tar**-zxf magent-0.6.tar.gz**/**sbin**/**ldconfig**sed **-i "s#LIBS = -levent#LIBS = -levent -lm#g" Makefile**make****cp** magent **/**usr**/**bin**/**magent
0.6make错误多，可用0.5版
## 安装时错误解决方法
make时报错
magent.c: In **function** ‘writev_list’:
magent.c:729: error: ‘SSIZE_MAX’ undeclared **(**first use **in** this **function****)**
magent.c:729: error: **(**Each undeclared identifier is reported only once
magent.c:729: error: **for** each **function** it appears in.**)**
make: *********[**magent.o**]** Error 1
解决：
**vim** ketama.h
在开头加入    *#ifndef SSIZE_MAX**#define SSIZE_MAX      32767**#endif*
再次make报错：
gcc: **/**usr**/**lib64**/**libevent.a: No such **file** or directory
gcc: **/**usr**/**lib64**/**libm.a: No such **file** or directory
解决：
**vi** Makefile
找到 LIBS = **/**usr**/**lib64**/**libevent.a **/**usr**/**lib64**/**libm.a这一行
将libevent 的安装路径修改为**/**usr**/**local**/**lib**/** libevent.a
或者**ln**-s**/**usr**/**local**/**lib**/**libevent*******/**usr**/**lib64**/****ln**-s**/**usr**/**lib64**/**libm.so **/**usr**/**lib64**/**libm.a
再次make报错:
**/**usr**/**local**/**lib**/**libevent.a**(**event.o**)**: In **function****`**gettime':
undefined reference to `clock_gettime'
collect2: **ld** returned 1**exit** status
解决：
**vim** Makefile
将  
CFLAGS = -Wall-g-O2 -I**/**usr**/**local**/**include $**(**M64**)**
改为：    
CFLAGS = -lrt-Wall-g-O2 -I**/**usr**/**local**/**include $**(**M64**)**
删除临时文件rm -rf *.o后
再次make成功生成magent
最后cp magent /usr/bin/magent
## [[编辑](http://192.168.1.100/elesos_com/index.php?title=Memcached%E4%BB%A3%E7%90%86%E8%BD%AF%E4%BB%B6magent%E5%AE%89%E8%A3%85%E5%B0%8F%E7%BB%93&action=edit&section=2)]艺搜参考
[http://blog.snsgou.com/post-820.html](http://blog.snsgou.com/post-820.html)
