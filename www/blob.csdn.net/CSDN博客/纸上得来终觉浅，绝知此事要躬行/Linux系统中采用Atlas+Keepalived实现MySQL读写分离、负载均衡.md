# Linux系统中采用Atlas+Keepalived实现MySQL读写分离、负载均衡 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2016年05月31日 16:47:29[boonya](https://me.csdn.net/boonya)阅读数：925









==========================================================================================

**一、基础介绍**

==========================================================================================

**1、背景描述**

目前我们的高可用DB的代理层采用的是360开源的Atlas，从上线以来，已稳定运行2个多月。无论是从性能上，还是稳定性上，相比其他开源组件（amoeba、cobar、MaxScale、MySQL-Proxy等），还是很出色的。




当初我们之所以选择Atlas，主要看中它有以下优点：

(1)、基于mysql-proxy-0.8.2进行修改，代码完全开源；

(2)、比较轻量级，部署配置也比较简单；

(3)、支持DB读写分离；

(4)、支持从DB读负载均衡，并自动剔除故障从DB；

(5)、支持平滑上下线DB；

(6)、具备较好的安全机制（IP过滤、账号认证）；

(7)、版本更新、问题跟进、交流圈子都比较活跃。




在测试期间以及线上问题排查过程中，得到了360 Atlas作者朱超的热心解答，在此表示感谢。有关更多Atlas的介绍，我就不一一例举，可以参考以下链接：

