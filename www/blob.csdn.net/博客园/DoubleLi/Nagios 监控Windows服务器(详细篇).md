# Nagios 监控Windows服务器(详细篇) - DoubleLi - 博客园








1. 监控内容

windows服务器的内部参数包括以下

a. 内存使用状况

b. CPU负载

c. 磁盘使用状况

d. 服务状态

e. 运行的进程

2. 监控原理

在windows服务器内安装NSClient++的监控引擎，nagios服务器通过check_nt来获取监控数据

![](https://wepodq.blu.livefilestore.com/y1mAYc4KY7CFBZUckf37clRmB6wnMigAmjal4-9bET0WgN3gXHGfp8KdG14M6wR_23vGL0HJX4Tn2MTEGrD5Mn_K8pw2hGhsdGmPuIDBcu6yMel4Vcp0__AlBwvWnOWjRPQoWsGDa9JKls/monitoring-windows.png)

3.配置步骤

a.修改启动参数

b.windows服务器安装监控引擎

c.在Nagios服务器内建立监控主机和监控服务配置

d.重新启动Nagios服务

4.确认已经配置是否存在

在/usr/local/nagios/etc/objects/commands.cfg中是否有以下内容（允许使用check_nt来监控windows服务）

# 'check_nt' command definition
define command{
        command_name    check_nt
        command_line    $USER1$/check_nt -H $HOSTADDRESS$ -p 12489 -v $ARG1$ $ARG2$
        }

在/usr/local/nagios/etc/object/templates.cfg中是否有以下内容（这个用于未来添加新主机的时候的范例）

define host{
        name                    windows-server  ; The name of this host template
        use                     generic-host    ; Inherit default values from the generic-host template
        check_period            24x7            ; By default, Windows servers are monitored round the clock
        check_interval          5               ; Actively check the server every 5 minutes
        retry_interval          1               ; Schedule host check retries at 1 minute intervals
        max_check_attempts      10              ; Check each server 10 times (max)
        check_command           check-host-alive        ; Default command to check if servers are "alive"
        notification_period     24x7            ; Send notification out at any time - day or night
        notification_interval   30              ; Resend notifications every 30 minutes
        notification_options    d,r             ; Only send notifications for specific host states
        contact_groups          admins          ; Notifications get sent to the admins by default
        hostgroups              windows-servers ; Host groups that Windows servers should be a member of
        register                0               ; DONT REGISTER THIS - ITS JUST A TEMPLATE
        }

5.nagios在第一次监控windows所必须修改的内容

修改/usr/local/nagios/etc/nagios.cfg

查找

#cfg_file=/usr/local/nagios/etc/objects/windows.cfg

改为

cfg_file=/usr/local/nagios/etc/objects/windows.cfg

保存文件。

/usr/local/nagios/etc/objects/windows.cfg 内包含了一些样例的主机、主机群和服务的定义模式。可以通过修改这些案例来测试监控对象。

6.安装windows监控引擎

下载NSClient++的安装包，注意根据实际的32/64CPU来下载

下载地址 [http://sourceforge.net/projects/nscplus](http://sourceforge.net/projects/nscplus)

安装方式，解压缩到C:\NSClient++

在指令模式下，在该目录下面运行以下命令，注册NSCient++的服务

> 
nsclient++ /install


将NSCient++作为启动进程

> 
nsclient++ SysTray


在服务列表中找到NSClientpp的服务。双击打开该服务的配置窗口则在登录分页中将“允许服务与桌面交互”勾选上。

编辑NSC.INI，做以下更改
- 在[Module]部分，除了CheckWMI.dll和RemoteConfiguration.dll这两行的注释不要去掉，其他DLL的注释都去掉。
- 在[Setting]部分的password设定一个密码，并把前面的注释去掉。

> 
password=secret-password 改为 password=123456

- 在[Setting]部分的allowed_hosts部分设定Nagios的服务器IP或者设定一个IP段，并把前面的注释去掉。

> 
allowed_hosts=192.168.200.249/32

- 在[NSClient]部分将port前的注释去掉，确认port的端口号是12489

启动nsclient++服务

当右下角出现了一个蓝色十字的ICON这表示

7.配置Nagios

建立需要监控的windows主机和服务对象

编辑文件windows.cfg

/usr/local/nagios/etc/objects/windows.cfg

在host的定义部分，可以修改实例中的host_name,alias,address三个部分来的参数。第一个参数use则是继承了template.cfg中对Windows的监控参数

define host{
        use             windows-server  ; 从template中继承相关监控参数
        host_name       winserver       ; 主机名称
        alias           My Windows Server       ; 别名
        address         192.168.1.2     ; IP地址
        }

在Service定义部分

首先需要设定以下内容，用于确认监控对象的NSClient++的版本是否正确
define service{
	use			generic-service
	host_name			winserver
	service_description	NSClient++ Version
	check_command		check_nt!CLIENTVERSION
	}

然后是定义监控对象的正常运行时间

check_command           check_nt!UPTIME

随后是定义CPU的负载状况，下面的定义表示在5分钟内的平均负载超过80%则发出警告WARNING，而超过90%则是危机报警CRITICAL alert

check_command           check_nt!CPULOAD!-l 5,80,90

定义内存负载状况，当内存使用率达到80则warning 90%则CRITICAL alert

check_command check_nt!MEMUSE!-w 80 -c 90

监控C盘空间，使用率达到80则warning 90%则CRITICAL alert

check_command check_nt!USEDDISKSPACE!-l c -w 80 -c 90

监控服务状态的格式，当服务停止了则发送CRITICAL alert

check_command check_nt!SERVICESTATE!-d SHOWALL -l W3SVC

监控系统进程，当进程处于非运行状态时，则发送CRITICAL alert

check_command check_nt!PROCSTATE!-d SHOWALL -l Explorer.exe

8.密码设定

编辑/usr/local/nagios/etc/objects/commands.cfg 在check_nt部分中，command_line后面添加一个参数-s "NSClient主机的密码"

command_line $USER1$/check_nt -H $HOSTADDRESS$ -p 12489 -s PASSWORD -v $ARG1$ $ARG2$

可以通过以下指令测试设备响应是否正常

/usr/local/nagios/libexec/check_nt -H 主机IP -p 12489 -s 密码 -v UPTIME

如果反馈的信息是System Uptime - 0 day(s) 8 hour(s) 44 minute(s) 则表示连接正常。

如果显示为could not fetch information from server，则有以下可能

密码不正确（最傻的可能，也是经常发生的可能）

服务器上有防火墙，需要开放12489端口。

在windows上的NSC.INI上的allow_hosts没有添加正确的nagios IP、

9.重新启动Nagios服务

在修改nagios配置文件之后，则可以运行/usr/local/nagios/bin/nagios -v /usr/local/nagios/etc/nagios.cfg

来验证配置文件是否正确。

service nagios restart

看看nagios的管理页面，被监控的windows服务器是否显示



参考资料

[http://nagios.sourceforge.net/docs/3_0/monitoring-windows.html](http://nagios.sourceforge.net/docs/3_0/monitoring-windows.html)













