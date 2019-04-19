# Ubuntu 上更改 MySQL 数据库数据存储目录 - 文章 - 伯乐在线
原文出处： [潇湘隐者](https://www.cnblogs.com/kerrycode/p/9795474.html)
之前写过一篇博客“[MySQL更改数据库数据存储目录](http://blog.jobbole.com/114479/)”，当时的测试环境是RHEL和CentOS，谁想最近在Ubuntu下面更改MySQL数据库数据存储目录时遇到了之前未遇到的问题，之前的经验用不上了（或者说之前的总结不是太全面），修改完MySQL数据库数据存储目录后重启MySQL，发现MySQL服务无法启动。


```
root@mylnx2:/etc/mysql/mysql.conf.d# service mysql start
 
Job for mysql.service failed because the control process exited with error code. See "systemctl status mysql.service" and "journalctl -xe" for details.
```
连MySQL的错误日志也未生成，使用service mysql status命令可以输出一些较详细的信息，如下所示：


```
root@mylnx2:/etc/mysql/mysql.conf.d# service mysql status
● mysql.service - MySQL Community Server
   Loaded: loaded (/lib/systemd/system/mysql.service; enabled; vendor preset: enabled)
   Active: activating (start-post) (Result: exit-code) since Mon 2018-10-15 22:33:00 CST; 28s ago
  Process: 12947 ExecStart=/usr/sbin/mysqld (code=exited, status=1/FAILURE)
  Process: 12932 ExecStartPre=/usr/share/mysql/mysql-systemd-start pre (code=exited, status=0/SUCCESS)
 Main PID: 12947 (code=exited, status=1/FAILURE);         : 12948 (mysql-systemd-s)
    Tasks: 2
   Memory: 1.9M
      CPU: 367ms
   CGroup: /system.slice/mysql.service
           └─control
             ├─12948 /bin/bash /usr/share/mysql/mysql-systemd-start post
             └─13045 sleep 1
 
Oct 15 22:33:00 mylnx2 systemd[1]: Starting MySQL Community Server...
Oct 15 22:33:01 mylnx2 mysqld[12947]: 2018-10-15T14:33:01.013763Z 0 [Warning] Changed limits: max_open_files: 1024 (requested 5000)
Oct 15 22:33:01 mylnx2 mysqld[12947]: 2018-10-15T14:33:01.013836Z 0 [Warning] Changed limits: table_open_cache: 431 (requested 2000)
Oct 15 22:33:01 mylnx2 mysqld[12947]: 2018-10-15T14:33:01.207533Z 0 [Warning] TIMESTAMP with implicit DEFAULT value is deprecated. Please use --explicit
Oct 15 22:33:01 mylnx2 mysqld[12947]: 2018-10-15T14:33:01.207663Z 0 [Warning] Can't create test file /mysql_data/mysql/mylnx2.lower-test
Oct 15 22:33:01 mylnx2 mysqld[12947]: 2018-10-15T14:33:01.207717Z 0 [Note] /usr/sbin/mysqld (mysqld 5.7.23-0ubuntu0.16.04.1-log) starting as process 129
Oct 15 22:33:01 mylnx2 mysqld[12947]: 2018-10-15T14:33:01.215413Z 0 [Warning] Can't create test file /mysql_data/mysql/mylnx2.lower-test
Oct 15 22:33:01 mylnx2 mysqld[12947]: 2018-10-15T14:33:01.215445Z 0 [Warning] Can't create test file /mysql_data/mysql/mylnx2.lower-test
Oct 15 22:33:01 mylnx2 systemd[1]: mysql.service: Main process exited, code=exited, status=1/FAILURE
```

错误信息为“**[Warning] Can’t create test file /mysql_data/mysql/mylnx2.lower-test**”。其实这里是踩到了“AppArmor”这个坑，之前对Ubuntu了解不多，所以直到遇到这个问题，才了解、知道这么个概念。下面是百科对AppArmor的介绍：
AppArmor是一个高效和易于使用的Linux系统安全应用程序。AppArmor对操作系统和应用程序所受到的威胁进行从内到外的保护，甚至是未被发现的0day漏洞和未知的应用程序漏洞所导致的攻击。AppArmor安全策略可以完全定义个别应用程序可以访问的系统资源与各自的特权。AppArmor包含大量的默认策略，它将先进的静态分析和基于学习的工具结合起来，AppArmor甚至可以使非常复杂的应用可以使用在很短的时间内应用成功。
AppArmor对MySQL所能使用的目录权限做了限制，如下截图所示，规定了MySQL使用的数据文件路径权限。
# cat /etc/apparmor.d/usr.sbin.mysqld
![clip_image001](http://jbcdn2.b0.upaiyun.com/2018/11/c52fe66b891ce5d266ba7650b63b04c9.png)
我将MySQL的数据库数据存储目录从/var/lib/mysql 切换到/mysql_data/mysql下面。所以就遇到了上面错误，需要修改或新增两条记录，从而使mysqld可以使用/mysql_data/mysql这个目录
/mysql_data/mysql/ r,
/mysql_data/mysql/** rwk,
然后重启AppArmor服务后，然后就可以重启MySQL服务了。
sudo service apparmor restart
当然/etc/apparmor.d/usr.sbin.mysqld还有Allow plugin access需要调整，这个不是重点，在此略过。
犹豫了一会，还是记录一下这个小小案例！虽然网上已有不少人总结这个，但是自己动手总结一下，印象也深刻一点！
