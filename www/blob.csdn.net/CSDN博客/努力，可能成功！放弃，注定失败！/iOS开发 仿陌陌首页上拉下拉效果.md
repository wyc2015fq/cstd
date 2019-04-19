# iOS开发 仿陌陌首页上拉下拉效果 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2018年11月22日 09:07:16[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：228
- 这几天产品小弟说要把首页做成像陌陌的首页，作为一个程序员本来是想剁了他的，但是考虑到最近工作不好找，就忍了的。。。
### 陌陌首页分析：
![](https://upload-images.jianshu.io/upload_images/9892787-768f1c252eeef991.gif?imageMogr2/auto-orient/strip%7CimageView2/2/w/360/format/webp)
陌陌操作图.gif
陌陌首页长这样
- 
往上推的时候明显感觉是下面的盖住了上面的，所以可以分成几个图层，上面的在最下面的图层，下面的tableview在上层。
- 
其中主要的问题就是 下面的tableview的滑动手势 和 页面的滑动冲突，所以就是依靠pan手势，
	思路就是 ：在某个区域范围里面，禁止掉tableview的滑动，超出之后，禁止掉Pan手势；
- 
手势的两个代理方法
```
// 在手势冲突时候调用，默认返回NO表示默认不能同时识别两个手势
- (BOOL)gestureRecognizer:(UIGestureRecognizer *)gestureRecognizer shouldRecognizeSimultaneouslyWithGestureRecognizer:(UIGestureRecognizer *)otherGestureRecognizer;
//在手势想要从UIGestureRecognizerStatePossible状态变化的时候调用，返回NO则表示手势状态是UIGestureRecognizerStateFailed
- (BOOL)gestureRecognizerShouldBegin:(UIGestureRecognizer *)gestureRecognizer;
```
```
-(void)observeValueForKeyPath:(NSString *)keyPath ofObject:(id)object change:(NSDictionary<NSKeyValueChangeKey,id> *)change context:(void *)context{
   if ((pan.state == UIGestureRecognizerStateEnded) && (frame.origin.y == level2)) {
  .....................................................................
}
}
```
- 在手势的代理方法里面禁掉tableview的滑动,然后对pan进行手势的监控，判断手势滑动结束并且不在互动范围里面的时候禁掉手势, 放开tableview的滑动。
效果图见下面
![](https://upload-images.jianshu.io/upload_images/9892787-386b00899f6770f1.gif?imageMogr2/auto-orient/strip%7CimageView2/2/w/360/format/webp)
效果图.gif
具体Demo见github链接 下载就能运行
[https://github.com/1784777348ly/MOMOHomePage](https://github.com/1784777348ly/MOMOHomePage)
作者：没穿秋裤
链接：https://www.jianshu.com/p/d0211078fd77
來源：简书
简书著作权归作者所有，任何形式的转载都请联系作者获得授权并注明出处。
