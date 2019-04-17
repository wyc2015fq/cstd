# Nagios监控Windows的网卡流量 - DoubleLi - 博客园






**Nagios监控Windows的网卡流量**

使用/usr/local/nagios/libexec/中的check_traffic.sh，不但可以监控Linux的网卡流量，也可以监控Windows服务器的流量。

# 1 Check_traffic.sh用法用法


|[root@oracle libexec]#/usr/local/nagios/libexec/check_traffic.sh -h|
|----|
|Usage:./check_traffic.sh [ -v ] [ -6 ] [ -r ] -V 1|2c|3 -C snmp-community -H host [ -L ] -I interface -w in, out-warning-value  -c in, out-critical-value -K/M -B/b|
|Example:./check_traffic.sh -V 2c -C public -H 127.0.0.1 -I 4 -w 200,100 -c 300,200 -K -B|
|We can use -r to use Range Value Options:Example:./check_traffic.sh -V 2c -C public -H 127.0.0.1 -I 4 -r -w 200-300,100-200 -c 100-400,50-250 -K -BIf you don't use -K/M -B/b options, default -K -b, corresponding to Kbps|
|Make sure that the check interval greater than 5 Seconds.Or modify the Min_Interval var in this file Line 180.And, if you want in Verbose mode, use -v, to check the debug messages in the file /tmp/check_traffic.$$.Or use ./check_traffic.sh [ -v ] -V 1|2c|3 -C snmp-community -H host -LTo list all interfaces on specify host.|



# 2 Windows客户端的设置

在此例中使用windows 2008举例，IP为192.168.19.142。











## 2.1 安装SNMP服务 

Windows 2008中是通过添加feature来添加snmp服务的，详细过程请查看附件文档。 

而Windows2003是在控制面板-添加删除程序-添加删除组件-Management and Monitoring Tools中安装snmp服务的。

## 2.2 配置SNMP服务

使用services.msc启动服务控制台，设置snmp community的traps和security，如下：(详细配置请查看附件文档)



然后重启snmp服务，可以使用cmdànetstat –an | find “161”验证snmp服务启动情况

# 3 Nagios服务器的设置

## 3.1 安装net-snmp的相关组件

         # yum install –y net-snmp*

## 3.2 编辑command.cfg文件

vi /usr/local/nagios/etc/object/command.cfg添加如下内容：


|define command{      command_name    check_bandwidth      command_line    $USER1$/check_traffic.sh -V 2c -C snmp-community -H $HOSTADDRESS$ -I $ARG1$ -w $ARG2$,$ARG3$ -c $ARG4$,$ARG5$        }注：-C后参数为在服务器上设置的community值；-I 为大写的i，为网卡接口的index值-w与-c后的两个参数分别为in/out的流量阈值|
|----|



## 3.3 编辑windows.cfg主机配置文件

首先使用如下的命令查看被监控的windows主机的网卡信息：

/usr/local/nagios/libexec/check_traffic.sh -V 2c -C snmp-community -H IPADD -L

然后会列出被监控机的网卡信息，查看需要监控的网卡，记录其index值即可。

如上例中所示中命令为：

/usr/local/nagios/libexec/check_traffic.sh -V 2c -C public1 -H 192.168.19.142 –L如下图所示：(图片在附件文档中)



vi windows.cfg添加如下内容：


|define service{        use                     generic-service        host_name               winserver        service_description        Traffic        check_command          check_bandwidth!10!200000!300000!400000!500000        }此处的10为网卡信息的index值，此值通过如上的命令查看：|
|----|



然后重启nagios服务即可。



若安装有PNP的话还可以查看波形图的流量监控。









