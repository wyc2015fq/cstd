
# docker基础：非root用户操作并将数据目录进行nfs mount - 知行合一 止于至善 - CSDN博客

2019年03月19日 07:39:35[liumiaocn](https://me.csdn.net/liumiaocn)阅读数：273标签：[docker																](https://so.csdn.net/so/search/s.do?q=docker&t=blog)[nfs																](https://so.csdn.net/so/search/s.do?q=nfs&t=blog)[mount																](https://so.csdn.net/so/search/s.do?q=mount&t=blog)[非root																](https://so.csdn.net/so/search/s.do?q=非root&t=blog)[普通用户																](https://so.csdn.net/so/search/s.do?q=普通用户&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=非root&t=blog)[
																					](https://so.csdn.net/so/search/s.do?q=mount&t=blog)个人分类：[Docker																](https://blog.csdn.net/liumiaocn/article/category/6328276)
[
																								](https://so.csdn.net/so/search/s.do?q=mount&t=blog)
[
				](https://so.csdn.net/so/search/s.do?q=nfs&t=blog)
[
			](https://so.csdn.net/so/search/s.do?q=nfs&t=blog)
[
		](https://so.csdn.net/so/search/s.do?q=docker&t=blog)

本文记录一下如何使用非root用户进行docker操作，并将数据目录从其他机器进行nfs mount。
# 场景说明
安装docker所在机器由于磁盘空间不足，将数据目录/var/lib/docker进行mount，使用别的机器的空间。在一些企业中由于标准虚机的大小可能已经规范，会产生类似的问题。
# 环境模拟
|host|ip|用途|软件安装|进程启动|
|---|---|---|---|---|
|host118|192.168.163.118|安装了docker的机器|docker与nfs-utils|systemctl start docker|
|host131|192.168.163.131|NFS服务器，提供docker的数据目录的空间|nfs-utils与rpcbind|systemctl start rpcbind 与systemctl start nfs|
docker非root用户的安装设定方式可参看
[https://liumiaocn.blog.csdn.net/article/details/88651955](https://liumiaocn.blog.csdn.net/article/details/88651955)
nfs mount操作具体可参看
[https://blog.csdn.net/liumiaocn/article/details/71157568](https://blog.csdn.net/liumiaocn/article/details/71157568)
事前准备
NFS服务器
事前准备
使用非root用户appman进行操作，事先准备该用户与sudo权限
[root@host131 ~]\# useradd appman[root@host131 ~]\# passwd appmanChanging passwordforuser appman.
New password: 
Retype new password: 
passwd: all authentication tokens updated successfully.[root@host131 ~]\# vi /etc/sudoers[root@host131 ~]\# grep appman /etc/sudoersappman	ALL=(ALL)ALL[root@host131 ~]\#创建用于mount的目录
创建/opt/lib/docker目录用于mount，可以看到此目录所在卷仍有一些使用空间。
[appman@host131 ~]$iduid=1000(appman)gid=1000(appman)groups=1000(appman)context=unconfined_u:unconfined_r:unconfined_t:s0-s0:c0.c1023[appman@host131 ~]$sudomkdir-p /opt/lib/docker[sudo]passwordforappman:[appman@host131 ~]$sudols-l /opt/lib/docker
total 0[appman@host131 ~]$sudols-ld /opt/lib/docker
drwxr-xr-x. 2 root root 6 Feb 16 12:58 /opt/lib/docker[appman@host131 ~]$df/opt/lib/docker
Filesystem              1K-blocks    Used Available Use% Mounted on
/dev/mapper/centos-root  17811456 1304176  16507280   8% /[appman@host131 ~]$注：本文mount源使用了root所在卷，这不是一个好的习惯，仅用于示例使用方式。
设定NFS服务器侧exports
[appman@host131 ~]$cat/etc/exports[appman@host131 ~]$ls-l /etc/exports
-rw-r--r--. 1 root root 0 Jun  7  2013 /etc/exports[appman@host131 ~]$sudovi/etc/exports[sudo]passwordforappman:[appman@host131 ~]$cat/etc/exports
/opt/lib/docker 192.168.163.118(rw,sync,no_root_squash)[appman@host131 ~]$[appman@host131 ~]$sudoexportfs[appman@host131 ~]$确认nfs
[appman@host131 ~]$sudoshowmount -e localhost
Export listforlocalhost:
/opt/lib/docker 192.168.163.118[appman@host131 ~]$docker服务所在机器
事前准备
停下原有docker服务，为了显示此目录为nfs mount之后的目录，演示时删除了原有内容，请根据需要进行备份
[appman@host118 ~]$iduid=1003(appman)gid=1003(appman)groups=1003(appman),1004(docker)context=unconfined_u:unconfined_r:unconfined_t:s0-s0:c0.c1023[appman@host118 ~]$sudols-l /var/lib/docker[sudo]passwordforappman: 
total 0
drwx------. 2 root root  6 Mar 12 08:52 containers
drwx------. 3 root root 21 Mar 12 08:52 image
drwxr-x---. 3 root root 19 Mar 12 08:52 network
drwx------. 2 root root  6 Mar 12 08:52 overlay
drwx------. 4 root root 32 Mar 12 08:52 plugins
drwx------. 2 root root  6 Mar 12 08:52 swarm
drwx------. 2 root root  6 Mar 12 08:52 tmp
drwx------. 2 root root  6 Mar 12 08:52 trust
drwx------. 2 root root 25 Mar 12 08:52 volumes[appman@host118 ~]$ systemctl status docker
● docker.service - Docker Application Container Engine
   Loaded: loaded(/usr/lib/systemd/system/docker.service;enabled;vendor preset: disabled)Active: active(running)since Tue 2019-03-12 08:52:19 CST;53min ago
     Docs: http://docs.docker.com
 Main PID: 7641(dockerd)CGroup: /system.slice/docker.service
           ├─7641 /usr/bin/dockerd -H tcp://0.0.0.0:4243 -H unix:///var/run/docker.sock --selinux-enabled=false --log-opt max-size=1g...
           └─7644 docker-containerd -l unix:///var/run/docker/libcontainerd/docker-containerd.sock --metrics-interval=0 --start-timeo...[appman@host118 ~]$ systemctl stop docker====AUTHENTICATING FOR org.freedesktop.systemd1.manage-units===Authentication is required to manage system services or units.
Authenticating as: root
Password:====AUTHENTICATION COMPLETE===[appman@host118 ~]$sudorm-rf /var/lib/docker[appman@host118 ~]$创建挂载点
[appman@host118 ~]$sudols-l /var/lib/docker
ls: cannot access /var/lib/docker: No suchfileor directory[appman@host118 ~]$sudomkdir-p /var/lib/docker[appman@host118 ~]$sudols-l /var/lib/docker
total 0[appman@host118 ~]$执行挂载
可以使用如下一次性方式进行挂载
mount命令：mount -t nfs 192.168.163.131:/opt/lib/docker /var/lib/docker
但建议写到fstab中，避免因重启导致问题
[appman@host118 ~]$grepdocker /etc/fstab[appman@host118 ~]$sudovi/etc/fstab[appman@host118 ~]$grepdocker /etc/fstab
192.168.163.131:/opt/lib/docker /var/lib/docker nfs rw[appman@host118 ~]$df/var/lib/docker
Filesystem              1K-blocks     Used Available Use% Mounted on
/dev/mapper/centos-root  36805060 34040224   2764836  93% /[appman@host118 ~]$确认是否可进行挂载
[appman@host118 ~]$ showmount -e 192.168.163.131
Export listfor192.168.163.131:
/opt/lib/docker 192.168.163.118[appman@host118 ~]$执行挂载
[appman@host118 ~]$sudomount/var/lib/docker[sudo]passwordforappman:[appman@host118 ~]$df/var/lib/docker
Filesystem                      1K-blocks    Used Available Use% Mounted on
192.168.163.131:/opt/lib/docker  17811456 1304064  16507392   8% /var/lib/docker[appman@host118 ~]$ls-l /var/lib/docker
total 0[appman@host118 ~]$启动docker
[appman@host118 ~]$ systemctl start docker====AUTHENTICATING FOR org.freedesktop.systemd1.manage-units===Authentication is required to manage system services or units.
Authenticating as: root
Password:====AUTHENTICATION COMPLETE===[appman@host118 ~]$ docker version
Client:
 Version:      1.13.1
 API version:  1.26
 Go version:   go1.7.5
 Git commit:   092cba3
 Built:        Wed Feb  8 08:47:51 2017
 OS/Arch:      linux/amd64
Server:
 Version:      1.13.1
 API version:  1.26(minimum version 1.12)Go version:   go1.7.5
 Git commit:   092cba3
 Built:        Wed Feb  8 08:47:51 2017
 OS/Arch:      linux/amd64
 Experimental:false[appman@host118 ~]$[appman@host118 ~]$ls-l /var/lib/docker
ls: cannotopendirectory /var/lib/docker: Permission denied[appman@host118 ~]$sudols-l /var/lib/docker
total 0
drwx------. 2 root root  6 Feb 17 02:01 containers
drwx------. 3 root root 21 Feb 17 02:01 image
drwxr-x---. 3 root root 19 Feb 17 02:01 network
drwx------. 2 root root  6 Feb 17 02:01 overlay
drwx------. 4 root root 32 Feb 17 02:01 plugins
drwx------. 2 root root  6 Feb 17 02:01 swarm
drwx------. 2 root root  6 Feb 17 02:01 tmp
drwx------. 2 root root  6 Feb 17 02:01 trust
drwx------. 2 root root 25 Feb 17 02:01 volumes[appman@host118 ~]$同样此时确认NFS服务器的/opt/lib/docker目录，也会发现刚刚生成的内容
[appman@host131 ~]$sudols/opt/lib/docker
containers  image  network  overlay  plugins  swarm  tmp  trust  volumes[appman@host131 ~]$使用docker load验证一下docker动作
如下所示，动作可以正常执行。
[appman@host118 ~]$ docker images
REPOSITORY          TAG                 IMAGE ID            CREATED             SIZE[appman@host118 ~]$ docker load -i /tmp/alpine.tar
503e53e365f3: Loading layer[==================================================>]5.793 MB/5.793 MB
Loaded image: alpine:3.9[appman@host118 ~]$ docker images
REPOSITORY          TAG                 IMAGE ID            CREATED             SIZE
alpine              3.9                 caf27325b298        5 weeks ago         5.52 MB[appman@host118 ~]$总结
虽然验证了这种方式，建议不要做的这么纠结。

