# 安装PostgreSQL数据库(Linux篇) - DoubleLi - 博客园






0.编译环境
- Linux: CentOS 5.5
- gcc: 4.1.2

1. 安装PostgreSQL

1) 解压postgresql-9.1.7.tar.bz2

#tar jxvf postgresql-9.1.7.tar.bz2

![](https://images0.cnblogs.com/blog/119630/201302/08202733-7c3361d8cbcd4b2f959792fbb70dc23f.png)

2) 进入解压后的postgresql-9.1.7目录

#cd postgresql-9.1.7

![](https://images0.cnblogs.com/blog/119630/201302/08203207-9476995af40b4ea48327274a56fdef1c.png)

3) 编译postgresql源码

#./configure --prefix=/opt/pgsql-9.1.7

![](https://images0.cnblogs.com/blog/119630/201302/08203421-de028547596248e18be3e71612735bd9.png)

 #make

![](https://images0.cnblogs.com/blog/119630/201302/08203554-73b3c8228b4646b0a88093259c8e944e.png)

#make install

![](https://images0.cnblogs.com/blog/119630/201302/08204027-dcaaa0db153449aeb70d74277f27643e.png)

至此，完成postgresql的安装。进入/opt/pgsql-9.1.7目录可以看到安装后的postgresql的文件。

#ls /opt/pgsql-9.1.7

![](https://images0.cnblogs.com/blog/119630/201302/08204237-c49c111409d34b14bf2ec74757dc4d28.png)

2.创建postgresql数据库

1) 创建postgres用户

#useradd postgres

修改postgres密码

#passwd postgres

![](https://images0.cnblogs.com/blog/119630/201302/08204847-216d541c19094bcab1ab5d8670602df8.png)

2) 设置postgres用户的环境变量

切换到postgres用户

#su - postgres

![](https://images0.cnblogs.com/blog/119630/201302/08205000-34c857e7999d4fd0bc8823420dfec10f.png)

进入postgres的主目录

#cd ~

![](https://images0.cnblogs.com/blog/119630/201302/08205236-f8990963882947fc95e787952f112ded.png)

编辑~/.bash_profile文件

#vi ~/.bash_profile

设置以下的环境变量

export PGHOME=/opt/pgsql-9.1.7

export PGDATA=~/data

![](https://images0.cnblogs.com/blog/119630/201302/08205529-a7e7098342c141a8b8413f1ea0e3f8a4.png)

保存，退出vi。执行以下命令，使环境变量生效

#source ~/.bash_profile

![](https://images0.cnblogs.com/blog/119630/201302/08205650-8095f3f6f47b492a8e82bdc6753a1609.png)

3) 初始化postgres数据库

#initdb

![](https://images0.cnblogs.com/blog/119630/201302/08205807-692f9228eab44d9db5652b58c1d8a635.png)

至此，完成postgres数据库的初始化。

4) 启动postgres数据库实例

#pg_ctl start

![](https://images0.cnblogs.com/blog/119630/201302/08210012-899114cc4f004b6a90d7882ab724af95.png)

可以看到postgresql数据库实例已经启动，通过下面的命令可以查看系统中运行的postgres进程

#ps -ef | grep postgres

![](https://images0.cnblogs.com/blog/119630/201302/08210441-700ef574b2be400db707702f6f58ba66.png)

5) 连接postgresql数据库

#psql -h 127.0.0.1 -d postgres -U postgres

![](https://images0.cnblogs.com/blog/119630/201302/08210147-7b3e91d3b9df494986ed32691f3110c0.png)

6) 停止postgresql数据库实例

#pg_ctl stop

#ps -ef |  grep postgres

![](https://images0.cnblogs.com/blog/119630/201302/08210626-f42ab2ccfab441e5834ec9803d743aca.png)

可以看到已经没有postgres进程

3. 设置PostgreSQL开机自启动

PostgreSQL的开机自启动脚本位于PostgreSQL源码目录的contrib/start-scripts路径下

![](https://images0.cnblogs.com/blog/119630/201303/05172453-11c4443c7ef045dbb1b013ab3cdc4f8f.png)

linux文件即为linux系统上的启动脚本

1）修改linux文件属性，添加X属性

#chmod a+x linux

2) 复制linux文件到/etc/init.d目录下，更名为postgresql

#cp linux /etc/init.d/postgresql

3）修改/etc/init.d/postgresql文件的两个变量

prefix设置为postgresql的安装路径：/opt/pgsql-9.1.2

PGDATA设置为postgresql的数据目录路径：

![](https://images0.cnblogs.com/blog/119630/201303/05172940-3a36ce9a2af746fa90ab1ceb31128a8d.png)

4) 执行service postgresql start，就可以启动PostgreSQL服务

#service postgresql start

![](https://images0.cnblogs.com/blog/119630/201303/05173131-1eb5af1328ac49d5b537ce9536a335bd.png)

5）设置postgresql服务开机自启动

#chkconfig --add postgresql

执行上面的命令，就可以实现postgresql服务的开机自启动。









