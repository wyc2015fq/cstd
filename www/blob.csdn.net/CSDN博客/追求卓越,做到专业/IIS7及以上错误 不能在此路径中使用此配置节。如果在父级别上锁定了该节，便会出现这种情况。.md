
# IIS7及以上错误:不能在此路径中使用此配置节。如果在父级别上锁定了该节，便会出现这种情况。 - 追求卓越,做到专业 - CSDN博客


2016年01月20日 16:16:39[Waldenz](https://me.csdn.net/enter89)阅读数：13308


IIS 7访问的时候出现以下异常：
不能在此路径中使用此配置节。如果在父级别上锁定了该节，便会出现这种情况。锁定是默认设置的(overrideModeDefault="Deny")......

解决方案：

因为 IIS 7 采用了更安全的 web.config 管理机制，默认情况下会锁住配置项不允许更改。
运行命令行
C:\windows\system32\inetsrv\appcmd unlock config -section:system.webServer/handlers 。
其中的 handlers 是错误信息中红字显示的节点名称。
如果modules也被锁定，可以运行
C:\windows\system32\inetsrv\appcmd unlock config -section:system.webServer/modules

注意：cmd.exe要以管理员身份启动，在c:\windows\system32下找到cmd.exe，右键管理员启动，输入上面的命令即可。

