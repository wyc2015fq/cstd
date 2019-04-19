# CentOs7下安装mysql5.7.23（rpm安装） - Big Smile - CSDN博客
2018年07月29日 15:42:46[王啸tr1912](https://me.csdn.net/tr1912)阅读数：5763
所属专栏：[mysql系列](https://blog.csdn.net/column/details/26482.html)
# 准备
        一台装有centos7.3的电脑（或虚拟机），mysql-5.7.23-1.el7.x86_64.rpm-bundle.tar   mysql的rpm安装集合包
下载地址：[https://dev.mysql.com/downloads/mysql/5.7.html#downloads](https://dev.mysql.com/downloads/mysql/5.7.html#downloads)
![](https://img-blog.csdn.net/20180729144754571?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3RyMTkxMg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
注意这样选择，可以获得centos支持的版本。
# 一、安装环境
        我们先把下载的mysql-5.7.23-1.el7.x86_64.rpm-bundle.tar 上传到我们的centos系统中，最好传到/opt 文件夹中
     然后我们建立一个新的，用于盛放tar文件释放后文件的文件夹：
```
cd /opt
mkdir mysql
```
然后运行解压命令：
`tar -xf mysql-5.7.23-1.el7.x86_64.rpm-bundle.tar -C mysql`
这时，我们再到mysql的目录下就可以看到解压出来的rpm包：
![](https://img-blog.csdn.net/20180729145431903?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3RyMTkxMg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
资源都准备好了，现在我们来看一下环境：
    由于CentOs7之后会在系统中自带一个免费的数据库：mariadb，并且它的驱动和mysql的驱动是有冲突的，所以我们必须先要卸载他。（ps.mariadb和mysql是同一个作者创作的数据库，由于权益等其他问题才分开的，所以有兴趣的同志可以研究一下。）
   首先用rpm命令查看一下是否有mariadb，然后再删除：
`rpm -qa | grep -i mariadb`
 可能会出现这种情况：
![](https://img-blog.csdn.net/20180729150025845?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3RyMTkxMg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
然后我们用rpm命令删除这个版本的mariadb：
`rpm -e --nodeps mariadb-libs-5.5.52-1.el7.x86_64`
这个名字在不同版本的系统上可能不一样，大家需要注意。
如果是centos7之前的版本的话，可能会自带mysql，这是我们需要按照上述步骤删除mysql，这里不再赘述。
由于mysql的server端需要perl语言的支持，因此我们还需要在系统中安装perl支持：
`yum install perl -y`
# 二、安装
        由于我们上面解压出来的几个rpm包有依赖关系，所以我们需要按照顺序执行安装：
```
rpm -ivh mysql-community-common-5.7.23-1.el7.x86_64.rpm
rpm -ivh mysql-community-libs-5.7.23-1.el7.x86_64.rpm
rpm -ivh mysql-community-client-5.7.23-1.el7.x86_64.rpm
rpm -ivh mysql-community-server-5.7.23-1.el7.x86_64.rpm
```
这几个包的安装完成之后，我们的数据库也就安装完成了，有几个需要注意的地方：
- 数据库目录：/var/lib/mysql/
- 命令配置：/usr/share/mysql  (mysql.server命令及配置文件)
- 相关命令：/usr/bin   (mysqladmin mysqldump等命令)
- 启动脚本：/etc/rc.d/init.d/   (启动脚本文件mysql的目录)
- 系统配置：/etc/my.conf
# 三、运行-初始化
        安装完成了，我们需要先把mysql运行起来，这样我们才能进行接下来的配置。
ps.强烈建议在root用户环境下进行初始化配置，方便用户和密码的创建。
        现在我们可以先看一下 /var/lib/mysql/  目录下是否有东西，我们可以发现这个目录下是没有东西的，或者是连mysql这个目录都没有，也就是说数据库还没有初始化。
注意：
这里我跳过了初始化root用户，或者是mysql用户的步骤，因为网上看在5.7.12上可以成功的步骤在5.7.23上却会在启动步骤报错，原因未知。
         现在我们来配置mysql的启动，个人比较推荐使用服务的方式启动（centos7特有）：
`systemctl start mysqld.service`
 这样就把mysqld这个服务添加到系统中了，我们可以用service进行启动了，启动，停止，重启的命令如下：
启动：
```
使用 service 启动：service mysqld start
使用 mysqld 脚本启动：/etc/inint.d/mysqld start
使用 safe_mysqld 启动：safe_mysqld&
```
停止：
```
使用 service 启动：service mysqld stop
使用 mysqld 脚本启动：/etc/inint.d/mysqld stop
mysqladmin shutdown
```
重启：
```
使用 service 启动：service mysqld restart
使用 mysqld 脚本启动：/etc/inint.d/mysqld restart
```
我们在启动了mysql之后，这个时候等待一段时间，数据库需要进行初始化，这个时候我们可以看一下数据库的日志，会发现mysql帮我们初始化了一个root用户：
`cat /var/log/mysqld.log`
![](https://img-blog.csdn.net/20180729144903346?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3RyMTkxMg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
由于服务已经启动了，所以可以用root和这个密码登陆了：
`mysql -u root -p `
password 输入自己获得的密码
修改密码：
`set password = password('你的密码');`
设置外部远程访问权限：
```
grant all privileges on *.* to 'root' @'%' identified by '123456'; 
flush privileges;
```
**开放端口：**
         这里设置完之后，就可以在其他局域网内电脑上用Navicat连接一下看看能不能连接了，如果遇到带有数字的报错（非denind）的话，可以尝试开启防火墙的端口限制：
```
#开启3306端口
firewall-cmd --zone=public --add-port=3306/tcp --permanent  
#刷新防火墙
firewall-cmd --reload
```
防火墙操作学习：[https://www.cnblogs.com/moxiaoan/p/5683743.html](https://www.cnblogs.com/moxiaoan/p/5683743.html)
**设置开机启动：**
`chkconfig mysqld on`
至此，我们的mysql数据库就已经安装完成，并能随系统启动和外部访问了。
