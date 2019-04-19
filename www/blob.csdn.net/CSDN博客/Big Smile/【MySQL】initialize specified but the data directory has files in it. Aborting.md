# 【MySQL】initialize specified but the data directory has files in it. Aborting - Big Smile - CSDN博客
2018年07月29日 15:56:54[王啸tr1912](https://me.csdn.net/tr1912)阅读数：5343
所属专栏：[mysql系列](https://blog.csdn.net/column/details/26482.html)
我们在安装mysql的时候可能会遇到这个问题：
![](https://img-blog.csdn.net/20180729154443567?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3RyMTkxMg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
是不是执行了这个命令呢？
`mysqld --initialize --user=mysql`
        报这个错误的原因是因为你的mysql数据库已经进行初始化了，所以不能用这种方式再进行初始化用户了，因为mysql在初始化的时候会自动创建一个root用户的。
这个时候我们应该怎么办呢？
**1、找到数据库目录**
我们可以在mysql的配置文件中找到数据库的目录：
`vi /etc/my.cnf`
在其中我们可以找到datadir这个配置：
![](https://img-blog.csdn.net/20180729155019333?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3RyMTkxMg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
这里配置的就是我们的mysql数据库的目录
**2、移动（备份）数据库文件**
首先要保证mysql处于停止状态，然后
```
cd /var/lib
mv mysql mysql_bak
```
我们这里是把mysql文件夹重命名成了mysql_bak，然后我们就可以执行初始化用户的语句了：
`mysqld --initialize --user=mysql`
然后我们可以启动数据库看看能不能用这个账户登陆了。
**其他方法**
移动了数据库文件之后，我们可以清理一下日志文件：
`echo ''->/var/log/mysqld.log`
然后重新启动mysql的服务：
`service mysqld start`
这个时候会在日志中看到数据库初始化时候创建的root用户和密码：
![](https://img-blog.csdn.net/20180729155626490?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3RyMTkxMg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
之后，我们可以用这个密码登陆数据库进行修改密码的操作了。
