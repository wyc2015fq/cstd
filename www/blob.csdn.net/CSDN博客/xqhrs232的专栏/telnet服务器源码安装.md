# telnet服务器源码安装 - xqhrs232的专栏 - CSDN博客
2016年08月09日 16:43:24[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1094
原文地址::[http://blog.csdn.net/xiaoxiaozhu2010/article/details/18229591](http://blog.csdn.net/xiaoxiaozhu2010/article/details/18229591)
相关文章
1、Arm-Linux下使用的ftp服务和telnet服务1  ----[http://download.csdn.net/download/yeah2000/4740765](http://download.csdn.net/download/yeah2000/4740765)
2、linux下telnet源代码  ----[http://download.csdn.net/download/songcheng_zhang/6668815](http://download.csdn.net/download/songcheng_zhang/6668815)
3、utelnetd-0.1.9  ----[http://download.csdn.net/download/sjdi521/6370855](http://download.csdn.net/download/sjdi521/6370855)
4、telnet 客户端和服务端源代码  ----[http://download.csdn.net/download/hudong1984/1490004](http://download.csdn.net/download/hudong1984/1490004)
一、ftp server
选用wu-ftp
1。首先到ftp://ftp.wu-ftpd.org/pub/wu-ftpd/下载wu-ftpd-2.6.2.tar.Z
2。把wu-ftp解压缩后，进入解压缩的目录，执行export CC=arm-linux-gcc
3。执行./configure --host=arm-linux
4。执行make，编译完成后，在bin目录下会生成ckconfig  ftpcount  ftpd  ftprestart  ftpshut  ftpwho六个文件，其中ftpd是我们想要的。
5。把ftpd拷贝到nfs文件系统的/usr/sbin目录下面。
6。把wu-ftp目录下的doc/examples目录里面的东西拷贝到nfs文件系统的/etc/wu-ftpd目录下。
7。修改nfs目录下面的etc下的inetd.conf,把里面的ftp行改成下面形式：
     ftp     stream  tcp     nowait  root    /usr/sbin/ftpd  ftpd
8。启动开发板连接nfs，然后adduser -G ftpuser user
问题：
1. "yacc" command not found  
sudo apt-get install byacc.
二、telent server
utelnetd-0.1.9.tar.gz
修改makefile文件的编译程序
make
三、修改端口号
./ftpd -p 25 &
./utelnetd -p 22 &
这两个程序下载地址如下：
http://download.csdn.net/download/yeah2000/4740765
