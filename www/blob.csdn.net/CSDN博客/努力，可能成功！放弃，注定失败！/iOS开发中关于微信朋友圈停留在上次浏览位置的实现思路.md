# iOS开发中关于微信朋友圈停留在上次浏览位置的实现思路 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2018年12月14日 13:25:00[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：179
看这篇文章的人肯定使用过微信，不知道你有没有注意到一个现象：我们点击进入朋友圈浏览朋友圈，然后返回到“发现”主页面，在短时间内再次进入朋友圈会发现页面停留在了上一次浏览的位置。
iOS开发者都知道像朋友圈这样的页面都是每次进入的时候创建一个新的页面，当从该页面返回上一个页面的时候此页面就被销毁掉了。
那么如何实现微信的效果呢？
实现思路
1.记录朋友圈页面滑动的位置
2.不让朋友圈这个页面销毁（使用单例）
我采用的是第二种实现思路。先看效果
效果
![](https://upload-images.jianshu.io/upload_images/1154538-27a7a491691ee91e.gif?imageMogr2/auto-orient/strip%7CimageView2/2/w/599/format/webp)
0.0git.gif
核心代码
```
- (void)viewDidDisappear:(BOOL)animated{
[super viewDidDisappear:animated];
disappearTime = 0;
self.timer = [NSTimer scheduledTimerWithTimeInterval:1 repeats:YES block:^(NSTimer * _Nonnull timer) {
    disappearTime ++;
    NSLog(@"disappearTime:---%lu", disappearTime);
    if (disappearTime == 2) {
        [self.tableView setContentOffset:CGPointMake(self.tableView.contentOffset.x, -_y) animated:YES];
    }
}];
}
```
解释：在页面消失后到一定时间让Tableview滑动到顶部。
demo地址：***[https://gitee.com/liangsenliangsen/wechat_friends_circle_location](https://gitee.com/liangsenliangsen/wechat_friends_circle_location)
这里还要说一些单例，单例大家都熟悉，记得在最开始使用单例的时候都会重写alloc等一系列方法防止其他开发人员使用的时候让单例出现多份不同的实例。我看了IQKeyboardManager（也是单例）这个三方的写法，它并没有去重写那些方法，而是直接禁止用户去使用alloc方法。
![](https://upload-images.jianshu.io/upload_images/1154538-da453cd7d69323d5.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/966/format/webp)
0.0单例.png
决定今后就这样写单例了。
****本篇文章到这里就结束了，愿大家加班不多工资多，男同胞都有女朋友，女同胞都有男朋友。😊***
作者：梁森森
链接：https://www.jianshu.com/p/0bbdd752ca7b
來源：简书
简书著作权归作者所有，任何形式的转载都请联系作者获得授权并注明出处。
