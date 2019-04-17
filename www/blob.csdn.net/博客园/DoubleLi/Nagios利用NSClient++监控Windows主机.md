# Nagios利用NSClient++监控Windows主机 - DoubleLi - 博客园






**一、简介**

1、获取更多的NSClient原理，请参考NSClient++官方站点

NSClient++官网：[http://www.nsclient.org/](http://www.nsclient.org/)



2、NSClient++与NRPE

NSClient++的工作原理



![wKiom1QnYaewuJNPAACIXWqRWWw312.jpg](http://s3.51cto.com/wyfs02/M02/4A/E9/wKiom1QnYaewuJNPAACIXWqRWWw312.jpg)



NRPE的工作原理

![wKiom1Qnc3zAjECMAAEUsOqv9kM145.jpg](http://s3.51cto.com/wyfs02/M00/4A/F0/wKiom1Qnc3zAjECMAAEUsOqv9kM145.jpg)



Nagios对Windows主机的监控主要有三种方法

第一种是NSclient++

第二种是NRPE

第三种是SNMP(不是很常用)



NSclient++与nrpe最大的区别就是:

1、被监控机上安装有nrpe,并且还有插件,最终的监控是由这些插件来进行的.当监控主机将监控请求发给nrpe后,nrpe调用插件来完成监控.

2、NSclient++则不同,被监控机上只安装NSclient++,没有任何的插件.当监控主机将监控请求发给NSclient++后,NSclient++直接完成监控,所有的监控是由NSclient++完成的。

这也说明了NSclient++的一个很大的问题,不灵活,没有可扩展性.它只能完成自己本身包含的监控操作,不能由一些插件来扩展.好在NSclient++已经做的不错了,基本上可以完全满足我们的监控需要。



**二、check_nt的方式监控windows主机**

1、下载NSClient++

下载地址：[http://sourceforge.net/projects/nscplus/files/nscplus/](http://sourceforge.net/projects/nscplus/files/nscplus/)



2、安装NSClient++

![wKiom1QmswLChu7BAAF5ugAoQLo820.jpg](http://s3.51cto.com/wyfs02/M01/4A/B2/wKiom1QmswLChu7BAAF5ugAoQLo820.jpg)

![wKioL1QmsyqyCXr-AAHsG6bmgZw306.jpg](http://s3.51cto.com/wyfs02/M02/4A/B4/wKioL1QmsyqyCXr-AAHsG6bmgZw306.jpg)

![wKiom1QmswOwzMYBAAGsdr9E0Pc403.jpg](http://s3.51cto.com/wyfs02/M02/4A/B2/wKiom1QmswOwzMYBAAGsdr9E0Pc403.jpg)

![wKioL1QmsyqTfejsAAGRfdnH-xU473.jpg](http://s3.51cto.com/wyfs02/M00/4A/B4/wKioL1QmsyqTfejsAAGRfdnH-xU473.jpg)

Allowed hosts：(this is the IP of the nagios (or other)server)

允许的主机地址：Nagios服务器端的IP地址



**NSClient password（only userd via check_nt）**

NSClient的密码：填写Nagios跟NSClient++进程通信的密码，可以不设置



Modules to load：

安装并加载相应的模块：NSClient++自带的有check_plugins插件，check_nt,check_nrpe,NSCA,WMI

在这里我们全部选择，以后会有用到的。

![wKiom1QmswOTN7ZdAAF8CMhGqWs779.jpg](http://s3.51cto.com/wyfs02/M00/4A/B2/wKiom1QmswOTN7ZdAAF8CMhGqWs779.jpg)

![wKioL1Qmsyvjmrj1AADuL2UQatY830.jpg](http://s3.51cto.com/wyfs02/M02/4A/B4/wKioL1Qmsyvjmrj1AADuL2UQatY830.jpg)

点击【Finsh】，表示安装NSClient++完成

![wKioL1QmsyujK89FAAGCBr_MNf0266.jpg](http://s3.51cto.com/wyfs02/M01/4A/B4/wKioL1QmsyujK89FAAGCBr_MNf0266.jpg)



3、查看NSClient++服务是否已经启用

![wKioL1QnXTmgMzAWAAQEAtOdHlA759.jpg](http://s3.51cto.com/wyfs02/M02/4A/E9/wKioL1QnXTmgMzAWAAQEAtOdHlA759.jpg)

![wKiom1QnYYexKlUCAACG8_0iqhA152.jpg](http://s3.51cto.com/wyfs02/M01/4A/E9/wKiom1QnYYexKlUCAACG8_0iqhA152.jpg)



4、查看NSClient++的配置文件

默认是安装在C:\Program Files\NSClient++ 目录下，NSC.ini即为NSClient服务的配置文件，一般我们无需修改，但是当我们监控端的IP地址改变时，或者密码忘记，即可以在这里修改了。

安装时加载的模块

![wKioL1QnXLeisp6DAAOmUPyr8zQ318.jpg](http://s3.51cto.com/wyfs02/M00/4A/E9/wKioL1QnXLeisp6DAAOmUPyr8zQ318.jpg)

设置允许连接的地址：为Nagios的IP地址

![wKiom1QnXJDxt5BDAAZlbauD00Y308.jpg](http://s3.51cto.com/wyfs02/M00/4A/E7/wKiom1QnXJDxt5BDAAZlbauD00Y308.jpg)

NRPE的默认端口号

![wKioL1QnXLnCQe-sAATIjXBJX2M538.jpg](http://s3.51cto.com/wyfs02/M01/4A/E9/wKioL1QnXLnCQe-sAATIjXBJX2M538.jpg)



三、NSClient应用监控

NSClient++与Nagios服务器通信，主要使用Nagios服务器的check_nt插件。原理图如下

![wKiom1QnXc7Rghy-AAC9NsiJ0jA271.jpg](http://s3.51cto.com/wyfs02/M02/4A/E8/wKiom1QnXc7Rghy-AAC9NsiJ0jA271.jpg)



1、check_nt插件的使用说明



```
```bash
[root@Nagios ~]
```

```bash
# cd /usr/local/nagios/libexec/
```



```bash
[root@Nagios libexec]
```

```bash
# ./check_nt -h   #查看check_nt的语法
```



```bash
check_nt v2.0.3 (nagios-plugins 2.0.3)
```



```bash
Copyright (c) 2000 Yves Rubin (rubiyz@yahoo.com)
```



```bash
Copyright (c) 2000-2014 Nagios Plugin Development Team
```



```bash

```

```bash
<devel@nagios-plugins.org>
```





```bash
This plugin collects data from the NSClient service running on a
```



```bash
Windows NT
```

```bash
/2000/XP/2003
```

```bash
server.
```







```bash
Usage:
```



```bash
check_nt -H host -
```

```bash
v
```

```bash
variable [-p port] [-w warning] [-c critical]
```



```bash
[-l params] [-d SHOWALL] [-u] [-t timeout]
```





```bash
Options:
```



```bash

```

```bash
-h, --help
```



```bash

```

```bash
Print detailed help
```

```bash
screen
```



```bash

```

```bash
-V, --version
```



```bash

```

```bash
Print version information
```



```bash

```

```bash
--extra-opts=[section][@
```

```bash
file
```

```bash
]
```



```bash

```

```bash
Read options from an ini
```

```bash
file
```

```bash
. See
```



```bash

```

```bash
https:
```

```bash
//www
```

```bash
.nagios-plugins.org
```

```bash
/doc/extra-opts
```

```bash
.html
```



```bash

```

```bash
for
```

```bash
usage and examples.
```



```bash
Options:
```



```bash

```

```bash
-H, --
```

```bash
hostname
```

```bash
=HOST
```



```bash

```

```bash
Name of the host to check
```



```bash

```

```bash
-p, --port=INTEGER
```



```bash

```

```bash
Optional port number (default: 1248)
```

```bash
#默认端口号
```



```bash

```

```bash
-s, --secret=<password>
```

```bash
#指定的密码
```



```bash

```

```bash
Password needed
```

```bash
for
```

```bash
the request
```



```bash

```

```bash
-w, --warning=INTEGER
```



```bash

```

```bash
Threshold
```

```bash
which
```

```bash
will result
```

```bash
in
```

```bash
a warning status
```



```bash

```

```bash
-c, --critical=INTEGER
```



```bash

```

```bash
Threshold
```

```bash
which
```

```bash
will result
```

```bash
in
```

```bash
a critical status
```



```bash

```

```bash
-t, --timeout=INTEGER
```



```bash

```

```bash
Seconds before connection attempt
```

```bash
times
```

```bash
out (default:  -l, --params=<parameters>
```



```bash

```

```bash
Parameters passed to specified check (see below) -d, --display={SHOWALL}
```



```bash

```

```bash
Display options (currently only SHOWALL works) -u, --unknown-timeout
```



```bash

```

```bash
Return UNKNOWN on timeouts10)
```



```bash

```

```bash
-h, --help
```



```bash

```

```bash
Print this help
```

```bash
screen
```



```bash

```

```bash
-V, --version
```



```bash

```

```bash
Print version information
```



```bash

```

```bash
-
```

```bash
v
```

```bash
, --variable=STRING
```



```bash

```

```bash
Variable to check
```





```bash
Valid variables are:
```



```bash

```

```bash
CLIENTVERSION = Get the NSClient version
```



```bash

```

```bash
If -l <version> is specified, will
```

```bash
return
```

```bash
warning
```

```bash
if
```

```bash
versions differ.
```



```bash

```

```bash
CPULOAD =
```



```bash

```

```bash
Average CPU load on last x minutes.
```



```bash

```

```bash
Request a -l parameter with the following syntax:
```



```bash

```

```bash
-l <minutes range>,<warning threshold>,<critical threshold>.
```



```bash

```

```bash
<minute range> should be
```

```bash
less
```

```bash
than 24*60.
```



```bash

```

```bash
Thresholds are percentage and up to 10 requests can be
```

```bash
done
```

```bash
in
```

```bash
one shot.
```



```bash

```

```bash
ie: -l 60,90,95,120,90,95
```



```bash

```

```bash
UPTIME =
```



```bash

```

```bash
Get the uptime of the machine.
```



```bash

```

```bash
-l <unit>
```



```bash

```

```bash
<unit> = seconds, minutes, hours, or days. (default: minutes)
```



```bash

```

```bash
Thresholds will use the unit specified above.
```



```bash

```

```bash
USEDDISKSPACE =
```



```bash

```

```bash
Size and percentage of disk use.
```



```bash

```

```bash
Request a -l parameter containing the drive letter only.
```



```bash

```

```bash
Warning and critical thresholds can be specified with -w and -c.
```



```bash

```

```bash
MEMUSE =
```



```bash

```

```bash
Memory use.
```



```bash

```

```bash
Warning and critical thresholds can be specified with -w and -c.
```



```bash

```

```bash
SERVICESTATE =
```



```bash

```

```bash
Check the state of one or several services.
```



```bash

```

```bash
Request a -l parameters with the following syntax:
```



```bash

```

```bash
-l <service1>,<service2>,<service3>,...
```



```bash

```

```bash
You can specify -d SHOWALL
```

```bash
in
```

```bash
case
```

```bash
you want to see working services
```



```bash

```

```bash
in
```

```bash
the returned string.
```



```bash

```

```bash
PROCSTATE =
```



```bash

```

```bash
Check
```

```bash
if
```

```bash
one or several process are running.
```



```bash

```

```bash
Same syntax as SERVICESTATE.
```



```bash

```

```bash
COUNTER =
```



```bash

```

```bash
Check any performance counter of Windows NT
```

```bash
/2000
```

```bash
.
```



```bash

```

```bash
Request a -l parameters with the following syntax:
```



```bash

```

```bash
-l
```

```bash
"\\<performance object>\\counter"
```

```bash
,"<description>
```



```bash

```

```bash
The <description> parameter is optional and is given to a
```

```bash
printf
```



```bash

```

```bash
output
```

```bash
command
```

```bash
which
```

```bash
requires a float parameter.
```



```bash

```

```bash
If <description> does not include
```

```bash
"%%"
```

```bash
, it is used as a label.
```



```bash

```

```bash
Some examples:
```



```bash

```

```bash
"Paging file usage is %%.2f %%%%"
```



```bash

```

```bash
"%%.f %%%% paging file used."
```



```bash

```

```bash
INSTANCES =
```



```bash

```

```bash
Check any performance counter object of Windows NT
```

```bash
/2000
```

```bash
.
```



```bash

```

```bash
Syntax: check_nt -H <
```

```bash
hostname
```

```bash
> -p <port> -
```

```bash
v
```

```bash
INSTANCES -l <counter object>
```



```bash

```

```bash
<counter object> is a Windows Perfmon Counter object (eg. Process),
```



```bash

```

```bash
if
```

```bash
it is two words, it should be enclosed
```

```bash
in
```

```bash
quotes
```



```bash

```

```bash
The returned results will be a comma-separated list of instances on
```



```bash

```

```bash
the selected computer
```

```bash
for
```

```bash
that object.
```



```bash

```

```bash
The purpose of this is to be run from
```

```bash
command
```

```bash
line to determine what instances
```



```bash

```

```bash
are available
```

```bash
for
```

```bash
monitoring without having to log onto the Windows server
```



```bash

```

```bash
to run Perfmon directly.
```



```bash

```

```bash
It can also be used
```

```bash
in
```

```bash
scripts that automatically create Nagios service
```



```bash

```

```bash
configuration files.
```



```bash

```

```bash
Some examples:
```



```bash

```

```bash
check_nt -H 192.168.1.1 -p 1248 -
```

```bash
v
```

```bash
INSTANCES -l Process
```

```bash
#check_nt的语法
```



```bash

```



```bash
Notes:
```



```bash

```

```bash
- The NSClient service should be running on the server to get any information
```



```bash

```

```bash
(http:
```

```bash
//nsclient
```

```bash
.ready2run.
```

```bash
nl
```

```bash
).
```



```bash

```

```bash
- Critical thresholds should be lower than warning thresholds
```



```bash

```

```bash
- Default port 1248 is sometimes
```

```bash
in
```

```bash
use by other services. The error
```



```bash

```

```bash
output when this happens contains
```

```bash
"Cannot map xxxxx to protocol number"
```

```bash
.
```



```bash

```

```bash
One fix
```

```bash
for
```

```bash
this is to change the port to something
```

```bash
else
```

```bash
on check_nt
```



```bash

```

```bash
and on the client service it's connecting to.
```





```bash
Send email to help@nagios-plugins.org
```

```bash
if
```

```bash
you have questions regarding use
```



```bash
of this software. To submit patches or suggest improvements, send email to
```



```bash
devel@nagios-plugins.org
```
```



2、check_nt命令的使用

check_nt参数解释

-w：警告比例

-c：紧急比例

-l：过去5分钟的平均值，80%的警告  90%紧急![wKiom1QmuKqBRFWJAAJvlnFCVQ4764.jpg](http://s3.51cto.com/wyfs02/M01/4A/B4/wKiom1QmuKqBRFWJAAJvlnFCVQ4764.jpg)



四、定义命令、主机、服务

1、定义命令



```
```bash
[root@Nagios ~]
```

```bash
# vim /usr/local/nagios/etc/objects/commands.cfg
```



```bash
# 'check_win' command definition
```



```bash
define
```

```bash
command
```

```bash
{
```



```bash

```

```bash
command_name    check_win
```



```bash

```

```bash
command_line    $USER1$
```

```bash
/check_nt
```

```bash
-H $HOSTADDRESS$ -p 12489 -
```

```bash
v
```

```bash
$ARG1$ $ARG2$
```



```bash

```

```bash
}
```



```bash
注释：
```



```bash
$..$    表示系统内置的宏，也就是所谓的变量
```



```bash
$USER1$ 表示插件所在的目录
```



```bash
-H      指定主机地址
```



```bash
$HOSTADDRESS$  应用到哪个主机，就用哪个主机的地址
```



```bash
$ARG1$  传递的参数,形参
```



```bash
-s      指定密码 默认为空
```
```





2、定义主机和服务



```
```bash
[root@Nagios objects]
```

```bash
# cp windows.cfg windows106.cfg
```



```bash
[root@Nagios objects]
```

```bash
# sed -i 's/winserver/Windows106/g' windows106.cfg
```



```bash
[root@Nagios objects]
```

```bash
# sed -i 's/192.168.1.2/192.168.0.106/' windows106.cfg
```
```



4、检测配置文件是否有语法错误



```
```bash
[root@Nagios ~]
```

```bash
# service nagios configtest
```





```bash
Nagios Core 4.0.7
```



```bash
Copyright (c) 2009-present Nagios Core Development Team and Community Contributors
```



```bash
Copyright (c) 1999-2009 Ethan Galstad
```



```bash
Last Modified: 06-03-2014
```



```bash
License: GPL
```





```bash
Website: http:
```

```bash
//www
```

```bash
.nagios.org
```



```bash
Reading configuration data...
```



```bash

```

```bash
Read main config
```

```bash
file
```

```bash
okay...
```



```bash

```

```bash
Read object config files okay...
```





```bash
Running pre-flight check on configuration data...
```





```bash
Checking objects...
```



```bash

```

```bash
Checked 8 services.
```



```bash

```

```bash
Checked 1 hosts.
```



```bash

```

```bash
Checked 1 host
```

```bash
groups
```

```bash
.
```



```bash

```

```bash
Checked 0 service
```

```bash
groups
```

```bash
.
```



```bash

```

```bash
Checked 1 contacts.
```



```bash

```

```bash
Checked 1 contact
```

```bash
groups
```

```bash
.
```



```bash

```

```bash
Checked 25 commands.
```



```bash

```

```bash
Checked 5
```

```bash
time
```

```bash
periods.
```



```bash

```

```bash
Checked 0 host escalations.
```



```bash

```

```bash
Checked 0 service escalations.
```



```bash
Checking
```

```bash
for
```

```bash
circular paths...
```



```bash

```

```bash
Checked 1 hosts
```



```bash

```

```bash
Checked 0 service dependencies
```



```bash

```

```bash
Checked 0 host dependencies
```



```bash

```

```bash
Checked 5 timeperiods
```



```bash
Checking global event handlers...
```



```bash
Checking obsessive compulsive processor commands...
```



```bash
Checking misc settings...
```





```bash
Total Warnings: 0
```



```bash
Total Errors:   0
```





```bash
Things
```

```bash
look
```

```bash
okay - No serious problems were detected during the pre-flight check
```



```bash
Object precache
```

```bash
file
```

```bash
created:
```



```bash
/usr/local/nagios/var/objects
```

```bash
.precache
```
```





5、重启nagios服务



```
```bash
[root@Nagios objects]
```

```bash
# service nagios restart
```



```bash
Running configuration check...
```



```bash
Stopping nagios: .
```

```bash
done
```

```bash
.
```



```bash
Starting nagios:
```

```bash
done
```

```bash
.
```
```





四、浏览器查看监控信息

1、登陆后点击【Hosts】，查看新监控的Windows主机信息

![wKioL1Qmw93SnhVOAAOI_HKVwgw436.jpg](http://s3.51cto.com/wyfs02/M00/4A/BB/wKioL1Qmw93SnhVOAAOI_HKVwgw436.jpg)



2、点击【Services】，查看Windows服务的状态信息

![wKiom1Qmw7awxW6cAAfMmerFYOc506.jpg](http://s3.51cto.com/wyfs02/M00/4A/B9/wKiom1Qmw7awxW6cAAfMmerFYOc506.jpg)



3、等待几分钟后状态就正常了，如下图所示

![wKiom1Qmys7Sl-qRAAVcoYMMwSw515.jpg](http://s3.51cto.com/wyfs02/M02/4A/BC/wKiom1Qmys7Sl-qRAAVcoYMMwSw515.jpg)



**三、NRPE的方式监控windows主机**

1、修改NSClient++的配置文件

![wKioL1QnawbRvD3RAAO2J91L10Q919.jpg](http://s3.51cto.com/wyfs02/M02/4A/EE/wKioL1QnawbRvD3RAAO2J91L10Q919.jpg)

2、重新启动NSClient++服务

![wKioL1Qna8GTPNRKAASz4_IEazg262.jpg](http://s3.51cto.com/wyfs02/M00/4A/EE/wKioL1Qna8GTPNRKAASz4_IEazg262.jpg)

3、Nagios服务端测试NRPE命令



```
```bash
[root@Nagios ~]
```

```bash
# cd /usr/local/nagios/libexec/
```



```bash
[root@Nagios libexec]
```

```bash
# ./check_nrpe -h
```





```bash
NRPE Plugin
```

```bash
for
```

```bash
Nagios
```



```bash
Copyright (c) 1999-2008 Ethan Galstad (nagios@nagios.org)
```



```bash
Version: 2.15
```



```bash
Last Modified: 09-06-2013
```



```bash
License: GPL v2 with exemptions (-l
```

```bash
for
```

```bash
more
```

```bash
info)
```



```bash
SSL
```

```bash
/TLS
```

```bash
Available: Anonymous DH Mode, OpenSSL 0.9.6 or higher required
```





```bash
Usage: check_nrpe -H <host> [ -b <bindaddr> ] [-4] [-6] [-n] [-u] [-p <port>] [-t <timeout>] [-c <
```

```bash
command
```

```bash
>] [-a <arglist...>]
```





```bash
Options:
```



```bash

```

```bash
-n         = Do no use SSL
```



```bash

```

```bash
-u         = Make socket timeouts
```

```bash
return
```

```bash
an UNKNOWN state instead of CRITICAL
```



```bash

```

```bash
<host>     = The address of the host running the NRPE daemon
```



```bash

```

```bash
<bindaddr> = bind to
```

```bash
local
```

```bash
address
```



```bash

```

```bash
-4         = user ipv4 only
```



```bash

```

```bash
-6         = user ipv6 only
```



```bash

```

```bash
[port]     = The port on
```

```bash
which
```

```bash
the daemon is running (default=5666)
```



```bash

```

```bash
[timeout]  = Number of seconds before connection
```

```bash
times
```

```bash
out (default=10)
```



```bash

```

```bash
[
```

```bash
command
```

```bash
]  = The name of the
```

```bash
command
```

```bash
that the remote daemon should run
```



```bash

```

```bash
[arglist]  = Optional arguments that should be passed to the
```

```bash
command
```

```bash
.  Multiple
```



```bash

```

```bash
arguments should be separated by a space.  If provided, this must be
```



```bash

```

```bash
the last option supplied on the
```

```bash
command
```

```bash
line.
```





```bash
Note:
```



```bash
This plugin requires that you have the NRPE daemon running on the remote host.
```



```bash
You must also have configured the daemon to associate a specific plugin
```

```bash
command
```



```bash
with the [
```

```bash
command
```

```bash
] option you are specifying here.  Upon receipt of the
```



```bash
[
```

```bash
command
```

```bash
] argument, the NRPE daemon will run the appropriate plugin
```

```bash
command
```

```bash
and
```



```bash
send the plugin output and
```

```bash
return
```

```bash
code back to *this* plugin.  This allows you
```



```bash
to execute plugins on remote hosts and
```

```bash
'fake'
```

```bash
the results to
```

```bash
make
```

```bash
Nagios think
```



```bash
the plugin is being run locally.
```







```bash
check_nrpe语法：
```



```bash
check_nrpe ... -c <
```

```bash
command
```

```bash
> [-a <argument> <argument> <argument>]
```







```bash
check_nrpe的内置命令：
```



```bash
· CheckAlwaysCRITICAL (check)
```



```bash
· CheckAlwaysOK (check)
```



```bash
· CheckAlwaysWARNING (check)
```



```bash
· CheckCPU (check)
```



```bash
· CheckCRITICAL (check)
```



```bash
· CheckCounter (check)
```



```bash
· CheckEventLog
```

```bash
/CheckEventLog
```

```bash
(check)
```



```bash
· CheckFile (check)
```



```bash
· CheckFileSize (check)
```



```bash
· CheckMem (check)
```



```bash
· CheckMultiple (check)
```



```bash
· CheckOK (check)
```



```bash
· CheckProcState (check)
```



```bash
· CheckServiceState (check)
```



```bash
· CheckTaskSched
```

```bash
/CheckTaskSched
```

```bash
(check)
```



```bash
· CheckUpTime (check)
```



```bash
· CheckVersion (check)
```



```bash
· CheckWARNING (check)
```



```bash
· CheckWMI
```

```bash
/CheckWMI
```

```bash
(check)
```



```bash
· CheckWMIValue (check)
```
```





```
```bash
[root@Nagios libexec]
```

```bash
# ./check_nrpe -H 192.168.1.142 -p 5666 -c CheckCPU -a warn=80 crit=90 time=20m time=10s time=4
```



```bash
OK CPU Load ok.|
```

```bash
'20m'
```

```bash
=0%;80;90
```

```bash
'10s'
```

```bash
=0%;80;90
```

```bash
'4'
```

```bash
=0%;80;90
```



```bash

```
```











