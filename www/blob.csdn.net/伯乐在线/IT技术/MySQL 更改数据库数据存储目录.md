# MySQL 更改数据库数据存储目录 - 文章 - 伯乐在线
原文出处： [潇湘隐者](https://www.cnblogs.com/kerrycode/p/4371938.html)
MySQL数据库默认的数据库文件位于/var/lib/mysql下，有时候由于存储规划等原因，需要更改MySQL数据库的数据存储目录。下文总结整理了实践过程的操作步骤。
**1：确认MySQL数据库存储目录**


```
[root@DB-Server tmp]# mysqladmin -u root -p variables | grep datadir
Enter password: 
| datadir | /var/lib/mysql/
```
**2：关闭MySQL服务**
在更改MySQL的数据目录前，必须关闭MySQL服务。
**方式****1****：**


```
[root@DB-Server ~]# service mysql status
 
MySQL running (9411)[ OK ]
 
[root@DB-Server ~]# service mysql stop
 
Shutting down MySQL..[ OK ]
 
[root@DB-Server ~]#
```

![clip_image001](http://jbcdn2.b0.upaiyun.com/2018/11/760989e65b8e4826f6b4d7e0f1946381.png)
**方式****2****：**


```
[root@DB-Server ~]# /etc/rc.d/init.d/mysql status
 
MySQL running (8900)[ OK ]
 
[root@DB-Server ~]# /etc/rc.d/init.d/mysql stop
 
Shutting down MySQL..[ OK ]
 
[root@DB-Server ~]#
```
**3：创建新的数据库存储目录**


```
[root@DB-Server ~]# cd /u01
[root@DB-Server u01]# mkdir mysqldata
```
**4：移动MySQL数据目录到新位置**


```
[root@DB-Server ~]# mv /var/lib/mysql /u01/mysqldata/
```
**5****：****修改配置文件****my.cnf**
并不是所有版本都包含有my.cnf这个配置文件，在MySQL 5.5版本，我就找不到my.cnf这个配置文件， 而有些MySQL版本该文件位于/usr/my.cnf，如果/etc/目录下没有my.cnf配置文件，请到/usr/share/mysql/下找到*.cnf文件，拷贝其中一个到/etc/并改名为my.cnf中。命令如下：
![clip_image003](http://jbcdn2.b0.upaiyun.com/2018/11/13bbfad29ea85c8b40006710b7275b63.png)


```
[root@DB-Server mysql]# cp /usr/share/mysql/my-medium.cnf /etc/my.cnf
```
编辑/etc/my.cnf文件，修改参数socket
**                         MySQL 5.5 版本**
**![clip_image004](http://jbcdn2.b0.upaiyun.com/2018/11/08b0c8cd51a5b4577634c2ad22b69f76.png)**


```
# The following options will be passed to all MySQL clients
[client]
#password       = your_password
port            = 3306
socket          = /u01/mysqldata/mysql/mysql.sock
 
# Here follows entries for some specific programs
 
# The MySQL server
[mysqld]
port            = 3306
socket          = /u01/mysqldata/mysql/mysql.sock
skip-external-locking
key_buffer_size = 16M
max_allowed_packet = 1M
table_open_cache = 64
sort_buffer_size = 512K
net_buffer_length = 8K
read_buffer_size = 256K
read_rnd_buffer_size = 512K
myisam_sort_buffer_size = 8M
```

![clip_image005](http://jbcdn2.b0.upaiyun.com/2018/11/bbead4cc2ef2124a8c2124f6497425d4.png)
**6：修改启动脚本/etc/init.d/mysql**
将参数datadir修改为datadir=/u01/mysqldata/mysql/
![clip_image006](http://jbcdn2.b0.upaiyun.com/2018/11/3f02af33620a5568586c971e1c3e6981.png)
**7：启动MySQL服务并验证MySQL数据库路径**


```
[root@DB-Server ~]# service mysql start
Starting MySQL..[  OK  ]
[root@DB-Server ~]# mysqladmin -u root -p variables | grep datadir
Enter password: 
| datadir        | /u01/mysqldata/mysql/
```

**我的疑问：**
1： 在修改数据库的存储目录前，/var/lib/mysql/目录下根本没有mysql.sock文件，安装上面配置后，就会生成mysql.sock文件。
关于mysql.sock文件，搜索了一下资料：mysql.sock是用于socket连接的文件。也就是只有你的守护进程启动起来这个文件才存在。但是你的mysql程序（这个程序是客户端，服务器端是mysqld）可以选择是否使用mysql.sock文件来连接（因为这个方法只适合在Unix主机上面连接本地的mysqld），对于非本地的任何类型的主机。那么这个文件是否一定需要的呢？ 这个需要进一步了解清楚。
2：我在网上看有些网友总结的修改MySQL数据路径，有些需要给新建的目录的权限做一些处理，而有些有不用对目录权限进行授权，我没有处理，也没有什么问题。到底要不要对新的数据库目录授权呢？
3：我在MySQL_5.6.20这个版本测试时，不修改my.cnf，只修改启动脚本/etc/init.d/mysql，也完全没有啥问题。也没有myssql.sock文件生成。
4: 注意如果没有禁用selinux, 修改MySQL的数据路径后启动MySQL服务会遇到一些错误。关于这个的解释是后台服务都需要有对相应目录的对应权限，而 mysql 的默认路径/var/lib/mysql 已经添加了相应的策略，修改路径后由于没有相应的策略，导致后台进程读取文件被selinux阻止，从而出现权限错误。 所以要么关闭Selinux或修改文件安全上下文。


```
[root@DB-Server mysql]# /etc/init.d/mysql start
 
Starting MySQL....The server quit without updating PID file (/u01/mysqldata/mysql//DB-Server.localdomain.pid).[FAILED]
 
[root@DB-Server mysql]# 
 
[root@DB-Server mysql]# chcon -R -t mysqld_db_t /u01/mysqldata/mysql/
 
[root@DB-Server mysql]# /etc/init.d/mysql start
 
Starting MySQL.[ OK ]
 
[root@DB-Server mysql]#
```

**参考资料：**
[http://database.ctocio.com.cn/tips/449/7566949.shtml](http://database.ctocio.com.cn/tips/449/7566949.shtml)
[www.linuxidc.com/Linux/2012-12/75647.htm](www.linuxidc.com/Linux/2012-12/75647.htm)
[http://blog.csdn.net/hellyhe/article/details/8309470](http://blog.csdn.net/hellyhe/article/details/8309470)
