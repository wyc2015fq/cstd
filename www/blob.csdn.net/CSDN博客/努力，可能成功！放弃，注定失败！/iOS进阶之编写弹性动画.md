# iOS进阶之编写弹性动画 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2016年08月15日 10:10:22[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：1776
## 前言
之前在[iOS开发干货 第1期](http://blog.devlong.com/2016/04/30/library-share/)中提到过一个挺有意思的数字转变动画[NumberMorphView](https://github.com/me-abhinav/NumberMorphView) , 如下图：
![](https://raw.githubusercontent.com/me-abhinav/NumberMorphView/dev/sample.gif)
NumberMorphView
我将通过几篇文章对这个开源库做一些分析，当然，这篇文章不会对它做全面的解析，而是利用这个库的一些技术概念来做一些简单的示例，也算是一个引子，后面会抽时间再写一篇对这个库的代码分析，敬请期待。
## 要做些什么
我们将会使用CADisplayLink + CAShapeLayer + UIBezierPath结合制作一个毫秒级的画圆动画，不同的是，这个动画具有弹性效果，下面先来看看制作的效果：
![](http://upload-images.jianshu.io/upload_images/416851-dfdecdd4c73e4280.gif?imageMogr2/auto-orient/strip)
效果预览
## 开始
### 准备工作
- 先新建一个Single View Application项目，在项目中添加类RRCircleAnimationView，继承于UIView。
- 打开Main.storyboard，将唯一的一个ViewController的view custom class修改为RRCircleAnimationView。
至此，准备工作已经完成。
### 动手来画个圆
先来个简单任务，我们来实现画圆动画。
第一步，为RRCircleAnimationView添加属性：
```
@implementation RRCircleAnimationView
{
    CADisplayLink *_displayLink; // CADisplayLink可以确保系统渲染每一帧的时候我们的方法都被调用, 从而保证了动画的流畅性，毫秒级动画就靠他。
    UIBezierPath *_path; // 用于创建基于矢量的路径
    CGPoint _beginPoint; // 开始触摸位置
    CGPoint _endPoint;  // 触摸结束的位置
    CAShapeLayer *_shapeLayer; // 可以结合UIBezierPath进行绘画
}
```
接着初始化实例变量，由于我们用的是storyboard进行加载，所以可以在awakeFromNib方法里面初始化
```
// 注意这里我们是直接从xib加载当前view。
- (void)awakeFromNib
{
    _shapeLayer = [CAShapeLayer layer];
    [self.layer addSublayer:_shapeLayer];
    _shapeLayer.fillColor = [UIColor colorWithRed:0.400 green:0.400 blue:1.000 alpha:1.000].CGColor;
    _displayLink = [CADisplayLink displayLinkWithTarget:self selector:@selector(updateFrame)];
    [_displayLink addToRunLoop:[NSRunLoop currentRunLoop] forMode:NSRunLoopCommonModes];
}
```
接下来实现上面CADisplayLink要不停调用的`updateFrame`方法，我们在此方法内不断地画圆。
```
- (void)updateFrame {
    // 画圆
    _path = [UIBezierPath bezierPathWithArcCenter:_beginPoint radius:[self getRadius] startAngle:0 endAngle:M_PI*2 clockwise:YES];
    _shapeLayer.path = _path.CGPath;
}
```
上面我们用开始触摸的点的位置作为圆心的位置，再根据特定的半径进行绘制一个圆，这个半径是根据我们触摸的开始点和结束点进行计算出来的，开始触摸点到结束点的距离就是这个圆的半径。
我们先把触摸的起始和结束点给找到：
```
- (void)touchesBegan:(NSSet<UITouch *> *)touches withEvent:(UIEvent *)event
{
    UITouch *touch = [touches anyObject];
    CGPoint point = [touch locationInView:self];
    _beginPoint = point;
    _endPoint = point;
}
- (void)touchesMoved:(NSSet<UITouch *> *)touches withEvent:(UIEvent *)event
{
    UITouch *touch = [touches anyObject];
    CGPoint point = [touch locationInView:self];
    _endPoint = point;
}
```
最后计算用上我们中学的数学知识，根据两点坐标距离公式
![](http://upload-images.jianshu.io/upload_images/416851-b2f319ff4c58c8f4.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
两点坐标距离公式
可以得到我们起始和结束两点的距离，也就是圆的半径是：
```cpp
```cpp
- (CGFloat)getRadius
{
    CGFloat result = sqrt(pow(_endPoint.x - _beginPoint.x, 2) + pow(_endPoint.y - _beginPoint.y, 2));
    return result;
}
```
```
到这里画圆动画完成。
### 加入弹性效果
上面只是的画圆动画看起来是没什么问题了，不过总感觉缺少动感，接下来我们来帮他加入些活力！
- 
添加一下成员变量到RRCircleAnimationView类中。
```
BOOL _isTouchEnd; // 触摸结束标志
     int _currentFrame; // 当前的帧数
```
- 
在`- (void)touchesBegan:(NSSet<UITouch *> *)touches withEvent:(UIEvent *)event`方法内添加以下代码：
```
_isTouchEnd = NO; //重置触摸状态
     _currentFrame = 1; //重置当前的帧数
```
- 
添加以下方法：
```
- (void)touchesEnded:(NSSet<UITouch *> *)touches withEvent:(UIEvent *)event
 {
     _isTouchEnd = YES; //触摸结束，更新触摸状态
 }
```
- 
将方法`- (CGFloat)getRadius`修改如下：
```
- (CGFloat)getRadius
 {
     CGFloat result = sqrt(pow(_endPoint.x-_beginPoint.x, 2)+pow(_endPoint.y-_beginPoint.y, 2));
     if (_isTouchEnd) {
         CGFloat animationDuration = 1.0; // 弹簧动画持续的时间
         int maxFrames = animationDuration / _displayLink.duration;
         _currentFrame++;
             if (_currentFrame <= maxFrames) {
                 CGFloat factor = [self getSpringInterpolation:(CGFloat)(_currentFrame) / (CGFloat)(maxFrames)]; //根据公式计算出弹簧因子
                 return MAX_RADIUS + (result - MAX_RADIUS) * factor; // 根据弹簧因子计算当前帧的圆半径
             }else {
                 return MAX_RADIUS;
             }
     }
     return result;
 }
```
- 
最后加入神奇的公式：
```cpp
```cpp
- (CGFloat)getSpringInterpolation:(CGFloat)x
 {
     CGFloat tension = 0.3; // 张力系数
     return pow(2, -10 * x) * sin((x - tension / 4) * (2 * M_PI) / tension);
 }
```
```
这个公式用数学符号表达出来是：
![](http://upload-images.jianshu.io/upload_images/416851-ee60369469a6c045.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
Math
可以用Mac OS X自带的软件叫`Grapher`画出此函数的的图像，如下图：
![](http://upload-images.jianshu.io/upload_images/416851-4a9971912fa51c85.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
Grapher
这个函数的作用其实就是通过x值，也就是当前帧数除以允许的最大帧数。
`(CGFloat)(_currentFrame) / (CGFloat)(maxFrames)`
因此，x的值的范围也就是(0, 1]。
我们所要的动画效果是把圆拉大到超过或者小于设定的目标半径`MAX_RADIUS`时，需要一个弹性动画逐渐回到设定好的目标半径。
回头再看一下实时计算动画半径的公式：
`MAX_RADIUS + (result - MAX_RADIUS) * factor`
为了让x = 1的时候，半径 = MAX_RADIUS，所以这时factor就应该为0，也就是f(1) = 0。
再看看刚才的函数图像，在x = 0到1之前振动，随着x的增加振幅逐渐减少，当x = 1的时候，y值为0。
## 最后
这篇文章讲述了如何自己实现具有弹性的帧动画，如果能理解好这种动画制作原理，对动画效果开发是很有帮助的，后面有时间会继续写其他的一些动画制作的方法，实现更多的动画效果。
差点忘了说了，目前这个动画已经放到github上面，传送门：[RRongAnimation](https://github.com/MellongLau/RRongAnimation)
![](https://raw.github.com/MellongLau/RRongAnimation/master/Screenshots/screenshot.gif)
RRongAnimation
*The End*
文／Mellong（简书作者）
原文链接：http://www.jianshu.com/p/cf9f600a5342
著作权归作者所有，转载请联系作者获得授权，并标注“简书作者”。
