# win7下安装wince6.0遇到问题的解决 - xqhrs232的专栏 - CSDN博客
2016年07月25日 20:52:17[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：642
原文地址::[http://blog.csdn.net/yizhou2010/article/details/6154330](http://blog.csdn.net/yizhou2010/article/details/6154330)
相关文章
1、[WIN7 64位系统搭建WINCE6.0系统遇到的问题](http://blog.csdn.net/loongembedded/article/details/37689743)----[http://blog.csdn.net/loongembedded/article/details/37689743](http://blog.csdn.net/loongembedded/article/details/37689743)
2、[基于Windows
 7旗舰版搭建WinCE6.0开发环境的过程](http://www.cnblogs.com/we-hjb/archive/2010/08/23/1806092.html)----[http://www.cnblogs.com/we-hjb/archive/2010/08/23/1806092.html](http://www.cnblogs.com/we-hjb/archive/2010/08/23/1806092.html)
3、[win7下安装wince 6.0遇到的问题](http://blog.csdn.net/tyronewj/article/details/4994152)----[http://blog.csdn.net/tyronewj/article/details/4994152](http://blog.csdn.net/tyronewj/article/details/4994152)
由于习惯了win7操作系统，于是就想将wince6.0也安装到win7系统下，本以为是简简单单的过程，谁知道一开始安装就错误不断，搞得头昏脑胀。最后在牛人的帮助下，终于成功安装。现将安装过程中遇到的问题做个记录，以便备查。
首先是安装顺序：
1、先装Visual Studio 2005；
2、安装Visual Studio 2005 Service Pack 1；
3、安装Visual Studio 2005 Service Pack 1 Update for Windows Vista；
4、安装MSDN（可选），从VS2005的安装页里选择；
5、安装Windows Embedded CE 6.0；
6、安装Windows Embedded CE 6.0 Platform Builder Service Pack 1；
7、安装Wince 6.0 R2；
8、安装Wince 6.0 R3。
报错信息：
1、ToolsMsmCA(Error): Setup package issue or CoreCon DataStore corrupted: CDeviceSDKInstallShim Add/Remove failed. HR=0x8007005。
2、ToolsMsmCA(Error): Setup package issue or CoreCon DataStore corrupted: DeviceSDKInstComp Add/Remove failed. HR=0x8007005。
对于0x8007005之类的错误是由于从Windows的Vista之后，运行程序都有一个管理员权限的设置造成的。只要在安装的时候采用管理员权限就可以顺利的安装好。
WinCE6.0的安装程序点击右键却没有“使用管理员权限”的菜单，处理办法就是用“管理员权限”打开cmd.exe，然后通过命令行的方式，使用Call安装执行程序就可以了。例如：安装文件在C盘的Wince6.0文件夹下CE6.0.msi 输了的命令为“call c:/wince6.0/ce6.0.msi“
之后又 在installing platform manager时出现了以下的错误：
"The installer has encountered an unexpected error   installing this package .This may indicate a problem with this package.This error
 code is 2738."
解决方法：
先尝试一下 命令提示符中输入了regsvr32 vbscript.dll ,加载了这个dll ,再安装。
如果不行的话，这个可能是由于您当前的使用的用户没有用administrator权限来注册这个东西。
首先，先移除注册表中的这几项，如果它们有值存在的话。
         VBScript,
         HKEY_CURRENT_USER./SOFTWARE/Classes/CLSID/{ B54F3741-5B07-11CF-A4B0-00AA004A55E8}
然后再重新安装试一下。
经过这几个步骤，终于在win7系统中成功安装了。
