# ubuntu下环境变量详解 bashrc, profile, environment - 零度的博客专栏 - CSDN博客
2017年06月09日 11:07:39[零度anngle](https://me.csdn.net/zmx729618)阅读数：1923
在Ubuntu中有如下几个文件可以设置环境变量
/etc/profile:在登录时,操作系统定制用户环境时使用的第一个文件,此文件为系统的每个用户设置环境信息,当用户第一次登录时,该文件被执行.
/etc/environment:在登录时操作系统使用的第二个文件,系统在读取你自己的profile前,设置环境文件的环境变量
~/.profile:在登录时用到的第三个文件是.profile文件,每个用户都可使用该文件输入专用于自己使用的shell信息,当用户登录时,该文件仅仅执行一次!默认情况下,他设置一些环境变量,执行用户的.bashrc文件.
/etc/bashrc:为每一个运行bash shell的用户执行此文件.当bash shell被打开时,该文件被读取.
~/.bashrc:该文件包含专用于你的bash shell的bash信息,当登录时以及每次打开新的shell时,该该文件被读取.
通常设置环境变量有三种方法：
一、临时设置
export PATH=/home/yan/share/usr/local/arm/3.4.1/bin:$PATH
二、当前用户的全局设置
打开~/.bashrc，添加行：
export PATH=/home/yan/share/usr/local/arm/3.4.1/bin:$PATH
使生效
source .bashrc
三、所有用户的全局设置
$ vim /etc/profile
在里面加入：
export PATH=/home/yan/share/usr/local/arm/3.4.1/bin:$PATH
使生效
source profile
测试 echo $PATH
