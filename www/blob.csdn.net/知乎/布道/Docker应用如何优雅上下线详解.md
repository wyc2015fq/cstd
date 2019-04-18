# Docker应用如何优雅上下线详解 - 知乎
# 


Docker stop和Docker kill分别实现了优雅退出和强行退出两个操作：
Docker stop：向容器内1号进程，发送SIGTERM信号，在10S之后（可通过参数指定）再发送SIGKILL信号。
Docker kill：直接发送SIGKILL信号。
显然Docker已经考虑到应用优雅退出的问题，但在实际使用中，会遇到下面2个困难：
1.  只有1号进程才收到SIGTERM信号，但Docker中有很多1号进程为monitor或者为初始化脚本的进程，并不是工作进程，且1号进程不能处理SIGTERM信号，以mariadb为例，容器内的进程关系如下：
1 /bin/sh /usr/bin/mysqld_safe --wsrep-cluster-address=gcomm://
2 /usr/sbin/mysqld --basedir=/usr --datadir=/var/lib/mysql/ ...
其中2号进程为1号进程的子进程，虽然2号进程可以处理SIGTERM信号但其收不到该信号，而1号进程虽然能收到SIGTERM但并不能处理此信号。
2. 即使1号进程能处理SIGTERM信号，但若其有子进程为外部命令（非build in命令），且子进程为前台阻塞状态，那么1号进程在直到子进程退出前仍然不能收到SIGTERM信号。如下bash是不会处理SIGTERM信号的：
#！/bin/bash
trap 'exit 0' SIGTERM
sleep 10000
关于这一点需要了解进程处理信号的限制：只有当进程阻塞在内建命令时才可以响应SIG信号，否则会一直等待子进程退出后再处理，如上面的bash，要等到10000秒之后才能处理SIGTERM。关于内建命令和外部命令，描述如下：
内部命令实际上是shell程序的一部分，shell不需要创建子进程，比如：exit，history，cd，echo，wait，trap等，linux系统加载运行时shell就被加载并驻留在系统内存中，一般不会fork子进程。
外部命令是linux系统中的实用程序部分，需要时才将其调用内存。一般会fork出子进程。        
用type命令可以分辨内部命令与外部命令。
综上所述，对于多进程Docker，我建议在容器中使用自定义bash脚本作为容器入口，脚本中使用后台方式执行具体应用的命令，然后使用内建wait阻塞，并通过trap指令监听SIGTERM，执行应用退出操作，下面以容器化mariadb为例，描述其脚本的大概实现：
#!/bin/bash
trap 'mysqladmin -uroot -p123456 shutdown' SIGTERM
mysqld_safe --wsrep-cluster-address=gcomm://10.158.113.207,10.158.113.80,10.158.113.79 &
wait $!
以上述脚本为入口的maraidb容器内进程关系如下：
mysql        1  0.0  0.0  [11628  1352](tel:11628%201352) ?        Ss+  07:59   0:00 /bin/bash /usr/bin/test.sh
mysql        9  0.0  0.0  [11764  1636](tel:11764%201636) ?        S+   07:59   0:00 /bin/sh /usr/bin/mysqld_safe --wsrep-cluster-address=gcomm://10.158.113.207,10.158.113.80,10
mysql      188  1.0  3.7 [1087368](tel:1087368) 300168 ?      Sl+  07:59   0:16  \_ /usr/sbin/mysqld --basedir=/usr --datadir=/var/lib/mysql --plugin-dir=/usr/lib64/mysql/p
当执行docker stop ***的时候，该容器会自动调用mysqladmin shutdown优雅退出。

