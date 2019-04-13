
# Kubernetes安装系列之Docker安装与设定 - 知行合一 止于至善 - CSDN博客

2019年03月28日 04:16:20[liumiaocn](https://me.csdn.net/liumiaocn)阅读数：209所属专栏：[深入浅出kubernetes](https://blog.csdn.net/column/details/12761.html)



这篇文章整理以下Node节点的docker的安装与设定方法，本文以脚本的方式进行固化，内容仍然放在github的easypack上。
# 整体操作
[https://blog.csdn.net/liumiaocn/article/details/88413428](https://blog.csdn.net/liumiaocn/article/details/88413428)
# docker的设定文件
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
# cat /etc/docker/docker.conf
```
```python
DOCKER_OPTS
```
```python
=
```
```python
"--registry-mirror=https://hub-mirror.c.163.com \
-H tcp://0.0.0.0:4243 \
-H unix:///var/run/docker.sock \
--selinux-enabled=false \
--log-opt max-size=1g"
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
# Systemd服务配置文件
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
http://docs.docker.io
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
EnvironmentFile
```
```python
=
```
```python
-/run/flannel/docker
EnvironmentFile
```
```python
=
```
```python
-/etc/docker/docker.conf
ExecStart
```
```python
=
```
```python
/usr/local/bin/dockerd
```
```python
$DOCKER_NETWORK_OPTIONS
```
```python
$DOCKER_OPTS
```
```python
ExecReload
```
```python
=
```
```python
/bin/kill -s HUP
```
```python
$MAINPID
```
```python
Restart
```
```python
=
```
```python
on-failure
RestartSec
```
```python
=
```
```python
5
LimitNOFILE
```
```python
=
```
```python
infinity
LimitNPROC
```
```python
=
```
```python
infinity
LimitCORE
```
```python
=
```
```python
infinity
Delegate
```
```python
=
```
```python
yes
KillMode
```
```python
=
```
```python
process
```
```python
[
```
```python
Install
```
```python
]
```
```python
WantedBy
```
```python
=
```
```python
multi-user.target
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
# 脚本示例
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
# cat step7-install-docker.sh
```
```python
#!/bin/sh
```
```python
.
```
```python
./install.cfg
```
```python
echo
```
```python
-e
```
```python
"\n##  docker service"
```
```python
systemctl stop docker 2
```
```python
>
```
```python
/dev/null
```
```python
mkdir
```
```python
-p
```
```python
${ENV_DOCKER_DIR_BIN}
```
```python
${ENV_DOCKER_DIR_ETC}
```
```python
chmod
```
```python
755
```
```python
${ENV_HOME_DOCKER}
```
```python
/*
```
```python
cp
```
```python
-p
```
```python
${ENV_HOME_DOCKER}
```
```python
/docker*
```
```python
${ENV_DOCKER_DIR_BIN}
```
```python
if
```
```python
[
```
```python
$?
```
```python
-ne 0
```
```python
]
```
```python
;
```
```python
then
```
```python
echo
```
```python
"please check docker binary files existed in
```
```python
${ENV_HOME_DOCKER}
```
```python
/ or not"
```
```python
exit
```
```python
fi
```
```python
# create docker configuration file
```
```python
cat
```
```python
>
```
```python
${ENV_DOCKER_DIR_ETC}
```
```python
/
```
```python
${ENV_DOCKER_ETC}
```
```python
<<
```
```python
EOF
DOCKER_OPTS="--registry-mirror=
```
```python
${ENV_DOCKER_REGISTRY_MIRROR}
```
```python
\\
-H tcp://0.0.0.0:4243 \\
-H unix:///var/run/docker.sock \\
--selinux-enabled=false \\
--log-opt max-size=
```
```python
${ENV_DOCKER_OPT_LOG_MAX_SIZE}
```
```python
"
EOF
```
```python
# Create the docker service.
```
```python
cat
```
```python
>
```
```python
${ENV_DOCKER_SERVICE}
```
```python
<<
```
```python
EOF
[Unit]
Description=Docker Application Container Engine
Documentation=http://docs.docker.io
[Service]
EnvironmentFile=-
```
```python
${ENV_DOCKER_FLANNEL_CONF}
```
```python
EnvironmentFile=-
```
```python
${ENV_DOCKER_DIR_ETC}
```
```python
/
```
```python
${ENV_DOCKER_ETC}
```
```python
ExecStart=
```
```python
${ENV_DOCKER_DIR_BIN}
```
```python
/dockerd \
```
```python
$DOCKER_NETWORK_OPTIONS
```
```python
\
```
```python
$DOCKER_OPTS
```
```python
ExecReload=/bin/kill -s HUP \
```
```python
$MAINPID
```
```python
Restart=on-failure
RestartSec=5
LimitNOFILE=infinity
LimitNPROC=infinity
LimitCORE=infinity
Delegate=yes
KillMode=process
[Install]
WantedBy=multi-user.target
EOF
```
```python
echo
```
```python
-e
```
```python
"\n##  daemon reload service "
```
```python
systemctl daemon-reload
```
```python
echo
```
```python
-e
```
```python
"\n##  start docker service "
```
```python
systemctl start docker
```
```python
echo
```
```python
-e
```
```python
"\n##  enable docker service "
```
```python
systemctl
```
```python
enable
```
```python
docker
```
```python
echo
```
```python
-e
```
```python
"\n##  check  docker status"
```
```python
systemctl status docker
```
```python
echo
```
```python
echo
```
```python
-e
```
```python
"##  check docker version"
```
```python
docker version
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
# 执行示例
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
# sh step7-install-docker.sh
```
```python
##  docker service
```
```python
##  daemon reload service
```
```python
##  start docker service
```
```python
##  enable docker service
```
```python
##  check  docker status
```
```python
● docker.service - Docker Application Container Engine
   Loaded: loaded
```
```python
(
```
```python
/usr/lib/systemd/system/docker.service
```
```python
;
```
```python
enabled
```
```python
;
```
```python
vendor preset: disabled
```
```python
)
```
```python
Active: active
```
```python
(
```
```python
running
```
```python
)
```
```python
since Sun 2019-03-24 17:52:41 CST
```
```python
;
```
```python
226ms ago
     Docs: http://docs.docker.io
 Main PID: 4949
```
```python
(
```
```python
dockerd
```
```python
)
```
```python
CGroup: /system.slice/docker.service
           ├─4949 /usr/local/bin/dockerd --bip
```
```python
=
```
```python
172.200.240.1/21 --ip-masq
```
```python
=
```
```python
false --mtu
```
```python
=
```
```python
1450 --registry-mirror
```
```python
=
```
```python
https://hub-mirror.c.163.com -H tcp://0.0.0.0
```
```python
..
```
```python
.
           └─4952 docker-containerd -l unix:///var/run/docker/libcontainerd/docker-containerd.sock --metrics-interval
```
```python
=
```
```python
0 --start-timeout 2m --state-dir /va
```
```python
..
```
```python
.
Mar 24 17:52:41 host131 systemd
```
```python
[
```
```python
1
```
```python
]
```
```python
: Started Docker Application Container Engine.
Mar 24 17:52:42 host131 dockerd
```
```python
[
```
```python
4949
```
```python
]
```
```python
: time
```
```python
=
```
```python
"2019-03-24T17:52:42.015287389+08:00"
```
```python
level
```
```python
=
```
```python
warning msg
```
```python
=
```
```python
"[!] DON'T BIND ON ANY IP ADDRESS WITHOUT set...OING [!]"
```
```python
Mar 24 17:52:42 host131 dockerd
```
```python
[
```
```python
4949
```
```python
]
```
```python
: time
```
```python
=
```
```python
"2019-03-24T17:52:42.034052032+08:00"
```
```python
level
```
```python
=
```
```python
info msg
```
```python
=
```
```python
"libcontainerd: new containerd process, pid: 4952"
```
```python
Hint: Some lines were ellipsized, use -l to show
```
```python
in
```
```python
full.
```
```python
##  check docker version
```
```python
Client:
 Version:      17.03.2-ce
 API version:  1.27
 Go version:   go1.7.5
 Git commit:   f5ec1e2
 Built:        Tue Jun 27 01:35:00 2017
 OS/Arch:      linux/amd64
Server:
 Version:      17.03.2-ce
 API version:  1.27
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
 Git commit:   f5ec1e2
 Built:        Tue Jun 27 01:35:00 2017
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
root@host131 shell
```
```python
]
```
```python
#
```
flannel设定之后各node节点都会统一管理ip，不同容器之间的互联互通成为可能，本文的docker安装是在flannel的网络设定基础之上的，可以看到如下docker0已经使用flannel的配置了。
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
8: docker0:
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
    link/ether 02:42:73:01:b1:6d brd ff:ff:ff:ff:ff:ff
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

