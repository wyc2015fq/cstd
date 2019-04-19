# xp操作系统下配置iis，出现了server application error的解决办法 - 毛小亮 - 博客园
# [xp操作系统下配置iis，出现了server application error的解决办法](https://www.cnblogs.com/xianerwonder/p/4527152.html)
在网上搜索了很多解决办法，最后发现一个差不多的：
Server Application Error 
The server has encountered an error while loading an application during the processing of your request. Please refer to the event log for more detail information. Please contact the server administrator for assistance. 
访问IIS客户端出现如上错信息提示,
在网上查了查解决方案如下:
1。右键我的电脑--管理--本地用户和组，给IUSR_机器名和IWAM_机器名两个用户设置密码，要一样。  
2。开始--运行--打cmd，  
然后cd D:InetpubAdminscripts（我的系统在D盘），  
然后cscript.exe adsutil.vbs set w3svc/wamuserpass 你的密码，  
然后cscript.exe adsutil.vbs set w3svc/anonymoususerpass 你的密码  
看一下，行了没有？如果还不行，那么  
cscript.exe synciwam.vbs -v，  
然后iisreset。  
据说就可以了,但是到最后一部提示8004e00f的错误,此错误是MSDTC服务不正常造成的,解决方法:
首先进入组件服务，查看组件服务/计算机/我的电脑/COM+应用程序，结果报错“COM+ 无法与 Microsoft 分布式事务协调程序交谈”，无法查看里面的对象。  
2、进入事件查看器，发现msdtc服务没有正常启动。  
3、删除注册表中的键：  
• HKEY_LOCAL_MACHINESYSTEMCurrentControlSetServicesMSDTC  
• HKEY_LOCAL_MACHINESOFTWAREMicrosoftMSDTC  
• HKEY_CLASSES_ROOTCID  
4、停止MSDTC服务：net stop msdtc  
5、卸载MSDTC服务：msdtc -uninstall  
6、重新安装MSDTC服务：msdtc -install  
7、确认在事件查看器中msdtc服务已经正常启动[这步很关键，如果没有，重新启动下电脑看看]  
8、重新设置IIS的IWAM账号密码。[在计算机管理中的用户管理里]  
9、同步IIS metabase中IWAM_MYSERVER的密码，在CMD中：c:inetputadminscripts>adsutil set w3svc/wamuserpass "yourpassword"  
10、同步COM+应用程序所用的IWAM_MYSERVER密码，在CMD中：c:inetputadminscripts>cscript synciwam.vbs -v  
到这部分网上大部分都说可以了,但是我这里虽然没有8004e00f的错误提示了,但是访问页面Server Application Error 错误依旧,最后终于找到了解决方法如下:
解决办法：
[1]：
检查你的DTC服务（全名：Distributed Transaction Coordinator）是否可以正常启动，
如果正常的话请你跳过此步骤；如果出错，无法正常启动，请在开始菜单的运行中 运行：msdtc -resetlog 以创建日志文件。重起机器，检查IIS是否可以正常使用，若不行继续。
[2]：
在CMD下执行以下命令：
cd %windir%\system32\inetsrv 
rundll32 wamreg.dll, CreateIISPackage 
regsvr32 asptxn.dll
(到这里我就可以用了，不用重启的)
以上的方法我尝试了下，结果报错“服务器应用程序不可用”。于是把.netFramework2.0重装了一下，全部卸载，先重装4，再重装2
终于能用了，一把辛酸泪。
xp系统配置起来还是太复杂，如果能换系统，还是用win7吧。

