# ios引导页简单易用(可满足一般设计的需求) - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2017年01月22日 16:18:10[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：645
> 
基本上每个app都有引导页，虽然现在这种demo已经比比皆是，但感觉都不全，所以自己整理了一个，只需要传入图片，就可以正常加载出来。由于UIPageControl的小圆点大小和颜色经常与UI设计的不相符，所以后面也会提到重写类方法，进行修改。
先看下效果(图片是在网上随便找的)
![](http://upload-images.jianshu.io/upload_images/2270690-6260e89862b1a3ef.gif?imageMogr2/auto-orient/strip)
Untitled.gif
把指导页图片传入guideImages中
```
- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions {
    NSArray* guideImages = @[@"welcomePage_1",@"welcomePage_2",@"welcomePage_3"];
    AppInstructionView* guide = [[AppInstructionView alloc]initWithFrame:CGRectMake(0, 0, ScreenWidth, ScreenHeight)];
    guide.guideImages = guideImages;
    [guide rewritePageControl];
    [self.window.rootViewController.view addSubview:guide];
    return YES;
}
```
如果小圆点不符合需求则在下面修改
currentColor传入当前圆点的颜色，nextColor传入其他的颜色，size表示大小
```
#pragma mark - 重写pageControl方法
-(void)rewritePageControl{
    _pc = [[CHPageControl alloc]initWithFrame:CGRectMake(_pageSize.width * 0.5, _pageSize.height - 50, 0,0) currentColor:COLOR(72.0, 160.0, 220.0, 1) nextColor:COLOR(99.0, 99.0, 99.0, 1) size:8];
    [_pc setBackgroundColor:[UIColor clearColor]];
    _pc.userInteractionEnabled=NO;
    [_pc setCurrentPage:0];
    [_pc setNumberOfPages:_guideImages.count];
    [self addSubview:_pc];
}
```
