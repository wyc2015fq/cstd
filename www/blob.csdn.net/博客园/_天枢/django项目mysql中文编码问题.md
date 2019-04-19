# django项目mysql中文编码问题 - _天枢 - 博客园
## [django项目mysql中文编码问题](https://www.cnblogs.com/yhleng/p/8566344.html)
2018-03-14 11:26 by _天枢, ... 阅读, ... 评论, [收藏](#),  [编辑](https://i.cnblogs.com/EditPosts.aspx?postid=8566344)
**在做django+mysql项目的时候，遇到中文报错问题。**
**问题分析：是由于mysql数据库，字符集的问题**
**在cmd命令行模式进入mysql**
```
mysql -uroot -p以root身份进入mysql客户端,
show variables like 'char%';查看数据库编码
```
**发现有一些不是utf8**
```
| character_set_database   | latin1                     |
| character_set_server     | latin1                     |
```
**那么怎么修改成utf8从而支持中文呢。**
在mysql跟目录下，有一个my.ini  如果没有手动建一个my.ini文件。
填入以下内容即可：
```
[mysqld]
basedir="D:/software/mysql"
datadir="D:/software/mysql/data"
port=3306
init_connect='SET NAMES utf8'
character-set-server = utf8
[client]
port=3306
default-character-set=utf8
```
```
找到[client]添加default-character-set=utf8
找到[mysqld]添加character-set-server = utf8
```
**网上资料很多，但是不一定好使哈，这主要是根据mysql版本有关。**
```
**网上有的解决办法是在[mysqld]中添加default-character-set=utf8这个方法在新版中是没有的。 如果你添加了这个，mysql服务启动会报错。无法启动。**
```
**我的mysql版本是5.7.19   按照上边修改完my.ini配置后，以管理员的身份进么cmd，输入net start mysql。服务启动成功。**
**注：**有一部分人可能说net start mysql命令不好使啊，那我可以告诉你，是因为你没有安装mysql服务。这样的话你要启动只能在mysql/bin目录下找到mysqld进行启动。
或者使用mysqld install进行安装服务。之后就可以用net start了。
进入数据库后：**show variables like 'char%';  编码已经修改。**
```
mysql> show variables like 'char%';
+--------------------------+-----------------------------------+
| Variable_name            | Value                             |
+--------------------------+-----------------------------------+
| character_set_client     | utf8                              |
| character_set_connection | utf8                              |
| character_set_database   | utf8                              |
| character_set_filesystem | binary                            |
| character_set_results    | utf8                              |
| character_set_server     | utf8                              |
| character_set_system     | utf8                              |
| character_sets_dir       | D:\software\mysql\share\charsets\ |
+--------------------------+-----------------------------------+
8 rows in set, 1 warning (0.00 sec)
```
