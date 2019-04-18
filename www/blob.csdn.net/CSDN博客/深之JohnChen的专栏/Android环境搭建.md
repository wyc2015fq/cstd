# Android环境搭建 - 深之JohnChen的专栏 - CSDN博客

2013年07月25日 00:14:51[byxdaz](https://me.csdn.net/byxdaz)阅读数：4889
个人分类：[Android](https://blog.csdn.net/byxdaz/article/category/715665)



**Windows系统下搭建一个Android开发环境**

**一、安装JDK**

**1、JDK下载**

打开网站[http://java.sun.com/](http://java.sun.com/)，点击右侧的“Java
 SE”，出现图1-1，点击“JDK Download”即（https://cds.sun.com/is-bin/INTERSHOP.enfinity/WFS/CDS-CDS_Developer-Site/en_US/-/USD/ViewFilteredProducts-SingleVariationTypeFilter）下载jdk-6u23-windows-i586.exe。

**图1-1 Java SE下载页面**

**![](https://img-blog.csdn.net/20130724234846250?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYnl4ZGF6/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)**

**2、安装JDK**

双击jdk-6u21-windows-i586.exe，得到如图1-2界面。

![](https://img-blog.csdn.net/20130724234918171?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYnl4ZGF6/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

**图1-2 Java SDK设置界面**

点击“下一步”，更改并确认安装路径（D:\Java\jdk1.6.0_21\，请注意若改变路径后请加上Java\jdk1.6.0_21\，防止安装文件与D盘文件混合），点击“下一步”。

![](https://img-blog.csdn.net/20130724234922750?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYnl4ZGF6/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

**图1-3 Java目标文件夹安装界面**

更改路径后点击下一步，然后“完成”。

**3、环境变量设置**

XP：“我的电脑”->右键属性->“高级”->“环境变量”->系统变量（WIN7：“我的电脑”->右键属性->“高级系统设置”->“高级”->“环境变量”->系统变量）。

设置以下三个属性以及其值：

java_home：D:\ProgramFiles\Java\jdk1.6.0_21

指向JDK安装路径，在该路径下你应该能够找到bin、lib等目录。JDK的安装路径可以选择任意磁盘目录，但是建议目录层次浅一点。

path：%java_home%\bin;%java_home%\jre\bin

指向JDK的bin目录，该目录下存放的是各种编译执行命令，使系统可以在任意路径下识别java命令，并且在控制台编译运行时就无需键入大串的路径，否则以后每运行一次java程序就要先把它的class文件移动到%java_home%\bin目录下，然后打开DOS将路径改到该路径下面，执行class文件。

由于安装了JDK后，该目录下就有了两个虚拟机（JDK下的JRE和JRE），所以需要包括两个虚拟机下的bin文件夹。

倘若在path中没有添加“jdk1.6.0_21\bin”，将会出现图1-4所示情况：

![](https://img-blog.csdn.net/20130724234958281?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYnl4ZGF6/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

**图1-4 javac问题**

classpath：.;%java_home%\lib\dt.jar;

指向java程序编译运行时的类文件搜索路径，告诉JVM要使用或执行的class放在什么路径上，便于JVM加载class文件，目的达到用户可以import
 *，“.;”表示编译运行时先查找当前目录的class文件，dt.jar作用是运行环境类库，提供Java
 Swing组建显示的支持，可以用WinRAR打开看看，jar和zip格式一样，只是扩展名不同；tools.jar作用是提供工具类库和实用程序的非核心类；lib提供开发工具使用文件。

打开“开始”à“运行”，打入“cmd”，进入dos系统界面。然后输入“javac”，如果安装成功，系统会显示一系列关于javac命令的参数以及用法。

特别注意：java_home后面不能加“;”，并且注意路径名是JDK的路径，而不是JRE，并且对变量名是不区分大小写的。

**二、安装Eclipse**

**1、Eclipse下载**

打开官方网站[http://www.eclipse.org/](http://www.eclipse.org/)，点击Downloads，选择Windows
 32 Bit下的Eclipse Classic 3.6.1下载，即[http://www.eclipse.org/downloads/download.php?file=/eclipse/downloads/drops/R-3.6.1-201009090800/eclipse-SDK-3.6.1-win32.zip](http://www.eclipse.org/downloads/download.php?file=/eclipse/downloads/drops/R-3.6.1-201009090800/eclipse-SDK-3.6.1-win32.zip)。下载到D:\。

Windows64 Bitt下载http://www.eclipse.org/downloads/download.php?file=/technology/epp/downloads/release/kepler/R/eclipse-standard-kepler-R-win32-x86_64.zip

**2、Eclipse安装**

将下载到的zip压缩包解压，将eclipse.exe图创建桌面的快捷方式（右键图表->“发送到”->“桌面快捷方式”），以便于启动。

注意：SDK就像是java的JDK，ADT只是一个eclipse的插件，所以两者的安装次序没有严格的要求。

**三、安装ADT**

ADT想要在Eclipse上正常使用的关键是它们的版本要兼容。因此推荐在线安装ADT插件，它会根据Eclipse版本自动找到能够适应的版本的。
1.在线安装ADT插件

启动Eclipse，如果是Eclipse3.5或Eclipse3.6选择“Help”>“Install
 New Software...”，如果Eclipse3.4及以下版本菜单是“Help”>“Software Updates...”准备安装插件。打开的对话框Eclipse版本不同对话框也是不同的，Eclipse3.5或Eclipse3.6开的对话框如图3-1所示。

![](https://img-blog.csdn.net/20130724235258515?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYnl4ZGF6/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

图3-1 Eclipse在线安装插件

Eclipse3.4开的对话框如图3-2所示：

![](https://img-blog.csdn.net/20130724235325000?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYnl4ZGF6/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

图3-2Eclipse3.4在线安装插件

点击图3-1界面“Add”弹出图3-3所示对话框，点击图3-2界面中“Add
 Site”也弹出同样的对话框。

[](http://www.2cto.com/uploadfile/2012/0222/20120222100048476.jpg)

![](https://img-blog.csdn.net/20130724235426593?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYnl4ZGF6/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

图3-3 
在线安装Add Site

在“Location”中输入Android ADT插件在线下载地址：[https://dl-ssl.google.com/android/eclipse/](https://dl-ssl.google.com/android/eclipse/)，点击OK按钮就可以下载安装了，但是一定要保证电脑能够上网。

2.本地安装ADT插件

离线安装，从[http://developer.android.com/sdk/index.html网站中下ADT](http://developer.android.com/sdk/index.html%E7%BD%91%E7%AB%99%E4%B8%AD%E4%B8%8BADT),包括32bit和64bit版本。

**打开Eclipse,在菜单栏上选择**

help->InstallNew SoftWare出现如下界面:

![](https://img-blog.csdn.net/20130724235532312?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYnl4ZGF6/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)[](http://img1.51cto.com/attachment/201204/001246139.jpg)

点击Add按钮,出现如下界面

[](http://img1.51cto.com/attachment/201204/001450535.png)

在Name这而随意输入一个名字：ADT15；

点击打开Archives如图：

![](https://img-blog.csdn.net/20130724235642968?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYnl4ZGF6/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)[](http://img1.51cto.com/attachment/201204/001851310.png)[](http://img1.51cto.com/attachment/201204/001653593.png)

记得啦ADT一定要是ZIP格式的呢。

打开后如图：

![](https://img-blog.csdn.net/20130724235843171?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYnl4ZGF6/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)[](http://img1.51cto.com/attachment/201204/001851310.png)

点击“Next”：

![](https://img-blog.csdn.net/20130724235916625?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYnl4ZGF6/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)[](http://img1.51cto.com/attachment/201204/001932917.png)

再次点击“Next”：

![](https://img-blog.csdn.net/20130724235942562?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYnl4ZGF6/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)[](http://img1.51cto.com/attachment/201204/002026229.png)

选择“Iaccept the ......”，就可以点击“Finish”啦：

![](https://img-blog.csdn.net/20130725000020187?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYnl4ZGF6/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)[](http://img1.51cto.com/attachment/201204/002142708.png)

嘻嘻，Eclipse问你要安装吗。肯定是“OK”：

![](https://img-blog.csdn.net/20130725000118406?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYnl4ZGF6/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)[](http://img1.51cto.com/attachment/201204/002227745.png)

继续选择“OK”

![](https://img-blog.csdn.net/20130725000146015?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYnl4ZGF6/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)[](http://img1.51cto.com/attachment/201204/002257351.png)

Eclipse问你要重启Eclipse吗？当然要啦，不然怎么使用ADT。还是“OK”：

重启Eclipse后，**点击菜单window->preferences,进入如下界面**

![](https://img-blog.csdn.net/20130725000155875?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYnl4ZGF6/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)[](http://img1.51cto.com/attachment/201204/002322233.png)

选择你的androidSDK解压后的目录，选错了就会报错，这个是升级工具，目前还没有一个版本的SDK。

就可以安装好啦。

**四、安装SDK**

**1、下载SDK**

打开网站http://developer.android.com/sdk/index.html下载[android-sdk_r22.0.1-windows.zip](http://dl.google.com/android/android-sdk_r22.0.1-windows.zip)，并解压到D:\Android。

**2、SDK安装**

运行文件夹D:\Android\android-sdk_r22.0.1-windows\android-sdk-windows中的SDK
 Manager.exe，出现图4-1，选择需要安装的文件，然后“Install”，等待long long的时间。

![](https://img-blog.csdn.net/20130725000247859?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYnl4ZGF6/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

**图4-1 SDK安装界面**

如果遇到消息“Failed to fetch URL…”的错误提示，那么需要将HTTPS方式改为HTTP方式，在“Android
 SDK and AVD Manager”窗口的左侧选择“Settings”，选中“Force https://...”选项，点击“Save
 & Apply”，并重新运行SDK Setup.exe

在这里我花了很长时间，后来发现下载了一些过时的文件，所以需要注意。点击Available packages选择高版本的SDK。

![](https://img-blog.csdn.net/20130725000330187?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYnl4ZGF6/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

**图4-2 Available packages**

![](https://img-blog.csdn.net/20130725000500765?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYnl4ZGF6/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

**图4-3 Installed Packages**

注意：为了让使用者减少长久的等待时间，这里将所下载的目录打包成压缩文件，这样就可以直接解压使用而不需要执行步骤SDK安装。参见软件目录中的android-sdk-windows.rar压缩包。

安装结束之后文件列表如下：其中一部分可能是多余的，网上的解释：

add-ons:一些扩展库，例如 Google APIs Add-On

docs:API文档等等

platforms:各个版本的平台组件

samples:一些实例程序

tools:各种辅助工具

usb_driver:windows下的一些usb驱动

temp:存放下载平台组件过程中的临时文件

**3、SDK配置**

将tools所在文件夹路径（我的路径是D:\ProgramFiles\Android\android-sdk-windows\tools;）加入到path的环境变量中，方法与设置环境变量方法相同。

请注意：此时必须完善Android SDK的安装方可成功，具体操作如下：

1）SDK Location指向android目录

Windowàpreferences中的android选项，SDK
 Location所填的内容应该是Android SDK的安装路径（在这里是D:\Android\android-sdk-windows）即可。

补充：若在“SDK安装”过程中，是直接解压android-sdk-windows.rar压缩包，那么需要先进行SDK配置，才能完善SDK的安装。

![](https://img-blog.csdn.net/20130725000558843?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYnl4ZGF6/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

**图4-4 Android SDK路径配置**

如果不设置SDK Location的值，将出现图4-5错误。

![](https://img-blog.csdn.net/20130725000619796?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYnl4ZGF6/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

**图4-5未添加SDK路径错误**

**五、创建模拟器（AVD）**

WindowsàAndroid SDK and AVD ManageràVirtualdevicesàNew按照图5-1填写，最后点击CreateAVD即可。

注意：如果你点击左侧面板的VirtualDevices，再右侧点击New，而target下拉列表没有可选项时，这时候点击左侧面板的AvailablePackages，在右侧勾选https://dl-ssl.google.com/android/repository/repository.xml，然后点击Install
 Selected按钮，接下来就是按提示做。

![](https://img-blog.csdn.net/20130725000711656?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYnl4ZGF6/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

图5-1 
创建新的模拟器

解释：Target是模拟器的可用平台；SD Card是记忆卡，也就是手机的内存卡；Skin是模拟器显示的屏幕大小，具体参数为：

WVGA：800*480

QVGA：320*240

VGA：640*480

HVGA：480*320

WQVGA400：240*400

WQVGA432：240*432

WVGA800：800*480

WVGA854：854*480

或者也可以选择Resolution，自由设置模拟器屏幕大小。Hardware（AVD所需要的特殊设备）中的值：Abstracted
 LCD density（分辨率）其值是由选择的Skin自动设置的等等。

![](https://img-blog.csdn.net/20130725000711906?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYnl4ZGF6/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

图5-2 
运行模拟器

选中SDKAndroid2.2，点中StartàLaunch，将出现图5-3的模拟器。

![](https://img-blog.csdn.net/20130725000729484?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYnl4ZGF6/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

图5-3 
启动模拟器

![](https://img-blog.csdn.net/20130725000745500?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYnl4ZGF6/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

图5-4 
未知虚拟设备

android模拟器出错，emulator: ERROR: unknown virtual device name

原因：我的文档的默认位置改变，由于创建的文件路径引用错误造成的。

方法一：把“F:\Users\<username>\.android”下的文件复制到“C:\Users\<username>\.android“下面即可解决这个问题，不过这样的解决方案有一个明显的缺点，那就是如果又新建立AVD，又得重新复制。

方法二：“我的电脑”à右击属性à“高级”à“环境变量”à“系统变量”à“新建”变量名为“ANDROID_SDK_HOME”(注意，这个变量名不能改变，只能是这个名字！)，然后把变量值改为你想把AVD所在的“.android”文件夹放置的位置，比如：”F:\AndroidEmulator”，在这里我把它放在Android
 SDK包中，值为：“D:\ProgramFiles\Android\android-sdk-windows”

**六、HelloWorld程序**

**1、创建Android Project**

“File”à“New”à“Other”à“Android”à“Android
 Project”，出现图6-1界面，然后根据需要填写Project name、Application name、Package
 name、Create Activity内容，并选择Build Target即可。

其中：

Project name：一个项目的名称，实际对应一个文件夹。

Build Target：选择该应用程序所使用的SDK版本。

Application name：程序的名称，一般会出现在应用程序的标题栏。

Pakcage name：此名理论上可以随意，但Pakcage有一定的命名规范，即：第一目指明组织类型，比如com一般指公司，org指组织，edu指教育部分；第二目指该组织的名称，比如sun等，我是自己用了自己的名字；第三目及以后则可根据自己的分类进行定义。

Creak Activity：Activity是一个Andriod程序的一个运行实体，有点类似于C语言的main函数，所不同的是Android程序可以有多个类似与main函数的实体。

Min SDK Version：该项一般与Build Target一一对应，不需要特别指出。

![](https://img-blog.csdn.net/20130725000950250?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYnl4ZGF6/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

**图6-1 New Android Project**

**2、创建AVD设备**

见第五节。如果没有创建AVD设备，也没有关系，可以右击工程à“Run
 As”à“Run Configurations”à“Android
 Application”à“Android”（填上需要运行的工程名）à“Target”（将部署设备的选择设为Mannul，防止有多个部署设备时的混乱）à“Run”。在跳出的对话框中选择“Launch
 a new Android Virtual Device”，选中SDK，点击OK即可。

**3、编译运行程序**

打开hello.java文件，其内容如图6-2所示：

![](https://img-blog.csdn.net/20130725000913640?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYnl4ZGF6/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

**图6-2 hello.java文件内容**

![](https://img-blog.csdn.net/20130725001022468?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYnl4ZGF6/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

**图6-3 Java Build Path设置**

ProjectàProperties，出现图6-3对话框，将Android
 2.2勾上，点击OK；RunàRun（Ctrl+F11）->Yes->选中SDK，点击StartàLaunchàOK，等待2分钟将出现图6-4结果界面，表明成功。

注意：右击所要运行的java文件->“Run As”->“Run
 Configurations”à“Android Application”->“Android”（填上需要运行的工程名）->“Target”（将部署设备的选择设为Mannul，防止有多个部署设备时的混乱）->“Run”即可。

![](https://img-blog.csdn.net/20130725001313953?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYnl4ZGF6/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

**图6-4运行结果界面**

参考来源：[http://blog.csdn.net/zhq56030207/archive/2010/08/31/5852130.aspx](http://blog.csdn.net/zhq56030207/archive/2010/08/31/5852130.aspx)

**4、调试程序**

**通过DDMS来调试程序：**

（1）将Eclipse开发工具的工作界面切换到DDMS标签。如图6-5和图6-6所示。

![](https://img-blog.csdn.net/20130725001333718?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYnl4ZGF6/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

**图6-5打开视图布局显示操作**

![](https://img-blog.csdn.net/20130725001352046?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYnl4ZGF6/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

**图6-6视图布局选择框**

（2）在“DDMS”界面中选择“Devices”标签，查看其菜单功能：Debug
 Process（调试进程）、Update Threads（更新线程）、Update Heap（更新堆）、Cause
 GC（引起垃圾回收）、Stop Process（停止线程）、Screen Capture（屏幕截图）、Reset adb（重启Android
 Debug Bridge）菜单选项。

**通过DDMS的“Logcat”来调试程序**

（1）“Logcat”通过“andrdoi.util.Log”类的静态方法来查找错误和打印系统日志消息。

andrdoi.util.Log常用的方法有以下5个：

Log.v(String tag, String msg); VERBOSE

Log.d(String tag, String msg); DEBUG

Log.i(String tag, String msg); INFO

Log.w(String tag, String msg); WARN

Log.e(String tag, String msg); ERROR，适用于“断点”方式调试程序

Log.v、Log.d信息应当只存在于开发过程中，最终版本只可以包含Log.i、Log.w、Log.e三种日志信息。

（2）点击“Run”-->“Debug”菜单命令，进入调试模式。

（3）切换到“DDMS”界面，点击“Logcat”标签，查看我们刚刚在程序中打印的Log信息。用不同颜色表示不同等级的信息，方便跟踪程序。

（4）调试方法

1）设置断点，单步执行：双击某行的标记栏

2）声明条件断点，触发完成：断点在表达式值发生变化时触发。双击设置完断点后右击断点，选择“Breakpoint Properties”在勾上“Enable”、“Conditional”，在框中写上表达式变量，当满足时，程序就会挂起；同时，在输入条件表达式时，也可以使用代码帮助。在上下文菜单中，通过Ctrl+Shift+I或右击变量并选择Inspect选择，会显示结果。

![](https://img-blog.csdn.net/20130725001420484?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYnl4ZGF6/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

**图6-7设置调试界面**

Linux系统下搭建一个Android开发环境，参考：http://huangz.iteye.com/blog/1103590

