# linux 更新yum源  改成163源 - weixin_33985507的博客 - CSDN博客
2015年04月18日 22:40:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：5
```
安装完CentOS6.3后，为避免从国外站点安装更新速度过慢，需要更改yum更新源，所以从网上找了下更改linux yum源的方法，和大家进行下分享。原理很简单，就是把yum配置文件中的更新源改一下，这里直接用163的镜像站点中的配置文件。
更改方法
1、进入yum配置文件目录
cd /etc/yum.repos.d/
2、备份配置文件
mv CentOS-Base.repo CentOS-Base.repo.bak
3、下载163的配置
wget http://mirrors.163.com/.help/CentOS6-Base-163.repo，下载下来的文件名为 CentOS6-Base-163.repo
4、改名
mv CentOS6-Base-163.repo CentOS-Base.repo
5、更新数据库
yum update
```
