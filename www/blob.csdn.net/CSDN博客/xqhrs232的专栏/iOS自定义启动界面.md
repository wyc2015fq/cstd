# iOS自定义启动界面 - xqhrs232的专栏 - CSDN博客
2019年03月12日 11:46:15[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：30
个人分类：[Mac OS/i OS技术](https://blog.csdn.net/xqhrs232/article/category/1252162)
原文地址::[https://www.jianshu.com/p/8625eb962c49](https://www.jianshu.com/p/8625eb962c49)
### 简介：
最近在重构app，原app用的是xcode自带的启动图设置。但相对来说自定义启动图可扩展性更强一点，今天花了一些时间弄了一下~
思路是自定义一个控制器，在viewDidLoad的方法中调用要设置的启动图的方法，方法中主要是创建一个UIImageView，对UIImageView的image进行定义，设置启动停留时间，启动完毕移除该控件并跳转到首页或者登录界面。
### 上代码：
**1、设置UIImageView以及image的属性**
![](https://upload-images.jianshu.io/upload_images/4098477-c0768578fc8e25f8.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1000/format/webp)
图 1
**2、获取指定图片资源**
![](https://upload-images.jianshu.io/upload_images/4098477-2df28a1839433f02.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1000/format/webp)
图 2
**解析：**先根据UIInterfaceOrientation获取设备的横竖屏状态，在苹果官方文档对UIInterfaceOrientation这样描述：The orientation of the app's user interface. 用户应用程序的方向。它的枚举有5种情况：如图3
![](https://upload-images.jianshu.io/upload_images/4098477-82bed01bc383345c.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/996/format/webp)
图 3
然后通过[NSBundle mainBundle] infoDictionary]获取资源包的信息，控制台打印：如图4
![](https://upload-images.jianshu.io/upload_images/4098477-a20af389a0022fef.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/928/format/webp)
图 4
从图片库中获取符合当前设备的图片，如图5
![](https://upload-images.jianshu.io/upload_images/4098477-5458e203096deec6.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/828/format/webp)
图 5
启动图就算完成了，需要注意的是选取图片尺寸要正确，否则会导致图片不能完全填充控制器的现象，对于动画结束后出现白屏的现象，可能和设置的动画、透明度或者控制器背景色有关~
谢谢～
作者：真的真心瓜子
链接：https://www.jianshu.com/p/8625eb962c49
来源：简书
简书著作权归作者所有，任何形式的转载都请联系作者获得授权并注明出处。
