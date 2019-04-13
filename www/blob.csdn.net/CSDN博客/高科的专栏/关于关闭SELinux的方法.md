
# 关于关闭SELinux的方法 - 高科的专栏 - CSDN博客

2015年03月06日 15:16:05[高科](https://me.csdn.net/pbymw8iwm)阅读数：496标签：[SELInux																](https://so.csdn.net/so/search/s.do?q=SELInux&t=blog)个人分类：[linux																](https://blog.csdn.net/pbymw8iwm/article/category/863753)


原贴:http://www.diybl.com/course/6_system/linux/Linuxjs/2008629/129166.html
关闭SELinux的方法：
修改/etc/selinux/config文件中的SELINUX="" 为 disabled ，然后重启。
如果不想重启系统，使用命令setenforce 0
注：
setenforce 1 设置SELinux 成为enforcing模式
setenforce 0 设置SELinux 成为permissive模式
在lilo或者grub的启动参数中增加：selinux=0,也可以关闭selinux
\#---------------------------------------------------------------
查看selinux状态：
/usr/bin/setstatus -v
如下：
SELinux status:                 enabled
SELinuxfs mount:                /selinux
Current mode:                   permissive
Mode from config file:          enforcing
Policy version:                 21
Policy from config file:        targeted
getenforce/setenforce查看和设置SELinux的当前工作模式

