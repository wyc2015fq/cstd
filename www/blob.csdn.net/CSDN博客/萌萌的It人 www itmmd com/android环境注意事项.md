
# android环境注意事项 - 萌萌的It人 www.itmmd.com - CSDN博客


2015年02月02日 13:10:36[Jlins](https://me.csdn.net/dyllove98)阅读数：2082


**这两天一直在配置环境问题，****我总结出一个方法就是（不懂的地方一定要好好查百度谷哥，千万别蒙混过关，因为开始的时候会遇到好多问题）****出现了如下问题：**
在安装eclipse的时候，我发现我不清楚选择哪个版本的开发工具，后来选择了luna这个版本来进行开发，同时我发现外国人对这个版本命名的方法就是用人名来区分的。一张图就知道什么情况了。

**但是在网上查询之后，eclipse的版本最后是选择****juno和indigo这两个版本比较好****，因为越是大的版本相反他对电脑的配置要求也是很高的。所以，****建议不升级eclipse。**
**所以我们解决了eclipse的版本问题。但是我们知道安装不同的版本需要不同的ADT来支持eclipse，所以我查询到juno版本使用adt18就可以支持到安卓系统4.0，所以在下载sdk的时候不能选择太高的API（这是我的理解），而且一旦自己升级SDK的话，可能导致需要更高的eclipse版本和adt插件。**
**那么一旦你升级了sdk怎么办，我的想法是直接覆盖掉之前的版本，然后再次使用时候就就不要升级sdk工具，否则又需要使用高版本的adt了。**
**所以我查询来了好多资料我知道这么个情况：**
**indigo 3.7.2+adt18，可以支持到安卓平台4.0了，如果你想android4.2的，那么就需要升级ADT和sdk**
**juno+adt22 支持到安卓4.2**
**luna+adt23或者更高 支持安卓4.4和安卓L等更高版本。**
**所以当你安装完adt，打开sdk的时候肯定会提示你更新这个跟我们平常用的软件不一样，不一定是向下兼容的同时也不是版本越高越好！！（害得我弄了2天- - ）**
**而且升级的越高，到时候你会发现4.2以上的版本考虑到兼容低版本同时会出现好多其他的包，而且也可能会导致无法执行。（后面遇到过这种情况）**
后来了解到sdk和adt是对应的问题，而我想写android4.2版本的，那么我需要下载一个juno版本的eclipse然后与之对应的adt22就可以开发了，切记不要升级。。。
那么现在需要了解的一个问题是：怎么让sdk和adt还有eclipse版本相对应呢？
我们安装的顺序最好是：eclipse——adt——sdk
那么一个就是离线安装adt 这个在网上我查到过（百度吧）就是解压adt然后点击local安装。
一个就是选择合适的安卓sdk版本 记得不要选择过高的版本导致需要升级adt
DT 18.0.0 (April 2012)
**Dependencies:**
**Java 1.6 or higher is required for ADT 18.0.0.**
**Eclipse Helios (Version 3.6.2) or higher is required for ADT 18.0.0.**
**ADT 18.0.0 is designed for use with SDK Tools r18.**
那么接下来的问题就是：配置eclipse。
字体配置

**在菜单windows-->prefereces里面，找到General-->Appearance-->Colors and Fonts，然后在右边找到你要修改的字体或背景，点Edit...即可。**
像我这种菜鸟也要配置一下软件代码补全。
**Eclipse中类文件（*.java）自动补全配置：**
**Windows->preferance->java->Editor->Content Assist**
**面板最下端 Auto Activation 将Auto activation triggers for Java后面的文本框中的"."替换成"abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ."（注意后面还有一个"."）**
**Eclipse中*.xml文件自动补全配置****：**
**Windows->preferance->XML->XML Files->Editor->Content Assist**
**面板最上端 Auto Activation 将Prompt when these characters are inserted后面的文本框中的"<=:"替换成"<=:abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ "（注意后面还有一个空格）**
其他android学习教程

## [android学习笔记（57）Handler消息传递机制](http://www.itmmd.com/201502/575.html)
## [android学习笔记（56）android重写onConfigurationChange...](http://www.itmmd.com/201501/570.html)
## [android学习笔记（55）android Configuration响应的系统设置的事...](http://www.itmmd.com/201501/568.html)


