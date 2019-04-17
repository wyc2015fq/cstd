# MySql默认编码所造成的乱码麻烦1.222 - 在思索中前行！ - CSDN博客





2014年10月13日 20:25:55[_Tham](https://me.csdn.net/txl16211)阅读数：580标签：[MySQL编码																[修改MySQL默认编码																[MySQL乱码](https://so.csdn.net/so/search/s.do?q=MySQL乱码&t=blog)
个人分类：[Database mysql](https://blog.csdn.net/txl16211/article/category/2333871)





## 1、前言

    MySQL在安装时，最后的一步，会让你选择MySQL服务器及客户端、数据库、连接接口的默认编码。通常可选择

UTF8和GB2312.

    但是，如果你选择了utf8的时候，恰好你要从另一个数据库上迁移一个gb2312编码格式的数据库过来，且那个系统的前端和后端都是gb2312编码的时候，直接新建数据库，导入数据恐怕会产生乱码了。

## 2、安装MySQL之后修改编码方式

MySQL的默认编码是Latin1，不支持中文，要支持中午需要把数据库的默认编码修改为gbk或者utf8。


1、需要以root用户身份登陆才可以查看数据库编码方式(以root用户身份登陆的命令为：
      >mysql -u root –p,之后两次输入root用户的密码)，查看数据库的编码方式命令为:



   >show variables like 'character%'; 或者 > status

+--------------------------+----------------------------+ 

| Variable_name | Value | 

+--------------------------+----------------------------+ 

| character_set_client | latin1 | 

| character_set_connection | latin1 | 

| character_set_database | latin1 | 

| character_set_filesystem | binary | 

| character_set_results | latin1 | 

| character_set_server | latin1 | 

| character_set_system | utf8 | 

| character_sets_dir | /usr/share/mysql/charsets/ | 

+--------------------------+----------------------------+



从以上信息可知数据库的编码为latin1，需要修改为gbk或者是utf8；

其中，character_set_client为客户端编码方式；

      character_set_connection为建立连接使用的编码；

      character_set_database数据库的编码；

      character_set_results结果集的编码；

      character_set_server数据库服务器的编码；

只要保证以上四个采用的编码方式一样，就不会出现乱码问题。

另一个查看数据库编码的命令：

>show variables like ‘collation%’;

2、linux系统下，修改MySQL数据库默认编码的步骤为：
- 


停止MySQL的运行

/etc/init.d/mysql start (stop) 为启动和停止服务器



- 
MySQL主配置文件为my.cnf，一般目录为/etc/mysql


var/lib/mysql/ 放置的是数据库表文件夹，这里的mysql相当于windows下mysql的date文件夹
- 


当我们需要修改MySQL数据库的默认编码时，需要编辑my.cnf文件进行编码修改,在linux下修改mysql的配置文件my.cnf，文件位置默认/etc/my.cnf文件 




找到客户端配置[client] 在下面添加 

default-character-set=utf8 默认字符集为utf8 

在找到[mysqld] 添加 

default-character-set=utf8 默认字符集为utf8 

init_connect='SET NAMES utf8' （设定连接mysql数据库时使用utf8编码，以让mysql数据库为utf8运行） 




修改好后，重新启动mysql 即可，重新查询数据库编码可发现编码方式的改变：






>show variables like 'character%'; 

+--------------------------+----------------------------+ 

| Variable_name | Value | 

+--------------------------+----------------------------+ 

| character_set_client | utf8 | 

| character_set_connection | utf8 | 

| character_set_database | utf8 | 

| character_set_filesystem | binary | 

| character_set_results | utf8 | 

| character_set_server | utf8 | 

| character_set_system | utf8 | 

| character_sets_dir | /usr/share/mysql/charsets/ | 

+--------------------------+----------------------------+ 




此方法用于标准mysql版本同样有效，对于/etc/my.cnf文件，需要从mysql/support-files的文件夹cp
 my-large.cnf一份到/etc/my.cnf



3、windows系统下可以删除MySQL数据库，并重新安装，在安装过程中可以直接用Ｍysql
 Server Instance Config Wizard 进行设置

  4、当MySQL数据库服务器已经有数据不适合删除重装时，可以个别指定数据库的编码方式。MySQL指定编码的方式是非常灵活并多样化的，可以指定表级别的编码，行级别编码，甚至可以指定字段级别的编码。

 以下示例给出创建数据库时指定编码的两种方式：

 1）CREATE DATABASE ms_db CHARACTER SET utf8 COLLATE utf8_general_ci；

 2）create database if not exists netctoss default character set utf8;

   5、如果你采用的是外部接入的方式，可以在连接中确定请求的编码格式如：jdbc:mysql://localhost:3306/mysql?useUnicode=true&characterEncoding=utf-8(注意：不要出现任何空格，否则出错)

   6、执行脚本：指定编码格式set names gbk(注意，不是UTF-8)可以修改

执行前：



执行后：



从执行命令前后可知，set names gbk只可以修改character_set_client、character_set_connection、character_set_results的编码方式，并且这种修改是窗口级别的，只针对本窗口有效，打开另外一个窗口修改无效。也可发现数据库底层的编码方式没有改变，插入数据后还是以utf8编码方式保持。

## 3、[修改mysql数据库的默认编码方式](http://blog.csdn.net/wangjun_pfc/article/details/3827631)


    修改my.ini文件

    加上 default-character-set=gb2312

    设定数据库字符集

    alter database da_name default character set 'charset'

    1)设置数据库编码 /etc/my.cnf（Linux下的文件）

    [mysqld]

    default-character-set=gbk

    ...

    [client]

    default-character-set=gbk

－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－

   2 )按字符集导出

  $mysqldump -u root -p dbname --default-character-set=gbk > a.sql;

   3)查看SQL文件的编码

   [ root@localhost gethtml]# file a.sql

      a.sql: UTF-8 Unicode ...

   [root@localhost gethtml]# iconv -f utf-8 -t gbk a.sql > a2.sql

   [root@localhost gethtml]# file a2.sql

      a2.sql: Non-ISO extended-ASCII English text 这时已经是gbk的编码了

  3)导入

  查看数据库服务和客户端字符集 

   mysql> status;

   Server characterset:    gbk

   Db     characterset:      gbk

   Client characterset:      latin1

   Conn. characterset:     latin1

   mysql> set names 'gbk';   //这样

   mysql> status;

  Server characterset:   gbk

  Db     characterset:     gbk

  Client characterset:     gbk

  Conn. characterset:    gbk

  这时才能导数据

  mysql> source a.sql;

