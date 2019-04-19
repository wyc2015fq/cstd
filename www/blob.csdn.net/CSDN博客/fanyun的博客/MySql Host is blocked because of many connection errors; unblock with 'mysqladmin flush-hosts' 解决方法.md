# MySql Host is blocked because of many connection errors; unblock with 'mysqladmin flush-hosts' 解决方法 - fanyun的博客 - CSDN博客
2016年03月18日 14:01:40[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：8892
所属专栏：[数据库](https://blog.csdn.net/column/details/database-01.html)
**    原因：**同一个ip在短时间内产生太多（超过mysql数据库max_connect_errors的最大值）中断的数据库连接而导致的阻塞；
   解决方法：
   1、提高允许的max_connection_errors数量：
　　① 进入Mysql数据库查看max_connection_errors： show variables like '%max_connect_errors%'; 
　   ② 修改max_connection_errors的数量为1000： set global max_connect_errors = 1000; 
　　③ 查看是否修改成功：show variables like '%max_connect_errors%'; 
   2、使用mysqladmin flush-hosts 命令清理一下hosts文件（不知道mysqladmin在哪个目录下可以使用命令查找：whereis mysqladmin）；
　　① 在查找到的目录下使用命令修改：mysqladmin --socket=/tmp/kkimdb.sock --port=3306 -uhyman -p flush-hosts
　　备注： 配置有master/slave主从数据库的要把主库和从库都修改一遍的（我就吃了这个亏明明很容易的几条命令结果折腾了大半天）； 
      mysql> flush hosts; 也可以
      max_connect_errors是一个MySQL中与安全有关的计数器值，它负责阻止过多尝试失败的客户端以防止暴力破解密码的情况。max_connect_errors的值与性能并无太大关系。
      默认情况下，my.cnf文件中可能没有此行，如果需要设置此数值，手动添加即可。
[root@www ~]# vi /etc/my.cnf
max_connect_errors = 1000
      配置说明
      当此值设置为10时，意味着如果某一客户端尝试连接此MySQL服务器，但是失败（如密码错误等等）10次，则MySQL会无条件强制阻止此客户端连接。
如果希望重置此计数器的值，则必须重启MySQL服务器或者执行 mysql> flush hosts; 命令。 
      当这一客户端成功连接一次MySQL服务器后，针对此客户端的max_connect_errors会清零。
影响与错误形式
     如果max_connect_errors的设置过小，则网页可能提示无法连接数据库服务器；
而通过SSH的mysql命令连接数据库，则会返回 ERROR 1129 (00000): Host 'gateway' is blocked because of many connection errors; unblock with 'mysqladmin flush-hosts' 错误。 
     功能与作用
     一般来说建议数据库服务器不监听来自网络的连接，仅仅通过sock连接，这样可以防止绝大多数针对mysql的攻击；如果必须要开启mysql的网络连接，则最好设置此值，以防止穷举密码的攻击手段。
