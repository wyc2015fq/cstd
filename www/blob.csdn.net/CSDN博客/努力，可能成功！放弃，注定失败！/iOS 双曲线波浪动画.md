# iOS 双曲线波浪动画 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2016年11月14日 15:12:38[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：1018
今天来搞一下 双曲线波浪动画,那么什么是双曲线波浪呢 看效果
![](http://upload-images.jianshu.io/upload_images/1907672-ff5bc7c0f4ac8eca.gif?imageMogr2/auto-orient/strip)
6月-24-2016 09-38-54.gif
一,
效果就是这样子 ,通过调整百分比 可以实现球形的水面的占比,那么实现的思路是什么呢
1.首先肯定是连个图层 一个深蓝色 一个浅蓝色 (我做的时候 RGB 是一样的不过是修改了透明度 )
2.就是曲线条 当然很直观的可以看出来这个曲线 可以用 正弦 或 余弦函数实现.通过CoreGraphics 来搞这个曲线 关于 CoreGraphics的只是可以百度了解一下
- 就是通过定时器NSTimer 来改变正弦函数的 欧米伽值 实现波动的效果 刷新UI就好了
思路就是这个思路 ,这个那点就在于 正弦函数图形的绘制和颜色填充
二 ,
创建一个类,继承与UIview .h 文件暴漏初始化方法 同事.m 实现初始化方法
对 UIview 绘制图形的话一般都是在这个 方法里面
```
- (void)drawRect:(CGRect)rect{
//关于这个函数呢  他是系统调用的 如果想执行他 可以 调用setNeedsDisplay 就可以调用方法
}
```
接下来开始绘制曲线
```
CGContextRef context = UIGraphicsGetCurrentContext();
    // 创建路径
    CGMutablePathRef path = CGPathCreateMutable();
    //画水
    CGContextSetLineWidth(context, 1);
    UIColor * blue = [UIColor colorWithRed:0 green:0 blue:1 alpha:0.3];
    CGContextSetFillColorWithColor(context, [blue CGColor]);
    float y= (1 - self.present) * rect.size.height;
     float y1= (1 - self.present) * rect.size.height;
    CGPathMoveToPoint(path, NULL, 0, y);
    for(float x=0;x<=rect.size.width * 3.0;x++){
//正弦函数
        y=  sin( x/rect.size.width * M_PI + _fa/rect.size.width *M_PI ) *_bigNumber + (1 - self.present) * rect.size.height ;
        CGPathAddLineToPoint(path, nil, x, y);
    }
    CGPathAddLineToPoint(path, nil, rect.size.width , rect.size.height );
    CGPathAddLineToPoint(path, nil, 0, rect.size.height );
   // CGPathAddLineToPoint(path, nil, 0, 200);
    CGContextAddPath(context, path);
    CGContextFillPath(context);
    CGContextDrawPath(context, kCGPathStroke);
    CGPathRelease(path);
```
创建另一个 线条
```
CGMutablePathRef path1 = CGPathCreateMutable();
  //  float y1=200;
    //画水
    CGContextSetLineWidth(context, 1);
    UIColor * blue1 = [UIColor colorWithRed:0 green:0 blue:1 alpha:0.8];
    CGContextSetFillColorWithColor(context, [blue1 CGColor]);
  //  float y1= 200;
    CGPathMoveToPoint(path1, NULL, 0, y1);
    for(float x=0;x<=rect.size.width;x++){
        y1= sin( x/rect.size.width * M_PI + _fa/rect.size.width *M_PI  +M_PI ) *_bigNumber + (1 - self.present) * rect.size.height ;
        CGPathAddLineToPoint(path1, nil, x, y1);
    }
    CGPathAddLineToPoint(path1, nil, rect.size.height, rect.size.width );
    CGPathAddLineToPoint(path1, nil, 0, rect.size.height );
    //CGPathAddLineToPoint(path, nil, 0, _currentLinePointY);
    CGContextAddPath(context, path1);
    CGContextFillPath(context);
    CGContextDrawPath(context, kCGPathStroke);
    CGPathRelease(path1);
```
两个曲线的区别就是正弦函数 相差半个或一个周期
接下来就是穿件定时器 让其波动
```
- (void)createTimer{
    if (!self.myTimer) {
        self.myTimer = [NSTimer scheduledTimerWithTimeInterval:0.01 target:self selector:@selector(action) userInfo:nil repeats:YES];
    }
    [self.myTimer setFireDate:[NSDate distantPast]];
}
- (void)action{
    //让波浪移动效果
        _fa = _fa+10;
    if (_fa >= _MYframe.size.width * 2.0) {
        _fa = 0;
    }
//对于这个_fa 是什么鬼呢  他只是一个基本数据类型他的作用就是让波动起来
//y = sin(a*x + _fa) *b  +k; 
// y=  sin( x/rect.size.width * M_PI + _fa/rect.size.width *M_PI ) //*_bigNumber + (1 - self.present) * rect.size.height ;
//对比函数就知道了 如果还不明白  百度一下 高中数学(注意哦  不是高数  是高中//数学)
    [self setNeedsDisplay];
 }
```
大概呢到这里就已经差不多了 不过这样还会有问题 就死 由于 波的幅度太大 导致 百分比在100% 和 0% 时 不完美
![](http://upload-images.jianshu.io/upload_images/1907672-f491b233de1dd155.gif?imageMogr2/auto-orient/strip)
6月-24-2016 10-10-34.gif
解决办法就是 把 当为 0% 是 幅度改为0;
```cpp
```cpp
y1= sin( x/rect.size.width * M_PI + _fa/rect.size.width *M_PI  +M_PI ) *_bigNumber + (1 - self.present) * rect.size.height ;
//就是修改函数里面的 _bigNumber  这个值
```
```
```cpp
```cpp
- (void)setPresent:(CGFloat)present{
    _present = present;
//百分数的 set 方法
    //启动定时器
      [self createTimer];
     //修改波浪的幅度
    if (present <= 0.5) {
//当 百分比小于50% 幅度  幅度 和百分比成正比
        _bigNumber = _MYframe.size.height * 0.1 * present * 2;
    }else{
//当 百分比大于50% 幅度  幅度 和百分比成反比
        _bigNumber = _MYframe.size.height * 0.1 * (1 - present) * 2;
    }
}
```
```
问题到此结局 代码地址
[https://git.oschina.net/GAOZEJIAN/double-line.git](https://git.oschina.net/GAOZEJIAN/double-line.git)
欢迎各位道友指点
文／健健锅（简书作者）
原文链接：http://www.jianshu.com/p/7db295fd38eb
著作权归作者所有，转载请联系作者获得授权，并标注“简书作者”。
