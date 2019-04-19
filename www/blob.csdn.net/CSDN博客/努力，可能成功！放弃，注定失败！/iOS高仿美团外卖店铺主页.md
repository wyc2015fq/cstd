# iOS高仿美团外卖店铺主页 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2018年09月17日 10:08:40[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：896
高仿美团外卖的店铺主页（包括下拉动画效果，解决各种手势问题，并且cell有列表样式，九宫格样式，卡片样式），各种动画效果纵享丝滑，因为写的比较急，还待优化.!
解决UIScrollView嵌套UIScrollView、UITableview或者UIcollectionView的问题，结合手势和仿动力学UIKit Dynamic实现自定义scollView效果。
手势冲突参考文章：[iOS 嵌套UIScrollview的滑动冲突另一种解决方案 - 简书](https://www.jianshu.com/p/df01610b4e73)
[GitHub](https://github.com/chikang/CKMeiTuanShopView)：[https://github.com/chikang/CKMeiTuanShopView](https://github.com/chikang/CKMeiTuanShopView)
UIAttachmentBehavior：附着行为
UICollisionBehavior：碰撞行为
UIGravityBehavior：重力行为
UIDynamicItemBehavior：动态元素行为
UIPushBehavior：推行为
UISnapBehavior：吸附行为
上述的各种行为可单独使用，也可以组合使用更复杂的动画效果。
![](https://upload-images.jianshu.io/upload_images/2549388-908928675e34aaa2.gif?imageMogr2/auto-orient/strip%7CimageView2/2/w/136/format/webp)
高仿美团外卖GIF
需求核心问题就一个，如何优雅的解决：当内容还未到最大可显示范围时，列表里的内容不能滚动；当内容显示到最大的时候，如何不断开当前滚动手势，继续滚动列表里的内容。
#原理
为内容承载视图添加一个UIPanGestureRecognizer，成为其delegate，并让shouldRecognizeSimultaneouslyWithOtherGestureRecognizer方法返回true；如此一来，内容承载视图与列表视图的滚动手势可以同时响应了。接着，我们需要处理好当内容承载视图未显示到最大值(_maxOffset_Y)时，列表视图(UITableView、UICollectionView)的contentOffset.y会被强制设置为0，营造一种列表内容未滚动的假象；当内容承载视图滚动到最大的时候，就放开对列表视图的滚动限制,并且通过仿动力学UIKit Dynamic实现自定义scollView效果.
1：手势问题，可参考👆的文章，解释的很详细，包括手势问题，以及如何实现自定义scrollView效果，模拟scrollView的回弹速度，阻尼效果等等.
2：tableview和collectionView都继承于scrollview，把父视图scrollview内容承载视图,在内容承载视图上添加tableview或collectionView，计算父视图scrollview(内容承载视图)向上滑动与导航条无缝对接需要的最大偏移量_maxOffset_Y，再根据偏移量是否大于_maxOffset_Y，来判断是父视图scrollview(内容承载视图)在进行偏移，还是子视图scrollview在进行偏移，从而设置scrollview.contentOffset.
3:根据scrollview的代理方法scrollViewDidScroll，来监听scrollview的偏移量，来实现头部的动画效果以及导航条的动画效果。
4：判断向下滑动偏移量是否大于设定好的距离（70），来让整个商品列表平移向下消失，展示店铺活动优惠券视图。通过滑动手势，从底部向上滑动或者点击导航条的返回按钮，让商品列表平移向上动画展示出来。
5：实现二级联动效果，根据父视图scrollview(内容承载视图)的偏移量来计算左侧菜单menuTableView的高度，实现动态高度，达到跟美团外卖一样的效果.
6：添加横向scrollview，实现可以横向滑动。
7：实现评价列表上拉加载效果，解决与自定义scrollview偏移量冲突问题。（使用MJRefresh会有问题。）
![](https://upload-images.jianshu.io/upload_images/2549388-a0d01ce8da269307.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1000/format/webp)
点击左侧cell,右侧滚到到对应的section
![](https://upload-images.jianshu.io/upload_images/2549388-1862505b035fb58a.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1000/format/webp)
左侧tableview滑动，左侧滚动到对应的行
![](https://upload-images.jianshu.io/upload_images/2549388-23cf28bd9e060a71.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/250/format/webp)
【商家店铺列表样式】tot
![](https://upload-images.jianshu.io/upload_images/2549388-59818bf1e7ada779.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/250/format/webp)
【商家店铺卡片样式】
![](https://upload-images.jianshu.io/upload_images/2549388-f272e834a92bd932.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/250/format/webp)
【商家店铺九宫格样式】
![](https://upload-images.jianshu.io/upload_images/2549388-e8041d5637fdc63f.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/300/format/webp)
评价页面
![](https://upload-images.jianshu.io/upload_images/2549388-41ccd2213f2b9b08.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/250/format/webp)
【店铺活动】
![](https://upload-images.jianshu.io/upload_images/2549388-697e9d5d2a41b5c1.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/250/format/webp)
【类似QQ菜单视图】
![](https://upload-images.jianshu.io/upload_images/2549388-361491269f215fbc.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/250/format/webp)
【商家店铺信息】
作者：xiaoxiaokang
链接：https://www.jianshu.com/p/aa920502a12f
來源：简书
简书著作权归作者所有，任何形式的转载都请联系作者获得授权并注明出处。
