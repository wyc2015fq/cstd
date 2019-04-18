# MySQL中文支持 - Orisun - 博客园







# [MySQL中文支持](https://www.cnblogs.com/zhangchaoyang/articles/2023377.html)





MySQL中数据类型varchar(n)占用的最长字节是3*n.我们知道utf-8编码1个汉字占3个字节.

即

varchar(n)最多可以容纳3个英文字符或者3个汉字,当然混合的(如1个汉字2个字母)也行.

安装MySQL默认的编码是latin,修改

MySQL安装目录下的my.cnf文件,我的ubuntu10.04是在/etc/mysql路径下.

先关闭

MySQL-----$sudo /etc/init.d/mysql stop.

在[client]和[mysqld]下面分别加上这么一句话:

default-character-set=utf8

但是这之前你创建的database还是不支持中文.

或者在执行SQL语句前先执行"set names gb2312"或

"set names utf8"

注意如果没有关闭

MySQL就修改my.cnf文件那你就跟我一样悲剧了----/var/run/mysqld/下的2个文件都丢失了,其中包括mysqld.sock,这就导致你再也无法登录

MySQL了.

网上说了移找回

mysqld.sock文件的方法,但都不行,我重装MySQL了.

话说把MySQL卸了重装也不是那么容易滴,要把MySQL卸干净,你得:

1.sudo apt-get autoremove --purge mysql-server-5.1(我怎么知道我装的版本是5.1呢?可以去ubuntu软件中心里看一下)

2.sudo apt-get remove mysql-server

3.sudo apt-get autoremove mysql-server

4.sudo apt-get remove mysql-common //这个很重要

由于我之前还装了个phpMyAdmin,做完以上4步后出现一个Broken Package,这时候可以使用"新得力"解决,好像是叫这个名字,英文名字是"Synaptic Package Manager",在System/Administration下面.

Edit --> Fix Broken Packages --> Mark All Updgrades --> Apply

破损包修复完毕.

下面要重新安装MySQL了:

1.sudo apt-get install mysql-server

2.sudo apt-get install mysql-client

3.sudo apt-get install php5-mysql     // 安装php5-mysql 是将php和mysql连接起来

运行sudo netstat -tap | grep mysql

可以看到类似下面的行：

tcp 0 0 localhost.localdomain:mysql *:* LISTEN -

如果服务器不能正常运行，可以通过下列命令启动它：

sudo /etc/init.d/mysql restart

进入mysql

$mysql -u root -p

Password:















