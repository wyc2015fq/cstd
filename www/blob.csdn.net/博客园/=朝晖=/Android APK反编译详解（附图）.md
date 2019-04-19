# Android APK反编译详解（附图） - =朝晖= - 博客园
# [Android APK反编译详解（附图）](https://www.cnblogs.com/dhcn/p/7120891.html)
原文之前，贴个反编译工具打包版本：http://www.oschina[.NET](http://lib.csdn.net/base/dotnet)/p/gapktool
http://www.oschina[.net](http://lib.csdn.net/base/dotnet)/question/54100_33457
这段时间在学[Android](http://lib.csdn.net/base/android)应用开发，在想既然是用[Java](http://lib.csdn.net/base/java)开发的应该很好反编译从而得到源代码吧，google了一下，确实很简单，以下是我的实践过程。
在此郑重声明，贴出来的目的不是为了去破解人家的软件，完全是一种学习的态度，不过好像通过这种方式也可以去汉化一些外国软件。
一、反编译Apk得到Java源代码
首先要下载两个工具：dex2jar和JD-GUI
前者是将apk中的classes.dex转化成Jar文件，而JD-GUI是一个反编译工具，可以直接查看Jar包的源代码。以下是下载地址：
dex2jar：[http://laichao.googlecode.com/files/dex2jar-0.0.7-SNAPSHOT.zip](http://laichao.googlecode.com/files/dex2jar-0.0.7-SNAPSHOT.zip)
JD-GUI：[http://laichao.googlecode.com/files/jdgui.zip](http://laichao.googlecode.com/files/jdgui.zip)
具体步骤：
首先将apk文件，将后缀改为zip，解压，得到其中的classes.dex，它就是java文件编译再通过dx工具打包而成的；
解压下载的dex2jar，将classes.dex复制到dex2jar.bat所在目录。在命令行下定位到dex2jar.bat所在目录
运行
dex2jar.bat    classes.dex
生成
classes.dex.dex2jar.jar
生成jar文件的截图如下：
![](http://static.oschina.net/uploads/img/201112/03203123_sHNp.gif)
运行JD-GUI（jd-gui.exe），打开上面生成的jar包，即可看到源代码了
![](http://static.oschina.net/uploads/img/201112/03203124_87Bc.gif)
HelloAndroid源码（编译前的apk源码对照）如下：
![](http://static.oschina.net/uploads/img/201112/03203124_JyhQ.gif)
二、反编译apk生成程序的源代码和图片、XML配置、语言资源等文件
如果是只是汉化软件，这将特别有用。
首先还是下载工具，这次用到的是apktool
下载地址：[http://code.google.com/p/android-apktool/downloads/list](http://code.google.com/p/android-apktool/downloads/list)
下载：[apktool1.4.1.tar.bz2](http://android-apktool.googlecode.com/files/apktool1.4.1.tar.bz2) 和 [apktool-install-windows-r04-brut1.tar.bz2](http://android-apktool.googlecode.com/files/apktool-install-windows-r04-brut1.tar.bz2)（两个包都下载）
具体步骤：
将下载的两个包解压到同一个文件夹下，应该会有三个文件：aapt.exe，apktool.bat，apktool.jar
在命令行下定位到apktool.bat文件夹，输入以下命令：apktool d C:\*.apk C:\*文件夹，如下图：
![](http://static.oschina.net/uploads/img/201112/03203124_JknX.gif)
命令行解释：apktool   d   [apk文件 ]   [输出文件夹]
反编译的文件如下（AndroidManifest.xml为例）：
![](http://static.oschina.net/uploads/img/201112/03203124_5Jv5.gif)
特别注意：你要反编译的文件一定要放在C盘的根目录里
将反编译完的文件重新打包成apk，很简单，输入apktool b c:\***文件夹（你编译出来文件夹）即可，命令如下：
![](http://static.oschina.net/uploads/img/201112/03203124_uaDn.gif)
打包apk后的文件在目录C:\HelloAndroid下，生成了两个文件夹：
build
dist
其中，打包生成的HelloAndroid.apk，在上面的dist文件夹下，Ok

