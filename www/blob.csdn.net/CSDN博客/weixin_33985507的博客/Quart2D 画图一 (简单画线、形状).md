# Quart2D 画图一 (简单画线、形状) - weixin_33985507的博客 - CSDN博客
2016年06月29日 10:07:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：7
自我使用学习画图的一些用法，可以由简单开始，实现复杂画图操作
### 画线
```
#pragma mark - 绘制曲线
- (void)drawCurve
{
    /** 绘制曲线 */
    
    /** 1.获取上下文 */
    CGContextRef ctf = UIGraphicsGetCurrentContext();
    
    /** 2.描述路径 */
    /** 一定要有起点 */
    CGContextMoveToPoint(ctf, 50, 50);
    CGContextAddQuadCurveToPoint(ctf, 150, 20, 250, 50);
    
    /** 3.渲染上下文 */
    CGContextStrokePath(ctf);
}
#pragma mark - 线的状态
/**
 * beaier 封装的画法
 */
- (void)drawStateBezier
{
    UIBezierPath *path = [UIBezierPath bezierPath];
    
    [path moveToPoint:CGPointMake(50, 50)];
    
    [path addLineToPoint:CGPointMake(200, 200)];
    
    path.lineWidth = 10;
    
    [path stroke];
    
    
    UIBezierPath *path1 = [UIBezierPath bezierPath];
    
    [path1 moveToPoint:CGPointMake(50, 50)];
    
    [path1 addLineToPoint:CGPointMake(200, 200)];
    
    path1.lineWidth = 10;
    
    [path1 stroke];
}
/**
 * 系统画法
 */
- (void)drawContextRefState
{
    /** 获取图形上下文 */
    CGContextRef ctf = UIGraphicsGetCurrentContext();
    
    /** 获取路径 */
    CGContextMoveToPoint(ctf, 50, 50);
    CGContextAddLineToPoint(ctf, 50, 200);
    
    /** 重新设置起点，两根线会分开 */
    //    CGContextMoveToPoint(ctf, 50, 50);
    /** 默认上一根线的终点是下一根线的起点 */
    CGContextAddLineToPoint(ctf, 200, 50);
    
    /** 设置绘图状态, 一定要在渲染之前 */
    /** 颜色 */
    [[UIColor redColor] setStroke];
    /** 线宽 */
    CGContextSetLineWidth(ctf, 30);
    /** 设置连接样式 */
    CGContextSetLineJoin(ctf, kCGLineJoinRound);/** 圆拐角 */
    /** 设置顶角样式 */
    CGContextSetLineCap(ctf, kCGLineCapRound);
    
    CGContextStrokePath(ctf);
}
#pragma mark - 画线的三种方式
/**
 * bezier画线
 */
- (void)drawBezierLines
{
    /** UIKit封装了绘图功能 */
    
    /** 贝塞尔曲线 */
    
    /** 创建路径 */
    UIBezierPath *path = [UIBezierPath bezierPath];
    
    /** 设置起点 */
    [path moveToPoint:CGPointMake(50, 50)];
    
    /** 添加一个线 */
    [path addLineToPoint:CGPointMake(200, 200)];
    
    /** 渲染 */
    [path stroke];
}
/**
 * 系统画线
 */
- (void)drawLine
{
    /** 1.获取图形上下文 */
    /** 目前我们用的上下文都是UIGraphics开头 */
    CGContextRef ctx = UIGraphicsGetCurrentContext();
    
    /** 2.描述路径 */
    /** 创建路径 */
    CGMutablePathRef path = CGPathCreateMutable();
    /** 设置起点 */
    CGPathMoveToPoint(path, NULL, 50, 30);
    /** 添加一根线到莫个点 */
    CGPathAddLineToPoint(path, NULL, 200, 200);
    
    /** 3.路径添加到上下文 */
    CGContextAddPath(ctx, path);
    
    /** 4.渲染上下文到view的图层上 */
    CGContextStrokePath(ctx);
}
/**
 * 省略的系统画线方法
 */
- (void)drawLineSimple
{
    /** 获取上下文 */
    CGContextRef ctf = UIGraphicsGetCurrentContext();
    
    /** 描述路径 */
    /** 设置起点 */
    CGContextMoveToPoint(ctf, 50, 50);
    /** 设置终点 */
    CGContextAddLineToPoint(ctf, 200, 200);
    
    /** 渲染上下文 */
    CGContextStrokePath(ctf);
    
}
```
### 画形状
```
#pragma mark - 画简单形状
- (void)drawCycleLine
{
    /** 圆角矩形  */
    UIBezierPath *path = [UIBezierPath bezierPathWithRoundedRect:CGRectMake(40, 50, 40, 60) cornerRadius:5];
    
    [path stroke];
    
    /** 圆弧
     * Center: 圆心
     * angle:  弧度
     * clockwise: yes是顺时针
     */
    UIBezierPath *path1 = [UIBezierPath bezierPathWithArcCenter:CGPointMake(125, 125) radius:100 startAngle:0 endAngle:M_PI clockwise:YES];
    
    [path1 stroke];
    
    //    [path fill];/** 一个完整的封闭路径，用填充，整个路径的面都是一样的 */
}
#pragma mark - 画扇形
- (void)drawSector
{
    CGPoint center = CGPointMake(125, 125);
    
    UIBezierPath *path = [UIBezierPath bezierPathWithArcCenter:center radius:80 startAngle:0 endAngle:M_PI_2 clockwise:YES];
    
    [path addLineToPoint:center];
    
    /** 关闭路径
     * 默认从终点画到起点
     */
//    [path closePath];
    
    /** 如果使用填充，会自动关闭路径 */
    [path fill];
    
}
```
#### 注意: 这些方法一定要在视图的 drawRect 方法中调用
```
- (void)drawRect:(CGRect)rect {
    // Drawing code
    
    [self drawSector];
}
```
