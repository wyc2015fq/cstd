# 基于Docker搭建MySQL主从复制 - 菜鸟路上的小白 - CSDN博客





2018年05月25日 11:26:49[somnus_小凯](https://me.csdn.net/u012486840)阅读数：551










## 搭建环境

Centos 7.2 64位

MySQL 5.7.13

Docker 1.13.1

接下来，我们将会在一台服务器上安装docker，并使用docker运行三个MySQL容器，分别为一主两从。

## 安装docker

执行命令
`[root@VM_0_17_centos ~]# yum install docker`
如果有提示，一路y下去

安装成功启动Docker后，查看版本

```
[root@VM_0_17_centos ~]# docker version
Client:
 Version:         1.13.1
 API version:     1.26
 Package version: <unknown>
 Go version:      go1.8.3
 Git commit:      774336d/1.13.1
 Built:           Wed Mar  7 17:06:16 2018
 OS/Arch:         linux/amd64

Server:
 Version:         1.13.1
 API version:     1.26 (minimum version 1.12)
 Package version: <unknown>
 Go version:      go1.8.3
 Git commit:      774336d/1.13.1
 Built:           Wed Mar  7 17:06:16 2018
 OS/Arch:         linux/amd64
 Experimental:    false
```

出现版本信息，则安装成功

## 启动Docker

启动Docker并设置为开机自启动

```
```
[root@VM_0_17_centos ~]# systemctl  start docker.service
[root@VM_0_17_centos ~]# systemctl  enable docker.service
```
```

## 安装MySQL

使用Docker拉取MySQL镜像
``[root@VM_0_17_centos ~]# docker pull mysql:5.7.13``
## 运行主容器

```
[root@VM_0_17_centos ~]# docker run --name master -p 3306:3306 -e MYSQL_ROOT_PASSWORD=root -d mysql:5.7.13
```

--name 为容器指定名称，这里是master

-p 将容器的指定端口映射到主机的指定端口，这里是将容器的3306端口映射到主机的3306端口

-e 设置环境变量，这里是指定root账号的密码为root

-d 后台运行容器，并返回容器ID

mysql:5.7.13 指定运行的mysql版本

## 检验是否启动成功

docker ps -a 显示所有的容器，包括未运行的

```
[root@VM_0_17_centos ~]# docker ps -a
ee86c19336f8        mysql:5.7.13        "docker-entrypoint..."   About an hour ago   Up About an hour    0.0.0.0:3306->3306/tcp   master
```

注意，是UP状态，表示正在运行中

开放3306端口

```
[root@VM_0_17_centos ~]# firewall-cmd --zone=public --add-port=3306/tcp --permanent
[root@VM_0_17_centos ~]# firewall-cmd --reload
```

--permanent 永久开启，避免下次开机需要再次手动开启端口

使用Navicat连接测试

![](https://static.oschina.net/uploads/space/2018/0510/114916_0h3I_3773384.png)

MySQL主容器已经启动成功

## 创建主容器的复制账号

![](https://static.oschina.net/uploads/space/2018/0510/120249_0ZQx_3773384.png)

使用Navicat友好的图像化界面执行SQL

```
```
GRANT REPLICATION SLAVE ON *.* to 'backup'@'%' identified by 'backup';
show grants for 'backup'@'%';
```
```

出现如下信息表示授权成功

![](https://static.oschina.net/uploads/space/2018/0510/121018_Vtxn_3773384.png)

## 修改MySQL配置环境

创建配置文件目录

目录结构如下

/usr/local/mysql/master

/usr/local/mysql/slave1

/usr/local/mysql/slave2

拷贝一份MySQL配置文件

```
[root@VM_0_17_centos local]# docker cp master:/etc/mysql/my.cnf /usr/local/mysql/master/my.cnf
```

进到master目录下，已存在拷贝的my.cnf

```
```
[root@VM_0_17_centos master]# ll
total 4
-rw-r--r-- 1 root root 1801 May 10 10:27 my.cnf
```
```

修改my.cnf，在 [mysqld] 节点最后加上后保存

```
log-bin=mysql-bin
server-id=1
```

log-bin=mysql-bin 使用binary logging，mysql-bin是log文件名的前缀

server-id=1 唯一服务器ID，非0整数，不能和其他服务器的server-id重复

将修改后的文件覆盖Docker中MySQL中的配置文件

```
[root@VM_0_17_centos master]# docker cp /usr/local/mysql/master/my.cnf master:/etc/mysql/my.cnf
```

重启 mysql 的docker , 让配置生效
`[root@VM_0_17_centos master]# docker restart master`
启动后，重新测试连接，连接成功表示主容器配置成功

## 运行MySQL从容器

首先运行从容器

```
[root@VM_0_17_centos ~]# docker run --name slave1 -p 3307:3306 -e MYSQL_ROOT_PASSWORD=root -d mysql:5.7.13
```

与主容器相似，拷贝配置文件至slave1目录修改后覆盖回Docker中

```
log-bin=mysql-bin
server-id=2
```

别忘记，重启slave1容器，使配置生效

## 配置主从复制

使用Navicat连接slave1后新建查询，执行以下SQL

```
```
CHANGE MASTER TO 
MASTER_HOST='ip',
MASTER_PORT=3306,
MASTER_USER='backup',
MASTER_PASSWORD='backup';

START SLAVE;
```
```

MASTER_HOST 填Navicat连接配置中的ip应该就可以

MASTER_PORT 主容器的端口

MASTER_USER 同步账号的用户名

MASTER_PASSWORD 同步账号的密码

## 检查是否配置成功
``show slave status;``
![](https://static.oschina.net/uploads/space/2018/0510/123902_gnvI_3773384.png)

Slave_IO_State 如果是Waiting for master to send event，那么就成功一半了，如果是Connecting to master，基本就是配置失败了，建议重新检查下配置，具体失败的原因可以查看日志追踪
`[root@VM_0_17_centos master]# docker logs slave -f`
我遇到的是MASTER_USER和MASTER_PASSWORD是否手打输错了，贴出错误日志

```
2018-05-10T02:57:00.688887Z 11 [ERROR] Slave I/O for channel '': error connecting to master 'bakcup@ip:3306' - retry-time: 60  retries: 2, Error_code: 1045
2018-05-10T02:58:00.690476Z 11 [ERROR] Slave I/O for channel '': error connecting to master 'bakcup@ip:3306' - retry-time: 60  retries: 3, Error_code: 1045
```

注意看日志中的bakcup，解决方法如下

```
```
STOP SLAVE;

CHANGE MASTER TO 
MASTER_HOST='连接Navicat的ip',
MASTER_PORT=正确的端口,
MASTER_USER='正确的用户名',
MASTER_PASSWORD='正确的密码';

START SLAVE;
```
```

接着上文，我们说成功一半，并没有说成功了，那么另一半在于Slave_IO_Running与Slave_SQL_Running

如果都是Yes，那么恭喜你，可以测试主从复制的效果了，如果有一个不是Yes，一半是重启从容器后，事务回滚引起的，那么给出解决方法如下

```
```
stop slave ;
set GLOBAL SQL_SLAVE_SKIP_COUNTER=1;
start slave ;
```
```

执行后，再次观察三个关键字段应该就都没问题了

至此，一主一从已经搭建完成，再添加从实例的方式与上文一致，这里就不在赘述了。

## 测试主从复制

首先，在主实例中创建一个测试数据库

![](https://static.oschina.net/uploads/space/2018/0510/130207_7Ty1_3773384.png)

打开（刷新）从实例，可见test库已存在

![](https://static.oschina.net/uploads/space/2018/0510/130133_ITNO_3773384.png)

在test库中创建一个表t_test，添加一个id测试字段

向表中添加几个数据

![](https://static.oschina.net/uploads/space/2018/0510/130357_IEZg_3773384.png)

刷新从库，可见t_test表及其中1、2、3、4数据已存在

至此，一个具备主从复制的一主两从的MySQL就已搭建完成。







