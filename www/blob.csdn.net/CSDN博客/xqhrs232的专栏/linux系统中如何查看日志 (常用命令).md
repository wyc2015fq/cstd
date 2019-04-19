# linux系统中如何查看日志 (常用命令) - xqhrs232的专栏 - CSDN博客
2016年04月08日 14:55:57[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：390
原文地址::[http://os.51cto.com/art/201408/448248.htm](http://os.51cto.com/art/201408/448248.htm)
相关文章
1、Linux系统中查看日志的常用命令----[http://wenku.baidu.com/link?url=AruSD5ulHLLVVTXoULFAT9lj-gzO498ZcjxyywTgQFAKPefsELSTJD2PQb9RUKjxBPZl97_NbVWZaTgpwo2ViXrlyb-BlDXo91jqo-mINJy](http://wenku.baidu.com/link?url=AruSD5ulHLLVVTXoULFAT9lj-gzO498ZcjxyywTgQFAKPefsELSTJD2PQb9RUKjxBPZl97_NbVWZaTgpwo2ViXrlyb-BlDXo91jqo-mINJy)
2、[linux系统中如何查看日志 (常用命令)](http://mushme.iteye.com/blog/1001478)----[http://mushme.iteye.com/blog/1001478](http://mushme.iteye.com/blog/1001478)
cat
tail -f
日 志 文 件 说 明
- /var/log/message 系统启动后的信息和错误日志，是Red Hat Linux中最常用的日志之一  
- /var/log/secure 与安全相关的日志信息  
- /var/log/maillog 与邮件相关的日志信息  
- /var/log/cron 与定时任务相关的日志信息  
- /var/log/spooler 与UUCP和news设备相关的日志信息  
- /var/log/boot.log 守护进程启动和停止相关的日志消息 
系统：
- # uname -a # 查看内核/操作系统/CPU信息  
- # cat /etc/issue  
- # cat /etc/redhat-release # 查看操作系统版本  
- # cat /proc/cpuinfo # 查看CPU信息  
- # hostname # 查看计算机名  
- # lspci -tv # 列出所有PCI设备  
- # lsusb -tv # 列出所有USB设备  
- # lsmod # 列出加载的内核模块  
- # env # 查看环境变量 
资源：
- # free -m # 查看内存使用量和交换区使用量  
- # df -h # 查看各分区使用情况  
- # du -sh <目录名> # 查看指定目录的大小  
- # grep MemTotal /proc/meminfo # 查看内存总量  
- # grep MemFree /proc/meminfo # 查看空闲内存量  
- # uptime # 查看系统运行时间、用户数、负载  
- # cat /proc/loadavg # 查看系统负载 
磁盘和分区：
- # mount | column -t # 查看挂接的分区状态  
- # fdisk -l # 查看所有分区  
- # swapon -s # 查看所有交换分区  
- # hdparm -i /dev/hda # 查看磁盘参数(仅适用于IDE设备)  
- # dmesg | grep IDE # 查看启动时IDE设备检测状况  
网络：
- # ifconfig # 查看所有网络接口的属性  
- # iptables -L # 查看防火墙设置  
- # route -n # 查看路由表  
- # netstat -lntp # 查看所有监听端口  
- # netstat -antp # 查看所有已经建立的连接  
- # netstat -s # 查看网络统计信息 
进程：
- # ps -ef # 查看所有进程  
- # top # 实时显示进程状态（另一篇文章里面有详细的介绍） 
用户：
- # w # 查看活动用户  
- # id <用户名> # 查看指定用户信息  
- # last # 查看用户登录日志  
- # cut -d: -f1 /etc/passwd # 查看系统所有用户  
- # cut -d: -f1 /etc/group # 查看系统所有组  
- # crontab -l # 查看当前用户的计划任务 
服务：
- # chkconfig –list # 列出所有系统服务  
- # chkconfig –list | grep on # 列出所有启动的系统服务 
程序：
- # rpm -qa # 查看所有安装的软件包 
原文链接：[http://mushme.iteye.com/blog/1001478](http://mushme.iteye.com/blog/1001478)

//==============================================================================================================================
备注::
1>启动的文件日志在 /var/log/boot.log 
