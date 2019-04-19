# iOS时钟动画实现 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2016年03月17日 09:15:29[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：1065
# 序言
笔者对动画是很钟情的，今天我们一起来学习学习如何通过Core Animation实现钟的秒针、分针和时针无限动画移动，与苹果手机上的世界闹钟中的秒针、分针和时针类似。通过观察，笔者感觉是动画来实现的，而不是定时针。
不过，这里提供了两种方式来实现：
- 
通过定时器实现刷新，与挂钟一样，移动没有动画效果
- 
通过Core Animation实现，与苹果的世界时钟一样，动画均匀地移动
# 效果图
![image](http://www.henishuo.com/wp-content/uploads/2016/03/clockdemo.gif)
# 关于时针、分针和秒针
这里我们为了更轻量一些，直接继承于UIView，而不是UIImageView。将图片直接给layer.contents就可以了，也就没有那么重了。
对于时针、分针和秒针，我们也是直接通过添加layer来实现的。下面的方法是用于生成这三种针的layer的，其中最关键的是设置锚点。
```
```
|123456789101112131415161718|-(CALayer*)layerWithBackgroundColor:(UIColor*)color size:(CGSize)size{CALayer*layer=[CALayerlayer];layer.backgroundColor=color.CGColor;layer.anchorPoint=CGPointMake(0.5,1);// 设置为中心layer.position=CGPointMake(self.frame.size.width/2,self.frame.size.height/2);// 时针、分针、秒针长度是不一样的layer.bounds=CGRectMake(0,0,size.width,size.height);// 加个小圆角layer.cornerRadius=4;[self.layer addSublayer:layer];returnlayer;}|
首先，position属性是决定子layer在父layer上的位置，默认为（0，0）。其次，anchorPoint属性是决定子layer上的哪个点会在position所指定的位置。好像很抽象啊，确实也很难理解。当年自觉coco2dx的时候，也是学习了好久才弄明白锚点。
可能大家不太懂锚点，推荐大家阅读：[锚点与position](http://www.cnblogs.com/wendingding/p/3800736.html)
现在，对于我们这里，我们要让position所指定的位置，也就是在时钟的正中间，那么锚点需要设置为（0.5，1）。这样所计算出来的子layer的初始位置（0.5 * layer.bounds.size.width, 1 * layer.bounds.size.height），这也就是我们所设置的正中间了。
如果这段话看不懂，请先阅读上面所推荐的文章，因此这个锚点确实不好理解。但是，要想做好动画，必须要先掌握好锚点与position的关系。
# 定时器实现
为了更好地说明，直接先上核心代码。下面先看看如何创建时针、分针、秒针：
```
```
|123456789101112131415161718192021222324252627282930|-(instancetype)initWithFrame:(CGRect)frame imageName:(NSString*)imageName{if(self=[super initWithFrame:frame]){UIImage*image=[UIImage imageNamed:imageName];self.layer.contents=(__bridgeid_Nullable)(image.CGImage);// hour layer set upself.hourLayer=[self layerWithBackgroundColor:[UIColorblackColor]                                               size:CGSizeMake(3,self.frame.size.width/2-40)];// 秒针与分针一样长self.minuteLayer=[self layerWithBackgroundColor:[UIColorblackColor]                                                 size:CGSizeMake(3,self.frame.size.width/2-20)];self.secondLayer=[self layerWithBackgroundColor:[UIColorredColor]                                                 size:CGSizeMake(1,self.frame.size.width/2-20)];self.secondLayer.cornerRadius=0;NSTimer*timer=[NSTimer scheduledTimerWithTimeInterval:1.0                                                      target:self                                                    selector:@selector(onTimerUpdate:)                                                    userInfo:nil                                                     repeats:YES];[[NSRunLoopcurrentRunLoop] addTimer:timer forMode:NSRunLoopCommonModes];_timer=timer;[selfupdateUI];}returnself;}|
这里创建了定时器，通过定时器定时地去刷新UI，所以没有动画的过程。下面是更新UI显示的方法，主要是计算这个角度的问题：
```
```
|12345678910111213141516171819202122232425|-(void)updateUI{NSCalendar*calender=[NSCalendarcurrentCalendar];NSDateComponents*date=[calender components:NSCalendarUnitSecond|NSCalendarUnitMinute|NSCalendarUnitHour                                       fromDate:[NSDatedate]];NSIntegersecond=date.second;NSIntegerminute=date.minute;NSIntegerhour=date.hour;CGFloatperHourMove=1.0/12.*360.0;CGFloathourAngle=hour*perHourMove+minute*(1.0/60.0)*perHourMove;self.hourLayer.transform=CATransform3DMakeRotation(kAngleToRadion(hourAngle),0,0,1);// 一分钟就是一圈，也就是每秒走度CGFloatminuteAngle=minute*360.0/60.0;self.minuteLayer.transform=CATransform3DMakeRotation(kAngleToRadion(minuteAngle),0,0,1);CGFloatsecondAngle=second*360.0/60.0;self.secondLayer.transform=CATransform3DMakeRotation(kAngleToRadion(secondAngle),0,0,1);}|
- 
每秒钟时针转的度数：1.0 / 12.0 * 360.0，因为一圈有12个小时，共360度，所以一小时就是这么多度
- 
每秒钟分针转的度数：一分钟转一圈，正在是360度，所以每秒分针转360/60度
- 
每秒钟秒针转的度数：60秒转一圈，共360度，所以每秒转360/60度
最后，别忘了在需要的地方调用释放定时器：
```
```
|123|[self.clockViewreleaseTimer];|
# Core Animation实现时钟
其它代码与上面的采用定时器是差不多的，只是将定时器的部分改成了Core Animation部分：
```
```
|12345678910111213141516171819202122232425262728293031323334353637|// 添加秒针动画-(void)addSecondAnimationWithAngle:(CGFloat)angle{CABasicAnimation*animation=[CABasicAnimation animationWithKeyPath:@"transform.rotation.z"];animation.repeatCount=HUGE_VALF;animation.duration=60;animation.removedOnCompletion=NO;animation.timingFunction=[CAMediaTimingFunction functionWithName:kCAMediaTimingFunctionLinear];animation.fromValue=@(angle*M_PI/180);animation.byValue=@(2*M_PI);[self.secondLayer addAnimation:animation forKey:@"SecondAnimationKey"];}// 添加分针动画-(void)addMinuteAnimationWithWithAngle:(CGFloat)angle{CABasicAnimation*animation=[CABasicAnimation animationWithKeyPath:@"transform.rotation.z"];animation.repeatCount=HUGE_VALF;animation.duration=60*60;animation.removedOnCompletion=NO;animation.timingFunction=[CAMediaTimingFunction functionWithName:kCAMediaTimingFunctionLinear];animation.fromValue=@(angle*M_PI/180);animation.byValue=@(2*M_PI);[self.minuteLayer addAnimation:animation forKey:@"MinuteAnimationKey"];}// 添加时针动画-(void)addHourAnimationWithAngle:(CGFloat)angle{CABasicAnimation*animation=[CABasicAnimation animationWithKeyPath:@"transform"];animation.repeatCount=HUGE_VALF;animation.duration=60*60*60*12;animation.removedOnCompletion=NO;animation.timingFunction=[CAMediaTimingFunction functionWithName:kCAMediaTimingFunctionLinear];animation.fromValue=@(angle*M_PI/180);animation.byValue=@(2*M_PI);[self.hourLayer addAnimation:animation forKey:@"HourAnimationKey"];}|
这里要注意，我们刚开始就可能不是0角度，因此fromValue要从angle开始。另外，我们要保证一圈一圈地转，因此要使用byValue而不是toValue。
如果不太了解Core Animation，推荐大家阅读笔者曾经在公司所进行的一次分享的内容：[说说Core
 Animation](http://www.henishuo.com/core-animation/)，这里会有很多基本的概念及属性说明，也许能帮你快速了解动画。
- 
对于秒针动画：由于我们设置要转一圈，所以需要duration为60秒。
- 
对于分针动画：由于转一圈就是一小时，所以需要duration为60*60秒
- 
对于时针动画：由于转一圈就是12小时，所以需要duration为60*60*60*12秒
# 最后
本demo是看到[CocoaChina](http://code.cocoachina.com/view/129939)上的一个小demo效果，也想尝试一下。不过所提供demo的作者只写了通过定时器来实现的，而且代码比较乱一些。既然要玩，就要玩好一点点，所以在实现了定时器的方式的同时，笔者又想如何通过核心动画来实现，所以就有了下文了~
祝大家好运吧，相信这段核心动画的代码对很多人一定是很有用的。刚开始我还查了很多资料如何实现无限动画循环转圈圈呢，结果都不如人意。
最后，还是要靠自己去参悟。经过深入查看API，发现还有一个byValue属性。根据所说过的cocoa2dx，那里正好也有by开头的很多API，都是移动或者翻转多少而不是移动或者翻转到哪里。因为我们要转满一圈，而开始又不是从0开始，所以最好的方式还是使用byValue。
# 源代码
大家可以到GITHUB下载代码运行来看看，喜欢就给个Star支持一下！
[CoderJackyHuang：ClockAnimationDemo](https://github.com/CoderJackyHuang/ClockAnimationDemo)
# 推荐阅读
- [CABasicAnimation精讲](http://www.henishuo.com/cabasicanimation-introduce-in-detail/)
- [CAAnimation解读](http://www.henishuo.com/caanimation-indtroduce-in-detail/)
