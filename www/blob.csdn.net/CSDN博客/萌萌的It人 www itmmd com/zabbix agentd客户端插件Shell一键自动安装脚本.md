
# zabbix agentd客户端插件Shell一键自动安装脚本 - 萌萌的It人 www.itmmd.com - CSDN博客


2015年04月11日 10:02:31[Jlins](https://me.csdn.net/dyllove98)阅读数：5716


这次生产环境上线了多台Linux服务器，需要全部纳入Zabbix监控范畴，一台一台的去装Zabbix Agentd插件那就太苦逼了，所幸Zabbix客户端插件是支持绿色安装的，就写了个简单的一键安装脚本，然后配合 Secure CRT 的多窗口交互命令一次性就可以搞定了。
![zabbix](http://static.zhangge.net/wp-content/uploads/2015/01/zabbix.png)
正常启动Zabbix客户端服务其实只需要2个文件： zabbix_agentd 和 zabbix_agentd.conf，需要特别说明的是：zabbix_agentd 最好是和 Zabbix_Server 一同编译所得，保证版本和配置文件的路径是一致的，否则可能无法使用Linux系统的 service 服务启动模式。

## 一、准备工作
Zabbix 主机肯定搭建了WEB服务，所以正好可以将所需放置到WEB目录，方便下载。
客户端插件 zabbix_agentd 位于 Zabbix 安装目录下的 sbin 目录，比如：/usr/local/zabbix/sbin/zabbix_agentd
服务控制脚本 zabbix_agentd 位于 zabbix 源码编译目录下的misc/init.d/fedora/core/zabbix_agentd
我们要做的就是将这些文件拷贝到 WEB目录即可，比如 /var/www/html/zabbix_agent/ ，根据系统版本的不同，我们可以准备64和32位的 zabbix_agentd，方便后续不同系统下的安装。
拷贝后，手工验证下文件是否可以下载：
> 客户端插件：http://192.168.1.40/zabbix_agent/64/zabbix_agentd

> 服务控制脚本：http://192.168.1.40/zabbix_agent/init.d/zabbix_agentd

## 二、编写脚本
①、将以下代码保存为 zabbix_agentd.sh ，上传到第一步中的 zabbix_agent 目录。
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
|21
|22
|23
|24
|25
|26
|27
|28
|29
|30
|31
|32
|33
|34
|35
|36
|37
|38
|39
|40
|41
|42
|43
|44
|45
|46
|47
|48
|49
|50
|51
|52
|53
|54
|55
|56
|57
|58
|59
|60
|61
|62
|63
|64
|65
|66
|67
|68
|69
|70
|71
|72
|73
|74
|75
|\#!/bin/bash
|\#Desc: Install Zabbix Agent in OneKey.
|\#Date: 2015-04-02
|\#Author: ZhangGe
|\#设置zabbix服务器地址
|if|[|-|z|$|1|]
|then
|Server|=|"192.168.1.40"
|else
|Server|=|$|1
|fi
|\#检测相关
|function|Check|(|)|{
|netstat|-|ntlp|||grep|zabbix_agentd|>|/|dev|/|null|&&|echo|"Exit for zabbix_agentd has been already installed."|&&|exit
|test|-|f|zabbix_agent|.sh|&&|rm|-|f|zabbix_agent|.sh
|test|-|f|/|usr|/|local|/|zabbix|/|sbin|/|zabbix_agentd|&&|rm|-|rf|/|usr|/|local|/|zabbix|/|sbin|/|zabbix_agentd
|test|-|f|/|etc|/|init|.d|/|zabbix_agentd|&&|rm|-|f|/|etc|/|init|.d|/|zabbix|_|agentd
|}
|\#配置相关
|function|Config|(|)|{
|Server|=|$|1
|\#获取IP
|IP|=|"`ifconfig | grep 192.168 | grep "|inet|addr|:|" | awk -F "|Bcast|:|" '{print $1}' | awk -F "|inet|addr|:|" '{print $2}'`"
|mkdir|/|usr|/|local|/|zabbix|/|sbin|/|-|p
|mkdir|/|usr|/|local|/|zabbix|/|etc|/|-|p
|cd|/|usr|/|local|/|zabbix|/|sbin|/
|\#写入配置文件
|cat|>|/|usr|/|local|/|zabbix|/|etc|/|zabbix_agentd|.conf|<<|EOF
|Server|=|$|{|Server|}
|Hostname|=|$|{|IP|}
|BufferSize|=|1024
|DebugLevel|=|2
|LogFileSize|=|1024
|EnableRemoteCommands|=|1
|\#Include=/etc/zabbix/zabbix_command.conf
|EOF
|}
|function|Install|(|)|{
|Config|$|1
|\#下载服务控制脚本
|wget|-|P|/|etc|/|init|.d|/|http|:|/|/|192.168.1.40|/|zabbix_agent|/|init|.d|/|zabbix_agentd
|\#判断系统位数，下载不同版本
|if|[|[|"$(uname -m)"|==|"x86_64"|]|]
|then
|wget|http|:|/|/|192.168.1.40|/|zabbix_agent|/|64|/|zabbix_agentd
|else
|wget|http|:|/|/|192.168.1.40|/|zabbix_agent|/|32|/|zabbix_agentd
|fi
|chmod|+|x|/|etc|/|init|.d|/|zabbix_agentd
|chmod|+|x|/|usr|/|local|/|zabbix|/|sbin|/|zabbix_agentd
|\#新增用户和用户组
|groupadd|zabbix
|useradd|-|s|/|sbin|/|nologin|-|M|-|g|zabbix|zabbix
|\#启动服务并加入开启启动
|service|zabbix_agentd|start|&&|chkconfig|zabbix_agentd|on
|echo|--|--|--|--|--|--|--|--|--|--|-|Result|--|--|--|--|--|--|--|--|--|--|--|--|--|--|--|--|-
|\#检查安装结果
|netstat|-|ntlp|||grep|zabbix_agentd|&&|echo|-|e|"\033[33minstall Succeed.\033[0m"||||echo|-|e|"\033[31minstall Failed.\033[0m"
|}
|\#main
|Check
|Install|$Server
|

②、Service 服务控制脚本
为了方便没找到 zabbix agent 服务控制脚本的朋友，额外提供服务控制代码。将代码保存为zabbix_agentd，上传到第一步的 zabbixz_agent/init.d/ 目录备用。
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
|21
|22
|23
|24
|25
|26
|27
|28
|29
|30
|31
|32
|33
|34
|35
|36
|37
|38
|39
|40
|41
|42
|43
|44
|45
|46
|47
|48
|49
|50
|51
|52
|53
|54
|55
|56
|57
|\#!/bin/sh
|\#chkconfig:
 345 95 95
|\#description:Zabbix
 agent
|\#
 Zabbix
|\#
 Copyright (C) 2001-2013 Zabbix SIA
|\#
|\#
 This program is free software; you can redistribute it and/or modify
|\#
 it under the terms of the GNU General Public License as published by
|\#
 the Free Software Foundation; either version 2 of the License, or
|\#
 (at your option) any later version.
|\#
|\#
 This program is distributed in the hope that it will be useful,
|\#
 but WITHOUT ANY WARRANTY; without even the implied warranty of
|\#
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
|\#
 GNU General Public License for more details.
|\#
|\#
 You should have received a copy of the GNU General Public License
|\#
 along with this program; if not, write to the Free Software
|\#
 Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
|\#
 Start/Stop the Zabbix agent daemon.
|\#
 Place a startup script in /sbin/init.d, and link to it from /sbin/rc[023].d
|SERVICE|=|"Zabbix
 agent"
|DAEMON|=|/|usr|/|local|/|zabbix|/|sbin|/|zabbix_agentd
|PIDFILE|=|/|tmp|/|zabbix_agentd|.pid
|BASEDIR|=|/|usr|/|local|/|zabbix
|ZABBIX_AGENTD|=|$BASEDIR|/|sbin|/|zabbix_agentd
|case|$|1|in
|'start'|)
|if|[|-|x|$|{|DAEMON|}|]
|then
|$DAEMON
|\#
 Error checking here would be good...
|echo|"${SERVICE}
 started."
|else
|echo|"Can't
 find file ${DAEMON}."
|echo|"${SERVICE}
 NOT started."
|fi
|;|;
|'stop'|)
|if|[|-|s|$|{|PIDFILE|}|]
|then
|if|kill|`|cat|$|{|PIDFILE|}|`|>|/|dev|/|null|2|>|&|1
|then
|echo|"${SERVICE}
 stoped."
