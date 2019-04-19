# 在Ubuntu Server 32bit搭建hustoj -- 中 - dutong0321 -- 追寻计算机中的神灵 - CSDN博客
2016年05月11日 09:41:01[dutong0321](https://me.csdn.net/dutong0321)阅读数：5424标签：[ubuntu																[server																[hustoj](https://so.csdn.net/so/search/s.do?q=hustoj&t=blog)](https://so.csdn.net/so/search/s.do?q=server&t=blog)](https://so.csdn.net/so/search/s.do?q=ubuntu&t=blog)
个人分类：[Server](https://blog.csdn.net/dutong0321/article/category/6222374)
  OS：ubuntu server 32bit 14.04
  接下来就是安装HUST OJ了。
  1.换源，因为是校园网的环境，所以，中科大的源不错，阿里云的源在校园里也很牛，但是感觉中科大的更胜一筹，如果我在家里那就不用说了肯定是阿里的。
    先备份，再修改，最后更新。。。
    更新源后，必须更新软件，不然，安装hustoj可能会出错！
```
sudo cp /etc/apt/sources.list /etc/apt/sources.list.bak
sudo vim /etc/apt/sources.list
```
```
deb http://mirrors.ustc.edu.cn/ubuntu/ trusty main restricted universe multiverse
  deb http://mirrors.ustc.edu.cn/ubuntu/ trusty-security main restricted universe multiverse
  deb http://mirrors.ustc.edu.cn/ubuntu/ trusty-updates main restricted universe multiverse
  deb http://mirrors.ustc.edu.cn/ubuntu/ trusty-proposed main restricted universe multiverse
  deb http://mirrors.ustc.edu.cn/ubuntu/ trusty-backports main restricted universe multiverse
  deb-src http://mirrors.ustc.edu.cn/ubuntu/ trusty main restricted universe multiverse
  deb-src http://mirrors.ustc.edu.cn/ubuntu/ trusty-security main restricted universe multiverse
  deb-src http://mirrors.ustc.edu.cn/ubuntu/ trusty-updates main restricted universe multiverse
  deb-src http://mirrors.ustc.edu.cn/ubuntu/ trusty-proposed main restricted universe multiverse
  deb-src http://mirrors.ustc.edu.cn/ubuntu/ trusty-backports main restricted universe multiverse
```
```
sudo apt-get update
sudo apt-get upgrade
```
  2.安装软件，一次把能安的都安了，部分已经安装了，没事，apt-get会自动识别的，说不定有的还能更新一下呢。
```
sudo apt-get install gcc g++ openjdk-7-jdk apache2 mysql-server mysql-client subversion libmysqlclient-dev php5 libapache2-mod-php5 make flex
```
  3.修改mysql的默认字符集（上次没有更改，好像也能用，不过不好管理，最后也改了）。
```
sudo vim /etc/mysql/my.cnf
```
在里面的[mysqld]下，加入以下语句：
```
character-set-server	= utf8 
collation-server	= utf8_general_ci
```
  在里面的[client]下，加入下面的语句：`default-character-set	= utf8`  然后重启mysql服务`sudo /etc/init.d/mysql restart`  想要查看的话，可以打开mysql，输入一下指令：`show variables like 'char%';`如果出现这样就OK了：
```
+--------------------------+----------------------------+
| Variable_name            | Value                      |
+--------------------------+----------------------------+
| character_set_client     | utf8                       |
| character_set_connection | utf8                       |
| character_set_database   | utf8                       |
| character_set_filesystem | binary                     |
| character_set_results    | utf8                       |
| character_set_server     | utf8                       |
| character_set_system     | utf8                       |
| character_sets_dir       | /usr/share/mysql/charsets/ |
+--------------------------+----------------------------+
```
也可以编写一个小例子：
```
create database test;
use test
create table test(value varchar(20));
insert test VALUES("你好,世界");
select * from test;
```
效果图：
```
+---------------+
| value         |
+---------------+
| 你好,世界     |
+---------------+
```
 
 4.修改APACHE2的配置文档`sudo vim /etc/apache2/sites-enabled/000-default.conf `  修改地方：`DocumentRoot /var/www`  5.下载hustoj的源代码`sudo svn co https://github.com/zhblue/hustoj/trunk/trunk/install hustoj`  6.安装hustoj
```
cd hustoj/
sudo bash install-interactive.sh 
cd /var/www
sudo mv JudgeOnline/ ../
```
OK,到此为止，就全部安装成功了。可以，远程打开看一看。网址：http://地址/JudgeOnline
接下来是一些设置上面的问题，下次再说！

