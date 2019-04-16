# Centos7 PostgreSQL安装 - 菜鸟路上的小白 - CSDN博客





2016年09月09日 13:17:39[somnus_小凯](https://me.csdn.net/u012486840)阅读数：1180








Linux安装PostgreSQL 9.4步骤

一、安装PostgreSQL  CentOS 7 64bit




rpm -Uvh http://yum.postgresql.org/9.4/redhat/rhel-7-x86_64/pgdg-centos94-9.4-2.noarch.rpm   


二、执行安装命令

yum update  

yum install postgresql94-server postgresql94-contrib  


三、验证是否安装成功

rpm -aq| grep postgres  

执行结果如下：  

postgresql94-libs-9.4.2-1PGDG.rhel7.x86_64 

postgresql94-server-9.4.2-1PGDG.rhel7.x86_64 

postgresql94-9.4.2-1PGDG.rhel7.x86_64  

postgresql94-contrib-9.4.2-1PGDG.rhel7.x86_64  


四、初始化数据库

/usr/pgsql-9.4/bin/postgresql94-setup initdb  






五、启动服务并设置为开机启动

systemctl enable postgresql-9.4  

systemctl start postgresql-9.4  






六、开放防火墙端口

firewall-cmd --permanent --add-port=5432/tcp
  

firewall-cmd --permanent --add-port=80/tcp
  

firewall-cmd --reload  


七、访问PostgreSQL

su - postgres  





输出结果如下：

上一次登录：一 5月 18 15:17:29 CST 2015pts/0 上

-bash-4.2$

输入命令psql将看到PostgrSQL的版本信息。

psql (9.4.1)

输入 "help" 来获取帮助信息.


八、设置postgres用户密码

postgres=# \password postgres  






 九、停止和启动postgresql服务

service postgresql-9.4 start  

service postgresql-9.4 stop  



十、卸载postgres、



# **yum remove postgresql***



记：postgres帐号密码 都为postgres


