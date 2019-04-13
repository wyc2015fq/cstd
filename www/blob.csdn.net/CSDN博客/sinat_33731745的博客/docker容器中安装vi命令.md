
# docker容器中安装vi命令 - sinat_33731745的博客 - CSDN博客

2017年12月24日 22:37:08[Tao-Tao-Tao](https://me.csdn.net/sinat_33731745)阅读数：306


在使用docker容器时，有时候里边没有安装vim，敲vim命令时提示说：vim: command not found，这个时候就需要安装vim，可是当你敲apt-get install vim命令时，提示：
Reading package lists… Done
Building dependency tree
Reading state information… Done
E: Unable to locate package vim
这时候需要敲：apt-get update，这个命令的作用是：同步 /etc/apt/sources.list 和 /etc/apt/sources.list.d 中列出的源的索引，这样才能获取到最新的软件包。
等更新完毕以后再敲命令：apt-get install vim命令即可。

