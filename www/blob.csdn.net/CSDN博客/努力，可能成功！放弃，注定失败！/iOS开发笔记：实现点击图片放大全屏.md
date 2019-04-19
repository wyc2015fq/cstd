# iOS开发笔记：实现点击图片放大全屏 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2016年04月10日 22:34:15[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：3591

最近遇到这个问题，点击图片，让图片全屏。参考了一些博客和源码，大概明白了是怎么实现这个效果的。
这里就简单说明一下：
原理和之前的[实现修改头像](http://www.jianshu.com/p/6da2bbbff26d)有点类似：修改头像的时候，我们给imageView添加一个手势事件，然后绑定一个方法。只不过之前绑定的方法是用来弹出提示框，而这里绑定的方法用来弹出一个新的视图，然后给视图添加一个黑色背景（view）和一张图片（即要放大的图片），这样就从视觉上实现了图片的放大。
![](http://upload-images.jianshu.io/upload_images/1469332-58dc0c0074440f97.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
原图
> 
PS：一口气补完了《一拳超人》，好困，现在已经是凌晨两点，明天还要早起上班，所以稍稍偷下懒，直接上代码
#### 完整代码如下:
```
#import "ViewController.h"
#import <QuartzCore/QuartzCore.h>
    //设置放大后图片的宽高，为了省时间，偷了下懒，建议最好结合实际做下运算
#define BIG_IMG_WIDTH  325
#define BIG_IMG_HEIGHT 325
@interface ViewController ()
@property (strong, nonatomic) IBOutlet UIImageView *minImageView;
@end
@implementation ViewController{
    UIView *background;
}
- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view, typically from a nib.
        //允许用户交互
    _minImageView.userInteractionEnabled = YES;
        //添加点击手势
    UITapGestureRecognizer *tapGesture = [[UITapGestureRecognizer alloc]initWithTarget:self action:@selector(tapAction)];
    [_minImageView addGestureRecognizer:tapGesture];
}
    //点击图片后的方法(即图片的放大全屏效果)
- (void) tapAction{
    //创建一个黑色背景
        //初始化一个用来当做背景的View。我这里为了省时间计算，宽高直接用的5s的尺寸
    UIView *bgView = [[UIView alloc]initWithFrame:CGRectMake(0, 0, 320, 600)];
    background = bgView;
    [bgView setBackgroundColor:[UIColor blackColor]];
    [self.view addSubview:bgView];
    //创建显示图像的视图
        //初始化要显示的图片内容的imageView（这里位置继续偷懒...没有计算）
    UIImageView *imgView = [[UIImageView alloc] initWithFrame:CGRectMake(0, 100, BIG_IMG_WIDTH, BIG_IMG_HEIGHT)];
        //要显示的图片，即要放大的图片
    [imgView setImage:[UIImage imageNamed:@"dog"]];
    [bgView addSubview:imgView];
    imgView.userInteractionEnabled = YES;
        //添加点击手势（即点击图片后退出全屏）
    UITapGestureRecognizer *tapGesture = [[UITapGestureRecognizer alloc]initWithTarget:self action:@selector(closeView)];
    [imgView addGestureRecognizer:tapGesture];
    [self shakeToShow:bgView];//放大过程中的动画
}
-(void)closeView{
    [background removeFromSuperview];
}
    //放大过程中出现的缓慢动画
- (void) shakeToShow:(UIView*)aView{
    CAKeyframeAnimation* animation = [CAKeyframeAnimation animationWithKeyPath:@"transform"];
    animation.duration = 0.5;
    NSMutableArray *values = [NSMutableArray array];
    [values addObject:[NSValue valueWithCATransform3D:CATransform3DMakeScale(0.1, 0.1, 1.0)]];
    [values addObject:[NSValue valueWithCATransform3D:CATransform3DMakeScale(1.0, 1.0, 1.0)]];
    animation.values = values;
    [aView.layer addAnimation:animation forKey:nil];
}
- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}
@end
```
![](http://upload-images.jianshu.io/upload_images/1469332-08952835eec34cdf.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
点击原图后的全屏效果
