# WinCE6.0 R3的安装步骤 - xqhrs232的专栏 - CSDN博客
2013年01月21日 15:16:13[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：736
原文地址::[http://www.cnblogs.com/we-hjb/archive/2010/03/12/1684754.html](http://www.cnblogs.com/we-hjb/archive/2010/03/12/1684754.html)
相关网帖
1、[WinCE6.0 R3和2009年补丁安装顺序简介](http://blog.csdn.net/cefighter/article/details/5277527)----[http://blog.csdn.net/ceFighter/article/details/5277527](http://blog.csdn.net/ceFighter/article/details/5277527)
2、[WinCE6.0](http://blog.csdn.net/cefighter/article/details/5277527)安装顺序----[http://www.360doc.com/content/10/0831/20/1325155_50213430.shtml](http://www.360doc.com/content/10/0831/20/1325155_50213430.shtml)
3、[关于wince6.0 升级全年包的注意事项（作者：gooogleman）](http://blog.csdn.net/gooogleman/article/details/7561422)----[http://blog.csdn.net/gooogleman/article/details/7561422](http://blog.csdn.net/gooogleman/article/details/7561422)
4、[关于WinCE6.0补丁包的一点说明](http://www.cnblogs.com/we-hjb/archive/2010/07/15/1778429.html)----[http://www.cnblogs.com/we-hjb/archive/2010/07/15/1778429.html](http://www.cnblogs.com/we-hjb/archive/2010/07/15/1778429.html)
5、[2008-2009年WinCE6.0补丁下载地址](http://www.cnblogs.com/we-hjb/archive/2010/07/21/1782517.html)----[http://www.cnblogs.com/we-hjb/archive/2010/07/21/1782517.html#1877093](http://www.cnblogs.com/we-hjb/archive/2010/07/21/1782517.html#1877093)
6、[基于Windows 7旗舰版搭建WinCE6.0开发环境的过程](http://www.cnblogs.com/we-hjb/archive/2010/08/23/1806092.html)----[http://www.cnblogs.com/we-hjb/archive/2010/08/23/1806092.html](http://www.cnblogs.com/we-hjb/archive/2010/08/23/1806092.html)
7、[WinCE6.0补丁包的恢复与再更新](http://www.cnblogs.com/we-hjb/archive/2010/08/20/1805015.html)----[http://www.cnblogs.com/we-hjb/archive/2010/08/20/1805015.html](http://www.cnblogs.com/we-hjb/archive/2010/08/20/1805015.html)
很早以前装过一次R3，装完以后直接导致编译不过，当时也没找到解决办法，最后只能放弃，一直没再动它。这两天ZWF在调试程序的过程中发现了一个BUG，应用程序无法正确设置文件夹的隐藏属性。起初我以为是NAND驱动或者是注册表配置的问题。但从这两方面入手都没能成功。后来通过实验发现，在根目录下可以将文件夹设置为隐藏并且能生效，同样的代码在NAND目录下就不能正常工作。在根目录下设置为隐藏的文件夹，拷贝到另外任意一个目录下，会变成非隐藏的。据此猜测，可能是WinCE文件系统本身的BUG。于是找到09年全年补丁包的详细说明，仔细查看一番，终于确认了问题所在。
Component:  FILESYS
- 
[091031_KB976628](http://support.microsoft.com/kb/976628) - Users may not be able to update the attributes of a directory on a FAT32 file
 system formatted device (FLASH/USB Mass Storage device, etc.). This affects customers applications that need to set/reset attributes on specific directories used by the application on the device.
　　09年10月份的补丁包解决了这个问题，而该补丁依赖于R3。再也躲不过了，只能硬着头皮再搞一把。吸取上次失败的教训，这回先确定了各补丁的安装顺序，主要以微软发布时间的先后做参考，具体安装步骤如下:
```
[](http://www.cnblogs.com/we-hjb/archive/2010/03/12/1684754.html)
            Visual Studio 2005
            Visual Studio 2005 Service Pack 1
            Visual Studio 2005 Service Pack 1 Update for Windows Vista (if applicable)
            Windows Embedded CE 6.0 Platform Builder
            Windows Embedded CE 6.0 SP1 (required if PB 6.0 Tools have been installed)
            Windows Embedded CE 6.0 R2 
            Windows Embedded CE 6.0 Cumulative Product Update Rollup 12/31/2007
            Windows Embedded CE 6.0 Cumulative Product Update Rollup 12/31/2008
            
            Windows Embedded CE 6.0 2009 QFE M01 to M09
            
            Windows Embedded CE 6.0 R3 10/29/2009
            
            Windows Embedded CE 6.0 R3 Update Rollup 11/3/2009
            
            Windows Embedded CE 6.0 2009 QFE M10 to M12
            
            Windows Embedded CE 6.0 2010 QFE M01 to M02
[](http://www.cnblogs.com/we-hjb/archive/2010/03/12/1684754.html)
```
　　按照这个步骤，小心翼翼的逐一安装下来，很是幸运，一路通畅。
　　打开工程，执行Clean Sysgen，然后诚惶诚恐的等着。万幸，0 Error！甚至有点怀疑是否真的成功打上了R3补丁。下载到设备运行，在系统属性的版本里也没有看到任何有关R3的影子。不过，目录无法隐藏的问题解决了。为了验证R3的真实性，又添加了QQ、Flash和Office阅读器等新增组件，测试了一把，基本能用，至少说明不是被R3的。
　　很高兴，终于解决掉这个心病。^_^录制了一段WinCE6.0 R3的视频，有兴趣的同志可以看看。[http://files.cnblogs.com/we-hjb/WinCE6.0R3Demo.rar](http://files.cnblogs.com/we-hjb/WinCE6.0R3Demo.rar)
//==========================
备注::
1、今天整了一下，死活都没编译过去，报23个错误，只在全部装完的时候打了个2011年的全年包（也就是装完R2的时候没打什么补丁包），不知道是不是这个问题，明天再整整，实在不行只能重新折腾一遍了。以前整的都没记录了，又得摸一遍了，技术来不得虚的啊！
2、昨天晚上在家里就是装完所有的东西，包括R2+R3+QQ+PDF+OFFICE再打一最近的年补丁包就可以了，建立了一个模拟工程是可以编译通过并跑起来的，看来不一定非得在装了R2的时候就去打什么补丁。可以统一到最后去打，月补丁太多了不打也罢！
3、奇怪的是拿了个6410的BSP包在按上面的安装方式安装的WINCE60系统编译，也可以编译通过，但就是在实际的硬件上跑不起来，我试了不管是在家里的电脑还是公司的电脑，这样编译出来的IMAGE在实际的硬件是都是跑不起来的，看来做技术真没捷径可走啊！还真的按步骤一步步来啊！
4、看来打补丁包真有讲究啊！打的不好真有可能启动不了！！！----[关于wince6.0 升级全年包的注意事项（作者：gooogleman）](http://blog.csdn.net/gooogleman/article/details/7561422)----[http://blog.csdn.net/gooogleman/article/details/7561422](http://blog.csdn.net/gooogleman/article/details/7561422)
5、好像2012年的补丁必须打，要不然启动不了系统
兄弟，2011年之前的升级包不用装了，R3都包含了wince发行以来所有代码及库的补丁，
安装顺序参考如下
VS2005
VS2005 SP1
VS2005 SP1 for Vista(如果是xp，不需要此项)
PB
PB SP1
R2
R3
WinCEPB60-111231-Product-Update-Rollup-Armv4I.msi
这样就OK了
之后可以继续安装
WinCEPB60-120131-2012M01-Armv4I.msi等
6、今天按[基于Windows 7旗舰版搭建WinCE6.0开发环境的过程](http://blog.csdn.net/xqhrs232/article/details/8567112)----[http://blog.csdn.net/xqhrs232/article/details/8567112](http://blog.csdn.net/xqhrs232/article/details/8567112)提到的安装顺序，总算把UT6410的工程编译通过并跑起来了。害我重装了2次WINCE660。真是悲催！不过现在网上都已经找不到2009/2010年的年补丁包了，这还真的是个问题！
7、在没有2006/2007/2008/2009/2010年年包的情况下安装WINCE60补丁
试了一下还真的是可以啊！----只装了WinCEPB60-R3-2009-Update-Rollup-Armv4I.msi++++WinCEPB60-121231-Product-Update-Rollup-Armv4I.msi，我怀疑只装WinCEPB60-121231-Product-Update-Rollup-Armv4I.msi也是可以的。
8、手上有一年补丁包还要不要打过去的补丁包，看一看对应的Windows Embedded CE 6.0 R3_Product_Update_Rollup_2012.htm文件就可以了，其中会列出包含的补丁，举例如下，从下面2012的补丁文件可以看到已经包含了09/10/11/12年的补丁了，所以前面都可以不打了。
Component:  Bluetooth
- 
[090930_KB970362](http://support.microsoft.com/kb/970362) - Under certain circumstances Bluetooth FTP record may get removed.
- 
[100126_KB979228](http://support.microsoft.com/kb/979228) - Bluetooth PAN profile does not allow more than one simultaneous connection to be initiated.
- 
[100531_KB2118663](http://support.microsoft.com/kb/2118663) - Bluetooth Host Control Interface (HCI) handle may leak when calling BthGetHardwareStatus API.
- 
[110622_KB2556378](http://support.microsoft.com/kb/2556378) - This update addresses some Bluetooth issues.
- 
[120430_KB2695045](http://support.microsoft.com/kb/2695045) - This update addresses test failures.
- 
[120625_KB2708132](http://support.microsoft.com/kb/2708132) - Under certain circumstances all existing RFCOMM connections are disconnected. A new registry key is also introduced 'AvoidMultipleDisconnectMsgs'
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
1、[090930_KB970362](http://support.microsoft.com/kb/970362) -
 Under certain circumstances Bluetooth FTP record may get removed.
是WinCEPB60-R3-2009-Update-Rollup-Armv4I.msi的一个补丁，所以WinCEPB60-R3-2009-Update-Rollup-Armv4I.msi也不用打了。
2、对应的补丁列表通过他们提供的时间也可以对应找到！！！

