
# Linux 添加用户并添加组过程 - 萌萌的It人 www.itmmd.com - CSDN博客


2014年11月16日 20:37:05[Jlins](https://me.csdn.net/dyllove98)阅读数：1202


1、创建一个用户名和密码都为hadoop的用户:
useradd hadoop
passwd hadoop
1、我们添置名为hadoops有组
groupadd hadoops
2、将hadoop用户添加到hadoops组里面
gpasswd -a hadoop hadoops
Adding user hadoop to group hadoops --出现这行表示添加成功
说明：把hadoop用户退回出hadoops组
gpasswd -d hadoop hadoops
Removing user hadoop from group hadoops--出现这行表示移除成功
![](http://images.cnitblog.com/blog/437282/201411/041405283617635.gif)
欢迎大家访问我的个人网站[萌萌的IT人](http://www.itmmd.com)

