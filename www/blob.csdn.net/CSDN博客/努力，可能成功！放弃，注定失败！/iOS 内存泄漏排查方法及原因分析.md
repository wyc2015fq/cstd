# iOS 内存泄漏排查方法及原因分析 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2018年11月22日 09:03:18[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：204
> 
级别： ★★☆☆☆
标签：「iOS」「内存泄漏排查」「Leaks工具」
作者： [MrLiuQ](https://www.jianshu.com/u/6663b66c3df3)
审校： [QiShare团队](https://www.jianshu.com/c/b3bd94559163)
本文将从以下两个层面解决**iOS内存泄漏**问题：
- 内存泄漏排查方法（工具）
- 内存泄漏原因分析（解决方案）
> 
在正式开始前，我们先区分两个基本概念：
- [内存泄漏（memory leak）](https://baike.so.com/doc/6084712-6297814.html)：是指申请的内存空间使用完毕之后**未回收**。
	一次内存泄露危害可以忽略，但若一直泄漏，无论有多少内存，迟早都会被占用光，最终导致程序`crash`。（因此，开发中我们要尽量避免内存泄漏的出现）
- [内存溢出（out of memory）](https://baike.so.com/doc/6467899-6681594.html)：是指程序在申请内存时，没有足够的内存空间供其使用。
	通俗理解就是内存不够用了，通常在运行大型应用或游戏时，应用或游戏所需要的内存远远超出了你主机内安装的内存所承受大小，就叫内存溢出。最终导致机器`重启`或者程序`crash`。
简单来说：
|概念|区别说明|
|----|----|
|内存泄漏|供应方（操作系统）能提供给需求方（App）的内存越来越少。|
|内存溢出|需求方（App）需要的内存过大，超过供应方（操作系统）负载。|
### 一、排查方法
> 
我们知道，iOS开发有“ARC机制”帮忙管理内存，但在实际开发中，如果处理不好堆空间上的内存还是会存在内存泄漏的问题。如果内存泄漏严重，最终会导致程序的崩溃。
首先，我们需要检查我们的App有没有内存泄漏，并且快速定位到内存泄漏的代码。目前比较常用的内存泄漏的排查方法有两种，都在Xcode中可以直接使用：
- 第一种：静态分析方法（`Analyze`）
- 第二种：动态分析方法（`Instrument`工具库里的`Leaks`）。一般推荐使用第二种。
1.1 静态内存泄漏分析方法：
- 
第一步：通过Xcode打开项目，然后点击Product->Analyze，开始进入静态内存泄漏分析。
	如下图所示：
	
![](https://upload-images.jianshu.io/upload_images/3407530-951953f010ce33bc.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1000/format/webp)
静态内存泄漏分析
- 
第二步：等待分析结果。
- 
第三步：根据分析的结果对可能造成内存泄漏的代码进行排查，如下图所示。
![](https://upload-images.jianshu.io/upload_images/3407530-a3fb3499c0cbd4a2.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1000/format/webp)
静态内存泄漏分析结果
> 
PS：静态内存泄漏分析能发现大部分问题，但只是静态分析，并且并不准确，只是有可能发生内存泄漏。一些动态内存分配的情形并没有分析。如果需要更精准一些，那就要用到下面要介绍的动态内存泄漏分析方法（Instruments工具中的`Leaks`方法）进行排查。
1.2 动态内存泄漏分析方法：
> 
静态内存泄漏分析不能把所有的内存泄漏排查出来，因为有的内存泄漏发生在运行时，当用户做某些操作时才发生内存泄漏。这是就要使用动态内存泄漏检测方法了。
步骤如下：
- 第一步：通过Xcode打开项目，然后点击Product->Profile，如下图所示：
![](https://upload-images.jianshu.io/upload_images/3407530-30db54850f263ae1.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/190/format/webp)
动态内存泄漏分析-1
- 第二步：按上面操作，build成功后跳出Instruments工具，如上图右侧图所示。选择`Leaks`选项，点击右下角的【choose】按钮。如下图：
![](https://upload-images.jianshu.io/upload_images/3407530-5185d6af1dcd2d93.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1000/format/webp)
动态内存泄漏分析-2
- 第三步：这时候项目程序也在模拟器或手机上运行起来了，在手机或模拟器上对程序进行操作，工具显示效果如下：
![](https://upload-images.jianshu.io/upload_images/3407530-d3cc45afe9874bb6.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1000/format/webp)
Leaks使用
点击左上角的红色圆点，这时项目开始启动了，由于`Leaks`是动态监测，所以手动进行一系列操作，可检查项目中是否存在内存泄漏问题。如图所示，橙色矩形框中所示绿色为正常，如果出现如右侧红色矩形框中显示红色，则表示出现内存泄漏。
![](https://upload-images.jianshu.io/upload_images/3407530-2a59e90cb1d772cd..png?imageMogr2/auto-orient/strip%7CimageView2/2/w/870/format/webp)
选中Leaks Checks,在Details所在栏中选择CallTree,并且在右下角勾选`Invert Call Tree` 和`Hide System Libraries`，会发现显示若干行代码，双击即可跳转到出现内存泄漏的地方，修改即可。
举个例子：
![](https://upload-images.jianshu.io/upload_images/3407530-98383cd9abf26ea4.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1000/format/webp)
> 
PS：AFHTTPSessionManager内存泄漏是一个很常见的问题：解决方法有两种：[点击这里](https://www.jianshu.com/p/3848b038808b)
### 二、内存泄漏的原因分析
目前，在ARC环境下，导致内存泄漏的根本原因是代码中**存在循环引用**，从而导致一些内存无法释放，最终导致dealloc()方法无法被调用。主要原因大概有一下几种类型：
2.1 ViewController中存在NSTimer
如果你的ViewController中有NSTimer，那么你就要注意了，因为当你调用
```
[NSTimer scheduledTimerWithTimeInterval:1.0 
                                 target:self 
                               selector:@selector(updateTime:) 
                               userInfo:nil 
                                repeats:YES];
```
- 理由：这时 `target: self`，增加了ViewController的`retain count`，
	即`self`强引用`timer`，`timer`强引用`self`。造成循环引用。
- 解决方案：在恰当时机调用`[timer invalidate]`即可。
2.2 ViewController中的代理delegate
> 
代理在一般情况下，需要使用weak修饰。如果你这个VC需要外部传某个delegate进来，通过delegate+protocol的方式传参数给其他对象，那么这个delegate一定不要强引用，尽量使用weak修饰，否则你的VC会持续持有这个delegate，直到代理自身被释放。
- 理由：如果代理用`strong`修饰，ViewController（`self`）会强引用`View`，`View`强引用`delegate`，`delegate`内部强引用ViewController（`self`）。造成内存泄漏。
- 解决方案：尽量代理使用`weak`修饰。
举个例子：一般代理用`weak`修饰，避免循环引用。
```
@class QiAnimationButton;
@protocol QiAnimationButtonDelegate <NSObject>
@optional
- (void)animationButton:(QiAnimationButton *)button willStartAnimationWithCircleView:(QiCircleAnimationView *)circleView;
- (void)animationButton:(QiAnimationButton *)button didStartAnimationWithCircleView:(QiCircleAnimationView *)circleView;
- (void)animationButton:(QiAnimationButton *)button willStopAnimationWithCircleView:(QiCircleAnimationView *)circleView;
- (void)animationButton:(QiAnimationButton *)button didStopAnimationWithCircleView:(QiCircleAnimationView *)circleView;
- (void)animationButton:(QiAnimationButton *)button didRevisedAnimationWithCircleView:(QiCircleAnimationView *)circleView;
@end
@interface QiAnimationButton : UIButton
@property (nonatomic, weak) id <QiAnimationButtonDelegate> delegate;
- (void)startAnimation; //!< 开始动画
- (void)stopAnimation; //!< 结束动画
- (void)reverseAnimation; //!< 最后的修改动画
```
2.3 ViewController中块
> 
在我们日常开发中，如果块使用不当，很容易导致内存泄漏。
- 理由：如果`block`被当前ViewController（`self`）持有，这时，如果block内部再持有ViewController（`self`），就会造成循环引用。
- 解决方案：在`block`外部对**弱化**`self`，再在块内部**强化**已经弱化的`weakSelf`
例如：
```
__weak typeof(self) weakSelf = self;
    [self.operationQueue addOperationWithBlock:^{
        __strong typeof(weakSelf) strongSelf = weakSelf;
        if (completionHandler) {
            
            KTVHCLogDataStorage(@"serial reader async end, %@", request.URLString);
            
            completionHandler([strongSelf serialReaderWithRequest:request]);
        }
    }];
```
> 
关注我们的途径有：
[QiShare（简书）](https://www.jianshu.com/u/3db23baa08c7)
[QiShare（掘金）](https://juejin.im/user/5b542b76e51d4533d2042b62)
[QiShare（知乎）](https://www.zhihu.com/people/edit)
[QiShare（GitHub）](https://github.com/QiShare)
[QiShare（CocoaChina）](http://www.cocoachina.com/bbs/u.php?tid=658244)
[QiShare（StackOverflow）](https://stackoverflow.com/users/10118400/qishare)
QiShare（微信公众号）
> 
推荐文章：
[Web安全漏洞之CSRF](https://zhuanlan.zhihu.com/p/44877542?utm_source=wechat_session&utm_medium=social&from=timeline&isappinstalled=0)
[2018苹果秋季新品发布会速览](https://www.jianshu.com/p/4f0a45654b37)
作者：QiShare 
链接：HTTPS：//www.jianshu.com/p/c9fd287ec7ab 
来源：书繁简
繁简书著作权归作者所有，任何形式的转载都请联系作者获得授权并注明出处。
