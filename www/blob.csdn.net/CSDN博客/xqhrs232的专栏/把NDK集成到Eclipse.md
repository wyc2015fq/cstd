# 把NDK集成到Eclipse - xqhrs232的专栏 - CSDN博客
2011年06月20日 22:35:00[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：2145
原文地址::[http://dev.10086.cn/cmdn/wiki/index.php?doc-view-5298.html](http://dev.10086.cn/cmdn/wiki/index.php?doc-view-5298.html)
在[Android](index.php?doc-view-1789)的游戏开发中，你迟早会发现有必要把部分代码跑得更快。用C写的Android代码比同样的[Java](index.php?doc-view-1306)版本，快上10~100倍。我可以证实这个事实，在我最新的3D游戏引擎中，我把部分组件用原生程序（即用C/C++写代码，编译成二进制的动态链接库文件）重写。
这样带来了极大的性能提升，但我们也不得不面对这么一个事实：Eclipse对Java的开发提供了极佳的支持的同时，对C的开发却极其糟糕。Eclipse不适合C的开发，对吗？错，现在向你展示如何在Eclipse中，建立一个快速的NDK开发环境。
       首先声明一点，Eclipse不仅仅适合于开发Java[应用程序](index.php?doc-view-1781)。Eclilpse为Java开发提供了极好的支持，它的设计初衷也是为了Java。但得益于它的设计架构，Eclipse能够为包括C语言在内的任何语言的开发提供支持。一个名为CDT的插件，使得在Eclipse里能对C/C++进行开发。尽管我说得有些过了头。接下来这些，是你所需要的：
Android NDK（Native Development Kit）[http://developer.android.com/sdk/ndk/1.6_r1/index.html](http://developer.android.com/sdk/ndk/1.6_r1/index.html)
Eclipse CDT(C Development Tooling) [http://www.eclipse.org/cdt/](http://www.eclipse.org/cdt/)
如果用的是windows操作系统，你需要安装Cygwin，并安装上重要的包（特别是GCC和Make）。
       接下来的步骤是：
       安装上述三个包。我把NDK安装到C:/Android_NDK。在后面的文章中，我都默认为该目录是NDK的安装目录。
       为了熟悉NDK，在C:/Android_NDK/apps目录中，你需要配置每个项目的“app”。就像NDK中的例子一样，它们能够正确编译运行，并要求严格的配置。
       测试NDK的办法：
       启动cygwin
       cd /cygdrive/c/Android_NDK
       make APP=hello-jni
       根据上述的步骤，你将能够正确编译。如果cygwin漏装了GCC、Make或者其它开发包，你有必要重新安装Cygwin，并且确保全部必要的开发包已经安装。如果你遇到什么特殊的错误，我建议把这些错误报告给NDK用户组。
       一旦NDK能够正常工作，你就能够添加和配置好你的NDK开发项目。参考NDK的example一样，你需要一个详细的build文件（Android.mk），该文件告诉NDK编译器编译什么文件。JNI代码文件通常放在应用程序项目的jni文件夹下。该目录下的Android.mk文件指引NDK编译器如何编译程序。
       在完成这些基本配置后，你会想去写C代码。NDK使用Java标准的JNI技术实现。大量的关于JNI的文档在这里都适用。怎样去编写JNI代码，已经超出了本文的范畴。
       接下来是有用的部分：
       如果开发过NDK程序，你很有可能习惯使用文本编辑器或者vim去编辑的你C/C++代码，然后使用make APP=myapp的命令来编译，接着点击更新在Eclipse中的项目，最后盼望着那已部署上的共享库是最新的。多么痛苦的一件事啊！其实，有一个更好的方法。
       目前，已经安装上了CDT，你就可以在Eclipse中编译你的C/C++文件。右击C/C++源文件，选择使用C/C++编辑器打开，就能够使用CDT编译器了。多友好！因为尚未编译C/C++源文件，你并不清楚代码正确与否。但这也使得你更友好地在一个界面下编辑代码。
       生成项目也是快速而灵活的。你曾经使用过Eclipse的生成器吗？它们是一些可配置的开关，用于执行你所配置的命令和刷新工程资源。以下是我的配置：
       右击你的项目，选择“properties”
       在左边的列表中选择“Builders”
       在右侧，点击“New…”按钮
       在列表框，选择“Program”作为配置类型；点击“OK”按钮，进入下一个配置对话框。
       命名name为“Native Builder”
       选择Location为“c:/cygwin/bin/bash.exe”
       “Working Directory”为“c:/cygwin/bin”
       “Arguments”为：
--login –c “cd /cygdrive/c/Android_NDK && make APP=myapp”
确保在login前有两个连接号，-c后面的引号。
       接着打开“refresh”的tab面板：
       选中“Refresh resources upon completion”
       选择“Specific resources”
       点击“Specify resources”按钮，选择你项目的lib目录
       选中“Recursively include sub-folders”
       接着打开“Build Options”的tab面板：
       选中“Allocate Console”
       选中“Launch in background”
       选中“Run the builder After a Clean”
       选中“Run the builder During manual builds”
       选中“Run the builder During auto builds”
       选中“Specify working set of relevant resources”
       点击“Specify Resources”按钮，选择你项目的JNI目录，并包含所有的文件。
       点击“OK”按钮。
       这里假设：cygwin安装在c:/cygwin；NDK安装在c:/Android_NDK；项目工程名称是“myapp”。根据你的安装配置，相应地替换这些参数。
       你刚刚做了什么呢？你使得，在JNI的目录下，每次修改任何文件，Eclipse都通过cygwin自动启动NDK编译你的工程，并在控制台输出编译信息。当它编译完成，将自动刷新工程输出目录（lib），并触发ADT生成一个新的APK安装包。这一切都极其简便。
       把NDK集成到Eclipse，实实在在地加速了我当前项目的开发进度。希望你能够在文本中受益。
//==============================================================
备注几点----2011--6--21 xqh
    1》建立Builders时如果要使用下面的参数格式
--login -c "cd /cygdrive/e/android-ndk-r5/samples/hello-jni && $NDK/ndk-build"
    那么NDK应该在Builders的Environment栏进行定义----也就是加NDK这个Variable其值为你的路径，比如：：E:/DirectX/Android_NDK_JNI/android-ndk-r5-windows/android-ndk-r5
   要不然会报下面的错误：
/usr/bin/bash: /ndk-build:no such file or directory.
    2》自己建立的针对C/C++的Builders要在JAVA的Builders之前，确保C/C++能够在编译JAVA之前编译
    3》要方便的使用NDK集成到ECLIPSE的好处，你的带NDK的JAVA工程应该放到你的NDK目录下面去，要不然自己建立的NDK Builders定位不到你的工程文件的。IDE/程序只能做你安排它做的事，它没有思维能力，所以一切路径什么的/编译器全部都应该我们事先指定！！！
    4》编译的情况会在ECLIPSE的CONSOLE窗口里面显示出来！
    5》对一个工程增加了一个Builders会在工程目录下的.externalToolBuilders文件夹建立一个XXX.launch文件，其中XXX文件为你建立的Builders名称！

