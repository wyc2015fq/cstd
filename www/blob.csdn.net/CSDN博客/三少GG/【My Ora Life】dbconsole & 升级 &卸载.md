# 【My Ora Life】dbconsole & 升级 &卸载 - 三少GG - CSDN博客
2011年06月23日 18:19:00[三少GG](https://me.csdn.net/scut1135)阅读数：1750
**利用 emctl status dbconsole 指令：**
[aime@slc00bkw bin]$ emctl status dbconsole
Oracle Enterprise Manager 11g Database Control Release 11.1.0.6.0 
Copyright (c) 1996, 2007 Oracle Corporation.  All rights reserved.
https://***:1158/em/console/aboutApplication
Oracle Enterprise Manager 11g is running. 
------------------------------------------------------------------
Logs are generated in directory /scratch/aime/prod_install/app/product/11.1.0/db_1/slc00bkw.us.oracle.com_orcl/sysman/log 
故地址为: https://***:**1158**/em/**  注意是https!!! 不是http!**
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
1、楼主输入的登录地址是什么？一般要输入http://ip地址:1158/em
2、em服务是否启动，通过emctl status dbconsole查一下，如没启动执行emctl start dbconsole，然后注意观察em启动时的端口号，一般默认端口为1158
3、确认em是否正确安装
4、试着执行emca -repos create重建em资料库，重建时要指定oracle sid、端口号、sys用户名和密码
 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
升级：
相关帮助文档： [https://support.oracle.com/CSP/ui/flash.html#tab=KBHome(page=KBHome&id=()),(page=KBNavigator&id=(bmDocID=800550.1&bmDocDsrc=KB&from=BOOKMARK&viewingMode=1143&bmDocTitle=11.1.0.7%20Patch%20Set%20-%20Linux%20x86%2064bit%20README&bmDocType=README](https://support.oracle.com/CSP/ui/flash.html#tab=KBHome(page=KBHome&id=()),(page=KBNavigator&id=(bmDocID=800550.1&bmDocDsrc=KB&from=BOOKMARK&viewingMode=1143&bmDocTitle=11.1.0.7%20Patch%20Set%20-%20Linux%20x86%2064bit%20README&bmDocType=README)))
利用database upgrade assistant：
Upgrade database using **dbua**
注意设置环境变量：setenv ORACLE_HOME /scratch/aime/prod_install/app/product/11.1.0/db_1
没有"/" 不是setenv ORACLE_HOME /scratch/aime/prod_install/app/product/11.1.0/db_1/
否则会弹出消息：
![oracle 实践日志(2) aix 升级 The following processes need to be shutdown before continuing:java ksh - prefectliu - 大漠飞鹰--展翅翱翔于滚滚沙尘之大漠高空](http://img609.ph.126.net/MM2hZWPSHbxl3EAEfpW-dg==/1891793318473958521.png)
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
使用DBCA创建数据库时走到第四步时：
提示："You need to run Netca to configure a listener before you can proceed.Otherwise you may choose to continue without Database Control configuration"
解决办法：
> 
If you need a port to connect to Oracle, then you certainly need to configure a LISTENER, which is not done by the install. To do this, **you should run "netca" ("Net configuration assistant"), located at $ORACLE_HOME/bin/netca** in UNIX (it should be something like this in Windows, and you must have a shortcut in your "Start" menu). Choose "Configure a listener" (or something like this), then "Add a listener" ... go on ... and it will ask for the port on which the listener is gonna listen (1521 by default). Once the listener is configured, quit netca, and **retry your connection from Enterprise Manager. Now it should work.** If the [Oracle server](http://writeblog.csdn.net/#) is on another machine, you should also configure your "tnsnames.ora" file, which can be done either "by hand" or with "Net Manager" ($ORACLE_HOME/bin/netmgr, "Local->service name resolution" menu or something like this).
I hope that will help.
备注：其实不一定新建，如果原来有listener的话，重新配置一下即可（也不需要更改什么东西）
+++++++++++++++++++++++++++++++++++++++
卸载：
