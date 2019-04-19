# iOS开发插件推荐（1）：利用神插件injectionforxcode倍增你的开发效率 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2016年05月23日 09:41:33[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：1147
### 前言
这款插件曾经被我扔到了垃圾堆，直到最近UI部门要联调App,为了改一个像素，改个颜色，我不得不修改代码，停止运行，再重启模拟器，再一个页面页面的跳转到调试所在页面，其实崩溃的不是代码不是代码，是我是我是我。。。还好我想起了它，还好有`Github`，否则我将追悔莫及，恨一万年都不够。今天就跟大家介绍这款神级插件`injectionforxcode`。
### 神奇之处
在程序运行过程中插入`OC/Swift`代码，不需要重启模拟器就可以查看模拟器运行效果。
![](http://upload-images.jianshu.io/upload_images/1112722-b44f2349c108a9c6.gif?imageMogr2/auto-orient/strip)
injected.gif
### 开门请神
跟安装其他插件的方式一样，推荐使用Alcatraz插件管理器安装。
打开Xcode我们可以看到Product菜单下多出了如下两个选项代表你请神成功了。
![](http://upload-images.jianshu.io/upload_images/1112722-03f6d34018a846d8.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
Paste_Image.png
### 召唤神
1.在APP运行过程中修改代码，比如在controller中加入以下方法
```python
```python
- (void)injected
{
    NSLog(@"I've been injected: %@", self);
}
```
```
2.按下`ctrl` + `=`，有没有出现一个加载的进度？？？有的话，说明你插件安装成功，马上你就可以看到控制台打印了***I've been injected***。是不是很神奇，如果你不这么觉得我就敢说你是神经病。
### 拜拜神，给你带来财富
现在你可以尝试在在代码插入任何代码了，记住按`ctrl` + `=`召唤神，让模拟器立刻响应你的代码变化吧，看看神是如何神奇地提高你的调试效率的。
### 它为什么这么神
它通过解析应用的build日志来判断源代码文件上次是怎么被编译的。然后会把这些重新编译一遍包在一个已经通过动态加载器（`dynamic loader`）注入到应用的`bundle`里。这个时候其实有两个版本的类在app里，一个原始的和一个修改过的版本。修改过的版本通过转发`"swizzled"`作用在原来的类上来产生效果。
这个`swizzling`技巧事实上利用了OC的运行时绑定方法调用和方法实现的来达到目的。这个也可以在Swift中没有标记为`final`或者`private`的方法（比如可以被`override`的方法）。
**原理图**
![](http://upload-images.jianshu.io/upload_images/1112722-fa5b4d516138bb70.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
yuan'li'tu
### 提供的技能
它提供了三个实现回调的方法
1.- (void)injected实例方法。它可以让我们按下`ctrl` + `=`更新一个类。比如在该方法里调用viewDidLoad方法，就可以再次调用控制器的viewDidLoad方法。
2.+ (void)injected类方法，可以让我们更新全局函数。
3.监听名为`INJECTION_BUNDLE_NOTIFICATION`的通知，因此我们可以让其他类监听该通知。
4.[使用tunable_parameters](https://github.com/johnno1962/injectionforxcode/blob/master/documentation/tunable_parameters.md)
![](http://upload-images.jianshu.io/upload_images/1112722-f6c15f030173f70a.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
Paste_Image.png
### 神是万能的
- 1.我其实还支持Storyboard [Storyboard-based iOS projects](https://github.com/johnno1962/injectionforxcode/blob/master/documentation/storyboards.md).
- 2.我其实也可以可视化的调整颜色 [collection of tunable parameters](https://github.com/johnno1962/injectionforxcode/blob/master/documentation/tunable_parameters.md)
- 3.当你开始注入时，我会在同一个目录下生成一个项目文件：iOSInjectionProject 或者 OSXInjectionProject。这个项目用于注入你的修改到原项目。**建议加入.gitignore。**
- 4.`ctrl`+``=的快捷键可以在 "Tunable App Parameters"面板里自定义
- 5.如果想在一台设备上使用，参考这个文档[patch to your project.](https://github.com/johnno1962/injectionforxcode/blob/master/documentation/patching_injection.md).
低调低调，你知道我很神就行了。
### 神仙下凡时
我再神也要下凡的啊，不要笑话就行啊，看看就得了，敢笑话我？我就让你写的程序一直有`bug`,一直有一直有Z Z Z 。
![](http://upload-images.jianshu.io/upload_images/1112722-c86a8d37a84fd4f3.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
Paste_Image.png
- 1.为了防止出现两个单例（之前提到swizzle会创建一个相同的类），shared前缀修饰的类方式不能注入代码。
- 2.dispatch_on_main方法不能注入。
- 3.第三点我看不懂，不知道怎么翻译，可以去Github看起官方原文，不好意西。
### 找到此神
GitHub :[Injection Plugin for Xcode](https://github.com/johnno1962/injectionforxcode)
作者twitter :[@orta](https://twitter.com/orta)
作者亲自录的教程（英文） :
[https://www.youtube.com/watch?v=uftvtmyZ8TM&feature=youtu.be](https://www.youtube.com/watch?v=uftvtmyZ8TM&feature=youtu.be)
### 补充
给你介绍这么好的东西，你不需要谢谢我，不要给我打赏，千万不要，你要是打赏了，我一定不会。。。手软。
文／jackiehoo（简书作者）
原文链接：http://www.jianshu.com/p/070d8b1a6190
著作权归作者所有，转载请联系作者获得授权，并标注“简书作者”。