----------------------------------------------------------------------------------

  单独设置某个数据库：

  alter database testdb character set utf8;

  查看mysql支持的编码：

  show character set;



  查看系统的字符集和排序方式的设定可以通过下面的两条命令：

  mysql> SHOW VARIABLES LIKE ''character_set_%'';

  LINUX 如下:

  1)导出数据

  [root@www.cnscn.org ~]$ mysqldump -u root -p dbname --default-character-set=gbk > base_user.sql;

  2)查看导出的sql文件的编码

  [root@www.cnscn.org ~]$ file base_user.sql 

　 base_user.sql: UTF-8 Unicode text, with very long lines

3)转成要用的编码

[root@www.cnscn.org ~]$ iconv -f utf-8 -t gbk base_user.sql >base_user_gbk.sql

4)连接数据库并查看当前库的编码

[root@www.cnscn.org ~]$ mysql -uroot

　mysql> use testdb;

mysql> status;

    Server characterset:    latin1

    Db     characterset:    gbk

    Client characterset:    latin1

    Conn. characterset:    latin1

5)设置成需要的编码

mysql>set names 'gbk';

6)查看现在的编码

mysql> status;

    Server characterset:    latin1

    Db     characterset:    gbk

    Client characterset:    gbk

    Conn. characterset:    gbk

7)导入我们转换成gbk后的文件

mysql> source base_user_gbk.sql;](https://so.csdn.net/so/search/s.do?q=修改MySQL默认编码&t=blog)](https://so.csdn.net/so/search/s.do?q=MySQL编码&t=blog)




