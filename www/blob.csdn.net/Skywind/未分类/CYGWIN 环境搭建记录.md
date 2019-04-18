# Skywind Inside » CYGWIN 环境搭建记录
## CYGWIN 环境搭建记录
August 12th, 2011[skywind](http://www.skywind.me/blog/archives/author/skywind)[Leave a comment](#respond)[Go to comments](#comments)
[sshd]
ssh-host-config
/var/empty目录的拥有者必须是sshd进程的拥有者, 而且权限必须是700; 如果sshd进程的拥有者不是SYSTEM, 则只有这个拥有者可以通过ssh登录系统, 因为sshd需要执行seteuid, 非系统账户执行seteuid将自己有效id设为其他用户时会被拒绝。
如果是在 Win7下面需要建立一个独立的用户，这时CYGWIN终端需要以管理员权限运行，否则用户设置失败，无法安装sshd服务。
[svnserve]
cygrunsrv -I svnserve -d “CYGWIN svnserve” –chdir /data/svnroot –path /usr/bin/svnserve –args “–foreground –daemon –root=/data/svnroot –listen-port=3690″ –neverexits –type auto -o
如果发现svnserve.exe无法监听在相应端口, 需要添加额外参数(–args中): –listen-host=0.0.0.0 (或监听在特定的ip或主机名上).
[passwd]
如果在cygwin中发现自己所在的组变成了”mkpaswd”, 这说明/etc/passwd或者/etc/group需要重建了, 运行:
mkpasswd -l >/etc/passwd; mkgroup -l >/etc/group
[httpd]
启动httpd服务时可能会报告”Bad system call”错误, 这是由于apache2需要cygserver, 运行:
cygserver-config
而且环境变量$CYGWIN中包含server, 参见[he CYGWIN environment variable].
[init]
配置启动 systemv 的 init 服务，管理员启动cygwin，并运行：
init-config
然后查看 /etc/inittab 和 /etc/rc.d/rc 两个文件，根据需要编辑，首先是   /etc/inittab:
```shell
# id:runlevels:action:process
id:3:initdefault:
rc::bootwait:/etc/rc
l0:0:wait:/etc/rc.d/rc 0
l3:3:wait:/etc/rc.d/rc 3
#S0:2345:respawn:/sbin/agetty -L -T vt100 -n ttyS0 9600 ttyS0
```
接下来是 /etc/rc.d/rc：
```shell
#! /bin/bash
# Now find out what the current and what the previous runlevel are.
argv1="$1"
set `/sbin/runlevel`
runlevel=$2
previous=$1
export runlevel previous
# Source function library.
. /etc/rc.d/init.d/functions
# Get first argument. Set new runlevel to this argument.
[ -n "$argv1" ] && runlevel="$argv1"
# echo "rc $runlevel" >> /tmp/rc.log
# Is there an rc directory for this new runlevel?
if [ -d /etc/rc.d/rc$runlevel.d ]; then
        # First, run the KILL scripts.
        for i in /etc/rc.d/rc$runlevel.d/K*; do
                # Check if the script is there.
                [ ! -f $i ] && continue
                # stop script
                $i stop
        done
        # Now run the START scripts.
        for i in /etc/rc.d/rc$runlevel.d/S*; do
                # Check if the script is there.
                [ ! -f $i ] && continue
                # start script
                $i start
        done
fi
```
不要直接复制上面的代码，最好照着用手敲，页面上 copy/paste 经常会把 indent 搞乱，bash脚本的缩进一乱就没法运行了。使用时把启动停止脚本放到 /etc/rc/init.d 下面，然后启动时候做软连接：
```shell
/etc/rc.d/rc0.d/K*     # 停止脚本
/etc/rc.d/rc3.d/S*     # 启动脚本
```
别忘记 init-config 时候把 init 安装成 windows 服务，如此，可以自由的增加自动启动脚本了。
[supervisor]
安装 python2-setuptools 然后：
```shell
easy_install_2.7 pip
pip install supervisor
mkdir /etc/supervisor
mkdir /etc/supervisor/conf.d
mkdir /var/log/supervisor
```
编辑 /etc/supervisor/supervisord.conf ：
```shell
[unix_http_server]
file=/var/run/supervisord.sock
chmod=666
[supervisord]
logfile=/var/log/supervisor/supervisord.log
logfile_maxbytes=50000000
logfile_backup=10
loglevel=info
pidfile=/var/run/supervisord.pid
nodaemon=false
minfds=2048
minprocs=200
childlogdir=/var/log/supervisor
[rpcinterface:supervisor]
supervisor.rpcinterface_factory = supervisor.rpcinterface:make_main_rpcinterface
[supervisorctl]
serverurl=unix:///var/run/supervisord.sock
[include]
files=/etc/supervisor/conf.d/*.conf
```
然后就可以使用了
