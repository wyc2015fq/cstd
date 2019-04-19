# 判断View是否显示在界面上 - 月若无涯的专栏 - CSDN博客
2015年07月24日 18:11:44[月若无涯](https://me.csdn.net/u010124617)阅读数：5797
我们都知道ViewController有viewWillAppear和viewDidAppear等关于页面生命周期的方法，用来对视图做一些管理，比如页面出现时怎么样，页面消失时怎么样。。
但是对于View，有没有类似的方法可以判断是否view出现了，view消失或者被遮挡了呢？
先介绍一下场景吧，朋友的一个项目，有个界面是直接做了个View，然后add到某个ViewController上去了（层级比较深，不方便在ViewController里操作这个view）。后来需求变动，需要这个View中做一些判断，该界面被其他vc遮挡后，再返回来时需要刷新这个view。 
正常的做法是通过vc来控制这个view，但是这次由于时间紧迫，项目也不方便改动，于是需要这个view自己实现相应处理。
上解决方案： 
对于view来说，有两个关于自身页面状态的方法
`-(void)willMoveToWindow:(UIWindow *)newWindow``- (void)willMoveToSuperview:(UIView *)newSuperview`
如何利用这些方法呢，经过测试
`-(void)willMoveToWindow:(UIWindow *)newWindow`
这方法在页面被添加到视图时会调用，且newWindow是存在的。 
当有新的vc遮挡时，这方法也会被调用（连续调用了两次，原因有待查明），但是newWindow是nil，利用这一点，可以简单判断页面的状态。
最后要重申一点，这种方法并非开发的正道，正确做法仍然是通过VC来管理页面。
