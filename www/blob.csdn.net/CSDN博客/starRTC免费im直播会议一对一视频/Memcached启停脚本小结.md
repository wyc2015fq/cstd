# Memcached启停脚本小结 - starRTC免费im直播会议一对一视频 - CSDN博客
2016年08月26日 18:12:44[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：1088
## 编写配置文件
**vim****/**etc**/**memcached.conf
内容如下：
-m128-d-p11211-c1024
## 编写启动脚本
vim /etc/rc.d/init.d/memcached
*#!/bin/bash**#**# memcached This shell script takes care of starting and stopping**# standalone memcached.**#*
. **/**etc**/**rc.d**/**init.d**/**functions
PATH=**/**usr**/**local**/**sbin:**/**usr**/**local**/**bin:**/**sbin:**/**bin:**/**usr**/**sbin:**/**usr**/**bin
DAEMON=**/**usr**/**local**/**bin**/**memcached
DAEMONBOOTSTRAP=**/**usr**/**local**/**bin**/**start-memcached
DAEMONCONF=**/**etc**/**memcached.conf
NAME=memcached
DESC=memcached
PIDFILE=**/**var**/**run**/**$NAME.pid
**[**-x$DAEMON**]****||****exit**0**[**-x$DAEMONBOOTSTRAP**]****||****exit**0RETVAL=0
start**(****)****{****echo**-n $"Starting $DESC: "
daemon $DAEMONBOOTSTRAP$DAEMONCONFRETVAL=$?**[**$RETVAL-eq0**]****&&****touch**$PIDFILE**echo****return**$RETVAL**}**
stop**(****)****{****echo**-n $"Shutting down $DESC: "
killproc $NAMERETVAL=$?**echo****[**$RETVAL-eq0**]****&&****rm**-f$PIDFILE**return**$RETVAL**}***# See how we were called.***case**"$1"**in**
start**)**
start
**;;**
stop**)**
stop
**;;**
restart**|**reload**)**
stop
start
RETVAL=$?**;;**
status**)**
status $progRETVAL=$?**;;*********)****echo** $"Usage: $0 {start|stop|restart|status}"**exit**1**esac****exit**$RETVAL
## [[编辑](http://192.168.1.100/elesos_com/index.php?title=Memcached%E5%90%AF%E5%81%9C%E8%84%9A%E6%9C%AC%E5%B0%8F%E7%BB%93&action=edit&section=3)]编写start-memcached脚本
vim /usr/local/bin/start-memcached
*#!/usr/bin/perl -w**# start-memcached*
use strict;
**if****(**$**>****!**= 0 and $**<****!**= 0**)****{**
print STDERR "Only root wants to run start-memcached.**\n**";
**exit**;
**}**
my $etcfile = **shift****||**"/etc/memcached.conf";
my $params = **[****]**;
my $etchandle;
*# This script assumes that memcached is located at /usr/bin/memcached, and**# that the pidfile is writable at /var/run/memcached.pid*
my $memcached = "/usr/local/bin/memcached";
my $pidfile = "/var/run/memcached.pid";
*# If we don't get a valid logfile parameter in the /etc/memcached.conf file,**# we'll just throw away all of our in-daemon output. We need to re-tie it so**# that non-bash shells will not hang on logout. Thanks to Michael Renner for**# the tip*
my $fd_reopened = "/dev/null";
sub handle_logfile **{**
my **(**$logfile**)** = **@**_;
$fd_reopened = $logfile;
**}**
sub reopen_logfile **{**
my **(**$logfile**)** = **@**_;
open *****STDERR, ">>$logfile";
open *****STDOUT, ">>$logfile";
open *****STDIN, ">>/dev/null";
$fd_reopened = $logfile;
**}***# This is set up in place here to support other non -[a-z] directives*
my $conf_directives = **{**"logfile" =**>** \**&**handle_logfile
**}**;
**if****(**open $etchandle, $etcfile**)****{**
foreach my $line**(****<**$etchandle**>****)****{**$line =~ s**/**\*#.*//go;*$line = **join**' ', **split**' ', $line;
next unless $line;
next **if**$line =~ **/**^\-**[****dh****]****/**o;
**if****(**$line =~ **/**^**[**^\-**]****/**o**)****{**
my **(**$directive, $arg**)** = $line =~ **/**^**(**.*****?**)**\s+**(**.*******)****/**;
$conf_directives-**>****{**$directive**}**-**>****(**$arg**)**;
next;
**}**
push **@**$params, $line;
**}****}**
unshift **@**$params, "-u root" unless **(****grep**$_ eq '-u', **@**$params**)**;
$params = **join**" ", **@**$params;
**if****(**-e $pidfile**)****{**
open PIDHANDLE, "$pidfile";
my $localpid = **<**PIDHANDLE**>**;
close PIDHANDLE;
chomp $localpid;
**if****(**-d "/proc/$localpid"**)****{**
print STDERR "memcached is already running.**\n**";
**exit**;
**}****else****{****`****rm**-f$localpid**`**;
**}****}**
my $pid = fork**(****)**;
**if****(**$pid == 0**)****{**
reopen_logfile**(**$fd_reopened**)**;
**exec**"$memcached$params";
**exit****(**0**)**;
**}** elsif **(**open PIDHANDLE,">$pidfile"**)****{**
print PIDHANDLE $pid;
close PIDHANDLE;
**}****else****{**
print STDERR "Can't write pidfile to $pidfile.**\n**";
**}**
## 修改权限
chmod 755 /etc/rc.d/init.d/memcached
chmod 755 /usr/local/bin/start-memcached
## 启动
service memcached start
## [[编辑](http://192.168.1.100/elesos_com/index.php?title=Memcached%E5%90%AF%E5%81%9C%E8%84%9A%E6%9C%AC%E5%B0%8F%E7%BB%93&action=edit&section=6)]艺搜参考
[http://forums.eukhost.com/f15/memcached-best-caching-option-linux-web-servers-5269/](http://forums.eukhost.com/f15/memcached-best-caching-option-linux-web-servers-5269/)
[http://duntuk.com/how-install-memcached-centos-memcached-php-extension-centos](http://duntuk.com/how-install-memcached-centos-memcached-php-extension-centos)
[http://complitech.net/steps-for-install-memcached-on-fedoara-linux-platform-and-create-service-for-memcached/](http://complitech.net/steps-for-install-memcached-on-fedoara-linux-platform-and-create-service-for-memcached/)
