# iOS 全屏侧滑手势/UIScrollView/UISlider间滑动手势冲突 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2018年09月07日 09:38:11[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：36
![](https://upload-images.jianshu.io/upload_images/1708447-3ef7a155b61b3616.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/450/format/webp)
七夕节福利
![](https://upload-images.jianshu.io/upload_images/1708447-83ae3654030d8414.gif?imageMogr2/auto-orient/strip%7CimageView2/2/w/275/format/webp)
效果预览
一、前期准备
> 
有一个支持全屏侧滑返回的视图控制器ViewController，ViewController.view上有一个UIScrollView，UIScrollView上有UISlider。俺直接在之前的示例Demo上演示，简书地址：[iOS 自定义转场动画](https://www.jianshu.com/p/a9b1307b305b) ，Github地址 ：[WSLTransferAnimation](https://github.com/wslcmk/WSLTransferAnimation.git)
二、问题展示
- **现象 1**、*UIScrollView当前在第一页即contentOffset.x=0时，左滑不能触发全屏侧滑pop返回的手势* ；
![](https://upload-images.jianshu.io/upload_images/1708447-a9eacdbc7092ff36.gif?imageMogr2/auto-orient/strip%7CimageView2/2/w/276/format/webp)
UIScrollView和全屏侧滑pop返回手势冲突示意图
- **现象2** 、*问题1解决后，你会发现拖拽UIScrollView第一页上的UISlider时，向右拖拽时却触发了全屏侧滑pop返回的手势，而UISlider本身的拖拽事件却没有响应；向左拖拽UISlider时，响应的是UIScrollView的拖动事件，而UISlider本身的拖拽事件也没有响应*。
![](https://upload-images.jianshu.io/upload_images/1708447-dd07d227fe222ecb.gif?imageMogr2/auto-orient/strip%7CimageView2/2/w/276/format/webp)
UISlider与UIScrollView、全屏侧滑pop返回手势冲突示意图
- **现象3** 、*当你长按UISlider超过150ms后直接拖拽，就不存在现象2中UISlider与UIScrollView、全屏侧滑返回的冲突问题了*。
![](https://upload-images.jianshu.io/upload_images/1708447-418b5c5d99e7de6c.gif?imageMogr2/auto-orient/strip%7CimageView2/2/w/276/format/webp)
手势冲突.gif
三、分析解决问题
> 
这些问题很显然，肯定跟iOS事件的传递和响应链机制有关系，不了解的可以看看这篇文章 [史上最详细的iOS之事件的传递和响应机制-原理篇](https://www.jianshu.com/p/2e074db792ba)或者[iOS响应链全家桶](https://www.jianshu.com/p/c294d1bd963d)
- **分析解决问题 1**
*如果你了解事件的传递和响应链机制的话，应该能想到，是由于UIScrollView的内部手势方法阻断了全屏侧滑返回手势的的响应，那我们就找到这个方法，代码如下* ；
> 
创建一个UIScrollView的类别UIScrollView+GestureConflict，重写如下方法：
```
//处理UIScrollView上的手势和侧滑返回手势的冲突
-(BOOL)gestureRecognizer:(UIGestureRecognizer *)gestureRecognizer shouldRecognizeSimultaneouslyWithGestureRecognizer:(UIGestureRecognizer *)otherGestureRecognizer{
    // 首先判断otherGestureRecognizer是不是系统pop手势
    if ([otherGestureRecognizer.view isKindOfClass:NSClassFromString(@"UILayoutContainerView")]) {
        // 再判断系统手势的state是began还是fail，同时判断scrollView的位置是不是正好在最左边
        if (otherGestureRecognizer.state == UIGestureRecognizerStateBegan && self.contentOffset.x == 0) {
            return YES;
        }
    }
    return NO;
}
```
- **分析解决问题 2和3**
*方案一：这个跟UIScrollView的一个属性delaysContentTouches有关。*
> 
scrollView.delaysContentTouches = NO;
delaysContentTouches 默认值为YES 表示延迟scrollView上子视图的响应，所以当直接拖动UISlider时，如果此时touch时间在150ms以内，UIScrollView会认为是拖动自己，从而拦截了event，导致UISlider接收不到滑动的event。但是只要长按住UISlider一会儿再拖动，此时touch时间超过150ms，因此滑动的event会发送到UISlider上，然后UISlider再作出响应；设置为NO后，拖动UISlider时就可以直接做出响应，解决了UISlider与UIScrollView之间的冲突，同时也解决了向右拖拽时却触发了全屏侧滑pop返回的问题。
*方案二： 重写类别UIScrollView+GestureConflict中的如下方法来解决UISlider与UIScrollView之间的冲突，然后还需要执行下面 ***问题补充*** 中的操作来处理UISlider的滑动与全屏侧滑pop返回事件的冲突。*
```
//拦截事件的处理 事件传递给谁，就会调用谁的hitTest:withEvent:方法。
//处理UISlider的滑动与UIScrollView的滑动事件冲突
- (UIView *)hitTest:(CGPoint)point withEvent:(UIEvent *)event {
    /*
     直接拖动UISlider，此时touch时间在150ms以内，UIScrollView会认为是拖动自己，从而拦截了event，导致UISlider接受不到滑动的event。但是只要按住UISlider一会再拖动，此时此时touch时间超过150ms，因此滑动的event会发送到UISlider上。
     */
    UIView *view = [super hitTest:point withEvent:event];
    
    if([view isKindOfClass:[UISlider class]]) {
        //如果接收事件view是UISlider,则scrollview禁止响应滑动
        self.scrollEnabled = NO;
    } else {   //如果不是,则恢复滑动
        self.scrollEnabled = YES;
    }
    return view;
}
```
- **问题补充**
*示例Demo中的UISlider是在UIScrollView上的，如果UISlider不是在UIScrollView上，而是直接就在ViewController.view上，那也就会出现拖拽UISlider时却响应了全屏侧滑pop返回手势的问题。*
> 
在支持全屏侧滑返回的UINavigationController的子类WSLNavigatioController中，遵守协议<UIGestureRecognizerDelegate>,实现如下方法：
```
#pragma mark -- UIGestureRecognizerDelegate
//触发之后是否响应手势事件
//处理侧滑返回与UISlider的拖动手势冲突
- (BOOL)gestureRecognizer:(UIGestureRecognizer *)gestureRecognizer shouldReceiveTouch:(UITouch *)touch{
    //如果手势是触摸的UISlider滑块触发的，侧滑返回手势就不响应
    if ([touch.view isKindOfClass:[UISlider class]]) {
        return NO;
    }
    return YES;
}
```
四、应用示例
手势冲突应用示例.gif
> 
好了，俺要去鹊桥跟俺家织女相会咯✌️(*❦ω❦)，就说这么多了，今天七夕节，也祝各位单身猿告白成功，玩的开心O(∩_∩)O哈哈~
![](https://upload-images.jianshu.io/upload_images/1708447-03c8b6f0c64ef56d.gif?imageMogr2/auto-orient/strip%7CimageView2/2/w/198/format/webp)
表白
推荐阅读：
[iOS 自定义转场动画](https://www.jianshu.com/p/a9b1307b305b)
[iOS 瀑布流封装](https://www.jianshu.com/p/9fafd89c97ad)
[WKWebView的使用](https://www.jianshu.com/p/5cf0d241ae12)
[UIScrollView视觉差动画](https://www.jianshu.com/p/3b30b9cdd274)
[iOS 传感器集锦](https://www.jianshu.com/p/5fc26af852b6)
[iOS 音乐播放器之锁屏歌词+歌词解析+锁屏效果](https://www.jianshu.com/p/35ce7e1076d2)
[UIActivityViewController系统原生分享-仿简书分享](https://www.jianshu.com/p/b6b44662dfda)
[iOS UITableView获取特定位置的cell](https://www.jianshu.com/p/70cdcdcb6764)
作者：且行且珍惜_iOS
链接：https://www.jianshu.com/p/a9a322052f26
來源：简书
简书著作权归作者所有，任何形式的转载都请联系作者获得授权并注明出处。
