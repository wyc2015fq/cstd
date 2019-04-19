# iOS 走进Facebook POP的世界 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2016年06月14日 09:08:23[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：379标签：[iOS常用方法																[iOS 资源大全																[iOS学习资料																[facebook																[pop](https://so.csdn.net/so/search/s.do?q=pop&t=blog)](https://so.csdn.net/so/search/s.do?q=facebook&t=blog)](https://so.csdn.net/so/search/s.do?q=iOS学习资料&t=blog)](https://so.csdn.net/so/search/s.do?q=iOS 资源大全&t=blog)](https://so.csdn.net/so/search/s.do?q=iOS常用方法&t=blog)
个人分类：[iOS开发](https://blog.csdn.net/qq_30513483/article/category/6072895)
![](http://upload-images.jianshu.io/upload_images/1506501-5b5033469dd680a3.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
听说贴上一个图是规矩
##### POP: 一个流行的可扩展的动画引擎iOS,它支持spring和衰变动态动画,使其可用于构建现实,基于物理交互。`Objective - C` API允许快速集成, 对于所有的动画和过渡他是成熟的.
解释:
*1.1* POP 使用 Objective-C++ 编写，Objective-C++ 是对 C++ 的扩展，就像 Objective-C 是 C 的扩展。而至于为什么他们用 Objective-C++ 而不是纯粹的 Objective-C.
*1.2* POP 目前由四部分组成：1. Animations；2. Engine；3. Utility；4. WebCore。下图有助于你更好的理解它的架构
![](http://upload-images.jianshu.io/upload_images/1506501-6e79634b572cc421.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
构成
- 它支持CocoaPods 你可以这样
```bash
pod 'pop', '~> 1.0.8'
```
- 
或者这样[点击下载](https://github.com/facebook/pop)拉入工程
- 
我使用的Cocoapods 所以使用之前你需要这样
`#import <POP.h>`
![](http://upload-images.jianshu.io/upload_images/1506501-0ac55a9dd5f857f1.gif?imageMogr2/auto-orient/strip)
XT_01
#### E1:
```cpp
```cpp
- (void)clickPopAction
{
    // kPOPLayerPositionY 向下
    // kPOPLayerPositionX 向右
    POPSpringAnimation *anim = [POPSpringAnimation animationWithPropertyNamed:kPOPLayerPositionY];
    // 移动距离
    anim.toValue = [[NSNumber alloc] initWithFloat:_btnPop.center.y + 200];
    // 从当前 + 1s后开始
    anim.beginTime = CACurrentMediaTime() + 1.0f;
    // 弹力--晃动的幅度 (springSpeed速度)
    anim.springBounciness = 15.0f;
    [_btnPop pop_addAnimation:anim forKey:@"position"];
    POPSpringAnimation *anim1 = [POPSpringAnimation animationWithPropertyNamed:kPOPLayerBounds];
    anim1.toValue = [NSValue valueWithCGRect:CGRectMake(100, 100, 99, 99)];
    [_btnPop pop_addAnimation:anim1 forKey:@"size"];
}
```
```
![](http://upload-images.jianshu.io/upload_images/1506501-11a437d7f1b16918.gif?imageMogr2/auto-orient/strip)
XT_02
#### E2:在很多金融类app中比较常见、支付宝中的余额包、京东金融余额、就类似这样
```
// 初始化
    POPBasicAnimation *anim = [POPBasicAnimation animation];
    // 限时 1s
    anim.duration = 3.0;
    POPAnimatableProperty * prop = [POPAnimatableProperty propertyWithName:@"count++" initializer:^(POPMutableAnimatableProperty *prop) {
        prop.readBlock = ^(id obj, CGFloat values[]){ values[0] = [[obj description] floatValue]; };
        prop.writeBlock = ^(id obj, const CGFloat values[])
        {
            [obj setText:[NSString stringWithFormat:@"%.2f",values[0]]];
        };
        prop.threshold = 0.01;
    }];
    anim.property = prop;
    anim.fromValue = @(0.0);
    anim.toValue = @(1314.52);
    [self.xt_countLabel pop_addAnimation:anim forKey:@"counting"];
```
![](http://upload-images.jianshu.io/upload_images/1506501-e8047812cc916073.gif?imageMogr2/auto-orient/strip)
XT_03
#### E3
```cpp
```cpp
CALayer *layer0 = [CALayer layer];
    layer0.opacity = 1.0;
    layer0.transform = CATransform3DIdentity;
    [layer0 setMasksToBounds:YES];
    [layer0 setBackgroundColor:[UIColor colorWithRed:0.5448 green:0.6836 blue:0.9986 alpha:1.0].CGColor];
    [layer0 setCornerRadius:12.5];
    [layer0 setBounds:CGRectMake(0, 0, 25, 25)];
    [self.view.layer addSublayer:layer0];
    layer0.position = CGPointMake(self.view.center.x, 266);
    [self performAnimation:layer0];
```
```
```
- (void)performAnimation:(CALayer *)layer
{
    [layer pop_removeAllAnimations];
    POPBasicAnimation *anim = [POPBasicAnimation animationWithPropertyNamed:kPOPLayerScaleXY];
    static BOOL ani = YES;
    if (ani) {
        anim.toValue = [NSValue valueWithCGPoint:CGPointMake(1.0, 1.0)];
    }else{
        anim.toValue = [NSValue valueWithCGPoint:CGPointMake(1.5, 1.5)];
    }
    ani = !ani;
    anim.completionBlock = ^(POPAnimation *anim, BOOL finished) {
        if (finished) { [self performAnimation:layer]; }
    };
    [layer pop_addAnimation:anim forKey:@"Animation"];
}
```
[Swift版本仿网易云音乐播放音乐动画效果-- 简书](http://www.jianshu.com/p/23b0fa948bcf)
[三分钟教你把代码托管到Github -- 简书](http://www.jianshu.com/p/1dab64e7c9ba)
[Swift 很强大的图表库-Charts使用 -- 简书](http://www.jianshu.com/p/0e025931a49a)
[Swift版仿简书App淘宝App很友好弹出view效果 -- 简书](http://www.jianshu.com/p/01a420681ca9)
[iOS UICollectionView高级用法“长按自由移动cell重排 -- 简书](http://www.jianshu.com/p/31d07bf32d62)
[iOS仿微信App添加带箭头的view -- 简书](http://www.jianshu.com/p/260ca284fda0)
[iOS仿美团外卖饿了吗App点餐动画 -- 简书](http://www.jianshu.com/p/d0ae176bc40b)
[你要的Swift资料 -- 简书](http://www.jianshu.com/p/4a1978845eac)
#### 微博@夏天是个大人了欢迎你关注我
你还可以加入我创建技术交流群: 498143780 与我交流. 欢迎你加入
文／夏天然后（简书作者）
原文链接：http://www.jianshu.com/p/0bc2127692e5
著作权归作者所有，转载请联系作者获得授权，并标注“简书作者”。
