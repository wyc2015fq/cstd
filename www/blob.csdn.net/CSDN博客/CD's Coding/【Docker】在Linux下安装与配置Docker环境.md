# 【Docker】在Linux下安装与配置Docker环境 - CD's Coding - CSDN博客





2018年11月20日 11:17:32[糖果天王](https://me.csdn.net/okcd00)阅读数：529








## 0x00 前言

没有太多说的，这个可能也到处都有…… 每个人的方法大概也大同小异？

主要是自己第一次接触docker，就当作简单的做一个记录与总结吧。

（注意：本篇和以前的环境配置不同，遇到的问题过多，而且最终不是自己独立完成

（故主要着眼于出现问题的记录与过程，过程中记下来一些笔记，以及心得体会。
运气好的同学可以直接尝试简单配置法

（这里的“运气”包括网络稳定性与源的可联通性）：

```
sudo curl -fsSL https://get.docker.com | bash -s docker --mirror Aliyun
```

## 0x01 自己的胡乱配置

### 查看当前系统版本

> 
**Linux 系统下，版本需要不小于 3.10.x**

下述输出的意思是：系统为`Linux`，名称为`c9`，版本为`3.10.0`，64位系统


```
[chend@c9 ~]$ uname -a
Linux c9 3.10.0-693.el7.x86_64 #1 SMP Tue Aug 22 21:09:27 UTC 2017 x86_64 x86_64 x86_64 GNU/Linux
```

### 好像确实没有装过docker

> 
如果没装过docker的话，这么run一下会告诉你没装过的

哦当然 `docker -h` 也可以用来看看自己装过没


```
[chend@c9 ~]$ ./bin/run_docker -p 8081 -u
please install docker, see https://docs.docker.com/engine/installation
```

### 改成root权限登入

> 
安装的时候会需要用户组之类的权限设置，所以需要root权限


```
[chend@c9 ~]$ sudo su
[root@c9 ~]#
```

### 取消selinux

> 
不取消的话会有一些已知的冲突情况，不过我vim的时候发现它竟然本来就是`disabled`


```
[root@c9 ~]# vim /etc/selinux/config
SELINUX=disabled
SELINUXTYPE=targeted
```

### 配置Fedora EPEL源 & hop5.in源

> 
EPEL源必要，其它的源各位可以凭喜好添加


```
[root@c9 ~] yum install http://ftp.riken.jp/Linux/fedora/epel/6/i386/epel-release-6-8.noarch.rpm
[root@c9 ~] cd /etc/yum.repos.d
[root@c9 yum.repos.d] wget http://www.hop5.in/yum/el6/hop5.repo
```

### 安装

> 
听说大家装完这一步就可以直接成功可以 `docker -h` 测试了，羡慕不已


```
[root@c9 yum.repos.d] yum install docker-io
```

### more space needed on the /boot filesystem

> 
喜闻乐见报了错呀，可怜的我配环境的时候怎么总是不能一下装好呢

这次的错误是 `/boot` 盘下空间不足


![在这里插入图片描述](https://img-blog.csdnimg.cn/201811191636026.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L29rY2QwMA==,size_16,color_FFFFFF,t_70)

### 清理 /boot 释放空间

> 
网上说可以通过dpkg来找到那些过时的内核删掉腾出空间，然而——

我没有这个`dpkg`命令……


```
[chend@c9 ~]$ dpkg
bash: dpkg: command not found...
```

寻找半天，无果，我不是Debian系统没有dpkg呀，

还是手动跑去/boot里剪切了一些看起来不怎么用到的东西出来（带.bak的）。

```
[chend@c9 ~]$ cd /boot
[chend@c9 boot]$ ls
config-3.10.0-693.el7.x86_64  grub2                                                    initramfs-3.10.0-693.el7.x86_64.img.bak   symvers-3.10.0-693.el7.x86_64.gz                   vmlinuz-3.10.0-693.el7.x86_64
efi                           initramfs-0-rescue-11f968f6f6144b53a5daa8495063de5e.img  initramfs-3.10.0-693.el7.x86_64kdump.img  System.map-3.10.0-693.el7.x86_64
grub                          initramfs-3.10.0-693.el7.x86_64.img                      initrd-plymouth.img                       vmlinuz-0-rescue-11f968f6f6144b53a5daa8495063de5e
[chend@c9 boot]$ ls -lht
total 169M
-rw-------. 1 root root  19M May 30 20:26 initramfs-3.10.0-693.el7.x86_64kdump.img
-rw-------. 1 root root  30M May 30 18:39 initramfs-3.10.0-693.el7.x86_64.img
drwx------. 5 root root   97 May 30 03:34 grub2
-rw-------. 1 root root  30M May 30 03:34 initramfs-3.10.0-693.el7.x86_64.img.bak
-rwxr-xr-x. 1 root root 5.7M May 30 03:32 vmlinuz-0-rescue-11f968f6f6144b53a5daa8495063de5e
-rw-------. 1 root root  67M May 30 03:32 initramfs-0-rescue-11f968f6f6144b53a5daa8495063de5e.img
-rw-r--r--. 1 root root 9.8M May 30 03:30 initrd-plymouth.img
drwxr-xr-x. 2 root root   27 May 30 03:22 grub
drwxr-xr-x. 3 root root   17 May 30 03:20 efi
-rw-r--r--. 1 root root 287K Aug 23  2017 symvers-3.10.0-693.el7.x86_64.gz
-rwxr-xr-x. 1 root root 5.7M Aug 23  2017 vmlinuz-3.10.0-693.el7.x86_64
-rw-r--r--. 1 root root 138K Aug 23  2017 config-3.10.0-693.el7.x86_64
-rw-------. 1 root root 3.1M Aug 23  2017 System.map-3.10.0-693.el7.x86_64
[chend@c9 boot]$ mv initramfs-3.10.0-693.el7.x86_64.img.bak /data/chend/
downloads/           error_detector_data/ 
[chend@c9 boot]$ mv initramfs-3.10.0-693.el7.x86_64.img.bak /data/chend/
downloads/           error_detector_data/ 
[chend@c9 boot]$ mv initramfs-3.10.0-693.el7.x86_64.img.bak /data/chend/
mv: cannot open ‘initramfs-3.10.0-693.el7.x86_64.img.bak’ for reading: Permission denied
[chend@c9 boot]$ sudo su
[root@c9 boot]# mv initramfs-3.10.0-693.el7.x86_64.img.bak /data/chend/
```

### 回头继续安装docker

```
# yum install docker-io
# docker -h
```

![在这里插入图片描述](https://img-blog.csdnimg.cn/20181119165233874.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L29rY2QwMA==,size_16,color_FFFFFF,t_70)

### Unit not found

> 
哎呀终于装起来了，那跑起来跑起来……

一波未平一波又起…… `Unit not found`


```
[root@c9 boot]# sudo service docker start
Redirecting to /bin/systemctl start docker.service
Failed to start docker.service: Unit not found.
```

> 
关于Unit not found的问题，查了查[资料](https://www.jb51.net/article/111680.htm)

照着试了试感觉没有太大效果，（后记：其实是因为`docker-common`冲突）

怀疑大概是docker-io不好用，改装普通的docker试试看？
```
$ sudo yum remove docker-io
$ sudo yum -y install docker
```

### Job for docker.service failed

> 
总之就是处于一种装好了但就是start不起来的悲惨境地


```
[chend@c9 AutoDoc]$ sudo systemctl start docker.service
Job for docker.service failed because the control process exited with error code. See "systemctl status docker.service" and "journalctl -xe" for details.

[chend@c9 AutoDoc]$ sudo systemctl status docker.service
● docker.service - Docker Application Container Engine
   Loaded: loaded (/usr/lib/systemd/system/docker.service; disabled; vendor preset: disabled)
   Active: failed (Result: exit-code) since Mon 2018-11-19 19:33:14 CST; 1min 51s ago
     Docs: http://docs.docker.com
  Process: 1900 ExecStart=/usr/bin/dockerd-current --add-runtime docker-runc=/usr/libexec/docker/docker-runc-current --default-runtime=docker-runc --exec-opt native.cgroupdriver=systemd --userland-proxy-path=/usr/libexec/docker/docker-proxy-current --init-path=/usr/libexec/docker/docker-init-current --seccomp-profile=/etc/docker/seccomp.json $OPTIONS $DOCKER_STORAGE_OPTIONS $DOCKER_NETWORK_OPTIONS $ADD_REGISTRY $BLOCK_REGISTRY $INSECURE_REGISTRY $REGISTRIES (code=exited, status=1/FAILURE)
 Main PID: 1900 (code=exited, status=1/FAILURE)

Nov 19 19:33:14 c9 systemd[1]: Starting Docker Application Container Engine...
Nov 19 19:33:14 c9 dockerd-current[1900]: Error starting daemon: pid file found, ensure docker is not running or delete /var/run/docker.pid
Nov 19 19:33:14 c9 systemd[1]: docker.service: main process exited, code=exited, status=1/FAILURE
Nov 19 19:33:14 c9 systemd[1]: Failed to start Docker Application Container Engine.
Nov 19 19:33:14 c9 systemd[1]: Unit docker.service entered failed state.
Nov 19 19:33:14 c9 systemd[1]: docker.service failed.
```

### docker自动配置脚本

> 
这次打算试试 `get.docker.com` 的自动脚本，curl你怎么又出问题了……


配置脚本如下（curl正常的同学用这一行就足够了）：
`sudo curl -fsSL https://get.docker.com | bash -s docker --mirror Aliyun`

```
[chend@c9 download]$ sudo curl -fsSL https://get.docker.com | bash -s docker --mirror Aliyun
curl: (35) Encountered end of file
[chend@c9 download]$  curl --version
curl 7.29.0 (x86_64-redhat-linux-gnu) libcurl/7.29.0 NSS/3.28.4 zlib/1.2.7 libidn/1.28 libssh2/1.4.3
Protocols: dict file ftp ftps gopher http https imap imaps ldap ldaps pop3 pop3s rtsp scp sftp smtp smtps telnet tftp 
Features: AsynchDNS GSS-Negotiate IDN IPv6 Largefile NTLM NTLM_WB SSL libz unix-sockets 
[chend@c9 download]$ sudo curl -fsSL https://get.docker.com | bash -s docker
curl: (35) Encountered end of file
[chend@c9 download]$ curl -fsSL https://get.docker.com | bash -s docker     
curl: (35) Encountered end of file
```

## 0x02 等来救援，配置由此开始

### curl报错检查问题出处

> 
经查证，这次curl的问题属于 `NSS is the same`的问题
[https://superuser.com/questions/916077/what-is-nss-error-5961-pr-connect-reset-error](https://superuser.com/questions/916077/what-is-nss-error-5961-pr-connect-reset-error)
[https://unix.stackexchange.com/questions/259152/curl-fails-with-ntlm-proxy](https://unix.stackexchange.com/questions/259152/curl-fails-with-ntlm-proxy)
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181120105008199.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L29rY2QwMA==,size_16,color_FFFFFF,t_70)

### 重新ssh连入，解决无法下载的问题

> 
无计可施，于是去搬了救兵，感谢三位热心的大神带我逃离苦海QvQ

问了问dalao，该怎么办——

唉？重连ssh这是什么情况 0.0，和重启一个道理么？（后面有提及）
![在这里插入图片描述](https://img-blog.csdnimg.cn/2018111921192750.png)

### 做一个甩手掌柜，让dalao帮忙试试

> 
专业的就是不一样…… 以及实在对不起把这么麻烦的任务交给你了QvQ

总结一下的话就是：`官方源难以连通`、`注意冲突依赖项`、`占用同服务进程`


![在这里插入图片描述](https://img-blog.csdnimg.cn/20181120104557133.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L29rY2QwMA==,size_16,color_FFFFFF,t_70)

### 其它忠告

> 
然后dalao和dalao之间开始了愉快的交流，

总结一下就是：`docker官方源不稳定`、`docker-compose不要用yum来装`、`scp代替直连下载`


![在这里插入图片描述](https://img-blog.csdnimg.cn/20181120104826197.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L29rY2QwMA==,size_16,color_FFFFFF,t_70)

### groups & newgrp docker

> 
这次是安装好了，那首先需要登陆一下 `docker login https://xxx.xxx.com`

登陆前要记得把自己的用户加用户组：
`sudo usermod -aG docker $USER`
`sudo gpasswd -a chend docker`

但是我把我自己加过docker用户组了为什么还是不行呢？
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181120110901438.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L29rY2QwMA==,size_16,color_FFFFFF,t_70)

> 
然后请来了大师答疑解惑——
**@kdr2**: 主要是 newgrp 这个，一般你加了组后，得重新登录才行

newgrp 就是不退出当前 shell 直接登进那个组，你再执行 groups 就能看到 docker 组了
```
$ sudo usermod -aG docker $USER
$ sudo gpasswd -a chend docker
$ groups
$ newgrp docker
$ docker login
```

## 0xFF 成功用上了Docker

> 
太不容易了QvQ，记录一下，好好看好好学，学习道路还很长……

一些常用的docker指令: [https://blog.csdn.net/dongdong9223/article/details/52998375](https://blog.csdn.net/dongdong9223/article/details/52998375)


![在这里插入图片描述](https://img-blog.csdnimg.cn/20181120111542159.png)














