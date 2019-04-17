# Linux下PostgreSQL 的安装与配置 - DoubleLi - 博客园






一、简介

PostgreSQL 是一种非常复杂的对象-关系型数据库管理系统（ORDBMS），也是目前功能最强大，特性最丰富和最复杂的自由软件数据库系统。有些特性甚至连商业数据库都不具备。这个起源于伯克利（BSD）的数据库研究计划目前已经衍生成一项国际开发项目，并且有非常广泛的用户。

二、系统环境

系统平台：[CentOS](http://www.linuxidc.com/topicnews.aspx?tid=14) release 6.3 (Final)

PostgreSQL 版本：PostgreSQL 9.2.4

防火墙已关闭/iptables: Firewall is not running.

SELINUX=disabled

三、安装方式

A. 下载RPM包安装

B. yum 安装

C. 源码包安装

四、安装过程

A. RPM包安装

1. 检查PostgreSQL 是否已经安装
[root@TS-DEV ~]# rpm -qa|grep postgres
[root@TS-DEV ~]# 
若已经安装，则使用rpm -e 命令卸载。

2. 下载RPM包
#wget http://yum.postgresql.org/9.2/[RedHat](http://www.linuxidc.com/topicnews.aspx?tid=10)/rhel-6-i386/postgresql92-server-9.2.4-1PGDG.rhel6.i686.rpm
#wget http://yum.postgresql.org/9.2/redhat/rhel-6-i386/postgresql92-contrib-9.2.4-1PGDG.rhel6.i686.rpm
#wget http://yum.postgresql.org/9.2/redhat/rhel-6-i386/postgresql92-libs-9.2.4-1PGDG.rhel6.i686.rpm
#wget http://yum.postgresql.org/9.2/redhat/rhel-6-i386/postgresql92-9.2.4-1PGDG.rhel6.i686.rpm
3. 安装PostgreSQL，注意安装顺序
# rpm -ivh postgresql92-libs-9.2.4-1PGDG.rhel6.i686.rpm 
# rpm -ivh postgresql92-9.2.4-1PGDG.rhel6.i686.rpm
# rpm -ivh postgresql92-server-9.2.4-1PGDG.rhel6.i686.rpm 
# rpm -ivh postgresql92-contrib-9.2.4-1PGDG.rhel6.i686.rpm
4. 初始化PostgreSQL库

PostgreSQL 服务初次启动的时候会提示初始化。

![](http://www.linuxidc.com/upload/2013_10/131030170921291.jpg)
# service postgresql-9.2 initdb
![](http://www.linuxidc.com/upload/2013_10/131030170921292.jpg)

5. 启动服务
# service postgresql-9.2 start
![](http://www.linuxidc.com/upload/2013_10/131030170921293.jpg)

6. 把PostgreSQL 服务加入到启动列表
# chkconfig postgresql-9.2 on
# chkconfig --list|grep postgres
![](http://www.linuxidc.com/upload/2013_10/131030170921294.jpg)

7. 修改PostgreSQL数据库用户postgres的密码(注意不是linux系统帐号)

PostgreSQL数据库默认会创建一个postgres的数据库用户作为数据库的管理员，默认密码为空，我们需要修改为指定的密码，这里设定为’postgres’。
# su - postgres
$ psql
# ALTER USER postgres WITH PASSWORD 'postgres';
# select * from pg_shadow ;
![](http://www.linuxidc.com/upload/2013_10/131030170921295.jpg)

8. 测试数据库

8.1 创建测试数据库

# create database david;

![](http://www.linuxidc.com/upload/2013_10/131030170921296.jpg)

8.2 切换到david 数据库

# \c david

![](http://www.linuxidc.com/upload/2013_10/131030170921297.jpg)

8.3 创建测试表
david=# create table test (
id integer,
name text);
![](http://www.linuxidc.com/upload/2013_10/131030170921298.jpg)

8.4 插入测试数据
david=# insert into test values (1,'david');
INSERT 0 1
david=# 
8.5 选择数据
david=# select * from test ;
 id | name  
----+-------
  1 | david
(1 row)

david=# 
测试成功。

**更多详情见请继续阅读下一页的精彩内容**： [http://www.linuxidc.com/Linux/2013-10/92111p2.htm](http://www.linuxidc.com/Linux/2013-10/92111p2.htm)

**相关阅读**：

PostgreSQL删除表中重复数据行 [http://www.linuxidc.com/Linux/2013-07/87780.htm](http://www.linuxidc.com/Linux/2013-07/87780.htm)

PostgreSQL数据库连接池PgBouncer的搭建 [http://www.linuxidc.com/Linux/2013-06/85928.htm](http://www.linuxidc.com/Linux/2013-06/85928.htm)

Windows平台编译 PostgreSQL [http://www.linuxidc.com/Linux/2013-05/85114.htm](http://www.linuxidc.com/Linux/2013-05/85114.htm)

PostgreSQL备份心得笔记 [http://www.linuxidc.com/Linux/2013-04/82812.htm](http://www.linuxidc.com/Linux/2013-04/82812.htm)









