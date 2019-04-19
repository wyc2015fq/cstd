# iOS UIView Block 动画- (基础动画, 关键帧动画, 动画组) - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2018年06月14日 15:36:36[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：431

#### UIView本身对于基本动画和关键帧动画、转场动画都有相应的封装，在对动画细节没有特殊要求的情况下使用起来也要简单的多
#### 1、UIView Block 基础动画 ,转场动画+ 缩放
```
// UIView Block连续多个单动画 ,转场动画+ 缩放
- (void)blockBasicAnimation:(UIButton *)tap {
    
    [UIView transitionWithView:self.iconImgView duration:1.0 options:UIViewAnimationOptionTransitionFlipFromLeft animations:^{
        self.iconImgView.image = [UIImage imageNamed:@"pkliving_icon_follow2"];
    } completion:^(BOOL finished) {
        [UIView animateWithDuration:0.5 animations:^{
            self.redView.transform = CGAffineTransformMakeScale(0.1, 0.1);
            self.iconImgView.transform = CGAffineTransformMakeScale(0.1, 0.1);
        } completion:^(BOOL finished) {
            [self.redView removeFromSuperview];
            [UIView animateWithDuration:0.8 animations:^{
                self.backImgView.alpha = 1.0;
            }];
        }];
    }];
    
}
```
![](https://upload-images.jianshu.io/upload_images/2050812-9c06bc1b9c2f2392.gif?imageMogr2/auto-orient/strip%7CimageView2/2/w/188)
basic.gif
#### 2、关键帧动画（里面加上基础动画就是动画组了：关键帧动画+基础动画（旋转和alpha））
```
-(void)blockGroupAnimation:(UIButton *)sender {
    [UIView animateKeyframesWithDuration:5.0 delay:0.0 options:UIViewKeyframeAnimationOptionCalculationModeCubic animations:^{
        [UIView addKeyframeWithRelativeStartTime:0.0 relativeDuration:0.2 animations:^{
            _titleLab.center = CGPointMake(300, 600);
             _titleLab.alpha = 0.9;
//            _titleLab.transform = CGAffineTransformMakeRotation(M_PI_4);
        }];
        
        [UIView addKeyframeWithRelativeStartTime:0.2 relativeDuration:0.2 animations:^{
            _titleLab.center = CGPointMake(200, 550);
//            _titleLab.transform = CGAffineTransformMakeRotation(M_PI_2);
             _titleLab.alpha = 0.8;
        }];
        
        [UIView addKeyframeWithRelativeStartTime:0.4 relativeDuration:0.2 animations:^{
            _titleLab.center = CGPointMake(350, 500);
//            _titleLab.transform = CGAffineTransformMakeRotation(M_PI_4);
             _titleLab.alpha = 0.7;
        }];
        [UIView addKeyframeWithRelativeStartTime:0.6 relativeDuration:0.2 animations:^{
            _titleLab.center = CGPointMake(250, 450);
//            _titleLab.transform = CGAffineTransformMakeRotation(M_PI_2);
             _titleLab.alpha = 0.4;
        }];
        [UIView addKeyframeWithRelativeStartTime:0.8 relativeDuration:0.2 animations:^{
            _titleLab.center = CGPointMake(400, 400);
//            _titleLab.transform = CGAffineTransformMakeRotation(M_PI);
             _titleLab.alpha = 0.1;
        }];
    } completion:^(BOOL finished) {
        [_titleLab removeFromSuperview];
    }];
}
```
![](https://upload-images.jianshu.io/upload_images/2050812-bab1c889f799fd35.gif?imageMogr2/auto-orient/strip%7CimageView2/2/w/250)
grop.gif
#### 3、UIVIew block spring 动画
```
// UIView Spring动画
//     usingSpringWithDamping:(CGFloat)//震动效果，范围0~1，数值越小震动效果越明显
//      initialSpringVelocity:(CGFloat)//初始速度，数值越大初始速度越快
//                    options:(UIViewAnimationOptions)//动画的过渡效果
- (void)blockSpringAnimation:(UIButton *)sender {
    [UIView animateWithDuration:4.0 delay:0.0 usingSpringWithDamping:0.3 initialSpringVelocity:0.6 options:UIViewAnimationOptionCurveLinear animations:^{
         _ballImgView.center = CGPointMake(300, 350);
    } completion:^(BOOL finished) {
        NSLog(@"spring over");
        
    }];
}
```
![](https://upload-images.jianshu.io/upload_images/2050812-d5ae7eaf3dc0f3bd.gif?imageMogr2/auto-orient/strip%7CimageView2/2/w/184)
ball.gif
#### 4、逐帧动画
```
/**
 * 1、iOS的定时器NSTimer定时更新图片来达到逐帧动画的效果,缺点是定时器方法调用有时可能会因为当前系统执行某种比较占用时间的任务造成动画连续性出现问题。
 * 2、通过设置UIImageView的animationImages属性，然后调用它的startAnimating方法去播放这组图片，但UIImageView一次性加载大量图片的问题，中间的过程就不可控
 * 3、核心动画没有直接提供逐帧动画类型，但是却提供了用于完成逐帧动画的相关对象CADisplayLink.
 * CADisplayLink是一个计时器，但是同NSTimer不同的是，CADisplayLink的刷新周期同屏幕完全一致,是60次/秒.
 * 这样一来使用它完成的逐帧动画（又称为“时钟动画”）完全感觉不到动画的停滞情况。
 */
// 逐帧动画
- (void)setDisplayLayer:(UIButton *)sender {
    //创建图像显示图层
    _displayLayer=[[CALayer alloc]init];
    _displayLayer.bounds=CGRectMake(0, 0, 14, 14);
    _displayLayer.position=CGPointMake(160, 284);
    [self.view.layer addSublayer:_displayLayer];
    
    //由于鱼的图片在循环中会不断创建，而10张鱼的照片相对都很小
    //与其在循环中不断创建UIImage不如直接将10张图片缓存起来
    _images=[NSMutableArray array];
    for (int i=0; i<10; ++i) {
        NSString *imageName=[NSString stringWithFormat:@"gift_icon_asign%i",i+1];
        UIImage *image=[UIImage imageNamed:imageName];
        [_images addObject:image];
    }
    
    //定义时钟对象
    CADisplayLink *displayLink=[CADisplayLink displayLinkWithTarget:self selector:@selector(stepBystep)];
    //添加时钟对象到主运行循环
    [displayLink addToRunLoop:[NSRunLoop mainRunLoop] forMode:NSDefaultRunLoopMode];
}
- (void)stepBystep {
    static int s = 0;
    if (++s %10 == 0) {
        UIImage *image =_images[_index];
        _displayLayer.contents = (id)image.CGImage;
        _index = (_index+1) % IMG_COUNT;
    }
}
```
![](https://upload-images.jianshu.io/upload_images/2050812-bd7f76d7f3bf8fcc.gif?imageMogr2/auto-orient/strip%7CimageView2/2/w/144)
zhen.gif
#### 5、贝塞尔曲线
```
- (void)circleAnimationTypeOne
{
    
    //创建出CAShapeLayer
    self.shapeLayer = [CAShapeLayer layer];
    self.shapeLayer.frame = self.rect;
    self.shapeLayer.fillColor = [UIColor whiteColor].CGColor;
    
    //设置线条的宽度和颜色
    self.shapeLayer.lineWidth = self.lineWidth;
    self.shapeLayer.strokeColor = [UIColor lightGrayColor].CGColor;
    
    
    //创建出圆形贝塞尔曲线
    
    UIBezierPath* circlePath=[UIBezierPath bezierPathWithArcCenter:CGPointMake(self.rect.size.width / 2, self.rect.size.height / 2 )radius:self.rect.size.height / 2 startAngle:M_PI_2 endAngle:2.5*M_PI  clockwise:YES];
    
    //让贝塞尔曲线与CAShapeLayer产生联系
    self.shapeLayer.path = circlePath.CGPath;
    
    //添加并显示
    [self.layer addSublayer:self.shapeLayer];
    
    
    
    
    
    UIBezierPath* circlePath2=[UIBezierPath bezierPathWithArcCenter:CGPointMake(self.rect.size.width / 2, self.rect.size.height / 2 )radius:self.rect.size.height / 2 startAngle:M_PI_2 endAngle:2*M_PI*_haveFinished+M_PI_2 clockwise:YES];
    
    
    
    //创建出CAShapeLayer
    self.shapeLayer2 = [CAShapeLayer layer];
    self.shapeLayer2.frame = self.rect;
    
    self.shapeLayer2.fillColor = [UIColor clearColor].CGColor;
    
    //设置线条的宽度和颜色
    self.shapeLayer2.lineWidth = self.lineWidth;
    self.shapeLayer2.strokeColor =  kCColor(0,174,239).CGColor;
    
    //让贝塞尔曲线与CAShapeLayer产生联系
    self.shapeLayer2.path = circlePath2.CGPath;
    
    //添加并显示
    [self.layer addSublayer:self.shapeLayer2];
    
    
    UIView* view=[[UIView alloc]initWithFrame:CGRectMake(0, 0, 20, 20)];
    view.center = CGPointMake(self.rect.size.width / 2, self.rect.size.height);
    view.layer.cornerRadius=10;
    view.layer.masksToBounds=YES;
    view.backgroundColor= kCColor(0,174,239);
    _roundView=view;
    [self addSubview:view];
    
}
```
![](https://upload-images.jianshu.io/upload_images/2050812-97386806ea2514a2.gif?imageMogr2/auto-orient/strip%7CimageView2/2/w/408)
flowQuery.gif
[demo下载:](https://github.com/ztq123456/FlowQuery)
更多详情请参考：
[iOS动画，绝对够分量！](https://www.jianshu.com/p/9fa025c42261)
[iOS开发系列--让你的应用“动”起来](https://www.cnblogs.com/kenshincui/p/3972100.html)
作者：朝雨晚风
链接：https://www.jianshu.com/p/e91cda977804
來源：简书
简书著作权归作者所有，任何形式的转载都请联系作者获得授权并注明出处。
