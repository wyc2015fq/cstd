# 关于mysql服务器3306端口不能远程连接的解决 - 专注于数据挖掘算法研究和应用 - CSDN博客





2017年06月05日 11:48:49[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：23337








                
问题：本机可以通过3306端口连接mysql服务器，但远程不行。



1、网络检测

   1)ping主机可以；

   2)telnet 主机3306端口不可以；

     telnet 主机22端口可以；

   说明与本机网络没有关系；



2、端口检测

   1)netstat -ntpl |grep 3306

    tcp        0      0 :::3306                     :::*                        LISTEN      - 

   2)netstat -ntpl |grep 22

    tcp        0      0 0.0.0.0:22                  0.0.0.0:*                   LISTEN      -   

   可以看出22端口监听所有地址，而3306只监听本机地址（绑定了到了本地），所以远程无法访问。

    对于端口只允许本机访问，有两个地方启用，一个是防火墙启用3306，一个就是mysql配置绑定本机地址。



3、防火墙检测

   1)iptables --list查看；

   2)开启防火墙3306端口

     vi /etc/sysconfig/iptables

     -A INPUT -m state –state NEW -m tcp -p tcp –dport 3306 -j ACCEPT（允许3306端口通过防火墙） 

     /etc/init.d/iptables restart（重启防火墙使配置生效）

   3)或者直接关闭防火墙；



   参考：http://blog.csdn.net/fjssharpsword/article/details/50973283



4、mysql配置文件检查

   检查my.cnf的配置，bind-address=addr可以配置绑定ip地址。

   不配置或者IP配置为0.0.0.0，表示监听所有客户端连接。

   #ps -aux | grep mysql  查看进程ID是3340

   #ll /proc/3340 查看进程程序情况，找配置文件

   或者#which mysql 找程序路径



5、mysql用户访问权限

   进入mysql数据库

   $mysql -u root -p

   >use mysql;

   >select host,user from user;

    MySQL建用户的时候会指定一个host，默认是127.0.0.1/localhost只能本机访问；

    其它机器用这个用户帐号访问会提示没有权限，host改为%，表示允许所有机器访问。






