# iOS 8 自适应 Cell - =朝晖= - 博客园
# [iOS 8 自适应 Cell](https://www.cnblogs.com/dhcn/p/7130927.html)
http://vit0.com/blog/2014/11/13/[iOS](http://lib.csdn.net/base/ios)-8-zi-shi-ying-cell/
在使用 table view 的时侯经常会遇到这样的需求：table view 的 cell 中的内容是动态的，导致在开发的时候不知道一个 cell 的高度具体是多少，所以需要提供一个计算 cell 高度的[算法](http://lib.csdn.net/base/datastructure)，在每次加载到这个 cell 的时候计算出 cell 真正的高度。
## 在 iOS 8 之前
没有使用 Autolayout 的情况下，需要实现 table view delegate 的 `tableView(tableView: UITableView, heightForRowAtIndexPath indexPath: NSIndexPath) -> CGFloat`方法，在这个方法中计算并返回 cell 的高度。比如，我有一个可以显示任意行数的纯文本 cell，计算 cell 的代码可以是这样：
```
```
override func tableView(tableView: UITableView, heightForRowAtIndexPath indexPath: NSIndexPath) -> CGFloat {
    let content = self.datas[indexPath.row] as String
    let padding: CGFloat =
```
```
上面的代码是一个最简单的例子，这个例子看起来好像没有什么问题。但是通过查看这个 delegate 方法的文档后，可以知道，在每次 reload tableview 的时候，程序会先计算出每一个 cell 的高度，等所有高度计算完毕，确定了 tableview 的总的高度后，才开始渲染视图并显示在屏幕上。这意味着在显示 table view 之前需要执行一堆的计算，并且这是在主线程中进行的，如果计算量太大程序就很有可能出现卡顿感。比如： table view 的数据有上千条，或者计算高度的代码中还要先获取图片再根据图片计算高度，这些操作都是非常慢的。
如果在 cell 中使用了 autolayout，在计算 cell 高度时会更麻烦。有兴趣的可以看这里有篇关于[如何在 autolayout 下动态计算高度](http://johnszumski.com/blog/auto-layout-for-table-view-cells-with-dynamic-heights) 的文章。
为什么不能等滚动到某个 cell 的时候，再调用计算这个 cell 高度的 delegate 呢？原因是 tableview 需要获得它的内容的总高度，用这个高度去确定滚动条的大小等。直到 [ios](http://lib.csdn.net/base/ios)7 `UITableViewDelegate`中添加了新的 API
```
```
tableView(tableView: UITableView, estimatedHeightForRowAtIndexPath indexPath: NSIndexPath) -> CGFloat
```
```
这个方法用于返回一个 cell 的预估高度，如果在程序中实现了这个方法，tableview 首次加载的时候就不会调用`heightForRowAtIndexPath` 方法，而是用 `estimatedHeightForRowAtIndexPath` 返回的预估高度计算 tableview 的总高度，然后 tableview 就可以显示出来了，等到 cell 可见的时候，再去调用`heightForRowAtIndexPath` 获取 cell 的正确高度。
通过使用`estimatedHeightForRowAtIndexPath` 这个 Delegate 方法，解决了首次加载 table view 出现的性能问题。但还有一个麻烦的问题，就是在 cell 没有被加载的时候计算 cell 的高度，上面给出的代码中，仅仅是计算一个 NSString 的高度，就需要不少代码了。这种计算实际上是必须的，然而在 iOS 8 开始，你可能可以不用再写这些烦人的计算代码了！
## iOS 8 的魔法
在 iOS 8 中，self size cell 提供了这样一种机制：cell 如果有一个确定的宽度/高度，autolayout 会自动根据 cell 中的内容计算出对应的高度/宽度。
### TableView 中的 cell 自适应
要让 table view 的 cell 自适应内容，有几个要点：
- 设置的 AutoLayout 约束必须让 cell 的 contentView 知道如何自动延展。关键点是 contentView 的 4 个边都要设置连接到内容的约束，并且内容是会动态改变尺寸的。
- UITableView 的 `rowHeight` 的值要设置为 `UITableViewAutomaticDimension`
- 和 iOS 7 一样，可以实现 `estimatedHeightForRowAtIndexPath` 方法提升 table view 的第一次加载速度。
- 任何时候 cell 的 `intrinsicContentSize` 改变了（比如 table view 的宽度变了），都必须重新加载 table view 以更新 cell。
#### 例子
在 Xcode 中新建一个项目，在 storyboard 中创建一个 UITableViewController 的 IB，创建一个如下样子的 cell：
![图1 cell 外观](http://vit0.com/images/posts/2014-11-13-self-size-cell-1.jpg)
图1 cell 外观
这个 cell 中有 3 个元素，其中 imageView 的 autoLayout 约束为：
- imageView 左边离 contentView 左边 0
- imageView 上边离 contentView 上边 0
- imageView 的 width 和 height 为 80
- imageView 下边离 contentView 下边大于等于 0（为了防止内容太少，导致 cell 高度小于图片高度）
titleLabel 的 autoLayout 约束为：
- titleLabel 左边离 imageView 右边 8
- titleLabel 上边和 imageView 上边在同一只线上
- titleLabel 右边离 contentView 右边 0
- titleLabel 下边离 description 上边 8
- titleLabel 的高度小于等于 22，优先级为 250
descriptionLabel 的约束为：
- descriptionLabel 左边和 titleLabel 左边在同一直线上
- descriptionLabel 上边里 titleLabel 8
- descriptionLabel 下边里 contentView 下边 0
- descriptionLabel 右边离 contentView 右边 0
然后在这个 IB 对应的 UITableViewController 中加载一些数据进去，显示效果如图：
![图2 自适应 cell 效果图](http://vit0.com/images/posts/2014-11-13-self-size-cell-2.jpg)
图2 自适应 cell 效果图
实现这个效果，我除了设置了 autoLayout，还设置了 tableView 的 `rowHeight = UITableViewAutomaticDimension`，然后就是这样了。一点计算 cell 高度的代码都没有！！我连 `heightForRowAtIndexPath`都不用实现，真的是….爽出味啊！所以如果已经在开发 iOS 8 Only 的应用了一定要用autolayout，把烦人的计算交给 autolayout 去吧。
### CollectionView 中的 cell 自适应
在 collection view 中也能让 cell 自适应内容大小，如果 UICollectionView 的 layout 是一个 UICollectionViewFlowLayout，只需要将 `layout.itemSize = ...` 改成 `layout.estimatedItemSize = ...`。 只要设置了 layout 的 estimatedItemSize，collection view 就会根据 cell 里面的 autolayout 约束去确定cell 的大小。
原理：
- collection view 根据 layout 的 estimatedItemSize 算出估计的 contentSize，有了 contentSize collection view 就开始显示
- collection view 在显示的过程中，即将被显示的 cell 根据 autolayout 的约束算出自适应内容的 size
- layout 从 collection view 里获取更新过的 size attribute
- layout 返回最终的 size attribute 给 collection view
- collection 使用这个最终的 size attribute 展示 cell
## 总结
这次 iOS 8 的发布对 UI 开发来说是越来方便了，很多以前需要写大量计算的代码现在都可以通过拖拖 IB 上的 UI 控件就可以实现了，当然首先你要会 autolayout。 如果很幸运的在开发 iOS 8 only 的应用，真的可以删除`heightForRowAtIndexPath`中那些繁重的计算代码了！让 autolayout 帮我们完成所有的工作吧。
## 参考
- [WWDC Session 226 Whats New in Table and Collection Views](https://developer.apple.com/videos/wwdc/2014/#226-video)
- [iOS 8 Tutorial Series: Auto Sizing Table Cells](http://captechconsulting.com/blog/tyler-tillage/ios-8-tutorial-series-auto-sizing-table-cells)
- 

