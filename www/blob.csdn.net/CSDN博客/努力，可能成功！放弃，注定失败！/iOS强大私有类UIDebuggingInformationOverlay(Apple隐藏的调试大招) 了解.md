# iOS强大私有类UIDebuggingInformationOverlay(Apple隐藏的调试大招) 了解 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2017年05月31日 14:14:21[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：1679
> 
本文来自简书，原文地址:[http://www.jianshu.com/p/a228cafd418f](http://www.jianshu.com/p/a228cafd418f)
今天吃完饭，有点闲。就去逛逛某博，看看关注的大神们最近有何动态，能不能学到啥，其实就是去抱抱大腿。然后刷着刷着，眼前发光，UIDebuggingInformationOverlay出现在眼见，哇，这么厉害的东西我怎么没见过呢，说的我好像很厉害的样子，哈哈。夸张了，大家别信，我没见过很正常。
UIDebuggingInformationOverlay是个什么东西呢，大家先来看个图吧：
![](http://upload-images.jianshu.io/upload_images/1488825-279074db866c0051.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
牛X的png.png
看到了吗，他可以在让我们在App中直接弹出一个系统的调试界面，图中间的View就是调试界面了。
网上是这样解释的：UIDebuggingInformationOverlay是一个UIKit的私有类，通过他可以弹出这样一个界面，方便开发者去调试自己的App。比如通过它查看自己当前contorller，view的层次结构。
![](http://upload-images.jianshu.io/upload_images/1488825-1570cac520fa4aa5.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
又是个牛逼的png
![](http://upload-images.jianshu.io/upload_images/1488825-04e16cda4e086c9f.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
还是牛逼的png
还有些其他强大的功能，比如查看变量，对比界面(将UI设计图片映射到App界面上形成对比，这样我再也不用和UI大哥争吵我有没有按照他的设计来开发了)。
![](http://upload-images.jianshu.io/upload_images/1488825-275287836821a215.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
开始对比UI.png
![](http://upload-images.jianshu.io/upload_images/1488825-e9ca2ab7463f099c.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
对比UI让大哥无话可说.png
UIDebuggingInformationOverlay的具体细节以及其他牛X的功能也就不解释了，我也刚刚才发现，别以为我多厉害，一下就学会了，这是在厚着脸皮在吹呢。
更多相关介绍可前往：[大神家的网址](http://ryanipete.com/blog/ios/swift/objective-c/uidebugginginformationoverlay/)
上面我吹牛到，我可以在自家APP随便弹出这个界面。但已有大神做到对别家App进行了牛x的弹框，膜拜，泪奔，无话可说，此处省略一万字的膜拜...不说了，留下截图，我要去拜师了。
![](http://upload-images.jianshu.io/upload_images/1488825-ec2882b31e12589e.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
超级牛.jpg
这是[大神的微博](http://weibo.com/575203337?from=feed&loc=at&nick=Naituw)
![](http://upload-images.jianshu.io/upload_images/1488825-e55ed6da88b7a74a.PNG?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
膜拜.PNG
喔对了，自己写了Demo，大家可以去看看，简单的尝试弹出这个框
[demo](https://github.com/hwzss/TestUIDebuggingInformationOverlay/tree/master)
调用这两行代码就可以了，然后再用两个手指tap下手机的status bar就可以了
```
Class Test = NSClassFromString(@"UIDebuggingInformationOverlay");
    [[Test class] performSelector:@selector(prepareDebuggingOverlay)];
```
# 更新补充
这是我拜师回来后的成果,（我怎么可能有人收呢，刚去偷学的。这是我的成果,在名为（Skeype）的app中进行显示:
![](http://upload-images.jianshu.io/upload_images/1488825-efab680210436177.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
厉害了.png
查看下界面层级：
![](http://upload-images.jianshu.io/upload_images/1488825-323f60ad92cec6ce.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
感觉很牛X了.png
![](http://upload-images.jianshu.io/upload_images/1488825-efb9f2fafcdee062.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
感觉自己很牛X了的_66666.png
Naituw大神使用的IPAPatch相关学习资料在这里([藏有宝藏的链接](http://weibo.com/ttarticle/p/show?id=2309404086977153611942))，学会了挺有趣的。让我们一起走在装X的道路上..
