# WinCE5.0/6.0下，通过command line实现自动化编译 - xqhrs232的专栏 - CSDN博客
2013年08月21日 22:39:43[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：717
个人分类：[WINCE](https://blog.csdn.net/xqhrs232/article/category/906924)
原文地址::[http://space.itpub.net/16912323/viewspace-504721](http://space.itpub.net/16912323/viewspace-504721)
先来解释一下：一般我们要编译WinCE都是先创建工程，然后再IDE的开发环境中进行编译。对于WinCE5.0来说，是用Platform.
 Builder5.0；对于WinCE6.0来说，就是用VS2005，因为PB6.0已经集成到VS2005里面了。这样有个问题，就是每次都要人来手动编译。让我来举个例子，这是我在做项目中遇到的。公司基于ARM的平台开发BSP，而且已经release了，但是公司有很多产品都基于这个平台，每个平台根据功能不同对WinCE进行了不同程度的裁剪，所以每个产品对应一个工程。这样问题就出现了，如果要进行测试，我们就需要有人手动的把每一个工程都编译出来，然后才能进行测试，这样会很耽误时间。如果是用自动化编译，只需要通过脚本命令把要编译的工程输入，然后运行，到了第二天早上，所有产品的image就都编译出来了，这样会很节省时间。
  
现在就开始介绍如何实现，有个前提是你已经创建了一个WinCE工程并且在里面为这个工程创建了SDK，当然如果你不需要导出SDK的话，你就不需要创建SDK了。WinCE5.0和WinCE6.0的命令有些小的区别，我会分开介绍：  
对于WinCE5.0：
1. pbxmlutils.exe
 在“\Windows CE Platform. Builder\5.00\CEPB\BIN”下面可以找到，这个命令可以从PB的文档中查到。主要作用是通过解析你的工程文件<OS Design Name>.pbxml来获得与该工程相关的环境变量，并产生一个脚本来设置这些环境变量。格式如下：  
pbxmlutils /getbuildenv /workspace "%_WINCEROOT%\PBWorkspaces\<OS design name>\<OS design name>.pbxml" /config "<BSP>: <Target device>" > SetEnv.bat
看着可能有点晕，我来举个例子：
pbxmlutils /getbuildenv /workspace "%_WINCEROOT%\PBWorkspaces\MyWorkspace\MyWorkspace.pbxml" /config "CEPC: x86_Release" > SetEnv.bat
其实就两个参数，第一个参数很多人都知道如何设置，第二个参数config可能有些人不太了解，你可以打开你的工程，然后查环境变量PBCONFIG的值，用这个值就可以了。
2. Blddemo.exe
这个应该比较熟悉。编译WinCE的时候，实际上就是调用Blddemo -q，不多说了。
3. buildsdk.exe
这个是用来编译SDK的。格式如下；
buildsdk [MyOSDesign.pbxml]
很简单，不过还是给个例子：
buildsdk "%_WINCEROOT%\PBWorkspaces\MyWorkspace\MyWorkspace.pbxml"    
对于WinCE6.0：
1. pbxmlutils.exe
在“\Microsoft Platform. Builder\6.00\cepb\IdeVS”下面可以找到，这个命令可以从PB的文档中查到。主要作用是通过解析你的工程文件<OS Design Name>.pbxml来获得与该工程相关的环境变量，并产生一个脚本来设置这些环境变量。格式如下：  
pbxmlutils /getbuildenv /workspace "%_WINCEROOT%\OSDesigns\<OS design name>\<OS design name>\<OS design name>.pbxml" /config "<BSP> <Target device>" > SetEnv.bat
来举个例子：
pbxmlutils /getbuildenv /workspace "C:\WINCE600\OSDesigns\MyWorkspace\MyWorkspace\MyWorkspace.pbxml" /config "CEPC x86 Release" > SetEnv.bat
和WinCE5.0有些小的区别，要注意一下。同样如果不知道第二个参数config如何设置，可以打开工程，查环境变量PBCONFIG的值，用这个值就可以。
2. Blddemo.exe
这个和WinCE5.0用法一样。编译WinCE的时候，实际上就是调用Blddemo -q。
3. buildsdk.exe
这个是用来编译SDK的。格式如下；
buildsdk [MyOSDesign.pbxml] [MySdkConfig.sdkcfg]
很简单，不过还是给个例子：
buildsdk "\OSDesigns\MyWorkspace\MyWorkspace\MyWorkspace.pbxml" "\OSDesigns\MyWorkspace\MyWorkspace\SDKs\SDK1\SDK1.sdkcfg"  
基本上，自动化编译所需的主要命令都已经介绍完了，下面介绍如何使用上面的命令实现：  
对于WinCE5.0：
@echo off
@set _WINCEROOT=c:\wince500
@set _PBROOT="c:\Program Files\Windows CE Platform. Builder\5.00"
@set PATH=%PATH%;c:\Program Files\Windows CE Platform. Builder\5.00\cepb\bin
cd /D %_WINCEROOT%
if exist build.log del build.log
if exist build.wrn del build.wrn
if exist build.err del build.err
pbxmlutils /getbuildenv /workspace "%_WINCEROOT%\PBWorkspaces\MyDesign\MyDesign.pbxml" /config "Samsung SMDK2410: ARMV4I_Release" > buildenv.bat
call buildenv.bat
call %_TARGETPLATROOT%\%_TGTPLAT%.bat
call Blddemo -q
buildsdk "%_WINCEROOT%\PBWorkspaces\MyDesign\MyDesign.pbxml"
对于WinCE6.0：
@echo off
@set _WINCEROOT=d:\wince600
@set _PBROOT=C:\Program Files\Microsoft Platform. Builder\6.00
@set PATH=%_PBROOT%\cepb\idevs;%PATH%
cd /D %_WINCEROOT%
if exist build.log del build.log
if exist build.wrn del build.wrn
if exist build.err del build.err
pbxmlutils /getbuildenv /workspace "%_WINCEROOT%\OSDesigns\MyDesign\MyDesign\MyDesign.pbxml" /config "CEPC x86 Release" > buildenv.bat
call buildenv.bat
call %_TARGETPLATROOT%\%_TGTPLAT%.bat
call Blddemo -q
call buildsdk "%_WINCEROOT%\OSDesigns\MyDesign\MyDesign\MyDesign.pbxml" "%_WINCEROOT%\OSDesigns\MyDesign\MyDesign\SDKs\SDK1\SDK1.sdkcfg"
上面针对WinCE5.0和WinCE6.0的两个命令略有不同，但是原理是一样的。只要有点Windows脚本命令的知识，了解WinCE的整个编译过程，看懂上面的代码应该没问题，这里就不作介绍了。
上面给出的只是例子，上面的代码，可以实现command line编译WinCE并且还可以导出SDK。如果想实现对多个工程进行编译，只需要对上面的脚本作些修改就可以了。上面的脚本完全可以改写成函数的形式，通过传递工程文件名和平台的config来编译WinCE，这样就可以对多个工程顺序进行编译，这里就不介绍了。
