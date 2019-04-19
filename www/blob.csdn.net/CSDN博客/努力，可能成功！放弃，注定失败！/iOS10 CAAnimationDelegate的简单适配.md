# iOS10 CAAnimationDelegate的简单适配 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2016年09月26日 22:34:57[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：4190
# 前言
最近关于适配iOS10的文章有很多，内容覆盖的点都差不多，只是详略差异，本文只提一个简单的点：`CAAnimationDelegate`的适配，像这样的点，iOS10中有很多，都是坑。。。
# 1.iOS10中CAAnimationDelegate的警告
原有的工程用xcode8打开编译后，莫名的增加了许多警告，其中关于动画的警告有这样一个，虽然运行后发现并没有什么影响，但还是要探究一下：
![](http://upload-images.jianshu.io/upload_images/1468630-9950580a44159699.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
举个�
关于警告的原因可以参考：
> 
[IOS Assigning to 'id<Delegate>' from incompatible type...解决办法](http://blog.csdn.net/u010983974/article/details/51034940)
在这里的原因就很明了了，动画的代理没有遵循协议。
于是：
![](http://upload-images.jianshu.io/upload_images/1468630-8da72afffa0039b6.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
接着举�
警告消除，万事大吉~
等下，真的就这么万事大吉了？把这段代码再用xcode7编译一下试试。。。报错了。。。。。。为什么呢？
仔细想下，在iOS10之前写动画的协议方法，从来就没记得要去遵循系统的动画代理，iOS10偏偏又因为这里给了个警告，还是先仔细对比下API吧：
![](http://upload-images.jianshu.io/upload_images/1468630-73492745d08d28f1.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
iOS9以前的CAAnimationDelegate
![](http://upload-images.jianshu.io/upload_images/1468630-5a36197f0b61a3fb.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
iOS10以后的CAAnimationDelegate
真的是有变化的，iOS10之前，`CAAnimationDelegate`只是基类的分类方法，是扩展，至于为什么也要像协议方法那样指定代理对象，应该是和其内部实现有关（只是用该指针在内部调用对应类的方法），需要代理对象。但在iOS10以后，`CAAnimationDelegate`从分类蜕变为独立的协议方法了，所以不遵循协议，就给了警告。仔细观察一下`CAAnimationDelegate`的`delegate`对象，它一直是一个`strong`类型，一般代理变量不都是`weak`吗？关于这点，本人只是结合API的注释理解为：animation是跟随layer->view的生命周期的，需要特定的设置释放，所以循环引用的问题不作考虑。
好吧，由于本人对于代理模式的理解并没有那么深刻，上述臆测多少是有偏差甚至是错误的，还望斧正。
# 2.iOS10中CAAnimationDelegate的适配
上面啰嗦了一堆没有什么“价值”的东西，还是切入主题，说说怎么适配这个新特性吧。
关于这样的新特性，本人用了下面的方式去进行版本适配，不知道方法算不算好，但是有效：
利用`__IPHONE_OS_VERSION_MAX_ALLOWED`系统宏进行条件编译，做法如下：
![](http://upload-images.jianshu.io/upload_images/1468630-8441f916d0298f30.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
还是个�
这样，在不同版本的环境下运行就做了区分，再等等……xcode7上编译还是报错，这段代码有什么问题吗？
```
#if __IPHONE_OS_VERSION_MAX_ALLOWED >= __IPHONE_10_0
  @interface ViewController () <CAAnimationDelegate>
  #else
  @interface ViewController ()
  #endif
```
有！问题在于`__IPHONE_10_0`这个宏，这个宏代表了系统版本，每次系统跟新，宏也会对应增加新的，看下其定义：
```cpp
```cpp
#define __IPHONE_9_3      90300
  #define __IPHONE_10_0    100000
```
```
这就是问题所在了，因为旧版本的API中，下面关于iOS10这句版本宏定义，根本不存在，也就是说，`#if __IPHONE_OS_VERSION_MAX_ALLOWED >= __IPHONE_10_0`这句在xcode8之前的版本上运行，是个恒成立的条件。。。条件编译的限制也就失去了本来的意义。。。
所以应该这么写就对了：
```
#if __IPHONE_OS_VERSION_MAX_ALLOWED >= 100000
  @interface ViewController () <CAAnimationDelegate>
  #else
  @interface ViewController ()
  #endif
```
这样做，就不会有问题了，将对应的版本号宏直接写成对应的数，但是这也存在一定的风险：就是`__IPHONE_10_0`这个宏的具体对应值是否会因为api的又一次升级而改变，这个很难说，至少之前都是比较稳定的，只是对应增加，而没有变化，但这毕竟是人家在维护的东西，**如果直接用对应的值，在每次系统升级时，做一下检查是十分有必要的！**
![](http://upload-images.jianshu.io/upload_images/1468630-1a3947683896f95b.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
__IPHONE_NA
所以前面提到，这样做适配，可能并不是一个好的办法，当然如果不嫌麻烦，反着来，多写几句条件编译的逻辑，就可以避免直接用上面的宏所对应的值，但是这意味着要考虑自己的工程兼容到的最低的系统版本了，写起来应该会很。。。
下面是与本文相关的一个简单的Demo，有兴趣的可以参考下：
> 
[Demo_Github](https://github.com/kukumaluCN/JXT_iOS_Demos)
![](http://upload-images.jianshu.io/upload_images/1468630-16bba83c14213123.gif?imageMogr2/auto-orient/strip)
Demo
如果你有更好的方式来解决这个适配的问题，欢迎在评论区留言讨论。
**参考文章：**
1.[IOS Assigning to 'id<Delegate>' from incompatible type...解决办法](http://blog.csdn.net/u010983974/article/details/51034940)
2.[iOS不同版本适配问题(#ifdef __IPHONE_7_0)](http://blog.csdn.net/xyxjn/article/details/40425895)
3.[Using SDK-Based Development](https://developer.apple.com/library/content/documentation/DeveloperTools/Conceptual/cross_development/Using/using.html#//apple_ref/doc/uid/20002000-SW5)
4.[iOS 【关于iOS/OS X 一些废弃API标识】](http://blog.csdn.net/felicity294250051/article/details/52144461)
文／霖溦（简书作者）
原文链接：http://www.jianshu.com/p/e9feb2c0c3f1
著作权归作者所有，转载请联系作者获得授权，并标注“简书作者”。
