
# 使用flannel配置docker网络 - 知行合一 止于至善 - CSDN博客

2019年03月27日 20:31:17[liumiaocn](https://me.csdn.net/liumiaocn)阅读数：179所属专栏：[深入浅出kubernetes](https://blog.csdn.net/column/details/12761.html)



这篇文章主要总结一下如何在使用flannel进行docker的配置，介绍docker尚未安装或者是已经安装的情况下，如何与flannel进行连接，并以一个具体的例子介绍一下如何在docker和flannel分别安装的情况下进行设定。
# 主要思路
主要需要确认的是docker0是否在flannel的设定下统一进行容器IP的管理和分配。
# 全新docker安装
flannel在安装时，一般会在systemd的service文件的ExecStartPost中设定如下执行内容：
> [mk-docker-opts.sh](http://mk-docker-opts.sh)
> -k DOCKER_NETWORK_OPTIONS -d /run/flannel/docker
这样则会生成/run/flannel/docker文件, 在这个文件中一般会给docker提供一个设定参数DOCKER_NETWORK_OPTIONS，DOCKER_NETWORK_OPTIONS中会包含如下设定信息：
--bip
--ip-masq
--mtu
而在docker的systemd的service中，只需要将DOCKER_NETWORK_OPTIONS传递给dockerd即可。
# 已有安装
## 事前准备
docker详细信息如下所示（centos下的yum install docker的版本1.13）
```python
[
```
```python
root@host131 shell
```
```python
]
```
```python
# docker version
```
```python
Client:
 Version:         1.13.1
 API version:     1.26
 Package version: docker-1.13.1-91.git07f3374.el7.centos.x86_64
 Go version:      go1.10.3
 Git commit:      07f3374/1.13.1
 Built:           Wed Feb 13 17:10:12 2019
 OS/Arch:         linux/amd64
Server:
 Version:         1.13.1
 API version:     1.26
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
Package version: docker-1.13.1-91.git07f3374.el7.centos.x86_64
 Go version:      go1.10.3
 Git commit:      07f3374/1.13.1
 Built:           Wed Feb 13 17:10:12 2019
 OS/Arch:         linux/amd64
 Experimental:
```
```python
false
```
```python
[
```
```python
root@host131 shell
```
```python
]
```
```python
#
```
flannel的版本为v0.10.0
```python
[
```
```python
root@host131 shell
```
```python
]
```
```python
# flanneld --version
```
```python
v0.10.0
```
```python
[
```
```python
root@host131 shell
```
```python
]
```
```python
#
```
flannel 和docker分别安装，可以看到flannel.1和docker0所在网段不同
```python
[
```
```python
root@host131 shell
```
```python
]
```
```python
# ip addr show flannel.1
```
```python
5: flannel.1:
```
```python
<
```
```python
BROADCAST,MULTICAST,UP,LOWER_UP
```
```python
>
```
```python
mtu 1450 qdisc noqueue state UNKNOWN group default 
    link/ether 22:22:02:92:1e:74 brd ff:ff:ff:ff:ff:ff
    inet 172.200.240.0/32 scope global flannel.1
       valid_lft forever preferred_lft forever
    inet6 fe80::2022:2ff:fe92:1e74/64 scope
```
```python
link
```
```python
valid_lft forever preferred_lft forever
```
```python
[
```
```python
root@host131 shell
```
```python
]
```
```python
#
```
```python
[
```
```python
root@host131 shell
```
```python
]
```
```python
# ip addr show docker0
```
```python
6: docker0:
```
```python
<
```
```python
NO-CARRIER,BROADCAST,MULTICAST,UP
```
```python
>
```
```python
mtu 1500 qdisc noqueue state DOWN group default 
    link/ether 02:42:5f:2b:90:6b brd ff:ff:ff:ff:ff:ff
    inet 172.17.0.1/16 scope global docker0
       valid_lft forever preferred_lft forever
```
```python
[
```
```python
root@host131 shell
```
```python
]
```
```python
#
```
## 确定设定文件
确认如下文件是否存在
```python
[
```
```python
root@host131 shell
```
```python
]
```
```python
# cat /run/flannel/docker
```
```python
DOCKER_OPT_BIP
```
```python
=
```
```python
"--bip=172.200.240.1/21"
```
```python
DOCKER_OPT_IPMASQ
```
```python
=
```
```python
"--ip-masq=false"
```
```python
DOCKER_OPT_MTU
```
```python
=
```
```python
"--mtu=1450"
```
```python
DOCKER_NETWORK_OPTIONS
```
```python
=
```
```python
" --bip=172.200.240.1/21 --ip-masq=false --mtu=1450"
```
```python
[
```
```python
root@host131 shell
```
```python
]
```
```python
# cat /run/flannel/subnet.env
```
```python
FLANNEL_NETWORK
```
```python
=
```
```python
172.200.0.0/16
FLANNEL_SUBNET
```
```python
=
```
```python
172.200.240.1/21
FLANNEL_MTU
```
```python
=
```
```python
1450
FLANNEL_IPMASQ
```
```python
=
```
```python
true
```
```python
[
```
```python
root@host131 shell
```
```python
]
```
```python
#
```
如果不存在的情况下，考虑如下操作：
重启flanneld：systemctl restart flanneld
执行命令：[mk-docker-opts.sh](http://mk-docker-opts.sh)-k DOCKER_NETWORK_OPTIONS -d /run/flannel/docker
## 设定docker0
使用subnet.env的内容进行docker0设定
```python
[
```
```python
root@host131 shell
```
```python
]
```
```python
# source /run/flannel/subnet.env
```
```python
[
```
```python
root@host131 shell
```
```python
]
```
```python
# echo ${FLANNEL_SUBNET}
```
```python
172.200.240.1/21
```
```python
[
```
```python
root@host131 shell
```
```python
]
```
```python
# ifconfig docker0 ${FLANNEL_SUBNET}
```
```python
[
```
```python
root@host131 shell
```
```python
]
```
```python
#
```
## 重启docker
重启docker后即可
```python
[
```
```python
root@host131 shell
```
```python
]
```
```python
# ip ad s docker0
```
```python
7: docker0:
```
```python
<
```
```python
NO-CARRIER,BROADCAST,MULTICAST,UP
```
```python
>
```
```python
mtu 1500 qdisc noqueue state DOWN group default 
    link/ether 02:42:6f:f5:6e:a2 brd ff:ff:ff:ff:ff:ff
    inet 172.200.240.1/21 scope global docker0
       valid_lft forever preferred_lft forever
```
```python
[
```
```python
root@host131 shell
```
```python
]
```
```python
#
```
# 启动后恢复缺省的ip的对应方法
经常会碰到的问题是ifconfig 命令之后修改成功，但是重启之后又恢复原装，可以考虑如下方式进行解决。
## 删除docker0设备并重启
执行示例如下所示
```python
[
```
```python
root@host131 shell
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
root@host131 shell
```
```python
]
```
```python
# ip link set dev docker0 down
```
```python
[
```
```python
root@host131 shell
```
```python
]
```
```python
# brctl delbr docker0
```
```python
[
```
```python
root@host131 shell
```
```python
]
```
```python
# ip a s docker0
```
```python
Device
```
```python
"docker0"
```
```python
does not exist.
```
```python
[
```
```python
root@host131 shell
```
```python
]
```
```python
# systemctl restart docker
```
```python
[
```
```python
root@host131 shell
```
```python
]
```
```python
#
```
## 修改docker的service文件并重启docker
centos下缺省安装的docker，由于环境变量文件中将DOCKER_NETWORK_OPTIONS进行了设定，即使你在外部进行了source操作，docker在重启的时候仍然会将你读入的正确设定设成缺省的""，所以会恢复原装。
```python
[
```
```python
root@host131 shell
```
```python
]
```
```python
# cat /usr/lib/systemd/system/docker.service
```
```python
[
```
```python
Unit
```
```python
]
```
```python
Description
```
```python
=
```
```python
Docker Application Container Engine
Documentation
```
```python
=
```
```python
http://docs.docker.com
After
```
```python
=
```
```python
network.target
Wants
```
```python
=
```
```python
docker-storage-setup.service
Requires
```
```python
=
```
```python
docker-cleanup.timer
```
```python
[
```
```python
Service
```
```python
]
```
```python
Type
```
```python
=
```
```python
notify
NotifyAccess
```
```python
=
```
```python
main
EnvironmentFile
```
```python
=
```
```python
-/run/containers/registries.conf
EnvironmentFile
```
```python
=
```
```python
-/etc/sysconfig/docker
EnvironmentFile
```
```python
=
```
```python
-/etc/sysconfig/docker-storage
EnvironmentFile
```
```python
=
```
```python
-/etc/sysconfig/docker-network
```
```python
..
```
```python
.省略
```
```python
[
```
```python
root@host131 shell
```
```python
]
```
```python
#
```
/etc/sysconfig/docker-network
```python
[
```
```python
root@host131 shell
```
```python
]
```
```python
# cat /etc/sysconfig/docker-network
```
```python
# /etc/sysconfig/docker-network
```
```python
DOCKER_NETWORK_OPTIONS
```
```python
=
```
```python
[
```
```python
root@host131 shell
```
```python
]
```
```python
#
```
修改方法
按照本文提示的第一种方式，设定此参数，示例如下
EnvironmentFile=-/etc/sysconfig/docker-network
EnvironmentFile=-/run/flannel/docker重启并确认
[root@host131 shell]\# systemctl daemon-reload[root@host131 shell]\# systemctl restart docker[root@host131 shell]\# ip addr show docker07: docker0:<NO-CARRIER,BROADCAST,MULTICAST,UP>mtu 1500 qdisc noqueue state DOWN group default 
    link/ether 02:42:6f:f5:6e:a2 brd ff:ff:ff:ff:ff:ff
    inet 172.200.240.1/21 scope global docker0
       valid_lft forever preferred_lft forever[root@host131 shell]\#

