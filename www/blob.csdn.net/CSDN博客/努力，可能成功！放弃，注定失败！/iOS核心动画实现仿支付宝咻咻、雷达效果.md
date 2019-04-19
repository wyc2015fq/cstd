# iOS核心动画实现仿支付宝咻咻、雷达效果 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2016年05月16日 11:29:17[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：1216
今天闲余时间写了一个类似支付宝咻咻或者说雷达效果。望大神勿笑、写的粗糙。
更新了下文章，贴出Demo地址
[github](https://github.com/MoreCookie/RadarAnimationView)
先上效果图吧：
![](http://upload-images.jianshu.io/upload_images/1599305-640a867bc9f97dd2.gif?imageMogr2/auto-orient/strip)
雷达效果图
##### 说下主要思路吧：
> 
主要用到了CAShapeLayer和UIBezierPath的经典结合画出自己想要的图再结合CAAnimation将动画添加到layer上实现效果。
> 
timingFunction:动画的运动轨迹，用于变化起点和终点之间的插值计算，即它决定了动画运行的节奏，比如是均 匀变化（相同时间变化量相同）还是先快后慢，先慢后快还是先慢再快再慢。
动画的开始与结束的快慢，有五个预置分别为:
kCAMediaTimingFunctionLinear
线性，即匀速
kCAMediaTimingFunctionEaseIn 先慢后快
kCAMediaTimingFunctionEaseOut 先快后慢
kCAMediaTimingFunctionEaseInEaseOut 先慢后快再慢
kCAMediaTimingFunctionDefault 中间快
##### 代码部分 （ 基本全部代码）
###### 自定义一个view-- RadarAnimationView.h：
> 
这里提供了一些属性、可以在外部进行设置。
```
//定义Block的目的是为了点击雷达中间按钮时调用，此处定义，方便以后使用。
  typedef void(^RaderBtnBlock)(); 
  typedef void(^RaderBtnSelectBlock)();
  #import <UIKit/UIKit.h>
 //雷达效果
 @interface RadarAnimationView : UIView
 /**
 *  按钮图片  
 */
 @property (nonatomic ,strong) UIImage *image;
 /**
  *  按钮选中图片
 */
 @property (nonatomic ,strong) UIImage *selectImage;
 /**
 *  波纹颜色
 */
  @property (nonatomic,strong) UIColor *raderColor;
 /**
  *  按钮block  未选择状态
 */
 @property (nonatomic,copy) RaderBtnBlock block;
 /**
 *  按钮block  选择状态
 */
    @property (nonatomic,copy) RaderBtnSelectBlock selectBlock;
 @end
```
##### RadarAnimationView.m实现部分
```
static const float timeinterval = 0.5;
#import <QuartzCore/QuartzCore.h>
#import "RadarAnimationView.h"
@interface RadarAnimationView ()
{
    //定时器
    NSTimer *_timer;
}
//按钮图片
@property (nonatomic,strong)UIButton *iamgeBtn;
@end
@implementation RadarAnimationView
- (void)drawRect:(CGRect)rect {
    self.alpha = 0.5;
    //创建定时器
    _timer = [NSTimer scheduledTimerWithTimeInterval:timeinterval target:self selector:@selector(setUp) userInfo:nil repeats:YES];
}
- (instancetype)initWithFrame:(CGRect)frame
{
    self = [super initWithFrame:frame];
    if (self) {
        _iamgeBtn = [[UIButton alloc]    initWithFrame:self.bounds];
        [_iamgeBtn addTarget:self action:@selector(imageBtnClick:)     forControlEvents:UIControlEventTouchUpInside];
        [self addSubview:_iamgeBtn];
        //设置默认波纹颜色
        self.raderColor = [UIColor blueColor];
    }
    return self;
}
//按钮点击事件
-(void)imageBtnClick:(UIButton *)btn
{
    if (btn.selected) {
        NSLog(@"1");
        //调用block
        self.block(@"1");
    }else
    {
        NSLog(@"2");
        //调用block
        self.selectBlock(@"2");
    }
    btn.selected = !btn.selected;
}
//重写setter方法
-(void)setImage:(UIImage *)image
{
    _image = image;
    [self.iamgeBtn setImage:image forState:0];
}
-(void)setSelectImage:(UIImage *)selectImage
{
    _selectImage = selectImage;
    [self.iamgeBtn setImage:selectImage forState:1];
}
-(void)setRaderColor:(UIColor *)raderColor
{
    _raderColor = raderColor;
}
//画雷达圆圈图
-(void)setUp
{
    CGPoint center = CGPointMake(self.bounds.size.height / 2, self.bounds.size.width / 2);
    //使用贝塞尔画圆
    UIBezierPath *path = [UIBezierPath bezierPathWithArcCenter:center radius:50 startAngle:0 endAngle:2 * M_PI clockwise:YES];
    CAShapeLayer *shapeLayer = [CAShapeLayer layer];
    shapeLayer.frame = self.bounds;
    //填充颜色
    shapeLayer.fillColor = self.raderColor.CGColor;
    //设置透明度、此处最好和动画里的一致、否则效果会不好
    shapeLayer.opacity = 0.2;
    shapeLayer.path = path.CGPath;
    [self.layer insertSublayer:shapeLayer below:self.iamgeBtn.layer];
    [self addAnimation:shapeLayer];
}
-(void)addAnimation:(CAShapeLayer *)shapeLayer
{
    //雷达圈圈大小的动画
    CABasicAnimation *basicAnimation = [CABasicAnimation animation];
    basicAnimation.keyPath = @"path";
    CGPoint center = CGPointMake    (self.bounds.size.height / 2, self.bounds.size.width / 2);
    UIBezierPath *path1 = [UIBezierPath bezierPathWithArcCenter:center radius:1 startAngle:0 endAngle:2 * M_PI clockwise:YES];
    UIBezierPath *path2 = [UIBezierPath bezierPathWithArcCenter:center radius:[UIScreen mainScreen].bounds.size.height startAngle:0 endAngle:2 * M_PI clockwise:YES];
    basicAnimation.fromValue = (__bridge id _Nullable)(path1.CGPath);
    basicAnimation.toValue = (__bridge id _Nullable)(path2.CGPath);
    //保持最新状态
    basicAnimation.fillMode = kCAFillModeForwards;
    //雷达圈圈的透明度
    CABasicAnimation *opacityAnimation = [CABasicAnimation animation];
    opacityAnimation.keyPath = @"opacity";
    opacityAnimation.fromValue = @(0.2);
    opacityAnimation.toValue = @(0);
    opacityAnimation.fillMode = kCAFillModeForwards;
    //组动画
    CAAnimationGroup *group = [CAAnimationGroup animation];
    group.animations = @[basicAnimation,opacityAnimation];
    group.duration = 7;
   //动画定时函数属性 先快后慢 
    group.timingFunction = [CAMediaTimingFunction functionWithName:kCAMediaTimingFunctionEaseOut];
    group.delegate = self;
    //指定的时间段完成后,动画就自动的从层上移除
 group.removedOnCompletion = YES;
    //添加动画到layer
    [shapeLayer addAnimation:group forKey:nil];
}
#pragma mark 动画结束
-(void)animationDidStop:(CAAnimation *)anim     finished:(BOOL)flag
{
     //虽然效果实现了、而且在内存上也基本在25m旁边.但是实现雷达波纹的波浪是一直在创建对象、所以下叙是为了及时释放不用的Shapelayear。
     //此种方法欠妥、后面有时间会做更新
    if (flag) {
        NSLog(@"----");
        //释放动画结束的对象  获取最上层layer
        if ([self.layer.sublayers[0] isKindOfClass:[CAShapeLayer class]]) {
            CAShapeLayer *shaperLayer = (CAShapeLayer *)self.layer.sublayers[0];
            [shaperLayer removeFromSuperlayer];
            shaperLayer = nil;
            NSLog(@"%lu",self.layer.sublayers.count);
        }
    }
}
@end
```
###### 在VC中调用
```
//
#import "ViewController.h"
#import "RadarAnimationView.h"
@interface ViewController ()
@end
@implementation ViewController
- (void)viewDidLoad {
    [super viewDidLoad];
    self.view.backgroundColor = [UIColor colorWithRed:23/255.0 green:26/255.0 blue:40/255.0 alpha:1];
    RadarAnimationView *rader = [[RadarAnimationView alloc]initWithFrame:CGRectMake(100, 100, 50, 50)];
    rader.center = self.view.center;
    rader.backgroundColor = [UIColor lightGrayColor];
    rader.image = [UIImage imageNamed:@"IMG_1314.JPG"];
    //点击按钮时两种状态调用的两种block
    rader.block = ^{
        NSLog(@"1");
    };
    rader.selectBlock = ^{
        NSLog(@"2");
    };
    [self.view addSubview:rader];
}
@end
```
好了基本实现了。

文／MoreCookies（简书作者）
原文链接：http://www.jianshu.com/p/c99bea074aad
著作权归作者所有，转载请联系作者获得授权，并标注“简书作者”。
