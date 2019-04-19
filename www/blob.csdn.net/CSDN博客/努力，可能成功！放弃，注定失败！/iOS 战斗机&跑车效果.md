# iOS 战斗机&跑车效果 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2017年06月23日 17:15:30[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：205标签：[iOS 资源大全																[iOS学习资料](https://so.csdn.net/so/search/s.do?q=iOS学习资料&t=blog)](https://so.csdn.net/so/search/s.do?q=iOS 资源大全&t=blog)
个人分类：[iOS开发](https://blog.csdn.net/qq_30513483/article/category/6072895)
[本文来自简书，原文地址:http://www.jianshu.com/p/f6fdf467b7f0](http://www.jianshu.com/p/f6fdf467b7f0)
近来看到QQ音乐里面有一些酷酷的礼物效果，手痒，从网上找到一些素材，尝试做一下，效果有点粗糙，但是还是学到了一些东西，故和大家一起探讨分析一下。
先上跑车效果：
![](http://upload-images.jianshu.io/upload_images/711728-fdd5796fb8217a47.gif?imageMogr2/auto-orient/strip)
跑车.gif
然后是战斗机效果：
![](http://upload-images.jianshu.io/upload_images/711728-37b7985a8e97b5bb.gif?imageMogr2/auto-orient/strip)
飞机.gif
先说说这个跑车效果怎么做。
第一步：分析
跑车图片和轮子图片是一个整体，无论是放大还是移动，一定是同步的。
第二步：搭建
跑车图片上，放置前后轮子，轮子需要滚动，这里使用序列帧动画来无限循环播放轮子的图片，给用户一种滚动的效果。
![](http://upload-images.jianshu.io/upload_images/711728-0fdde08fc996c99c.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
车体原图.png
第三步：动画
这里使用关键帧动画来控制跑车的移动和大小。
我们需要注意的是跑车尽可能的在屏幕中间停留，所以要控制不同关键帧的动画时间。
动画操作：
```
- (void)startAnimation
{
    CAAnimationGroup *animationGroup = [CAAnimationGroup animation];
    //大小动画
    CAKeyframeAnimation *scaleAnimation = [self scaleKeyAnimation];
    //平移动画
    CAKeyframeAnimation *transAnmation = [self translationAnimation];
    animationGroup.animations = @[scaleAnimation,transAnmation];
    animationGroup.duration = self.animateDuration;
    [self.layer addAnimation:animationGroup forKey:nil];
}
```
接下来是战斗机的效果分析：
第一步：分析
等同于跑车的思路，战斗机为整体，平移操作即可。
第二步：搭建
战斗机的五个螺旋桨需要不停地旋转，所以这五个小部件都需要不停地做旋转动画。
![](http://upload-images.jianshu.io/upload_images/711728-adb4d9a5d3f8b1d5.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
战斗机主体.png
第三步：动画
这里的动画分为4个部分
1、战斗机出场动画
2、投弹动画
3、战斗机离场动画
4、钻石隐藏动画
我们需要理清楚动画的触发时机：
1、战斗机出场动画。
2、战斗机出场动画结束后，开始投弹动画。投弹动画为序列帧动画。
3、投弹动画结束后，战斗机离场动画与钻石隐藏动画同时开始。
动画操作：
```
- (void)startAnimation
{
    //如果正在爆炸，也就意味之前的动画没结束
    if (self.isBooming) {
        //把动画移除了
        [self.boomImageView.layer removeAnimationForKey:@"boomAnimation"];
        [self.boomImageView.layer removeAnimationForKey:@"boomOpacityAnimation"];
        //把爆炸视图隐藏了
        [self hideBoomImageView];
    }
    //不在爆炸状态
    self.isBooming = NO;
    self.boomImageView.hidden = YES;
    //把未完成的动画移除掉
    [self.planeImageView.layer removeAnimationForKey:@"planeAnimation"];
    [self.boomImageView.layer removeAnimationForKey:@"boomOpacityAnimation"];
    [self.planeShadowImageView.layer removeAnimationForKey:@"shadowAnimation"];
    //构建新的动画
    CAKeyframeAnimation *transAnmation = [self planeTranslationAnimation];
    CAKeyframeAnimation *shadowAnimation = [self shadowTranslationAnimation];
    //配置动画属性
    transAnmation.duration = self.showDuration;
    transAnmation.delegate = self;
    transAnmation.removedOnCompletion = NO;
    transAnmation.fillMode = kCAFillModeForwards;
    shadowAnimation.duration = self.showDuration;
    shadowAnimation.delegate = self;
    shadowAnimation.removedOnCompletion = NO;
    shadowAnimation.fillMode = kCAFillModeForwards;
    //添加动画
    [self.planeImageView.layer addAnimation:transAnmation forKey:@"planeAnimation"];
    [self.planeShadowImageView.layer addAnimation:shadowAnimation forKey:@"shadowAnimation"];
}
```
额外的注意事项是：当我动画未结束时，又重新开始了动画，那么需要根据动画的触发时机来处理各个动画。
该视图很多属性都公开了，为了保留足够的外部定制，调用起来会稍微复杂一点，但这样是值得的。
调用跑车：
```
- (void)loadCustomCarImageView
{
    //跑车位置
    car = [[CarImageView alloc]initCarImageViewWithOriginCenter:CGPointMake(-240, 0)];
    //跑车开始动画的位置
    car.startAnimatePoint = CGPointMake(0, 100);
    //跑车结束动画的位置
    car.endAnimatePoint = CGPointMake(600, 500);
    //跑车的尺寸大小
    car.controlScaleArray = @[@"0.5",@"1",@"1.2"];
    //跑车途径的点
    NSValue *onePoint = [NSValue valueWithCGPoint:CGPointMake(120, 150)];
    NSValue *twoPoint = [NSValue valueWithCGPoint:CGPointMake(180, 210)];
    NSValue *threePoint = [NSValue valueWithCGPoint:CGPointMake(240, 260)];
    car.controlPointArray = @[onePoint,twoPoint,threePoint];
    //移动时的帧动画时间
    car.pointTimeArray = @[@0,@0.15,@0.45,@0.7,@0.85,@1];
    //尺寸的帧动画时间
    car.scaleTimeArray = @[@0,@0.15,@0.45,@1];
    //整个动画的时间
    car.animateDuration = 5.0;
    [self.view addSubview:car];
}
```
调用战斗机：
```
- (void)loadPlaneView
{
    //初始化
    plane = [[PlaneView alloc]initPlaneView];
    //开始位置
    plane.startPoint = CGPointMake(PhoneScreen_WIDTH , 0);
    //展示时的位置，从右向屏幕中间飞，途径的点
    NSValue *onePoint = [NSValue valueWithCGPoint:CGPointMake(PhoneScreen_WIDTH-30, 50)];
    NSValue *twoPoint = [NSValue valueWithCGPoint:CGPointMake(PhoneScreen_WIDTH/2+50, PhoneScreen_HEIGHT/2-90)];
    //展示时的位置
    plane.showPointArray = @[onePoint,twoPoint];
    //展示时每一帧的时间，0-1之间
    plane.showTimeArray = @[@0,@0.15,@0.5,@1.0];
    //飞机离开时的，从屏幕中部向屏幕左侧飞
    NSValue *oneLeavePoint = [NSValue valueWithCGPoint:CGPointMake(PhoneScreen_WIDTH/2-150, PhoneScreen_HEIGHT/2)];
    NSValue *twoLeavePoint =[NSValue valueWithCGPoint:CGPointMake(-200, PhoneScreen_HEIGHT/2+100)];
    //飞机位置
    plane.leavePointArray = @[oneLeavePoint,twoLeavePoint];
    //每一帧的时间
    plane.leaveTimeArray = @[@0,@0.5,@1.0];
    //飞机的初始位置，屏幕右侧
    plane.startPoint = CGPointMake(PhoneScreen_WIDTH, 0);
    //飞机
    plane.stayPoint = CGPointMake(PhoneScreen_WIDTH/2, PhoneScreen_HEIGHT/2-70);
    //飞机出场时间（从最右侧到屏幕中央的时间）
    plane.showDuration = 3.0;
    //飞机里长时间（从屏幕中央到最左侧的时间）
    plane.leaveDuration = 2.0;
    //爆炸的时间，也是飞机在屏幕中央停留的时间
    plane.boomDuration = 3.0;
    [self.view addSubview:plane];
}
```
至此，战斗机和跑车动画都实现了～
抛砖引玉，期待能让小伙伴们有所收获，更期待各位大神的指点和建议。
整个项目因为有不少图片，所以有15M，需要的小伙伴请到[这里下载项目代码](https://github.com/Coolll/GiftProject)
