
# sysemd配置文件的两个注意事项 - 知行合一 止于至善 - CSDN博客

2019年03月27日 07:34:56[liumiaocn](https://me.csdn.net/liumiaocn)阅读数：237


这篇文章memo一下systemd的服务配置文件书写的时候需要注意的两个小问题，FULLPATH与变量的使用。
# 问题示例
以下flanneld的service文件某一行有两处问题，在systemd中无法正常使用。
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
# cat /etc/flannel/flannel.conf
```
```python
FLANNELD_OPTS
```
```python
=
```
```python
"-etcd-cafile=/etc/ssl/ca/ca.pem \
  -etcd-certfile=/etc/ssl/flannel/flanneld.pem \
  -etcd-keyfile=/etc/ssl/flannel/flanneld-key.pem \
  -etcd-endpoints=https://192.168.163.131:2379 \
  -etcd-prefix=/coreos.com/network \
  -iface=enp0s3 \
  -ip-masq"
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
# cat /usr/lib/systemd/system/flanneld.service
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
Flanneld Service
Documentation
```
```python
=
```
```python
https://github.com/coreos/flannel
After
```
```python
=
```
```python
network.target
After
```
```python
=
```
```python
network-online.target
Wants
```
```python
=
```
```python
network-online.target
After
```
```python
=
```
```python
etcd.service
Before
```
```python
=
```
```python
docker.service
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
-/etc/flannel/flannel.conf
ExecStart
```
```python
=
```
```python
flanneld
```
```python
${FLANNELD_OPTS}
```
```python
ExecStartPost
```
```python
=
```
```python
/usr/local/bin/mk-docker-opts.sh -k DOCKER_NETWORK_OPTIONS -d /run/flannel/docker
Restart
```
```python
=
```
```python
on-failure
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
RequiredBy
```
```python
=
```
```python
docker.service
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
# 问题说明
出现问题的是如下这一行
`ExecStart=flanneld ${FLANNELD_OPTS}`这一行主要有两个问题：
## systemd中需要使用FULLPATH
flanneld的写法的前提是到PATH中去搜索，systemd需要使用full path，不过这个问题比较简单，因为写错的化提示会非常清晰，以下是错误提示示例
`Mar 24 13:19:30 host131 systemd[1]: [/usr/lib/systemd/system/flanneld.service:12] Executable path is not absolute, ignoring: flanneld ${FLANNELD_OPTS}
Mar 24 13:19:30 host131 systemd[1]: flanneld.service lacks both ExecStart= and ExecStop= setting. Refusing.`
### 对应方法
> 对应方法：flanneld -> /usr/local/bin/flanneld

## 变量的使用问题
第二个问题在于变量的使用问题，${FLANNELD_OPTS}是在脚本中常用的方式，直接这样在systemd中使用目前是有问题的，而且不会正确提示，然后你把相关的命令拷贝到终端去执行有没有任何问题，因为终端是sh在进行解析。
`[root@host131 shell]# vi /usr/lib/systemd/system/flanneld.service
[root@host131 shell]# 
[root@host131 shell]# grep flanneld /usr/lib/systemd/system/flanneld.service
ExecStart=/usr/local/bin/flanneld ${FLANNELD_OPTS}
[root@host131 shell]# systemctl daemon-reload
[root@host131 shell]# systemctl start flanneld
[root@host131 shell]# systemctl status flanneld
● flanneld.service - Flanneld Service
   Loaded: loaded (/usr/lib/systemd/system/flanneld.service; enabled; vendor preset: disabled)
   Active: failed (Result: start-limit) since Sun 2019-03-24 13:27:09 CST; 2s ago
     Docs: https://github.com/coreos/flannel
  Process: 14675 ExecStartPost=/usr/local/bin/mk-docker-opts.sh -k DOCKER_NETWORK_OPTIONS -d /run/flannel/docker (code=exited, status=0/SUCCESS)
  Process: 14674 ExecStart=/usr/local/bin/flanneld ${FLANNELD_OPTS} (code=exited, status=1/FAILURE)
 Main PID: 14674 (code=exited, status=1/FAILURE)
Mar 24 13:27:09 host131 systemd[1]: flanneld.service: main process exited, code=exited, status=1/FAILURE
Mar 24 13:27:09 host131 systemd[1]: Unit flanneld.service entered failed state.
Mar 24 13:27:09 host131 systemd[1]: flanneld.service failed.
Mar 24 13:27:09 host131 systemd[1]: flanneld.service holdoff time over, scheduling restart.
Mar 24 13:27:09 host131 systemd[1]: Stopped Flanneld Service.
Mar 24 13:27:09 host131 systemd[1]: start request repeated too quickly for flanneld.service
Mar 24 13:27:09 host131 systemd[1]: Failed to start Flanneld Service.
Mar 24 13:27:09 host131 systemd[1]: Unit flanneld.service entered failed state.
Mar 24 13:27:09 host131 systemd[1]: flanneld.service failed.
[root@host131 shell]#`
## 对应方法
> ${FLANNELD_OPTS} -> $FLANNELD_OPTS


