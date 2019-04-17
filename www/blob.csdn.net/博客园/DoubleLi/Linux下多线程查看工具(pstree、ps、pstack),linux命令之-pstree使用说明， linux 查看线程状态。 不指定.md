# Linux下多线程查看工具(pstree、ps、pstack),linux命令之-pstree使用说明， linux 查看线程状态。  不指定 - DoubleLi - 博客园






0.最常用 pstree：
[root@iZ25dcp92ckZ temp]# pstree -a|grep multe
  |       |   `-multepoolser
  |       |       `-multepoolser
  |       |           `-2*[{multepoolser}]

1. > top

可以显示所有系统进程

按u, 再输入相应的执行用户名称，比如Tom

可以看到Tom用户启动的所有进程和对应的pid

2. > pstack pid

可以看到此pid下，各线程的运行状态、

[root@test multepoolserver]# pstack  14944  （进程的PID号）
Thread 2 (Thread 0x41ed5940 (LWP 14945)):
#0  0x0000003c9ae0d5cb in read () from /lib64/libpthread.so.0
#1  0x00000000004017b6 in sync_additional_writing_worker ()
#2  0x0000003c9ae064a7 in start_thread () from /lib64/libpthread.so.0
#3  0x0000003c9a2d3c2d in clone () from /lib64/libc.so.6
Thread 1 (Thread 0x2b24b3094250 (LWP 14944)):
#0  0x0000003c9a2d4018 in epoll_wait () from /lib64/libc.so.6
#1  0x0000000000401d59 in Process ()
#2  0x00000000004029b8 in main ()

来自：http://blog.csdn.net/wind_324/article/details/6152912

方法一：
ps -ef f 
用树形显示进程和线程
在Linux下面好像因为没有真正的线程，是用进程模拟的，有一个是辅助线程，所以真正程序开的线程应该只有一个。

方法二：
[root@apache dhj]# ps axm|grep httpd

方法三：
另外用pstree -c也可以达到相同的效果，但是没有线程号：
[root@apache dhj]# pstree -c|grep httpd

来自：http://blog.chinaunix.net/uid-346158-id-2131012.html

1. pstree

pstree以树结构显示进程

- root@119.10.6.*:~# pstree  
- init─┬─NetworkManager  
-      ├─abrt-dump-oops  
-      ├─abrtd  
-      ├─atd  
-      ├─auditd───{auditd}  
-      ├─automount───4*[{automount}]  
-      ├─certmonger  
-      ├─crond  
-      ├─dbus-daemon  
-      ├─hald─┬─hald-runner─┬─hald-addon-acpi  
-      │      │             └─hald-addon-inpu  
-      │      └─{hald}  
-      ├─httpd─┬─httpd  
-      │       └─4*[httpd───26*[{httpd}]]  
-      ├─irqbalance  
-      ├─mcelog  
-      ├─6*[mingetty]  
-      ├─modem-manager  
-      ├─mysqld_safe───mysqld───38*[{mysqld}]  
-      ├─nginx───13*[nginx]  
-      ├─php-fpm───76*[php-fpm]  
-      ├─portreserve  
-      ├─rpc.idmapd  
-      ├─rpc.statd  
-      ├─rpcbind  
-      ├─rsyslogd───3*[{rsyslogd}]  
-      ├─2*[sendmail]  
-      ├─sshd───sshd───bash───pstree  
-      ├─udevd───2*[udevd]  
-      ├─wpa_supplicant  
-      └─xinetd  




2. ps -Lf
$ ps -Lf 1892
如下：PHP进程共启动了0个线程
root@119.10.6.**:~# ps -Lf 1892
UID        PID  PPID   LWP  C NLWP STIME TTY      STAT   TIME CMD
root      1892     1  1892  0    1 Jan15 ?        Ss     0:53 php-fpm: master process (/usr/local/php/etc/php-fpm.conf)

Mysql多个线程：
root@119.10.6.*:~#  ps -Lf 3005
UID        PID  PPID   LWP  C NLWP STIME TTY      STAT   TIME CMD
mysql     3005  2011  3005  0   39 Jan15 ?        Sl     2:25 /usr/local/mysql/bin/mysqld --basedir=/usr/local/mysql --datadir=/data/mysql --plugin-dir=
mysql     3005  2011  3081  0   39 Jan15 ?        Sl     0:00 /usr/local/mysql/bin/mysqld --basedir=/usr/local/mysql --datadir=/data/mysql --plugin-dir=
mysql     3005  2011  3082  0   39 Jan15 ?        Sl     0:05 /usr/local/mysql/bin/mysqld --basedir=/usr/local/mysql --datadir=/data/mysql --plugin-dir=
mysql     3005  2011  3083  0   39 Jan15 ?        Sl     0:00 /usr/local/mysql/bin/mysqld --basedir=/usr/local/mysql --datadir=/data/mysql --plugin-dir=
mysql     3005  2011  3084  0   39 Jan15 ?        Sl     0:00 /usr/local/mysql/bin/mysqld --basedir=/usr/local/mysql --datadir=/data/mysql --plugin-dir=
mysql     3005  2011  3085  0   39 Jan15 ?        Sl     0:00 /usr/local/mysql/bin/mysqld --basedir=/usr/local/mysql --datadir=/data/mysql --plugin-dir=
mysql     3005  2011  3086  0   39 Jan15 ?        Sl     0:00 /usr/local/mysql/bin/mysqld --basedir=/usr/local/mysql --datadir=/data/mysql --plugin-dir=
mysql     3005  2011  3087  0   39 Jan15 ?        Sl     0:05 /usr/local/mysql/bin/mysqld --basedir=/usr/local/mysql --datadir=/data/mysql --plugin-dir=

3. pstack
pstack显示每个进程的栈跟踪,PHP的fpm主进程，发现这玩意是走的epoll：
root@119.10.6.23:~# pstack 1892
#0  0x00000030e9ae8fb3 in __epoll_wait_nocancel () from /lib64/libc.so.6
#1  0x0000000000856a74 in fpm_event_epoll_wait ()
#2  0x000000000084afff in fpm_event_loop ()
#3  0x0000000000845ee7 in fpm_run ()
#4  0x000000000084d900 in main ()
You have new mail in /var/spool/mail/root

**作者：[justwinit](http://www.justwinit.cn/user/1/)@[向东博客 专注WEB应用 构架之美 --- 构架之美，在于尽态极妍 | 应用之美，在于药到病除](http://www.justwinit.cn/)地址：[http://www.justwinit.cn/post/7737/](http://www.justwinit.cn/post/7737/)版权所有。转载时必须以链接形式注明作者和原始出处及本声明！**









