# Xcode频繁意外退出的解决方案 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2016年08月02日 22:27:30[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：5770
在开发中经常会遇到项目冲突， 程序崩溃，Xcode异常等情况，有些问题第一次遇到时基本上就懵了,下面就来总结下常见的异常解决方案。
![](http://upload-images.jianshu.io/upload_images/1170656-2c39a185070bae64.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
看看怎么办
#### 完全不明所以的频繁退出
**第一种 排除SVN冲突**
在团队开发中，SVN冲突是最常见的了，程序异常时查看SVN文件冲突基本上成了本能。
![](http://upload-images.jianshu.io/upload_images/1170656-5ed1815ccc1e8505.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
排除SVN冲突
首先，右键主项目文件即xcodeproj文件，显示包内容，用文本编辑器查看包里的project.pbxproj，查找文件里是否有“<<<<<“ “=======”字符，有的话即表示冲突，删除冲突的地方即可解决。
![](http://upload-images.jianshu.io/upload_images/1170656-efe7d5be0702a48d.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
此种异常一般就是程序无法运行，或是无法提交SVN等。在异常情况下算是小case了。
**第二种 匪夷所思的频繁崩溃**
![](http://upload-images.jianshu.io/upload_images/1170656-f5132009ff39eed6.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
崩溃
向这种频繁崩溃的情况，直接暴力解决。
首先右键主项目文件即xcodeproj文件，显示包内容。找到找到project.xcworkspace文件，右键显示包内容。
![](http://upload-images.jianshu.io/upload_images/1170656-b9a2cf59c9453124.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
Snip20160702_3.png
删除xcuserdata文件夹，解决。
![](http://upload-images.jianshu.io/upload_images/1170656-b5943882fee31fc5.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
xcuserdata文件夹里保存着当前项目的一些临时信息，很多崩溃其实是加载失败导致的，删除这个文件夹，Xcode会重新布局加载信息。如此崩溃就解决了。
#### 小知识，程序崩溃的处理
程序在崩溃的时候，xcode经常没有给出准确的堆栈信息，而是定位在了main方法里，这个让人很是头疼，这个可以通过添加一个Exception Breakpoint，选择 All来解决。另一种是Crash,EXC_BAD_ACCESS ,这个比较头疼，因为Crash的时候，可能是比较早之前的某个变量释放了，现在访问时出问题。Console里也没显示什么日志。开Scheme选项选择Edit Scheme然后按图勾上Enable Zombie Objects 和Malloc Stack那两项，记住一般**只有在定位EXC_BAD_ACCESS时候才勾选**。
重新跑一下，如果是到Exception Breakpoint处停止了，可以在Console中输入：c（continue)按回车继续跑，直到Crash。看下Console是不是有跟SIGABRT类似的错误信息日志了。如果还没有日志，在Console中输入
```bash
po $eax
```
$eax标志出错的地方，适用模拟器，真机用$r0（话说EXC_BAD_ACCESS这种 错误模拟器定位就行）,“$eax”是cup的一个寄存器。在一个异常的情况下，这个寄存器将会包含一个异常对象的指针。注意：$eax只会在模拟器里面工作，假如你在设备上调试，你将需要使用”$r0″寄存器。还可以输入比如：
```bash
po [$eax name]     
po [$eax reason]
```
等指令查看错误其他信息（注意方括号后没分号的）。
文／吴白（简书作者）
原文链接：http://www.jianshu.com/p/f55f3a4db2f0
著作权归作者所有，转载请联系作者获得授权，并标注“简书作者”。
