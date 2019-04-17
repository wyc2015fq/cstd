# Linux root用户及权限管理 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2016年12月27日 15:25:00[boonya](https://me.csdn.net/boonya)阅读数：9024








对于Linux root用户的管理操作很重要，至少修改密码要会吧。



## 添加用户



命令：adduser boonya  #添加一个名为boonya的用户


## 修改密码

命令：passwd boonya   #修改boonya密码

注意：需要输入两次密码。

## 删除用户

命令：userdel -r boonya  #删除用户boonya 在系统文件中(主要是/etc/passwd, /etc/shadow, /etc/group等)的记录,同时删除用户的主目录。




## 赋予root权限



方法一：修改 /etc/sudoers 文件，找到下面一行，把前面的注释（#）去掉

## Allows people in group wheel to run all commands

%wheel    ALL=(ALL)    ALL

然后修改用户，使其属于root组（wheel），命令如下：

usermod -g root boonya

修改完毕，现在可以用boonya帐号登录，然后用命令 su root，即可获得root权限进行操作。

**方法二：**修改 /etc/sudoers 文件，找到下面一行，在root下面添加一行，如下所示：

## Allow root to run any commands anywhere

root    ALL=(ALL)     ALL

boonya ALL=(ALL)     ALL

修改完毕，现在可以用boonya帐号登录，然后用命令 sudo su 输入密码，即可获得root权限进行操作。

**方法三：**修改 /etc/passwd 文件，找到如下行，把用户ID修改为 0 ，如下所示：

boonya:x:0:33:tommy:/data/webroot:/bin/bash

## 授予目录权限

命令：chmod 777 /home/boonya    #仅把/home/user目录的权限设置为rwxrwxrwx

命令：chmod -R 777 /home/boonya   #表示将整个/home/user目录与其中的文件和子目录的权限都设置为rwxrwxrwx




## 授予*sh文件权限

一般操作文件没有权限会提示：./*.sh: Permission denied

处理命令：chmod u+x *.sh






