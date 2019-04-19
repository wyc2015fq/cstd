# iOS实践：通过核心动画完成过山车 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2017年06月07日 10:51:42[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：484
[本文来自简书，原文地址:http://www.jianshu.com/p/f6e147547fe0](http://www.jianshu.com/p/f6e147547fe0)
呼哧，终于今天到了最后一篇啦，也是醉了，弄了两三个月。从最开始计划只写三篇就好了，结果自己没把握好，一点点加成了今天这个样子。因为增加的内容太多，也差点变成太监文，不过好在没有放弃自己。所以各位行行好，要是看上去觉得还不错，就点个赞，打赏小的点儿。这玩意儿写的我是头发乱发，两眼通红。哇哇哇哇～
接下来要写啥，确实还没想好。现在的感觉就是胸口的一块大石头没有了，要去尽情的嗨皮！！！！
之前在一个网站上看到了一个HTML5/SVG实现的过山车动画，[点这里看网页版](http://www.html5tricks.com/html5-svg-shanche-animation.html)。 觉得很棒，想想咱们iOS也完全可以实现，正好还可以全面回顾一下之前分享过的关于iOS中间动画系列会使用到的各个内容。不过今天的内容稍微有点多，我呢尽量只说最重要的部分，这里面所有的内容都是通过代码绘制出来的。
实现后的效果图：（这也是为了简书抓图用的，不知道为啥现在如果是gif，简书不会当成文章的缩略图。好心烦～）
![](http://upload-images.jianshu.io/upload_images/2248583-7b6cd87e8a7e99e4.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/500)
Paste_Image.png
完成后的动态图：
![](http://upload-images.jianshu.io/upload_images/2248583-f00a51f8e4fc3907.gif?imageMogr2/auto-orient/strip%7CimageView2/2/w/500)
过山车.gif
# 1. 思路和所用到的内容
## 1.1 思维导图
![](http://upload-images.jianshu.io/upload_images/2248583-2ec18fc743984868.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
过山车思维导图.png
## 1.2 所用到的知识
在这里，我们使用到了：
- CALayer、CAShapeLayer、CAGradientLayer三种layer。
- UIBeizerPath的使用，包括二次贝塞尔曲线、三次贝塞尔曲线的应用，使用BeizerPath绘画圆。
- CAKeyframeAnimation的应用。
所有上面的内容之前的文章里面都有仔细的写过怎么使用哒，要是不清楚的小伙伴们可以翻翻之前的文章。几乎绝大部分的内容都在iOS动画系列这个里面。
## 1.3 最耗费时间的地方
特别想拿出来说说这个最耗费时间的东东。想都不用想，当然是火车轨道比较麻烦啦。但是这个对我来说还不是花费时间最长的，花费时间最长的居然是那两座雪山。为了绘画那两座雪山，还有山上面的积雪简直是费老鼻子劲了。所以火车轨道、雪山俺会单独拿出两小节来说说这个令人头疼的玩意。
# 2. 辅助元素的创建（背景颜色、草坪、大地、小树、云彩）
辅助元素完成后的效果图：
![](http://upload-images.jianshu.io/upload_images/2248583-a2bc5d67a82842ea.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/400)
Paste_Image.png
## 2.1 渐变的天空背景
使用CAGradientLayer进行设置，就是一个最基本的应用，让成45度角进行变换。
受篇幅限制，代码我就不贴了，在源代码里面自己看吧。注释写的还算比较详细啦，自我感觉。哈哈～
CAGradientLayer的基础部分可以看看这个文章，[第九篇：iOS动画系列之九：实现点赞的动画及播放起伏指示器](http://www.jianshu.com/p/15aa50fdcd60)。
## 2.2 草坪
主要是使用两个二次贝塞尔曲线实现的。
```
[leftLawnPath moveToPoint:leftStartPoint];
    [leftLawnPath addLineToPoint:CGPointMake(0, k_SIZE.height - 100)];
    //    画一个二次贝塞尔曲线
    [leftLawnPath addQuadCurveToPoint:CGPointMake(k_SIZE.width / 3.0, k_LAND_BEGIN_HEIGHT) controlPoint:CGPointMake(k_SIZE.width / 5.0, k_SIZE.height - 100)];
    leftLawn.path = leftLawnPath.CGPath;
    leftLawn.fillColor = [UIColor colorWithDisplayP3Red:82.0 / 255.0 green:177.0 / 255.0 blue:52.0 / 255.0 alpha:1.0].CGColor;
    [self.view.layer addSublayer:leftLawn];
    CAShapeLayer *rightLawn = [[CAShapeLayer alloc] init];
    UIBezierPath *rightLawnPath = [[UIBezierPath alloc] init];
```
### 二次贝塞尔曲线
`-(void)addQuadCurveToPoint:(CGPoint)endPoint controlPoint:(CGPoint)controlPoint;`
![](http://upload-images.jianshu.io/upload_images/2248583-088474d0dbcb54c1.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/400)
二次贝塞尔曲线.png
## 2.3 云彩动画的实现
云彩的漂浮就是通过CAKeyframeAnimation，让其沿着绘画的直线曲线进行运动。
```
CALayer *cloud = [[CALayer alloc]init];
    cloud.contents = (__bridge id _Nullable)([UIImage imageNamed:@"cloud"].CGImage);
    cloud.frame = CGRectMake(0, 0, 63, 20);
    [self.view.layer addSublayer:cloud];
    UIBezierPath *cloudPath = [[UIBezierPath alloc] init];
    [cloudPath moveToPoint:CGPointMake(k_SIZE.width + 63, 50)];
    [cloudPath addLineToPoint:CGPointMake(-63, 50)];
    CAKeyframeAnimation *ani = [CAKeyframeAnimation animationWithKeyPath:@"position"];
    ani.path = cloudPath.CGPath;
    ani.duration = 30;
    ani.autoreverses = NO;
    ani.repeatCount = CGFLOAT_MAX;
    ani.calculationMode = kCAAnimationPaced;
    [cloud addAnimation:ani forKey:@"position"];
```
## 2.4 大地、小树的实现
就是分别创建了大地和小树的CALayer，为了使用不同的方法，大地我们通过backgroundColor填充了图片。小树的Layer，我们通过设置contents进行了图片填充。
```
//大地的背景填充
    _landLayer.backgroundColor = [UIColor colorWithPatternImage:[UIImage imageNamed:@"ground"]].CGColor;
//小树的背景设置
    treeLayer.contents = (__bridge id _Nullable)(tree.CGImage);
```
为了能够层次不齐的放置小树，所以用了几个循环，在不同的y轴位置，添加了若干个小树。
# 3. 雪山的实现
雪山的实现其实并不是特别难，主要是很繁琐。自己还忘记了一个初中的小公式，在这个地方耽误了点时间。
## 3.1 雪山的思路
以一座雪山为例子，乍一看，以为雪山分成了两部分：雪山下半部分+山顶的雪。很快的，自己就放弃了这个思路。这样的话，中间的曲线部分画起来简直就要了人命了。所以就换了一个思路：先画一个全部被雪覆盖满的山体，然后在这个之上再画一个有棕色土地的部分。
完成后是这个样子的：
![](http://upload-images.jianshu.io/upload_images/2248583-0c8be693b227000c.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/400)
雪山.png
## 3.2 雪山绘画的步骤
STEP ONE:
![](http://upload-images.jianshu.io/upload_images/2248583-c5f3f2d1c19f3260.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/200)
覆盖满白雪的雪山.png
STEP TWO:
![](http://upload-images.jianshu.io/upload_images/2248583-5d8b1b2a8181994f.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/200)
给雪山添加棕色山体.png
STEP THREE:
![](http://upload-images.jianshu.io/upload_images/2248583-1b3e916ba9e1fc2b.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/200)
第二坐被白雪覆盖的雪山.png
STEP FOUR:
![](http://upload-images.jianshu.io/upload_images/2248583-0c8be693b227000c.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/400)
雪山.png
## 3.3 需要注意的点
在画山的过程中，最复杂的是找到山上左右两侧山坡上边缘的那个点的CGPoint。
以第一座山左边上坡上开始有雪的那个点来说。其实要计算的是从山脚到山顶两点之间的连线上任意一点的坐标。知道了X轴坐标，要计算Y轴坐标。
这个就是咱们初中学到的计算公式，y = kx + b。 k是斜率，b是截距。起点、终点已经知道了，可以很容易的计算出斜率k。根据k，再计算出b。这样给出这条线段上任意一点x轴坐标，就能轻易的算出y轴坐标了。xy都知道了，CGPoint不就知道了嘛。
```
- (CGPoint)calculateWithXValue:(CGFloat)xvalue startPoint:(CGPoint)startPoint endpoint:(CGPoint)endpoint{
    //    求出两点连线的斜率
    CGFloat k = (endpoint.y - startPoint.y) / (endpoint.x - startPoint.x);
    CGFloat b = startPoint.y - startPoint.x * k;
    CGFloat yvalue = k * xvalue + b;
    return CGPointMake(xvalue, yvalue);
}
```
## 3.4 以左边山为例
```
//    左边第一座山顶,其实就是一个白色的三角形
    CAShapeLayer *leftSnowberg = [[CAShapeLayer alloc] init];
    UIBezierPath *leftSnowbergPath = [[UIBezierPath alloc] init];
    //    把bezierpath的起点移动到雪山左下角
    [leftSnowbergPath moveToPoint:CGPointMake(0, k_SIZE.height - 120)];
    //    画一条线到山顶
    [leftSnowbergPath addLineToPoint:CGPointMake(100, 100)];
    //    画一条线到右下角->左下角->闭合
    [leftSnowbergPath addLineToPoint:CGPointMake(k_SIZE.width / 2, k_LAND_BEGIN_HEIGHT)];
    [leftSnowbergPath addLineToPoint:CGPointMake(0, k_LAND_BEGIN_HEIGHT)];
    [leftSnowbergPath closePath];
    leftSnowberg.path = leftSnowbergPath.CGPath;
    leftSnowberg.fillColor = [UIColor whiteColor].CGColor;
    [self.view.layer addSublayer:leftSnowberg];
    //    开始画山体没有被雪覆盖的部分
    CAShapeLayer *leftSnowbergBody = [[CAShapeLayer alloc] init];
    UIBezierPath *leftSnowbergBodyPath = [[UIBezierPath alloc] init];
    //    把bezierpath的起点移动到雪山左下角相同的位置
    CGPoint startPoint = CGPointMake(0, k_SIZE.height - 120);
    CGPoint endPoint = CGPointMake(100, 100);
    CGPoint firstPathPoint = [self calculateWithXValue:20 startPoint:startPoint endpoint:endPoint];
    [leftSnowbergBodyPath moveToPoint:startPoint];
    [leftSnowbergBodyPath addLineToPoint:firstPathPoint];
    [leftSnowbergBodyPath addLineToPoint:CGPointMake(60, firstPathPoint.y)];
    [leftSnowbergBodyPath addLineToPoint:CGPointMake(100, firstPathPoint.y + 30)];
    [leftSnowbergBodyPath addLineToPoint:CGPointMake(140, firstPathPoint.y)];
    [leftSnowbergBodyPath addLineToPoint:CGPointMake(180, firstPathPoint.y - 20)];
    CGPoint secondPathPoint = [self calculateWithXValue:(k_SIZE.width / 2 - 125) startPoint:endPoint endpoint:CGPointMake(k_SIZE.width / 2, k_LAND_BEGIN_HEIGHT)];
    [leftSnowbergBodyPath addLineToPoint:CGPointMake(secondPathPoint.x - 30, firstPathPoint.y)];
    [leftSnowbergBodyPath addLineToPoint:secondPathPoint];
    [leftSnowbergBodyPath addLineToPoint:CGPointMake(k_SIZE.width / 2, k_LAND_BEGIN_HEIGHT)];
    [leftSnowbergBodyPath addLineToPoint:CGPointMake(0, k_LAND_BEGIN_HEIGHT)];
    [leftSnowbergBodyPath closePath];
    leftSnowbergBody.path = leftSnowbergBodyPath.CGPath;
    UIColor *snowColor = [UIColor colorWithDisplayP3Red:139.0 /255.0 green:92.0 /255.0 blue:0.0 /255.0 alpha:1.0];
    leftSnowbergBody.fillColor = snowColor.CGColor;
    [self.view.layer addSublayer:leftSnowbergBody];
```
# 4. 轨道的实现
![](http://upload-images.jianshu.io/upload_images/2248583-96208941a6ecfdd5.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/300)
Paste_Image.png
轨道这部分主要就是花了几个二次贝塞尔曲线，三次贝塞尔曲线。那我们用最复杂的绿色这个带圆圈的轨道来分享一下。它是由三部分组成的，考虑到在最后我们会让过山车从右边进入，跑到左边去，我们就从最右侧开始画起。
最右侧有一个二次贝塞尔曲线，中间画了一个圆圈，左边是一个三次贝塞尔曲线。画完了之后，使用图片进行填充就完成了90%的工作。
为了让轨道看起来更好看一些，对轨道的边缘进行镂空，内部填充色变成透明。
## 4.1 绘画的步骤
1,先画最右边的弧线，一个二次贝塞尔曲线。
![](http://upload-images.jianshu.io/upload_images/2248583-9c830ac6a7408455.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/300)
Paste_Image.png
2，画一个圆圈。注意控制圆的半径以及圆心的位置。
![](http://upload-images.jianshu.io/upload_images/2248583-7112182da9a7f178.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/300)
Paste_Image.png
3，画最左边的那条曲线，一个三次贝塞尔曲线。其实就是有两个控制点的曲线。
![](http://upload-images.jianshu.io/upload_images/2248583-1234b97b8ed92f72.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/300)
Paste_Image.png
4，将曲线进行闭合。
![](http://upload-images.jianshu.io/upload_images/2248583-6cb6be5ee723f707.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/300)
Paste_Image.png
5，把曲线的背景颜色填充为准备好的小格子。
![](http://upload-images.jianshu.io/upload_images/2248583-030fa1f75416977a.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/300)
Paste_Image.png
6，为了让轨道看起来更加逼真，让曲线的边缘变成虚线。
![](http://upload-images.jianshu.io/upload_images/2248583-800b7cd05bb4fec3.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/300)
Paste_Image.png
## 4.2 三次贝塞尔曲线
`- (void)addCurveToPoint:(CGPoint)endPoint controlPoint1:(CGPoint)controlPoint1 controlPoint2:(CGPoint)controlPoint2;`
起点用moveToPoint的方法进行设定，endPoint：贝塞尔曲线的终点；controlPoint1:控制点1；controlPoint2:控制点2。
曲线是由起点趋向控制点1，之后趋向控制点2，最后到达终点的曲线。
![](http://upload-images.jianshu.io/upload_images/2248583-c80efcc8fb18e898.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/400)
Paste_Image.png
## 4.3 代码实现
绿色轨道绘制部分的代码：
```
//    绿色铁轨的火车从右侧进入，所以从右侧开始绘画。需要画三条曲线，右边一条+中间的圆圈+左边一条
    UIBezierPath *path = [[UIBezierPath alloc] init];
    [path moveToPoint:CGPointMake(k_SIZE.width + 10, k_LAND_BEGIN_HEIGHT)];
    [path addLineToPoint:CGPointMake(k_SIZE.width + 10, k_SIZE.height - 70)];
    [path addQuadCurveToPoint:CGPointMake(k_SIZE.width / 1.5, k_SIZE.height - 70) controlPoint:CGPointMake(k_SIZE.width - 150, 200)];
    //    画圆圈
    [path addArcWithCenter:CGPointMake(k_SIZE.width / 1.6, k_SIZE.height - 140) radius:70 startAngle:M_PI_2 endAngle:2.5 * M_PI clockwise:YES];
    [path addCurveToPoint:CGPointMake(0, k_SIZE.height - 100) controlPoint1:CGPointMake(k_SIZE.width / 1.8 - 60, k_SIZE.height - 60) controlPoint2:CGPointMake(150, k_SIZE.height / 2.3)];
    [path addLineToPoint:CGPointMake(- 10, k_LAND_BEGIN_HEIGHT)];
    _greenTrack.path = path.CGPath;
    _greenTrack.fillColor = [UIColor colorWithPatternImage:[UIImage imageNamed:@"green"]].CGColor;
```
轨道边缘镂空的代码：
```
//    为了能够让弧线更好看一点，需要加入镂空的虚线
    CAShapeLayer *trackLine = [[CAShapeLayer alloc] init];
    trackLine.lineCap = kCALineCapRound;
    trackLine.strokeColor = [UIColor whiteColor].CGColor;
    trackLine.lineDashPattern = @[@1.0,@6.0];
    trackLine.lineWidth = 2.5;
    trackLine.fillColor = [UIColor clearColor].CGColor;
    trackLine.path = path.CGPath;
    [_greenTrack addSublayer:trackLine];
```
-----------------------写在最后-------------------------------------------------
如果看官你觉得确实这篇文章有点收获，那就给个赞或者粉一下呗。再进一步，如果您认可小的这点儿辛苦，打赏点可乐钱，小的愿意给爷笑一个^&^
OC版本的源代码在这里：[http://git.oschina.net/atypical/rollercoaster](http://git.oschina.net/atypical/rollercoaster)
群众要是呼声比较高，那就还是老惯例，随后再写swift版本的。要是没啥人有兴趣，俺就拿着大家大赏的银子去买点小片儿看看，乐呵乐呵。
-----------------------华丽分割线，iOS动画系列全集链接-------------------------------------------------
[第一篇：iOS动画系列之一：通过实战学习CALayer和透视的原理。做一个带时分秒指针的时钟动画(上)](http://www.jianshu.com/p/26a7ccbc55c4)
[第二篇：iOS动画系列之二：通过实战学习CALayer和透视的原理。做一个带时分秒指针的时钟动画。包含了OC和Swift两种源代码(下)](http://www.jianshu.com/p/3a96cc95da5c)
[第三篇：iOS动画系列之三：Core Animation。介绍了Core Animation的常用属性和方法。](http://www.jianshu.com/p/2f1d0be8e15b)
[第四篇：CABasic Animation。iOS动画系列之四：基础动画之平移篇](http://www.jianshu.com/p/52375d3dd87b)
[第五篇：CABasic Animation。iOS动画系列之五：基础动画之缩放篇&旋转篇](http://www.jianshu.com/p/58ce1d8b43ef)
[第六篇：iOS动画系列之六：利用CABasic Animation完成带动画特效的登录界面](http://www.jianshu.com/p/fb40f8c084d8)
[第七篇：iOS动画系列之七：实现类似Twitter的启动动画](http://www.jianshu.com/p/1bf7fc25f17e)
[第八篇：iOS动画系列之八：使用CAShapeLayer绘画动态流量图](http://www.jianshu.com/p/69f0b7f38cb6)
[第九篇：iOS动画系列之九：实现点赞的动画及播放起伏指示器](http://www.jianshu.com/p/15aa50fdcd60)
[第十篇：实战系列:绘制过山车场景](http://www.jianshu.com/p/f6e147547fe0)