[https://github.com/Qihoo360/Atlas/blob/master/README_ZH.md](https://github.com/Qihoo360/Atlas/blob/master/README_ZH.md)




**2、总体架构图**

![wKioL1Sw6iagbaHjAAJX6OZk-GM940.jpg](http://s3.51cto.com/wyfs02/M00/58/72/wKioL1Sw6iagbaHjAAJX6OZk-GM940.jpg)




**3、系统环境**

CentOS 6.3 x86_64




==========================================================================================

**二、安装部署**

==========================================================================================

**1、需注意的地方**

(1)、本次安装不使用系统默认的glib库，之前的yum安装只是为了先解决依赖库的问题；

(2)、LUA库的版本不能太高，为5.1.x即可；

(3)、glib库的版本也不能太高，为glib-2.32.x即可；

(4)、对于编译不成功的情况，注意查看下面的说明。




**2、GLIB依赖的基础库安装**

# yum -y install *glib*




**3、LUA库安装**

[http://ftp.gnu.org/pub/gnu/ncurses/ncurses-5.9.tar.gz](http://ftp.gnu.org/pub/gnu/ncurses/ncurses-5.9.tar.gz)

# tar xvzf ncurses-5.9.tar.gz

# cd ncurses-5.9

# ./configure --prefix=/usr/local

# make && make install




[ftp://ftp.gnu.org/gnu/readline/readline-6.2.tar.gz](ftp://ftp.gnu.org/gnu/readline/readline-6.2.tar.gz)

# tar xvzf readline-6.2.tar.gz

# cd readline-6.2

# ./configure --prefix=/usr/local

# make && make install




[http://www.lua.org/ftp/lua-5.1.5.tar.gz](http://www.lua.org/ftp/lua-5.1.5.tar.gz)

# tar xvzf lua-5.1.5.tar.gz

# cd lua-5.1.5

# make linux install




**注意：**

修改当前目录下的“Makefile”中的INSTALL_TOP=
 /usr/local为INSTALL_TOP= /usr/local/lua

主要是为了避免与系统自带的lua库发生冲突的可能




在“src/Makefile”文件中加入“-lncurses”，完整内容如下：

linux:

        $(MAKE) $(ALL) SYSCFLAGS="-DLUA_USE_LINUX" SYSLIBS="-Wl,-E -ldl-lncurses -lreadline"




**4、GLIB库安装**

[ftp://sourceware.org/pub/libffi/libffi-3.0.13.tar.gz](ftp://sourceware.org/pub/libffi/libffi-3.0.13.tar.gz)

# tar xvzf libffi-3.0.13.tar.gz

# cd libffi-3.0.13

# ./configure --prefix=/usr/local

# make && make install




[http://ftp.gnu.org/pub/gnu/libiconv/libiconv-1.14.tar.gz](http://ftp.gnu.org/pub/gnu/libiconv/libiconv-1.14.tar.gz)

# tar xvzf libiconv-1.14.tar.gz

# cd libiconv-1.14

# ./configure --prefix=/usr/local

# make && make install




[http://tukaani.org/xz/xz-5.0.5.tar.gz](http://tukaani.org/xz/xz-5.0.5.tar.gz)

# tar xvzf xz-5.0.5.tar.gz

# cd xz-5.0.5

# ./configure --prefix=/usr/local

# make && make install

# /sbin/ldconfig




[http://ftp.gnome.org/pub/gnome/sources/glib/2.32/glib-2.32.4.tar.xz](http://ftp.gnome.org/pub/gnome/sources/glib/2.32/glib-2.32.4.tar.xz)

# xz -d glib-2.32.4.tar.xz

# tar -xvf glib-2.32.4.tar

# cd glib-2.32.4

# ./configure --prefix=/usr/local/glib-2.32.4 \

--with-libiconv=/usr/local \

LIBFFI_CFLAGS="-I/usr/local/include" \

LIBFFI_LIBS="-L/usr/local/lib -lffi"

# make && make install




**注意：编译报错处理**

**(1)、configure阶段**

# vim ./glib/gconvert.c

注释掉第26、28行的内容

注释掉从61行到67行的内容




# vim ./configure

在7880行之上添加如下内容：

found_iconv=yes




**(2)、make阶段**

# ln -s /usr/local/lib/libffi-3.0.13/include/ffi.h /usr/local/include

# ln -s /usr/local/lib/libffi-3.0.13/include/ffitarget.h /usr/local/include




glib库需要安装在单独的目录“/usr/local/glib-2.32.4”，也是为了避免与系统自带的glib库发生冲突的可能




**5、Atlas安装**

**(1)、其他基础组件安装**

[https://github.com/downloads/libevent/libevent/libevent-2.0.21-stable.tar.gz](https://github.com/downloads/libevent/libevent/libevent-2.0.21-stable.tar.gz)

# tar xvzf libevent-2.0.21-stable.tar.gz

# cd libevent-2.0.21-stable

# ./configure --prefix=/usr/local

# make && make install




[http://www.openssl.org/source/openssl-1.0.1h.tar.gz](http://www.openssl.org/source/openssl-1.0.1h.tar.gz)

# tar xvzf openssl-1.0.1h.tar.gz

# cd openssl-1.0.1h

# ./config shared --prefix=/usr/local

# make && make install




**(2)、MySQL安装（无需启动）**

[http://wwwNaNake.org/files/v2.8/cmake-2.8.10.2.tar.gz](http://wwwNaNake.org/files/v2.8/cmake-2.8.10.2.tar.gz)

# tar -xvzf cmake-2.8.10.2.tar.gz

# cd cmake-2.8.10.2

# ./bootstrap --prefix=/usr/local

# gmake --jobs=`grep processor /proc/cpuinfo | wc -l`

# gmake install




[http://downloads.mysql.com/archives/get/file/mysql-5.5.24.tar.gz](http://downloads.mysql.com/archives/get/file/mysql-5.5.24.tar.gz)

# tar -xvzf mysql-5.5.24.tar.gz

# cd mysql-5.5.24

# rm-f CMakeCache.txt

# cmake -DCMAKE_INSTALL_PREFIX=/usr/local/mysql \

-DMYSQL_UNIX_ADDR=/var/run/mysql/mysql.sock \

-DDEFAULT_CHARSET=utf8 \

-DDEFAULT_COLLATION=utf8_general_ci \

-DEXTRA_CHARSETS=all \

-DWITH_MYISAM_STORAGE_ENGINE=1 \

-DWITH_INNOBASE_STORAGE_ENGINE=1 \

-DWITH_READLINE=1 \

-DENABLED_LOCAL_INFILE=1 \

-DWITH_EMBEDDED_SERVER=1 \

-DMYSQL_DATADIR=/data/dbdata/data \

-DMYSQL_TCP_PORT=3306

# make --jobs=`grep processor /proc/cpuinfo | wc -l`

# make install




**(3)、DB中间件安装**

[https://github.com/Qihoo360/Atlas/archive/2.2.1.tar.gz](https://github.com/Qihoo360/Atlas/archive/2.2.1.tar.gz)

# tar xvzf Atlas-2.2.1.tar.gz

# cd Atlas-2.2.1

# ./configure --prefix=/usr/local/mysql-proxy \

--with-lua=/usr/local/lua \

--with-mysql=/usr/local/mysql \

GLIB_CFLAGS="-I/usr/local/glib-2.32.4/include/glib-2.0" \

GLIB_LIBS="-L/usr/local/glib-2.32.4/lib/glib-2.0 -lglib-2.0" \

GMODULE_CFLAGS="-I/usr/local/glib-2.32.4/include" \

GMODULE_LIBS="-L/usr/local/glib-2.32.4/lib -lgmodule-2.0" \

GTHREAD_CFLAGS="-I/usr/local/glib-2.32.4/include" \

GTHREAD_LIBS="-L/usr/local/glib-2.32.4/lib -lgthread-2.0" \

LUA_CFLAGS="-I/usr/local/lua/include" \

LUA_LIBS="-L/usr/local/lua/lib -llua-5.1" \

CFLAGS="-DHAVE_LUA_H -O2" \

LDFLAGS="-L/usr/local/lib -L/usr/local/lib64 -lm -ldl -lcrypto"

# make && make install




**注意：**

编译报错处理

# ln -s /usr/local/glib-2.32.4/lib/glib-2.0/include/glibconfig.h /usr/local/glib-2.32.4/include/glib-2.0




# cd /usr/local

# mv mysql-proxy atlas-2.2.1 && ln -s atlas-2.2.1 mysql-proxy




**6、DB中间层配置**

**(1)、主配置**

# vim /usr/local/mysql-proxy/conf/mysql-proxy.cnf



```
`[mysql-proxy]`

`admin-username = sysadmin`

`admin-password = admin2356!@()`

` `

`proxy-backend-addresses = 10.222.5.224:3306`

`proxy-read-only-backend-addresses = 10.240.95.107:3306,10.240.95.108:3306`

` `

`pwds = health_check1:/iZxz+0GRoA=,health_check2:/iZxz+0GRoA=`

` `

`daemon = true`

`keepalive = true`

` `

`event-threads = 16`

` `

`log-level = message`

`log-path = /usr/local/mysql-proxy/log`

`sql-log = ON`

` `

`proxy-address = 0.0.0.0:3306`

`admin-address = 10.209.6.101:3307`

` `

`charset = utf8`
```



**(2)、启动脚本**

# vim /etc/init.d/mysql-proxy



```
```bash
#!/bin/sh
```



```bash
#
```



```bash
# mysql-proxy This script starts and stops the mysql-proxy daemon
```



```bash
#
```



```bash
# chkconfig: - 78 30
```



```bash
# processname: mysql-proxy
```



```bash
# description: mysql-proxy is a proxy daemon to mysql
```



```bash
# config: /usr/local/mysql-proxy/conf/mysql-proxy.cnf
```



```bash
# pidfile: /usr/local/mysql-proxy/log/mysql-proxy.pid
```



```bash
#
```



```bash
PATH=
```

```bash
/usr/local/sbin
```

```bash
:
```

```bash
/usr/local/bin
```

```bash
:
```

```bash
/sbin
```

```bash
:
```

```bash
/bin
```

```bash
:
```

```bash
/usr/sbin
```

```bash
:
```

```bash
/usr/bin
```



```bash

```



```bash
DAEMON=
```

```bash
"/usr/local/mysql-proxy/bin/mysql-proxy"
```



```bash
CONFIGFILE=
```

```bash
"/usr/local/mysql-proxy/conf/mysql-proxy.cnf"
```



```bash
PIDFILE=
```

```bash
"/usr/local/mysql-proxy/log/mysql-proxy.pid"
```



```bash
LOCKFILE=
```

```bash
"/var/lock/subsys/mysql-proxy"
```



```bash
PROG=`
```

```bash
basename
```

```bash
$DAEMON`
```



```bash

```



```bash
RETVAL=0
```



```bash

```



```bash
start() {
```



```bash

```

```bash
echo
```

```bash
-n $
```

```bash
"Starting ${PROG}......"
```



```bash

```

```bash
[ -x $DAEMON ] ||
```

```bash
exit
```

```bash
5
```



```bash

```

```bash
[ -f $CONFIGFILE ] ||
```

```bash
exit
```

```bash
6
```



```bash

```

```bash
${DAEMON} --defaults-
```

```bash
file
```

```bash
=${CONFIGFILE} ||
```

```bash
echo
```

```bash
-n
```

```bash
"${PROG} already running"
```



```bash

```



```bash

```

```bash
RETVAL=$?
```



```bash

```

```bash
echo
```



```bash

```

```bash
[[ $RETVAL -
```

```bash
eq
```

```bash
0 ]] &&
```

```bash
touch
```

```bash
$LOCKFILE
```



```bash

```



```bash

```

```bash
return
```

```bash
$RETVAL
```



```bash
}
```



```bash

```



```bash
stop() {
```



```bash

```

```bash
echo
```

```bash
-n $
```

```bash
"Stopping ${PROG}......"
```



```bash

```

```bash
if
```

```bash
[[ `
```

```bash
ps
```

```bash
aux |
```

```bash
grep
```

```bash
bin
```

```bash
/mysql-proxy
```

```bash
|
```

```bash
grep
```

```bash
-
```

```bash
v
```

```bash
grep
```

```bash
|
```

```bash
wc
```

```bash
-l` -gt 0 ]];
```

```bash
then
```



```bash

```

```bash
kill
```

```bash
-TERM `
```

```bash
ps
```

```bash
-A -oppid,pid,cmd |
```

```bash
grep
```

```bash
bin
```

```bash
/mysql-proxy
```

```bash
|
```

```bash
grep
```

```bash
-
```

```bash
v
```

```bash
grep
```

```bash
|
```

```bash
awk
```

```bash
'{print $2}'
```

```bash
`
```



```bash

```

```bash
fi
```



```bash

```



```bash

```

```bash
RETVAL=$?
```



```bash

```

```bash
echo
```



```bash

```

```bash
[[ $RETVAL -
```

```bash
eq
```

```bash
0 ]] &&
```

```bash
rm
```

```bash
-f $LOCKFILE $PIDFILE
```



```bash

```



```bash

```

```bash
return
```

```bash
$RETVAL
```



```bash
}
```



```bash

```



```bash
restart() {
```



```bash

```

```bash
stop
```



```bash

```

```bash
sleep
```

```bash
1
```



```bash

```

```bash
start
```



```bash
}
```



```bash

```



```bash
case
```

```bash
"$1"
```

```bash
in
```



```bash
start)
```



```bash

```

```bash
start
```



```bash

```

```bash
;;
```



```bash

```



```bash
stop)
```



```bash

```

```bash
stop
```



```bash

```

```bash
;;
```



```bash

```



```bash
restart)
```



```bash

```

```bash
restart
```



```bash

```

```bash
;;
```



```bash

```



```bash
condrestart)
```



```bash

```

```bash
[[ -e $LOCKFILE ]] && restart
```



```bash

```

```bash
;;
```



```bash

```



```bash
*)
```



```bash

```

```bash
echo
```

```bash
"Usage: $0 {start|stop|restart|condrestart}"
```



```bash

```

```bash
RETVAL=1
```



```bash

```

```bash
;;
```



```bash
esac
```



```bash

```



```bash
exit
```

```bash
$RETVAL
```
```



# chmod +x /etc/init.d/mysql-proxy

# chmod 0660 /usr/local/mysql-proxy/conf/mysql-proxy.cnf

# service mysql-proxy start

# ps aux | grep mysql-prox[y]

![wKiom1Sw7APBGLlPAABNGlf7ct0239.jpg](http://s3.51cto.com/wyfs02/M01/58/75/wKiom1Sw7APBGLlPAABNGlf7ct0239.jpg)




**7、Atlas高可用【Keepalived】环境安装**

[http://rpm5.org/files/popt/popt-1.14.tar.gz](http://rpm5.org/files/popt/popt-1.14.tar.gz)

# tar xvzf popt-1.14.tar.gz

# cd popt-1.14

# ./configure --prefix=/usr/local

# make && make install




[http://www.carisma.slowglass.com/~tgr/libnl/files/libnl-3.2.24.tar.gz](http://www.carisma.slowglass.com/~tgr/libnl/files/libnl-3.2.24.tar.gz)

# tar xvzf libnl-3.2.24.tar.gz

# cd libnl-3.2.24

# ./configure --prefix=/usr/local

# make && make install

# ln -s /usr/local/include/libnl3/netlink /usr/local/include

# /sbin/ldconfig




[http://www.keepalived.org/software/keepalived-1.2.10.tar.gz](http://www.keepalived.org/software/keepalived-1.2.10.tar.gz)

# tar xvzf keepalived-1.2.10.tar.gz

# cd keepalived-1.2.10

# ./configure --prefix=/usr/local/keepalived

# make && make install




# ln -s /usr/local/keepalived/sbin/keepalived /usr/sbin

# ln -s /usr/local/keepalived/etc/sysconfig/keepalived /etc/sysconfig

# ln -s /usr/local/keepalived/etc/rc.d/init.d/keepalived /etc/init.d

# chkconfig --add keepalived




**8、Atlas高可用【Keepalived】配置**

# mkdir –p /etc/keepalived /data/scripts




**(1)、主节点配置**

# vim /etc/keepalived/keepalived.conf



```
`global_defs {`

`    ``notification_email {`

`        ``lovezym5@126.com`

`    ``}`

` `

`    ``notification_email_from lovezym5@126.com`

`    ``smtp_server 127.0.0.1`

`    ``smtp_connect_timeout 30`

`    ``router_id dbproxy1`

`}`

` `

`vrrp_script chk_mysql_proxy_health {`

`    ``script "/data/scripts/keepalived_check_mysql_proxy.sh"`

`    ``interval 1`

`    ``weight -2`

`}`

` `

`vrrp_instance VI_1 {`

`    ``state MASTER`

`    ``interface eth1`

`    ``virtual_router_id 51`

`    ``priority 100`

`    ``advert_int 1`

`    ``smtp_alert`

` `

`    ``authentication {`

`        ``auth_type PASS`

`        ``auth_pass 123456`

`    ``}`

` `

`    ``virtual_ipaddress {`

`        ``10.209.6.115`

`    ``}`

` `

`    ``track_script {`

`        ``chk_mysql_proxy_health`

`    ``}`

` `

`    ``notify_master "/data/scripts/notify.sh master"`

`    ``notify_bakcup "/data/scripts/notify.sh backup"`

`    ``notify_fault "/data/scripts/notify.sh fault"`

`}`
```



**(2)、备用节点配置**

# vim /etc/keepalived/keepalived.conf



```
`global_defs {`

`    ``notification_email {`

`        ``lovezym5@126.com`

`    ``}`

` `

`    ``notification_email_from lovezym5@126.com`

`    ``smtp_server 127.0.0.1`

`    ``smtp_connect_timeout 30`

`    ``router_id dbproxy2`

`}`

` `

`vrrp_script chk_mysql_proxy_health {`

`    ``script "/data/scripts/keepalived_check_mysql_proxy.sh"`

`    ``interval 1`

`    ``weight -2`

`}`

` `

`vrrp_instance VI_1 {`

`    ``state BACKUP`

`    ``interface eth1`

`    ``virtual_router_id 51`

`    ``priority 90`

`    ``advert_int 1`

`    ``smtp_alert`

` `

`    ``authentication {`

`        ``auth_type PASS`

`        ``auth_pass 123456`

`    ``}`

` `

`    ``virtual_ipaddress {`

`        ``10.209.6.115`

`    ``}`

` `

`    ``track_script {`

`        ``chk_mysql_proxy_health`

`    ``}`

` `

`    ``notify_master "/data/scripts/notify.sh master"`

`    ``notify_bakcup "/data/scripts/notify.sh backup"`

`    ``notify_fault "/data/scripts/notify.sh fault"`

`}`
```



**(3)、VIP切换通知脚本**

# vim /data/scripts/notify.sh



```
```bash
#!/bin/sh
```



```bash
PATH=
```

```bash
/sbin
```

```bash
:
```

```bash
/bin
```

```bash
:
```

```bash
/usr/sbin
```

```bash
:
```

```bash
/usr/bin
```

```bash
:
```

```bash
/usr/local/bin
```

```bash
:
```

```bash
/usr/local/sbin
```



```bash

```



```bash
KEEPALIVE_CONF=
```

```bash
"/etc/keepalived/keepalived.conf"
```



```bash

```



```bash
VIP=`
```

```bash
grep
```

```bash
-A 1 virtual_ipaddress ${KEEPALIVE_CONF} |
```

```bash
tail
```

```bash
-1 |
```

```bash
sed
```

```bash
's/\t//g; s/ //g'
```

```bash
`
```



```bash
ETH1_ADDR=`
```

```bash
/sbin/ifconfig
```

```bash
eth1 |
```

```bash
awk
```

```bash
'/inet addr:/{print $2}'
```

```bash
|
```

```bash
awk
```

```bash
-F:
```

```bash
'{print $2}'
```

```bash
`
```



```bash

```



```bash
MONITOR=
```

```bash
"/usr/local/oms/agent/alarm/BusMonitorAgent"
```



```bash
TOKEN=
```

```bash
"ha_monitor"
```



```bash

```



```bash
function
```

```bash
notify() {
```



```bash

```

```bash
TITLE=
```

```bash
"$ETH1_ADDR to be $1: $VIP floating"
```



```bash

```

```bash
CONTENT=
```

```bash
"vrrp transition, $ETH1_ADDR changed to be $1"
```



```bash

```

```bash
${MONITOR} -c 2 -f ${TOKEN} -t
```

```bash
"${TITLE}"
```

```bash
-i
```

```bash
"${CONTENT}"
```



```bash
}
```



```bash

```



```bash
case
```

```bash
"$1"
```

```bash
in
```



```bash
master)
```



```bash

```

```bash
notify master
```



```bash

```

```bash
exit
```

```bash
0
```



```bash

```

```bash
;;
```



```bash

```



```bash
backup)
```



```bash

```

```bash
notify backup
```



```bash

```

```bash
exit
```

```bash
0
```



```bash

```

```bash
;;
```



```bash

```



```bash
fault)
```



```bash

```

```bash
notify fault
```



```bash

```

```bash
exit
```

```bash
0
```



```bash

```

```bash
;;
```



```bash

```



```bash
*)
```



```bash

```

```bash
echo
```

```bash
'Usage: `basename $0` {master|backup|fault}'
```



```bash

```

```bash
exit
```

```bash
1
```



```bash

```

```bash
;;
```



```bash
esac
```
```



**(4)、DB中间层进程检查脚本**

# vim /data/scripts/keepalived_check_mysql_proxy.sh



```
```bash
#!/bin/sh
```



```bash
PATH=
```

```bash
/sbin
```

```bash
:
```

```bash
/bin
```

```bash
:
```

```bash
/usr/sbin
```

```bash
:
```

```bash
/usr/bin
```

```bash
:
```

```bash
/usr/local/bin
```

```bash
:
```

```bash
/usr/local/sbin
```



```bash

```



```bash
if
```

```bash
[[ `pgrep mysql-proxy |
```

```bash
wc
```

```bash
-l` -
```

```bash
eq
```

```bash
0 ]];
```

```bash
then
```



```bash

```

```bash
/sbin/service
```

```bash
mysql-proxy start &&
```

```bash
sleep
```

```bash
5
```



```bash

```

```bash
[[ -z `pgrep mysql-proxy` ]] &&
```

```bash
/sbin/service
```

```bash
keepalived stop
```



```bash
fi
```
```



# chmod +x /data/scripts/*.sh

# service keepalived start

![wKioL1Sw72OBWcdcAABQovflyow736.jpg](http://s3.51cto.com/wyfs02/M00/58/72/wKioL1Sw72OBWcdcAABQovflyow736.jpg)

# ip addr show eth1

![wKiom1Sw7r3S6v6_AACfXZvxonQ064.jpg](http://s3.51cto.com/wyfs02/M02/58/75/wKiom1Sw7r3S6v6_AACfXZvxonQ064.jpg)

# ps aux | grep keepalive[d]

![wKiom1Sw7tnzsSOAAABqz91YIVo562.jpg](http://s3.51cto.com/wyfs02/M01/58/75/wKiom1Sw7tnzsSOAAABqz91YIVo562.jpg)




==========================================================================================

**三、其他设置**

==========================================================================================

**1、Atlas服务监控**

# vim /usr/local/mysql-proxy/bin/check_service.sh



```
```bash
#!/bin/sh
```



```bash
PATH=
```

```bash
/sbin
```

```bash
:
```

```bash
/bin
```

```bash
:
```

```bash
/usr/sbin
```

```bash
:
```

```bash
/usr/bin
```

```bash
:
```

```bash
/usr/local/bin
```

```bash
:
```

```bash
/usr/local/sbin
```



```bash

```



```bash
[[ $
```

```bash
# -ne 3 ]] && echo "$0 端口号 协议类型 服务名" && exit 1
```



```bash

```



```bash
SRV_PORT=$1
```

```bash
## 端口号
```



```bash
SRV_PROT=$2
```

```bash
## 协议类型
```



```bash
SRV_NAME=$3
```

```bash
## 服务名
```



```bash

```



```bash
MONITOR=
```

```bash
"/usr/local/oms/agent/alarm/BusMonitorAgent"
```



```bash
TOKEN=
```

```bash
"ha_monitor"
```



```bash

```



```bash
TITLE=
```

```bash
"${SRV_NAME}服务异常监控"
```



```bash
CONTENT=
```

```bash
"${SRV_NAME}服务发生异常，已自动拉起！"
```



```bash

```



```bash
## 是否已正确扫描
```



```bash
SCAN_FLAG=0
```



```bash

```



```bash
function
```

```bash
RESTART_SRV_AND_ALERT()
```



```bash
{
```



```bash

```

```bash
local
```

```bash
CUR_SRV_NAME
```



```bash

```



```bash

```

```bash
[[ $
```

```bash
# -ne 1 ]] && exit 1
```



```bash

```

```bash
CUR_SRV_NAME=$1
```



```bash

```



```bash

```

```bash
TMP_SRV_NAME=`
```

```bash
echo
```

```bash
${CUR_SRV_NAME} |
```

```bash
tr
```

```bash
'[A-Z]'
```

```bash
'[a-z]'
```

```bash
`
```



```bash

```

```bash
[[ ! -f
```

```bash
/etc/init
```

```bash
.d/${TMP_SRV_NAME} ]] && TMP_SRV_NAME=
```

```bash
"${TMP_SRV_NAME}d"
```



```bash

```



```bash

```

```bash
killall -9 ${TMP_SRV_NAME}
```



```bash

```



```bash

```

```bash
if
```

```bash
[[ -z `
```

```bash
ps
```

```bash
aux |
```

```bash
grep
```

```bash
${TMP_SRV_NAME} |
```

```bash
grep
```

```bash
-
```

```bash
v
```

```bash
grep
```

```bash
` ]];
```

```bash
then
```



```bash

```

```bash
/sbin/service
```

```bash
${TMP_SRV_NAME} start >
```

```bash
/dev/null
```

```bash
2>&1
```



```bash

```

```bash
fi
```



```bash

```



```bash

```

```bash
${MONITOR} -c 2 -f ${TOKEN} -t
```

```bash
"${TITLE}"
```

```bash
-i
```

```bash
"${CONTENT}"
```



```bash

```

```bash
rm
```

```bash
-f `
```

```bash
pwd
```

```bash
`
```

```bash
/connect_error
```

```bash
.log
```



```bash
}
```



```bash

```



```bash
ETH1_ADDR=`
```

```bash
/sbin/ifconfig
```

```bash
eth1 |
```

```bash
awk
```

```bash
-F
```

```bash
':'
```

```bash
'/inet addr/{print $2}'
```

```bash
|
```

```bash
sed
```

```bash
's/[a-zA-Z ]//g'
```

```bash
`
```



```bash
TMP_SRV_PROT=`
```

```bash
echo
```

```bash
${SRV_PROT} |
```

```bash
tr
```

```bash
'[A-Z]'
```

```bash
'[a-z]'
```

```bash
`
```





```bash
if
```

```bash
[[
```

```bash
"${TMP_SRV_PROT}"
```

```bash
==
```

```bash
"tcp"
```

```bash
]];
```

```bash
then
```



```bash

```

```bash
PROT_OPT=
```

```bash
"S"
```



```bash
elif
```

```bash
[[
```

```bash
"${TMP_SRV_PROT}"
```

```bash
==
```

```bash
"udp"
```

```bash
]];
```

```bash
then
```



```bash

```

```bash
PROT_OPT=
```

```bash
"U"
```



```bash
else
```



```bash

```

```bash
echo
```

```bash
"未知的协议类型！"
```

```bash
&&
```

```bash
exit
```

```bash
1
```



```bash
fi
```



```bash

```



```bash
## 最多扫描3次，成功一次即可，以避免网络抖动而导致误判
```



```bash
for
```

```bash
((i=0; i<3; i++));
```

```bash
do
```



```bash

```

```bash
RETVAL=`
```

```bash
/usr/bin/nmap
```

```bash
-n -s${PROT_OPT} -p ${SRV_PORT} ${ETH1_ADDR} |
```

```bash
grep
```

```bash
open
```

```bash
`
```



```bash

```

```bash
[[ -n
```

```bash
"${RETVAL}"
```

```bash
]] && SCAN_FLAG=1;
```

```bash
break
```

```bash
||
```

```bash
sleep
```

```bash
10
```



```bash
done
```



```bash

```



```bash
## 1、针对Atlas服务端口不通的情况，也就是服务彻底挂掉
```



```bash
[[ ${SCAN_FLAG} -
```

```bash
ne
```

```bash
1 ]] && RESTART_SRV_AND_ALERT ${SRV_NAME}
```



```bash

```



```bash
## 2、检查Atlas服务是否正常工作，也就是服务端口正常，但访问异常的情况【高权限DB用户】
```



```bash
mysqladmin -h${ETH1_ADDR} -uhealth_check1 -p123456 --connect-timeout=15 --
```

```bash
shutdown
```

```bash
-timeout=15
```

```bash
ping
```



```bash
[[ $? -
```

```bash
ne
```

```bash
0 ]] && RESTART_SRV_AND_ALERT ${SRV_NAME}
```



```bash

```



```bash
## 3、检查Atlas服务是否正常工作，也就是服务端口正常，高权限DB用户访问也正常，但低权限
```



```bash
##    DB用户访问异常的情况【低权限DB用户】
```



```bash
mysqladmin -h${ETH1_ADDR} -uhealth_check2 -p123456 --connect-timeout=15 --
```

```bash
shutdown
```

```bash
-timeout=15
```

```bash
ping
```



```bash
[[ $? -
```

```bash
ne
```

```bash
0 ]] && RESTART_SRV_AND_ALERT ${SRV_NAME}
```
```



**2、Atlas访问日志切割**

# vim /data/scripts/cut_and_clear_access_log.sh



```
```bash
#!/bin/sh
```



```bash
# 切割Atlas的访问日志，同时清理15天之前的日志
```



```bash
#
```



```bash
PATH=
```

```bash
/sbin
```

```bash
:
```

```bash
/bin
```

```bash
:
```

```bash
/usr/sbin
```

```bash
:
```

```bash
/usr/bin
```

```bash
:
```

```bash
/usr/local/bin
```

```bash
:
```

```bash
/usr/local/sbin
```



```bash

```



```bash
## mysql-proxy日志路径
```



```bash
LOGPATH=
```

```bash
"/usr/local/mysql-proxy/log"
```



```bash

```



```bash
[[ `
```

```bash
/sbin/ip
```

```bash
addr show eth1 |
```

```bash
grep
```

```bash
inet |
```

```bash
wc
```

```bash
-l` -
```

```bash
eq
```

```bash
2 ]] ||
```

```bash
exit
```

```bash
1
```



```bash
cd
```

```bash
${LOGPATH}
```



```bash

```



```bash
## 日志切割
```



```bash
HISTORY_LOG_PATH=`
```

```bash
date
```

```bash
-d
```

```bash
'-1 hour'
```

```bash
+
```

```bash
"%Y-%m-%d/sql_mysql-proxy_%H.log"
```

```bash
`
```



```bash
[[ -d `
```

```bash
dirname
```

```bash
${HISTORY_LOG_PATH}` ]] ||
```

```bash
mkdir
```

```bash
-p `
```

```bash
dirname
```

```bash
${HISTORY_LOG_PATH}`
```



```bash
cp
```

```bash
-a sql_mysql-proxy.log ${HISTORY_LOG_PATH}
```



```bash

```



```bash
echo
```

```bash
> sql_mysql-proxy.log
```



```bash

```



```bash
## 日志清理
```



```bash
HISTORY_LOG_PATH=`
```

```bash
date
```

```bash
-d
```

```bash
'15 days ago'
```

```bash
+
```

```bash
'%Y-%m-%d'
```

```bash
`
```



```bash
[[ -d ${HISTORY_LOG_PATH} ]] &&
```

```bash
rm
```

```bash
-rf ${HISTORY_LOG_PATH}
```
```



**3、crontab内容添加**

# touch /var/lock/check_service.lock

# echo 'touch /var/lock/check_service.lock' >> /etc/rc.d/rc.local

# crontab -uroot -e



```
`* * * * * (flock --timeout=0 /var/lock/check_service.lock /usr/local/mysql-proxy/bin/check_service.sh 3306 tcp mysql-proxy >/dev/null 2>&1)`

`00 * * * * /data/scripts/cut_and_clear_access_log.sh >/dev/null 2>&1`
```





**4、平滑设置功能**

# mysql -h10.209.6.101 -P3307 -usysadmin -p'admin2356!@()'

![wKioL1Sw8S_gXZOuAALgQK7R39c195.jpg](http://s3.51cto.com/wyfs02/M01/58/72/wKioL1Sw8S_gXZOuAALgQK7R39c195.jpg)







本文出自 “[人生理想在于坚持不懈](http://sofar.blog.51cto.com)” 博客，请务必保留此出处[http://sofar.blog.51cto.com/353572/1601552](http://sofar.blog.51cto.com/353572/1601552)




