# 使用python获取系统所有进程PID以及进程名称 - 小灰笔记 - CSDN博客





2017年04月13日 00:50:52[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：19169








psutil模块中提供了进程管理方法，引用其中的几个方法就能够获得进程的相关信息。简单写个小脚本测试一下，顺便看看我的Mac中到底运行着多少个进程，进程都是什么。

代码：

  1 #!/usr/bin/python

  2 

  3 import psutil

  4 

  5 pids = psutil.pids()

  6 for pid in pids:

  7     p = psutil.Process(pid)

  8 print("pid-%d,pname-%s" %(pid,p.name()))

  9 

 10 




运行结果如下：

GreydeMac-mini:01_系统基础信息模块详解 greyzhang$ python pid.py 

pid-1633,pname-Python

pid-1631,pname-AddressBookSourceSync

pid-1606,pname-mdworker

pid-1588,pname-Java Updater

pid-1586,pname-mdworker

pid-1585,pname-mdworker

pid-1584,pname-mdworker

pid-1565,pname-ocspd

pid-1562,pname-cupsd

pid-1470,pname-distnoted

pid-1468,pname-secinitd

pid-1467,pname-com.apple.geod

pid-1373,pname-mdworker

pid-1357,pname-printtool

pid-1353,pname-com.apple.appkit.xpc.openAndSavePanelService

pid-1349,pname-Pages

pid-1303,pname-com.apple.appkit.xpc.openAndSavePanelService

pid-1209,pname-Numbers

pid-1144,pname-CoreServicesUIAgent

pid-1138,pname-com.apple.CommerceKit.TransactionService

pid-1126,pname-mdworker

pid-1124,pname-mdworker

pid-1122,pname-AssetCacheLocatorService

pid-1116,pname-com.apple.CommerceKit.StoreAEService

pid-1103,pname-com.apple.CommerceKit.TransactionService

pid-1102,pname-com.apple.CommerceKit.TransactionService

pid-1099,pname-com.apple.appstore.PluginXPCService

pid-1098,pname-storeuid

pid-1051,pname-syspolicyd

pid-804,pname-USBAgent

pid-785,pname-mdworker

pid-784,pname-mdworker

pid-755,pname-bash

pid-754,pname-login

……

……

……

pid-60,pname-mds

pid-59,pname-warmd

pid-57,pname-airportd

pid-53,pname-logd

pid-49,pname-mobileassetd

pid-48,pname-powerd

pid-47,pname-configd

pid-46,pname-appleeventsd

pid-43,pname-mediaremoted

pid-41,pname-fseventsd

pid-40,pname-kextd

pid-39,pname-uninstalld

pid-37,pname-syslogd

pid-36,pname-UserEventAgent

pid-1,pname-launchd

number of PIDs: 260




轻松获取了系统中的进程清单，统计完之后共有260个进程。



