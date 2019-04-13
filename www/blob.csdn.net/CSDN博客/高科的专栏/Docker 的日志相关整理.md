
# Docker 的日志相关整理 - 高科的专栏 - CSDN博客

2017年10月10日 12:14:33[高科](https://me.csdn.net/pbymw8iwm)阅读数：1929


**1 Docker daemon日志的位置**
Docker daemon日志的位置，根据系统不同各不相同。
Ubuntu - /var/log/upstart/docker.log
Boot2Docker - /var/log/docker.log
Debian GNU/Linux - /var/log/daemon.log
CentOS - /var/log/daemon.log | grep docker
Fedora - journalctl -u docker.service
Red Hat Enterprise Linux Server - /var/log/messages | grep docker
**2 Docker各个容器内部的日志位置**
一般在 /var/lib/docker/containers 目录下来查看各个容器，至于是哪个容器对应那个目录，可以用docker ps 查看一下当前容器的id


