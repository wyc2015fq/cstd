# linux下安装PostgreSQL数据库 - 菜鸟路上的小白 - CSDN博客





2016年09月06日 09:48:52[somnus_小凯](https://me.csdn.net/u012486840)阅读数：6182








0.编译环境
- Linux: CentOS 5.5
- gcc: 4.1.2

1. 安装PostgreSQL

1) 解压postgresql-9.1.7.tar.bz2

#tar jxvf postgresql-9.1.7.tar.bz2

![](https://img-blog.csdn.net/20160921154332357?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

2) 进入解压后的postgresql-9.1.7目录

#cd postgresql-9.1.7

![](https://img-blog.csdn.net/20160921154345264?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

3) 编译postgresql源码

#./configure --prefix=/opt/pgsql-9.1.7

![](https://img-blog.csdn.net/20160921154356217?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

 #make

![](https://img-blog.csdn.net/20160921154411545?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

#make install

![](https://img-blog.csdn.net/20160921154421811?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

至此，完成postgresql的安装。进入/opt/pgsql-9.1.7目录可以看到安装后的postgresql的文件。

#ls /opt/pgsql-9.1.7

![](https://img-blog.csdn.net/20160921154435530?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

2.创建postgresql数据库

1) 创建postgres用户

#useradd postgres

修改postgres密码

#passwd postgres

![](https://img-blog.csdn.net/20160921154448577?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

2) 设置postgres用户的环境变量

切换到postgres用户

#su - postgres

![](https://img-blog.csdn.net/20160921154500015?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

进入postgres的主目录

#cd ~

![](https://img-blog.csdn.net/20160921154511796?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

编辑~/.bash_profile文件

#vi ~/.bash_profile

设置以下的环境变量

export PGHOME=/opt/pgsql-9.1.7

export PGDATA=~/data

![](https://img-blog.csdn.net/20160921154526547?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

保存，退出vi。执行以下命令，使环境变量生效

#source ~/.bash_profile

![](https://img-blog.csdn.net/20160921154537860?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

3) 初始化postgres数据库

#initdb

![](https://img-blog.csdn.net/20160921154549845?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

至此，完成postgres数据库的初始化。

4) 启动postgres数据库实例

#pg_ctl start

![](https://img-blog.csdn.net/20160921154601734?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

可以看到postgresql数据库实例已经启动，通过下面的命令可以查看系统中运行的postgres进程

#ps -ef | grep postgres

![](https://img-blog.csdn.net/20160921154613111?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

5) 连接postgresql数据库

#psql -h 127.0.0.1 -d postgres -U postgres

![](https://img-blog.csdn.net/20160921154625892?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

6) 停止postgresql数据库实例

#pg_ctl stop

#ps -ef |  grep postgres

![](https://img-blog.csdn.net/20160921154637205?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

可以看到已经没有postgres进程

3. 设置PostgreSQL开机自启动

PostgreSQL的开机自启动脚本位于PostgreSQL源码目录的contrib/start-scripts路径下

![](https://img-blog.csdn.net/20160921154648156?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

linux文件即为linux系统上的启动脚本

1）修改linux文件属性，添加X属性

#chmod a+x linux

2) 复制linux文件到/etc/init.d目录下，更名为postgresql

#cp linux /etc/init.d/postgresql

3）修改/etc/init.d/postgresql文件的两个变量

prefix设置为postgresql的安装路径：/opt/pgsql-9.1.2

PGDATA设置为postgresql的数据目录路径：

![](https://img-blog.csdn.net/20160921154659831?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

4) 执行service postgresql start，就可以启动PostgreSQL服务

#service postgresql start

![](https://img-blog.csdn.net/20160921154710237?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

5）设置postgresql服务开机自启动

#chkconfig --add postgresql

执行上面的命令，就可以实现postgresql服务的开机自启动。



