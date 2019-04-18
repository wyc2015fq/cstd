# HTop依赖包 - weixin_33985507的博客 - CSDN博客
2016年05月02日 13:33:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：4
htop 是一个 Linux 下的交互式的进程浏览器，可以用来替换Linux下的top命令。
1、安装HTop时需要先安装依赖包：rpmforge-release-0.5.3-1.el6.rf.x86_64.rpm
```
wget http://rpmfind.net/linux/dag/redhat/el7/en/x86_64/dag/RPMS/rpmforge-release-0.5.3-1.el7.rf.x86_64.rpm
rpm -ivh rpmforge-release-0.5.3-1.el7.rf.x86_64.rpm
```
2、然后使用yum安装即可：
```
yum -y install htop
```
3、编译安装下载地址：http://hisham.hm/htop/releases/
```
[root@mycentos ~]# tar zxvf htop-2.2.0.tar.gz
[root@mycentos ~]# cd htop-2.2.0
[root@mycentos ~]# yum -y install ncurses-devel
[root@mycentos ~]# yum -y install gcc
[root@mycentos ~]# ./configure
[root@mycentos ~]# make
[root@mycentos ~]# make instal
```
