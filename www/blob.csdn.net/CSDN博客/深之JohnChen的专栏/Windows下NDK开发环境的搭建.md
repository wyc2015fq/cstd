# Windows下NDK开发环境的搭建 - 深之JohnChen的专栏 - CSDN博客

2013年08月01日 22:30:43[byxdaz](https://me.csdn.net/byxdaz)阅读数：2200


Windows下NDK开发环境的搭建

一：什么是NDK？

NDK 
提供了一系列的工具，帮助开发者快速开发C（或C++）的动态库，并能自动将so
和java 应用一起打包成apk。这些工具对开发者的帮助是巨大的。

NDK 
集成了交叉编译器，并提供了相应的mk 文件隔离CPU、平台、ABI
等差异，开发人员只需要简单修改mk 文件（指出“哪些文件需要编译”、“编译特性要求”等），就可以创建出so。NDK
可以自动地将so 和Java
应用一起打包，极大地减轻了开发人员的打包工作。比较简单的说，NDK是一套交叉编译工具，它可以帮你把你用C或C++书写的代码，编译为.so（类似与win下的.dll）格式的文件，使你可以在你的Android程序当中用Java语言（JNI）调用这些代码。

二：下载安装cygwin

由于NDK编译代码时必须要用到make和gcc，所以你必须先搭建一个linux环境，
 cygwin是一个在windows平台上运行的unix模拟环境,它对于学习unix/linux操作环境，或者从unix到windows的应用程序移植，非常有用。通过它，你就可以在不安装linux的情况下使用NDK来编译C、C++代码了。下面我们一步一步的安装cygwin吧。

cygwin的下载地址：[http://cygwin.com/setup.exe](http://cygwin.com/setup.exe)点击cygwin.exe文件

出现以下界面

![](https://img-blog.csdn.net/20130801222617734)

点击下一步

![](https://img-blog.csdn.net/20130801222715359)

如果您还没有下载就先选择第二个把安装文件下载下来，或者使用第一个在线安装，不过很慢。

因为所有的东西都是事先下载好的，所以选择的第三项。

点击下一步

![](https://img-blog.csdn.net/20130801222803625)

找到下载的安装文件目录，点击下一步

![](https://img-blog.csdn.net/20130801222837453)

把用红框标出的两个改成install状态，然后点击下一步下一步

最后直到安装成功。

在桌面上会有一个快捷方式

![](https://img-blog.csdn.net/20130801222911359)

测试一下cygwin是不是已经安装好?

运行cygwin，在弹出的命令行窗口输入：cygcheck -c cygwin命令，会打印出当前cygwin的版本和运行状态，如果status是ok的话，则cygwin运行正常。

然后依次输入gcc –v，g++ --version，make –version，gdb
 –version进行测试，(注意空格和-)如果都打印出版本信息和一些描述信息，非常高兴的告诉你，你的cygwin安装完成了！

三、下载安装AndroidNDK

Google 
出的最新NDK 下载地址为

[http://developer.android.com/sdk/ndk/index.html](http://developer.android.com/sdk/ndk/index.html)

四、配置NDK环境变量

1、首先找到cygwin的安装目录，找到一个home\<你的用户名>\.bash_profile文件，我的是：E:\cygwin\home\Administrator\.bash_profile，(注意：我安装的时候我的home文件夹下面神马都没有，解决的办法：首先打开环境变量，把里面的用户变量中的HOME变量删掉，在E:\cygwin\home文件夹下建立名为Administrator的文件夹（是用户名），然后把E:\cygwin\etc\skel\.bash_profile拷贝到该文件夹下)。

2、打开bash_profile文件，添加NDK=/cygdrive/<你的盘符>/<android
 ndk 目录> 例如：NDK=/cygdrive/e/android-ndk-r6b

export NDK

NDK这个名字是随便取的，为了方面以后使用方便，选个简短的名字，然后保存

3、打开cygwin，输入cd $NDK，如果输出上面配置的/cygdrive/e/android-ndk-r6b信息，则表明环境变量设置成功了。

五、编译演示工程

现在就可以编译相关NDK工程了（一些用c/c++写的程序），编译方法如下：

1、在cygwin命令行下，进入android项目工作目录：

cd $NDK

2、依次进入samples\hello-jni目录，（完整路径示例：E:\Andriod\develop\android-ndk-r4\samples\hello-jni）

3、运行

$NDK/ndk-build

注意：该命令$NDK和/ndk-build是连接到一起的，中间没有空格，实际使用时是打入完整的$NDK/ndk-build，不要只打ndk-build。

4、正常编译完成后会在你的项目libs/armeabi目录下生成对应的 .so文件。

六、导入静态目标库到Android项目

编译完成后会得到*.so静态目标库后，就可以导入工程环境Eclipse Android项目中使用，注意，NDK编程并非一定要把这个目标库导入Android项目使用，除了本步骤描述的使用方法之外，还有Android源码直接修改、编译，然后烧录到测试机的开发方式，那样可以实现应用程序默认安装、权限开机提升等等更“彻底”的功能。综合一下：

本步骤演示属于NDK目标库+AndriodAPK样式，此外还有Android源码直接开发、直接编译方式（确切一点已经淡化Android所谓的工程概念了，当然源码开发目前似乎还无法直接在Windows下进行，好像必须使用Linux家族系统进行，天缘稍后文章会有所涉及）

使用方法如下（示例）：

1、在Eclipse中新建一个工程HelloJni，HelloJni.java中的代码可参考NDK对应sample下的演示代码（android-ndk-r4\samples\hello-jni\tests\src\com\example\HelloJni）调用方法。

2、将NDK编译项目目录下的jni和libs文件夹拷贝到新建工程目录下

注意这两个文件夹要和工程中的src和res文件在同一目录下。然后进入Eclipse中刷新工程（会看到多出两个文件夹）。

3、运行Eclipse项目，就可以在虚拟机上看到hello-jni.c函数中输出字符串。

4、后续您可以试着修改库源程序或项目JAVA程序来熟悉NDK整个开发过程。                             


