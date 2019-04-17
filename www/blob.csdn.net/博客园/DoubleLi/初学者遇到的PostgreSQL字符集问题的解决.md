# 初学者遇到的PostgreSQL字符集问题的解决 - DoubleLi - 博客园






当初学者在使用PostgreSQL数据库，输入中文时，会遇到“ERROR:  invalid byte sequence for encoding "UTF8": 0xd6d0”的错误，原因是由于没有正确设置客户端字符集。



问题的原因：

默认情况下，PostgreSQL是不转换字符集的，如果你的数据库是UTF8的字符集，一般终端的中文字符集会设置为GBK，或en_US（查看终端的字符集可以看LANG环境变量的设置），所以你输入的中文是GBK的编码，这个编码不经转换的存入数据库中，而数据库是UTF8的，PostgreSQL一看没有这样的UTF8编码，所以当然报错了。

解决方法为：

方法一：设置postgresql的客户端编码为GBK，这时PostgreSQL就知道输入的内容是GBK编码的，这样PostgreSQL数据库会自动做字符集的转换，把其转换成UTF8编码。

方法二：直接设置终端的字符集编码为UTF8，让输入的编码直接为UTF8，而不是GBK。



看我具体的演示：



方法一：设置postgresql的客户端编码：

设置psql客户端字符集为GBK，方法有两种，一种是在psql中输入“\encoding GBK” ，另一种是设置环境变量“export PGCLIENTENCODING=GBK”，看我的演示：
#psql -d dsc
dsc=# insert into t values(1,'中国');
ERROR:  invalid byte sequence for encoding "UTF8": 0xd6d0
HINT:  This error can also happen if the byte sequence does not match the encoding expected by the server, which is controlled by "client_encoding".
dsc=# show client_encoding;
 client_encoding 
-----------------
 UTF8
(1 row)

dsc=# \encoding GBK  
dsc=# show client_encoding;
 client_encoding 
-----------------
 GBK
(1 row)

dsc=# insert into t values(1,'中国');
INSERT 0 1
dsc=# commit;
WARNING:  there is no transaction in progress
COMMIT
dsc=# select * from t;
 id | name 
----+------
  1 | 中国
(1 row)


[postgres@dsc ~]$ export PGCLIENTENCODING=GBK
[postgres@dsc ~]$ psql
psql: FATAL:  conversion between GBK and LATIN1 is not supported
[postgres@dsc ~]$ psql -d dsc
psql (8.4.3)
Type "help" for help.

dsc=# select * from t;
 id | name 
----+------
  1 | 中国
(1 row)

dsc=# insert into t values(2,'我的中国');
INSERT 0 1
dsc=# select * from t;                   
 id |   name   
----+----------
  1 | 中国
  2 | 我的中国
(2 rows)





方法二：设置终端的编码为UTF8：



[postgres@dsc ~]$ export LANG=zh_CN.UTF8



然后修改终端软件的字符集编码，我使用的是SecureCRT，修改方法为：

Option->Session Option->外观->字符编码，把那个下拉框的内容改成“UTF8”:

![](http://blogimg.chinaunix.net/blog/upfile2/100405232145.jpg)

然后再插入数据测试：


[postgres@dsc ~]$ psql -d dsc
psql (8.4.3)
Type "help" for help.

dsc=# select * from t;
 id |   name   
----+----------
  1 | 中国
  2 | 我的中国
(2 rows)

dsc=# insert into t values(3,'我的中国');
INSERT 0 1
dsc=# select * from t;                   
 id |   name   
----+----------
  1 | 中国
  2 | 我的中国
  3 | 我的中国
(3 rows)









