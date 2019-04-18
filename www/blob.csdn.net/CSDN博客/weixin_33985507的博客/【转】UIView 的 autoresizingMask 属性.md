# 【转】UIView 的 autoresizingMask 属性 - weixin_33985507的博客 - CSDN博客
2012年03月20日 10:01:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：5
在 UIView 中有一个autoresizingMask的属性，它对应的是一个枚举的值（如下），属性的意思就是自动调整子控件与父控件中间的位置，宽高。
这个autoresizingMask属性其实就是IB中的右边窗口的操作，只是这里用代码实现
enum {
   UIViewAutoresizingNone                 = 0,
   UIViewAutoresizingFlexibleLeftMargin   = 1 << 0,
   UIViewAutoresizingFlexibleWidth        = 1 << 1,
   UIViewAutoresizingFlexibleRightMargin  = 1 << 2,
   UIViewAutoresizingFlexibleTopMargin    = 1 << 3,
   UIViewAutoresizingFlexibleHeight       = 1 << 4,
   UIViewAutoresizingFlexibleBottomMargin = 1 << 5
};
typedef NSUInteger UIViewAutoresizing;
分别解释以上意思。
UIViewAutoresizingNone就是不自动调整。
UIViewAutoresizingFlexibleLeftMargin就是自动调整与superView左边的距离，也就是说，与superView右边的距离不变。
UIViewAutoresizingFlexibleRightMargin就是自动调整与superView的右边距离，也就是说，与superView左边的距离不变。
UIViewAutoresizingFlexibleTopMargin 
UIViewAutoresizingFlexibleBottomMargin
UIViewAutoresizingFlexibleWidth
UIViewAutoresizingFlexibleHeight
以上就不多解释了，参照上面的。
也可以多个枚举同时设置。如下：
subView.autoresizingMask = UIViewAutoresizingFlexibleLeftMargin  |UIViewAutoresizingFlexibleRightMargin;
如果有多个，就用“|”关联。
还有一个属性就是autoresizesSubviews，此属性的意思就是，是否可以让其subviews自动进行调整，默认状态是YES，就是允许，如果设置成NO，那么subView的autoresizingMask属性失效。
