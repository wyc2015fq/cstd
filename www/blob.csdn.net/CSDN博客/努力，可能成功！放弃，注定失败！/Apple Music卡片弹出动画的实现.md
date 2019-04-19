# Apple Music卡片弹出动画的实现 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2017年07月05日 10:31:52[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：206标签：[iOS学习资料																[iOS 资源大全](https://so.csdn.net/so/search/s.do?q=iOS 资源大全&t=blog)](https://so.csdn.net/so/search/s.do?q=iOS学习资料&t=blog)
个人分类：[iOS开发](https://blog.csdn.net/qq_30513483/article/category/6072895)
[本文来自简书，原文地址:http://www.jianshu.com/p/d8689bf8fce4](http://www.jianshu.com/p/d8689bf8fce4)
作为程序员，不是在埋头苦干就是在埋头苦干的路上。碰上公司赶项目的时候更是如此。但是，再怎么忙休息的时候也要抽出时间来放放松。健身，音乐，旅游。学会管理自己的生活才能活得更轻松。
说了这么多该回到正题。最近在听音乐时发现Apple Music的卡片弹出动画很有趣，琢磨了一阵子就打算练练手。也顺便想把自定义动画内容整理整理。最终实现的效果图如下所示
![](http://upload-images.jianshu.io/upload_images/4838624-3681417d5c8d9190.gif?imageMogr2/auto-orient/strip)
AppleMusic.gif
在开发之前把一些自定义转场动画先过一下
在iOS开发中有一般会自定义两种转场动画
- 导航栏动画---需要有导航栏，把下个控制器替换为当前控制器
- modal 动画--- 把下个控制器的视图覆盖到当前的视图上面
## 导航栏动画
设置当前要跳转的控制器的代理
例如：
遵守协议UINavigationControllerDelegate
例如
`self.navigationController.delegate = self;`
实现代理方法：
```
- (id<UIViewControllerAnimatedTransitioning>)navigationController:(UINavigationController *)navigationController
if (operation == UINavigationControllerOperationPush) {
        return self.pushAnimation;
    }else if (operation == UINavigationControllerOperationPop){
        return self.popAnimation;
    }
    return nil;
}
```
在上述代理方法中返回遵守`UIViewControllerAnimatedTransitioning`协议的对象。
而返回的对象分为两类：
一个是实现push时的动画对象，另一个是实现pop的动画对象，或者一个同时实现两种动画方法的对象。
这两个对象都必须要遵守`UIViewControllerAnimatedTransitioning`协议并且要实现两个方法
- 
```
- (NSTimeInterval)transitionDuration:(nullable
 id <UIViewControllerContextTransitioning>)transitionContext;
```
- 
```
- (void)animateTransition:(id
 <UIViewControllerContextTransitioning>)transitionContext
```
第一个方法是返回动画的时长，第二个方法实现动画的效果。
# modal
在modal之前设置presented（被弹出）的控制器的transitioningDelegate代理 ，并设置modalPresentationStyle 为Custom，
而作为代理需要遵守协议
`UIViewControllerTransitioningDelegate`
和
`UIViewControllerAnimatedTransitioning`
对比导航栏的代理 ，present的代理也要遵守UIViewControllerAnimatedTransitioning，但是多了一个UIViewControllerTransitioningDelegate。
它和导航栏的代理方法相似，前者是返回要实现present和dismiss的代理，后者是返回要push和pop的代理。
## 动画的实现
所有的动画都是在`- (void)animateTransition:(id<UIViewControllerContextTransitioning>)transitionContext`里面进行。
获取当前的控制器presenting以及将要显示的控制器presented
`fromVC = [transitionContext viewControllerForKey:UITransitionContextFromViewControllerKey];`
`toVc = [transitionContext viewControllerForKey:UITransitionContextToViewControllerKey];`
`[transitionContext containerView]`返回的是容器视图，动画的实现就是要把两个控制器的view结合容器视图进行。
在iOS 8，两个控制器的视图可以通过下面的方法获取：
```
UIView *fromView =  [transitionContext viewForKey:UITransitionContextFromViewKey];
UIView *toView =  [transitionContext viewForKey:UITransitionContextToViewKey];
```
需要注意的是，上面两个方法在UIModalPresentationCustom 模式下的 Modal 转场 ，获取得到的将是 nil 。所以对与modal转场只能通过UITransitionContextToViewControllerKey获取fromView 和toView
### modal和导航栏动画的区别
- modal 不需要添加fromView到容器上，而导航栏动画需要自己添加上去
- 对于导航栏动画，动画结束后 系统会自动把fromView transitionContext容器中移除事，你也可以手动处理提前将 fromView移除。而modal动画则不会移除掉fromView.++所以一般的卡片动画如Apple Music的弹出动画，JD下单的卡片动画都可以通过用modal来自定义实++现。
## Apple Music动画的实现
有几个要注意的点：
- 底部的View会缩放，两个View都有圆角
- tableView和下滑手势之间手势有冲突
- 手指下滑上层的卡片有阻尼效果
- 上拉tableView到底部，再下滑tableView后放开，上层卡片会有弹簧的效果。
手势冲突的处理是在tableView里面实现
```
//实现后表示，允许多个手势识别器共同识别
- (BOOL)gestureRecognizer:(UIGestureRecognizer *)gestureRecognizer shouldRecognizeSimultaneouslyWithGestureRecognizer:(UIGestureRecognizer *)otherGestureRecognizer {
    return YES;
}
```
然后根据不同的手势场景进行处理。
阻尼效果的实现我是利用了sin函数，来决定卡片下滑的位移
`self.dampOffset = sin(W * point.y ) * dismissOffset;//简陋的阻尼效果`
