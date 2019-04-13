
# CentOS 7 sytemctl 自定义服务开机启动 - 阳光岛主 - CSDN博客

2016年06月27日 09:29:41[阳光岛主](https://me.csdn.net/sunboy_2050)阅读数：16702


CentOS 7继承了RHEL 7的新的特性，例如强大的systemctl，而systemctl的使用也使得系统服务的/etc/init.d的启动脚本的方式发生重大改变，也大幅提高了系统服务的运行效率。但服务的配置和以往也发生了极大的不同，变的简单而易用了许多（仁者见仁，[米扑博客](http://blog.mimvp.com/2016/06/centos-7-sytemctl-custom-service-startup/)）。
systemd提供更优秀的框架以表示系统服务间的依赖关系，实现系统初始化时服务的并行启动，同时达到降低Shell的系统开销的效果。
systemd 目标是：尽可能启动更少进程；尽可能将更多进程并行启动，systemd尽可能减少对shell脚本的依赖。
systemd单位类型
systemctl –type=单位类型，用来过滤单位，
例如：systemctl –type=service
|1
|2
|3
|4
|5
|6
|7
|8
|9
|10
|11
|12
|13
|14
|15
|16
|17
|18
|19
|20
|[root@mimvp_usa ~]|\# systemctl --type=service
|UNIT                               LOAD   ACTIVE SUB     DESCRIPTION
|aegis.service                      loaded active running LSB: aegis update.
|agentwatch.service                 loaded active exited  SYSV: Starts and stops guest agent
|aliyun.service                     loaded active running auto run aliyunservice or agent
|crond.service                      loaded active running Command Scheduler
|dbus.service                       loaded active running D-Bus System Message Bus
|getty@tty1.service                 loaded active running Getty on tty1
|httpd.service                      loaded active running The Apache HTTP Server
|kmod-static-nodes.service          loaded active exited  Create list of required static device nodes|for|the current kernel
|lvm2-lvmetad.service               loaded active running LVM2 metadata daemon
|lvm2-monitor.service               loaded active exited  Monitoring of LVM2 mirrors, snapshots etc. using dmeventd or progress
|mariadb.service                    loaded active running MariaDB database server
|● mongod.service                     loaded failed failed  SYSV: Mongo is a scalable, document-oriented database.
|mongodb.service                    loaded active running mongodb
|network.service                    loaded active exited  LSB: Bring up|/down|networking
|nscd.service                       loaded active running Name Service Cache Daemon
|ntpd.service                       loaded active running Network Time Service
|polkit.service                     loaded active running Authorization Manager
|rc-|local|.service                   loaded active exited|/etc/rc|.d|/rc|.|local|Compatibility
|

服务（service）：管理着后台服务；
挂载（mount）自动挂载（automount）：用来挂载文件系统；
目票（target）：运行级别；
套接字（socket）：用来创建套接字，并在访问套接字后，立即利用依赖关系间接地启动另一单位；
**开机服务管理**
CentOS 7的服务systemctl脚本存放在：/usr/lib/systemd/，有系统（system）和用户（user）之分，
/usr/lib/systemd/system/
/usr/lib/systemd/user/
像需要开机不登陆就能运行的程序，存在系统服务，即：/usr/lib/systemd/system/ 目录下
每一个服务以.service结尾，一般会分为3部分：[Unit]、[Service]、[Install]
**[Unit]**主要是对这个服务的说明，内容包括Description和After，Description用于描述服务，After用于描述服务类别
**[Service]**是服务的关键，是服务的一些具体运行参数的设置，
Type=forking是后台运行的形式，
PIDFile为存放PID的文件路径，
ExecStart为服务的具体运行命令，
ExecReload为重启命令，
ExecStop为停止命令，
PrivateTmp=True表示给服务分配独立的临时空间
注意：[Service]部分的启动、重启、停止命令全部要求使用绝对路径，使用相对路径则会报错！
**[Install]**是服务安装的相关设置，可设置为多用户的
服务脚本按照上面编写完成后，以754的权限保存在/usr/lib/systemd/system/目录下，这时就可以利用systemctl进行配置
**示例：**
**自定义 mongodb 开机启动**
vim  /usr/lib/systemd/system/mongodb.service
|1
|2
|3
|4
|5
|6
|7
|8
|9
|10
|11
|12
|13
|[Unit]
|Description=mongodb
|After=auditd.service systemd-user-sessions.service|time|-|sync|.target network.target
|[Service]
|Type=forking
|PIDFile=|/var/run/mongodb/mongod|.pid
|ExecStart=|/root/script/mongo_server_start|.sh
|ExecStop=|/root/script/mongo_server_stop|.sh
|PrivateTmp=|true
|[Install]
|WantedBy=multi-user.target
|

**systemctl 配置命令**
systemctl status mongodb.service            // 查看mongodb启动状态
systemctl start mongodb.service             // 启动 mongodb
systemctl stop mongodb.service              // 关闭 mongodb
systemctl enable mongodb.service         // 开机启动 mongodb 服务
systemctl disable mongodb.service         // 开机关闭 mongodb 服务
示例：
systemctl status mongodb.service
|1
|2
|3
|4
|5
|6
|7
|8
|9
|10
|11
|12
|13
|14
|15
|● mongodb.service - mongodb
|Loaded: loaded (|/usr/lib/systemd/system/mongodb|.service; enabled; vendor preset: disabled)
|Active: active (running) since Wed 2016-06-15 19:05:41 CST; 2min 24s ago
|Process: 1143 ExecStop=|/root/script/mongo_server_stop|.sh (code=exited, status=1|/FAILURE|)
|Process: 1158 ExecStart=|/root/script/mongo_server_start|.sh (code=exited, status=0|/SUCCESS|)
|Main PID: 1161 (mongod)
|CGroup:|/system|.slice|/mongodb|.service
|└─1161|/usr/bin/mongod|-f|/etc/mongod|.conf
|Jun 15 19:05:41 mimvp_usa systemd[1]: Starting mongodb...
|Jun 15 19:05:41 mimvp_usa mongo_server_start.sh[1158]: about to fork child process, waiting|until|server is ready|for|con...ions.
|Jun 15 19:05:41 mimvp_usa mongo_server_start.sh[1158]: forked process: 1161
|Jun 15 19:05:41 mimvp_usa mongo_server_start.sh[1158]: child process started successfully, parent exiting
|Jun 15 19:05:41 mimvp_usa systemd[1]: Started mongodb.
|Hint: Some lines were ellipsized, use -l to show|in|full.
|

systemctl 虽然比较陌生，但是其实比init.d那种方式简单不少，而且使用简单，systemctl能简化的操作还有很多，现在也有不少的资料，看来RHEL/CentOS比其他的Linux发行版还是比较先进的，此次更新也终于舍弃了Linux 2.6内核，无论是速度还是稳定性都提升不少。



