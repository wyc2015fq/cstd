# 【关于ffmpeg和Android的种种】【VPlayer不开源】【Android常用调试方法】 - 三少GG - CSDN博客
2011年06月10日 21:01:00[三少GG](https://me.csdn.net/scut1135)阅读数：7400
1.  http://9.douban.com/subject/9306635/
### Android NDK可用的FFmpeg源码
2011-02-09 01:34:01[查看原文](http://9.douban.com/site/entry/171743743/view)
首先为自己的错误郑重的道歉，第一个错误是声明为”free forever“的VPlayer最后成了收费的了，第二个是没有及时的公布用到的FFmpeg源码。错了就是错了，没有什么值得为自己辩解的，得让实际行动证明一切。
VPlayer使用了[FFmpeg](http://ffmpeg.org/)的代码，基于[LGPLv3](http://www.gnu.org/licenses/lgpl.html)协议，修改过的代码可以[点击这里](http://vplayer.net/projects/vplayer/files)下载。
下载之后使用非常简单，**FFmpeg的代码没做任何修改，不需要configure，编译过程使用的是其中的Android标准编译文件Android.mk。只要把代码放到一个标准的Android工程中的jni目录下，运行ndk-build即可。**
之前写过一篇[用Android NDK编译FFmpeg](http://abitno.me/compile-ffmpeg-android-ndk)，写的过于繁琐，一直承诺会更新一个简单易用的方法，现在算是兑现了。
从那之后，这个博客也很少更新了，其实不只这里，我的[Twitter](http://twitter.com/abitno)等帐号也算是废弃了，基本上都是在开发VPlayer这个播放器，期间有很多学习和成功的快乐，也有各种纠结的事情，幸好有大家的支持，更幸运的是有星星陪我。
顺便说一下，随着Android NDKr5的发布，一切都变的简单起来，只要你会普通Linux上的编译流程，你应该也会用它。我也不介绍这种了，因为我没试过，只简单的看了下更新日志里有提到。
依然不停的思考 AND 编码，为了美好明天。。。
附注：现在好多Android上的播放器在使用FFmpeg，似乎都公布了**FFmpeg的代码**，可它们公布的貌似不是自己用到的。简单的验证：按照他们的文档介绍看一下能否正常编译，如果能正常编译，把得到的libffmpeg.so与他们程序里的对比一下大小是否一致，然后替换他们程序里的libffmpeg.so，看看还有没有同样的效果。
最后大家兔子年快乐～
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
2. http://blog.csdn.net/lxglxt/archive/2011/02/18/6193999.aspx
# 把ffmpeg移植到android的总结
终于成功做到了，现在才有时间来写一些关于技术的总结。
在这个工程中，有几个重要的参考，在我的链接中有。
冯晓东的博客，有一篇文章《Android用NDK编译Ffmpeg》，讲述ndk编译Ffmpeg设置步骤，基本上是按照这个来做的，很成功。
havlenapetr的档案中，有**一个完整的开源源码，实现了播放。这个给我的帮助很大，很多技术是从这里得到的。本来想在这个基础上更改成为一个播放器，后来还是放弃了。其对音视频同步处理并不成功，再就是显示部分我可以运行他的，但模仿不成功。**
tewilove的档案，是另一个开源代码，但有很大的不同。
JesseZhao(赵光)博客园，对Ffmpeg的代码分析和音视频同步处理讲解的非常好。
参照这些，基本框架和Ffmpeg的编译就可以完成了。从**网络上的一些交流看，最大的技术难点还是显示和音视频同步处理。**
我的显示处理实现：
视频祯在使用Ffmpeg解码后，使用sws_scale()进行格式转换和缩放。Ffmpeg解码出来是YUV420P的格式数据，在我的android2.2中，是不支持的，无法直接显示的。因此转换为RGB565格式的数据。
在java代码中，创建了一个view，将其Surface句柄传递给C代码层。C代码显示，基本就是对Surface操作了。Surface的lock操作后，获取buffer指针，sws_scale()数据转换时，将输出直接到这里就可以了。
音视频同步，参照赵光的博客，注意的一点就是，要把视频同步到音频是很关键的。因为对毫秒级别的延迟，人对声音比图像更敏感。
在完成这些后，新机型出来了，在我的10寸屏上，播放高清，简直是灾难，图像很卡。
对于多媒体，如果使用较高分辨率，更多的是依赖于硬件解码，软件解码，只能适合低分辨率，小屏幕的产品。
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
3. 好问题，不过尚未解决：
# [[多媒体]](http://www.eoeandroid.com/forum-forumdisplay-fid-45-filter-typeid-typeid-30.html)[android如何调用本地库绘图](http://www.eoeandroid.com/thread-75072-1-1.html)
android的**开源多媒体播放器havlenapetr-FFMpeg,视频显示是调用libjnivideo.so里的函数AndroidSurface_update();**
我想自己在native 层直接调用库<SkBitmap.h><SkCanvas.h>等里面的函数来绘图，应该怎么做呢?
在Cpp代码里面,直接导进来 是找不到的：
#include <SkBitmap.h>
#include <SkCanvas.h>
编译时报错：
error:SkBitmap.h : No such file or directory
求教高人指点一下，不胜感激！
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
4. 好问题，不过尚未解决：
http://www.eoeandroid.com/thread-78932-1-1.html
下了havlenapetr的FFMpeg
他自带了libjniaudio.so和libjnivideo.so
编译通过后运行，当点播放时程序挂了，查看日志显示：
06-07 09:46:29.365: DEBUG/dalvikvm(356): Shared lib 
'/data/data/cz.havlena.ffmpeg.ui/lib/libjniaudio.so' already loaded in 
same CL 0x43d0b8d8
06-07 09:46:29.384: INFO/dalvikvm(356): Unable to dlopen
(/data/data/cz.havlena.ffmpeg.ui/lib/libjnivideo.so): Cannot load 
library: find_library[1156]:    30 'libjnivideo.so' failed to load 
previously
06-07 09:46:29.435: INFO/dalvikvm(356): Unable to dlopen
(/data/data/cz.havlena.ffmpeg.ui/lib/libffmpeg_jni.so): Cannot load 
library: find_library[1156]:    30 'libffmpeg_jni.so' failed to load 
previously
用系统自带的libjniaudio.so和libjnivideo.so运行显示错误：
06-07 10:04:58.765: INFO/dalvikvm(434): Unable to dlopen(/data/data/cz.havlena.ffmpeg.ui/lib/libjniaudio.so): Cannot load library: reloc_library[1244]:    30 cannot locate '_ZN7android10AudioTrack3setEijiiijPFviPvS1_ES1_iRKNS_2spINS_7IMemoryEEEbi'...
06-07 10:04:58.885: INFO/dalvikvm(434): Unable to dlopen(/data/data/cz.havlena.ffmpeg.ui/lib/libjnivideo.so): Cannot load library: link_image[1721]:    30 could not load needed library 'libsurfaceflinger_client.so' for 'libjnivideo.so' (load_library[1051]:
 Library 'libsurfaceflinger_client.so' not found)
06-07 10:04:58.936: INFO/dalvikvm(434): Unable to dlopen(/data/data/cz.havlena.ffmpeg.ui/lib/libffmpeg_jni.so): Cannot load library: link_image[1721]:    30 could not load needed library 'libjniaudio.so' for 'libffmpeg_jni.so' (find_library[1156]:    30 'libjniaudio.so'
 failed to load previously)
谁用过他的FFMpeg，也来说一下这个libjniaudio.so和libjnivideo.so怎么弄呗，按他官方说要从他自己的framework/base下重新编译，怎么编译，我把他的Framework/base替换android源码中的framwork/base，编译报错
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# Android调试程序正确进行方式
[http://developer.51cto.com/art/201001/180491.htm](http://developer.51cto.com/art/201001/180491.htm)
摘要：
第一种是用Debug，设个断点就可以跟踪，但是我觉得不爽，我用System.out用惯了，不用System.out也可以用Log的。
第二种就是我要介绍的Log，看了别人介绍的方法之后，自己亲身试验了再写上来的哦~。首先简单介绍一下Android，Android实际上应该算是一种Linux移动平台的另一个版本(我对Android研究不深，我就是这么认为的)，那么既然是Linux就必定会用到命令。那么如何用命令运行程序呢？用adb命令！键入“cmd”，再键入“adb shell”，出现了个“#”号，恭喜恭喜，你可以使用命令来控制Android了。
运行“am -help”，可以查看“am”命令的帮助信息，试试运行“
am start -n com.google.android.browser/com.google.android.browser.BrowserActivity
”
看看？呵呵，在模拟器里面可以看到运行了浏览器，哈哈，Android调试程序就是这么简单(简单个P，为了找这个东西花了我好久的时间)。
tag是一个标识,可以是任意字符串,通常可以使用类名+方法名, 主要是用来在查看日志时提供一个筛选条件.
程序运行后 并不会在 ide的控制台内输出任何信息，那么如何查看日志输出？使用"adb logcat" 命令：
         adb logcat  
当执行 adb logcat 后会以tail方式实时显示出所有的日志信息.
+++++++++++++++
**DDMS 视图   +  adb +LogCat**
