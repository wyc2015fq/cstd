# 为CentOS配置网易163的yum源 - starRTC免费im直播会议一对一视频 - CSDN博客
2017年03月10日 12:30:59[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：370
Yum (Yellow dog Updater, Modified)是一个基于 RPM 包管理的字符前端软件包管理器。能够从指定的服务器自动下载 RPM 包并且安装，可以处理依赖性关系，并且一次安装所有依赖的软件包，无须繁琐地一次次下载、安装。
软件包来源:可供 Yum 下载的软件包包括 Fedora 本身的软件包以及源自 rpmfusion 等非官方软件仓库的软件包，全部是由 Linux 社区维护的，并且基本是自由软件。所有的包都有一个独立的 GPG 签名，主要是为了用户的系统安全。
yum install -y wget
首先备份/etc/yum.repos.d/CentOS-Base.repo
 mv /etc/yum.repos.d/CentOS-Base.repo /etc/yum.repos.d/CentOS-Base.repo.backup
下载 repo：
cd /etc/yum.repos.d/
wget [http://mirrors.163.com/.help/CentOS6-Base-163.repo](http://mirrors.163.com/.help/CentOS6-Base-163.repo)
mv CentOS6-Base-163.repo CentOS-Base.repo
centos7的：
[http://mirrors.163.com/.help/CentOS7-Base-163.repo](http://mirrors.163.com/.help/CentOS7-Base-163.repo)
更新 cache
 yum clean all
 yum makecache
## 艺搜参考
[http://mirrors.163.com/.help/centos.html](http://mirrors.163.com/.help/centos.html)
