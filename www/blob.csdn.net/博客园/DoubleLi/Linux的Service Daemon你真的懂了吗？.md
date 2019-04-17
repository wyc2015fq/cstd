# Linux的Service/Daemon你真的懂了吗？ - DoubleLi - 博客园






一 service与一般的程序的区别

service（也称为daemon）表示后台运行的程序，一般随系统的启动自动地启动且在用户logoff后仍然能够继续运行。该daemon进程一般在启动后需要与父进程断开关系，并使进程没有控制终端（tty）。因为daemon程序在后台执行，不需要于终端交互，通常就关闭STDIN、STDOUT和STDER。daemon无法输出信息，可以使用syslog或自己的日志系统进行日志处理。



可以使用/etc/rc.d/init.d/functions脚本中的daemon函数来将一般的程序启动为daemon，例如nfs daemon的启动：

echo -n $"Starting NFS daemon: "
daemon rpc.nfsd $RPCNFSDARGS $RPCNFSDCOUNT
ETVAL=$?
echo

[ $RETVAL -ne 0 ] && exit $RETVAL





二 跟 service相关的目录和文件

/etc/init.d -> /etc/rc.d/init.d : /etc/rc.d/init.d 下包含了所有的service的控制脚本，/etc/init.d 是 /etc/rc.d/init.d 的链接。

/etc/rc*.d -> /etc/rc.d/rc*.d ： /etc/rc.d/rc*.d 下包含了所有的service的自动启动选项的配置，/etc/rc*.d 是 /etc/rc.d/rc*.d 的链接。

/etc/rc.d/rc*.d/ 下的脚本是对 /etc/rc.d/init.d 下的脚本的链接，只是名字增加K或S前缀，用来表示自动启动或自动停止。



所有的service相关的目录树结构如下：

/etc
|-- init.d -> rc.d/init.d
|-- rc -> rc.d/rc
|-- rc.d
| |-- init.d
| | |-- NetworkManager
| | |-- network
| | |-- nfs
| | |-- xinetd
| | |-- ... ...
| |-- rc
| |-- rc.local
| |-- rc.sysinit
| |-- rc3.d
| | |-- K02NetworkManager -> ../init.d/NetworkManager
| | |-- K20nfs -> ../init.d/nfs
| | |-- S10network -> ../init.d/network
| | |-- S56xinetd -> ../init.d/xinetd
| | |-- ... ...
|-- rc.local -> rc.d/rc.local
|-- rc.news
|-- rc.sysinit -> rc.d/rc.sysinit
|-- rc0.d -> rc.d/rc0.d
|-- rc1.d -> rc.d/rc1.d
|-- rc2.d -> rc.d/rc2.d
|-- rc3.d -> rc.d/rc3.d
|-- rc4.d -> rc.d/rc4.d
|-- rc5.d -> rc.d/rc5.d
|-- rc6.d -> rc.d/rc6.d





三 一个service的组成和部署

一个service通常包含一个可执行的文件和一个service控制脚本。 作为service程序本身的可执行程序一般存储在/usr/bin下；作为控制service的脚本一般存储在/etc/rc.d/init.d下，且控制service的脚本的格式相对固定，至少支持start，stop，status参数。 例如smb的service控制脚本为：

#!/bin/sh
#
# chkconfig: - 91 35
# description: Starts and stops the Samba smbd and nmbd daemons \
# used to provide SMB network services.
#
# pidfile: /var/run/samba/smbd.pid
# pidfile: /var/run/samba/nmbd.pid
# config: /etc/samba/smb.conf


# Source function library.
if [ -f /etc/init.d/functions ] ; then
. /etc/init.d/functions
elif [ -f /etc/rc.d/init.d/functions ] ; then
. /etc/rc.d/init.d/functions
else
exit 1
fi

# Avoid using root's TMPDIR
unset TMPDIR

# Source networking configuration.
. /etc/sysconfig/network

if [ -f /etc/sysconfig/samba ]; then
. /etc/sysconfig/samba
fi

# Check that networking is up.
[ ${NETWORKING} = "no" ] && exit 1

# Check that smb.conf exists.
[ -f /etc/samba/smb.conf ] || exit 6

RETVAL=0


