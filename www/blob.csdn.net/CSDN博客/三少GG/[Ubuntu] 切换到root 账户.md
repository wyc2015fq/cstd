# [Ubuntu] 切换到root 账户|| - 三少GG - CSDN博客
2011年06月02日 00:16:00[三少GG](https://me.csdn.net/scut1135)阅读数：1597标签：[ubuntu																[unix																[path																[测试																[user](https://so.csdn.net/so/search/s.do?q=user&t=blog)](https://so.csdn.net/so/search/s.do?q=测试&t=blog)](https://so.csdn.net/so/search/s.do?q=path&t=blog)](https://so.csdn.net/so/search/s.do?q=unix&t=blog)](https://so.csdn.net/so/search/s.do?q=ubuntu&t=blog)
个人分类：[Ubuntu/OS X系统](https://blog.csdn.net/scut1135/article/category/680808)
默认root是没有密码的，因为你还没给root设置密码，你安装的时候要求输入的那个用户，也就是第一个 user 是在 admin 组 ，所以他可以给 root 设置密码
linuxidc@linuxidc-laptop:~$ sudo passwd root
[sudo] password for linuxidc:
输入新的 UNIX 口令：
重新输入新的 UNIX 口令：
passwd：已成功更新密码
linuxidc@linuxidc-laptop:~$ 
设定root的密码后，用root帐户登录（在关机键上点击切换用户），root+刚设的密码 
看是不是进入系统了
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
小技巧：
**1.-->设置NDK环境变量：**
1、用root用户登陆，打开命令窗口，输入cd /root
2、输入ls -a，会显示一个隐藏文件.bashrc
3、输入vi .bashrc，按i进入编辑模式
4、文件底部添加以下两行：
NDK_ROOT=/home/ndk。这里指你ndk的目录路径
export NDK_ROOT
5、按esc退出，再按:wq保存
你可以测试一下有没有安装成功：
$ cd $NDK_ROOT
$ ./ndk-build NDK_PROJECT_PATH=$NDK_ROOT/samples/two-libs
找到生成的文件就代表安装正常了。
**2. 然后打开命令窗口，cd ffmpeg源文件夹下。**
$ chmod +x config.sh        /*这个命令是改变config.sh执行权限而已*/
$ ./config.sh
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
