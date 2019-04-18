# 关于VS2015安装Team Explorer for Microsoft Visual Studio 2015问题总结（已解决） - 一世豁然的专栏 - CSDN博客





2017年12月24日 18:45:55[一世豁然](https://me.csdn.net/Explorer_day)阅读数：8970








最近不知道怎么，想使用VS2015查看代码，但是打开VS2015的时候却提醒VS2015软件不完整？我记得自己没有动过VS2015呀！按照以往的经验，自己试了以下方法：

一、在控制面板的卸载程序中，点击VS2015的“修复”选项，但是安装到一半就会出现以下问题：

![](https://img-blog.csdn.net/20171224184818598?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRXhwbG9yZXJfZGF5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)





我又查看了软件Log，最后关于Team Explorer出现以下日志：



```
[18F0:1960][2017-12-23T22:19:47]i301: Applying execute package: vs_teamExplorerCore, action: Install, path: C:\ProgramData\Package Cache\{791295AE-3B0A-3222-9E69-26C8C106E8D1}v14.0.23102\packages\TeamExplorer\Core\vs_teamExplorerCore.msi, arguments: ' MSIFASTINSTALL="7" USING_EXUIH="1"'
[05AC:1A7C][2017-12-23T22:20:02]i000: MUX:  ExecuteError: Package (vs_teamExplorerCore) failed: Error Message Id: 1722 ErrorMessage: There is a problem with this Windows Installer package. A program run as part of the setup did not finish as expected. Contact your support personnel or package vendor.  
[18F0:1960][2017-12-23T22:20:05]e000: Error 0x80070643: Failed to install MSI package.
[18F0:1960][2017-12-23T22:20:05]e000: Error 0x80070643: Failed to execute MSI package.
[05AC:1A7C][2017-12-23T22:20:05]e000: Error 0x80070643: Failed to configure per-machine MSI package.
[05AC:1A7C][2017-12-23T22:20:05]i000: MUX:  Installation size in bytes for package: vs_teamExplorerCore MaxAppDrive: 171569152  MaxSysDrive: 28168192  AppDrive: 4096  SysDrive: 23842816
[05AC:1A7C][2017-12-23T22:20:05]i000: MUX:  Return Code:0x80070643 Msi Messages:There is a problem with this Windows Installer package. A program run as part of the setup did not finish as expected. Contact your support personnel or package vendor.   Result Detail:0 Restart:None
[05AC:1A7C][2017-12-23T22:20:05]i000: MUX:  Set Result: Return Code=-2147023293 (0x80070643), Error Message=There is a problem with this Windows Installer package. A program run as part of the setup did not finish as expected. Contact your support personnel or package vendor.  , Result Detail=, Vital=True, Package Action=Install, Package Id=vs_teamExplorerCore
[05AC:1A7C][2017-12-23T22:20:05]i000: Setting string variable 'BundleResult' to value '1603'
[05AC:1A7C][2017-12-23T22:20:05]i319: Applied execute package: vs_teamExplorerCore, result: 0x80070643, restart: None
[05AC:1A7C][2017-12-23T22:20:05]e000: Error 0x80070643: Failed to execute MSI package.
[05AC:1A7C][2017-12-23T22:20:05]i000: MUX:  ExecutePackageBegin PackageId: vs_teamExplorerCore
[18F0:1960][2017-12-23T22:20:05]i318: Skipped rollback of package: vs_teamExplorerCore, action: Uninstall, already: Absent
[05AC:1A7C][2017-12-23T22:20:05]i000: MUX:  Installation size in bytes for package: vs_teamExplorerCore MaxAppDrive: 0  MaxSysDrive: 0  AppDrive: 0  SysDrive: 0
[05AC:1A7C][2017-12-23T22:20:05]i000: MUX:  Return Code:0x0 Msi Messages:There is a problem with this Windows Installer package. A program run as part of the setup did not finish as expected. Contact your support personnel or package vendor.   Result Detail:0 Restart:None
[05AC:1A7C][2017-12-23T22:20:05]i000: MUX:  Reset execution Result
[05AC:1A7C][2017-12-23T22:20:05]i000: MUX:  Reset Result
[05AC:1A7C][2017-12-23T22:20:05]i000: Setting string variable 'BundleResult' to value '1603'
[05AC:1A7C][2017-12-23T22:20:05]i319: Applied rollback package: vs_teamExplorerCore, result: 0x0, restart: None
[18F0:1960][2017-12-23T22:20:05]i351: Removing cached package: vs_teamExplorerCore, from path: C:\ProgramData\Package Cache\{791295AE-3B0A-3222-9E69-26C8C106E8D1}v14.0.23102\
[18F0:1960][2017-12-23T22:20:05]i372: Session end, registration key: SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\{1a4d4608-a36d-4dc7-accd-206e23253076}, resume: ARP, restart: None, disable resume: No
[18F0:1960][2017-12-23T22:20:05]i371: Updating session, registration key: SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\{1a4d4608-a36d-4dc7-accd-206e23253076}, resume: ARP, restart initiated: No, disable resume: No
[05AC:1A7C][2017-12-23T22:20:05]i000: MUX:  Apply Complete: Disk Space Used in bytes for Installation:  MaxAppDrive: 410365952  MaxSysDrive: 1165197312  AppDrive: 0  SysDrive: 630546432
[05AC:1A7C][2017-12-23T22:20:05]i000: MUX:  Free Disk Space after install:  SystemDrive C:\ 57547517952 bytes  AppDrive D:\ 47390670848 bytes
[05AC:1A7C][2017-12-23T22:20:05]i000: MUX:  Go to Finished page.
[05AC:1A7C][2017-12-23T22:20:06]i000: MUX:  Watson Bucketting Parameters
[05AC:1A7C][2017-12-23T22:20:06]i000: MUX:  P1 - vs_enterprise
[05AC:1A7C][2017-12-23T22:20:06]i000: MUX:  P2 - 14.0.23107.178
[05AC:1A7C][2017-12-23T22:20:06]i000: MUX:  P3 - 14.0.23107
[05AC:1A7C][2017-12-23T22:20:06]i000: MUX:  P4 - Repair
[05AC:1A7C][2017-12-23T22:20:06]i000: MUX:  P5 - vs_teamExplorerCore
[05AC:1A7C][2017-12-23T22:20:06]i000: MUX:  P6 - Install
[05AC:1A7C][2017-12-23T22:20:06]i000: MUX:  P7 - 0x80070643
[05AC:1A7C][2017-12-23T22:20:06]i000: MUX:  P8 - There is a problem with this Windows Installer package. A program run as part of the setup did not finish as expected. Contact your support personnel or package vendor.  
[05AC:1A7C][2017-12-23T22:20:06]i000: MUX:  P9 - 
[05AC:1A7C][2017-12-23T22:20:06]i399: Apply complete, result: 0x80070643, restart: None, ba requested restart:  No
```

二、难道真的是安装包有问题？

把VS2015又卸载之后，重新解压了新的VS2015 ISO文件安装，但是依旧出现以上问题。




三、百度/谷歌了许多资料，又有资料说以前的没有卸载干净，我使用官方提供的程序加上自己手动完全卸载之后，依旧会出现以上问题。

其中在谷歌上又有一下解决方案：

![](https://img-blog.csdn.net/20171224185213536?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRXhwbG9yZXJfZGF5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


我照着上面的步骤一步步完成，但是依旧出现以上问题。




四、有人建议重装系统，但自己实在不想重装。重装之后我那些开发软件又要重新安装，太麻烦了。




五、仔细查看了一下Log发现关于Team Explorer的有一个文件夹：C:\ProgramData\Package Cache（此为隐藏文件），我百度了了一下，说是要修改某个文件文件名的后缀，但自己也想不了那么多了，把整个文件夹都清空了。最后完美解决。如下图：

![](https://img-blog.csdn.net/20171225222702806?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRXhwbG9yZXJfZGF5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)





这个问题困扰了我三天了。终于找到了解决办法。写此博客来记录。






