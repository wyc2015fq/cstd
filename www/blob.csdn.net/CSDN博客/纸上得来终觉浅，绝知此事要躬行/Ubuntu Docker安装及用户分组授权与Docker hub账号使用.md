# Ubuntu Docker安装及用户分组授权与Docker hub账号使用 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2017年07月18日 12:01:15[boonya](https://me.csdn.net/boonya)阅读数：1289








我之前用的boot2docker镜像快速实现的Linux轻量提供，现在在Ubuntu上安装docker，网络采用NAT模式（个人比较习惯用此方式，桥接的方式比较麻烦，很多apt安装失败）。有时在Ubuntu安装docker需要耐心，安装过程会比较慢，安装完成后要将Linux用户加到docker用户分组才能正常使用docker命令。

## Ubuntu安装Docker

命令：sudo apt-get install -y docker.io



```
boonya@ubuntu:/usr/local/docker$ sudo apt-get install docker.io
[sudo] password for boonya: 
Reading package lists... Done
Building dependency tree       
Reading state information... Done
The following extra packages will be installed:
  apparmor aufs-tools cgroupfs-mount git git-man liberror-perl
Suggested packages:
  apparmor-profiles apparmor-docs apparmor-utils btrfs-tools debootstrap lxc rinse git-daemon-run git-daemon-sysvinit git-doc git-el git-email git-gui gitk gitweb git-arch git-cvs
  git-mediawiki git-svn
The following NEW packages will be installed:
  apparmor aufs-tools cgroupfs-mount docker.io git git-man liberror-perl
0 upgraded, 7 newly installed, 0 to remove and 87 not upgraded.
Need to get 8,408 kB of archives.
After this operation, 46.8 MB of additional disk space will be used.
Do you want to continue? [Y/n] Y
Get:1 http://us.archive.ubuntu.com/ubuntu/ vivid/main apparmor amd64 2.9.1-0ubuntu9 [352 kB]
Get:2 http://us.archive.ubuntu.com/ubuntu/ vivid/universe aufs-tools amd64 1:3.2+20130722-1.1 [92.3 kB]                                                                                       
Get:3 http://us.archive.ubuntu.com/ubuntu/ vivid/universe cgroupfs-mount all 1.1 [4,796 B]                                                                                                    
Get:4 http://us.archive.ubuntu.com/ubuntu/ vivid-updates/universe docker.io amd64 1.6.2~dfsg1-1ubuntu4~15.04.1 [4,420 kB]                                                                     
Get:5 http://us.archive.ubuntu.com/ubuntu/ vivid-updates/universe docker.io amd64 1.6.2~dfsg1-1ubuntu4~15.04.1 [4,420 kB]                                                                     
Get:6 http://us.archive.ubuntu.com/ubuntu/ vivid/main liberror-perl all 0.17-1.1 [21.1 kB]                                                                                                    
Get:7 http://us.archive.ubuntu.com/ubuntu/ vivid-updates/main git-man all 1:2.1.4-2.1ubuntu0.1 [701 kB]                                                                                       
Get:8 http://us.archive.ubuntu.com/ubuntu/ vivid-updates/main git-man all 1:2.1.4-2.1ubuntu0.1 [701 kB]                                                                                       
Get:9 http://us.archive.ubuntu.com/ubuntu/ vivid-updates/main git amd64 1:2.1.4-2.1ubuntu0.1 [2,817 kB]                                                                                       
Get:10 http://us.archive.ubuntu.com/ubuntu/ vivid-updates/main git amd64 1:2.1.4-2.1ubuntu0.1 [2,817 kB]                                                                                      
Get:11 http://us.archive.ubuntu.com/ubuntu/ vivid-updates/main git amd64 1:2.1.4-2.1ubuntu0.1 [2,817 kB]                                                                                      
Get:12 http://us.archive.ubuntu.com/ubuntu/ vivid-updates/main git amd64 1:2.1.4-2.1ubuntu0.1 [2,817 kB]                                                                                      
Get:13 http://us.archive.ubuntu.com/ubuntu/ vivid-updates/main git amd64 1:2.1.4-2.1ubuntu0.1 [2,817 kB]                                                                                      
Fetched 6,145 kB in 46min 44s (2,190 B/s)                                                                                                                                                     
Preconfiguring packages ...
Selecting previously unselected package apparmor.
(Reading database ... 76408 files and directories currently installed.)
Preparing to unpack .../apparmor_2.9.1-0ubuntu9_amd64.deb ...
Unpacking apparmor (2.9.1-0ubuntu9) ...
Selecting previously unselected package aufs-tools.
Preparing to unpack .../aufs-tools_1%3a3.2+20130722-1.1_amd64.deb ...
Unpacking aufs-tools (1:3.2+20130722-1.1) ...
Selecting previously unselected package cgroupfs-mount.
Preparing to unpack .../cgroupfs-mount_1.1_all.deb ...
Unpacking cgroupfs-mount (1.1) ...
Selecting previously unselected package docker.io.
Preparing to unpack .../docker.io_1.6.2~dfsg1-1ubuntu4~15.04.1_amd64.deb ...
Unpacking docker.io (1.6.2~dfsg1-1ubuntu4~15.04.1) ...
Selecting previously unselected package liberror-perl.
Preparing to unpack .../liberror-perl_0.17-1.1_all.deb ...
Unpacking liberror-perl (0.17-1.1) ...
Selecting previously unselected package git-man.
Preparing to unpack .../git-man_1%3a2.1.4-2.1ubuntu0.1_all.deb ...
Unpacking git-man (1:2.1.4-2.1ubuntu0.1) ...
Selecting previously unselected package git.
Preparing to unpack .../git_1%3a2.1.4-2.1ubuntu0.1_amd64.deb ...
Unpacking git (1:2.1.4-2.1ubuntu0.1) ...
Processing triggers for man-db (2.7.0.2-5) ...
Processing triggers for ureadahead (0.100.0-19) ...
Processing triggers for systemd (219-7ubuntu3) ...
Setting up apparmor (2.9.1-0ubuntu9) ...
update-rc.d: warning: start and stop actions are no longer supported; falling back to defaults
insserv: script mysql.server.sh: service mysql already provided!
Setting up aufs-tools (1:3.2+20130722-1.1) ...
Setting up cgroupfs-mount (1.1) ...
insserv: script mysql.server.sh: service mysql already provided!
Setting up docker.io (1.6.2~dfsg1-1ubuntu4~15.04.1) ...
insserv: script mysql.server.sh: service mysql already provided!
Setting up liberror-perl (0.17-1.1) ...
Setting up git-man (1:2.1.4-2.1ubuntu0.1) ...
Setting up git (1:2.1.4-2.1ubuntu0.1) ...
Processing triggers for libc-bin (2.21-0ubuntu4) ...
```



