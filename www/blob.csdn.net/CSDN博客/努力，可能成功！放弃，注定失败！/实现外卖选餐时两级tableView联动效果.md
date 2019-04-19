# 实现外卖选餐时两级tableView联动效果 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2016年12月13日 11:07:09[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：830
最近实现了下饿了么中选餐时两级tableView联动效果，先上效果图，大家感受一下：
![](http://upload-images.jianshu.io/upload_images/1432270-a195dd0c61daa21e.gif?imageMogr2/auto-orient/strip)
联动效果
**下面说下具体实现步骤：**
首先分解一下，实现这个需求主要是两点，一是点击左边tableView，同时滚动右边tableView到具体的位置。二是拖动右边tableView选中左边tableView对应的某一行。要实现这个需求有一点很重要：**左边的tableView每一行对应的是右边tableView的每个分区**，OK，Just Do It.
实现点击左边tableView同时滚动右边tableView，很简单，只需要实现tableView的代理方法`- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath;`然后在代理方法里边拿到右边的tableView，实现让其滚动到第indexPath.row分区，第0行即可，代码如下：
```
- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath{
    // 如果点击的是右边的tableView，不做任何处理
    if (tableView == self.rightTableView) return;
    // 点击左边的tableView，设置选中右边的tableView某一行。左边的tableView的每一行对应右边tableView的每个分区
    [self.rightTableView selectRowAtIndexPath:[NSIndexPath indexPathForRow:0 inSection:indexPath.row] animated:YES scrollPosition:UITableViewScrollPositionTop];
}
```
我们这里不处理右边tableView的点击事件，所以`if (tableView == self.rightTableView) return;`
接下来我们实现 拖动右边tableView选中左边tableView对应的某一行，我们要动态选中左边的tableView，就需要拿到现在滚动到了那个分区，UITableView有两个代理方法，`- (void)tableView:(UITableView *)tableView willDisplayHeaderView:(UIView *)view forSection:(NSInteger)section // 一个头标题即将显示的时候掉用`和
```
- (void)tableView:(UITableView
 *)tableView didEndDisplayingHeaderView:(UIView *)view forSection:(NSInteger)section // 一个头标题即将消失的时候掉用
```
**利用这两个方法就可以拿到当前所在分区实现这个功能了。**
但是我总觉得这个方法不好，还有个更简单的方法，其实tableView有个极不常用，但很牛X的方法，叫做indexPathsForVisibleRows，官方文档解释是：
> 
The value of this property is an array of NSIndexPath objects each representing a row index and section index that together identify a visible row in the table view. If no rows are visible, the value is nil.
简单意思就是，它返回一个装着目前屏幕上可见的cell的indexPath集合。
好的，重点来了，拿到这个集合，不就能拿到目前屏幕上顶端的cell的indexpath了吗，那就如愿以偿的拿到现在所在第indexpath.section个分区了。
说了这么多，上代码：
```
#pragma mark - UIScrollViewDelegate
-(void)scrollViewDidScroll:(UIScrollView *)scrollView{ // 监听tableView滑动
    // 如果现在滑动的是左边的tableView，不做任何处理
    if ((UITableView *)scrollView == self.leftTableView) return;
    // 滚动右边tableView，设置选中左边的tableView某一行。indexPathsForVisibleRows属性返回屏幕上可见的cell的indexPath数组，利用这个属性就可以找到目前所在的分区
    [self.leftTableView selectRowAtIndexPath:[NSIndexPath indexPathForRow:self.rightTableView.indexPathsForVisibleRows.firstObject.section inSection:0] animated:YES scrollPosition:UITableViewScrollPositionMiddle];
}
```
稍微解释一下，首先监听scrollView的拖动，本demo不处理左边tableView的滚动，所以`if ((UITableView *)scrollView == self.leftTableView) return;`
`self.rightTableView.indexPathsForVisibleRows.firstObject.section`这句是拿到当前屏幕上可见cell的第一行cell所在的分区，然后让左边的tableView选中第0分区(它只有一个分区)的这一行就OK了。
欢迎收藏本文章，最后附上demo ，Github传送门：[demo](https://github.com/WZBbiao/WZBLinkageTableView)
－－－－－－－－－－－－－－这叫分割线－－－－－－－－－－－－－－－
补充下：下边评论提到说点击左边tableView的时候会有阴影效果，其实是这样的，点击左边的tableView，右边的tableView是从当前位置**动画**滚动到相应位置的，既然有滚动，就会调`- (void)scrollViewDidScroll:(UIScrollView *)scrollView`这个代理方法，说白了就是拖动了右边tableView，拖动右边的过程中会陆续选中左边。那我想大家就明白了。
如果不想要这个效果，有两个办法，一个是直接吧`- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath`中的动画滚动的属性**animated值改成NO**
```
- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath{
    // 如果点击的是右边的tableView，不做任何处理
    if (tableView == self.rightTableView) return;
    // 点击左边的tableView，设置选中右边的tableView某一行。左边的tableView的每一行对应右边tableView的每个分区
    [self.rightTableView selectRowAtIndexPath:[NSIndexPath indexPathForRow:0 inSection:indexPath.row] animated:NO scrollPosition:UITableViewScrollPositionTop];
}
```
这样做右边的tableView就是无动画滚动了，也就不会再调scrollViewDidScroll:方法。但是如果还想右边tableViewyou滚动效果，另一种解决方法是：把`- (void)scrollViewDidScroll:(UIScrollView *)scrollView`方法换成`- (void)scrollViewDidEndDecelerating:(UIScrollView *)scrollView`这个代理方法方法就行了。饿了么有的界面好像就是这样做的，但是有bug(估计饿了么没测出来)，这个方法的注释为
> 
// called when scroll view grinds to a halt 当滚动视图戛然而止
－－有道翻译如是说
根据本人亲测，拖拽之后，这个方法调用与否在于你的手指是否在动画停止之前离开了屏幕，如果在动画结束之前手指离开屏幕，此方法调用没什么问题。but，如果动画已经停止，再把手指拿开，这个方法是不会调的。有图有真相：
![](http://upload-images.jianshu.io/upload_images/1432270-72881cf39a4a96aa.gif?imageMogr2/auto-orient/strip)
WZBLinkageTableViewGif.gif
解决这个bug的关键在于，让手指离开的时候手动调一次这个代理方法，那怎么才能知道手指什么时候离开呢？scrollView给我们了另一个代理方法：`- (void)scrollViewWillEndDragging:(UIScrollView *)scrollView withVelocity:(CGPoint)velocity targetContentOffset:(inout CGPoint *)targetContentOffset`这个方法在结束拖拽的时候调，正好解决了我们的问题：
```
- (void)scrollViewWillEndDragging:(UIScrollView *)scrollView withVelocity:(CGPoint)velocity targetContentOffset:(inout CGPoint *)targetContentOffset{
    // 推拽将要结束的时候手动调一下这个方法
    [self scrollViewDidEndDecelerating:scrollView];
}
```
OK，解决问题！
后续，github也会跟进的，感谢大家的喜欢和支持！
Github已经更新，最新demo在这 [demo](https://github.com/WZBbiao/WZBLinkageTableView)
文／杂雾无尘（简书作者）
原文链接：http://www.jianshu.com/p/dfb73aa08602
著作权归作者所有，转载请联系作者获得授权，并标注“简书作者”。
