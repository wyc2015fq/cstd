# Xcode 根据崩溃信息定位到某一行代码 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2019年01月17日 11:42:11[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：106
### 前言
最近，因为调试的问题浪费了不少时间，之前不打印崩溃日志的问题，近日又遇到崩溃信息无法定位到具体代码，只是简单的报错信息：
类似这样的：
`-[NSNull length]: unrecognized selector sent to instance 0x1b74e0878`
表面看是因为 调用 length 崩了，全局搜了 length 打断点也无济于事，实在头大，找了各种方法：
方法一：
第一步：需要保存打包的dSYMs文件(查找的时候一定要找到对应的dSYMs文件，不然找不到对应的代码)
![](https://upload-images.jianshu.io/upload_images/4790087-cbe18b5f32fa69cd.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/754/format/webp)
image.png
找到最终需要的符号表文件：
![](https://upload-images.jianshu.io/upload_images/4790087-7c149478f01f2d18.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1000/format/webp)
image.png
第二步：打开终端，
> 
dwarfdump --arch=arm64 --lookup 0x1b6e54878 `（报错的内存地址）` /Users/username/Library/Developer/Xcode/Archives/2018-11-11/GantangBus\ 2018-11-11\ 下午8.08.xcarchive/dSYMs/GantangBus.app.dSYM
后边为项目符号表的目录，一定是到 app.dSYM 结束，回车：
![](https://upload-images.jianshu.io/upload_images/4790087-e3b860755a4e7623.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1000/format/webp)
image.png
恭喜你中奖了，然并卵，这种方式实用于 release 版本的，本地打包的 release 版本也不行，因为 debug 版本 打包后压根没有 符号表文件，并且是上线以后用报错的地址，在上线的这个符号表中才能知道的，适用于上线后的调试，我猜测是这样，苹果有一个随机地址分配。
像集成腾讯 的 Bugly 上线后上传符号表文件，随后就看到找到相应报错的代码在第几行了，其他的一个原理。
方法一 kill;
方法二：
其实就是 Xcode 的全局断点，突然给忘了，忽略了，一时没想起来，网上查阅时才想起来，以前有的时候可能会不好使，有的好使，其实很简单 ，如下：
- 在 XCode 界面中按cmd + 相应的序号，快捷键，或者直接点击选项卡，跳到Breakpoint的tab 下
![](https://upload-images.jianshu.io/upload_images/4790087-b7e4323c3c3dc048.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/606/format/webp)
image.png
- 然后点击左下角的+号，增加一个Exception的断点，如下图所示。
![](https://upload-images.jianshu.io/upload_images/4790087-224f84925c844e46.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/426/format/webp)
image.png
- 点击接下来会出现一个“All Exception”的调试选项：
![](https://upload-images.jianshu.io/upload_images/4790087-400fbec66e79de47.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/648/format/webp)
image.png
其实已经 OK 了，一个全局断点已经添加了，将鼠标放到上面，右击选择“Edit Breakpoint”，可以查看选项的具体内容如下：（不用做任何修改，也可以设置相应的条件，编辑断点）
![](https://upload-images.jianshu.io/upload_images/4790087-87f22039506a47e7.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/952/format/webp)
image.png
OK，大功告成，当异常出现时，会自动停在异常处，而不会抛出到UIApplicationMain。就可以定位到具体抛出异常的代码了，完美的解决了我的问题，以前没发现这么好使，就没怎么用了，可能没注意到吧！
直接定位到我的代码行数，原来是因为后台的数据有一条为 null 的情况，没有做处理造成的。
> 
attributionName = "<null>"
作者：Superman168
链接：https://www.jianshu.com/p/6b6a6ca68c2b
來源：简书
简书著作权归作者所有，任何形式的转载都请联系作者获得授权并注明出处。
