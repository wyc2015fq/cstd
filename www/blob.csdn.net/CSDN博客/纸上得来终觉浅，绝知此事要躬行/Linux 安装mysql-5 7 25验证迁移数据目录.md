# Linux 安装mysql-5.7.25验证迁移数据目录 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2019年04月01日 17:16:32[boonya](https://me.csdn.net/boonya)阅读数：38








**目录**

[Mysql安装](#Mysql%E5%AE%89%E8%A3%85)

[下载安装包](#%E4%B8%8B%E8%BD%BD%E5%AE%89%E8%A3%85%E5%8C%85)

[解压安装包](#%E8%A7%A3%E5%8E%8B%E5%AE%89%E8%A3%85%E5%8C%85)

[创建数据文件存放目录](#%E5%88%9B%E5%BB%BA%E6%95%B0%E6%8D%AE%E6%96%87%E4%BB%B6%E5%AD%98%E6%94%BE%E7%9B%AE%E5%BD%95)

[创建系统用户组和用户](#%E5%88%9B%E5%BB%BA%E7%B3%BB%E7%BB%9F%E7%94%A8%E6%88%B7%E7%BB%84%E5%92%8C%E7%94%A8%E6%88%B7)

[修改当前目录权限](#%E4%BF%AE%E6%94%B9%E5%BD%93%E5%89%8D%E7%9B%AE%E5%BD%95%E6%9D%83%E9%99%90)

[配置/etc/my.cnf](#%E9%85%8D%E7%BD%AE%2Fetc%2Fmy.cnf)

[修改/etc/profile环境变量](#%E4%BF%AE%E6%94%B9%2Fetc%2Fprofile%E7%8E%AF%E5%A2%83%E5%8F%98%E9%87%8F)

[初始化数据库](#%E5%88%9D%E5%A7%8B%E5%8C%96%E6%95%B0%E6%8D%AE%E5%BA%93)

[添加开机启动](#%E6%B7%BB%E5%8A%A0%E5%BC%80%E6%9C%BA%E5%90%AF%E5%8A%A8)

[mysql指令操作](#mysql%E6%8C%87%E4%BB%A4%E6%93%8D%E4%BD%9C)

[安装后续操作](#%E5%AE%89%E8%A3%85%E5%90%8E%E7%BB%AD%E6%93%8D%E4%BD%9C)

[获取临时密码](#%E8%8E%B7%E5%8F%96%E4%B8%B4%E6%97%B6%E5%AF%86%E7%A0%81)

[mysql命令不可用](#mysql%E5%91%BD%E4%BB%A4%E4%B8%8D%E5%8F%AF%E7%94%A8)

[更改root密码](#%E6%9B%B4%E6%94%B9root%E5%AF%86%E7%A0%81)

[创建用户并开启远程访问](#%E5%88%9B%E5%BB%BA%E7%94%A8%E6%88%B7%E5%B9%B6%E5%BC%80%E5%90%AF%E8%BF%9C%E7%A8%8B%E8%AE%BF%E9%97%AE)

[迁移MYSQL数据目录](#%E8%BF%81%E7%A7%BBMYSQL%E6%95%B0%E6%8D%AE%E7%9B%AE%E5%BD%95)

[修改/etc/my.cnf](#%E4%BF%AE%E6%94%B9%2Fetc%2Fmy.cnf)

[转移/usr/local/mysql/data目录](#%E8%BD%AC%E7%A7%BB%2Fusr%2Flocal%2Fmysql%2Fdata%E7%9B%AE%E5%BD%95)

[最终迁移后的结果](#%E6%9C%80%E7%BB%88%E8%BF%81%E7%A7%BB%E5%90%8E%E7%9A%84%E7%BB%93%E6%9E%9C)

## Mysql安装

### 下载安装包
`wget https://dev.mysql.com/get/Downloads/MySQL-5.7/mysql-5.7.25-linux-glibc2.12-x86_64.tar.gz`
### 解压安装包

```
#解压安装包
tar -zxvf mysql-5.7.25-linux-glibc2.12-x86_64.tar.gz
#删除原始包(可以保留备用)
rm mysql-5.7.25-linux-glibc2.12-x86_64.tar.gz
#移动解压目录到/usr/local/mysql
mv mysql-5.7.25-linux-glibc2.12-x86_64 /usr/local/mysql
```

### 创建数据文件存放目录
`mkdir /usr/local/mysql/data`
### 创建系统用户组和用户

```
groupadd mysql    ---新建一个msyql组
useradd -g mysql mysql     ---新建msyql用户
```

### 修改当前目录权限
`chown -R mysql:mysql /usr/local/mysql`
### 配置/etc/my.cnf

```
[mysqld]
character_set_server=utf8
init_connect='SET NAMES utf8'
basedir=/usr/local/mysql
datadir=/usr/local/mysql/data
socket=/tmp/mysql.sock
#不区分大小写
lower_case_table_names = 1
#不开启sql严格模式
sql_mode = "STRICT_TRANS_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ERROR_FOR_DIVISION_BY_ZERO,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION"
log-error=/var/log/mysqld.log
pid-file=/usr/local/mysql/data/mysqld.pid
```

### 修改/etc/profile环境变量

在文件尾部加上如下代码：

```
export PATH=/usr/local/mysql/bin:$PATH
```

同步资源：

```
source /etc/profile
```

### 初始化数据库

```
#进入bin目录
cd /usr/local/mysql/bin
#初始化数据库
./mysqld --initialize --user=mysql --basedir=/usr/local/mysql --datadir=/usr/local/mysql/data
```

### 添加开机启动

修改配置文件


```
cp /usr/local/mysql/support-files/mysql.server  /etc/init.d/mysqld
vim /etc/init.d/mysqld
basedir=/usr/local/mysql
datadir=/usr/local/mysql/data
```

执行启动添加命令


```
chkconfig --add mysqld
chkconfig mysql on
```

### mysql指令操作

```
#启动
service mysqld start
#重启
service mysqld restart
#停止
service mysqld stop
```

## 安装后续操作

### 获取临时密码

```
[root@localhost bin]# grep 'temporary password' /var/log/mysqld.log 
2019-04-01T06:27:32.332025Z 1 [Note] A temporary password is generated for root@localhost: L351tWTtCo<>
2019-04-01T06:38:53.070454Z 1 [Note] A temporary password is generated for root@localhost: .whwt&bf2hyA
```

### mysql命令不可用
`source /etc/profile`
忘了同步环境变量会导致指令不可用。

### 更改root密码

```
mysql> alter user 'root'@'localhost' identified by 'root';
Query OK, 0 rows affected (0.00 sec)
```

### 创建用户并开启远程访问

```
mysql> CREATE USER 'dlwy'@'%' IDENTIFIED BY 'dlwy';
Query OK, 0 rows affected (0.00 sec)

mysql> grant all privileges on *.* to 'dlwy'@'%' identified by 'dlwy'
    -> ;
Query OK, 0 rows affected, 1 warning (0.00 sec)

mysql> flush privileges;
Query OK, 0 rows affected (0.00 sec)
```

## 迁移MYSQL数据目录

注意：迁移数据目录网上很多不可借鉴，只需要修改datadir和pid的位置（my.cnf配置目录不在/usr/local/mysql/data的都可以不用修改）。

停止mysql服务:

```
#停止
service mysqld stop
```

### 修改/etc/my.cnf

![](https://img-blog.csdnimg.cn/20190401184150130.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Jvb255YQ==,size_16,color_FFFFFF,t_70)

### 转移/usr/local/mysql/data目录

找一个分区目录空间充足的路径，这里以/home路径测试：

```
[root@localhost ~]# df -h
文件系统             容量  已用  可用 已用% 挂载点
/dev/mapper/cl-root   50G   20G   31G   40% /
devtmpfs             3.8G     0  3.8G    0% /dev
tmpfs                3.8G     0  3.8G    0% /dev/shm
tmpfs                3.8G  8.6M  3.8G    1% /run
tmpfs                3.8G     0  3.8G    0% /sys/fs/cgroup
/dev/sda1           1014M  140M  875M   14% /boot
/dev/mapper/cl-home  873G  166M  873G    1% /home
tmpfs                770M     0  770M    0% /run/user/0
[root@localhost ~]#
```

创建转移数据目录：

```
mkdir /home/mysql-data-transfer-dir
cd /home/mysql-data-transfer-dir
mkdir data
```

 复制数据：
`cp -a /usr/local/mysql/data  /home/mysql-data-transfer-dir`
重启mysql服务：

```
#启动
service mysqld start
```

### 最终迁移后的结果

![](https://img-blog.csdnimg.cn/20190401183714862.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Jvb255YQ==,size_16,color_FFFFFF,t_70)

按照你想要的目的，通过最简单的方式达到目标才是最高效的。



