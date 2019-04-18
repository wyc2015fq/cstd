# UIViewAnimation动画 - weixin_33985507的博客 - CSDN博客
2016年07月11日 18:57:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：3
UIViewAnimation, 是UIView自带的最基础的动画, 使用方便, 但是效果有限.
使用实例:
```
#import "ViewController.h"
@interface ViewController ()
//声明一个控件属性
@property (strong, nonatomic) UILabel *label;
@end
@implementation ViewController
- (void)viewDidLoad {
    [super viewDidLoad];
    
    #1. 初始化该控件
    self.label = [[UILabel alloc] init];
    #2. 设置控件的位置大小
    self.label.bounds = CGRectMake(0, 0, 100, 100);
    #3. 锚点(默认位置为(0.5, 0.5), 即控件的中心点, 相对于自身来说), 控件锚点的位置, 与父视图的(0, 0)点重合
    self.label.layer.anchorPoint = CGPointMake(0, 0);
  
    #4.  设置背景颜色, 并添加到视图上
    self.label.backgroundColor = [UIColor redColor];
    [self.view addSubview:self.label];
    
# 以上控件的设置, 是为了在动画执行时能够明显的表示出来
******************************************************************
    # 动画开始执行时触发的方法, 同下面的开始方法
    [UIView setAnimationWillStartSelector:@selector(startAC:)];
    
}
# 动画开始执行时触发的方法
- (void)animationWillStart:(NSString *)animationID  context:(void *)context{
    
    NSLog(@"开始*********");
    
}
#通过Touch手势, 触发动画
- (void)touchesBegan:(NSSet<UITouch *> *)touches withEvent:(UIEvent *)event{
    
    //1. 创建一个动画
    [UIView beginAnimations:nil context:nil];
    
    //2. 动画延迟设置
    [UIView setAnimationDelay:1];
    
    //3. 给动画添加代理(不遵循代理协议, 也能实现代理方法)
    [UIView setAnimationDelegate:self];
    
    //4. 给动画添加方法(动画结束后执行)
    [UIView setAnimationDidStopSelector:@selector(stopAc)];
    
    //5. 动画持续时间(完成动画所需时间)
    [UIView setAnimationDuration:2];
    
    //6. 设置动画是否会自动按原路径返回
    [UIView setAnimationRepeatAutoreverses:NO];
    
    //7. 设置动画移动的新位置
    self.label.frame = CGRectMake(100, 100, 100, 100);
    
    //8. 开始动画
    [UIView commitAnimations];
}
- (void)stopAc{
    
    NSLog(@"动画结束后执行的方法");
}
@end```
>#*UIViewAnimation常用的方法
    # 创建一个动画, 传递上下文等信息
    + (void)beginAnimations:(nullable NSString *)animationID context:(nullable void *)context;
    # 设置完动画参数后, 开始动画
    + (void)commitAnimations;  
    # 设置代理, 代理可以遵循也可以不
    + (void)setAnimationDelegate:(nullable id)delegate;                 // default = nil
    # 动画将要开始时,触发的方法
    + (void)setAnimationWillStartSelector:(nullable SEL)selector;       // default = NULL. -animationWillStart:(NSString *)animationID context:(void *)context
    # 动画结束后执行的方法 
    + (void)setAnimationDidStopSelector:(nullable SEL)selector;         // default = NULL. -animationDidStop:(NSString *)animationID finished:(NSNumber *)finished context:(void *)context
    # 设置动画执行的持续时间
    + (void)setAnimationDuration:(NSTimeInterval)duration;              // default = 0.2
    # 设置动画开始执行的延迟时间
    + (void)setAnimationDelay:(NSTimeInterval)delay;                    // default = 0.0
    + (void)setAnimationStartDate:(NSDate *)startDate;                  // default = now ([NSDate date])
    # 设置动画块中的动画属性变化的曲线 ,动画进入/退出视图的效果
    + (void)setAnimationCurve:(UIViewAnimationCurve)curve;              // default = UIViewAnimationCurveEaseInOut
    # 设置动画执行的次数
    + (void)setAnimationRepeatCount:(float)repeatCount;                 // default = 0.0.  May be fractional
    # 设置动画是否会"按原动画路径执行返回的动画"
    + (void)setAnimationRepeatAutoreverses:(BOOL)repeatAutoreverses;    // default = NO. used if repeat count is non-zero
    # 设置动画从当前状态开始播放
    + (void)setAnimationBeginsFromCurrentState:(BOOL)fromCurrentState;  // default = NO. If YES, the current view position is always used for new animations -- allowing animations to "pile up" on each other. Otherwise, the last end state is used for the animation (the default).
```
