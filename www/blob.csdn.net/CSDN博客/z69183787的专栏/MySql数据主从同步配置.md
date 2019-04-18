# MySql数据主从同步配置 - z69183787的专栏 - CSDN博客
2016年12月27日 18:31:06[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1088
由于需要配置[MySQL](http://lib.csdn.net/base/mysql)的主从同步配置，现将配置过程记录下，已被以后不时之需
# MySql数据主从同步
## 1.1. 同步介绍
Mysql的 主从同步 是一个异步的复制过程，从一个 Master复制到另一个 Slave上。在 Master 与 Slave 之间的实现整个复制过程主要由三个线程来完成，其中两个线程(Sql线程和IO线程)在 Slave 端，另外一个线程(IO线程)在 Master 端。　
　  要实现 MySQL 的 主从同步 ，首先必须打开 Master 端的BinaryLog(mysql-bin)功能，否则无法实现。因为整个复制过程实际上就是Slave从Master端获取该日志然后再在自己身上完全顺序的执行日志中所记录的各种操作。打开 MySQL 的 Binary Log 可以通过在启动 MySQL Server 的过程中使用 “—log-bin” 参数选项，或者在
 my.cnf 配置文件中的 mysqld 参数组([mysqld]标识后的参数部分)增加 “log-bin” 参数项。
## 1.2. Mysql主从服务器配置
主服务器IP:192.168.1.60
从服务IP:192.168.1.61
[Linux](http://lib.csdn.net/base/linux):centOS
Mysql版本：5.1.58
以下配置皆在此服务上进行配置
**一、主[数据库](http://lib.csdn.net/base/mysql)操作**
查看主库服务器的my.cnf配置文件，根据mysql安装包、版本不同，相关的路径和配置文件路径会大致不一样，不过大多情况下是一样的。该文件在 etc/目录下。Mysql服务也提供了对不同服务器配置使用不同的配置文件，要使用它们的时候，只需用它们任一一个替换上面的my.cnf文件即可。注：替换文件名必须为my.cnf。
cnf配置文件一般会有四个，可以根据命令 # ll /usr/share/mysql/*.cnf 查看；
my-small.cnf    内存少于或等于64M，只提供很少的的数据库服务；
my-medium.cnf   内存在32M--64M之间而且和其他服务一起使用，例如web；
my-large.cnf    内存有512M主要提供数据库服务；
my-huge.cnf     内存有1G到2G，主要提供数据库服务；
my-innodb-heavy-4G.cnf 内存有4G，主要提供较大负载数据库服务（一般服务器都使用这个）；
可以根据服务器配置的不同选择不同的cnf配置文件。
1、        配置my.cnf文件
采用系统默认的etc/my.cnf配置文件进行配置，对主服务器进行配置主要需要在配置文件中添加以下几项（在添加的时候要确保它们不在文件中存在，因为如选择上面几种不同的配置文件，配置里的内容会不一样,如果没有相应的项，需要添加）。
用vi etc/my.cnf打开文件，对文件进行修改，在[mysqld]下面进行添加修改：
**Server-id = 1**  #这是数据库ID,此ID是唯一的，主库默认为1，其他从库以此ID进行递增，ID值不能重复，否则会同步出错；
**log-bin = mysql-bin**  二进制日志文件，此项为必填项，否则不能同步数据；
**binlog-do-db = testcreate**  #需要同步的数据库，如果需要同步多个数据库；
则继续添加此项。
*binlog-do-db = testcreate1*
*binlog-do-db = testcreate2*
**binlog-ignore-db = mysql** 不需要同步的数据库；
至此主服务器配置my.cnf配置完成，保存退出文件。
现在对Mysql服务器进行重启：#servicemysqld restart 如果重启失败，则证明配置文件错误，需重新检查。
建议：对my.cnf修改之前最好对其进行备份，以免配置文件修改失败不能及时重启服务器。
2、        配置完成后需要建立同步用的数据库账户
主库需要提供一个账户让从库对起连接并同步，用命令mysql服务器
mysql>grantreplication slave on *.* to ‘testcreate’@’192.168.1.61’ identified by ‘123456’;
说明：’testcreate’：同步的数据库；
      ‘192.168.1.61’：同步的数据库地址；
        ‘123456’ ：同步的数据库密码，在从库配置时需要设置。
3、        显示主库信息
mysql>showmaster status;
执行上述命令显示：
看到上述信息则表示主库建立成功。
4、        备份数据库(如果从服务器也为新建库，则不需要此操作)
备份数据库的方法有很多种：如果在同一局域网可以直接复制数据库，也可以用工具直接进行数据导入。
复制数据库用打包的方法：
# cd/data  转到存放Mysql数据库的路径下
# tarcvfz testcreate.tar.gz db/
再从数据库中进行复制解压
# cd/data
# scp192.168.1.61:/data/testcreate.tar.gz
#tarxvfz testcreate.tar.gz
数据复制完毕；
**二、 从库配置**
从服务器的配置基本与主库配置差不多，选择的从库服务器为：192.168.1.61
1.    配置从库服务my.cnf文件
# vietc/my.cnf 对其进行如下修改：
**Server-id = 2** #这里ID改为2 因为主库为1;
**log-bin = mysql-bin  **必填项，用于数据同步;
**master-host = 192.168.1.60** 主库IP;
**master-user = test** 同步用的账户;
**master-password = 123456** 同步账户密码，主库时的设置;
**master-port = 3306** 同步数据库的端口号。
2.     重启从库服务器
#service mysqld restart
如果重启失败则配置文件有问题，需重新检查配置。
3.    调整复制过来的数据库的权限（如果有从主库复制过来的数据库）
主库复制过来的数据库权限都为root权限，需要修改为mysql权限
#chown–R mysql:mysql /var/lib/mysql
如果从库是新建的库就不需要进行此步骤
4.    显示从服务器状态
进行mysql服务器：
mysql>showslave status\G;  显示如图：
![](http://hi.csdn.net/attachment/201112/22/0_1324568301n48v.gif)
如果slave_io_running和slave_sql_running都为yes则表示从服务器配置成功。
如果遇到关于position相关的错误，就需要再手动设置以下File/Postion的信息
mysql>Changemaster to master_host = ‘192.168.1.60’
master_user= ‘test’
master_password=’123456’
master_log_file=’mysql-bin.000001’,  主库的文件信息
master_log_pos=’106’;主库的Position信息
**三、     主从服务器[测试](http://lib.csdn.net/base/softwaretest)**
在主服务器和从服务器建立同样的数据库：testCreate 表为:test,字段为ID,name
往主服务器添加数据
![](http://hi.csdn.net/attachment/201112/22/0_1324568430B8Br.gif)
此时从服务器192.168.1.61中testcreate数据库显示为
![](http://hi.csdn.net/attachment/201112/22/0_1324568430B8Br.gif)
表示主从服务器同步成功。
**四、     常见问题**
在配置成服务器时，从库不能进行修改操作，即从库只有读的权限，如果修改了从库数据库，则会造成同步失败：如发现失败，用showslave status\G;查看从服务器的状态信息
如果出现slave_IO_Running:为NO时，则表示从数据库连接失败
Last_Error:会显示出出错日志信息
![](http://hi.csdn.net/attachment/201112/22/0_1324568195Uehm.gif)
出现此问题主要是事物回滚问题，解决方法有：
1、
mysql>slave stop; 停止从服务 
mysql>set GLOBAL SQL_SLAVE_SKIP_COUNTER=1;
mysql>slave start; 启动从服务
2、手动重设从服务器
mysql>Changemaster to
master_host= ‘192.168.1.60’
master_user= ‘test’
master_password=’123456’
master_log_file=’mysql-bin.000001’; 主库的文件信息
master_log_pos=’106’;主库的Position信息
到此Mysql主从服务配置完毕
在主服务器中，主要是以写数据为主，建议用InnoDB数据引擎，在读方面用Mysql中的MyISAM引擎读的效率很不错。
MyIsAM引擎支持全文索引，但不支持事务，而InnoDB恰好相反！
