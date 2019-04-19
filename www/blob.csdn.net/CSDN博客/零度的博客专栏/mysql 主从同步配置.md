# mysql 主从同步配置 - 零度的博客专栏 - CSDN博客
2016年10月26日 10:52:13[零度anngle](https://me.csdn.net/zmx729618)阅读数：293标签：[mysql																[主从复制](https://so.csdn.net/so/search/s.do?q=主从复制&t=blog)](https://so.csdn.net/so/search/s.do?q=mysql&t=blog)
个人分类：[mysql																[数据库](https://blog.csdn.net/zmx729618/article/category/3108441)](https://blog.csdn.net/zmx729618/article/category/6441555)
先讲讲数据库的主从同步，两个好处：
　　 一是读写分离可以用上。比如 写操作就写到主数据库，读就从从数据库读取
         二是纯粹给数据库备份，以防硬盘彻底崩了
主从数据库操作准备：
       两台电脑，都安装好mysql 5.5及以上版本，最好两台电脑都安装同一版本数据库，且能单独正常使用。记住两台电脑的ip地址：
       主数据库：192.168.0.102   从数据库：192.168.0.104
【主数据库下操作】
步骤一：
       在主数据库mysql安装目录下找到my.ini文件打开，在mysqld下添加主服务器配置
       server-id=1    #服务器 id 
       log-bin=C:\Program Files\MySQL\MySQL Server 5.5\mysql-bin    #二进制文件存放路径
       binlog-do-db=test1    #待同步的数据库
       保存并重启mysql服务后你可以在C:\Program Files\MySQL\MySQL Server 5.5 下看到mysql-bin.index 文件和mysql-bin.000001文件
       打开终端，输入用户名和密码：mysql -u用户名 -p密码 (    mysql -uroot -proot   )
步骤二：
       给要连接的从服务器设置权限：grant replication slave,reload,super on *.* to slave@192.168.0.104 identified by 'root';    
       ps： 给主机192.168.0.104添加权限，用户名：slave，密码：root；（只需输入一次就可以了）
步骤三：
       输入命令 show master status;       # 找到File 和 Position 的值记录下来；
【从数据库上操作】
步骤四：
       在mysql下找到my.ini文件，在其[mysqld] 下添加从服务器端配置：
       server-id=2    #服务器 id ，不能和主服务器一致
       replicate-do-db=test1    #待同步的数据库
       保存并重启mysql服务
步骤五：
       从服务器上测试：mysql -u slave -p root -h 192.168.0.102，查看能否连接主数据库成功，
       然后，输入exi退出mysql命令行或者关闭该cmd窗口新开一个cmd
步骤六：
       修改对主数据库的连接的参数：
       mysql>change master to master_host='192.168.0.104',master_user='slave',master_password='root', master_log_file='mysql-bin.000001',master_log_pos=107;（
       这里输入的mysql-bin.000001和107请替换为**步骤三**中查询出来的值 否则不会同步的）  
       ps：这里可能还会报一个错误，大概的意思是说slave线程正在运行，不能设置，这样的话，执行mysql>stop slave; 停止slave线程，然后再设置连接的参数；
步骤七：
      设置完之后，执行 mysql> start slave; 开启slave线程；执行mysql> show slave status\G   (没有分号)，查看
      Slave_IO_Running: Yes
      Slave_SQL_Running: Yes
       以上是详细的步骤，按照这个步骤只要两个电脑网络互通，应该不会有问题的。不过需要注意的是： 如果对从数据库进行修改，是不会同步对主数据库进行修改的，这样就应该用主-主同步了，方法类似。所以insert,delete update 都应该是对主数据库的操作， 而select操作则对从表进行操作。
