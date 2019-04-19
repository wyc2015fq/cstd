# iOS给UIView添加延展使用与Button、uiview - xqhrs232的专栏 - CSDN博客
2019年04月19日 11:03:48[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：4
个人分类：[Mac OS/i OS技术](https://blog.csdn.net/xqhrs232/article/category/1252162)
原文地址::[https://www.jianshu.com/p/ca1270abe3b5](https://www.jianshu.com/p/ca1270abe3b5)
下面给大家做一个小小的演示代码，具体的工程中，大家在做设置了。。。
看一下代码调用
```
self.cutsView = [[UIView alloc] init];
    self.cutsView.X = 100;
    self.cutsView.Y = 100;
    self.cutsView.W = 50;
    self.cutsView.H = 200;
    self.cutsView.backgroundColor = [UIColor redColor];
    [self.view addSubview:self.cutsView];
```
如上述代码，我们在修改任何控件的一个x,y,w,h属性的时候都必须重现购置rect，太麻烦了。看看上面的代码，简单、快、方便、修改其中一个属性都不用在重新创建新的rect。并且是即刻生效。66666
UIView的属性是没有这些的，而且frame的属性是onlyready的，是不可修改的。
如果要是去修改就用延展，这是一种简单方式去修改UIView的大小和位置。
给UIView加扩展
就可以直接拿到你给对象添加属性。so easy！！！
代码实现如下
1：给uiview添加一个延展
.h文件如下
```
#import <UIKit/UIKit.h>
@interface UIView (custom)
@property (nonatomic, assign) CGFloat  X;
@property (nonatomic, assign) CGFloat  Y;
@property (nonatomic, assign) CGFloat  W;
@property (nonatomic, assign) CGFloat  H;
@end
```
.m文件如下
```
#import "UIView+custom.h"
@implementation UIView (custom)
-(void)setX:(CGFloat)X {
        CGRect origionRect = self.frame;
        CGRect newRect = CGRectMake(X, origionRect.origin.y, origionRect.size.width, origionRect.size.height);
        self.frame = newRect;
}
-(CGFloat)X {
    return self.frame.origin.x;
}
-(void)setY:(CGFloat)Y {
    CGRect origionRect = self.frame;
    CGRect newRect = CGRectMake(origionRect.origin.x, Y, origionRect.size.width, origionRect.size.height);
    self.frame = newRect;
}
-(CGFloat)Y {
    return self.frame.origin.y;
}
-(void)setW:(CGFloat)W {
    CGRect origionRect = self.frame;
    CGRect newRect = CGRectMake(origionRect.origin.x, origionRect.origin.y,W, origionRect.size.height);
    self.frame = newRect;
}
-(CGFloat)W {
    return self.frame.size.width;
}
-(void)setH:(CGFloat)H {
    CGRect origionRect = self.frame;
    CGRect newRect = CGRectMake(origionRect.origin.x, origionRect.origin.y,origionRect.size.width, H);
    self.frame = newRect;
}
-(CGFloat)H {
 return self.frame.size.height;
}
@end
```
在这里主要重写了变量的set和get方法。
2：获取当前控件的属性数据
那就可以直接取值了
```
self.cutsView.X
```
添加延展是利用了在oc中的控件继承图的关系。
