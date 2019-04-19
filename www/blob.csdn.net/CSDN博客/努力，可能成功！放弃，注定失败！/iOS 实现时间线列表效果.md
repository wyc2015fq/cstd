# iOS 实现时间线列表效果 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2016年05月16日 09:23:50[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：2503
之前看到美团的订单详情页面很有特色，于是决定模仿一下这个效果。
![](http://upload-images.jianshu.io/upload_images/1507403-306be2c008f73726.PNG?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
其实就是简单的 TableView 技巧，下面我们就来一步一步实现它。
# 画个泡泡
首先到 Sketch 里画出气泡的效果
![](http://upload-images.jianshu.io/upload_images/1507403-3fcdf467a07bf7d4.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
很简单，一个圆角矩形 + 一个三角形，然后 **Union** 操作一下，勾个边，done！
然后导出图片文件，添加到 **Xcode** 中。
# 设计 TableViewCell 原型
子类化一个新的 `UITableViewCell` + **Xib**，简单拖拽几个控件。
![](http://upload-images.jianshu.io/upload_images/1507403-129cb5eb5b662704.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
我这里那两条线就没用 **AutoLayout** 了，太小了，回来直接用代码布局就行了。
拖线连几个 **Outlet** ，然后把气泡背景设置一下：
![](http://upload-images.jianshu.io/upload_images/1507403-5942e833a2182ed2.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
事实上，气泡的背景图我制作了两个版本，一个是未选中的状态，另外一个是选中的状态，背景颜色不一样，所以我在 `setHighlighted(:_, :_)` 这个函数中进行设置，其他的保持默认即可。
然后在 **ViewController** 中随便做一些假数据，先看看效果。
![](http://upload-images.jianshu.io/upload_images/1507403-d683568877d3014c.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
哈哈，已经初具雏形了。但是还有一点问题：
- 底部的线是断掉的，并且颜色不对
- 下拉时上边的线也会断掉
那么下面我们就来修复这些问题。
# 完善细节效果
这里我的思路是在 **ViewController** 中再添加两个 **View**，并且监听 **TableView** 的滚动，然后动态调节两个 **View** 的位置。
首先声明两个新 View：
![](http://upload-images.jianshu.io/upload_images/1507403-166bfca8676a423f.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
然后修正线条的颜色：
![](http://upload-images.jianshu.io/upload_images/1507403-dd66464f52679442.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
我在 `tableView(tableView: UITableView, willDisplayCell cell: UITableViewCell, forRowAtIndexPath indexPath: NSIndexPath)` 中获取到 Cell 中线条的颜色，这样我们随时修改 Cell 中的颜色，这里就会自动变换，而不用
*Hardcode* 。
然后是比较困难的线条位置调整，我们需要得到 Cell 中线条的位置，让其 x 坐标和宽度一致，y 坐标和高度动态调整。
首先解决 x 坐标和宽度：
![](http://upload-images.jianshu.io/upload_images/1507403-ec5e6892344e69ff.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
还是在 willDisplay 的方法中，我们用到了 convertPoint 方法进行坐标系变换，就是将线条在 Cell 中的坐标映射到它应该在 SuperView 中的坐标。
然后解决 y 坐标和高度：
![](http://upload-images.jianshu.io/upload_images/1507403-bd76d199198cc74f.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
因为要动态调整，所以我们监听滚动事件，并在其中计算各个坐标。有关这个公式的推导大家可以看看下拉刷新里公式的推导，它们是一致的。
到这里我们就基本完成这个效果了，看看怎么样吧：
![](http://upload-images.jianshu.io/upload_images/1507403-3cf5472d0f3188c6.gif?imageMogr2/auto-orient/strip)
# 小提醒
不要在 `UITableViewController` 中添加自己的 **Subview** ，因为它的 
```
view
```
 属性就是 TableView，如果添加 **Subview**，它们会一起滚动，虽然 WWDC 中也介绍过怎么处理，但毕竟太麻烦，所以我还是推荐大家直接用 UIViewController + TableView 来处理这类比较复杂的效果。
Have fun!!
文／Cyandev（简书作者）
原文链接：http://www.jianshu.com/p/2504f2989556
著作权归作者所有，转载请联系作者获得授权，并标注“简书作者”。
