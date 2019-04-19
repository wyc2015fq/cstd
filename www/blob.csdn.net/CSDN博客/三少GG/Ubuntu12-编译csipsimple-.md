# Ubuntu12-编译csipsimple- - 三少GG - CSDN博客
2013年01月20日 22:39:50[三少GG](https://me.csdn.net/scut1135)阅读数：1696
[http://blog.csdn.net/jingyuansuifeng/article/details/8120523](http://blog.csdn.net/jingyuansuifeng/article/details/8120523)
### [教你如何在Ubuntu12下编译csipsimple](http://blog.csdn.net/jingyuansuifeng/article/details/8120523)
分类： [Ubuntu](http://blog.csdn.net/jingyuansuifeng/article/category/1153555)[CSipSimple](http://blog.csdn.net/jingyuansuifeng/article/category/1267937)[Android](http://blog.csdn.net/jingyuansuifeng/article/category/1269549)2012-10-28
 11:33667人阅读[评论](http://blog.csdn.net/jingyuansuifeng/article/details/8120523#comments)(6)[收藏]()[举报](http://blog.csdn.net/jingyuansuifeng/article/details/8120523#report)
教你如何在虚拟机Ubuntu下编译csipsimple
转载请注明出处：[http://blog.csdn.net/jingyuansuifeng/article/details/8120523](http://blog.csdn.net/jingyuansuifeng/article/details/8120523)
 首先是官方文档：[http://code.google.com/p/csipsimple/wiki/HowToBuild](http://code.google.com/p/csipsimple/wiki/HowToBuild)
1.      安装好VMware
2.      下载CSipSimple-CompilerVM.vmdk，这是官方编译好的环境，可以直接导入至虚拟机。下载地址：[https://docs.google.com/file/d/0ByFooYVveHXdYVV6eVhaUElWaGM/edit?pli=1](https://docs.google.com/file/d/0ByFooYVveHXdYVV6eVhaUElWaGM/edit?pli=1)
3.      启动VMware导入以上vmdk文件
3.1   新建虚拟机
![](https://img-my.csdn.net/uploads/201210/28/1351394612_5218.jpg)
![](https://img-my.csdn.net/uploads/201210/28/1351394630_2602.jpg)
![](https://img-my.csdn.net/uploads/201210/28/1351394643_1004.jpg)
![](https://img-my.csdn.net/uploads/201210/28/1351394676_7963.jpg)
![](https://img-my.csdn.net/uploads/201210/28/1351394687_3674.jpg)
![](https://img-my.csdn.net/uploads/201210/28/1351394704_3034.jpg)
注意：上图第一个是你虚拟机的名字，第二个是虚拟机存放的位置，你都可以随便设这，但是一定要自己记得。
![](https://img-my.csdn.net/uploads/201210/28/1351394822_3953.jpg)
![](https://img-my.csdn.net/uploads/201210/28/1351394859_4873.jpg)
上图分配内存一般根据自己机器情况设定，我选的是默认
![](https://img-my.csdn.net/uploads/201210/28/1351394886_9510.jpg)
上图是网略配置，我选的是默认
![](https://img-my.csdn.net/uploads/201210/28/1351394916_9752.jpg)
![](https://img-my.csdn.net/uploads/201210/28/1351394945_5890.jpg)
上图选择已经存在的虚拟机，也就你前面下载好的CSipSimple-CompilerVM.vmdk。
![](https://img-my.csdn.net/uploads/201210/28/1351394968_6888.jpg)
![](https://img-my.csdn.net/uploads/201210/28/1351395021_6590.jpg)
选择你下载好的虚拟机。使用已存在的格式
好了，至此虚拟环境就准备好了，下面我们看看里面都有哪些东西。
![](https://img-my.csdn.net/uploads/201210/28/1351395040_9812.jpg)
4.      编译Csipsimple
看到桌面有一个文件Before doing anything else PLEASE read this document first!.txt
原文如下：
Welcometo the Virtual Machine v0.1 for compiling cSipSimple.
#Disclaimer:We do not guarantee that this machine does everything correctly.
#Description
Thismachine can compile cSipSimple incl. the native libaries.
It isbased upon the description http://code.google.com/p/csipsimple/wiki/HowToBuild
#Get started
1. Read the HowToBuild-Pagehttp://code.google.com/p/csipsimple/wiki/HowToBuild
2. The project source is checkout to the desktop(/home/user/Desktop).
3. Before compile get the latest version from the repository:"svn up" for ActionBarSherlock and cSipSimple)
4. Check Eclipse plugins for updates: OpenEclipse->Help->Check for Updates and install all updates
5. Update NDK and SDK: Open Eclipse DDMS and download latestandroid version, latest NDK and SDK
6. Rebuild the native libraries: make update
7. Open Eclipse and check that you are compiling against thecorrect Android API (often the latest)
8. Try to run cSipSimple in emulator
9. Congratulation!
#Ifsomething compilation fails>
1. Checkthat the setup of the virtual machine is similar as descripted byhttp://code.google.com/p/csipsimple/wiki/HowToBuild
2. Checkthat you did nothing wrong.
3. Checkagain.
4. Check thegoogle developer group: http://groups.google.com/group/csipsimple-dev/
5. Checkthe google developer group carefully again
6.Compile a USEFUL, i.e. descriptive and complete report, and send it to thedeveloper group incl. a useful and descriptive title
7.(Optional) Post the solution if found
Technicaldetails:
* Ubuntu12.04 32bit
* AndroidNDK 8
* AndroidSDK 20
* Eclipsewith subclipse
* Use sudo to get root; password is identical to theusername
所以我们就按照他的步骤来，这时候要淡定
注意：这里我没有进行更新
然后就是：make （我也没有用 make update），这边更慢，大家一定要淡定。。。63M的东西要下载90分钟真心伤不起，下面有惊喜！
![](https://img-my.csdn.net/uploads/201210/28/1351395236_7863.jpg)
所以这里建议大家事先下载好SILK_SDK_SRC_v1.0.8.zip把文件放在CSipSimple/jni/silk下
下载地址：[http://developer.skype.com/silk/SILK_SDK_SRC_v1.0.8.zip](http://developer.skype.com/silk/SILK_SDK_SRC_v1.0.8.zip)
然后修改CSipSimple/jni/silk下的makefile去掉
@wget [http://developer.skype.com/silk/SILK_SDK_SRC_v1.0.8.zip;\](http://developer.skype.com/silk/SILK_SDK_SRC_v1.0.8.zip;/)
然后再运行make ，就会看到下面的画面
![](https://img-my.csdn.net/uploads/201210/28/1351395260_4934.jpg)
如果这边还是嫌慢的话，直接把解压好的SILK_SDK_SRC_v1.0.8复制到jni/opus目录下。
然后你就开始慢慢等吧。。。。。。
最后会出现
![](https://img-my.csdn.net/uploads/201210/28/1351395288_2871.jpg)
不过有两个错误，不知道为什么，先记着不去管它
打开eclipse
![](https://img-my.csdn.net/uploads/201210/28/1351395313_6019.jpg)
编译的时候有点慢，要淡定。。。
这时候要刷新下工程，然后clean下
![](https://img-my.csdn.net/uploads/201210/28/1351395329_6478.jpg)
现在就没有错误了
然后run一下，就看到效果了，这里我实在真机上测试的
![](https://img-my.csdn.net/uploads/201210/28/1351395343_7896.jpg)
OK，到此结束，由于我没有搭建服务器，所以通话怎么样就看你们了，基本的编译就那么多。至于以上两个error问题，有兴趣的人可以按照Before doing anything else PLEASE read this document first!.txt这个文档来，看看还有没有错误。
### [csipsimple编译遇到的问题总结](http://blog.csdn.net/harry_helei/article/details/7482724)
分类： [Android](http://blog.csdn.net/harry_helei/article/category/737895)2012-04-20
 20:261712人阅读[评论](http://blog.csdn.net/harry_helei/article/details/7482724#comments)(21)[收藏]()[举报](http://blog.csdn.net/harry_helei/article/details/7482724#report)
        最近在搞sip视频通话，选的是开源工程csipsimple工程。csipsimple用的sip协议栈是pjsip，也是一个开源的协议栈。多媒体协议栈用用的是开源的pjmedia。同时为了支持更多的音视频编解码，还引入众多开源编解码工程，如ffmpeg, silk, g726, g729等等。一路将在Ubuntu10.04系统上编下来遇到的问题总结如下：
# [CSipSimple短信发送机制探究](http://www.cnblogs.com/dwayne/archive/2012/07/10/csipsimple.html)
CSipSimple是运行在android设备上的一个开源的sip协议应用程序，介绍就不多说了网上一堆，有兴趣的童鞋可以研究下，项目地址为：[http://code.google.com/p/csipsimple/](http://code.google.com/p/csipsimple/)
本文[wayne](http://www.cnblogs.com/dwayne/)将就其中的短信发送机制进行大致分析。
项目中，短信发送利用了AIDL方法来实现。aidl是 Android Interface definition language的缩写，它是一种android内部进程通信接口的描述语言，通过它来定义进程间的通信接口，完成IPC（Inter-Process Communication，进程间通信）。
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

需求说明：
我最新研究发现，这个BUG是因为项目编译的时候没编译进去 libpjsipjni.so 。您可以看一下官方的APK中解压后有一个lib目录，源码编译是不会出现的。我尝试安装官方的APK后删除LIB中的libpjsipjni.so，官方的程序也会和源码编译出来的APK一样悲剧(~~2011-11-08 11:10:28~~)
另外说明一下，不可以在NDK环境下编译的。我要求必须在WINDOWS下的 eclipse 中成功编译并正常使用！谢谢大家(~~2011-11-08 12:10:12~~)
## VoIP测试环境搭建
![](https://img-my.csdn.net/uploads/201301/20/1358693031_5807.jpg)
