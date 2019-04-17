# linux中service *** start与直接运行/usr/bin/***的区别 - DoubleLi - 博客园






在linux想要运行启动一个服务有两种方法：

1，运行/etc/init.d/目录下的shell脚本，还可以有快捷方式，service *** start/ stop/restart /status，

 2，直接运行/usr/bin目录下的服务文件；



第一种方法启动的[程序](http://www.xuebuyuan.com/)可以通过service命令来管理，比如说查看状态 service --status-all

查看/etc/init.d目录，发现下面都是shell脚本，脚本里面运行的程序最终还是指向了/usr/bin下面的程序，只不过在shell脚本中可能直接对程序的一些运行参数继续进行了设置；



第二种方法，直接运行/usr/bin/****,如果运行时有配置文件的话，需要加上 --config *****，用这种方法启动程序后，不能用service命令查看状态；



举例：/etc/init.d/openstack-nova-api


suffix=api
prog=openstack-nova-$suffix
exec="/usr/bin/nova-$suffix"
config="/etc/nova/nova.conf"
pidfile="/var/run/nova/nova-$suffix.pid"
logfile="/var/log/nova/$suffix.log"


[ -e /etc/sysconfig/$prog ] && . /etc/sysconfig/$prog


lockfile=/var/lock/subsys/$prog


start() {
    [ -x $exec ] || exit 5
    [ -f $config ] || exit 6
    echo -n $"Starting $prog: "
    daemon --user nova --pidfile $pidfile "$exec --config-file $config --logfile $logfile &>/dev/null & echo \$! > $pidfile"
    retval=$?
    echo
    [ $retval -eq 0 ] && touch $lockfile
    return $retval
}


stop() {
    echo -n $"Stopping $prog: "
    killproc -p $pidfile $prog
    retval=$?
    echo
    [ $retval -eq 0 ] && rm -f $lockfile
    return $retval
}


restart() {
    stop
    start
}


reload() {
    restart
}


force_reload() {
    restart




然后用 ps aux |grep /usr/bin/nova-api来查看一下当前进程：

/usr/bin/python /usr/bin/nova-api --config-file /etc/nova/nova.conf --logfile /var/log/nova/api.log,



因此二者的等价关系为：

service openstack-nova-api start ====  /usr/bin/python /usr/bin/nova-api --config-file /etc/nova/nova.conf --logfile /var/log/nova/api.log,



[编写属于自己的Linux
Service命令](http://www.cnblogs.com/caoyuanzhanlang/archive/2012/12/03/2799657.html)






最近在玩Liunx，发现linux启动服务是用SERVICE + COMMAND，觉得很有意思，因为command命令本身也是一个脚本啊，比如说：service network restart；好奇心就来了，怎么能够做一个属于自己的service命令呢。

其实很简单，只要我们认识service这个命令的原理就行了。如果要知道service这个命令怎么用，你就自己man service下，在这里，我简要说下service的运行过程。举个例子来说：service network restart。

首先呢，sevice 会去寻找/etc/init.d下的network脚本，restart是network脚本里的一个参数（你可以去查看network这个脚本支持的参数），然后告诉系统运行network这个脚本，剩下的事情就交给network脚本去坐了，事实就是这么简单。

现在，你应该知道怎么编写属于自己的service命令了吧，编写一个脚本，然后把它放在/etc/init.d这个目录底下，你就可以用service +脚本名字 运行它。如果是要开机自动启动那就得用chkconfig命令了。

注意：

A、service这个命令往往是即时生效，不用开关机，但是重启后服务会回到默认状态。

B、chkconfig是用于把服务加到开机自动启动列表里，只要启动它，就能自动启动，重启后永久生效

**即：chkconfig --add COMMAND **

**      chkconfig COMMAND on/off    重启后永久生效**












