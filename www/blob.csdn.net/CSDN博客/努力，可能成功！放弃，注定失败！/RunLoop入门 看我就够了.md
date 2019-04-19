# RunLoop入门 看我就够了 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2017年02月28日 19:32:10[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：491
# 本文来自简书，原文地址:[http://www.jianshu.com/p/2d3c8e084205](http://www.jianshu.com/p/2d3c8e084205)
# 前言
刚刚听到RunLoop的时候我也是一脸懵逼，这是什么，有什么用呢，逼格貌似还挺高。然后就开始尝试去搞懂它，去找博客，但是几乎所有的博客都是枯燥乏味的，都是讲概念，然后给个实例，对于我这个小白来说，根本看不懂好吗！！
下面放几个比较好的讲解RunLoop的博客
[http://www.cocoachina.com/ios/20150601/11970.html](http://www.cocoachina.com/ios/20150601/11970.html)
[http://www.jianshu.com/p/b9426458fcf6](http://www.jianshu.com/p/b9426458fcf6)
下面开始讲讲我对RunLoop认识。
# 正文
## 一、简介
首先，先象征性的讲下RunLoop的概念
从字面上看，就可以看出就是兜圈圈，就是一个死循环嘛。
## 二、作用
1.保持程序运行
2.处理app的各种事件（比如触摸，定时器等等）
3.节省CPU资源，提高性能。
## 三、枯燥知识
下面是关于RunLoop的一些使用简述。也许有点枯燥，但是也是必须要知道的！（敲黑板ing），我尽量说的通俗易懂一点。
### 1.两个API
首先要知道iOS里面有两套API可以访问和使用RunLoop：
#### Foundation
NSRunLoop
#### Core Foundation
CFRunLoopRef
上面两套都可以使用，但是要知道CFRunLoopRef是用c语言写的，是开源的，相比于NSRunLoop更加底层，而NSRunLoop其实是对CFRunLoopRef的一个简单的封装。便于使用而已。这样说来，显然CFRunLoopRef的性能要高一点。
### 2.RunLoop与线程
1.每条线程都有唯一的与之对应的RunLoop对象。
2.主线程的RunLoop已经创建好了，而子线程的需要手动创建。（也就是说子线程的RunLoop默认是关闭的，因为有时候开了个线程但却没有必要开一个RunLoop，不然反而浪费了资源。 ）
3.RunLoop在第一次获取时创建，在线程结束时销毁。（这就相当于 线程是一个类，RunLoop是类里的实例变量，这样便于理解）
### 3.获取RunLoop对象
#### Foundation
```
[NSRunLoop currentRunLoop]; // 获得当前线程的RunLoop对象
[NSRunLoop mainRunLoop]; // 获得主线程的RunLoop对象
```
#### Core Foundation
```
CFRunLoopGetCurrent(); // 获得当前线程的RunLoop对象
CFRunLoopGetMain(); // 获得主线程的RunLoop对象
```
### 4.RunLoop相关类
在Core Foundation中有RunLoop的五个类
```
CFRunLoopRef
    CFRunLoopModeRef
    CFRunLoopSourceRef
    CFRunLoopTimerRef
    CFRunLoopObserverRef
```
这五个类的关系如下
![](http://upload-images.jianshu.io/upload_images/2312304-bd6975ea7d411fb9.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
CFRunLoopModeRef.png
由图中可以得出以下几点：
1.CFRunLoopModeRef代表的是RunLoop的运行模式。
2.一个 RunLoop 包含若干个 Mode，每个 Mode 又包含若干个 Source/Timer/Observer。
3.每次调用 RunLoop 的主函数时，只能指定其中一个 Mode，这个Mode被称作 CurrentMode。
4.如果需要切换 Mode，只能退出 Loop，再重新指定一个 Mode 进入。这样做主要是为了分隔开不同组的 Source/Timer/Observer，让其互不影响。
#### CFRunLoopModeRef
系统默认注册了5个mode
```
kCFRunLoopDefaultMode //App的默认Mode，通常主线程是在这个Mode下运行
    UITrackingRunLoopMode //界面跟踪 Mode，用于 ScrollView 追踪触摸滑动，保证界面滑动时不受其他 Mode 影响
    UIInitializationRunLoopMode // 在刚启动 App 时第进入的第一个 Mode，启动完成后就不再使用
    GSEventReceiveRunLoopMode // 接受系统事件的内部 Mode，通常用不到
    kCFRunLoopCommonModes //这是一个占位用的Mode，不是一种真正的Mode
```
至于CFRunLoopModeRef的使用我会在 下面的实验三timer的使用中 详细说到。
#### CFRunLoopSourseRef
CFRunLoopSourseRef是事件源，分为两种
##### sourse0：非基于port的 （port相当于是系统）
自己写的方法，响应
##### sourse1：基于port的
系统提供的
#### CFRunLoopObserverRef
CFRunLoopObserver是观察者，可以监听runLoop的状态改变
监听的状态如下：
```
typedef CF_OPTIONS(CFOptionFlags, CFRunLoopActivity) { 
kCFRunLoopEntry = (1UL << 0), //即将进入Runloop
kCFRunLoopBeforeTimers = (1UL << 1), //即将处理NSTimer 
kCFRunLoopBeforeSources = (1UL << 2), //即将处理Sources 
kCFRunLoopBeforeWaiting = (1UL << 5), //即将进入休眠 
kCFRunLoopAfterWaiting = (1UL << 6), //刚从休眠中唤醒 
kCFRunLoopExit = (1UL << 7), //即将退出runloop 
kCFRunLoopAllActivities = 0x0FFFFFFFU //所有状态改变};
```
## 四、实验讲解
### 一、main函数的实验
先看几行我们很熟悉的代码
```
int main(int argc, char * argv[]) {
    @autoreleasepool {
        return UIApplicationMain(argc, argv, nil, NSStringFromClass([AppDelegate class]));
    }
}
```
这个是main函数里面的代码，当app打开后就进入这里。main函数返回类型是int，也就是返回的是
`UIApplicationMain(argc, argv, nil, NSStringFromClass([AppDelegate class]));`
不知道你有没有想过，如果返回的是0或者1等等，会有什么结果呢，我们来试试。
首先我们先在vc中加一个按钮，然后添加一个相应，如图所示：
![](http://upload-images.jianshu.io/upload_images/2312304-2bf3e436496a44d5.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
RunLoopTest1.png
可以想到跑起来的结果：
![](http://upload-images.jianshu.io/upload_images/2312304-14981c9b7d2c50e9.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
RunLoopTest2.png
注意看Xcode左上角的按钮
![](http://upload-images.jianshu.io/upload_images/2312304-08241325927686d5.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
RunLoopTest3.png
始终处于运行状态，只有当我们点击停止才会停止程序。但是当我们改变代码后
```
int main(int argc, char * argv[]) {
    @autoreleasepool {
        return 0;
    }
}
```
再次run起来，发现如下结果：
![](http://upload-images.jianshu.io/upload_images/2312304-8cac53b934be1697.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
RunLoopTest4.png
其实可以猜到这样的结果，因为程序运行到return 0;这一行后就退出了，AppDeleage里面的所有方法都没有进去。
再来做个试验：在main里面输入以下代码
```
int res = UIApplicationMain(argc, argv, nil, NSStringFromClass([AppDelegate class]));
NSLog(@"-----");
return res;
```
你猜会输出 “-----” 吗？答案是否定的，你会发现程序始终不会到NSLog(@"-----");这一行来。这就说明了程序一直在运行着。其实这都是RunLoop的功劳，它的其中一个功能就是保持程序的持续运行。有了RunLoop，main里面相当于是这样的代码（伪代码）:
```
BOOL running = YES;
do {
   // 执行各种操作
} while (running);
return 0;
```
程序是始终在while里面的，是一个死循环。
说到这里你肯定又会疑惑，RunLoop是什么时候创建的。其实在UIApplicationMain(argc, argv, nil, NSStringFromClass([AppDelegate class]))这个函数的内部就已经启动了一个RunLoop，所以函数一直没有返回，这才使得程序保持运行。
（注意：这个默认启动的RunLoop是和主线程相关的!!!）
### 二、尝试打印RunLoop对象
既然在理论区学会了如何获得RunLoop对象，那么我们就打印看看到底是什么玩意。
在按钮的响应区添加一行输出：
```
- (IBAction)ButtonDidClick:(id)sender {
    NSLog(@"ButtonDidClick");
    NSLog(@"----%@", [NSRunLoop currentRunLoop]);
}
```
结果你会得到下图的输出：
![](http://upload-images.jianshu.io/upload_images/2312304-5dd016d9e4bf580c.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
RunLoopTest5.png
这张截图只是输出的一部分，看不懂没关系，只要先看一下我圈出来的这几个名词，这在后面会有讲到。
### 三、NSTimer的使用
在项目中用的NSTimer其实也和RunLoop有关系，下面我们来做个实验
#### 实验一 scheduledTimer方法
修改一下button的响应以及timerTest方法，代码如下
```
- (IBAction)ButtonDidClick:(id)sender {
    [NSTimer scheduledTimerWithTimeInterval:1.0 target:self selector:@selector(timerTest) userInfo:nil repeats:YES];
}
- (void)timerTest
{
    NSLog(@"timerTest----");
}
```
点击button可以看到输出台每隔一秒钟就打印"timerTest----"。
#### 实验二 timerWithTime方法
代码如下：
```
- (IBAction)ButtonDidClick:(id)sender {
    NSTimer *timer = [NSTimer timerWithTimeInterval:1.0 target:self selector:@selector(timerTest) userInfo:nil repeats:YES];
}
- (void)timerTest
{
    NSLog(@"timerTest----");
}
```
但是实验结果是，点击button后没有反应。为什么呢？
噢~原来是少加了一句话，添加后的代码如下:
```
- (IBAction)ButtonDidClick:(id)sender {
    NSTimer *timer = [NSTimer timerWithTimeInterval:1.0 target:self selector:@selector(timerTest) userInfo:nil repeats:YES];
    [[NSRunLoop currentRunLoop] addTimer:timer forMode:NSDefaultRunLoopMode];
}
- (void)timerTest
{
    NSLog(@"timerTest----");
}
```
可是，为什么实验二比实验一要多加一句话呢？解：那是因为scheduledTimer方法会自动添加到当前的runloop里面去，而且runloop的运行模式kCFRunLoopDefaultMode，也就是说实验一已经将timer自动加入到了一个运行模式为kCFRunLoopDefaultMode的runloop中。
#### 实验三 有scrollView的情况下使用Timer
首先，按钮响应以及timerTest的方法如下：
```
- (IBAction)ButtonDidClick:(id)sender {
    NSTimer *timer = [NSTimer timerWithTimeInterval:1.0 target:self selector:@selector(timerTest) userInfo:nil repeats:YES];
    [[NSRunLoop currentRunLoop] addTimer:timer forMode:NSDefaultRunLoopMode];
}
- (void)timerTest
{
    NSLog(@"timerTest----");
}
```
然后在vc中加一个textView，run起来，模拟器界面如下：
![](http://upload-images.jianshu.io/upload_images/2312304-106d74ec349de49e.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
RunLoopTest6.png
然后点击按钮，随后滑动textView，可以看到打印结果：
![](http://upload-images.jianshu.io/upload_images/2312304-e55c5e033a5270dd.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
RunLoopTest7.png
没错，我在18：22：55秒钟滑动了textView，然后发现滑动的时候是不打印的，奇怪吧。其实说到底还是RunLoop搞的鬼。可以看到，我们把timer加到了NSDefaultRunLoopMode的runLoop中，而在滑动textview的时候，RunLoop就切换到UITrackingRunLoopMode模式，而上面有提到说：在每次调用 RunLoop 的主函数时，只能指定其中一个 Mode，这个Mode被称作 CurrentMode。 所以定时器就不起作用了。
现在可以思考一下解决方法了！（敲黑板ing）
提示一下，问题出在了模式上面，是不是修改一下模式就好了呢。
解决方法：
上面有提到过五个mode
```
kCFRunLoopDefaultMode //App的默认Mode，通常主线程是在这个Mode下运行
    UITrackingRunLoopMode //界面跟踪 Mode，用于 ScrollView 追踪触摸滑动，保证界面滑动时不受其他 Mode 影响
    UIInitializationRunLoopMode // 在刚启动 App 时第进入的第一个 Mode，启动完成后就不再使用
    GSEventReceiveRunLoopMode // 接受系统事件的内部 Mode，通常用不到
    kCFRunLoopCommonModes //这是一个占位用的Mode，不是一种真正的Mode
```
其实如果把mode改为kCFRunLoopCommonModes的话就可以既支持kCFRunLoopDefaultMode又支持UITrackingRunLoopMode了。
修改如下：
修改mode类型
```
- (IBAction)ButtonDidClick:(id)sender {
    NSTimer *timer = [NSTimer timerWithTimeInterval:1.0 target:self selector:@selector(timerTest) userInfo:nil repeats:YES];
    [[NSRunLoop currentRunLoop] addTimer:timer forMode:NSRunLoopCommonModes];
}
```
然后run发现就算滚动textView也不会影响打印。有图有真相：
![](http://upload-images.jianshu.io/upload_images/2312304-31a2ee3ec4191631.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
RunLoopTest8.png
### 四、有关CFRunLoopSourseRef的实验
我们在button的响应注释，然后打个断点，run后点击button会发现如下：
![](http://upload-images.jianshu.io/upload_images/2312304-e8c52239980ff6bf.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
RunLoopTest9.png
![](http://upload-images.jianshu.io/upload_images/2312304-132d34a65523ab42.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
RunLoopTest10.png
说明了button的点击是属于sourse0的。
### 五、有关CFRunLoopObserverRef的实验
首先回顾CFRunLoopObserverRef，是RunLoop的监听者，监听的状态如下：
```
typedef CF_OPTIONS(CFOptionFlags, CFRunLoopActivity) { 
kCFRunLoopEntry = (1UL << 0), //即将进入Runloop
kCFRunLoopBeforeTimers = (1UL << 1), //即将处理NSTimer 
kCFRunLoopBeforeSources = (1UL << 2), //即将处理Sources 
kCFRunLoopBeforeWaiting = (1UL << 5), //即将进入休眠 
kCFRunLoopAfterWaiting = (1UL << 6), //刚从休眠中唤醒 
kCFRunLoopExit = (1UL << 7), //即将退出runloop 
kCFRunLoopAllActivities = 0x0FFFFFFFU //所有状态改变};
```
CF里面添加监听者的方法为
`CFRunLoopAddObserver(<#CFRunLoopRef rl#>, <#CFRunLoopObserverRef observer#>, <#CFStringRef mode#>)`
第一个参数：RunLoop
第二个参数：observer
第三个参数：mode
方法介绍完，开始敲代码，vc代码如下
```
- (void)viewDidLoad {
    [super viewDidLoad];
    [self createObserver];
}
- (void)createObserver
{
    CFRunLoopObserverRef observer = CFRunLoopObserverCreateWithHandler(CFAllocatorGetDefault(), kCFRunLoopAllActivities, YES, 0, ^(CFRunLoopObserverRef observer, CFRunLoopActivity activity) {
        NSLog(@"--------%zd", activity);
    });
    CFRunLoopAddObserver(CFRunLoopGetCurrent(), observer, kCFRunLoopDefaultMode);  // 添加监听者，关键！
    CFRelease(observer); // 释放
}
```
run！
可以看到打印结果：
![](http://upload-images.jianshu.io/upload_images/2312304-f9b514fd75875265.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
RunLoopTest11.png
这就是监听的作用，可以知道当前的RunLoop状态。
下图是RunLoop的内部逻辑已经文字描述：（图片来自网络）
![](http://upload-images.jianshu.io/upload_images/2312304-8e64b79a92d53567.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
RunLoop内部逻辑.png
![](http://upload-images.jianshu.io/upload_images/2312304-8dbf27d0666fd544.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
RunLoop的逻辑表述.png
# 五、最后
其实上述只是runloop的一个入门而已，想要深入其中，还是需要在项目中尝试使用它。
最后的最后，这篇文章的发表经历了整整三天的时间，总结，实验，截图，码字。。。总之，如果你觉得对你有所帮助，就请给我点个赞呗~
最后的最后的最后，我的水平有限，肯定有很多错误和不足之处，请各位不吝赐教，谢谢。
# 六、更新
也许你会觉得RunLoop的五个相关类会有点乱，下面我再来理一下。
```
CFRunLoopRef
    CFRunLoopModeRef
    CFRunLoopSourceRef
    CFRunLoopTimerRef
    CFRunLoopObserverRef
```
我的理解是，CFRunLoopRef就是RunLoop，而SourceRef、TimerRef、ObserverRef是CFRunLoopRef的内容，而ModeRef指的是mode的属性。
先放个代码：
`[[NSRunLoop currentRunLoop] addPort:[NSPort port] forMode:NSDefaultRunLoopMode];`
这句话的意思就是：给当前的RunLoop添加一个内容为Sourse的而且属性是NSDefaultRunLoopMode的mode。是不是一下子清楚了~如果还不懂，那就再举个栗子：
```
NSTimer *timer = [NSTimer timerWithTimeInterval:1.0 target:self selector:@selector(timerTest) userInfo:nil repeats:YES];
[[NSRunLoop currentRunLoop] addTimer:timer forMode:NSRunLoopCommonModes];
```
这句话的意思是：给当前的RunLoop添加一个内容为timer的而且属性是NSRunLoopCommonModes的mode。
现在是不是懂了呢~
看完入门，看实战呗~
传送门，RunLoop已入门？不来应用一下？：[http://www.jianshu.com/p/c0a550d2ac97](http://www.jianshu.com/p/c0a550d2ac97)
