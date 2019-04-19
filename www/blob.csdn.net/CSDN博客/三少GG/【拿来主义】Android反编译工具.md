# 【拿来主义】Android反编译工具 - 三少GG - CSDN博客
2012年03月20日 11:38:57[三少GG](https://me.csdn.net/scut1135)阅读数：3405标签：[android																[反编译工具																[java																[工具																[jar																[layout](https://so.csdn.net/so/search/s.do?q=layout&t=blog)](https://so.csdn.net/so/search/s.do?q=jar&t=blog)](https://so.csdn.net/so/search/s.do?q=工具&t=blog)](https://so.csdn.net/so/search/s.do?q=java&t=blog)](https://so.csdn.net/so/search/s.do?q=反编译工具&t=blog)](https://so.csdn.net/so/search/s.do?q=android&t=blog)
个人分类：[Android 常规](https://blog.csdn.net/scut1135/article/category/1057214)
# [Android学习笔记之反编译工具介绍及下载（Class文件反编译，xml文件反编译整理）](http://www.cnblogs.com/salam/archive/2010/09/24/1833818.html)
　　最近在网上查了一些Android文件反编译工具，供大家参考。
1.CLASS文件反编译工具：XJAD
![](http://pic002.cnblogs.com/images/2010/133128/2010092412065159.png)
![](http://pic002.cnblogs.com/images/2010/133128/2010092412075320.png)
2.xml文件工具AXMLPrinter2.jar
将它放到android-sdk-windows-1.5_r3\tools文件夹中
运行cmd,进入tools目录，运行java -jar AXMLPrinter2.jar main.xml > main.txt
![](http://pic002.cnblogs.com/images/2010/133128/2010092412124534.png)
这时我们会看到生成一个main.txt文件，就是反编译后的
3.dex文件反编译dex2jar.bat
1.首先找到Android软件安装包中的class.dex
     把apk文件改名为.zip，然后解压缩其中的class.dex文件，它就是java文件编译再通过dx工具打包成的,所以现在我们就用上述提到的2个工具来逆方向导出java源文件;
 2.把class.dex拷贝到dex2jar.bat所在目录。
     运行dex2jar.bat classes.dex，生成classes.dex.dex2jar.jar
![](http://pic002.cnblogs.com/images/2010/133128/2010092412183412.png)
这时你将看到生成的jar包，然后通过上述的XJAD反编译class文件。
[工具下载地址](http://files.cnblogs.com/salam/Re.rar)
[](http://blog.csdn.net/sunboy_2050/article/details/6727581)
### Android APK反编译详解（附图）
分类： [Android](http://blog.csdn.net/sunboy_2050/article/category/853274)2011-08-28
 22:4235315人阅读[评论](http://blog.csdn.net/sunboy_2050/article/details/6727581#comments)(33)[收藏](http://blog.csdn.net/sunboy_2050/article/details/6727581)[举报](http://blog.csdn.net/sunboy_2050/article/details/6727581#report)
这段时间在学Android应用开发，在想既然是用Java开发的应该很好反编译从而得到源代码吧，google了一下，确实很简单，以下是我的实践过程。
在此郑重声明，贴出来的目的不是为了去破解人家的软件，完全是一种学习的态度，不过好像通过这种方式也可以去汉化一些外国软件。
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
下载地址：[](http://code.google.com/p/android-apktool/)[http://code.google.com/p/android-apktool/downloads/list](http://code.google.com/p/android-apktool/downloads/list)
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
其中，打包生成的HelloAndroid.apk，在上面的dist文件夹下，Ok
**注：**本android反编译教程，是在Windows 7  Ultimate  64bit ，测试通过
[上述反编译工具包下载](http://d.download.csdn.net/down/3556426/sunboy_2050)
另外，作为应用开发者，肯定不希望自己的代码被反编译的，下一遍博客中讲述如何通过混淆代码放在反编译：
[Android如何防止apk程序被反编译](http://blog.csdn.net/sunboy_2050/article/details/6727640)
### [Android反编译工具绿色版V2.0（改进版）](http://morry.iteye.com/blog/776718)
**博客分类：**- [android](http://morry.iteye.com/category/119459)
[Android](http://www.iteye.com/blogs/tag/Android)[XML](http://www.iteye.com/blogs/tag/XML)[Google](http://www.iteye.com/blogs/tag/Google)
更多内容请到我的博客:[http://www.mikameng.com](http://mikameng.com/)
之前一篇文章写过   [自制Android反编译工具](http://morry.iteye.com/blog/769521) ， 用到了2个开源工具dex2Jar和AXMLPrinter2.
用dex2jar反编译dex 到jar没有问题，但是用AXMLPrinter2反编译的XML文件有些地方不太好， 比如标题，颜色，还有Id属性， layout属性等等 其他属性，这个文件应该将编译后的xml文件直接反编译回来的, 可读性不强。 
请看下面一个用AXMLPrinter2反编译后的xml文件，以反编译支付宝手机客户端 为例 ， 这个xml文件是支付宝about 文件（alipay_about_320_480.xml）：
![](http://dl.iteye.com/upload/attachment/324678/2829543c-fffd-38c6-8b7d-58394c93c966.png)
从上面的文件可以看到， 反编译后的文件可读性不好。
最近发现有一个开源工具apktool（http://code.google.com/p/android-apktool/）， 这个工具功能比较强悍， 可以反编译成源码，xml文件和语言资源文件； 我们还可以对xml文件进行修改（汉化）， 然后在利用此工具编译打包APK，然后签名后就可以在手机上安装了。 简单来说apktool 可以反编译apk文件， 也可以编译源码打包成APK文件。
V2.0 修改：  用apktoo替换AXMLPrinter2 来反编译xml文件。
软件主界面：
![](http://dl.iteye.com/upload/attachment/324660/9ef16477-c9ad-3a60-ab15-ac488dc04bfe.gif)
还是以上面的about页面为例子， 展示一下apktool反编译的效果：
![](http://dl.iteye.com/upload/attachment/324674/d505e112-4f1a-3bcc-b566-ce64a5592b9b.png)
这个反编译的效果可读性比较好了吧， 通过这个例子可以看到AXMLPrinter2 和 apktool的差距了吧。
**1. 反编译后的apk主目录如下图：**
![](http://dl.iteye.com/upload/attachment/324662/9a3a2fc1-9e43-32a2-8c4c-fe718edbfad9.gif)
**2. res文件目录：**
![](http://dl.iteye.com/upload/attachment/324664/7cd06ea9-610f-3967-90bd-57c16b53372d.gif)
如果使用AXMLPrinter2来进行反编译， 就没有values文件夹。
**3.登录菜单**
![](http://dl.iteye.com/upload/attachment/324666/c644dc33-1de9-3633-b649-cf15c7cb7f90.gif)
![](http://dl.iteye.com/upload/attachment/324672/fb7e3b6b-8cd0-3aa4-a75c-890f190d9ccb.png)
**4. 看下res/values下的string.xml，**如果使用AXMLPrinter2来进行反编译， 就没有values文件夹 。
![](http://dl.iteye.com/upload/attachment/324670/4d3f837f-e0e1-3a6a-bd2a-971c5f210ad8.gif)
声明： 本软件仅供大家学习之用， 请尊重软件原版。
- [Android反编译工具.rar](http://dl.iteye.com/topics/download/18c8c261-e8af-38b8-ab11-d55afd938d21) (3.8 MB)
- 
下载次数: 3160
- [查看图片附件](http://morry.iteye.com/blog/776718#)
# Android程序如何反编译？
发表于[2010 年 11 月 10 日](http://toneygong.com/?p=31)由[admin](http://toneygong.com/?author=1)
### Apk文件的格式
Android application package文件。每个要安装到android平台的应用都要被编译打包为一个单独的文件，后缀名为.apk，其中包含了应用的二进制代码、资源、配置文件等。
apk文件实际是一个zip压缩包，可以通过解压缩工具解开。可以用zip解开*.apk文件，下面是一个helloword的apk示例文件
|- AndroidManifest.xml
|- META-INF
| |- CERT.RSA
| |- CERT.SF
| `- MANIFEST.MF
|- classes.dex
|- res
| |- drawable
| | `- icon.png
| `- layout
| `- main.xml
`- resources.arsc
- 
Manifest文件：AndroidManifest.xml是每个应用都必须定义和包含的，它描述了应用的名字、版本、权限、引用的库文件等等信息[ , ]，如要把apk上传到Google Market上，也要对这个xml做一些配置。注意：在apk中的xml文件是经过压缩的，不可以直接打开。
- 
Res文件：res文件夹下为所有的资源文件。
- 
resources.arsc文件：为编译后的二进制资源文件，许多做汉化软件的人都是修改该文件内的资源以实现软件的汉化的。
- 
META-INF目录：META-INF目录下存放的是签名信息，用来保证apk 包的完整性和系统的安全。在eclipse编译生成一个api包时，会对所有要打包的文件做一个校验计算，并把计算结果放在META-INF目录下。而在 OPhone平台上安装apk包时，应用管理器会按照同样的算法对包里的文件做校验，如果校验结果与META-INF下的内容不一致，系统就不会安装这个 apk。这就保证了apk包里的文件不能被随意替换。比如拿到一个apk包后，如果想要替换里面的一幅图片，一段代码，
 或一段版权信息，想直接解压缩、替换再重新打包，基本是不可能的。如此一来就给病毒感染和恶意修改增加了难度，有助于保护系 统的安全。
- 
classes.dex是java源码编译后生成的java字节码文件。但由于Android使用的dalvik虚拟机与标准的java虚拟机是不兼容的，dex文件与class文件相比，不论是文件结构还是opcode都不一样。
### XML文件的反编译
在apk中的xml文件是经过压缩的，可以通过AXMLPrinter2工具解开，具体命令为：
java -jar AXMLPrinter2.jar AndroidManifest.xml
HelloAndroid程序中Manifest文件的实例：
<?xml version=”1.0″ encoding=”utf-8″?>
<manifest
xmlns:android=”http://schemas.android.com/apk/res/android”
android:versionCode=”1″
android:versionName=”1.0″
package=”name.feisky.android.test”
>
<application
android:label=”@7F040001″
android:icon=”@7F020000″
>
<activity
android:label=”@7F040001″
android:name=”.HelloAndroid”
>
<intent-filter
>
<action
android:name=”android.intent.action.MAIN”
>
</action>
<category
android:name=”android.intent.category.LAUNCHER”
>
</category>
</intent-filter>
</activity>
</application>
<uses-sdk
android:minSdkVersion=”6″
>
</uses-sdk>
</manifest>
而原文件为：
<?xml version=”1.0″ encoding=”utf-8″?>
<manifest xmlns:android=”http://schemas.android.com/apk/res/android”
package=”name.feisky.android.test”
android:versionCode=”1″
android:versionName=”1.0″>
<application android:icon=”@drawable/icon” android:label=”@string/app_name”>
<activity android:name=”.HelloAndroid”
android:label=”@string/app_name”>
<intent-filter>
<action android:name=”android.intent.action.MAIN” />
<category android:name=”android.intent.category.LAUNCHER” />
</intent-filter>
</activity>
</application>
<uses-sdk android:minSdkVersion=”6″ />
</manifest>
### classes.dex文件反编译
classes.dex是java源码编译后生成的java字节码文件。但由于Android使用的dalvik虚拟机与标准的java虚拟机是不兼容的，dex文件与class文件相比，不论是文件结构还是opcode都不一样。目前常见的java反编译工具都不能处理dex文件。
Android模拟器中提供了一个dex文件的反编译工具，dexdump。用法为首先启动Android模拟器，把要查看的dex文件用adb push上传的模拟器中，然后通过adb shell登录，找到要查看的dex文件，执行dexdump xxx.dex。但是这样得到的结果，其可读性是极差的。下面介绍一个可读性比较好的工具。
**工具准备：**
1、把dex文件反编译为jar文件的工具。（[dex2jar](http://toneygong.com/wp-content/uploads/2010/11/dex2jar-0.0.7.5-SNAPSHOT.zip)）
2、把jar反编译为java的工具。（[JD-GUI](http://toneygong.com/wp-content/uploads/2010/11/jd-gui.exe)）
**反编译的步骤**
1、从APK中提取classes.dex文件，对APK文件解压即可得到。 将其放到dex2jar的目录下，打开cmd，运行dex2jar.bat classes.dex，生成classes.dex.dex2jar.jar。
2、运行JD-GUI工具，打开上面的jar文件，即可看到源代码。
HelloAndroid实例：
package name.feisky.android.test;
import android.app.Activity;
import android.os.Bundle;
public class HelloAndroid extends Activity
{
public void onCreate(Bundle paramBundle)
{
super.onCreate(paramBundle);
setContentView(2130903040);
}
}
其原程序为：
package name.feisky.android.test;
import android.app.Activity;
import android.os.Bundle;
public class HelloAndroid extends Activity {
/** Called when the activity is first created. */
@Override
public void onCreate(Bundle savedInstanceState) {
super.onCreate(savedInstanceState);
setContentView(R.layout.main);
}
}
### [Android 中的拿来主义(编译，反编译，AXMLPrinter2,smali,baksmali)!](http://blog.csdn.net/Android_Tutor/article/details/5724435)
分类： [Android高手进阶](http://blog.csdn.net/Android_Tutor/article/category/674246)[Android入门教程](http://blog.csdn.net/Android_Tutor/article/category/605362)[Android基础教程](http://blog.csdn.net/Android_Tutor/article/category/605365)2010-07-09
 22:2332756人阅读[评论](http://blog.csdn.net/Android_Tutor/article/details/5724435#comments)(66)[收藏](http://blog.csdn.net/Android_Tutor/article/details/5724435)[举报](http://blog.csdn.net/Android_Tutor/article/details/5724435#report)
**一、前言：**
大家好，今天给大家分享一下Android中的拿来主 义，我们时常会碰到一个自己觉得很漂亮很帅气的应用(apk),所以我们会尝试用WinRAR等之类工具查看，而一般的应用程序打包后的目录通常是这样的 如下图：
![](http://hi.csdn.net/attachment/201007/6/0_127842027099yj.gif)
当然**res里的图片是可以拿来就用的(笔者的好多应用的图片都是从别人的apk里扣出来的)**，而诸如layout里的布局及权限文件 (AndroidManifest.xml)已经是一堆乱码了，完全看不懂，想看看别人是怎么布局的都不容易。还有源代码都被编译成了 classes.dex，完全看不出什么线索。基于以上的困惑，笔者给大家分享一下Android中的拿来主义。
**二、所需工具(点击各自连接进入下载页面)：**
1.[AXMLPrinter2.jar](http://code.google.com/p/android4me/downloads/list)
2.[baksmali.jar](http://code.google.com/p/smali/)
3.[smali.jar](http://code.google.com/p/smali/)
**三、准备工作**
为了方便起见，作者把AXMLPrinter2.jar, 还有baksmali.jar,还有smali.jar(下下来为了方便重命名)，放在Android SDK tools文件夹中如下图所示:
![](http://hi.csdn.net/attachment/201007/6/0_1278422814OtL8.gif)
为了便于大家更容易程序比对，作者写了一个简单的应用(叫APKInstaller)目录结构如下图所示:
![](http://hi.csdn.net/attachment/201007/6/0_1278423065QDw8.gif)
**四、开始拿来主义**
**1.用 AXMLPrinter2.jar查看apk中的布局xml文件:**
将ApkInstaller应用生成的ApkInstaller.apk(为了方便起见放到tools目录里)用WinRAR等 工具打开，将res/layout/main.xml解压出来(也还是放在tools目录里哦)
打开main.xml文件，内容如下(一堆天文):
![](http://hi.csdn.net/attachment/201007/6/0_12784234694nns.gif)
这时候AXMLPrinter2.jar派上用场了，打开cmd终端，一直进入到tools目录下，输入如下命令:
**java -jar AXMLPrinter2.jar main.xml > main.txt.**  (如下图所示)
![](http://hi.csdn.net/attachment/201007/6/0_12784239040J8s.gif)
打开main.txt代码如下(是不是有个123了呵呵~):
**[java]**[view
 plain](http://blog.csdn.net/Android_Tutor/article/details/5724435#)[copy](http://blog.csdn.net/Android_Tutor/article/details/5724435#)
- <?xml version="1.0" encoding="utf-8"?>  
- <LinearLayout  
-     xmlns:android="http://schemas.android.com/apk/res/android"
-     android:orientation="1"
-     android:layout_width="-1"
-     android:layout_height="-1"
-     >  
-     <WebView  
-         android:id="@7F050000"
-         android:layout_width="-1"
-         android:layout_height="-2"
-         >  
-     </WebView>  
- </LinearLayout>  
为了比对打开源程序中的main.xml代码如下(大家比对一下吧):
**[java]**[view
 plain](http://blog.csdn.net/Android_Tutor/article/details/5724435#)[copy](http://blog.csdn.net/Android_Tutor/article/details/5724435#)
- <?xml version="1.0" encoding="utf-8"?>  
- <LinearLayout xmlns:android="http://schemas.android.com/apk/res/android"
-     android:orientation="vertical"
-     android:layout_width="fill_parent"
-     android:layout_height="fill_parent"
-     >  
- <WebView  
-     android:id="@+id/apk_web"
-     android:layout_height="wrap_content"
-     android:layout_width="fill_parent"
- 
- />  
- </LinearLayout>  
**2.用baksmali.jar反编译classes.dex:**
将ApkInstaller.apk里的classes.dex解压到tools目录里，然后baksmali.jar就派上用场了，在cmd命令行里输入如下命令:
j**ava -jar baksmali.jar -o classout/ classes.dex** .(如下图所示:)
![](http://hi.csdn.net/attachment/201007/9/0_1278689088FZzB.gif)
你将会发现在tools里多一个classout文件夹里面(我代码的包目录结构清晰可见呀),如下图所示:
![](http://hi.csdn.net/attachment/201007/9/0_1278689230oeZ2.gif)
从上面看出除了Android本身资源的类R开头的，我的源程序里只有一个ApkInstaller.java，完全吻合，真TMD的猥琐呵呵~
下面我们看一下ApkInstaller.smali内容是什么，如以下代码：
**[java]**[view
 plain](http://blog.csdn.net/Android_Tutor/article/details/5724435#)[copy](http://blog.csdn.net/Android_Tutor/article/details/5724435#)
- .classpublic Lcom/tutor/apkinstaller/ApkInstaller;  
- .super Landroid/app/Activity;  
- .source "ApkInstaller.java"
- # instance fields  
- .field private apkWeb:Landroid/webkit/WebView;  
- # direct methods  
- .method public constructor <init>()V  
-     .registers 1
-     .prologue  
-     .line 8
-     invoke-direct {p0}, Landroid/app/Activity;-><init>()V  
- return-void
- .end method  
- # virtual methods  
- .method public onCreate(Landroid/os/Bundle;)V  
-     .registers 5
-     .parameter "savedInstanceState"
-     .prologue  
-     .line 13
-     invoke-super {p0, p1}, Landroid/app/Activity;->onCreate(Landroid/os/Bundle;)V  
-     .line 14
- const/high16 v2, 0x7f03
-     invoke-virtual {p0, v2}, Lcom/tutor/apkinstaller/ApkInstaller;->setContentView(I)V  
-     .line 15
- const/high16 v2, 0x7f05
-     invoke-virtual {p0, v2}, Lcom/tutor/apkinstaller/ApkInstaller;->findViewById(I)Landroid/view/View;  
-     move-result-object v2  
-     check-cast v2, Landroid/webkit/WebView;  
-     iput-object v2, p0, Lcom/tutor/apkinstaller/ApkInstaller;->apkWeb:Landroid/webkit/WebView;  
-     .line 16
-     iget-object v2, p0, Lcom/tutor/apkinstaller/ApkInstaller;->apkWeb:Landroid/webkit/WebView;  
-     invoke-virtual {v2}, Landroid/webkit/WebView;->getSettings()Landroid/webkit/WebSettings;  
-     move-result-object v1  
-     .line 17
-     .local v1, webSettings:Landroid/webkit/WebSettings;  
- const/4 v2, 0x1
-     invoke-virtual {v1, v2}, Landroid/webkit/WebSettings;->setJavaScriptEnabled(Z)V  
-     .line 19
- const-string v0, "http://frankiewei.net/apk/demos/main/index.html#home"
-     .line 20
-     .local v0, apkUrl:Ljava/lang/String;  
-     iget-object v2, p0, Lcom/tutor/apkinstaller/ApkInstaller;->apkWeb:Landroid/webkit/WebView;  
-     invoke-virtual {v2, v0}, Landroid/webkit/WebView;->loadUrl(Ljava/lang/String;)V  
-     .line 21
- return-void
- .end method  
同样为了比对我们看一下ApkInstaller.java的源代码如下:
**[java]**[view
 plain](http://blog.csdn.net/Android_Tutor/article/details/5724435#)[copy](http://blog.csdn.net/Android_Tutor/article/details/5724435#)
- package com.tutor.apkinstaller;  
- import android.app.Activity;  
- import android.os.Bundle;  
- import android.webkit.WebSettings;  
- import android.webkit.WebView;  
- publicclass ApkInstaller extends Activity {  
- 
- private WebView apkWeb;  
- @Override
- publicvoid onCreate(Bundle savedInstanceState) {  
- super.onCreate(savedInstanceState);  
-         setContentView(R.layout.main);  
-        apkWeb = (WebView)findViewById(R.id.apk_web);  
-        WebSettings webSettings = apkWeb.getSettings();  
-        webSettings.setJavaScriptEnabled(true);  
- 
-        String apkUrl = "http://frankiewei.net/apk/demos/main/index.html#home";  
-        apkWeb.loadUrl(apkUrl);  
-     }  
- }  
我相信大家 已经能看出来门道来了吧，hoho~
**3.用smali.jar编译classout成classes.dex:**
我们上一步已经将classes.dex反编译成了.smali文件，好了，我们看看smali文件看够了，在偿试把它编译成classes.dex吧，
输入如下命令:**java -jar smali.jar classout/ -o classes.dex.** 如下图所示:
![](http://hi.csdn.net/attachment/201007/9/0_1278691360fK0g.gif)
我们可以将新生成的classes.dex塞入ApkInstaller.apk里覆盖原来的classes.dex文件，这样我们的apk还是一样能用的哦~
Ok~今天就暂时写到这里，这片文章真是不容易呀，写了这么长时间，之间还出现过被来自武汉IP的人把这篇文章删除 过，我又重新写了一遍。希望大家多多进步！thx~
其余优秀参考文章：
1. [http://www.eoeandroid.com.sixxs.org/forum.php?mod=viewthread&tid=155840](http://www.eoeandroid.com.sixxs.org/forum.php?mod=viewthread&tid=155840)
先说一下反编译的流程和原理：
　　1.用apktool 把apk--> 资源包(java代码变成smali文件看不懂的)，可以修改资源包里面的文件。
　　2.apk后缀名改成zip或rar解压，获取 classes.dex 文件，用dex2jar转换成jar包(注：直接解压出来的资源文件是不能直接打开的，要用第一步的反编译工具)。
　　3.用jd-ui等java反编译工具直接查看java代码。
　　4.把java代码和第一版的资源包整到一起重新组成一个新的应用。
　　5.用apktool 重新编译。
　　6.用签名工具重新签名。
　　7.重新发布带新的签名的应用。
　　注：如果不用改java代码，只是换换风格和汉化2.3.4步则不用做。
　　google code上的开源项目
　　apktool [http://code.google.com.sixxs.org/p/android-apktool/](http://code.google.com.sixxs.org/p/android-apktool/)
　　dex2jar [http://code.google.com.sixxs.org/p/dex2jar/](http://code.google.com.sixxs.org/p/dex2jar/)
　　另外有人做了个工具套装，集成了apktool dex2jar jd-ui, win7下可以运行！！！
　　hackapk [http://code.google.com.sixxs.org/p/hackapk/](http://code.google.com.sixxs.org/p/hackapk/)
# HACKAPK V1.0
# 功能简介
> 
本软件套装是为了快速的对ANDROID软件进行逆向工程，汉化，以及分析软件的设计和实现而收集整理。
> 
目前本软件集成了 JD-GUI,DDX,DEX2JAR,APKTOOL，AXMLPRINTER2.
# 用法
> 
winhackapk.bat target.apk outputdir
2. [http://www.unpack.cn.sixxs.org/thread-68735-1-1.html](http://www.unpack.cn.sixxs.org/thread-68735-1-1.html)
