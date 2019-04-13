
# docker基础：使用非root用户操作docker - 知行合一 止于至善 - CSDN博客

2019年03月18日 23:19:35[liumiaocn](https://me.csdn.net/liumiaocn)阅读数：263


这篇文章用于介绍使用非root用户操作docker。
# 场景
根据企业的安全策略，有一些情况下无法获取root用户的直接使用方式，通过docker或者sudo的方式使用docker变成了这种情况下的选择之一。
# 参考内容
根据参考文章中的docker的官方内容，有如下信息说明。
`Manage Docker as a non-root user
The Docker daemon binds to a Unix socket instead of a TCP port. By default that Unix socket is owned by the user root and other users can only access it using sudo. The Docker daemon always runs as the root user.
If you don’t want to preface the docker command with sudo, create a Unix group called docker and add users to it. When the Docker daemon starts, it creates a Unix socket accessible by members of the docker group.`从上述信息可以清楚地看到，关于如何使用非root用户对docker进行管理，由于Docker的守护进程会绑定Unix Socket而不是使用TCP端口方式，而缺省情况下Unix socket为root用户所有，其他用户只能通过sudo来进行访问，所以Docker的守护进程只能以root用户进行启动。
如果不希望在docker命令前面添加sudo进行执行，这时可以创建一个名为docker的Unix Group并向这个Group中添加用户，当Docker守护进程启动的时候，守护进程会创建一个Unix Socket文件，而docker这个group的成员对于该文件具有访问权限。
# 场景1：使用sudo方式操作docker
以用户appman为示例，来说明一下如何使用sudo方式操作docker。
事前确认信息如下：不存在/var/lib/docker目录和appman用户
```python
[
```
```python
root@host118 ~
```
```python
]
```
```python
# ls -ld /var/lib/docker
```
```python
ls: cannot access /var/lib/docker: No such
```
```python
file
```
```python
or directory
```
```python
[
```
```python
root@host118 ~
```
```python
]
```
```python
# id appman
```
```python
id: appman: no such user
```
```python
[
```
```python
root@host118 ~
```
```python
]
```
```python
#
```
## Step 1: 创建appman用户和group
如下创建该appman用户和group并初始化用户密码
```python
[
```
```python
root@host118 ~
```
```python
]
```
```python
# useradd appman
```
```python
[
```
```python
root@host118 ~
```
```python
]
```
```python
# id appman
```
```python
uid
```
```python
=
```
```python
1003
```
```python
(
```
```python
appman
```
```python
)
```
```python
gid
```
```python
=
```
```python
1003
```
```python
(
```
```python
appman
```
```python
)
```
```python
groups
```
```python
=
```
```python
1003
```
```python
(
```
```python
appman
```
```python
)
```
```python
[
```
```python
root@host118 ~
```
```python
]
```
```python
# passwd appman
```
```python
Changing password
```
```python
for
```
```python
user appman.
New password: 
BAD PASSWORD: The password is shorter than 7 characters
Retype new password: 
passwd: all authentication tokens updated successfully.
```
```python
[
```
```python
root@host118 ~
```
```python
]
```
```python
#
```
## Step 2: 添加appman的sudo用户权限
修改/etcsudoers文件，添加如下信息
`appman  ALL=(ALL)       ALL`
## Step 3: 使用sudo命令操作docker
切换到sudo用户appman，通过添加sudo前缀操作docker
`[appman@host118 ~]$ id
uid=1003(appman) gid=1003(appman) groups=1003(appman) context=unconfined_u:unconfined_r:unconfined_t:s0-s0:c0.c1023
[appman@host118 ~]$`启动docker
```python
[
```
```python
appman@host118 ~
```
```python
]
```
```python
$
```
```python
sudo
```
```python
systemctl restart docker
```
```python
[
```
```python
sudo
```
```python
]
```
```python
password
```
```python
for
```
```python
appman:
```
```python
[
```
```python
appman@host118 ~
```
```python
]
```
```python
$
```
确认启动状态
```python
[
```
```python
appman@host118 ~
```
```python
]
```
```python
$
```
```python
sudo
```
```python
docker
```
```python
ps
```
```python
CONTAINER ID        IMAGE               COMMAND             CREATED             STATUS              PORTS               NAMES
```
```python
[
```
```python
appman@host118 ~
```
```python
]
```
```python
$
```
```python
sudo
```
```python
docker version
Client:
 Version:      1.13.1
 API version:  1.26
 Go version:   go1.7.5
 Git commit:   092cba3
 Built:        Wed Feb  8 08:47:51 2017
 OS/Arch:      linux/amd64
Server:
 Version:      1.13.1
 API version:  1.26
```
```python
(
```
```python
minimum version 1.12
```
```python
)
```
```python
Go version:   go1.7.5
 Git commit:   092cba3
 Built:        Wed Feb  8 08:47:51 2017
 OS/Arch:      linux/amd64
 Experimental:
```
```python
false
```
```python
[
```
```python
appman@host118 ~
```
```python
]
```
```python
$
```
而如果不加sudo，则会提示错误
```python
[
```
```python
appman@host118 ~
```
```python
]
```
```python
$ docker
```
```python
ps
```
```python
Got permission denied
```
```python
while
```
```python
trying to connect to the Docker daemon socket at unix:///var/run/docker.sock: Get http://%2Fvar%2Frun%2Fdocker.sock/v1.26/containers/json: dial unix /var/run/docker.sock: connect: permission denied
```
```python
[
```
```python
appman@host118 ~
```
```python
]
```
```python
$
```
继续确认文件权限，会发现此种方式和直接使用docker没有区别，因为毕竟sudo就是切换到root用户而已。
`[appman@host118 ~]$ ls -l /var/run/docker.sock
srw-rw----. 1 root root 0 Mar 12 08:39 /var/run/docker.sock
[appman@host118 ~]$ ls -l /var/lib/docker
ls: cannot open directory /var/lib/docker: Permission denied
[appman@host118 ~]$ sudo ls -l /var/lib/docker
total 0
drwx------. 2 root root  6 Mar 12 08:39 containers
drwx------. 3 root root 21 Mar 12 08:39 image
drwxr-x---. 3 root root 19 Mar 12 08:39 network
drwx------. 2 root root  6 Mar 12 08:39 overlay
drwx------. 4 root root 32 Mar 12 08:39 plugins
drwx------. 2 root root  6 Mar 12 08:39 swarm
drwx------. 2 root root  6 Mar 12 08:39 tmp
drwx------. 2 root root  6 Mar 12 08:39 trust
drwx------. 2 root root 25 Mar 12 08:39 volumes
[appman@host118 ~]$ sudo ls -ld /var/lib/docker
drwx--x--x. 11 root root 134 Mar 12 08:39 /var/lib/docker
[appman@host118 ~]$`
# 场景2: 使用非root用户管理docker但是不希望直接使用sudo执行操作
事前准备：停止docker，并删除docker的缺省数据目录/var/lib/docker
```python
[
```
```python
root@host118 ~
```
```python
]
```
```python
# systemctl stop docker
```
```python
[
```
```python
root@host118 ~
```
```python
]
```
```python
# rm -rf /var/lib/docker
```
```python
[
```
```python
root@host118 ~
```
```python
]
```
```python
#
```
事前准备：删除appman的sudo权限
```python
[
```
```python
root@host118 ~
```
```python
]
```
```python
# vi /etc/sudoers
```
```python
[
```
```python
root@host118 ~
```
```python
]
```
```python
# grep appman /etc/sudoers
```
```python
[
```
```python
root@host118 ~
```
```python
]
```
```python
#
```
可以看到appman没有了sudo权限
```python
[
```
```python
appman@host118 ~
```
```python
]
```
```python
$
```
```python
sudo
```
```python
ls
```
```python
[
```
```python
sudo
```
```python
]
```
```python
password
```
```python
for
```
```python
appman: 
appman is not
```
```python
in
```
```python
the sudoers file.  This incident will be reported.
```
```python
[
```
```python
appman@host118 ~
```
```python
]
```
```python
$
```
## Step 1: 添加group并设定用户
`[appman@host118 ~]$ id
uid=1003(appman) gid=1003(appman) groups=1003(appman) context=unconfined_u:unconfined_r:unconfined_t:s0-s0:c0.c1023
[appman@host118 ~]$ sudo groupadd docker
[appman@host118 ~]$ sudo gpasswd -a ${USER} docker
Adding user appman to group docker
[appman@host118 ~]$ groups
appman
[appman@host118 ~]$ newgrp docker
[appman@host118 ~]$ groups
docker appman
[appman@host118 ~]$`
## Step2: 启动docker
启动docker，并确认结果
```python
[
```
```python
appman@host118 ~
```
```python
]
```
```python
$ systemctl restart docker
```
```python
==
```
```python
==
```
```python
AUTHENTICATING FOR org.freedesktop.systemd1.manage-units
```
```python
==
```
```python
=
```
```python
Authentication is required to manage system services or units.
Authenticating as: root
Password:
```
```python
==
```
```python
==
```
```python
AUTHENTICATION COMPLETE
```
```python
==
```
```python
=
```
```python
[
```
```python
appman@host118 ~
```
```python
]
```
```python
$
```
进行启动后的docker数据目录确认
```python
[
```
```python
appman@host118 ~
```
```python
]
```
```python
$
```
```python
ls
```
```python
-l /var/lib/docker
ls: cannot
```
```python
open
```
```python
directory /var/lib/docker: Permission denied
```
```python
[
```
```python
appman@host118 ~
```
```python
]
```
```python
$
```
```python
sudo
```
```python
ls
```
```python
-l /var/lib/docker
total 0
drwx------. 2 root root  6 Mar 12 08:52 containers
drwx------. 3 root root 21 Mar 12 08:52 image
drwxr-x---. 3 root root 19 Mar 12 08:52 network
drwx------. 2 root root  6 Mar 12 08:52 overlay
drwx------. 4 root root 32 Mar 12 08:52 plugins
drwx------. 2 root root  6 Mar 12 08:52 swarm
drwx------. 2 root root  6 Mar 12 08:52 tmp
drwx------. 2 root root  6 Mar 12 08:52 trust
drwx------. 2 root root 25 Mar 12 08:52 volumes
```
```python
[
```
```python
appman@host118 ~
```
```python
]
```
```python
$
```
```python
sudo
```
```python
ls
```
```python
-ld /var/lib/docker
drwx--x--x. 11 root root 134 Mar 12 08:52 /var/lib/docker
```
```python
[
```
```python
appman@host118 ~
```
```python
]
```
```python
$
```
## Step 3: 确认其他命令和docker.sock文件
确认docker.sock文件，发现docker在其group设定中，这一点也和官方说明一致。而执行其他命令，也无需再加前缀sudo即可
```python
[
```
```python
appman@host118 ~
```
```python
]
```
```python
$ docker version
Client:
 Version:      1.13.1
 API version:  1.26
 Go version:   go1.7.5
 Git commit:   092cba3
 Built:        Wed Feb  8 08:47:51 2017
 OS/Arch:      linux/amd64
Server:
 Version:      1.13.1
 API version:  1.26
```
```python
(
```
```python
minimum version 1.12
```
```python
)
```
```python
Go version:   go1.7.5
 Git commit:   092cba3
 Built:        Wed Feb  8 08:47:51 2017
 OS/Arch:      linux/amd64
 Experimental:
```
```python
false
```
```python
[
```
```python
appman@host118 ~
```
```python
]
```
```python
$ docker
```
```python
ps
```
```python
CONTAINER ID        IMAGE               COMMAND             CREATED             STATUS              PORTS               NAMES
```
```python
[
```
```python
appman@host118 ~
```
```python
]
```
```python
$
```
# 参考文章
[https://docs.docker.com/install/linux/linux-postinstall/](https://docs.docker.com/install/linux/linux-postinstall/)

