# mysql远程访问 - 左直拳的马桶_日用桶 - CSDN博客
2018年12月26日 16:26:18[左直拳](https://me.csdn.net/leftfist)阅读数：108
mysql本地访问是没有什么问题的，但远程就不然了。mysql默认是不支持远程访问的，问你死未。
那么要怎么样才能远程访问？有两个条件，缺一不可：
**一、mysql本身要允许**
**1、设置现有账号支持远程访问**
刚装好的mysql，只有一个账号，就是管理员root，限定只能本机访问。
```
mysql>SELECT User, Host FROM mysql.user;
```
```
+------+-----------+
| user | host      |
+------+-----------+
| root | 127.0.0.1 |
| root | ::1       |
| root | localhost |
+------+-----------+
3 rows in set (0.14 sec)
```
可以将host改为’%’，这样就能被所有机器访问了。
```
mysql>update user set host = '%' where user = 'root';
```
**2、创建一个可以远程访问的账号**
当然更好的办法是新建一个用户，开放给远程访问。root毕竟是管理员，权限太大了。
```
mysql>create user 'test' identified by '123456'；
mysql>grant all on db1.* to test@'%';
```
然后找另外一台机来访问一下：
大写P后接端口，大写D后接数据库名称。
```
C:\Users\Administrator> mysql -h 192.168.0.248 -P 3306 -D db1 -u test -p
```
**二、mysql所在服务器要允许**
通常是防火墙的问题。
安装mysql的时候，它设置了端口3306对外开放，但这个对外，只是对同一个域，所以，如果局域网内的其他机器，不在同一个域内，仍然无法连接。这时还要对专用、公用设为开放：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181226162538370.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xlZnRmaXN0,size_16,color_FFFFFF,t_70)
