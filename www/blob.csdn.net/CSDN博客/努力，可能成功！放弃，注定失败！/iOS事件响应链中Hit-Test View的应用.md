# iOS事件响应链中Hit-Test View的应用 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2017年03月14日 10:22:34[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：682
本文来自简书，原文地址:h[ttp://www.jianshu.com/p/d8512dff2b3e](http://www.jianshu.com/p/d8512dff2b3e)
最近又看了遍苹果的官方文档[《Event
 Handling Guide for iOS》](https://developer.apple.com/library/ios/documentation/EventHandling/Conceptual/EventHandlingiPhoneOS/event_delivery_responder_chain/event_delivery_responder_chain.html)，对事件响应链中的hit-test view 又多了些理解，个人觉的官方文档对这块讲的非常简单，很多东西都是点到为止，hit-test view的知识在项目的任何地方都用到了，但自己反而感知不到，接下来我会给大家讲hit-test view的项目中能解决痛点的三个应用
 。
什么叫 hit-test view？文档说：The lowest view in the view hierarchy that contains the touch point becomes the hit-test view，我的理解是：当你点击了屏幕上的某个view，这个动作由硬件层传导到操作系统，然后又从底层封装成一个事件（Event）顺着view的层级往上传导，一直要找到含有这个点击点且层级最高（文档说是最低，我理解是逻辑上最靠近手指）的view来响应事件，这个view就是hit-test
 view。
文档中说，决定谁hit-test view是通过不断递归调用view中的 - (UIView *)hitTest: withEvent: 方法和 -(BOOL)pointInside: withEvent: 方法来实现的，文段中的这段话太好理解，于是我仿照官方文档中这张图做了个Demo -> [Github地址](https://github.com/slemon/HitTestViewUsage)
![](http://upload-images.jianshu.io/upload_images/144142-3afc2c85792ade41.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
apple doucument pic
重载图中view的方法添加相应的log便于观察：
```
//in every view .m overide those methods
- (UIView *)hitTest:(CGPoint)point withEvent:(UIEvent *)event {
    NSLog(@"进入A_View---hitTest withEvent ---");
    UIView * view = [super hitTest:point withEvent:event];
    NSLog(@"离开A_View--- hitTest withEvent ---hitTestView:%@",view);
    return view;
}
- (BOOL)pointInside:(CGPoint)point withEvent:(nullable UIEvent *)event {
    NSLog(@"A_view--- pointInside withEvent ---");
    BOOL isInside = [super pointInside:point withEvent:event];
    NSLog(@"A_view--- pointInside withEvent --- isInside:%d",isInside);
    return isInside;
}
- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event
{
    NSLog(@"A_touchesBegan");
}
- (void)touchesMoved:(NSSet<UITouch *> *)touches withEvent:(nullable UIEvent *)event {
    NSLog(@"A_touchesMoved");
}
- (void)touchesEnded:(NSSet<UITouch *> *)touches withEvent:(nullable UIEvent *)event {
    NSLog(@"A_touchesEnded");
}
```
点击图中View_D，看下会发生什么
![](http://upload-images.jianshu.io/upload_images/144142-9850f716526bb4e8.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
View_D log
一是发现touchesBegan、touchesMoved、touchesEnded这些方法都是发生在找到hit-test view之后，因为touch事件是针对能响应事件的确定的某个view，比如你手指划出了scrollview的范围，只要你不松手继续滑动，scrollview依然会响应滑动事件继续滚动；二是寻找hit-test view的事件链传导了两遍，而且两次的调用堆栈是不同的，这点我有点搞不懂，为啥需要两遍，查阅了很多资料也不知道原因，发现真机和模拟器以及不同的系统版本之间还会有些区别（此为真机iOS9），大家可以下载我的[Demo](https://github.com/slemon/HitTestViewUsage)进行测试与研究。
把这个寻找的逻辑换成代码如下:
```
- (UIView *)hitTest:(CGPoint)point withEvent:(UIEvent *)event {
    if (!self.isUserInteractionEnabled || self.isHidden || self.alpha <= 0.01) {
        return nil;
    }
    if ([self pointInside:point withEvent:event]) {
        for (UIView *subview in [self.subviews reverseObjectEnumerator]) {
            CGPoint convertedPoint = [subview convertPoint:point fromView:self];
            UIView *hitTestView = [subview hitTest:convertedPoint withEvent:event];
            if (hitTestView) {
                return hitTestView;
            }
        }
        return self;
    }
    return nil;
}
```
如果有某个view的两个子view位置重叠，根据[View
 Programming Guide for iOS](https://developer.apple.com/library/ios/documentation/WindowsViews/Conceptual/ViewPG_iPhoneOS/WindowsandViews/WindowsandViews.html#//apple_ref/doc/uid/TP40009503-CH2-SW24)文档中说的 Visually, the content of a subview obscures all or part of the content of its parent view. If the subview is totally opaque, then the area occupied by the subview completely obscures the corresponding area
 of the parent. If the subview is partially transparent, the content from the two views is blended together prior to being displayed on the screen. Each superview stores its subviews in an ordered array and the order in that array also affects the visibility
 of each subview. If two sibling subviews overlap each other, the one that was added last (or was moved to the end of the subview array) appears on top of the other. 那最高层（逻辑最靠近手指的）view是view subviews数组的最后一个元素，只要寻找是从数组的第一个元素开始遍历，hit-test view的逻辑依然是有效的。
找到hit-test view后，它会有最高的优先权去响应逐级传递上来的Event，如它不能响应就会传递给它的superview，依此类推，一直传递到UIApplication都无响应者，这个Event就会被系统丢弃了。
## Hit-test view的应用举例：
### 1、扩大UIButton的响应热区
相信大家都遇到小图button点击热区太小问题，之前我是用UIButton的setImage方法来设置图片解决，但是调起坐标就坑了，得各种计算不说，写出的代码还很难看不便于维护，如果我们用用hit-test view的知识你就能轻松地解决这个问题。
重载UIButton的-(BOOL)pointInside: withEvent:方法，让Point即使落在Button的Frame外围也返回YES。
```
//in custom button .m
//overide this method
- (BOOL)pointInside:(CGPoint)point withEvent:(nullable UIEvent *)event {
    return CGRectContainsPoint(HitTestingBounds(self.bounds, self.minimumHitTestWidth, self.minimumHitTestHeight), point);
}
CGRect HitTestingBounds(CGRect bounds, CGFloat minimumHitTestWidth, CGFloat minimumHitTestHeight) {
    CGRect hitTestingBounds = bounds;
    if (minimumHitTestWidth > bounds.size.width) {
        hitTestingBounds.size.width = minimumHitTestWidth;
        hitTestingBounds.origin.x -= (hitTestingBounds.size.width - bounds.size.width)/2;
    }
    if (minimumHitTestHeight > bounds.size.height) {
        hitTestingBounds.size.height = minimumHitTestHeight;
        hitTestingBounds.origin.y -= (hitTestingBounds.size.height - bounds.size.height)/2;
    }
    return hitTestingBounds;
}
```
### 2、子view超出了父view的bounds响应事件
项目中常常遇到button已经超出了父view的范围但仍需可点击的情况，比如自定义Tabbar中间的大按钮，如下图闲鱼的app，点击超出Tabbar bounds的区域也需要响应，此时重载父view的-(UIView *)hitTest: withEvent:方法，去掉点击必须在父view内的判断，然后子view就能成为 hit-test view用于响应事件了。
![](http://upload-images.jianshu.io/upload_images/144142-3b8eacb1afb47c93.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
xiansyu
```
- (UIView *)hitTest:(CGPoint)point withEvent:(UIEvent *)event {
    if (!self.isUserInteractionEnabled || self.isHidden || self.alpha <= 0.01) {
        return nil;
    }
    /**
     *  此注释掉的方法用来判断点击是否在父View Bounds内，
     *  如果不在父view内，就会直接不会去其子View中寻找HitTestView，return 返回
     */
//    if ([self pointInside:point withEvent:event]) {
        for (UIView *subview in [self.subviews reverseObjectEnumerator]) {
            CGPoint convertedPoint = [subview convertPoint:point fromView:self];
            UIView *hitTestView = [subview hitTest:convertedPoint withEvent:event];
            if (hitTestView) {
                return hitTestView;
            }
        }
        return self;
//    }
    return nil;
}
```
### 3、ScrollView page滑动
这是app store 应用的app封面预览功能
![](http://upload-images.jianshu.io/upload_images/144142-cee9b6532795a034.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
scrollview page
上图的交互常常见于很多海报、封面展示的app，实现这个交互的方法有很多，但选择用scrollView来横向滑动来做是最简单的，让scrollview.pageEnabel = YES，就有了翻页的感觉，但这样scoreView的实际可滑动区域就只有一张照片那么宽，如果想让边侧留出的距离(蓝色框部分)响应滑动事件的话应该怎么办呢？这个时候又可以用到hit-test view的知识了，在scrollview的父view中把蓝色部分的事件都传递给scrollView就可以了，具体看下面代码：
```
//in scrollView.superView .m
- (UIView *)hitTest:(CGPoint)point withEvent:(UIEvent *)event {
    UIView *hitTestView = [super hitTest:point withEvent:event];
    if (hitTestView) {
        hitTestView = self.scrollView;
    }
    return hitTestView;
}
```
## 总结
事件响应链是UI层一个非常重要的概念，想做出非常棒的交互和动画，必须对其有一个深入的理解。我列举的只是我在开发中遇到的一些问题，如果有其他的对事件响应链的应用希望大家和我一起交流探讨。
