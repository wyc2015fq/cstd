
# Docker基础：变更docker缺省数据路径 - 知行合一 止于至善 - CSDN博客

2019年03月19日 20:10:07[liumiaocn](https://me.csdn.net/liumiaocn)阅读数：403标签：[docker																](https://so.csdn.net/so/search/s.do?q=docker&t=blog)[graph																](https://so.csdn.net/so/search/s.do?q=graph&t=blog)[-g																](https://so.csdn.net/so/search/s.do?q=-g&t=blog)[/var/lib/docker																](https://so.csdn.net/so/search/s.do?q=/var/lib/docker&t=blog)[Docker Root Dir																](https://so.csdn.net/so/search/s.do?q=Docker Root Dir&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=/var/lib/docker&t=blog)[
																					](https://so.csdn.net/so/search/s.do?q=-g&t=blog)个人分类：[Docker																](https://blog.csdn.net/liumiaocn/article/category/6328276)
[
																								](https://so.csdn.net/so/search/s.do?q=-g&t=blog)
[
				](https://so.csdn.net/so/search/s.do?q=graph&t=blog)
[
			](https://so.csdn.net/so/search/s.do?q=graph&t=blog)
[
		](https://so.csdn.net/so/search/s.do?q=docker&t=blog)

使用Systemd方式启动docker服务之后，如果未做设定，docker缺省会将数据保存在/var/lib/docker，由于/var是系统目录，在实际机器的管理中往往只是给了一个很小的卷的大小，而实际docker在运行过程中产生的大量的卷的内容显然在这里保存会有问题，而发现问题的时候往往是因为将/var下面的空间占满或接近占满，这里简单介绍一下救火方法。
# 场景模拟和说明
当前docker的目录信息: /var/lib/docker
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
$ docker info
```
```python
|
```
```python
grep
```
```python
/var/lib
Docker Root Dir: /var/lib/docker
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
目标：移动到/opt/lib/docker，不影响目前的使用
# 设定选项
docker提供了-g选项(–graph的缩写)，使用此选项可以进行设定docker 缺省的数据路径（Docker Root Dir）。
## 设定方式1
在systemd的service文件中直接添加-g /opt/lib/docker
## 设定方式2
修改/etc/docker/daemon.json，添加如下信息
```python
{
```
```python
“graph”
```
```python
:
```
```python
“
```
```python
/
```
```python
opt
```
```python
/
```
```python
lib
```
```python
/
```
```python
docker”
```
```python
}
```
# 变更示例
## Step 1: 停止当前docker服务
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
$ systemctl stop docker
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
## Step 2:  创建并同步原有数据
确保新的路径用于保存数据
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
mkdir
```
```python
-p /opt/lib/docker
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
```python
sudo
```
```python
ls
```
```python
/opt/lib/docker
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
拷贝 (使用copy或者其他方式均可)
`[appman@host118 ~]$ sudo rsync -a /var/lib/docker/ /opt/lib/docker
[appman@host118 ~]$`
## Step 3: 设定路径并重启docker服务
设定路径
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
vi
```
```python
/etc/docker/daemon.json
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
vi
```
```python
/etc/docker/daemon.json
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
grep
```
```python
graph /etc/docker/daemon.json
```
```python
"graph"
```
```python
:
```
```python
"/opt/lib/docker"
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
重启docker服务
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
$ systemctl daemon-reload
```
```python
==
```
```python
==
```
```python
AUTHENTICATING FOR org.freedesktop.systemd1.reload-daemon
```
```python
==
```
```python
=
```
```python
Authentication is required to reload the systemd state.
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
## 确认变更结果
确认变更后的Docker Root Dir信息
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
$ docker info
```
```python
|
```
```python
grep
```
```python
'Docker Root'
```
```python
Docker Root Dir: /opt/lib/docker
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
确认之前的镜像信息是否还存在
`[appman@host118 ~]$ docker images
REPOSITORY          TAG                 IMAGE ID            CREATED             SIZE
alpine              3.9                 caf27325b298        5 weeks ago         5.52 MB
[appman@host118 ~]$`
# 总结
头痛医头，脚痛医脚。我等江湖医生多为如此，而实际当然应该更好地进行整体的规划，确认一下事前可能会用到多少空间，不同发行版的linux下使用那种方式能更好的提高效率，外部的lvm如何设定，至少这些考虑一下之后，才是本文真正的解决之道。

