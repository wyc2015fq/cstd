# 动画实战：自定义提醒控件AlertView - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2017年03月03日 17:20:12[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：326标签：[自定义提醒控件AlertView																[iOS常用方法																[iOS学习资料																[iOS 资源大全](https://so.csdn.net/so/search/s.do?q=iOS 资源大全&t=blog)](https://so.csdn.net/so/search/s.do?q=iOS学习资料&t=blog)](https://so.csdn.net/so/search/s.do?q=iOS常用方法&t=blog)](https://so.csdn.net/so/search/s.do?q=自定义提醒控件AlertView&t=blog)
个人分类：[iOS开发](https://blog.csdn.net/qq_30513483/article/category/6072895)
![](http://upload-images.jianshu.io/upload_images/2312304-9aef3fe5b39dcf03.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
自定义提醒控件AlertView.png
# 本文转自简书，原文地址:[http://www.jianshu.com/p/d60dc6bbe537](http://www.jianshu.com/p/d60dc6bbe537)
# 前言
本文是文章[上手CAShapeLayer，动画其实并不难](http://www.jianshu.com/p/756373ff3294) 的实战，用到的知识有CAShapeLayer、UIBezierPath和CABasicAnimation。如果对这些类不大了解，可先去基础篇看看。
# 正文
### 一、效果展示
![](http://upload-images.jianshu.io/upload_images/2312304-f379c9fdff81502a.gif?imageMogr2/auto-orient/strip)
load.gif
![](http://upload-images.jianshu.io/upload_images/2312304-6c0aadae4b7763ec.gif?imageMogr2/auto-orient/strip)
success.gif
![](http://upload-images.jianshu.io/upload_images/2312304-12dabf1191773ad9.gif?imageMogr2/auto-orient/strip)
error.gif
### 二、思路讲解
既然要封装起来，那么肯定是想要用起来越方便越好。所以首先，我的思路是创建一个UIViewController的拓展类，在拓展类里写方法。比如当需要提示成功时，直接
```
[self
 showSuccessAlertView];
```
就可以。然后控件的初始化以及属性设置都放在`- (void)showSuccessAlert;`里面了。
其次，想到这个控件整个项目中都要用，所以可以用单例模式，这样可以节省不必要的性能消耗。
再者，考虑到由于不同内容的显示，AlertView中layer的路径也会不一样。我便创建了三个继承CAShapeLayer的layer。分别为`YQErrorLayer`、`YQLoadingLayer`、`YQSuccessLayer`。然后可在AlertView中控制layer的使用。因为这三个很类似，所以干脆再创一个继承CAShapeLayer的layer：
`YQShapeLayer`,再把上面三个layer继承自`YQShapeLayer`。这样一来有个好处，就是他们一样的方法可以写到父类中。方便管理。如果感觉有点乱，关系如下图：
![](http://upload-images.jianshu.io/upload_images/2312304-81081cabb1d7eb37.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
Layer关系图.png
### 三、开始代码
#### 1.创建vc拓展类
```
// .h文件
@interface UIViewController (YQAlertView)
- (void)showLoadingAlert;
- (void)showLoadingAlertWithMessage:(NSString *)message;
- (void)showErrorAlert;
- (void)showErrorAlertWithMessage:(NSString *)message;
- (void)showSuccessAlert;
- (void)showSuccessAlertWithMessage:(NSString *)message;
- (void)dismissAlert;
@end
```
对应的，.m文件中写好上述方法的实现。
不管别的，至少现在vc可以直接`[self showSuccessAlertView];`了。
#### 2.创建AlertView
创建一个继承自UIView的AlertView，在拓展类的.m文件中初始化AlertView。
直接上代码：
```
+ (instancetype)shareAlertView
{
    static YQAlertView *handle = nil;
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        handle = [[YQAlertView alloc] init];
    });
    return handle;
}
- (instancetype)init
{
    if (self = [super init]) {
        [self initView];
    }
    return self;
}
- (void)initView
{
    //省略了控件创建的代码
    //一些不变的控件都在这里创建，比如背景view，提醒label等
}
```
#### 3.定AlertView的属性
设置一些AlertView的属性，可以在初始化AlertView后设置这些属性。以下两个是必要的，其他的可以随自己爱好添加。
```
@property (nonatomic, assign) YQAlertViewType type;
@property (nonatomic, copy) NSString *message;
```
上述的YQAlertViewType：
```
typedef enum {
    YQAlertViewTypeSuccess,
    YQAlertViewTypeError,
    YQAlertViewTypeLoading
} YQAlertViewType;
```
然后写一个设置上述属性的方法:
`- (void)setAlertViewType:(YQAlertViewType)type message:(NSString *)message;`
到这里，可以在vc拓展类中初始化AlertView和设置属性啦。以成功为例子：
```
- (void)showSuccessAlert
{
    [self showSuccessAlertWithMessage:nil];
}
- (void)showSuccessAlertWithMessage:(NSString *)message
{
    YQAlertView *alert = [YQAlertView shareAlertView];
    [alert setAlertViewType:YQAlertViewTypeSuccess message:message];
    [self.view addSubview:alert];
}
```
效果如下：
![](http://upload-images.jianshu.io/upload_images/2312304-62391ddb4c1528af.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
未完成的提醒框.png
#### 4.创建各种layer
上面的效果图看来，现在还缺少layer以及动画。按照思路里说的，创建4个layer。
在layer中，写一个初始化方法，在其中设置属性以及添加子layer。同样已成功举例子：
![](http://upload-images.jianshu.io/upload_images/2312304-2c27b39da8d27ba3.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
成功layer.png
它由外圈和内钩组成。所以代码如下：
```
- (instancetype)initWithFrame:(CGRect)frame strokeColor:(UIColor *)color
{
    if (self = [super init]) {
        self.backgroundColor = [UIColor clearColor].CGColor;
        self.color = color;
        self.frame = frame;
        [self configShape];
    }
    return self;
}
- (void)configShape
{
    _circleLayer = [[CAShapeLayer alloc] init];
    _circleLayer.frame = self.bounds;
    _circleLayer.path = [UIBezierPath bezierPathWithOvalInRect:self.bounds].CGPath;
    _circleLayer.strokeStart = 1;
    _circleLayer.lineWidth = 3;
    _circleLayer.strokeColor = self.color.CGColor;
    _circleLayer.fillColor = [UIColor clearColor].CGColor;
    [_circleLayer setValue:@(-M_PI_2) forKeyPath:@"transform.rotation.z"];
    [self addSublayer:_circleLayer];
    _successLayer = [[CAShapeLayer alloc] init];
    _successLayer.frame = CGRectMake(self.frame.size.width*0.15, self.frame.size.height*0.15, self.frame.size.width*0.7, self.frame.size.width*0.7);
    _successLayer.path = [self successLayerPath].CGPath;
    _successLayer.strokeEnd = 0;
    _successLayer.lineWidth = 5;
    _successLayer.strokeColor = self.color.CGColor;
    _successLayer.fillColor = [UIColor clearColor].CGColor;
    _successLayer.lineCap = kCALineCapRound;
    _successLayer.lineJoin = kCALineJoinRound;
    [self addSublayer:_successLayer];
}
- (UIBezierPath *)successLayerPath
{
    // 中点为0.35 0.35
    UIBezierPath *path = [UIBezierPath bezierPath];
    [path moveToPoint:CGPointMake(self.frame.size.width*0.09, self.frame.size.width*0.37)];
    [path addLineToPoint:CGPointMake(self.frame.size.width*0.29, self.frame.size.width*0.57)];
    [path addLineToPoint:CGPointMake(self.frame.size.width*0.64, self.frame.size.width*0.22)];
    return path;
}
```
因为需要动画，所以我把圆的strokeStart设置为1，钩的strokeEnd设置为0。当执行动画的时候，改变strokeStart或者strokeEnd的值，便出现了绘制的动画了。
开始动画代码：
```
- (void)startAni
{
    CABasicAnimation *circleAni = [CABasicAnimation animationWithKeyPath:@"strokeStart"];
    circleAni.toValue = @(0);
    circleAni.duration = 1;
    circleAni.removedOnCompletion = NO;
    circleAni.fillMode = kCAFillModeForwards;
    circleAni.timingFunction = [CAMediaTimingFunction functionWithName:kCAMediaTimingFunctionEaseOut];
    [_circleLayer addAnimation:circleAni forKey:@"circleAni"];
    CABasicAnimation *successAni = [CABasicAnimation animationWithKeyPath:@"strokeEnd"];
    successAni.toValue = @(1);
    successAni.duration = 1;
    successAni.removedOnCompletion = NO;
    successAni.fillMode = kCAFillModeForwards;
    successAni.timingFunction = [CAMediaTimingFunction functionWithName:kCAMediaTimingFunctionEaseOut];
    [_successLayer addAnimation:successAni forKey:@"successAni"];
}
```
这里有一点需要注意，
`- (void)startAni;`
```
- (instancetype)initWithFrame:(CGRect)frame strokeColor:(UIColor
 *)color;
```
两个方法在三个layer中都有，所以可在父类中声明。
#### 5.AlertView中使用layer
由于需要知道该AlertView属于哪个类型才能初始化哪个layer类。所以我把layer的初始化放在了
```
- (void)setAlertViewType:(YQAlertViewType)type
 message:(NSString *)message;
```
中。代码如下：
```
- (void)setAlertViewType:(YQAlertViewType)type message:(NSString *)message
{
    self.type = type;
    self.message = message;
    self.alpha = 1;
    CGRect layerRect = CGRectZero;
    if (message) {
        layerRect = CGRectMake(60, 20, 80, 80);
        self.messageLabel.text = message;
    } else {
        layerRect = CGRectMake(60, 30, 80, 80);
    }
    if (_aniLayer) {
        [_aniLayer removeFromSuperlayer];
        _aniLayer = nil;
    }
    switch (type) {
        case YQAlertViewTypeError:
            _aniLayer = [[YQErrorLayer alloc] initWithFrame:layerRect strokeColor:[UIColor colorWithRed:0.0431 green:0.7569 blue:0.9412 alpha:1.0]];
            break;
        case YQAlertViewTypeSuccess:
            _aniLayer = [[YQSuccessLayer alloc] initWithFrame:layerRect strokeColor:[UIColor colorWithRed:0.0431 green:0.7569 blue:0.9412 alpha:1.0]];
            break;
        default:
            _aniLayer = [[YQLoadingLayer alloc] initWithFrame:layerRect strokeColor:[UIColor colorWithRed:0.0431 green:0.7569 blue:0.9412 alpha:1.0]];
            break;
    }
    [self.alertView.layer addSublayer:_aniLayer];
    if (type != YQAlertViewTypeLoading) {
        dispatch_after(dispatch_time(DISPATCH_TIME_NOW, (int64_t)(2 * NSEC_PER_SEC)), dispatch_get_main_queue(), ^{
            [self dismiss];
        });
    }
}
```
其实说白了，这个方法里，都是一些需要知道属性才能设置的代码。比如layer的位置设置或者layer的初始化或者messageLabel是否隐藏。
`_aniLayer`是父类YQShapeLayer类的变量，不管AlertView的type如何，不管初始化哪个layer，赋值给他都没问题，毕竟他是父类嘛。
这里也有个需要注意的地方，由于我们用的是单例模式，而layer的初始化并不是放在单例方法中，所以如果不执行
```
[_aniLayer
 removeFromSuperlayer];
```
方法，就会出现下图两个layer都存在的问题：
![](http://upload-images.jianshu.io/upload_images/2312304-6375dee4d8024434.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
问题.png
#### 6.消失处理
AlertView能出现，也需要能消失。
```
- (void)dismiss
{
    [UIView animateWithDuration:0.5 animations:^{
        self.alpha = 0.1;
    } completion:^(BOOL finished) {
       [self removeFromSuperview];
    }];
}
```
当需要消失的时候调用该方法即可。
这里也有一个注意点，就是load是不需要自己消失的，其他两个类型是需要自己消失的。所以在设置完type属性的时候，就应该判断是否不为load类型，如果是，就需要自己消失。
代码其实在第5步中已经写了。
#### 7.其他
剩下的是一些细节的处理了。比如AlertView出现时的动画设置，消失的动画处理等等。
# 结束
到这里，简单的提醒控件AlertView就实现了。由于我的能力较浅，暂时只能封装到这一步。文章中或者代码中有任何问题请不吝赐教！谢谢~
本文章对应代码：[https://github.com/JabberYQ/YQAlertView](https://github.com/JabberYQ/YQAlertView)
