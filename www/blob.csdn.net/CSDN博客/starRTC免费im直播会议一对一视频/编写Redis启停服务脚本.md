# 编写Redis启停服务脚本 - starRTC免费im直播会议一对一视频 - CSDN博客
2016年10月08日 10:06:22[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：505
脚本内容如下;
*#!/bin/bash**#**# redis - this script starts and stops the redis-server daemon**#**# chkconfig:   - 80 12**# description:  Redis is a persistent key-value database**# processname: redis-server**# config:      /etc/redis/redis.conf**# pidfile:     /var/run/redis.pid***source****/**etc**/**init.d**/**functions
 
BIN="/usr/local/bin"CONFIG="/etc/redis/redis.conf"PIDFILE="/var/run/redis.pid"*### Read configuration***[**-r"$SYSCONFIG"**]****&&****source**"$SYSCONFIG"RETVAL=0prog="redis-server"desc="Redis Server"
 
start**(****)****{****if****[**-e$PIDFILE**]**;**then****echo**"$desc already running...."**exit**1**fi****echo**-n $"Starting $desc: "
        daemon $BIN**/**$prog$CONFIGRETVAL=$?**echo****[**$RETVAL-eq0**]****&&****touch****/**var**/**lock**/**subsys**/**$prog**return**$RETVAL**}**
 
stop**(****)****{****echo**-n $"Stop $desc: "
        killproc $progRETVAL=$?**echo****[**$RETVAL-eq0**]****&&****rm**-f**/**var**/**lock**/**subsys**/**$prog$PIDFILE**return**$RETVAL**}**
 
restart**(****)****{**
    stop
    start
**}****case**"$1"**in**
  start**)**
        start
        **;;**
  stop**)**
        stop
        **;;**
  restart**)**
        restart
        **;;**
  condrestart**)****[**-e**/**var**/**lock**/**subsys**/**$prog**]****&&** restart
        RETVAL=$?**;;**
  status**)**
        status $progRETVAL=$?**;;*********)****echo** $"Usage: $0 {start|stop|restart|condrestart|status}"RETVAL=1**esac****exit**$RETVAL
下载脚本：[艺搜下载](http://dl.vmall.com/c0h5mt5d7i)
将下载下来的脚本放在/etc/init.d/目录下
- 更改脚本权限
chmod 777 /etc/init.d/redis
- 测试
service redis start
参见[编写Nginx启停服务脚本](http://192.168.1.100/elesos_com/index.php?title=%E7%BC%96%E5%86%99Nginx%E5%90%AF%E5%81%9C%E6%9C%8D%E5%8A%A1%E8%84%9A%E6%9C%AC&action=edit&redlink=1)
