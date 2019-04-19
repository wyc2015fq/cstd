# iOS-UIGestureRecognizer详解-原理篇 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2018年07月30日 16:41:57[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：509
## 前言
本文主要内容如下：
1. UIGestureRecognizer 属性、方法、代理和七个子类详解。
2. 讲讲 UIGestureRecognizer 和 UITouch 事件的关系。
3. 讲讲如何自定义手势？
## 一、手势识别器-UIGestureRecognizer
### 1.1 简介
UIGestureRecognizer是苹果在iOS 3.2之后，推出的手势识别功能。UIGestureRecognizer是一个抽象类，将触摸事件封装成了手势对象，大大简化了开发者的开发难度，同时也提升了用户的交互体验。UIGestureRecognizer有七个子类，它们具体实现了不同手势的功能。
![](https://upload-images.jianshu.io/upload_images/2955889-408606e8a20074c8.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/700)
手势结构关系图.png
### 1.2 属性、方法、代理
UIGestureRecognizer 是一个抽象类，所以它会提供很多共有的属性和方法给子类用，这也是抽象父类的作用。
1.2.1 初始化、添加target、移除target
```
//初始化方法 且 添加 target的方法
- (instancetype)initWithTarget:(nullable id)target action:(nullable SEL)action
//单独添加target的方法
- (void)addTarget:(id)target action:(SEL)action;
//移除target的方法
- (void)removeTarget:(nullable id)target action:(nullable SEL)action;
```
addTarget方法，允许**一个手势对象**可以**添加多个selector方法**，并且触发的时候，所有添加的selector都会被执行，我们以点击手势示例如下：
```
- (void)addTapGesture
{
    UITapGestureRecognizer *tap = [[UITapGestureRecognizer alloc] initWithTarget:self action:@selector(tapHandler:)];
    [tap addTarget:self action:@selector(tap1Handler:)];
    [self.view addGestureRecognizer:tap];
}
- (void)tapHandler:(UITapGestureRecognizer *)sender
{
    NSLog(@"tapHandler 点击了。。。");
}
- (void)tap1Handler:(UITapGestureRecognizer *)sender
{
    NSLog(@"tapHandler1 点击了。。。");
}
点击屏幕，打印内容如下：
2018-07-26 00:46:10.161513+0800 UIGestureRecognizerDemo[4004:479521] tapHandler 点击了。。。
2018-07-26 00:46:10.162740+0800 UIGestureRecognizerDemo[4004:479521] tapHandler1 点击了。。。
```
1.2.2 属性和方法
先把所有的属性和方法列举出来说说作用的，有的属性是很常用的，就不展开说了，有的属性不常用，但是比较重要，我就单独拿出来详细说一下。
```
//手势的状态
@property(nonatomic,readonly) UIGestureRecognizerState state;  
//手势代理
@property(nullable,nonatomic,weak) id <UIGestureRecognizerDelegate> delegate;
//手势是否有效  默认YES
@property(nonatomic, getter=isEnabled) BOOL enabled; 
//获取手势所在的view
@property(nullable, nonatomic,readonly) UIView *view; 
//取消view上面的touch事件响应  default  YES **下面会详解该属性**
@property(nonatomic) BOOL cancelsTouchesInView;       
//延迟touch事件开始 default  NO   **下面会详解该属性**
@property(nonatomic) BOOL delaysTouchesBegan;
//延迟touch事件结束 default  YES  **下面会详解该属性**
@property(nonatomic) BOOL delaysTouchesEnded;
//允许touch的类型数组，**下面会详解该属性**
@property(nonatomic, copy) NSArray<NSNumber *> *allowedTouchTypes 
//允许按压press的类型数组
@property(nonatomic, copy) NSArray<NSNumber *> *allowedPressTypes 
//是否只允许一种touchType 类型，**下面会详解该属性**
@property (nonatomic) BOOL requiresExclusiveTouchType 
//手势依赖（手势互斥）方法，**下面会详解该方法**
- (void)requireGestureRecognizerToFail:(UIGestureRecognizer *)otherGestureRecognizer;
//获取在传入view的点击位置的信息方法
- (CGPoint)locationInView:(nullable UIView*)view;                         
//获取触摸点数
@property(nonatomic, readonly) NSUInteger numberOfTouches;    
 //（touchIndex 是第几个触摸点）用来获取多触摸点在view上位置信息的方法                                     
- (CGPoint)locationOfTouch:(NSUInteger)touchIndex inView:(nullable UIView*)view; 
// 给手势加一个名字，以方便调式（iOS11 or later可以用）
@property (nullable, nonatomic, copy) NSString *name API_AVAILABLE(ios(11.0)
```
先来说说**requiresExclusiveTouchType**这个属性
是不是有很多人和我之前一样，把它理解成了设置为NO，就可以同时响应几种手势点击了呢？
这个属性的意思：**是否同时只接受一种触摸类型，而不是是否同时只接受一种手势**。默认是YES。设置成NO，它会同时响应 **allowedTouchTypes** 这个数组里的所有触摸类型。这个数组里面装的touchType类型如下：
```
//目前touchType有三种
typedef NS_ENUM(NSInteger, UITouchType) {
    UITouchTypeDirect,                       // 手指直接接触屏幕
    UITouchTypeIndirect,                     // 不是手指直接接触屏幕（例如：苹果TV遥控设置屏幕上的按钮）
    UITouchTypeStylus NS_AVAILABLE_IOS(9_1), // 触控笔接触屏幕
}
```
如果把**requiresExclusiveTouchType**设置为NO，假设view上添加了tapGesture手势，你同时用手点击和用触控笔点击该view，这个tapGesture手势的方法都会响应。
接下来说说**cancelsTouchesInView**、**delaysTouchesBegan**、**delaysTouchesEnd**这三个属性。
> - ***cancelsTouchesInView*** 属性默认设置为YES，如果识别到了手势，系统将会发送touchesCancelled:withEvent:消息，终止触摸事件的传递。也就是说默认当识别到手势时，touch事件传递的方法将被终止，如果设置为NO，touch事件传递的方法仍然会被执行。
> - ***delaysTouchesBegan*** 用于控制事件的开始响应的时机，"是否延迟响应触摸事件"。设置为NO，不会延迟响应触摸事件，如果我们设置为YES，在手势没有被识别失败前，都不会给事件传递链发送消息。
> - ***delaysTouchesEnd*** 用于控制事件结束响应的时机，"是否延迟结束触摸事件"，设置为NO，则会立马调用touchEnd:withEvent这个方法（如果需要调用的话）。设置为YES，会等待一个很短的时间，如果没有接收到新的手势识别任务，才会发送touchesEnded消息到事件传递链。
举栗子
**cancelsTouchesInView栗子**
```
- (void)addPanGesture
{
    UIPanGestureRecognizer *pan = [[UIPanGestureRecognizer alloc] initWithTarget:self action:@selector(panHandler:)];
    pan.cancelsTouchesInView = YES;
    [self.view addGestureRecognizer:pan];
}
- (void)panHandler:(UIPanGestureRecognizer *)sender
{
    NSLog(@"panHandler 调用了");
}
//tap.cancelsTouchesInView = YES; 控制台输出如下：
2018-07-26 15:31:13.034236+0800 GestureDemo[82008:1643784] touchesMoved调用了
2018-07-26 15:31:13.042147+0800 GestureDemo[82008:1643784] touchesMoved调用了
2018-07-26 15:31:13.042685+0800 GestureDemo[82008:1643784] touchesMoved调用了
2018-07-26 15:31:13.051290+0800 GestureDemo[82008:1643784] touchesMoved调用了
2018-07-26 15:31:13.051290+0800 GestureDemo[82008:1643784] touchesCancel调用了
2018-07-26 15:31:13.082702+0800 GestureDemo[82008:1643784] panHandler 调用了
2018-07-26 15:31:13.083552+0800 GestureDemo[82008:1643784] panHandler 调用了
2018-07-26 15:31:13.083918+0800 GestureDemo[82008:1643784] panHandler 调用了
2018-07-26 15:31:13.090601+0800 GestureDemo[82008:1643784] panHandler 调用了
2018-07-26 15:31:13.098323+0800 GestureDemo[82008:1643784] panHandler 调用了
//pan.cancelsTouchesView = NO;控制台输出如下：
2018-07-26 15:38:00.895361+0800 GestureDemo[82069:1649256] touchesMoved调用了
2018-07-26 15:38:00.903074+0800 GestureDemo[82069:1649256] panHandler 调用了
2018-07-26 15:38:00.903316+0800 GestureDemo[82069:1649256] touchesMoved调用了
2018-07-26 15:38:00.903696+0800 GestureDemo[82069:1649256] panHandler 调用了
2018-07-26 15:38:00.903962+0800 GestureDemo[82069:1649256] touchesMoved调用了
2018-07-26 15:38:00.911393+0800 GestureDemo[82069:1649256] panHandler 调用了
```
栗子中，pan.cancelsTouchesInView = YES时，为什么会打印"touchesMoved调用了"呢？这就涉及到第二个属性`delaysTouchesBegan`**,这是因为手势识别是有一个过程的，拖拽手势需要一个很小的手指移动的过程才能被识别为拖拽手势，而在一个手势触发之前，是会一并发消息给事件传递链的，所以才会有最开始的几个touchMoved方法被调用，当识别出拖拽手势以后，就会终止touch事件的传递。** 当pan.cancelsTouchsInView = NO，touchesMoved和panHandler依次被打印出来，touch事件继续响应。
**delaysTouchesBegan的栗子**
```
- (void)addPanGesture
{
    UIPanGestureRecognizer *pan = [[UIPanGestureRecognizer alloc] initWithTarget:self action:@selector(panHandler:)];
    pan.cancelsTouchesInView = YES;
    pan.delaysTouchesBegan = YES;
    [self.view addGestureRecognizer:pan];
}
- (void)panHandler:(UIPanGestureRecognizer *)sender
{
    NSLog(@"panHandler 调用了");
}
//pan.delaysTouchesBegan = YES;  控制台输出如下：
2018-07-26 16:06:59.682302+0800 GestureDemo[82294:1669777] panHandler 调用了
2018-07-26 16:06:59.689734+0800 GestureDemo[82294:1669777] panHandler 调用了
2018-07-26 16:06:59.689973+0800 GestureDemo[82294:1669777] panHandler 调用了
2018-07-26 16:06:59.697302+0800 GestureDemo[82294:1669777] panHandler 调用了
2018-07-26 16:06:59.697675+0800 GestureDemo[82294:1669777] panHandler 调用了
```
当**delaysTouchesBegan** 设置为YES时，手势识别成功之前都不会调用touches相关方法，因为手势识别成功了，所以控制台只打印了"panHandler 调用了"的信息。如果手势识别失败了，就会打印touchesMoved方法里的信息。
**delaysTouchesEnd的栗子**
```
- (void)addTapGesture
{
    UITapGestureRecognizer *tap = [[UITapGestureRecognizer alloc] initWithTarget:self action:@selector(tapHandler:)];
    tap.numberOfTapsRequired = 3;
    tap.delaysTouchesEnded = YES;
    [self.view addGestureRecognizer:tap];
}
- (void)tapHandler:(UITapGestureRecognizer *)sender
{
    NSLog(@"tapHandler 点击了");
}
// tap.delaysTouchesEnded = YES 时，控制台输出如下：
2018-07-26 16:58:05.101085+0800 GestureDemo[88344:1715678] touchesBegan调用了
2018-07-26 16:58:05.614449+0800 GestureDemo[88344:1715678] tapHandler 点击了
2018-07-26 16:58:05.614961+0800 GestureDemo[88344:1715678] touchesCancel调用了
//tap.delaysTouchesEnded = NO 时，控制台输出如下：
2018-07-26 16:48:15.722280+0800 GestureDemo[88254:1708453] touchesBegan调用了
2018-07-26 16:48:15.815430+0800 GestureDemo[88254:1708453] touchesEnded调用了
2018-07-26 16:48:15.896287+0800 GestureDemo[88254:1708453] touchesBegan调用了
2018-07-26 16:48:15.984245+0800 GestureDemo[88254:1708453] touchesEnded调用了
2018-07-26 16:48:16.057009+0800 GestureDemo[88254:1708453] touchesBegan调用了
2018-07-26 16:48:16.154256+0800 GestureDemo[88254:1708453] tapHandler 点击了
2018-07-26 16:48:16.154643+0800 GestureDemo[88254:1708453] touchesCancel调用了
```
就像上面对这个属性的分析一样 设置为NO，则会立马调用touchEnd:withEvent这个方法。设置为YES，会等待一个很短的时间，如果没有接收到新的手势识别任务，才会发送touchesEnded消息到事件传递链。
手势依赖方法-requireGestureRecognizerToFail
**用法：[A requireGestureRecognizerToFail：B] 当A、B两个手势同时满足响应手势方法的条件时，B优先响应，A不响应。如果B不满足条件，A满足响应手势方法的条件，则A响应。其实这就是一个设置响应手势优先级的方法。**
如果一个view上添加了多个手势对象的，默认这些手势是互斥的，一个手势触发了就会默认屏蔽其他手势动作。比如，单击和双击手势并存时，如果不做处理，它就只能发送出单击的消息。为了能够优先识别双击手势，我们就可以用**requireGestureRecognizerToFail：**这个方法设置优先响应双击手势。
1.2.3 UIGestureRecognizerDelegate代理方法
```
//开始进行手势识别时调用的方法，返回NO，则手势识别失败
- (BOOL)gestureRecognizerShouldBegin:(UIGestureRecognizer *)gestureRecognizer;
//手指触摸屏幕后回调的方法，返回NO则手势识别失败
- (BOOL)gestureRecognizer:(UIGestureRecognizer *)gestureRecognizer 
shouldReceiveTouch:(UITouch *)touch;
//是否支持同时多个手势触发
//返回YES，则可以多个手势一起触发方法，返回NO则为互斥
- (BOOL)gestureRecognizer:(UIGestureRecognizer *)gestureRecognizer 
shouldRecognizeSimultaneouslyWithGestureRecognizer:(UIGestureRecognizer *)
otherGestureRecognizer;
//下面这个两个方法也是用来控制手势的互斥执行的
//这个方法返回YES，第二个手势的优先级高于第一个手势
- (BOOL)gestureRecognizer:(UIGestureRecognizer *)gestureRecognizer 
shouldRequireFailureOfGestureRecognizer:(UIGestureRecognizer *)
otherGestureRecognizer 
//这个方法返回YES，第一个手势的优先级高于第二个手势
- (BOOL)gestureRecognizer:(UIGestureRecognizer *)gestureRecognizer 
shouldBeRequiredToFailByGestureRecognizer:(UIGestureRecognizer *)
otherGestureRecognizer
```
### 1.3 UIGestureRecognizer 子类
手势可以分为：**"离散手势"**和**"连续手势"**
**"离散手势"**：比如tapGesture、swipeGesture等
**"连续手势"**: 比如：panGesture,rotationGesture等。
对于连续手势，手势识别器可能使状态转换更多，如下图所示：
可能---->开始----> [已更改] ---->已取消
可能---->开始----> [已更改] ---->结束
```
//手势状态枚举值
typedef NS_ENUM(NSInteger, UIGestureRecognizerState) {
    UIGestureRecognizerStatePossible,   // 默认的状态，这个时候的手势并没有具体的情形状态
    UIGestureRecognizerStateBegan,      // 手势开始被识别的状态
    UIGestureRecognizerStateChanged,    // 手势识别发生改变的状态
    UIGestureRecognizerStateEnded,      // 手势识别结束，将会执行触发的方法
    UIGestureRecognizerStateCancelled,  // 手势识别取消
    UIGestureRecognizerStateFailed,     // 识别失败，方法将不会被调用
    UIGestureRecognizerStateRecognized = UIGestureRecognizerStateEnded 
};
```
1.3.1 点击手势——UITapGestureRecognizer
在视图上点击视图的手势------常用度五颗星
```
//设置点击次数，默认为单击
@property (nonatomic) NSUInteger  numberOfTapsRequired; 
//设置同时点击的手指数
@property (nonatomic) NSUInteger  numberOfTouchesRequired;
```
1.3.2 捏合手势——UIPinchGestureRecognizer
在视图上手指进行缩放的手势------常用度三颗星
```
//设置缩放比例
@property (nonatomic)          CGFloat scale; 
//设置捏合速度,只读
@property (nonatomic,readonly) CGFloat velocity;
```
1.3.3 旋转手势——UIRotationGestureRecognizer
在视图上手指旋转的手势------常用度三颗星
```
//设置旋转角度
@property (nonatomic)          CGFloat rotation;
//设置旋转速度 
@property (nonatomic,readonly) CGFloat velocity;
```
1.3.4 滑动手势——UISwipeGestureRecognizer
在视图上用手指进行有方向滑动的手势------常用度三颗星
```
/设置触发滑动手势的触摸点数
@property(nonatomic) NSUInteger                        numberOfTouchesRequired; 
//设置滑动方向
@property(nonatomic) UISwipeGestureRecognizerDirection direction;  
//枚举如下
typedef NS_OPTIONS(NSUInteger, UISwipeGestureRecognizerDirection) {
    UISwipeGestureRecognizerDirectionRight = 1 << 0,
    UISwipeGestureRecognizerDirectionLeft  = 1 << 1,
    UISwipeGestureRecognizerDirectionUp    = 1 << 2,
    UISwipeGestureRecognizerDirectionDown  = 1 << 3
};
```
1.3.5 长按手势——UILongPressGestureRecognizer
在视图上用手指进行长按的手势------常用度三颗星
```
//设置触发前的点击次数
@property (nonatomic) NSUInteger numberOfTapsRequired;    
//设置触发的触摸点数
@property (nonatomic) NSUInteger numberOfTouchesRequired; 
//设置最短的长按时间
@property (nonatomic) CFTimeInterval minimumPressDuration; 
//设置在按触时时允许移动的最大距离 默认为10像素
@property (nonatomic) CGFloat allowableMovement;
```
1.3.6 平移手势——UIPanGestureRecognzer
在视图上用手指进行平移的手势------常用度四颗星
```
//设置触发拖拽的最少触摸点，默认为1
@property (nonatomic)          NSUInteger minimumNumberOfTouches; 
//设置触发拖拽的最多触摸点
@property (nonatomic)          NSUInteger maximumNumberOfTouches;  
//获取当前位置
- (CGPoint)translationInView:(nullable UIView *)view;            
//设置当前位置
- (void)setTranslation:(CGPoint)translation inView:(nullable UIView *)view;
//设置获取平移速度
- (CGPoint)velocityInView:(nullable UIView *)view;
```
1.3.7 屏幕边缘平移手势——UIScreenEdgePanGestureRecognzer
手指在屏幕四个边缘平移的手势------常用度三颗星
```
//设置在屏幕哪个边缘触发手势
@property (readwrite, nonatomic, assign) UIRectEdge edges; 
typedef NS_OPTIONS(NSUInteger, UIRectEdge) {
    UIRectEdgeNone   = 0,
    UIRectEdgeTop    = 1 << 0,
    UIRectEdgeLeft   = 1 << 1,
    UIRectEdgeBottom = 1 << 2,
    UIRectEdgeRight  = 1 << 3,
    UIRectEdgeAll    = UIRectEdgeTop | UIRectEdgeLeft | UIRectEdgeBottom | UIRectEdgeRight
} NS_ENUM_AVAILABLE_IOS(7_0);
```
## 二、UIGestureRecognizer 和 UITouch 事件的关系
**从runLoop底层看事件响应和手势的关系：**
> 
事件响应
苹果使用RunLoop注册了一个 Source1 (基于 mach port 的) 用来接收系统事件，其回调函数为 __IOHIDEventSystemClientQueueCallback()。
当一个硬件事件(触摸/锁屏/摇晃等)发生后，首先由 IOKit.framework 生成一个
IOHIDEvent 事件并由 SpringBoard 接收。这个过程的详细情况可以参考[这里](http://iphonedevwiki.net/index.php/IOHIDFamily)。SpringBoard 只接收按键(锁屏/静音等)，触摸，加速，接近传感器等几种 Event，随后用 mach port 转发给需要的App进程。随后苹果注册的那个 Source1 就会触发回调，并调用 _UIApplicationHandleEventQueue() 进> 行应用内部的分发。
_UIApplicationHandleEventQueue() 会把 IOHIDEvent 处理并包装成 UIEvent 进行处理或> 分发，其中包括识别 UIGesture/处理屏幕旋转/发送给 UIWindow 等。通常事件比如 >UIButton 点击、touchesBegin/Move/End/Cancel 事件都是在这个回调中完成的。
手势识别
当上面的 _UIApplicationHandleEventQueue() 识别了一个手势时，其首先会调用 Cancel > 将当前的 touchesBegin/Move/End 系列回调打断。随后系统将对应的 UIGestureRecognizer 标记为待处理。
苹果注册了一个 Observer 监测 BeforeWaiting (Loop即将进入休眠) 事件，这个Observer的回调函数是 _UIGestureRecognizerUpdateObserver()，其内部会获取所有刚被标记为待> 处理的 GestureRecognizer，并执行GestureRecognizer的回调。
当有 UIGestureRecognizer 的变化(创建/销毁/状态改变)时，这个回调都会进行相应处理。
## 三、自定义手势
如果系统提供的手势不能满足你，你也可以自定义手势。自定义手势需要继承：UIGestrureRecognizer,并且需要导入头文件#import <UIKit/UIGestureRecognizerSubclass.h>，实现以下四个方法：
```
– touchesBegan:withEvent:  
– touchesMoved:withEvent:  
– touchesEnded:withEvent:  
- touchesCancelled:withEvent:
```
更多自定义手势内容请看[这里](https://developer.apple.com/documentation/uikit/touches_presses_and_gestures/implementing_a_custom_gesture_recognizer?language=objc)
关于iOS-UITouch事件处理请看 [UITouch事件处理-原理篇](https://www.jianshu.com/p/3d3156b10a36)
`本文借鉴了一些前辈的文章，如果有不对的地方请指正，欢迎大家一起交流学习。`
作者：若有草
链接：https://www.jianshu.com/p/4ca805af1570
來源：简书
简书著作权归作者所有，任何形式的转载都请联系作者获得授权并注明出处。
