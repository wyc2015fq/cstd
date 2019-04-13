
# Systemd基础篇：systemd vs SysVinit - 知行合一 止于至善 - CSDN博客

2019年04月07日 21:03:48[liumiaocn](https://me.csdn.net/liumiaocn)阅读数：78标签：[sysv																](https://so.csdn.net/so/search/s.do?q=sysv&t=blog)[init																](https://so.csdn.net/so/search/s.do?q=init&t=blog)[systemd																](https://so.csdn.net/so/search/s.do?q=systemd&t=blog)[chkconfig																](https://so.csdn.net/so/search/s.do?q=chkconfig&t=blog)[systemctl																](https://so.csdn.net/so/search/s.do?q=systemctl&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=chkconfig&t=blog)[
																					](https://so.csdn.net/so/search/s.do?q=systemd&t=blog)个人分类：[Unix/Linux																](https://blog.csdn.net/liumiaocn/article/category/6328278)
[
																								](https://so.csdn.net/so/search/s.do?q=systemd&t=blog)
[
				](https://so.csdn.net/so/search/s.do?q=init&t=blog)
[
			](https://so.csdn.net/so/search/s.do?q=init&t=blog)
[
		](https://so.csdn.net/so/search/s.do?q=sysv&t=blog)

![在这里插入图片描述](https://img-blog.csdnimg.cn/20190408194243881.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saXVtaWFvY24uYmxvZy5jc2RuLm5ldA==,size_16,color_FFFFFF,t_70)
Systemd已经基本取代了SysV的Init，这篇文章从几个方面整理一下Systemd与Init的使用上的区别。
[
](https://img-blog.csdnimg.cn/20190408194243881.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saXVtaWFvY24uYmxvZy5jc2RuLm5ldA==,size_16,color_FFFFFF,t_70)
# 命令比较: SysVinit vs Systemd
[
](https://img-blog.csdnimg.cn/20190408194243881.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saXVtaWFvY24uYmxvZy5jc2RuLm5ldA==,size_16,color_FFFFFF,t_70)|命令用途|SysVInit命令|Systemd命令|
|---|---|---|
|服务启动|service 服务名 start|systemd start 服务名.service (.service可省略，后同）|
|服务停止|service 服务名 stop|systemctl stop 服务名|
|服务重启|service 服务名 restart|systemctl restart 服务名|
|服务重新加载|service 服务名 reload|systemctl reload 服务名|
|服务状态确认|service 服务名 status|systemctl status 服务名|
|服务开机启动设定|chkconfig 服务名 on|systemctl enable 服务名|
|取消服务开机启动设定|chkconfig service off|systemctl disable 服务名|
|确认服务开机启动设定状态|chkconfig 服务名|systemctl is-enabled 服务名|
|加载服务配置文件|chkconfig 服务名 -add|systemctl deamon-reload|
|关机|halt|systemctl halt|
|关机（电源）|poweroff|systemctl poweroff|
|重启|reboot|systemctl reboot|
|休眠|pm-hibernate|systemctl hibernate|
|挂起|pm-suspend|systemctl suspend|
# RunLevel: SysVinit vs Systemd
[
](https://img-blog.csdnimg.cn/20190408194243881.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saXVtaWFvY24uYmxvZy5jc2RuLm5ldA==,size_16,color_FFFFFF,t_70)|RunLevel|SysVInit|Systemd|
|---|---|---|
|System halt|0|runlevel0.target, poweroff.target|
|Single user mode|1|runlevel1.target, rescure.target|
|Multi user|2|runlevel02target, multi-user.target|
|Multi user with network|3|runlevel3.target, multi-user.target|
|Experimental|4|runlevel4.target, multi-user.target|
|Multi user with network, graphical mode|5|runlevel5.target, graphical.target|
|Reboot|6|runlevel6.target, reboot.target|
# 日志确认方式
[
](https://img-blog.csdnimg.cn/20190408194243881.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saXVtaWFvY24uYmxvZy5jc2RuLm5ldA==,size_16,color_FFFFFF,t_70)
## SysVinit方式
[
](https://img-blog.csdnimg.cn/20190408194243881.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saXVtaWFvY24uYmxvZy5jc2RuLm5ldA==,size_16,color_FFFFFF,t_70)确认系统日志文件
[
](https://img-blog.csdnimg.cn/20190408194243881.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saXVtaWFvY24uYmxvZy5jc2RuLm5ldA==,size_16,color_FFFFFF,t_70)
> 文件名： /var/log/message

> 文件名：/var/log/syslog

> Systemd

> 确认系统日志信息

> 使用命令： journalctl -f

> 确认某一时间点之后的日志信息

> 使用命令：journalctl -since=xxx

> Systemd特殊命令

> 确认启动时间

> 使用命令：systemd-analyze 或者 systemd-analyze time

> [
> root@host ~
> ]
> \# systemd-analyze
> Startup finished
> in
> 1.638s
> (
> kernel
> )
> + 1.951s
> (
> initrd
> )
> + 14.177s
> (
> userspace
> )
> =
> 17.767s
> [
> root@host ~
> ]
> \#
> [
> root@host ~
> ]
> \# systemd-analyze time
> Startup finished
> in
> 1.638s
> (
> kernel
> )
> + 1.951s
> (
> initrd
> )
> + 14.177s
> (
> userspace
> )
> =
> 17.767s
> [
> root@host ~
> ]
> \#
> 停止服务相关进程

> 使用命令：systemctl kill 服务名

> hostname设定

> 使用命令：hostnamectl

> [
> root@host shell
> ]
> \# hostnamectl
> Static hostname: host.localdomain
         Icon name: computer-vm
           Chassis: vm
        Machine ID: d27a659b15fc379d24204584d5c051bd
           Boot ID: 3fe84ed9506248b9a41c492ca543748a
    Virtualization: kvm
  Operating System: CentOS Linux 7
> (
> Core
> )
> CPE OS Name: cpe:/o:centos:centos:7
            Kernel: Linux 4.10.4-1.el7.elrepo.x86_64
      Architecture: x86-64
> [
> root@host shell
> ]
> \#
> [
> root@host shell
> ]
> \#
> [
> root@host shell
> ]
> \# hostnamectl set-hostname liumiaocn
> [
> root@host shell
> ]
> \#
> 再次登录之后，hostname的变化即可看到

> [
> root@liumiaocn ~
> ]
> \#
> 时间设定

> 使用命令： timedatectl

> 注：使用方法可参看：

> [https://liumiaocn.blog.csdn.net/article/details/88408155](https://liumiaocn.blog.csdn.net/article/details/88408155)

> 管理login：loginctrl

> 使用例：查看当前登录用户的Session状况

> [
> root@liumiaocn ~
> ]
> \# loginctl
> SESSION        UID USER             SEAT            
        10          0 root                             
1 sessions listed.
> [
> root@liumiaocn ~
> ]
> \#
> 管理locale：localectl

> 使用例：查看当前本地化设定信息

> [
> root@liumiaocn ~
> ]
> \# localectl
> System Locale: LANG
> =
> en_US.UTF-8
       VC Keymap: us
      X11 Layout: us
> [
> root@liumiaocn ~
> ]
> \#
[
						](https://img-blog.csdnimg.cn/20190408194243881.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saXVtaWFvY24uYmxvZy5jc2RuLm5ldA==,size_16,color_FFFFFF,t_70)
[
	](https://img-blog.csdnimg.cn/20190408194243881.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saXVtaWFvY24uYmxvZy5jc2RuLm5ldA==,size_16,color_FFFFFF,t_70)
