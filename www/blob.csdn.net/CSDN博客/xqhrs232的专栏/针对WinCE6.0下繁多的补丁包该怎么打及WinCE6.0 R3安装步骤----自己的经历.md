# 针对WinCE6.0下繁多的补丁包该怎么打及WinCE6.0 R3安装步骤----自己的经历 - xqhrs232的专栏 - CSDN博客
2013年02月03日 23:52:51[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1265
相关网帖
1、
### [Windows Embedded CE 6.0开发环境的搭建](http://blog.csdn.net/vvmao/article/details/3986222)----[http://blog.csdn.net/vvmao/article/details/3986222](http://blog.csdn.net/vvmao/article/details/3986222)

     最近又整了一下WINCE系统，成功搭建一个环境就整了我好几天，期间的悲催与痛苦只有自己才知道啊！WINCE60整个系统来来回回装了4-5遍，不过还好，功夫不负有心人，今天总算把整个环境搭建成功，现记录如下
   期间还有一个插曲，就是拿模拟器工程来当做一个“HELLO WORLD”一样的工程来做实验，以为“HELLO WORLD”工程编译运行成功，整个WINCE60的编译环境也就成功了，孰不知当我拿一个真正的芯片BSP包到我的环境里面去编译的时候，编译出来的东西老是跑不起来，其中的痛苦折腾真是难受，搞到最后连自己都要放弃了。不过也怪自己学艺不精，所以难免要交学费。
    WINCE60 R3的安装顺序总结如下----自己亲自在WINXP上测试过,并测试了R3的新组件（PDF/WORD/QQ）都是可以正常跑起来的
Visual Studio 2005
            Visual Studio 2005 Service Pack 1
            Visual Studio 2005 Service Pack 1 Update for Windows Vista/7 (if applicable)
            Windows Embedded CE 6.0 Platform Builder
            Windows Embedded CE 6.0 SP1
            Windows Embedded CE 6.0 R2
            Windows Embedded CE 6.0 R3
            Windows Embedded CE 6.0 Cumulative Product Update Rollup 12/31/2012
MS在2013/1/29日发布了2012年的年补丁包----WinCEPB60-121231-Product-Update-Rollup-Armv4I.msi。所以去下载这个年补丁包安装就可以了，很多地方提到了装了R3还要装一个WinCEPB60-R3-2009-Update-Rollup-Armv4I.msi补丁包，其实没有必要,看Windows
 Embedded CE 6.0 R3_Product_Update_Rollup_2012.htm文档可以很明确地知道WinCEPB60-121231-Product-Update-Rollup-Armv4I.msi年补丁包已经包含了Windows
 Embedded CE 6.0_R3_2009_Update_Rollup.htm补丁包的内容。现举例如下
从下面2012的年补丁包文档可以看到已经包含了09/10/11/12年的补丁了，所以前面的补丁包都可以不打了。
Component:  Bluetooth
- 
[090930_KB970362](http://support.microsoft.com/kb/970362) - Under certain circumstances Bluetooth FTP record may get removed.
- 
[100126_KB979228](http://support.microsoft.com/kb/979228) - Bluetooth PAN profile does not allow more than one simultaneous connection
 to be initiated.
- 
[100531_KB2118663](http://support.microsoft.com/kb/2118663) - Bluetooth Host Control Interface (HCI) handle may leak when calling BthGetHardwareStatus
 API.
- 
[110622_KB2556378](http://support.microsoft.com/kb/2556378) - This update addresses some Bluetooth issues.
- 
[120430_KB2695045](http://support.microsoft.com/kb/2695045) - This update addresses test failures.
- 
[120625_KB2708132](http://support.microsoft.com/kb/2708132) - Under certain circumstances all existing RFCOMM connections are disconnected.
 A new registry key is also introduced 'AvoidMultipleDisconnectMsgs'
[HKEY_LOCAL_MACHINE\Software\Microsoft\Bluetooth\rfcomm] to turn ON/OFF the fix for this issue. The registry is set to '0' by default disabling the fix.
> 
The file(s) with the new source code changes for this component can be found in the directory %_WINCEROOT%\:
     public\common\oak\files\common.reg
     private\winceos\comm\bluetooth\rfcomm\rfcomm.cxx
     private\winceos\comm\bluetooth\l2cap\l2cap.cxx
     private\servers\obex\server\obexp.hxx
     private\servers\obex\server\obmain.cxx
     private\servers\obex\server\obutil.cxx
     private\winceos\comm\bluetooth\sdp\user\bthapi\sdprecord.cpp
     private\winceos\comm\bluetooth\pan\miniport.cxx     
     private\winceos\comm\bluetooth\hci\base\hci.cxx
     public\common\oak\inc\bt_ddi.h
     private\winceos\comm\bluetooth\sys\btutils.cxx
//=========================
其中。。。。。。
1、[090930_KB970362](http://support.microsoft.com/kb/970362) - Under certain circumstances Bluetooth FTP record may
 get removed.
是WinCEPB60-R3-2009-Update-Rollup-Armv4I.msi的一个补丁，所以WinCEPB60-R3-2009-Update-Rollup-Armv4I.msi也不用打了。[100126_KB979228](http://support.microsoft.com/kb/979228)+[100531_KB2118663](http://support.microsoft.com/kb/2118663)是2010年的补丁内容，[110622_KB2556378是2011年的补丁内容](http://support.microsoft.com/kb/2556378)，[120430_KB2695045+120625_KB2708132是2012年的补丁内容](http://support.microsoft.com/kb/2556378)。所以说2012年年包包括前面的所有年包的内容，所以打2012年的年包就可以了，但有些年包还是有问题的，比如2010/2011都是有问题的，看来选择打那个年包也是有学问啊！[](http://support.microsoft.com/kb/2556378)
[关于wince6.0
 升级全年包的注意事项（作者：gooogleman）](http://blog.csdn.net/gooogleman/article/details/7561422)----[http://blog.csdn.net/gooogleman/article/details/7561422](http://blog.csdn.net/gooogleman/article/details/7561422)
2、对应的补丁列表通过他们提供的时间也可以对应找到！！！要特别注意补丁编号的使用与检索，以前也是不知道要不要打那个补丁包，也没注意补丁编号与时间戳的事情
3、打完补丁包可以用CEUPDATECHECK工具看一下，你的WINCE编译环境的补丁包打的怎么样了。以前也不清楚怎么检查这些，还是研究的不深入啊！或者还可以恢复成没打补丁的情形，因为打补丁的时候，WINCE会帮你把以前的东西做好备份的，在Updates目录下，所以说补丁包没打好，其实也不用把整个WINCE重装一遍的，你只要一个个退回去就可以，要前进就重新把补丁装一遍就好了。
