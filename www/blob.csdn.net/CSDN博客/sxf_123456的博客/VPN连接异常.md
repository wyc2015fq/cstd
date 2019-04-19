# VPN连接异常 - sxf_123456的博客 - CSDN博客
2018年05月07日 14:31:42[sxf_0123](https://me.csdn.net/sxf_123456)阅读数：616标签：[vpn](https://so.csdn.net/so/search/s.do?q=vpn&t=blog)
个人分类：[VPN](https://blog.csdn.net/sxf_123456/article/category/7645764)
各个平台下OpenVPN连接log所在的位置：
Windows下：如果使用OpenVPN GUI进行连接，默认的log会存放在OpenVPN安装目录下log文件夹。
Linux下：在OpenVPN配置文件中添加一行：log-append /path/to/logfile，连接后/path/to/logfile即为连接log。
Mac OS下：tunnelblick里，Details菜单（感谢@tianyi提供信息:-)）
Android下：tunneldroid里，按menu查看log。
OpenVPN的log文件为普通文本文件，可以用各种文本编辑器，例如记事本、写字板、VIM等工具打开。一般情况下为了方便查看问题所在，建议在OpenVPN的配置文件中添加verb 5，可以在log里输出更多有效信息。
正常连接时，log的最后一行是：
Fri Mar 5 09:58:04 2010 Initialization Sequence Completed
这表明OpenVPN成功建立了连接并成功修改了路由表，已经可以正常使用VPN。
问题1：
Mon Mar 01 21:42:13 2010 us=78000 CreateFile failed on TAP device: \\.\Global\{3D3B7076-EC34-48DE-82A4-B34956532951}.tap
Mon Mar 01 21:42:13 2010 us=78000 All TAP-Win32 adapters on this system are currently in use.
Mon Mar 01 21:42:13 2010 us=78000 Exiting
解答：这个问题仅在Windows上出现。原因未知，怀疑与防火墙软件有关。通常情况下可以通过重启系统解决。如果没有解决的话，可以到OpenVPN安装目录下bin文件夹，依次执行delalltap.bat和addtap.bat，即可解决。注意，如果使用的是Vista/Win7/~~Win2003~~Win2008/Win10这样的带UAC的Windows系统，需要用管理员权限执行这两个脚本。
问题2：
Thu Dec 03 22:46:07 2009 us=651000 C:\WINDOWS\system32\route.exe ADD 128.0.0.0 MASK 128.0.0.0 10.7.0.1
Thu Dec 03 22:46:07 2009 us=667000 ROUTE: route addition failed using CreateIpForwardEntry: 拒绝访问。 [status=5 if_index=14]
Thu Dec 03 22:46:07 2009 us=667000 Route addition via IPAPI failed [adaptive]
Thu Dec 03 22:46:07 2009 us=667000 Route addition fallback to route.exe
Thu Dec 03 22:46:07 2009 us=667000 ERROR: Windows route add command failed [adaptive]: returned error code 1
解答：这是在Vista/Win7/~~Win2003~~Win2008等系统中没有用管理员权限安装及启动OpenVPN GUI造成的，OpenVPN进程没有相应权限修改系统路由表。解决方法是重新用管理员权限安装OpenVPN，并在启动OpenVPN GUI时右键选择使用管理员权限打开。
问题3：
Mon Feb 1 18:44:52 2010 us=241882 /system/xbin/bb/route add -net 128.0.0.0 netmask 128.0.0.0 gw 10.7.0.1
Mon Feb 1 18:44:52 2010 us=244293 ERROR: Linux route add command failed: could not execute external program
Mon Feb 1 18:44:52 2010 us=244628 Initialization Sequence Completed
解答：这是Android下，Cyanogen Mod和OpenEclair两个ROM，曾经有段时间使用了错误的OpenVPN二进制文件，导致的无法调用busybox添加ip和路由。具体请参考Android上OpenVPN的设置与使用这篇日志，重新安装OpenVPN二进制文件，相应地修复busybox
问题4：
Sat Mar 06 16:31:42 2010 us=107000 There are no TAP-Win32 adapters on this system. You should be able to create a TAP-Win32 adapter by going to Start -> All Programs -> OpenVPN -> Add a new TAP-Win32 virtual ethernet adapter.
解答：这个问题在Windows上发生，原因未知。解决方案如log所述：开始->所有程序->OpenVPN->Add a new TAP-Win32 virtual ethernet adapter。如果是Vista/Win7/Win10，用管理员权限执行
