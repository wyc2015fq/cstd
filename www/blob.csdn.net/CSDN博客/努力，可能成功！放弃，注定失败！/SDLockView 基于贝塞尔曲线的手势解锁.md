# SDLockView:基于贝塞尔曲线的手势解锁 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2016年09月23日 15:58:36[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：384
![](http://upload-images.jianshu.io/upload_images/1396375-7c33aa1b9badef51.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
#### 从造轮子到SDLockView
手势解锁这个三方在很早之前就存在了,只是以前是没有用到过,这两天中秋在家闲着没事的时候就写了一个关于手势解锁的三方SDLockView.其中SDLockView主要的功能有三种,分别是设置手势密码,修改手势密码,验证手势密码.接下来,我们就一睹为快SDLockView的效果图.
![](http://upload-images.jianshu.io/upload_images/1396375-49c322fce3c717f8.gif?imageMogr2/auto-orient/strip)
#### SDLockView的快速生成
有很多时候,我们需要快速生成SDLockView页面,这时候,对其,我们只需要简简单单的三步,就可以快速集成SDLockView.首先,我们要把Demo中的SDLockView文件夹拖入目标工程中.其中文件夹中包含了SDLockView实现类,UIImage+SizeUpdata(关于修改图片尺寸的延展),和两个图标.
![](http://upload-images.jianshu.io/upload_images/1396375-53ff82bd1e37c6e6.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
紧接着就是初始化我们的SDLockView对象,我们要选定它的LockViewType类型,LockViewType为一个枚举类型的数据.
```
typedef enum  {
    SetPassWordType,//设置密码类型界面
    UpdataPassWordType,//修改密码类型界面
    TestingPassWordType,//验证密码类型界面
} LockViewType;
```
选好类型之后,我们直接可以创建SDLockView页面了.
`SDLockView *view = [SDLockView initWithLockViewType:self.lockViewType];`
最后添加到我们的视图上即可.
`[self.view addSubview:view];`
#### SDLockView属性介绍
为了方便更多人定制属于自己的手势解锁,SDLockView暴露了许多实用的属性接口.属性主要分为基本用法,拓展用法设置,文字信息设置,提示信息设置四大模块.注释已经加的不能再详细了,再有什么问题,可以直接询问骚栋.谢谢.
基本用法
![](http://upload-images.jianshu.io/upload_images/1396375-e4ba36a44ca93b61.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
拓展用法设置
![](http://upload-images.jianshu.io/upload_images/1396375-ab272924c550f288.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
文字信息设置
![](http://upload-images.jianshu.io/upload_images/1396375-86a0c9ac35b283bc.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
提示信息设置
![](http://upload-images.jianshu.io/upload_images/1396375-776542401bb46820.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
#### SDLockView实现核心详解
SDLockView的实现核心其实就是贝塞尔曲线,但是我们要为绘制做大量的准备工作.首先我们通过touchesBegan、touchesMoved、touchesEnded三个方法把用户的触摸过的所有Button放置在selectedButtons数组中(要考虑到去重问题,使用isExistWithButton这个方法进行实现).代码如下
```
//touchesMoved中的去重问题
//判读是否存在,如果存在,则不存储.
if (![self isExistWithButton:button]) {
    [self.selectedButtons addObject:button];
 }
```
每一次调用touchesMoved方法的时候都让其重新绘制View页面,调用方法如下.
```
//重新绘制
[self setNeedsDisplay];
```
绘制过程其实就是调用了`- (void)drawRect:(CGRect)rect`这个方法,我们只需要方法
中绘制贝塞尔曲线以及提示文本信息即可.在绘制的过程中,我们只需要把selectedButtons中每一个Button的中心设置为线段的转折点即可.
```
- (void)drawRect:(CGRect)rect{
    NSMutableParagraphStyle *paragraphStyle = [[NSMutableParagraphStyle alloc] init];
    paragraphStyle.alignment = NSTextAlignmentCenter;//（两端对齐的）文本对齐方式：（左，中，右，两端对齐，自然）
    [self.promptMessage drawInRect:CGRectMake(0, self.distanceY - 40, KmainWidth, 40) withAttributes:@{
                 NSKernAttributeName: @8,
                 NSFontAttributeName : [UIFont systemFontOfSize:(self.wordSize?self.wordSize:17)],
                 NSForegroundColorAttributeName :(self.wordColor?self.wordColor:[UIColor blackColor]),
                 NSParagraphStyleAttributeName:paragraphStyle,
}];
    UIBezierPath *path = [UIBezierPath bezierPath];
    int i = 0;
    for (UIButton *button in self.selectedButtons) {
        if (i == 0) {
            [path moveToPoint:button.center];
        }else{
            [path addLineToPoint:button.center];
        }
        i++;
    }
    [self.lineColor set];
    [path setLineWidth:self.lineWidth];
    [path setLineJoinStyle:kCGLineJoinRound];
    [path stroke];
}
```
###### SDLockView的相关使用就说这么多了,如果您对SDLockView有任何疑义,欢迎提出,谢谢.最后附上SDLockView的github的传送门.
### [-->SDLockView的传送门](https://github.com/wang82426107/SDLockView)