## Docker为当前用户授权

命令：$sudo gpasswd -a ${USER} docker



```
boonya@ubuntu:/usr/local/docker$ sudo gpasswd -a root  docker
Adding user root to group docker
```

注意：授权后需要重启系统reboot。

若不进行授权，使用docker的其他命令会报如下错误 ：




```
boonya@ubuntu:/usr/local/docker$ docker images
FATA[0000] Get http:///var/run/docker.sock/v1.18/images/json: dial unix /var/run/docker.sock: permission denied. Are you trying to connect to a TLS-enabled daemon without TLS?
```




## 查看Docker的版本

命令：docker -v | docker --version




```
root@ubuntu:/home/boonya# docker --version
Docker version 1.6.2, build 7c8fca2
root@ubuntu:/home/boonya#
```


## 查看Docker进程

命令：ps -ef | grep docker




```
root@ubuntu:/home/boonya# ps -ef | grep docker
root       588     1  0 17:13 ?        00:00:20 /usr/bin/docker -d -H fd://
root@ubuntu:/home/boonya#
```

## Docker对容器的启动停止和重启操作

### 启动已被停止的容器container

命令：docker start container



### 停止运行中的容器container


命令：docker stop container



### 重启容器container


命令：docker restart container




## Docker Hub账号注册

[https://hub.docker.com/](https://hub.docker.com/)

![](https://img-blog.csdn.net/20170718121343271)




## Docker Hub命令行常用操作

### 登录



命令：docker login




```
root@ubuntu:/home/boonya# docker login
Username: boonyadocker
Password: 
Email: boonya@sina.com
WARNING: login credentials saved in /root/.dockercfg.
Login Succeeded
root@ubuntu:/home/boonya#
```



### 登出

命令：docker logout



```
root@ubuntu:/home/boonya# docker logout
Remove login credentials for https://index.docker.io/v1/
root@ubuntu:/home/boonya#
```



### Push镜像到仓库



命令： docker  push    id/tag





注意：id必须是注册的账号名称，tag为你的镜像名。







