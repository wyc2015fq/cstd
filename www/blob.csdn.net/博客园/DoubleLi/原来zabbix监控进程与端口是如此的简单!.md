# 原来zabbix监控进程与端口是如此的简单! - DoubleLi - 博客园






## 使用zabbix自带key监控进程与端口

每个公司都有自己的程序，自己的进程名与端口监听，对于nagios来说，这些都经常需要自己去写插件，但是zabbix不需要，它自己就有监控进程与端口的key。

在使用zabbix的key来监控进程与端口前，我也自己写了插件，但用起来很不爽，因为需要在所有agent上都要进行配置与维护。如果用zabbix自带的功能，只需要在server端维护就可以了。简直就是操十分心与操一分心的区别啊，为什么我就没有早点发现这功能呢。



开始正题......

#### 1、监控端口

监控端口使用如下key:

key：net.tcp.listen[port]

Checks if this port is in LISTEN state. 0 - it is not, 1 - it is inLISTEN state.

翻译：监听端口状态，返回结果为1，则运行；返回结果为0，则没有运行。

例如监控443端口，net.tcp.listen[443]

![wKiom1QH7NvhGfSIAAF7wgU_Y7M306.jpg](http://s3.51cto.com/wyfs02/M00/48/6B/wKiom1QH7NvhGfSIAAF7wgU_Y7M306.jpg)



#### 2、监控进程

如果一个进程没有监听端口，则可以使用如下key。

key：proc.num[<name>,<user>,<state>,<cmdline>]

Number of processes. <name> and <user> same as inproc.mem item. <state> all (default), run, sleep, zomb. <cmdline>filter by command line (supports regex).

name:是进程名

user：运行该进程的用户

state：进程状态包括run,sleep,zomb

cmdline：该项内容是模糊匹配，即用ps aux看到的最后一列中包含该字段则匹配上。

注:name匹配可能不是很准，一般根据cmdline来匹配进程



例如监控cond，proc.num[,root,all,crond]

配置如下：

![wKioL1QH7ODyJfXdAAIEsYjNt-g353.jpg](http://s3.51cto.com/wyfs02/M01/48/6C/wKioL1QH7ODyJfXdAAIEsYjNt-g353.jpg)



#### 3、触发器配置

对于端口的监控，正常则返回值=1；对于进程的监控，正常则返回值>=1；如果返回结果为0，则代表该进程宕掉。

例如对443端口的触发器配置如下：

如最后两次的结果都等于0时，则报警。对于进程的触发器配置，是一样的。

![wKiom1QH7NvQ9oMtAADok345-Jo880.jpg](http://s3.51cto.com/wyfs02/M00/48/6B/wKiom1QH7NvQ9oMtAADok345-Jo880.jpg)





zabbix对监控进程与端口存活状态实在太方便了。最近我在自学zabbix,慢慢的探索更多好用的功能，欢迎留言交流。









