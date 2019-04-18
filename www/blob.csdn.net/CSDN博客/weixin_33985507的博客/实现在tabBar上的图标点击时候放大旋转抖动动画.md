# 实现在tabBar上的图标点击时候放大旋转抖动动画 - weixin_33985507的博客 - CSDN博客
2018年08月15日 15:05:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：20
![1600494-552e04475f997aa0.gif](https://upload-images.jianshu.io/upload_images/1600494-552e04475f997aa0.gif)
动画效果
- 动画分类实现代码
```
-(void)aq_addRoaAnimation
{
    [UIView animateWithDuration:0.3 animations:^{
        self.layer.transform = CATransform3DMakeRotation(M_PI, 0, 1, 0);
    }];
    dispatch_after(dispatch_time(DISPATCH_TIME_NOW, (int64_t)(0.3 * NSEC_PER_SEC)), dispatch_get_main_queue(), ^{
        [UIView animateWithDuration:0.70 delay:0 usingSpringWithDamping:1 initialSpringVelocity:0.2 options:UIViewAnimationOptionCurveEaseOut animations:^{
            self.layer.transform = CATransform3DMakeRotation(2 * M_PI, 0, 1,0);
        } completion:nil];
    });
}
- (void)aq_addShakeAnimation
{
    CGFloat angle=M_PI*0.05;
    [self aq_addShakeAnimation:1 andAngle:angle];
}
- (void)aq_addShakeAnimation:(NSInteger)repeatCount andAngle:(CGFloat)angle
{
    //需要实现的帧动画，这里根据需求自定义
    [self.layer removeAnimationForKey:@"shake"];
    CAKeyframeAnimation *animation = [CAKeyframeAnimation animation];
    animation.keyPath = @"transform.rotation";
    
    animation.values = @[@(-angle),@(angle),@(-angle)];
    animation.repeatCount = repeatCount;
    animation.calculationMode = kCAAnimationCubic;
    [self.layer addAnimation:animation forKey:@"shake"];
}
-(void)aq_addScaleAnimation:(CGFloat)scale
{
    [self.layer removeAnimationForKey:@"scale"];
    CAKeyframeAnimation *animation = [CAKeyframeAnimation animation];
    animation.keyPath = @"transform.scale";
    
    animation.values = @[@(0.8),@(scale),@(1.0f),@(0.8),@(scale),@(1.0f)];
    animation.duration = 0.5;
    animation.calculationMode = kCAAnimationCubic;
    [self.layer addAnimation:animation forKey:@"scale"];
}
-(void)aq_addRotationAnimation:(NSInteger)repeatCount andBegainAngle:(CGFloat)begainAngle andEndAngel:(CGFloat)endAngel
{
    [self.layer removeAnimationForKey:@"rotation"];
    
    CAKeyframeAnimation *animation = [CAKeyframeAnimation animation];
    animation.keyPath = @"transform.rotation";
    
    animation.values = @[@(begainAngle),@(endAngel)];
    animation.repeatCount = repeatCount;
    animation.speed = 1;
    animation.duration = 1;
    animation.calculationMode = kCAAnimationCubic;
    [self.layer addAnimation:animation forKey:@"rotation"];
}
```
- 在自定义的TabBarViewController 中添加代理并实现下面代理方法
```
-(void)tabBar:(UITabBar *)tabBar didSelectItem:(UITabBarItem *)item
{
    NSInteger index = 0;
    for (UIView *v  in tabBar.subviews) {
        if ([v isKindOfClass:NSClassFromString(@"UITabBarButton")]) {
            if (item.tag==index) {
                for (UIView *aView in v.subviews) {
                    if ([aView isKindOfClass:NSClassFromString(@"UITabBarSwappableImageView")]) {
                        if (index==0) {
                            [aView aq_addRoaAnimation];
                        }else if (index==3){
                            [aView aq_addShakeAnimation];
                        } else{
                            [aView aq_addScaleAnimation:1.2];
                        }
                        return;
                    }
                }
            }
            index++;
        }
    }
}
```
- OK 完成效果了
- swift 3.0 实现方法 刚开始研究这块不足之处还请指教
```
override func tabBar(_ tabBar: UITabBar, didSelect item: UITabBarItem) {
        self.selectedIndex = item.tag;
        var index = 0
        for v in self.tabBar.subviews {
            if v.isKind(of: NSClassFromString("UITabBarButton")!){
                if index == item.tag {
                    for aV in v.subviews {
                        if(aV.isKind(of: NSClassFromString("UITabBarSwappableImageView") ?? NSObject.self)){
                            //核心动画
                            if index == 0 {
                                aV.layer.removeAllAnimations()
                                let rotationAni = CAKeyframeAnimation()
                                rotationAni.keyPath = "transform.rotation"
                                let angle = Double.pi/8.0
                                rotationAni.values = [angle,-angle,angle]
                                rotationAni.repeatCount = Float(2)
                                rotationAni.calculationMode = kCAAnimationCubic
                                aV.layer.add(rotationAni, forKey: "rotation")
                            }else if index == 1 {
                                aV.layer.removeAllAnimations()
                                let scaleAnimation = CAKeyframeAnimation()
                                scaleAnimation.keyPath = "transform.scale"
                                scaleAnimation.values = [0.8,1.0,1.2,1.0]
                                scaleAnimation.repeatCount = Float(1)
                                scaleAnimation.calculationMode = kCAAnimationCubic
                                aV.layer.add(scaleAnimation, forKey: "scale")
                            }else {
                                UIView.animate(withDuration: 0.3, animations: {
                                    aV.layer.transform = CATransform3DMakeRotation(CGFloat(Double.pi), 0, 1, 0)
                                }) { (finished) in
                                }
                                // GCD 延时
                                DispatchQueue.main.asyncAfter(deadline: DispatchTime.now() + 0.3, execute: {
                                    UIView.animate(withDuration: 0.3, animations: {
                                        aV.layer.transform = CATransform3DMakeRotation(2 * CGFloat(Double.pi), 0, 1, 0)
                                    })
                                })
                            }
                            return
                        }
                    }
                }
                index += 1
            }
        }
    }
```
