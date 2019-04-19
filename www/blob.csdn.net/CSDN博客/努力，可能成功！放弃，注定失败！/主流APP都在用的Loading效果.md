# 主流APP都在用的Loading效果 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2019年01月17日 11:45:02[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：44
![](https://upload-images.jianshu.io/upload_images/1396454-ade8880b9c8f9ad5.gif?imageMogr2/auto-orient/strip%7CimageView2/2/w/245/format/webp)
gif图比效果偏快
### 前言：
> 
越来越多的应用，比如饿了么、掘金、京东，在等待网络时使用闪烁的效果，那么这种效果，如何实现？且听我娓娓道来，相关代码已经放在[github](https://github.com/iDwyane/DWLoadingShimmer.git)
![](https://upload-images.jianshu.io/upload_images/1396454-e7d0a51386297cc4.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/782/format/webp)
本文概要
### 一、增加覆盖层以及动态效果
1、获取控件path
```
// 获取每个子控件的path，用于后面的加遮盖 mask layer
    // 添加圆角
    UIBezierPath *defaultCoverblePath = [UIBezierPath bezierPathWithRoundedRect:subview.bounds cornerRadius:subview.frame.size.height/2.0/*subview.layer.cornerRadius*/];
    if ([subview isMemberOfClass:[UILabel class]] || [subview isMemberOfClass:[UITextView class]]) {
        defaultCoverblePath = [UIBezierPath bezierPathWithRoundedRect:subview.bounds cornerRadius:4];
    }
    UIBezierPath *relativePath = defaultCoverblePath;
    // 计算subview相对super的view的frame
    CGPoint offsetPoint = [subview convertRect:subview.bounds toView:view].origin; 
    [subview layoutIfNeeded];
    [relativePath applyTransform:CGAffineTransformMakeTranslation(offsetPoint.x, offsetPoint.y)];
    UIBezierPath *totalCoverablePath = [[UIBezierPath alloc] init];
    [totalCoverablePath appendPath:relativePath];
```
> 
获取每个控件，并且求出控件的path（也就是控件最外边的那些线），原本的控件遮罩只是矩形，为了美观，我建议每个控件path添加圆角
2、添加覆盖层，仅显示控件path的渐变色图层
```
// 添加挡住所有控件的覆盖层(挡住整superview，包括 superview 的子控件)
    self.viewCover.frame = CGRectMake(0, 0, view.frame.size.width, view.frame.size.height);
    [view addSubview:self.viewCover];
// gradientLayer CAGradientLayer是CALayer的一个子类,用来生成渐变色图层
    CAGradientLayer *colorLayer = [CAGradientLayer layer];
    colorLayer.frame = (CGRect)self.view.bounds;
    colorLayer.startPoint = CGPointMake(-1.4, 0);
    colorLayer.endPoint = CGPointMake(1.4, 0);
    
    // 颜色分割线
    colorLayer.colors = @[(__bridge id)[UIColor colorWithRed:0 green:0 blue:0 alpha:0.03].CGColor,(__bridge id)[UIColor colorWithRed:0 green:0 blue:0 alpha:0.1].CGColor,(__bridge id)[UIColor colorWithRed:1 green:1 blue:1 alpha:0.02].CGColor, (__bridge id)[UIColor colorWithRed:0 green:0 blue:0 alpha:0.06].CGColor, (__bridge id)[UIColor colorWithRed:0 green:0 blue:0 alpha:0.04].CGColor];
    colorLayer.locations = @[
                             [NSNumber numberWithDouble:colorLayer.startPoint.x],
                             [NSNumber numberWithDouble:colorLayer.startPoint.x],
                             @0,
                             [NSNumber numberWithDouble:0.2],
                             [NSNumber numberWithDouble:1.2]];
    [self.viewCover.layer addSublayer:colorLayer];
    
    // superview添加mask(能显示的遮罩)
    CAShapeLayer *maskLayer = [CAShapeLayer layer];
    maskLayer.path = totalCoverablePath.CGPath;
    maskLayer.fillColor = [UIColor whiteColor].CGColor;
    colorLayer.mask = maskLayer;
```
> 
1、 添加覆盖控件的覆盖层
2、 添加渐变色图层到挡住控件的覆盖层
3、为渐变色图层设置mask，从而显示mask面积下面的渐变色图层(原理看下方)
**原理：**
> 
遮罩层必须至少有两个图层，上面的一个图层为“遮罩层”，下面的称“被遮罩层”；这两个图层中只有相重叠的地方才会被显示。也就是说在遮罩层中有对象的地方就是“透明”的，可以看到被遮罩层中的对象，而没有对象的地方就是不透明的，被遮罩层中相应位置的对象是看不见的。
它的原理是：上面一层是遮罩层，下面一层是被遮罩层。遮罩层上的图，自己是不显示的。它只起到一个透光的作用。假定遮罩层上是一个正圆，那么光线就会透过这个圆形，射到下面的被遮罩层上，只会显示一个圆形的图形。如果遮罩层上什么都没有，那么光线就无法透到下面来，那么下面的被遮罩层什么也显示不出来。
上述代码，得出如下效果：
![](https://upload-images.jianshu.io/upload_images/1396454-44f33fd314f52147.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/359/format/webp)
上述效果，其实很多app就单纯这样使用了，但是我们为了更美化，决定为其增加动态效果
3、为渐变层增加动态效果
```
// 动画 animate
    CABasicAnimation *animation = [CABasicAnimation animationWithKeyPath:@"locations"];
    animation.fromValue = colorLayer.locations;
    animation.toValue = @[
                          @0,
                          @1,
                          @1,
                          @1.2,
                          @1.2];
    animation.duration = 0.7;
    animation.repeatCount = HUGE;
    [animation setRemovedOnCompletion:NO];
    // 为渐变层增加添加动画
    [colorLayer addAnimation:animation forKey:@"locations-layer"];
```
### 二、移除所有覆盖层以及效果
```
// 移除动态效果以及图层
    [self.colorLayer removeAllAnimations];
    [self.colorLayer removeFromSuperlayer];
    [self.maskLayer removeFromSuperlayer];
    // 移除控件的覆盖层
    [self.viewCover removeFromSuperview];
```
就这样，完成了一个主流APP的Loding图，我针对此代码还进行了特殊封装，代码已经放在[github](https://github.com/iDwyane/DWLoadingShimmer.git)
![](https://upload-images.jianshu.io/upload_images/1396454-9c672092c2053c80.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/352/format/webp)
demo图
作者：Dwyane_Coding
链接：https://www.jianshu.com/p/262656763aa5
來源：简书
简书著作权归作者所有，任何形式的转载都请联系作者获得授权并注明出处。
