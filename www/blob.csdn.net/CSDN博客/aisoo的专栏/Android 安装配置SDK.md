# Android 安装配置SDK - aisoo的专栏 - CSDN博客
2010年08月22日 15:20:00[aisoo](https://me.csdn.net/aisoo)阅读数：1165
系统和软件需求
想要使用Android SDK中的代码和工具开发Android应用，你需要一个合适的开发机器和环境，如下所述。
支持的操作系统：
Windows XP or Vista
Mac OS X 10.4.8或者更高版本（仅支持X86平台）
Linux（在Ubuntu Dapper Drake上测试过）
支持的开发环境：
Eclipse：
       Eclipse 3.2，3.3（Europa）
       Android Development Tool plugin（可选）
其他开发环境或IDE：
       JDK5 or JDK6（只有JRE不行）
       不和GNU的Java/span>编译器（gcj）兼容
       Apache Ant 1.6.5 or later for Linux and Mac,1.7 or later for Windows
安装SDK
下载完SDK包之后，将压缩包解压到本机的一个合适的地方。在本文的后面部分，我们将用$SDK_ROOT作为你安装SDK的目录。
为方便以后的开发，你可以把目录$SDK_ROOT/tools加到系统PATH中去：
在Linux下，编辑文件~/.bash_profile或者~/.bashrc，找到设置PATH环境变量的行，把$SDK_ROOT/tools加入其中。如果没有设置PATH的语句，可以加入下面的内容：
export PATH=${PATH}:<$SDK_ROOT/tools的真实路径>
在Mac机器上，在你的home目录下找.bash_profile，操作类似Linux下。如果你机器上没有这个文件可以自己创建一个。
在Windows机器上，右键单击我的电脑，选择属性－>高级，点“环境变量”按钮，在出现的对话框中双击“系统变量”下面的Path变量进行修改，把$SDK_ROOT/tools的真实路径加入其中。
将$SDK_ROOT/tools目录加入系统路径中可以让你在使用adb（Android Debug Bridge）和其它命令行工具的时候不需要输入全路径名。如果你升级了你的SDK，如果和原来目录不一样的话，要记得更新你的PATH变量设置。
安装Eclipse的Plugin（ADT）
如果你希望使用Eclipse作为你开发Android应用程序的开发环境，你可以安装一个叫做ADT（Android Development Tools）的插件。ADT插件包含一系列功能强大的扩展功能，可以更快更容易的创建、运行并且调试Android应用程序。
如果你不打算使用Eclipse，那么就不需要下载和安装ADT插件。
为了下载和安装ADT插件，要先设置Eclipse远程更新站点，可以按照下面的步骤进行：
1. 启动Eclipse，选择菜单Help>Software Updates>Find and Install …
    2. 在显示出的对话框中选择Search for new features to install，按下Next继续。
      3. 按New Remote Site按钮。
      4. 在出现的对话框中，为远程更新站点输入一个名字（譬如Android Plugin），还要输入下面的URL：
[https://dl-ssl.google.com/android/eclipse/](https://dl-ssl.google.com/android/eclipse/)
按OK按钮。
      5. 你现在会看到新的站点被加入到搜索列表中，并且被勾选上，按下Finish按钮。
      6. 在消息更新结束后，你会看到Search Results对话框，在其中选中Android Plugin > Eclipse Integration > Android
Development Tools，然后按下Next按钮。
      7. 接下来是插件的许可协议，看完之后选择Accept terms of the license agreement，然后按Next继续。
      8. 在出现的Install对话框中按Finish，会开始下载ADT插件。
    9. ADT插件没有认证，你可以点击“Install All”按钮允许安装。
      10. 重新启动Eclipse。  
    11. 重启Eclipse之后，设置Eclipse的参数，使之指向开发包的根目录（$SDK_ROOT）：
   a) 选择Window>Preferences…打开参数设置面板（在Mac OS X上：Eclipse>Preferences）
   b) 在左侧面板中选择Android。
   c) 点击右侧面板的SDK Location后面的地址栏中输入SDK包的地址，或者点击Browse按钮在系统中寻找。
   d) 输入后点击确定。
更新ADT插件
可以通过下面的步骤更新ADT插件到最新版本：
1. 选择Help>Software Updates>Find and Install…
2. 选择Search for updates of the currently installed features，按Finish按钮。
3. 如果发现有ADT的更新的话，选中并且安装。
或者以下面的方式更形ADT插件：
1. 选择Help>Software Updates>Manage Configuration。
2. 从左侧树状导航条找到并选择Android Development Tools。
3. 在右侧的Available Task条目中点击Scan for Updates。
在Eclipse开发环境中开发Android应用程序
在Eclipse中开发Android应用程序之前，你需要先创建一个Android工程，然后设置启动配置。之后，你就可以写代码、运行和调试了。
本节之后的内容假定你已经在Eclipse中安装了ADT插件。如果你没有安装ADT插件的话，请先按照前面的步骤安装该插件。
创建Android工程
ADT插件提供了新工程向导，你可以快速创建一个Android工程。步骤如下：
1. 选择File>New>Project。
2. 选择Android>Android Project，按下Next按钮。
3. 选择两种不同的创建新项目方式：
     选择Create new project in workspace。创建新工程。
     选择Create project From existing source在已存在的代码上创建工程。你可以使用这个选项创建和运行SDK包中的例子代码，它们位于SDK包的samples目录下。
4. 点击Finish按钮，结束创建工程。
工程创建完后，ADT插件自动创建下面的文件和目录，放置特定的内容：
src/ :包含你的java源文件
res/ :包含资源文件
AndroidManifest.xml :工程的manifest文件
创建启动配置
在运行和调试Eclipse中的Android程序之前，必须先创建一个启动配置。启动配置定义了需要启动的项目，要开始的活动，仿真器选项等等。
通过下面的步骤创建启动配置：
1. 选择Run>Open Run Dialog…或者Run>Open Debug Dialog
2. 在左侧的树型工程类型列表中，右键单击Android Application，并选择New。
3. 在出现的对话框中输入你的配置的名字。 
    4. 在下面的Android表单中，点击Browse找到你的工程，选中要开始的活动。
5. 在Emulator表单中，设置想要的屏幕大小和网络状况。如果需要设置其它的仿真器选项，请参考
http://code.google.com/android/reference/emulator.html#startup-options
6. 可以在Common表单中设置额外的选项。
7. 点击Apply按钮，保存设置好的配置，或者点击Run或者Debug按钮。
运行和调试Android应用程序
现在你已经可以开始运行和调试Android程序了，Eclipse的使用就不多说了，：）下面这张图就是通过刚刚的步骤建立的helloandroid程序,虽然是空的什么功能都没有，不过看起来还是不错，有点苹果的风格。
