# 反编译Android APK 源代码和资源文件详解 - xqhrs232的专栏 - CSDN博客
2012年04月17日 16:16:02[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：955
原文地址::[http://zhengxinyu.blog.51cto.com/3729852/779693](http://zhengxinyu.blog.51cto.com/3729852/779693)
 在我们学习Android的时候也要想到别人的功能怎么实现的那如何去参考别人的代码呢？这就是涉及到了一些反编译的问题并且也会存在版权的问题  我们在学习的时间里是多去探究的过程而在商业中希望这是一条不可逾越的线
---------------------------------------------------------------------------------------------------------------------
**先说一下反编译的流程和原理：**
　　1.用apktool 把apk--> 资源包(java代码变成smali文件看不懂的)，可以修改资源包里面的文件。
　　2.apk后缀名改成zip或rar解压，获取 classes.dex 文件，用dex2jar转换成jar包(注：直接解压出来的资源文件是不能直接打开的，要用第一步的反编译工具)。
　　3.用jd-ui等java反编译工具直接查看java代码。
　　4.把java代码和第一版的资源包整到一起重新组成一个新的应用。
　　5.用apktool 重新编译。
　　6.用签名工具重新签名。
　　7.重新发布带新的签名的应用。
　　注：如果不用改java代码，只是换换风格和汉化2.3.4步则不用做。
----------------------------------------------------------------------------------------------------------------------
**一、反编译Apk得到Java源代码**
首先要下载两个工具：dex2jar和JD-GUI
前者是将apk中的classes.dex转化成Jar文件，而JD-GUI是一个反编译工具，可以直接查看Jar包的源代码。以下是下载地址：
**dex2jar：**[http://laichao.googlecode.com/files/dex2jar-0.0.7-SNAPSHOT.zip](http://laichao.googlecode.com/files/dex2jar-0.0.7-SNAPSHOT.zip)
**JD-GUI：**[http://laichao.googlecode.com/files/jdgui.zip](http://laichao.googlecode.com/files/jdgui.zip)
**具体步骤：**
首先将apk文件，将后缀改为zip，解压，得到其中的classes.dex，它就是java文件编译再通过dx工具打包而成的；
解压下载的dex2jar，将classes.dex复制到dex2jar.bat所在目录。在命令行下定位到dex2jar.bat所在目录
运行
dex2jar.bat    classes.dex
生成
classes.dex.dex2jar.jar
生成jar文件的截图如下：
![](http://hi.csdn.net/attachment/201108/28/0_1314542835TY9r.gif)
运行JD-GUI（jd-gui.exe），打开上面生成的jar包，即可看到源代码了
![](http://hi.csdn.net/attachment/201108/28/0_1314542895t80o.gif)
HelloAndroid源码（编译前的apk源码对照）如下：
**![](http://hi.csdn.net/attachment/201108/28/0_1314542945u7Th.gif)**
**二、反编译apk生成程序的源代码和图片、XML配置、语言资源等文件**
如果是只是汉化软件，这将特别有用。
首先还是下载工具，这次用到的是apktool
下载地址：[http://code.google.com/p/android-apktool/downloads/list](http://code.google.com/p/android-apktool/downloads/list)
下载：[apktool1.4.1.tar.bz2](http://android-apktool.googlecode.com/files/apktool1.4.1.tar.bz2) 和 [apktool-install-windows-r04-brut1.tar.bz2](http://android-apktool.googlecode.com/files/apktool-install-windows-r04-brut1.tar.bz2)（两个包都下载）
**具体步骤：**
将下载的两个包解压到同一个文件夹下，应该会有三个文件：aapt.exe，apktool.bat，apktool.jar
在命令行下定位到apktool.bat文件夹，输入以下命令：apktool d C:\*.apk C:\*文件夹，如下图：
![](http://hi.csdn.net/attachment/201108/28/0_1314543082NXJ7.gif)
命令行解释：apktool   d   [apk文件 ]   [输出文件夹]
反编译的文件如下（AndroidManifest.xml为例）：
![](http://hi.csdn.net/attachment/201108/28/0_131454313515no.gif)
特别注意：你要反编译的文件一定要放在C盘的根目录里
将反编译完的文件重新打包成apk，很简单，输入apktool b c:\***文件夹（你编译出来文件夹）即可，命令如下：
![](http://hi.csdn.net/attachment/201108/28/0_1314543163UrUV.gif)
打包apk后的文件在目录C:\HelloAndroid下，生成了两个文件夹：
build
dist
其中，打包生成的HelloAndroid.apk，在上面的dist文件夹下；
之前的方法是平常用命令的方法  在这还有一个图形界面的方法分享学习
--------------------------------------------------------------------------
  有人把apktool做了个封装 弄成exe文件 图形界面的方便使用，截个图出来给大家看看吧
![](http://images.cnblogs.com/cnblogs_com/greatverve/2012013002.png)
　　注：上面那个软件不能签名的，要用另外一个软件(APKSign)来进行签名,截图如下：
![](http://images.cnblogs.com/cnblogs_com/greatverve/2012013003.png)
　　另给出上面两个工具的下载连接 机锋论坛上面的，估计直接点击连接下载不了
　　Xwindows,带图形界面： apktool.rar (3.33 MB) -->这个下载后可以自己到 http://code.google.com/p/android-apktool/ 下载最新版的apktool.jar替换原来的。
　　签名工具：Auto-sign.rar(312.45 KB)
　　另外还有些命令行的工具，本人给APK编辑器不同版本上面的说明不对应浪费了很多时间
　　1.普通apk文件(就是从各种网站上下载下来的第三方软件)
　　(1)把他放到place-apk-here-for-modding文件夹里面。(不要有空格和中文，中文的改成英文的)
　　(2)打开Script.exe，在CMD窗口中输入22或23(设置目前的工程)，选择要编辑的apk文件的编号，回车确认
　　如上：有些版本设置目前的工程选项改成23了，但说明文件没有改过来，按22一直都无法选择apk，原因很不好找。
　　建议还是使用图形界面
本文出自 “[新宇专栏](http://zhengxinyu.blog.51cto.com/)” 博客，请务必保留此出处[http://zhengxinyu.blog.51cto.com/3729852/779693](http://zhengxinyu.blog.51cto.com/3729852/779693)
