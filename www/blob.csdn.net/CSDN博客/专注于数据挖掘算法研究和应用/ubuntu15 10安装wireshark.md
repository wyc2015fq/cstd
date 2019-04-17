# ubuntu15.10安装wireshark - 专注于数据挖掘算法研究和应用 - CSDN博客





2016年02月25日 17:18:27[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：2582









一、在线安装:sudo apt-get install wireshark


问题一：普通用户执行没有权限，也打不开网络端口捕捉，因dumpcap需要root权限。


解决办法：为使普通用户可抓包，赋予普通用户权限执行wireshark：


1、添加wireshark用户组 

sudo groupadd wireshark 


2、将dumpcap更改为wireshark用户组 

sudo chgrp wireshark /usr/bin/dumpcap 


3、让wireshark用户组有root权限使用dumpcap 

sudo chmod 4755 /usr/bin/dumpcap


(注意:如果设为4754 Wireshark还是会提示没有权限 ) 


4、将需要使用的普通用户名加入wireshark用户组：

sudo gpasswd -a yourname wireshark


如此，可以以普通用户yourname登陆打开Wireshark抓包。


问题二：成功抓到包（包计数在增加），但封包列表没有列表展示，正常情况每个包应该有列表展示；


暂无解决办法，通过编译安装来发现问题，初步怀疑是GTK和QT未安装原因。


卸载在线安装的wireshark：$ sudo apt-get remove --purge wireshark


笔者曾在ubuntu11版本上通过在线安装成功，但之前可能已经安装过相关组件。


二、编译安装


1、官网下载wireshark-2.0.1.tar.bz2并解压进入目录编译，编译命令如下： 



$./configure

$make

$sudo make install


2、执行 ./configure时会提示缺失的组件，虚拟机ubuntu15.10环境下，需补充安装：


    1）$sudo apt-get install build-essential  -gcc/g++环境，已有

      2）$sudo apt-get install checkinstall     -管理编译软件，已有，升级

      3）$sudo apt-get install libgtk2.0-dev libglib2.0-dev   -gtk+2版本无效

$sudo
 apt-get install libgtk-3-dev  --根据提示安装gtk+3版本

     4）sudo apt-get install flex bison
  

yacc(Yet Another Compiler Compiler)，是Unix/Linux上一个用来生成编译器的编译器（编译器代码生成器）。

 
    5）$sudo apt-get install libqt4-dev  -安装qt

       6）$sudo apt-get install libpcap-dev  -安装libpcap


       如要编译安装libpcap，官网www.tcpdump.org下载源码， 参见：http://blog.csdn.net/fjssharpsword/article/details/6765626



3、执行 $sudo ./wireshark