|rm|-|f|$|{|PIDFILE|}
|fi
|fi
|;|;
|'restart'|)
|$|0|stop
|sleep|10
|$|0|start
|;|;
|*|)
|echo|"Usage:
 $0 start|stop|restart"
|;|;
|esac
|


## 三、使用方法
登录到客户端系统，运行如下命令即可一键安装：
①、使用默认 zabbix_server 的IP地址：
|1
|wget|http|:|/|/|192.168.1.40|/|zabbix_agent|/|zabbix_agent|.sh|&&|chmod|+|x|zabbix_agent|.sh|&&|.|/|zabbix_agent|.sh
|

②、后面添加IP参数可指定到其他 zabbix_server 或 zabbix_proxy：
|1
|wget|http|:|/|/|192.168.1.40|/|zabbix_agent|/|zabbix_agent|.sh|&&|chmod|+|x|zabbix_agent|.sh|&&|.|/|zabbix_agent|.sh|192.168.1.41
|

![zabbix_agentd](http://static.zhangge.net/wp-content/uploads/2015/04/zabbix_agentd.png)
Secure CRT多会话交互执行：
![zabbix_agentd1](http://static.zhangge.net/wp-content/uploads/2015/04/zabbix_agentd1.png)
其他说明：此脚本中的 zabbix_agentd 编译路径（prefix）为 /usr/local/zabbix，如果编译的时候不是这个路径，则需要根据实际情况修改脚本里面相关路径，否则注册的zabbix_agentd服务将无法启动，就只能通过命令行启动了！

参考资料：



## [jquery
 ajax 设置json类型的返回串问题](http://www.itmmd.com/201504/694.html)
## http://www.itmmd.com/201504/694.html
## [java
 jsp实现万年历 + 日程管理，已经设计好u](http://www.itmmd.com/201504/693.html)
## http://www.itmmd.com/201504/693.html

## [jquery
 动态添加和删除 ul li列表](http://www.itmmd.com/201504/692.html)
## http://www.itmmd.com/201504/692.html
更多信息请访问：[萌萌的it人](http://www.itmmd.com/)



