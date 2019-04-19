# iOS扩大按钮的可点击范围 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2017年07月04日 23:34:11[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：611
# 本文来自简书原文地址:[http://www.jianshu.com/p/43c22fa3b42c](http://www.jianshu.com/p/43c22fa3b42c)
# 需求点
一般来说按钮的点击范围和按钮的大小是相等的，但是如果按钮很小，就会造成难以点击的情况，甚至有的时候按钮周围还有别的可点击区域，造成经常误点击的差体验。
# 实现方法
网上主要有几种实现的方法：
- 设置按钮的图片`setImage:`，然后将按钮的size设置得比图片大。
- 在按钮上面覆盖一层较大透明的`UIView`或`UIButton`，设置点击事件。
以上的方法本身局限性还是比较大的。第一条：如果按钮没有图片怎么办？第二条：会改变视图的层级。所以笔者从来没有试过。
笔者比较喜欢在WWDC 2012 Session 216视频中提到的一种解决方式:通过重写`UIButton`自身的
```
-(BOOL)pointInside:(CGPoint)point
 withEvent:(UIEvent*)event
```
方法。
那么现在让我们研究一下：
```
- (BOOL)pointInside:(CGPoint)point withEvent:(UIEvent*)event
{
   //获取当前button的实际大小
    CGRect bounds = self.bounds;
    //若原热区小于44x44，则放大热区，否则保持原大小不变
    CGFloat widthDelta = MAX(44.0 - bounds.size.width, 0);
    CGFloat heightDelta = MAX(44.0 - bounds.size.height, 0);
    //扩大bounds
    bounds = CGRectInset(bounds, -0.5 * widthDelta, -0.5 * heightDelta);
    //如果点击的点 在 新的bounds里，就返回YES
    return CGRectContainsPoint(bounds, point);
}
```
Apple的iOS人机交互设计指南中指出，按钮点击热区应不小于44x44pt，否则这个按钮就会让用户觉得“很难用”，所以在这段代码里，以44为峰值，如果宽度，或高度小于44，就要扩大按钮的区域，否则保持原有大小。
` bounds = CGRectInset(bounds, -0.5 * widthDelta, -0.5 * heightDelta);`
这段代码应该就是改变bouds的代码了。后两个参数笔者原来也不明白，后来经过研究，应该是指在左右方向和上下方向扩大或缩小的长度。正值为缩小，负值为扩大。
让我们通过Demo来具体做一下点击范围扩大和缩小的对比：
# Demo
### 效果图：
![](http://upload-images.jianshu.io/upload_images/859001-01dce076111bd83b.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
蓝色：按钮 | 绿色：可点击范围
图中，上两个蓝色的正方形就是按钮，一大一小。绿色的正方形是二者各自可点击的区域。笔者在这里将第一个小按钮的点击范围扩大了，将第二个大按钮的点击范围缩小了。
那么如何得知按钮点击的范围呢？
### 代码分析：
第一个按钮的重写：
```
- (BOOL)pointInside:(CGPoint)point withEvent:(UIEvent*)event
{
    CGRect bounds = self.bounds;
    bounds = CGRectInset(bounds, -100, -100);
     return CGRectContainsPoint(bounds, point);
}
```
笔者将该按钮的点击范围上下左右同时扩大了100，因此，`点击范围的宽度 = 按钮的宽度 + 100*2`;
因此，按钮的宽度是30，那么点击范围的宽度就是230，而且显然，二者的center应该是同一个。
同理，第二个按钮的重写：
```
- (BOOL)pointInside:(CGPoint)point withEvent:(UIEvent*)event
{
    CGRect bounds = self.bounds;
    bounds = CGRectInset(bounds, 20, 20);
    return CGRectContainsPoint(bounds, point);
}
```
第二个按钮的宽度为100，那么它的可点击宽度就被“缩”成了`60（100 - 2*20）`。
为了演示方便，本Demo修改的是正方形按钮的点击范围，而且点击范围在横向和纵向的长度也都是一样的。当然这种方法同样可以适用于长方形，而且扩大或缩小的点击范围在横向和纵向的长度也可以不一样，请读者自行尝试。
笔者非常喜欢这种方法，只要继承`UIButton`类，重写这个方法就可以任意改变按钮的可点击区域，而且不依赖是否有图片，也不改变视图的层级。
