# 关于arm-linux相关操作 - 囧囧有神的专栏 - CSDN博客





2011年12月27日 23:01:56[囧囧有神](https://me.csdn.net/liujiyong7)阅读数：836标签：[qt																[虚拟机																[ubuntu																[脚本																[图形																[windows](https://so.csdn.net/so/search/s.do?q=windows&t=blog)
个人分类：[linux](https://blog.csdn.net/liujiyong7/article/category/1098485)





                
主机 59.71.94.125 windows XP

虚拟机192.168.1.128 ubuntu 10.10   ftp用户名/密码：ubuntu/1

一、批处理脚本将虚拟机中程序传递到SD卡中

在虚拟机交叉编译环境中编译好的程序，将会放到路径 /home/ubuntu/ftp 下

SD卡插入电脑中后，会挂载为为F盘

通过ftp将虚拟机中程序传递到F盘中，ftp登陆后路径为 /home/ubuntu

/*ftp192.168.1.178to F drive.bat   放在桌面,双击就可方便的将数据从虚拟机中传到SD卡中*/

@echo off

D:

dir

ftp -s:ftp.src

pause

/*ftp.src  放在SD卡中*/

open 192.168.1.178

ubuntu

1

cd ftp

mget *

y

y

y

y

y

y

y

y

y

y

bye

二、虚拟机中，将交叉编译好的程序存放到/home/ubuntu/ftp路径中，清空、显示/home/ubuntu/ftp中的数据

/*mvftp*/

#!/bin/bash

#echo $PATH

mv $1 /home/ubuntu/ftp

/*cpftp*/

#!/bin/bash

#echo $PATH

cp $1 /home/ubuntu/ftp

/*lsftp*/

#!/bin/bash

#echo $PATH

ls /home/ubuntu/ftp

/*llftp*/

#!/bin/bash

#echo $PATH

ls -l /home/ubuntu/ftp

/*delftp*/

#!/bin/bash

#echo $PATH

cd /home/ubuntu/ftp;rm *

/*cdftp*/  执行时 . cdftp 即可改变路径到 /home/ubuntu/ftp下

cd /home/ubuntu/ftp

这些简单的脚本，会让自己以后操作更简单，脚本以后也会越写越多。

三、内核的配置，编译

1.解压内核源码包

ubuntu图形界面下直接右键->解压缩到此处

命令解压 tar -jxvf  ***.tar.bz2

2.内核配置

传统字符界面配置 make config 

优点是所有linux都有，通用，缺点是效率太低了，enter键会将手指累死

make menuconfig 界面稍微好看点，用空格键选择

make xconfig 图形界面最方便，但需要安装qt, 执行make xconfig后会提示你安装qt那个版本

具体配置按个人需求，可参看使用手册

3安装qt

qt3     
 sudo apt-get install qt3-dev-tools qt3-examples  python-qt3    qt3-designer qt3-assistant

qt4      sudo apt-get install qt4-dev-tools python-qt4  qt4-designer](https://so.csdn.net/so/search/s.do?q=图形&t=blog)](https://so.csdn.net/so/search/s.do?q=脚本&t=blog)](https://so.csdn.net/so/search/s.do?q=ubuntu&t=blog)](https://so.csdn.net/so/search/s.do?q=虚拟机&t=blog)](https://so.csdn.net/so/search/s.do?q=qt&t=blog)