start() {
KIND="SMB"
echo -n $"Starting $KIND services: "
daemon smbd $SMBDOPTIONS
RETVAL=$?
echo
KIND="NMB"
echo -n $"Starting $KIND services: "
daemon nmbd $NMBDOPTIONS
RETVAL2=$?
echo
[ $RETVAL -eq 0 -a $RETVAL2 -eq 0 ] && touch /var/lock/subsys/smb || \
RETVAL=1
return $RETVAL
}

stop() {
KIND="SMB"
echo -n $"Shutting down $KIND services: "
killproc smbd
RETVAL=$?
echo
KIND="NMB"
echo -n $"Shutting down $KIND services: "
killproc nmbd
RETVAL2=$?
[ $RETVAL -eq 0 -a $RETVAL2 -eq 0 ] && rm -f /var/lock/subsys/smb
echo ""
return $RETVAL
}

restart() {
stop
start
}

reload() {
echo -n $"Reloading smb.conf file: "
killproc smbd -HUP
RETVAL=$?
echo
return $RETVAL
}

rhstatus() {
status smbd
RETVAL=$?
status nmbd
RETVAL2=$?
if [ $RETVAL -ne 0 ] ; then
return $RETVAL
fi
if [ $RETVAL2 -ne 0 ] ; then
return $RETVAL2
fi
}


# Allow status as non-root.
if [ "$1" = status ]; then
rhstatus
exit $?
fi

# Check that we can write to it... so non-root users stop here
[ -w /etc/samba/smb.conf ] || exit 4



case "$1" in
start)
start
;;
stop)
stop
;;
restart)
restart
;;
reload)
reload
;;
status)
rhstatus
;;
condrestart)
[ -f /var/lock/subsys/smb ] && restart || :
;;
*)
echo $"Usage: $0 {start|stop|restart|reload|status|condrestart}"
exit 2
esac

exit $?



四 service的管理工具/sbin/service

可以使用工具/sbin/service来查看和管理系统上的所有的service，例如查看所有的service的状态使用/sbin/service --status-all, 或者启动停止或重启某个service为/sbin/service XXX start/stop/restat。



打开/sbin/service脚本，我们可以看到其实service命令实现很简单，就是遍历/etc/init.d （-> /etc/rc.d/init.d）下的所有的service控制脚本，执行相应service控制脚本中的操作。



你可以使用命令cat /sbin/service 来查看/sbin/service的实现。





五 service的自动启动控制工具/sbin/chkconfig

当你实现你的service且部署到/etc/init.d下后，你的脚本已经可以手动地启动和关闭，但是要实现在系统启动的时候你的service自动地启动，还需要做简单的配置。我们知道/etc/rc.d/rc*.d下的脚本是对/etc/rc.d/init.d下的service控制脚本的一个连接，用来控制service的自动启动选项的。例如在/etc/rc.d/rc3.d下lrwxrwxrwx 1 root root 16 2009-05-20 S12syslog -> ../init.d/syslog ，此连接文件以S开头表示了在runlevel为3下syslog service将会与系统一起自动地启动。



手动地在/etc/rc.d/rc*.d下创建连接文件来配置service是否自动地启动，实在是有点麻烦，我们可以使用工具/sbin/chkconfig来简化service启动选项的配置。/sbin/chkconfig工具要求service的控制脚本的前几行必须包含如下的类似信息： 

#
# chkconfig: - 91 35
# description: Starts and stops the Samba smbd and nmbd daemons \
# used to provide SMB network services.
#





使用chkconfig的实例如下：

chkconfig --list 用来查看所有的service的启动选项的配置；

chkconfig network 用来查看network service的启动选项的配置；





更多查看：[6个Linux chkconfig命令实例 - 增加，删除，查看和修改services的自动启动选项](http://www.cnblogs.com/itech/archive/2011/08/22/2149018.html)





六xinetd

xinetd本身是一个service，他的作用是监听所有的端口，根据配置对不同的端口启动不同的应用。 对于有些需要在后台运行的程序，可以选择设置为service在后台一直运行，也可以选择使用xinetd来配置此程序根据需要激活。



对于需要频繁访问的服务，需要在/etc/rc.d/init.d下配置为service；

对于不是频繁访问的服务，可以使用xinetd来激活，从而节约服务器的资源；

总之service与xinetd，选一即可。 

[](http://www.cnblogs.com/itech/archive/2011/01/17/1937447.html)

七 参考：[在Linux下编写Daemon](http://www.cnblogs.com/itech/archive/2011/01/17/1937447.html)


完！











