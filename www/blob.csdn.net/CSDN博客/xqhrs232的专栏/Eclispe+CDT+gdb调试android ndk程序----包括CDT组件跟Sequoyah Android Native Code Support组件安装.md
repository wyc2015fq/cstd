# Eclispe+CDT+gdb调试android ndk程序----包括CDT组件跟Sequoyah Android Native Code Support组件安装 - xqhrs232的专栏 - CSDN博客
2012年05月31日 17:23:32[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：987标签：[android																[eclipse																[application																[java																[include																[command](https://so.csdn.net/so/search/s.do?q=command&t=blog)](https://so.csdn.net/so/search/s.do?q=include&t=blog)](https://so.csdn.net/so/search/s.do?q=java&t=blog)](https://so.csdn.net/so/search/s.do?q=application&t=blog)](https://so.csdn.net/so/search/s.do?q=eclipse&t=blog)](https://so.csdn.net/so/search/s.do?q=android&t=blog)
个人分类：[Android/Java](https://blog.csdn.net/xqhrs232/article/category/906925)
原文地址::[http://www.cnblogs.com/shadox/archive/2011/12/02/2272564.html](http://www.cnblogs.com/shadox/archive/2011/12/02/2272564.html)
Eclipse+CDT+gdb调试android ndk程序
先介绍一下开发环境，在这个环境下，up主保证是没有问题的。
- 
ubuntu 11.10
- 
eclipse 3.7(indego) for java
- 
jdk 6
- 
android sdk 2.2
- 
andrid ndk r7
当然，在windows环境下通过cygwin等工具也是可以实现gdb调试的，我也确实实现过。但是性能实在太低，卡的根本没法用。Linux下直接用gdb调试本地方法是很流畅的。
再确定安装并配置好开发环境之后，就可以开始了。
首先得确定自己能够正常的运行一个ndk工程，连运行都成问题的话，也就谈不上什么调试了。
新建一个android项目，选择crete project form existing source，源代码位于
/home/shaodx/android/android-ndk-r7/samples/hello-jni，即ndk目录的samples/hello-jni。
![](http://pic002.cnblogs.com/images/2011/96142/2011120218065233.jpg)
Next>
Sdk target 就选2.2吧 ，其他的虽然没测试，应该也没什么问题。
![](http://pic002.cnblogs.com/images/2011/96142/2011120218070948.jpg)
Finsh
Hellojni项目极其简单，看代码就懂了。在activitie下建立一个textview，然后调用一个natvie方法来返回一个字符串，然后把textview的text设置为这个字符串。
为了方便看到调试后的效果和局部变量的变化情况，我加了几行对程序结果没影响的代码。
Java代码：
![](http://pic002.cnblogs.com/images/2011/96142/2011120218080160.jpg)
C++：
![](http://pic002.cnblogs.com/images/2011/96142/2011120218090065.jpg)
当然，这个时候直接运行的话，程序肯定会崩溃的。因为动态链接库还没编译好。Ndk根目录下的ndk-build负责编译so文件。首先进入项目所在目录，然后运行ndk-build即可。
效果如下：
![](http://pic002.cnblogs.com/images/2011/96142/2011120218093724.jpg)
出错了…..
Host 'awk' tool is outdated.
上网搜索之后的解决方案如下：
到/home/shaodx/android/android-ndk-r7/prebuilt/linux-x86/bin/目录下找到这个awk，file一下：
![](http://pic002.cnblogs.com/images/2011/96142/2011120218102725.jpg)
这个awk文件居然是64位版的，难怪之前说过期了。（google程序员粗心了吧）
解决方案居然是把这个awk删了就行了……………..
解决问题之后，换个姿势，再来一次
![](http://pic002.cnblogs.com/images/2011/96142/2011120218110753.jpg)
So文件存在于libs/armeabi/libhello-jni.so。
现在就可以运行程序了。
![](http://pic002.cnblogs.com/images/2011/96142/2011120218114447.jpg)
不过要是每一次修改c++代码都还要调用ndk-build的话，那也太麻烦了。介绍一个一劳永逸的方法。
首先，要给eclipse安装一个CDT，eclipse>help>install new software
下载地址为   [http://download.eclipse.org/tools/cdt/releases/indigo/](http://download.eclipse.org/tools/cdt/releases/indigo/)
![](http://pic002.cnblogs.com/images/2011/96142/2011120218122493.jpg)
反正我是把所有的选项都给安装了，也花不了太多的时间。
然后是安装sequoyah  地址为 [http://download.eclipse.org/sequoyah/updates/2.0/](http://download.eclipse.org/sequoyah/updates/2.0/)
这个就只需要安装Sequoyah Android Native Code Support 一项就可以了。
![](http://pic002.cnblogs.com/images/2011/96142/2011120218140252.jpg)
然后把当前项目转换为C++项目。
File>new>other:
![](http://pic002.cnblogs.com/images/2011/96142/2011120218142621.jpg)
Convert to a c/c++ Project
依照图中所示设置：
![](http://pic002.cnblogs.com/images/2011/96142/2011120218150051.jpg)
Finish。
这个时候，项目中的c++代码也会被识别了。一般这个时候就会爆出一堆错误，主要是gcc找不到jni.h头文件，同样也识别不了来自jni的一些函数和数据类型：
![](http://pic002.cnblogs.com/images/2011/96142/2011120218152868.jpg)
于是就要给gcc添加一个环境变量让它来找到jni.h了，环境变量名称为C_INCLUDE_PATH。这个名字不能乱写，windows下类似的环境变量叫做INCLUDE。
然后多刷新几次工程，这些错误就消失了。
接下来，进行C++代码的编译配置。
进入工程属性页，build command设置为ndk下ndk-build的完整路径。
![](http://pic002.cnblogs.com/images/2011/96142/2011120218160458.jpg)
将Behavior选项页下的build处的all替换为空格
![](http://pic002.cnblogs.com/images/2011/96142/2011120218163540.jpg)
接下来就可以直接运行了，每次运行的时候，就会自动把C++代码编译成so文件。同时，可以在eclipse中编写java代码和C++代码，很是方便。
![](http://pic002.cnblogs.com/images/2011/96142/2011120218355282.jpg)
往下的工作就真的跟调试相关了。
首先进入工程目录，然后运行ndk目录下的ndk-gdb。执行这一步骤时，应当先确保有模拟器在运行。如果有人更习惯于命令式的方法来调试程序的话，就可以直接用这个ndk-gdb来调试程序了。接下来的步骤就是把这个gdb调试图形化。
![](http://pic002.cnblogs.com/images/2011/96142/2011120218175889.jpg)
在eclipse下进入debug>debug configuration，选择C/C++ Application,main选项卡下的C/C++ Application处填写为/home/shaodx/android/android-ndk-r7/samples/hello-jni/obj/local/armeabi/app_process，这个文件是专为调试而存在的，假如发现找不到这个文件的话，就应当先运行一次ndk-gdb。只运行ndk-build是不会产生这个文件的。Project选择当前的HelloJni。
![](http://pic002.cnblogs.com/images/2011/96142/2011120218233779.jpg)
最下面的 process launcher  select other 选择stuanard
![](http://pic002.cnblogs.com/images/2011/96142/2011120218244637.jpg)
然后进入Debugger选项卡，Stop on startup at 填写的是C++程序的入口函数，这个项目中就是Java_com_example_hellojni_HelloJni_stringFromJNI了，我觉得这个参数应该没什么用的，记得设置断点就行了。Gdb debugger 为：home/shaodx/android/android-ndk-r7/toolchains/arm-linux-androideabi-4.4.3/prebuilt/linux-x86/bin/arm-linux-androideabi-gdb，这个路径下有很多文件针对其他平台的，别选错了。Gdb
 command file为/home/shaodx/android/android-ndk-r7/samples/hello-jni/obj/local/armeabi/gdb2.setup，目前这个gdb2.setup文件还不存在，待会在创建。底下的Verbose console mode 一定要记得勾上，这样才能在eclipse控制台中用指令来与gdb交互。
![](http://pic002.cnblogs.com/images/2011/96142/2011120218251728.jpg)
进入debugger options的 connection子项，type为tcp，端口为5039：
![](http://pic002.cnblogs.com/images/2011/96142/2011120218255434.jpg)
设置完毕之后，apply。
接下来只需要修改两个文件即可。修改之前记得备份
首先是ndk-gdb，把最底下的一行 “$GDBCLIENT -x `native_path $GDBSETUP`”直接去掉，保存。
![](http://pic002.cnblogs.com/images/2011/96142/2011120218262836.jpg)
然后把'/home/shaodx/android/android-ndk-r7/samples/hello-jni/obj/local/armeabi'目录下的gdb.setup复制一份，命名为gdb2.setup。把gdb2.setup打开，去掉最后一行的“target remote :5039”，千万不要在以为把gdb.setup修改好，然后把之前的设置指向gdb.setup会起作用，因为每次调用ndk-gdb的时候，都会产生一个新的gdb.setup 来覆盖掉修改。
![](http://pic002.cnblogs.com/images/2011/96142/2011120218265984.jpg)
然后就可以给代码设置断点了，首先在调用本地方法之前记得有一个断点，之后也设置一个。
![](http://pic002.cnblogs.com/images/2011/96142/2011120218273011.jpg)
C++的代码在函数入口处设置一个断点即可。
下面开始正式的调试了，先运行项目的java调试。程序会再运行到第一个断点处停下来。
这个时候赶紧运行在命令行下进入工程目录，然后运行ndk根目录下的ndk-gdb。
运行之后是没有任何输出的。
![](http://pic002.cnblogs.com/images/2011/96142/2011120218275438.jpg)
然后启动C++的debug，即之前配置好的那个jni debug。
如图：
![](http://pic002.cnblogs.com/images/2011/96142/2011120218281954.jpg)
由于在C++程序之也设置了断点，继续摁F6就可以直接执行到C++程序中。
来个大图，熟悉eclipse的调试就没什么压力了。右上角可以直接看到局部变量的值，包括传进来的参数。
![](http://pic002.cnblogs.com/images/2011/96142/2011120218285790.jpg)
同时，可以在控制台直接与gdb通讯，要退出C++程序的调试的话，continue即可，程序又回到java代码：
![](http://pic002.cnblogs.com/images/2011/96142/2011120218292518.jpg)
完毕。
