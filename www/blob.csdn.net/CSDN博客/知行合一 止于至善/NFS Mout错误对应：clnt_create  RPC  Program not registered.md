
# NFS Mout错误对应：clnt_create: RPC: Program not registered - 知行合一 止于至善 - CSDN博客

2019年03月19日 07:41:25[liumiaocn](https://me.csdn.net/liumiaocn)阅读数：304标签：[nfs																](https://so.csdn.net/so/search/s.do?q=nfs&t=blog)[mount																](https://so.csdn.net/so/search/s.do?q=mount&t=blog)[Program not registered																](https://so.csdn.net/so/search/s.do?q=Program not registered&t=blog)[rpcbind																](https://so.csdn.net/so/search/s.do?q=rpcbind&t=blog)[linux																](https://so.csdn.net/so/search/s.do?q=linux&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=rpcbind&t=blog)[
																					](https://so.csdn.net/so/search/s.do?q=Program not registered&t=blog)个人分类：[Unix/Linux																](https://blog.csdn.net/liumiaocn/article/category/6328278)
[
																								](https://so.csdn.net/so/search/s.do?q=Program not registered&t=blog)
[
				](https://so.csdn.net/so/search/s.do?q=mount&t=blog)
[
			](https://so.csdn.net/so/search/s.do?q=mount&t=blog)
[
		](https://so.csdn.net/so/search/s.do?q=nfs&t=blog)

本文记录了常见的NFS Mount时出现的Program not registered的对应方法。
# 环境
OS： CentOS 7.4
NFS版本信息
```python
[
```
```python
appman@host131 ~
```
```python
]
```
```python
$ rpm -qa
```
```python
|
```
```python
grep
```
```python
rpcbind
rpcbind-0.2.0-47.el7.x86_64
```
```python
[
```
```python
appman@host131 ~
```
```python
]
```
```python
$ rpm -qa
```
```python
|
```
```python
grep
```
```python
nfs
libnfsidmap-0.25-19.el7.x86_64
nfs-utils-1.3.0-0.61.el7.x86_64
```
```python
[
```
```python
appman@host131 ~
```
```python
]
```
```python
$
```
注：具体设定可参看如下文章。
[https://blog.csdn.net/liumiaocn/article/details/88653809](https://blog.csdn.net/liumiaocn/article/details/88653809)
# 错误现象
执行exportfs之后，使用showmount无法正常动作。
```python
[
```
```python
appman@host131 ~
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
showmount -e localhost
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
clnt_create: RPC: Program not registered
```
```python
[
```
```python
appman@host131 ~
```
```python
]
```
```python
$
```
# 原因
nfs服务未启动，需要保证rpcbind和nfs服务正常启动。
```python
[
```
```python
appman@host131 ~
```
```python
]
```
```python
$ systemctl status nfs
● nfs-server.service - NFS server and services
   Loaded: loaded
```
```python
(
```
```python
/usr/lib/systemd/system/nfs-server.service
```
```python
;
```
```python
disabled
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
Active: inactive
```
```python
(
```
```python
dead
```
```python
)
```
```python
[
```
```python
appman@host131 ~
```
```python
]
```
```python
$
```
# 启动服务并确认
```python
appman@host131 ~
```
```python
]
```
```python
$ systemctl start nfs
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
appman@host131 ~
```
```python
]
```
```python
$
```
```python
[
```
```python
appman@host131 ~
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
showmount -e localhost
Export list
```
```python
for
```
```python
localhost:
/opt/lib/docker 192.168.163.118
```
```python
[
```
```python
appman@host131 ~
```
```python
]
```
```python
$
```

