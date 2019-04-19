# Android平台从Froyo 2.2开始支持jni单步调试了！ - xqhrs232的专栏 - CSDN博客
2011年06月21日 11:42:00[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：828标签：[jni																[android																[平台																[path																[google																[脚本](https://so.csdn.net/so/search/s.do?q=脚本&t=blog)](https://so.csdn.net/so/search/s.do?q=google&t=blog)](https://so.csdn.net/so/search/s.do?q=path&t=blog)](https://so.csdn.net/so/search/s.do?q=平台&t=blog)](https://so.csdn.net/so/search/s.do?q=android&t=blog)](https://so.csdn.net/so/search/s.do?q=jni&t=blog)
个人分类：[Android/Java](https://blog.csdn.net/xqhrs232/article/category/906925)
原文地址::[http://blog.csdn.net/jinhill/archive/2011/01/09/6124996.aspx](http://blog.csdn.net/jinhill/archive/2011/01/09/6124996.aspx)
一、写在前面的话
前段时间从网上看了google的io大会，亲眼看到了google在it技术发展和创新领域所做的工作（在此也问候一下apple 公司的美术家门，你们辛苦了）。虽然google认为未来pc上只可能存在两种程序——browser和game，但是就目前来看，pc平台上的 application还是有一定的市场的，html5的推广和应用毕竟是需要一定时间的（第一个吃螃蟹的人，可能会吃的很好，但是如果第一个吃蘑菇的人呢？如果吃到的是毒蘑菇呢？业内都在等，在看，一旦别人吃的还行，那个时候才会蜂拥而上）。这就好像虽然google在大力鼓吹html5的各种特性，可是自己仍然在android平台上添加对flash
 10.0的支持一样（用原话说：“hey,man,people use flash!”），从这一点来看google相对更加亲民一些，而apple则更加激进一些（这并不是说谁好，谁不好，偶也喜欢激进一点的风格拉，毕竟 iphone颠覆了整个手机行业，一下子把ui设计的准绳拉得很高，这让一些诸如曾经卖木头的公司很郁闷）。
言归正传，froyo新增了 许多功能，也改进了很多地方，但是偶认为对开发者来说最值得高兴的事情就是android 2.2修改了dalvik的运行机制（应该说是加入了类似jit引擎的东西吧），可以让程序运行得更快，另外就是支持jni的单步调试（看到这个特性偶的眼泪是哗哗的，想当年面对着jni模块输出的密密麻麻的log偶痛苦地闭上了眼睛。。。）。其他的诸如app to sd，无线ap，exchange，push api，语音翻译，搜索，等等这些都是锦上添花的事情了。
从偶的观点来看——速度才是硬道 理！
要不然真是可惜了nexus one那1ghz的处理器了。
二、ndk-r4带给我们的惊喜
别的不说啥了，把changes.txt的重要的部分给翻译一下发出来就好了（英语 好的朋友可以直接忽略）。
changes.txt的原文在android-ndk-r4/docs目录下：
重用要的BUG修正：
（1）<fenv.h> 这个头文件在以前版本的ndk中放置的位置不对，这将导致在一些常规的编译过程中无法找到该文件进而引起编译失败。（这个问题在ndk-r4已经修正了）
重 要的变化：
（1）在win32环境下，ndk需要配合1.7或者更高版本的cygwin使用。目前，已经官方认定在cygwin 1.5的环境下，ndk有可能不会正常工作。
（2）进一步简化编译过程：
开发者不再需要进入ndk的根目录，然后运行 build/host-setup.sh脚本来初始化环境变量或者修改$NDK_ROOT/apps目录下面的任何文件了，取而代之的是在编译jni模块的时候，只需要在jni目录下面运行一个命令“ndk-build”即可自动完成编译过程（这个脚本甚至连子目录下面的模块都可以自动编译）。
ndk- build脚本的具体使用方法，可以参见overview.txt和ndk-build.txt这两个文档。
注意：
出于兼容性考虑，开发者仍然可以在"$NDK_ROOT/apps/<name>"中创建工程。但是ndk的示例工程已经从apps目录转移到了"samples /<name>"目录下，而且在编译jni模块的时候，一定要用"ndk-build"脚本进行编译。
（3）引入更加方便的调试手 段：
从android 2.2开始（同样适用于未来更高版本的android系统），开发者可以使用“ndk-gdb”工具很方便地对native的模块进行调试。具体的调试方法参见“ndk-gdb.txt”文档。（这也是偶本文要讲的内容了）
（4）支持硬件的FPU：
根据ARMv7-a级别的设备，可以指定 ndk生成新的“armeabi-v7a” ABI的机器码。
注意：
ndk在默认的配置下，仍然会生成旧的“armeabi” ABI（基于ARMv5TE指令集）的机器码，此配置可以在目前所知的所有android的系统image下工作。（费了半天劲，其实就是说ndk默认生成的模块兼容性比较好，而新引入的功能则可以经过优化以后性能更高，可以使用更先进的arm指令集）。
修改ndk生成机器码的配置主要通过修改mk文件的APP_ABI变量完成，具体的修改方法请参考application-mk.txt文档。
（5）提供了一个小型的 “cpufeatures”静态库，可以通过该库在jni运行时得到当前cpu支持的特性。该库支持从android 1.5开始所有的平台。关于该库的更详细的信息，请参考cpu-features.txt文档。
（6）支持可选的ARM Advanced SIMD (a.k.a. NEON)特性，吧啦吧啦吧啦。下面几个cpu相关的特性，还是给对硬件优化感兴趣的朋友们研究好了。需要提一嘴的是，samples目录下面有一个叫做 hello-neon的范例，就是演示cpu features的，感兴趣的朋友们不要错过。
（7）添加了<android/bitmap.h>头文件，可以提供jni模块对传入的android.graphics.Bitmap对象的直接操作（这个功能太有用了，否则，在jni里面操作Bitmap对象那叫一个麻烦。。。），使用方法可以参考samples目录下的bitmap-plasma 示例程序。
另外，就是支持一些确保不会让模块被内核执行的一些安全功能以及一些bug修正，最后再提一句就是ndk-r4已经开始支持.s结尾的纯汇编代码编译了（之前的之能够支持嵌入式的汇编支持），骨灰级别的玩家注意了！
最后就是一些c和c++库的头文件的修改和 微调，在此不再聒噪。
三、开始体验ndk-r4
看 了上面的修改变化，想必大家都开始手痒了吧？！好的，马上就开始安装这个ndk-r4，当然，前提是sdk一定要更新到android 2.2这个平台对应的最新版本，当然相应的adt也要升级喔！
老规矩，偶的所有介绍都是基于linux平台的。
1、下载ndk-r4
官方的下载地址当然是：developer.android.com/sdk/ndk/index.html啦，不过由于和谐的原因我们下载不了。可以通过代理的方式去下载，相信大家总会有办法下载下来的。
2、安装ndk-r4
其 实很简单了，只用一个命令就行：
$unzip android-ndk-r4-linux-x86.zip
解压完毕后会看到一个叫做 android-ndk-r4的目录，这个目录需要export到当前的PATH环境变量里面去：
$export PATH=~/android-ndk-r4:$PATH
然后这个ndk就安装好了。（不要问我怎么不运行build/host- setup.sh脚本，那东西已经在r4被淘汰掉了）
3、编译个 hello world看看
$cd ~/android-ndk-r4/samples/hello-jni/
$ndk-build
Gdbserver      : [arm-eabi-4.4.0] /home/wayne/android-ndk-r4/samples/hello-jni/libs/armeabi/gdbserver
Gdbsetup       : /home/wayne/android-ndk-r4/samples/hello-jni/libs/armeabi/gdb.setup
Gdbsetup       : + source directory /home/wayne/android-ndk-r4/samples/hello-jni/jni
Install        : libhello-jni.so => /home/wayne/android-ndk-r4/samples/hello-jni/libs/armeabi
如果看到上面的输出，呵 呵，恭喜恭喜，jni的so库已经编译成功了。
4、生成一个apk 看看吧
这一步比较烦，要求必须安装了ant以及最新的android sdk，并且把$ANDROID_SDK/tools和ant的bin目录也export到PATH环境变量里面才行。
在此偶只贴出来偶的配置：
$export PATH=/usr/local/apache-ant-1.8.1/bin:$PATH
$export PATH=/home/wayne/android-sdk-linux_86/tools:$PATH
当然了，这些对PATH的修改，可以直接写到/etc/profile文件中，下次一进入shell就会自动准备好，在此按下不表。
好了，开始生成apk
$cd ../
$android update project --path hello-jni
$cd hello-jni
$ant debug
Buildfile: /home/wayne/android-ndk-r4/samples/hello-jni/build.xml
    [setup] Android SDK Tools Revision 6
    [setup] Project Target: Android 2.2
    [setup] API level: 8
    [setup] WARNING: Attribute minSdkVersion in AndroidManifest.xml (3) is lower than the project target API level (8)
    [setup] Importing rules file: platforms/android-8/ant/ant_rules_r2.xml
-compile-tested-if-test:
-dirs:
     [echo] Creating output directories if needed...
-resource-src:
     [echo] Generating R.java / Manifest.java from the resources...
-aidl:
     [echo] Compiling aidl files into Java classes...
compile:
    [javac] /home/wayne/android-sdk-linux_86/platforms/android-8/ant/ant_rules_r2.xml:255: warning: 'includeantruntime' was not set, defaulting to build.sysclasspath=last; set to false for repeatable builds
    [javac] Compiling 1 source file to /home/wayne/android-ndk-r4/samples/hello-jni/bin/classes
-dex:
     [echo] Converting compiled files and external libraries into /home/wayne/android-ndk-r4/samples/hello-jni/bin/classes.dex...
-package-resources:
     [echo] Packaging resources
 [aaptexec] Creating full resource package...
-package-debug-sign:
[apkbuilder] Creating HelloJni-debug-unaligned.apk and signing it with a debug key...
[apkbuilder] Using keystore: /home/wayne/.android/debug.keystore
debug:
     [echo] Running zip align on final apk...
     [echo] Debug Package: /home/wayne/android-ndk-r4/samples/hello-jni/bin/HelloJni-debug.apk
BUILD SUCCESSFUL
Total time: 7 seconds
这些命令运行完毕后，如果看到上面的输出，代表生成的apk是正常 的，然后就是安装程序。
5、安装和调试程序
偶 是android的fans，因此，nexus one早早就刷了froyo 2.2，因此直接在手机上装了。鉴于很多朋友没有nexus one，偶在下面还是用emulator做演示比较好。
首先，运行android命令，新建一个2.2的emulator。（建立模拟器 的方法不再聒噪，如下图所示）
![[转]Android平台从Froyo <wbr>2.2开始支持jni单步调试了！](http://blogimg.chinaunix.net/blog/upfile2/100604145024.png)
其次，进入android-ndk-r4/samples/hello-jni/bin目录，应该能够找到一个apk安装包，直接安装即可。
$adb install ./HelloJni-debug.apk
1201 KB/s (78926 bytes in 0.064s)
        pkg: /data/local/tmp/HelloJni-debug.apk
Success
看到Success，就代表已经安装成功了。
然 后，注意，忍受了偶上面那么多废话的朋友们，下面就是关键的地方了！
$cd ~/android-ndk-r4/samples/hello-jni/
$ndk-gdb
此时，观察模拟器的反应，hoho，是不是看 到模拟器里面的hello-jni程序自己打开了？！
![[转]Android平台从Froyo <wbr>2.2开始支持jni单步调试了！](http://blogimg.chinaunix.net/blog/upfile2/100604145048.png)
然后就是ndk-gdb自动进入了一个命令行提示符的状态：
and track explicitly loaded dynamic code.
warning: shared library handler failed to enable breakpoint
0xafd0eb08 in ?? ()
(gdb) list
1      
17      #include <string.h>
18      #include <stdlib.h>
19      #include <jni.h>
20
(gdb) list
21      static void upper_str(char *s, int len) {
22        int i ;
23        for(i = 0 ; i < len ; i++) {
24          if((s[i] >='a') && (s[i] <='z')) {
25            s[i] = s[i] - 'a' + 'A' ;
26          }
27        }
28      }
29
30      /* This is a trivial JNI example where we use a native method
(gdb)
在linux做过开发的朋友们一定对gdb的命 令行模式非常熟悉吧？到这里就一切豁然开朗了。
可能有朋友要问，怎么有一大堆的warning，好象是说什么符号找不到？
其实这 很简单，android 2.2版本的模拟器自然是release版啦，debug版的一定慢得要死，所以，很多函数的库和符号是没有的，因此，在gdb里面根本找不到也是有情可原的，只要忽略掉这些就行了（偶是多么容易满足的人啊！在此赞叹一下。另外插一句，如果有人真的爱钻牛角尖，要弄清楚这个的话，可以尝试用android 的源代码编译一个debug版的android，偶试过的可以调试native的程序，而且也没有这些warning，不过又会是一个痛苦的过程）。
然 后就是常规的gdb调试命令了。
list-显示代码
x-查看内存
p-查看变量
b-设置断点
cont-程序继续运 行
s-单步执行，但可以跟踪进入函数
n-单步执行，但不会进入函数
吧啦吧啦吧啦，关于gdb的使用，偶就不再聒噪了，网上相关说 明非常多。
这里需要提醒一下，相信细心的朋友已经看到怎么偶的界面跟大家的hello-jni不一样？！
android 的在线调试的基本原理并不是本地的gdb程序，而是在模拟器（或者真机）上启动了一个叫做gdb-server的东西，通过adb的tcp中转，让开发者本机的gdb与模拟器（或者真机）上的gdb-server进行连接，然后进行调试。这个过程相对比较繁琐，感兴趣的朋友可以参考ndk-gdb这个脚本（hoho，想当年偶调试native程序的时候，是自己写的连接gdb和gdb-server的脚本，期间问候了无数次google的祖宗多辈，在此按下不表）
在hello-jni里面，jni函数的调用是放在activity的oncreate函数中的，也就是说一程序一启动就已经调用了jni，而在pc端的gdb联上模拟器的gdb-server的时候，jni已经运行完了，而且oncreate函数也不是随便说调用就调用的，因此，在什么地方下断点呢？！偶就想了个土法，那就是修改一下原有的hello-jni程序，在里面添加一个小button，按下这个button的时候才会调用jni的函数。
如此一来，使用ndk-gdb的时候只是启动了activity，并不会真的调用jni函数，当偶把函数的断点都设置好以后，再运行“cont”命令，然后单击一下按钮，这个时候才会真的跑到断点里面来。
为了大家方便起见，偶把修改过后的hello- jni的源代码给贴出来。hello-jni.tar.gz
希望本文能够对android平台开发的朋友们有所帮助。
原文地址：[http://blog.chinaunix.net/u/26691/showart_2247039.html](http://blog.chinaunix.net/u/26691/showart_2247039.html)
本文来自CSDN博客，转载请标明出处：[http://blog.csdn.net/jinhill/archive/2011/01/09/6124996.aspx](http://blog.csdn.net/jinhill/archive/2011/01/09/6124996.aspx)
本文来自CSDN博客，转载请标明出处：[http://blog.csdn.net/jinhill/archive/2011/01/09/6124996.aspx](http://blog.csdn.net/jinhill/archive/2011/01/09/6124996.aspx)
本文来自CSDN博客，转载请标明出处：[http://blog.csdn.net/jinhill/archive/2011/01/09/6124996.aspx](http://blog.csdn.net/jinhill/archive/2011/01/09/6124996.aspx)

