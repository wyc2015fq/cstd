# 移植havlenapetr的ffmpeg-->Android播放器 - 三少GG - CSDN博客
2011年06月10日 12:26:00[三少GG](https://me.csdn.net/scut1135)阅读数：14449

+++++++++++++++++++++++++++++++++++声明+++++++++++++++++++++++++++++++++++++++++
下列文章引用了我的内容，却未公布转载我的博客地址**！！！**
[1.  http://www.ajavn.com.sixxs.org/anzhuotuandui/4351.html](http://www.ajavn.com.sixxs.org/anzhuotuandui/4351.html)
来自安卓手机论坛的一篇文章http://www.ajavn.com.sixxs.org/
2. [http://hi.baidu.com/eefolks/blog/item/e0329e4682859129cefca351.html](http://hi.baidu.com/eefolks/blog/item/e0329e4682859129cefca351.html)
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
全格式！！！
重点文章（包括文章后的评论）：
[**http://blog.csdn.net/moruite/archive/2011/04/06/6305944.aspx**](http://blog.csdn.net/moruite/archive/2011/04/06/6305944.aspx)
> 
# ![原创](http://blog.csdn.net/images/authorship.gif)   解决Android平台移植ffmpeg的一揽子问题
”
> 
[havlenapetr](https://github.com/havlenapetr/FFMpeg) 最新的ffmpeg可以直接编译通过，不过有个bug,就是播放完毕后，video_decoder线程不能退出，主要原因是阻塞在packet队列的Queue->get函数里，这里处理逻辑需要考虑两种情况：
> 
1. AVPacket队列里没有音视频帧数据时，解码线程挂起进入等待状态，有新的AVPacket加入队列时激活解码线程
> 
2. 如果文件读取到末尾，Queue->get直接返回-1，解码线程发现队列空时直接退出
“
## havlenapetr/FFMpeg 下载地址
https://github.com/havlenapetr/FFMpeg
注意察看issues和文档更新说明！！ 目前更新日期2011/5/25
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
**步骤一.**
http://www.tlightsky.com/?p=55001
首先从git里面拖出debug分支的代码，时间是【2010年 11月 26日 星期五 22:48:41 CST】，注意目前的代码里面是
有bug的（在Vector里面），所以我们回滚两个周期
git reset –hard HEAD~2（就是在他加入Vector之前，这个可以看git log）
ok，现在我们尝试编译，**1. 在 (**"repair bulding process for everyone - you must build in ndkr5**")ndkr5根目录执行ndk-build。 具体指令为/usr/local/...ndk../ndk-bulid  NDK_PROJECT_PATH=/.../havlenapetr-FFMpeg  (下面的目录为jni !!!会在同一级内生成lib,obj文件夹)  注意：此时欲生成libffmpeg_jni.so  但需要如下的两个so文件:**
报错，**发现没有obj/local/armeabi/libjniaudio.so**
**git checkout libs/armeabi/***
**cp libs/armeabi/* obj/local/armeabi/**
**拷贝havlenapetr同学为我们编译好的两个so文件（这个是区分版本的！下面会说明）。**
**完整指令（针对**Android NDK r5 **&Android 2.2 ，整个编译时间大概10多分钟）： **
/usr/local/android-ndk-r5/ndk-build NDK_PROJECT_PATH=/.../havlenapetr-FFMpeg NDK_TOOLCHAIN=arm-eabi-4.4.0 NDK_PLATFORM
=android-8
先别急着编译
现在，我们在再修改
jni/jni/Android.mk
ifeq ($(IN_NDK),true)
LOCAL_LDLIBS := -llog
else
LOCAL_PRELINK_MODULE := false
LOCAL_SHARED_LIBRARIES := liblog
endif
改为
LOCAL_LDLIBS := -llog
—————————————————————-
好了现在你可以编译了～～
**步骤二：**
**2.写一个（或者从其他项目拷贝）default.properties**.( 里面其实没多少内容，只是定义默认版本貌似 ：# Project target.   target=android-9）
**重启eclipse**，即可看到build path-自动添加了system library    Android2.3。（之前不要手动添加android包，否则此时会弹出“Dx1 error; abortingConversion to Dalvik format failed with error 1错误”即有重复包）
**步骤三：**
接上，然后**3.自动 会生成gen文件夹， 里面有R.java**
#  补充1：android R.java 文件出错怎么办
1.先检查你的代码，代码有误，R.java文件是生成不了的
2. 把Project ----> Build Automatically那个选项勾上，让工程每次修改完后都会自动编译，这样R.java会是最新的（这时候程序有误，不会生成）
3. Project ---> Clean，点开后，选中你的工程，然后OK，会Reset你的工程设置，然后重新Build一下，这样三步下来，就能搞定你的问题了。
 补充2： Android R.java问题汇总
1. The type R is already defined. （很多时候我们在导入其他人的程序的时候，会遇到这个错误）
通常在project里有两个R.java，一个在src，一个在gen，通常删除src里那个保留gen里的就OK
2.R.java无法修改
R.java是ATD自行产生的，ID号都是唯一的，如果不熟悉，最好是不要动这个里面的东西，这个类里面通常定义的都是project的一些resource信息，attr、drawable、id、raw、layout、string以及xml等
如果发现R.java没法修改，要去看你的配置文件，比如xml等是否有错误。例如一个图片资源文件，如果我没有放在res/drawtable下时，R.java是没法修改的，当你把这个资源拷到res/drawtable下后，R.java就可打开了。
2.R.java不能自动更新
1）是你的project配置有问题
       XML格式错误等等
2) 设置的问题
       i. 打开ECLIPSE的Poject-->Build Automaticaly ，把它勾起来，以后就会自动给你维护R.java
       ii. 或者右击你的project，Android tools-->Fix project properties
       ii. 再不行，同样打开eclipse的project-->clean,点下会reset你的project的配置的，然后再rebuild下你的project
以上是本人之前学习时遇到的，如有表达不好的，望斧正，后续继续更新
**步骤四：**
此时，仍会报错：
- The type new MediaController.MediaPlayerControl(){} must implement the inherited abstract method
 MediaController.MediaPlayerControl.canSeekForward()
- The type new MediaController.MediaPlayerControl(){} must implement the inherited abstract method
 MediaController.MediaPlayerControl.canSeekBackward()
- The type new MediaController.MediaPlayerControl(){} must implement the inherited abstract method MediaController.MediaPlayerControl.canPause()
fixed as: add unimplemented methods
**4.实现相应的接口（内容无所谓）**
添加如下代码：
        public boolean canSeekForward() {
            return false;
        }
        public boolean canSeekBackward() {
            return false;
        }
        public boolean canPause() {
            return false;
        }
参考官网说明 公有方法： http://www.zuiniuwang.com/doc/reference/android/widget/MediaController.MediaPlayerControl.html
**5. 重新编译使用的 **libjnjvideo.so,libjniaudio.so的版本！**！！（转向步骤1）**
**自带libjnjvideo.so,libjniaudio.so 是Froyo(2.2) 版的。如果是其他版本，需要自己编译Echair(2.1)or Gingerbread(2.3 对应api-9).**
**用eclipse build（即第一步）之前 ，要把libjnjvidoe.so libjniaudio.so copy 到****libs/armeabi/ 和 obj/local/armeabi/**（两个目录下）
如何得到呢？如果是2.2 用自带的即可。这对2.3，
> 
问题：libjniaudio.so、libjnivideo.so 怎么编译的？
回答： these two libs are **wrappers**
to android native AudioTrack and surface flinger so they couldn't be build in ndk. So I have build them in android build system and copy them into my ffmpeg project
通过nm命令，可以看到，libjniaudio.so主要提供了AudioTrack相关接口，libjnivideo.so主要提供surface相关接口。这些东西包含在android源码下，必须在android build 环境中编译。
**如何编译？哥终于搞出来了！**
**两篇极其重要参考文章:**
**1. 经典文章，尤其留意后面的一系列留言及留言人的博客 [http://blog.csdn.net/moruite/archive/2011/04/06/6305944.aspx](http://blog.csdn.net/moruite/archive/2011/04/06/6305944.aspx)**
**2.  上面某一留言人的博客，讲解如何编译两个so文件**
[**http://blog.csdn.net/piaozhiye/archive/2011/04/26/6363512.aspx**](http://blog.csdn.net/piaozhiye/archive/2011/04/26/6363512.aspx)
其实，归根结底还是havlenapetr的网站上的issues 2，3等等
下面开始讲我的成功经历：
首先：
是要编译libjnivideo.so和libjniaudio.so 这两个so因为不同的版本或者修改过的framework是不一样的，所以需要在android源码中编译出自己的so。主要关注 framework/base目录 具体参考havlenapetr给出的Android2.2的目录结构
[https://github.com/havlenapetr/android_frameworks_base](https://github.com/havlenapetr/android_frameworks_base)
去上面网址base/native下的内容补充到android2.3的对应目录下。所做改变为增加了surface.cpp&h,audiotrack.cpp &h 及相应的mk文件。
（如果是想在2.1下 还要修改相应的头文件等（android的系统文件改变的原因）详见：[https://github.com/havlenapetr/FFMpeg/issues/3](https://github.com/havlenapetr/FFMpeg/issues/3)）
添加 libjniaudio.so and libjnivideo.so to the end of myandroid/build/core/prelink-linux-arm.map
然后重新编译整个android系统...漫长的 一个小时20分钟（**80分钟**）。。。（以后开发调试可肿麽办）
then I get the libjniaudio.so and libjnivideo.so in out/target/product/generic/obj/lib for Gingerbread.
同时也要把刚才两个编译出来的so放到obj/local/armeabi目录下，之后就可以了编译出libffmpeg_jni.so.
最后也要把刚才两个so放到libs/armeabi目录下。要不然运行的时候找不到。
播放的时候太卡了，声音也没有同步。
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
此编译过程参考文献为：
1. 编译havlenapetr的ffmpeg工程播放视频
http://wingjang.blog.163.com/blog/static/47913442201112161310334/
2. Compile havlenapetr / FFMpeg for android 
http://hi.baidu.com/eefolks/blog/item/e0329e4682859129cefca351.html
3.  Compile android source code on Fedora 13
http://hi.baidu.com/eefolks/blog/item/9b195fc5f722dc149c163db1.html
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
思考？
1. 只用sdk  如何编译： 不用eclipse的话
2.下面列出几个非常不错值得关注的网站！（跟本次内容不一定相关）
1. http://www.tlightsky.com/
 eg.
## [如何移植Android源码里面的东西到NDK](http://www.tlightsky.com/?p=58001)
## [Android源码分析之MediaFramework](http://www.tlightsky.com/?p=51001)
