# linux下Postgresql-9.2安装及数据库的创建过程 - DoubleLi - 博客园






### **公司写部署手册需要，现总结一些linux下postgresql的安装及数据库创建的详细步骤吧！**

### 1.1.1  软件安装

**  1.设置用户组和用户级别**

    Postgresql不能以root身份运行，要以其他的身份运行，所以必须建立对应的用户和组。

(1)新增postgresql用户组：groupadd postgresql

(2)新增postgres用户属于postgresql用户组：useradd -g postgresql postgres

(3)修改postgres用户密码:passwd postgres（这里设置密码为postgres）

**  2.安装postgresql软件**

(1)为 postgresql-9.2.4-1-[Linux](http://lib.csdn.net/base/linux)-x64.run赋予可执行权限：

(2)安装postgresql：./postgresql-9.2.4-1-[linux](http://lib.csdn.net/base/linux)-x64.run,一路Enter键，保持默认配置即可。默认安装路径/opt/PostgreSQL

(3)配置环境变量：执行vi /etc/profile修改全局环境变量文件，在profile文件里面添加下面三句话，

export LD_LIBRARY_PATH=/opt/PostgreSQL/9.2/lib;$LD_LIBRARY_PATH

export PATH=/opt/PostgreSQL/9.2/bin:$PATH

export PGDATA=/opt/PostgreSQL/9.2/data

执行成功后，立即生效可用source /etc/profile。

** 3.初始化[数据库](http://lib.csdn.net/base/mysql)**

   (1)切换至postgres账户：su postgres

   (2)修改系统编码为GBK：vi/etc/sysconfig/i18n

![](http://img.blog.csdn.net/20140318104736109?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveGgxNjMxOQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

   (3) 初始化数据库initdb -D /opt/PostgreSQL/9.2/data

   (4) 启动postgres服务： postmaster -i -D /opt/PostgreSQL/9.2/data & (后台挂起)

   pg_ctl start -l /opt/postgresql/log/pg_server.log (关闭ssh窗口则关闭服务)

   (5)停止postgresql服务： pg_ctl stop
   (6)修改数据库用户postgres密码：psql template1

   (7)查看端口被占用：netstat -lnpt | grep 5432    或者 ps -ef | grep postgresql

        结束进程：kill -9 pid

   (8)[测试](http://lib.csdn.net/base/softwaretest)主机端口是否联通：window下用telnet，linux下用nc -v host port 





### 1.1.2 数据库创建及脚本导入

  如果该服务器对外开通了5432端口的访问权限，则可通过pgAdmin III客户端执行以下操作，否则只能通过SSH客户端执行命令来实现数据库创建及脚本导入。

  1. 创建数据库：createdb lsgfss

  2.数据脚本导入： psql -d lsgfss -U postgres -f /opt/PostgreSQL/9.2/data/lsgfss.sql

  3.启动前端访问工具：psql lsgfss

  补充：linux下postgresql数据库中的一些命令：   

   template1=# \l 查看系统中现存的数据库  
   template1=# \q 退出客户端程序psql  
   template1=# \c 从一个数据库中转到另一个数据库中，如template1=# \c sales 从template1转到sales  
   template1=# \dt 查看表  
   template1=# \d 查看表结构  
   template1=# \di 查看索引  

  4.pgAdmin III客户端访问linux下该数据库的话，还需要为该客户端分配数据库访问权限， 修改pg_hba.conf 文件，该文件都是放在$PG_DATA目录下，也就是一般的...../data目录下。

 添加如下一句话，然后重启。

 host   all      all      10.123.0.0   255.255.0.0    trust   



**注释：**  Linux下，postgresql.conf   pg_hba.conf   都是放在$PG_DATA目录下，也就是一般的...../data目录下   

  访问的控制是修改pg_hba.conf文件，如果加入的是下面行：   
  host   all     all       192.168.0.0       255.255.255.0     md5   
  这时，你从那个网段上访问任何数据库时，就要密码，密码在传的过程中是加密码的，   
  如果加下面一行：   
  host   all    all       192.168.0.0        255.255.255.0     password   
  这时，你从那个网段上访问任何数据库时，就要密码，密码在传的过程中是没加密码的，   
  如果加下面一行：   
  host   all    all       192.168.0.0        255.255.255.0     trust   
  这时，你从那个网段上访问任何数据库时，就不要密码，   、

  如果出现这种：

   host all all 127.0.0.1/32 md5 
   其中32是子网掩码的网段；md5是密码验证方法









