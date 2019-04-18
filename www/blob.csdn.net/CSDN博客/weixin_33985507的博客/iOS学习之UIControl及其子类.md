# iOS学习之UIControl及其子类 - weixin_33985507的博客 - CSDN博客
2016年05月03日 21:22:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：9
今天为大家总结UIControl及其子类的一些常用属性.
代码全在ViewController.m里.
我习惯先在-(void)viewDidLoad里修改下self.view.backgroundColor, 一般改为白色.
接下来介绍一些常用控件的属性, 代码全在-(void)loadView里.
## UISwitch : 开关控件
```
UISwitch *s = [[UISwitch alloc] initWithFrame:CGRectMake(100, 200, 1000, 1000)];//宽和高是已经写好了的,想自定义只能重写
//常用属性:
//(1)外框颜色:
s.tintColor = [UIColor blueColor];
//(2)打开状态下的底色:
s.onTintColor = [UIColor redColor];
//(3)圆圈按钮的颜色:
s.thumbTintColor = [UIColor yellowColor];
//(4)设置开关状态:(on)
//    [s setOn:YES];
[s setOn:YES animated:YES];//是否使用动画效果
//(5)为开关按钮添加触发方法:
[s addTarget:self action:@selector(didClickedSwitch:) forControlEvents:UIControlEventValueChanged];
[self.view addSubview:s];
```
## UISlider : 滑块控件
```
//初始化imageView:
self.imageView = [[UIImageView alloc] initWithFrame:CGRectMake(50, 50, 166, 144)];
NSMutableArray *imageArray = [NSMutableArray array];
for (NSInteger i = 0; i <= 21; i++) {
    NSString *imageName = [NSString stringWithFormat:@"Zombie%ld.tiff", i];
    UIImage *image = [UIImage imageNamed:imageName];
    [imageArray addObject:image];
}
self.imageView.animationImages = imageArray;
self.imageView.animationDuration = 0.5;
[self.imageView startAnimating];//开始动画
[self.view addSubview:_imageView];
[_imageView release];
UISlider *slider = [[UISlider alloc] initWithFrame:CGRectMake(20, 300, 335, 50)];
//常用属性:
//(1)设置值的范围:
slider.minimumValue = 0;
slider.maximumValue = 2;
//(2)添加滑动触发方法:
[slider addTarget:self action:@selector(didSlideSlider:) forControlEvents:UIControlEventValueChanged];//值发生改变就触发!
//(3)设置初始值:
slider.value = 50.0f;
//(4)修改划过区域的颜色:
slider.minimumTrackTintColor = [UIColor redColor];
//(5)未划过区域的颜色:
slider.maximumTrackTintColor = [UIColor yellowColor];
//(6)设置滑块图片:
//    [slider setThumbImage:[UIImage imageNamed:@"3.jpg"] forState:UIControlStateNormal];
//(7)设置滑条左右两边图片:
//    [slider setMinimumValueImage:[UIImage imageNamed:@"1.png"]];
//    [slider setMaximumValueImage:[UIImage imageNamed:@"2.png"]];
[self.view addSubview:slider];
```
## UISegmentControl : 分段控件
```
//初始化三个视图:
//左边:
self.leftView = [[UIView alloc] initWithFrame:CGRectMake(20, 100, 335, 500)];
self.leftView.backgroundColor = [UIColor redColor];
[self.view addSubview:_leftView];
//中间:
self.middleView = [[UIView alloc] initWithFrame:self.leftView.frame];
self.middleView.backgroundColor = [UIColor blueColor];
[self.view addSubview:_middleView];
//右边:
self.rightView = [[UIView alloc] initWithFrame:self.middleView.frame];
self.rightView.backgroundColor = [UIColor greenColor];
[self.view addSubview:_rightView];
//将左视图移动到最上面:
[self.view bringSubviewToFront:_leftView];
UISegmentedControl *seg = [[UISegmentedControl alloc] initWithItems:@[@"左边", @"中间", @"右边"]];
seg.frame = CGRectMake(20, 50, 335, 50);
//为seg添加点击事件:
[seg addTarget:self action:@selector(didClickedSegmentedControl:) forControlEvents:UIControlEventValueChanged];
//(1)修改渲染色:
seg.tintColor = [UIColor redColor];
//(2)设置某个分区为图片
UIImage *img = [UIImage imageNamed:@"5.jpg"];
img = [img imageWithRenderingMode:UIImageRenderingModeAlwaysOriginal];//取消图片的渲染效果!
[seg setImage:img forSegmentAtIndex:1];
//(3)修改指定下标的title:
[seg setTitle:@"dog左边" forSegmentAtIndex:0];
//(4)修改点击状态(点击后变回原来的颜色)
seg.momentary = NO;//默认是NO
//(5)设置某个下标的分段不可点击:
[seg setEnabled:YES forSegmentAtIndex:0];
//    [seg setEnabled:NO];//整个控件失效
//(6)添加一个新的分段:
[seg insertSegmentWithTitle:@"NBA" atIndex:1 animated:NO];
//(7)移除某个分段:
[seg removeSegmentAtIndex:1 animated:NO];
//(8)设置默认选中的下标:
[seg setSelectedSegmentIndex:0]; 
[self.view addSubview:seg];
```
因为分段控件这里用了三个视图, 需要将leftView, middleView和rightView三个视图写为属性, 如下.
#import "ViewController.h"
@interface ViewController ()
@property(nonatomic, retain)UIImageView *imageView;
@property(nonatomic, retain)UIView *leftView;
@property(nonatomic, retain)UIView *middleView;
@property(nonatomic, retain)UIView *rightView;
@end
## UIPageControl : 分页控制
```
UIPageControl *pageControl = [[UIPageControl alloc] initWithFrame:CGRectMake(100, 567, 175, 50)];
pageControl.backgroundColor = [UIColor grayColor];
//(1)设置小点的个数:
pageControl.numberOfPages = 5;
//(2)为其添加触发方法:
[pageControl addTarget:self action:@selector(didClickedPageControl:) forControlEvents:UIControlEventValueChanged];
//设置小点默认的颜色:
pageControl.pageIndicatorTintColor = [UIColor blueColor];
//设置被选中的小点颜色:
pageControl.currentPageIndicatorTintColor = [UIColor yellowColor];
[self.view addSubview:pageControl];
```
常用属性在loadView里写, 里边一些控件涉及一些方法, 要写在loadView外. 接下来是上边使用到的方法. 使用地点请看备注.
```
#pragma mark -- 开关触发方法:
-(void)didClickedSwitch:(UISwitch *)sender
{
if (sender.on == YES) {
    NSLog(@"链接wifi");
}
else
{
    NSLog(@"关闭wifi");
}
}
```
```
#pragma mark -- 滑块控件触发方法:
-(void)didSlideSlider:(UISlider *)sender
{
//获取滑块当前值:
NSLog(@"%.2f", sender.value);
//设置僵尸动画的时间间隔:
self.imageView.animationDuration = sender.maximumValue - sender.value;
[self.imageView startAnimating];
}
```
```
#pragma mark -- 分段控件的触发方法:
-(void)didClickedSegmentedControl:(UISegmentedControl *)seg
{
NSLog(@"点击了第%ld个分区", seg.selectedSegmentIndex);
//显示相应分区视图:
switch (seg.selectedSegmentIndex) {
    case 0:
        [self.view bringSubviewToFront:_leftView];
        break;
    case 1:
        [self.view bringSubviewToFront:_middleView];
        break;
    case 2:
        [self.view bringSubviewToFront:_rightView];
        break;
        
    default:
        break;
}
}
```
```
#pragma mark -- 分页控件触发方法:
-(void)didClickedPageControl:(UIPageControl *)pageControl
{
NSLog(@"当前选中的是第%ld张图", pageControl.currentPage);
}
```
其实我也刚学不久, 上边都是自己的学习笔记, 适合一些初学的朋友们, 希望大家多提意见, 也希望能和大家一起交流, 共同进步.
