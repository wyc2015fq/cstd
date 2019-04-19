# 基于Windows 7旗舰版搭建WinCE6.0开发环境的过程 - xqhrs232的专栏 - CSDN博客
2013年02月03日 16:33:52[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：958
原文地址::[http://www.cnblogs.com/we-hjb/archive/2010/08/23/1806092.html](http://www.cnblogs.com/we-hjb/archive/2010/08/23/1806092.html)
  首先感谢[JeffreyLi](http://www.cnblogs.com/jeffreyli/)提供了更简便的[安装WinCE6.0
 R3的方法](http://www.cnblogs.com/we-hjb/archive/2010/07/21/1782517.html#1877093)，今天验证过了，确实比我之前整理的《[WinCE6.0 R3的安装步骤](http://www.cnblogs.com/we-hjb/archive/2010/03/12/1684754.html)》方便很多，十分感谢！
      为了方便大家参考，重新整理的安装步骤如下： 
```
[]()
*         Please install the following software in order
            Visual Studio 2005
            Visual Studio 2005 Service Pack 1
            Visual Studio 2005 Service Pack 1 Update for Windows Vista/7 (if applicable)
            Windows Embedded CE 6.0 Platform Builder
            Windows Embedded CE 6.0 SP1
            Windows Embedded CE 6.0 R2
            Windows Embedded CE 6.0 R3
            
            Windows Embedded CE 6.0 R3 Update Rollup 11/3/2009
            
            Windows Embedded CE 6.0 Cumulative Product Update Rollup 12/31/2009
            
            Windows Embedded CE 6.0 2010 QFE M01 to M07
[]()
```
       在WinXP下按照这个步骤来，应该不会有什么问题了。但今天在Windows 7旗舰版下安装还是碰到了一些状况。
      首先是VS2005与Windows 7兼容性的问题，如下图所示。
![image](http://images.cnblogs.com/cnblogs_com/we-hjb/WindowsLiveWriter/Windows7WinCE6.0_2F7/image_thumb.png)
      点击“联机检查解决方案”按钮，稍等片刻，弹出如下图所示的提示。
![image](http://images.cnblogs.com/cnblogs_com/we-hjb/WindowsLiveWriter/Windows7WinCE6.0_2F7/image_thumb_1.png)
![image](http://images.cnblogs.com/cnblogs_com/we-hjb/WindowsLiveWriter/Windows7WinCE6.0_2F7/image_thumb_2.png)
      可以看到，只要安装VS2005的SP1和针对Win7的SP1 Update即可解决VS2005与Windows 7兼容性的问题。
      安装PB6.0时，也弹出如下图所示的不兼容提示。
![image](http://images.cnblogs.com/cnblogs_com/we-hjb/WindowsLiveWriter/Windows7WinCE6.0_2F7/image_thumb_3.png)
      “联机检查解决方案”似乎也不能提供有用的东西，直接点击“运行程序”按钮即可。
      安装程序都尽量“以管理员身份运行”，否则可能会出现一些异常，如下图所示。
![image](http://images.cnblogs.com/cnblogs_com/we-hjb/WindowsLiveWriter/Windows7WinCE6.0_2F7/image_thumb_4.png)
      在安装PB6.0补丁包时尤其需要注意这一点，因为右键弹出的菜单中没有“以管理员身份运行”这一项。这里得想个小办法，间接的实现“以管理员身份运行”补丁包程序，方法如下： 
```
单击开始按钮
　　在搜索框中输入“cmd”
　　同时按下“Ctrl”键与“Shift”键，然后按“Enter”键
　　这样，打开的命令行窗口即运行于管理员权限下，此时即可正常安装PB6.0的补丁包
```
       为了保险起见，我以这种方式安装了所有的补丁包，如下图所示。
![image](http://images.cnblogs.com/cnblogs_com/we-hjb/WindowsLiveWriter/Windows7WinCE6.0_2F7/image_thumb_5.png)
      安装完成后，首先用CEUpdateCheck工具检查了一下补丁包的状态，如下图所示。
![image](http://images.cnblogs.com/cnblogs_com/we-hjb/WindowsLiveWriter/Windows7WinCE6.0_2F7/image_thumb_6.png)
       可以看到，补丁包的安装完全正常。到这里，我就放心很多了，应该不会有什么大问题。赶紧基于模拟器的BSP定制了一个系统编译试了一下。结果还是出了点小状况。如下图所示。
![image](http://images.cnblogs.com/cnblogs_com/we-hjb/WindowsLiveWriter/Windows7WinCE6.0_2F7/image_thumb_7.png)
      build.log中的提示如下： 
```
makefile.def: Including C:\WINCE600\public\common\oak\misc\sources.ReleaseType_OAK
NMAKE : fatal error U1073: don't know how to make 'C:\WINCE600\OSDesigns\Emulator\Emulator\Wince600\DeviceEmulator_ARMV4I\cesysgen\oak\lib\ARMV4I\retail\atls.lib'
Stop.
NMAKE : fatal error U1077: 'C:\WINCE600\sdk\bin\i386\nmake.exe' : return code '0x2'
Stop.
```
      看起来跟ATL有点关系，但在相应的目录下有这个文件，后来怀疑跟组件有关系，把“Active Template Library(ATL)”组件选中重新编译，果然就没有问题了。
      以上介绍了基于Windows 7旗舰版搭建WinCE6.0开发环境的过程和注意事项，稍微啰唆了点，但还是希望记录的足够详细，也方便新手做一些参考。
//==================================
备注：：
1、今天按上面的方法重装了一下WINCE60，总算把UT6410工程编译通过并跑起来了，看来可以成功编译模拟器工程还不能算是安装成功啊！
补丁包也只打了2009和2010的年包，2011的听说有问题也就没打，2012刚发布也不敢追求最新啊！这个安装顺序有个特别的包----Windows Embedded CE 6.0 R3 Update Rollup 11/3/2009
 这个要注意！！！
//下面的文章提到了2010和2011年补丁包都有问题！！！
[关于wince6.0 升级全年包的注意事项（作者：gooogleman）](http://blog.csdn.net/gooogleman/article/details/7561422)----[http://blog.csdn.net/gooogleman/article/details/7561422](http://blog.csdn.net/gooogleman/article/details/7561422)
2、现在网上很难下载到2009/2010年的年补丁包了，难道装最新的2012年年包就可以了吗？这还真的是个问题！
试了一下还真的是可以啊！----只装了WinCEPB60-R3-2009-Update-Rollup-Armv4I.msi++++WinCEPB60-121231-Product-Update-Rollup-Armv4I.msi，我怀疑只装WinCEPB60-121231-Product-Update-Rollup-Armv4I.msi也是可以的。
