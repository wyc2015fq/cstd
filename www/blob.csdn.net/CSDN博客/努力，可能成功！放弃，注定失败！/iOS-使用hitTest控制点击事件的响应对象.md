# iOS-使用hitTest控制点击事件的响应对象 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2018年10月11日 09:10:57[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：54
> 
之前在文章[《iOS-实现映客首页TabBar和滑动隐藏NavBar和TabBar》](https://www.jianshu.com/p/72228667cd7a)中，提到了`hitTest`方法，但是没有详细说明，导致有童鞋不理解为什么要这么做，这几天把`hitTest`的资料整理了一下，在这里介绍一些，解开疑惑。
这篇文章，最终的目的就是解释如何让中间按钮超出`TabBar`部分响应点击事件。效果图如下：
![](https://upload-images.jianshu.io/upload_images/809937-14b4f239fba4c545.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/374/format/webp)
中间按钮超出了`TabBar`的区域效果图
这篇文章将围绕一下几个问题来讲：
- `hitTest`是什么
- `hitTest`的调用顺序是怎么样的
- `hitTest`和事件传递有什么关系
- `hitTest`是如何解决子视图超出其视图范围还是能响应触摸事件的
下面我们一个一个来看。
### 1. hitTest是什么
`hitTest:withEvent:`是`UIView`的一个方法，该方法会被系统调用，是用于在视图(`UIView`)层次结构中找到一个最合适的`UIView`来响应触摸事件。
### 2. hitTest的调用顺序是怎么样的
一个触摸事件事件传递顺序大致如下：
```
touch->UIApplication->UIWindow->UIViewController.view->subViews->...->view
```
1） 触摸事件传递顺序
- 当用户点击屏幕时，会产生一个触摸事件，系统会将该事件加入到由`UIApplication`管理的事件队列中
- `UIApplication`会从事件队列中取出最早的事件进行分发处理，先发送事件给应用程序的主窗口`UIWindow`
- 主窗口会调用其`hitTest:withEvent:`方法在视图(`UIView`)层次结构中找到一个最合适的`UIView`来处理触摸事件
2） hitTest调用顺序
以下`pointInside:withEvent:`简称为`pointInside`，`hitTest:withEvent:`简称为`hitTest`
`hitTest`的代码逻辑大致如下：
```
- (UIView *)hitTest:(CGPoint)point withEvent:(UIEvent *)event {
    //系统默认会忽略isUserInteractionEnabled设置为NO、隐藏、alpha小于等于0.01的视图
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
执行顺序如下：
- 首先在当前视图的`hitTest`方法中调用`pointInside`方法判断触摸点是否在当前视图内
- 若`pointInside`方法返回`NO`，说明触摸点不在当前视图内，则当前视图的`hitTest`返回`nil`，该视图不处理该事件
- 若`pointInside`方法返回`YES`，说明触摸点在当前视图内，则从最上层的子视图开始（即从`subviews`数组的末尾向前遍历），遍历当前视图的所有子视图，调用子视图的`hitTest`方法重复步骤`1-3`
- 直到有子视图的`hitTest`方法返回非空对象或者全部子视图遍历完毕
- 若第一次有子视图的`hitTest`方法返回非空对象，则当前视图的`hitTest`方法就返回此对象，处理结束
- 若所有子视图的`hitTest`方法都返回`nil`，则当前视图的`hitTest`方法返回当前视图本身，最终由该对象处理触摸事件
上面的流程，看着可能有点绕，我们来看下面一个例子
![](https://upload-images.jianshu.io/upload_images/809937-6ae4ec6095ab66e2.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/513/format/webp)
例子
上图中有5个`View`，红点为手指点击区域，`ViewA`为父视图，`ViewB`和`ViewC`为`ViewA`的子视图，`ViewD`和`ViewE`为`ViewC`的子视图。
***（这里假设所有`View`都可以响应点击事件，而且`ViewB`在`ViewC`上层，`ViewD`在`ViewE`上层，即`ViewB`的`addSubView：`执行在`ViewC`之后，`ViewD`的`addSubView：`执行在`ViewE`之后）***
当点击`ViewE`时，`hitTest`执行顺序如下：
先看看点击大致走向图如下，其中，✅部分为执行`pointInside`为`YES`部分，`X`部分执行`pointInside`为`NO`部分，最终`hitTest`返回`ViewE`
![](https://upload-images.jianshu.io/upload_images/809937-b4e80207d4db47f9.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/389/format/webp)
hitTest走向图
- 首先调用`ViewA`的`hitTest`方法，由于触摸点在其范围内，`pointInside`返回`YES`，遍历其子视图，依次调用`ViewB`和`ViewC`的`hitTest`方法
- 执行`ViewB`的`hitTest`方法，由于触摸点是不在`ViewB`内，其`pointInside`方法返回`NO`，`hitTest`返回`nil`
- 执行`ViewC`的`hitTest`方法，由于触摸点是在`ViewC`内，其`pointInside`方法返回`YES`，遍历其子视图，依次调用`ViewD`和`ViewE`的`hitTest`方法
- 执行`ViewD`的`hitTest`方法，由于触摸点是不在`ViewD`内，其`pointInside`方法返回`NO`，所以其`hitTest`返回`nil`
- 执行`ViewE`的`hitTest`方法，由于触摸点是在 `ViewE`内，其`pointInside`方法返回`YES`，由于其没有子视图了，其`hitTest`返回其本身
- 最终，由`ViewE`来响应该点击事件
### 3. hitTest和事件传递有什么关系
事件传递的的顺序和`hitTest`中`pointInside`返回为`YES`的视图的执行顺序是相反的。事件传递是从最上层的视图开始传递的，直到`UIApplication`。
拿我们上面的例子来说，`hitTest`执行的结果是`ViewE`来响应事件，但是如果`ViewE`并不处理该事件，则其需要把该事件进行传递给下一个响应者，这个时候，它会将事件抛给`ViewC`，如果`ViewC`也不处理事件，则其会将事件传递给`ViewA`，如果`ViewA`也不处理，则该事件就不响应了。
以下由苹果官方文档提供的事件传递图
![](https://upload-images.jianshu.io/upload_images/809937-2cd6e6d2ca042990.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/659/format/webp)
苹果官方提供的事件传递图
上图事件的传递流程如下：
- 首先，由`initial view`尝试来处理事件，如果它处理不了，则会将事件传递给他的父视图`View`
- `View`尝试处理该事件，如果其也处理不了，再传递给它的父视图`UIViewController.view`
- `UIViewController.view`尝试来处理该事件，如果处理不了，将把该事件传递给`UIViewController`
- `UIViewController`尝试处理该事件，如果处理不了，将把该事件传递给主窗口`Window`
- 主窗口`Window`尝试来处理该事件，如果处理不了，将传递给应用单例`Application`
- 如果应用单例`Application`也处理不了，则该事件将会被丢弃
### 4. hitTest是如何解决子视图超出其视图范围还是能响应触摸事件的
我们来看看下面的图，下图中中间按钮超出了`TabBar`的区域
![](https://upload-images.jianshu.io/upload_images/809937-14b4f239fba4c545.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/374/format/webp)
中间按钮超出了`TabBar`的区域效果图
我们通过`Xcode`中下图红框按钮来查看该页面的层级关系
![](https://upload-images.jianshu.io/upload_images/809937-0b964c09e9a52187.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/290/format/webp)
Xcode中层级查看按钮
我来看下这个图的层级关系
![](https://upload-images.jianshu.io/upload_images/809937-b45f81cac8122300.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/389/format/webp)
层级结构图
从以上图可以看出，`TabBar`和`UITableView`，共同的父类为`UILayoutContainerView`，而`TabBar`的层级，相对于`UITableView`高些，它和`UITransitionView`是同级的。
当我们点击中间按钮超出`TabBar`部分（“中间按钮超出了TabBar
的区域效果图”红框部分），系统是如何处理的呢？我们跳过`UIWindow`，直接从`UILayoutContainerView`开始调用`hitTest`。
先看看大致走向图，其中，✅部分为执行`pointInside`为`YES`部分，`X`部分执行`pointInside`为`NO`部分
![](https://upload-images.jianshu.io/upload_images/809937-fb7ce5e14317f23c.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/802/format/webp)
hitTest走向图
- 调用`UILayoutContainerView`的`hitTest`方法，由于是在其区域内，`pointInside`返回`YES`，再遍历其子视图，调用`hitTest`
- 先调用`TabBar`的`hitTest`方法，由于点击区域是在`TabBar`之外的，所以`pointInside`返回`NO`，`hitTest`返回`nil`，`TabBar`并不响应该事件
- 再调用`UITransitionView`的`hitTest`方法，在其区域内，递归调用子视图`hitTest`方法，直到调用`UITableView`在突出按钮后的`UITableViewCell`的`hitTest`返回，返回该`Cell`，最终由`Cell`响应该事件
**所以，系统默认的处理方式，超出`TabBar`区域，中间按钮是不响应该事件的，而是由其后视图响应。**
想要超出父视图区域响应点击事件，必须将走向图该为如下所示（其中，✅部分为执行`pointInside`为`YES`部分，`X`部分执行`pointInside`为`NO`部分）：
![](https://upload-images.jianshu.io/upload_images/809937-f16f7a8634c326c6.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/617/format/webp)
修改后的hitTest走向图
要让中间按钮响应点击超出`TabBar`按钮部分的点击事件，则需要重写`TabBar`的`hitTest`方法了，在执行`hitTest`方法时，判断点击区域在中间按钮的区域，则返回中间按钮，响应该事件，代码如下：
```
//重写hitTest方法，去监听中间按钮的点击，目的是为了让凸出的部分点击也有反应
- (UIView *)hitTest:(CGPoint)point withEvent:(UIEvent *)event {
    
    //判断当前手指是否点击到中间按钮上，如果是，则响应按钮点击，其他则系统处理
    //首先判断当前View是否被隐藏了，隐藏了就不需要处理了
    if (self.isHidden == NO) {
        
        //将当前tabbar的触摸点转换坐标系，转换到中间按钮的身上，生成一个新的点
        CGPoint newP = [self convertPoint:point toView:self.centerBtn];
        
        //判断如果这个新的点是在中间按钮身上，那么处理点击事件最合适的view就是中间按钮
        if ( [self.centerBtn pointInside:newP withEvent:event]) {
            return self.centerBtn;
        }
    }
    
    return [super hitTest:point withEvent:event];
}
```
### 童鞋的疑问
这里，之前童鞋有一个疑问：
**问：**直接在中间按钮中事件`hitTest`直接来响应点击事件，行不行呢？
**答：**答案当然是不行的，如果你看懂了这篇文章，那就知道答案了。如果不在`TabBar`中重写`hitTest`方法，系统是先调用`TabBar`的`hitTest`方法的，在调用该`hitTest`方法时，判断点击超出`TabBar`部分，不在其区域内，`pointInside`就返回`NO`了，`hitTest`直接返回`nil`，`TabBar`不能响应该事件，其子视图（中间按钮）也就没机会执行`hitTest`方法了。所以是不行的。
### 参考文章
- [Hit-Testing in iOS](http://smnh.me/hit-testing-in-ios/)
- [Event Delivery: The Responder Chain](https://developer.apple.com/library/content/documentation/EventHandling/Conceptual/EventHandlingiPhoneOS/event_delivery_responder_chain/event_delivery_responder_chain.html)
作者：HK_Hank
链接：https://www.jianshu.com/p/ca3cd5306668
來源：简书
简书著作权归作者所有，任何形式的转载都请联系作者获得授权并注明出处。
