# Qt5.2 for Android 配置及部署到手机运行 - wishchinYang的专栏 - CSDN博客
2015年01月22日 14:43:50[wishchin](https://me.csdn.net/wishchin)阅读数：2472
         使用DNK编程也没有那么难，使用QT为安卓跨平台编程需要安装NDK，SDK通过NDK调用C++程序，偶尔能提高一些效率。
         SDK下载地址：http://developer.android.com/sdk/index.html#download
         原始链接：[http://blog.csdn.net/syrchina/article/details/17335945](http://blog.csdn.net/syrchina/article/details/17335945)   已修改大量原文。
         原始链接：[http://blog.csdn.net/aminfo/article/details/7814304](http://blog.csdn.net/aminfo/article/details/7814304)
**NDK概述**
        NDK全称：**Native Development Kit**。 
　　1、NDK是一系列工具的集合。 
　　* NDK提供了一系列的工具，帮助开发者快速开发C（或C++）的动态库，并能自动将so和java应用一起打包成apk。这些工具对开发者的帮助是巨大的。 
　　* NDK集成了交叉编译器，并提供了相应的mk文件隔离CPU、平台、ABI等差异，开发人员只需要简单修改mk文件（指出“哪些文件需要编译”、“编译特性要求”等），就可以创建出so。
　　* NDK可以自动地将so和Java应用一起打包，极大地减轻了开发人员的打包工作。 
　　2、NDK提供了一份稳定、功能有限的API头文件声明。 
　　Google明确声明该API是稳定的，在后续所有版本中都稳定支持当前发布的API。从该版本的NDK中看出，这些API支持的功能非常有限，包含有：C标准库（libc）、标准数学库（libm）、压缩库（libz）、Log库（liblog）。
**NDK带来什么**
**1、NDK的发布，使“Java+C”的开发方式终于转正，成为官方支持的开发方式。**
　　使用NDK，我们可以将要求高性能的应用逻辑使用C开发，从而提高应用程序的执行效率。使用NDK，我们可以将需要保密的应用逻辑使用C开发。毕竟，Java包都是可以反编译的。NDK促使专业so组件商的出现。（乐观猜想，要视乎Android用户的数量）
**2、NDK将是Android平台支持C开发的开端。**
　　NDK提供了的开发工具集合，使开发人员可以便捷地开发、发布C组件。同时，Google承诺在NDK后续版本中提高“可调式”能力，即提供远程的gdb工具，使我们可以便捷地调试C源码。在支持Android平台C开发，我们能感觉到Google花费了很大精力，我们有理由憧憬“C组件支持”只是Google Android平台上C开发的开端。毕竟，C程序员仍然是码农阵营中的绝对主力，将这部分人排除在Android应用开发之外，显然是不利于Android平台繁荣昌盛的。
软件准备：
1.**QT5.2  for android （windows）版。**
  下载地址：[https://qt-project.org/downloads](https://qt-project.org/downloads)
找到
![](https://img-blog.csdn.net/20131214113241531?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvamluNzYxMTUzNDU0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
  看清是windows 32位哦！别下错了。
2.**Java Platform (JDK) 7u45**
  下载地址：[http://www.oracle.com/technetwork/java/javase/downloads/jdk7-
        downloads-1880260.html](http://www.oracle.com/technetwork/java/javase/downloads/jdk7-downloads-1880260.html)
   按照下图所示步骤，找到jdk-7u45-windows-i586.exe下载。
![](https://img-blog.csdn.net/20131214115939265?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvamluNzYxMTUzNDU0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
   下完安装![](https://img-blog.csdn.net/20131214120251109?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvamluNzYxMTUzNDU0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)。
   安装会让你选择安装路径，希望使用默认的。安装完成你会发现在安装目录里有个2个文件夹。注意我的安装目录为默认的c:\program files\java
![](https://img-blog.csdn.net/20131214120340265?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvamluNzYxMTUzNDU0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
接下来设置环境变量：我的电脑-系统属性-高级，中找到“环境变量”点击新建即可。注意变量值都是以分号为间隔的。
我们新建系统变量ClassPath，填入C:\Program Files\Java\jdk1.7.0_45\bin
然后点确定。
![](https://img-blog.csdn.net/20131214120619265?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvamluNzYxMTUzNDU0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
同样方法再建立一个JAVA_HOME,值为C:\Program Files\Java\jdk1.7.0_45
然后确定。
![](https://img-blog.csdn.net/20131214121329640?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvamluNzYxMTUzNDU0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
再找到Path环境变量，加入qt的安装路径。如果这个变量值中有其他的值，要在我们添加的路径前加个英文态分号哦！;D:\Qt\Qt5.2.0\5.2.0\mingw48_32\bin。如果没有别的值就不要加；了。我把qt
 for android版的qt安装在了D盘。
然后确定。
![](https://img-blog.csdn.net/20131214121024375?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvamluNzYxMTUzNDU0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
3.**adt-bundle-windows-x86-20131030（安卓JDK）**
下载地址：[http://developer.android.com/sdk/index.html#download](http://developer.android.com/sdk/index.html#download)
找到下图所示的标志，点击，接着会弹出一个页面，你点击同意后，选择32位的下载即可。
![](https://img-blog.csdn.net/20131214122758234?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvamluNzYxMTUzNDU0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![](https://img-blog.csdn.net/20131214122813109?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvamluNzYxMTUzNDU0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
下载完成后是个压缩包，你需要解压。
我解压到了F盘，自己建立的android文件夹中了。你也可以跟我学，也在f盘建立一个android文件夹。这样你就可以完全复制我的变量值了。
F:\android\adt-bundle-windows-x86-20131030\sdk\tools填入PATH环境变量中。主要在路径前加个英文的分号哦。
如下图
![](https://img-blog.csdn.net/20131214123542531?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvamluNzYxMTUzNDU0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
接着找到SDK Manager。exe运行。打开后，它会自动检查更新，时间不短。趁这段时间，你可以加 Qt中国群218967017，闲聊下哦。也是不错的选择。大概2分钟左右吧
![](https://img-blog.csdn.net/20131214123701687?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvamluNzYxMTUzNDU0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![](https://img-blog.csdn.net/20131214124206812?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvamluNzYxMTUzNDU0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
然后还是这个软件界面，你会发现有几个包可以安装。具体几个我也记不起了。目前我这是显示一个。接着你点安装按钮。
（再此，安装Tools、Paltform、和相应的安卓版本）
![](https://img-blog.csdn.net/20131214124355968?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvamluNzYxMTUzNDU0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
然后会弹出一个界面点接受Aceept，然后点击安装install
![](https://img-blog.csdn.net/20131214124744375?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvamluNzYxMTUzNDU0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
接着没有更新了，关闭即可。
4.**android-ndk-r9b-windows-x86**
下载地址：[http://developer.android.com/tools/sdk/ndk/index.html](http://developer.android.com/tools/sdk/ndk/index.html)
找到32位下图所示的位置下载。
![](https://img-blog.csdn.net/20131214125232984?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvamluNzYxMTUzNDU0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
下载解压。我还是在F盘的android文件夹里哦。
新建androidNDKHOST环境变量
值为F:\android\android-ndk-r9b-windows-x86\android-ndk-r9b
![](https://img-blog.csdn.net/20131214125046343?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvamluNzYxMTUzNDU0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
5.**apache-ant-1.9.2-bin**
下载地址[http://ant.apache.org/bindownload.cgi](http://ant.apache.org/bindownload.cgi)
找到下图的位置下载
![](https://img-blog.csdn.net/20131214125641187?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvamluNzYxMTUzNDU0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
终于搞定了。接下来就是Creator的部分了
打开android 版 QT
![](https://img-blog.csdn.net/20131214125931062?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvamluNzYxMTUzNDU0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
选择浏览进行路径的配置。然后点击启动Android AVD管理器。添加一个虚拟手机即可。接着等待一会会在列表中显示了。我列表中显示的是名称1.   看看你的列表旁的添加按钮能不能用，如果能用，你直接点添加给个设备名字也行。
![](https://img-blog.csdn.net/20131214130319171?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvamluNzYxMTUzNDU0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

然后建立个android的项目。我建立的是QUICK项目建立过程中会让你选一个环镜。按下图来。
![](https://img-blog.csdn.net/20131214130700343?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvamluNzYxMTUzNDU0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
接着就是运行了。点调试按钮。如果环境你选的不对，就会出现运行错误。在模拟器界面中找到你的项目名，图标为小安卓机器人。
下图所示的我的项目名为untitled4
![](https://img-blog.csdn.net/20131214135033703?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvamluNzYxMTUzNDU0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
点击它运行运行结果。
![](https://img-blog.csdn.net/20131214135820609?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvamluNzYxMTUzNDU0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
这是在电脑上运行。如何在手机上呢。
请找到你的项目调试目录
build-untitled2-Android_for_armeabi_v7a_GCC_4_8_Qt_5_2_0-Debug下的android-build下的bin文件夹中就有apk包。
![](https://img-blog.csdn.net/20131214140636062?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvamluNzYxMTUzNDU0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
将此apk复制到手机中，然后点击安装即可在手机上运行。效果如下
![](https://img-blog.csdn.net/20131214140607578?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvamluNzYxMTUzNDU0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
![](https://img-blog.csdn.net/20131214140515343?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvamluNzYxMTUzNDU0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
最后一句，只剩一句：Q T中国群218967017 欢迎加入。
360云盘文件共享群：QT中国共享群，
打开邀请链接：http://qun.yunpan.360.cn/12187698
输入邀请码：2458
