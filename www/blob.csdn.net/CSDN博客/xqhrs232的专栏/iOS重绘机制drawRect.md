# iOS重绘机制drawRect - xqhrs232的专栏 - CSDN博客
2019年03月14日 16:20:38[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：16
个人分类：[Mac OS/i OS技术](https://blog.csdn.net/xqhrs232/article/category/1252162)
原文地址::[https://www.jianshu.com/p/4db5f01b258e](https://www.jianshu.com/p/4db5f01b258e)
相关文章
1、iOS 重绘之drawRect----[https://www.jianshu.com/p/93fc061433b2](https://www.jianshu.com/p/93fc061433b2)
2、iOS利用drawRect方法绘制图形----[https://www.jianshu.com/p/1aae00b4e440](https://www.jianshu.com/p/1aae00b4e440)
3、一张图看懂ios drawRect----[https://blog.csdn.net/shanglanxin/article/details/44595247](https://blog.csdn.net/shanglanxin/article/details/44595247)
4、iOS开发之drawRect的作用和调用机制 (转载)----[https://www.jianshu.com/p/6b808aba3a11](https://www.jianshu.com/p/6b808aba3a11)
5、[UIVIew]谈谈对drawRect的理解----[https://www.jianshu.com/p/7242bc413ca8](https://www.jianshu.com/p/7242bc413ca8)
OS的绘图操作是在UIView类的drawRect方法中完成的，所以如果我们要想在一个UIView中绘图，需要写一个扩展UIView 的类，或者写一个UIVIew的子类，并重写drawRect方法，在这里进行绘图操作，程序会自动调用此方法进行绘图。
下面先说明一下绘图，比如，你想绘制一个方块，你需要写一个类来扩展UIView并在drawRect方法中填入如下代码：
```
- (void)drawRect:(CGRect)rect {
    //获得处理的上下文
    CGContextRef context = UIGraphicsGetCurrentContext();
    //设置线条样式
    CGContextSetLineCap(context, kCGLineCapSquare);
    //设置线条粗细宽度
    CGContextSetLineWidth(context, 1.0);
    //设置颜色
    CGContextSetRGBStrokeColor(context, 1.0, 0.0, 0.0, 1.0);
    //开始一个起始路径
    CGContextBeginPath(context);
    //起始点设置为(0,0):注意这是上下文对应区域中的相对坐标，
    CGContextMoveToPoint(context, 0, 0);
    //设置下一个坐标点
    CGContextAddLineToPoint(context, 100, 100);
    //设置下一个坐标点
    CGContextAddLineToPoint(context, 0, 150);
    //设置下一个坐标点
    CGContextAddLineToPoint(context, 50, 180);
    //连接上面定义的坐标点
    CGContextStrokePath(context);
}
```
再说明一下重绘，重绘操作仍然在drawRect方法中完成，但是苹果不建议直接调用drawRect方法，当然如果你强直直接调用此方法，当然是没有效果的。苹果要求我们调用UIView类中的setNeedsDisplay方法，则程序会自动调用drawRect方法进行重绘。（调用setNeedsDisplay会自动调用drawRect）
例如：
定义一个TextField 继承于UITextField重写drawRect:方法如上，然后在ViewController去添加这么一个TextField，在初始化的时候不去设置它的frame,在touchesBegan：方法中去设置它的frame,然后调用setNeedsDisplay方法。在点击屏幕的时候，你会发现原本不出现的TextFiled重新出现在视图上
```
#import "ViewController.h"
#import "TextField.h"
@interface ViewController ()
@property (nonatomic,strong) TextField *textField;
@end
@implementation ViewController
- (void)viewDidLoad {
    [super viewDidLoad];
    self.textField = [[TextField alloc] init];
    [self.view addSubview:self.textField];
    self.view.backgroundColor = [UIColor whiteColor];
    self.textField.backgroundColor = [UIColor blueColor];
}
- (void)touchesBegan:(NSSet<UITouch *> *)touches withEvent:(UIEvent *)event
{
    self.textField.frame = CGRectMake(100, 100, 300, 300);
    [self.textField setNeedsDisplay];
}
@end
```
再举个自己工程中用到drawRect的例子：
还是定义一个TextField 继承于UITextField然后重写drawRect:方法如下:
```
- (void)drawRect:(CGRect)rect {
    CGContextRef context = UIGraphicsGetCurrentContext();
    CGContextSetFillColorWithColor(context, [UIColor grayColor].CGColor);
    CGContextFillRect(context, CGRectMake(0, CGRectGetHeight(self.frame) - 0.5, CGRectGetWidth(self.frame), 0.5));
}
```
然后在ViewController中代码如下
```
#import "ViewController.h"
#import "TextField.h"
@interface ViewController ()
@property (nonatomic,strong) TextField *userNameTF;
@property (nonatomic,strong) TextField *userPWDTF;
@end
@implementation ViewController
- (void)viewDidLoad {
    [super viewDidLoad];
    self.view.backgroundColor = [UIColor whiteColor];
    self.userNameTF = [[TextField alloc] initWithFrame:CGRectMake(100, 100, 200, 40)];
    self.userNameTF.placeholder = @"请输入用户名";
    [self.view addSubview:self.userNameTF];
    
    self.userPWDTF = [[TextField alloc] initWithFrame:CGRectMake(100, 160, 200, 40)];
    self.userPWDTF.placeholder = @"请输入密码";
    [self.view addSubview:self.userPWDTF];
}
@end
```
效果如图:
![](https://upload-images.jianshu.io/upload_images/1764181-66361a0f433c7ea3.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/312/format/webp)
屏幕快照 2016-04-23 下午4.23.56.png
