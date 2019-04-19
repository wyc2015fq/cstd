# UITableView自动计算cell高度并缓存，再也不用管高度啦 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2017年05月08日 17:13:33[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：996
本文来自简书，原文地址:[http://www.jianshu.com/p/64f0e1557562](http://www.jianshu.com/p/64f0e1557562)
这篇文章我们来讲一下UITableView的cell自适应高度，以及遇到的问题的解决办法。在看文章之前希望你已经会UITableView的基本使用了。
先奉上这篇文章的demo的Github地址：[UITableViewCellHeightDemo](https://github.com/lisongrc/UITableViewCellHeightDemo)。大家可以下载下来和文章配合看。
#### cell高度计算的历史
在iOS8之前，如果UITableViewCell的高度是动态的，如果想要显示正确的话，我们需要在下面这个UITableView的代理方法中，返回每一行的精确高度：
`- (CGFloat)tableView:(UITableView *)tableView heightForRowAtIndexPath:(NSIndexPath *)indexPath;`
如果cell的控件很多，样式很复杂的话，在这里面我们就可能需要写很多代码去做一些复杂的计算，甚至可能导致滑动不流畅。
后来也有一些人写了一些第三方去解决这个问题，例如[UITableView-FDTemplateLayoutCell](https://github.com/forkingdog/UITableView-FDTemplateLayoutCell)。只要给cell自上而下加好约束，它就可以帮我们去算cell的高度并且可以缓存，省去了我们自己写计算代码的成本。具体可以进链接里面看看它的demo。
但是在iOS10的系统下， `FDTemplateLayoutCell`会卡界面，而且tableview的行数越多表现的越卡。
而且苹果在iOS8之后，推出了一种超级简单的cell动态自适应的方法，使用起来比 `FDTemplateLayoutCell`也简单一些，而且现在iOS10都出来了，没有必要去支持iOS7了，所以最后我还是选择了用系统的办法。这样我们以后就再也不用写`heightForRowAtIndexPath`方法了哈哈哈。
#### 系统的cell自适应高度的使用方法
首先我们需要把cell上的控件自上而下加好约束，如果对约束不熟悉的话建议看看下面这两篇文章学习一下：
[](http://www.raywenderlich.com/115444/auto-layout-tutorial-in-ios-9-part-2-constraints)[Auto
 Layout Tutorial in iOS 9 Part 1: Getting Started](https://www.raywenderlich.com/115440/auto-layout-tutorial-in-ios-9-part-1-getting-started-2)([http://www.raywenderlich.com/115440/auto-layout-tutorial-in-ios-9-part-1-getting-started-2](http://www.raywenderlich.com/115440/auto-layout-tutorial-in-ios-9-part-1-getting-started-2))
[[Auto Layout Tutorial in iOS 9 Part 2: Constraints](https://www.raywenderlich.com/115444/auto-layout-tutorial-in-ios-9-part-2-constraints)
用xib加约束和用masonry加代码约束都是可以的。注意约束一定要自上而下加好，让系统知道怎么去计算高度。在这篇文章的demo里面的cell加的约束是这样的：
![](http://upload-images.jianshu.io/upload_images/1608265-8e19466b6e4ac956.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
cell约束
加好约束后，然后告诉tableView自己去适应高度就可以了。有两种写法：
```
self.tableView.rowHeight = UITableViewAutomaticDimension;
self.tableView.estimatedRowHeight = 100;
```
或者直接写这个代理方法就可以了
```
- (CGFloat)tableView:(UITableView *)tableView estimatedHeightForRowAtIndexPath:(NSIndexPath *)indexPath
{
    return 100;
}
```
这个的意思就是告诉tableView，你需要自己适应高度，我不给你算啦哈哈哈。但是我们需要告诉它一个大概高度，例如上面的100，理论上这个是可以随便写的，并不影响显示结果，但是越接近真实高度越好。
来看下demo效果：
![](http://upload-images.jianshu.io/upload_images/1608265-118a76ca9e5bb550.gif?imageMogr2/auto-orient/strip)
demo
我们看到，cell已经自己适应内容算出了高度，是不是很方便呢哼哼。
具体的代码大家可以去demo看哦。
其实section的header和footer也是可以自动适应的，对应的方法有：
```
- (CGFloat)tableView:(UITableView *)tableView estimatedHeightForHeaderInSection:(NSInteger)section;
- (CGFloat)tableView:(UITableView *)tableView estimatedHeightForFooterInSection:(NSInteger)section;
```
但是我们在实际开发中，一般都是根本没有header和footer，有的话一般也是给一个固定高度。所以在这里就不讲解了，原理都一样。
#### 可能遇到的问题和解决办法
1.高度不对
有时候有可能运行出来后看到cell的高度显示的不对，就像这样：
![](http://upload-images.jianshu.io/upload_images/1608265-198030f4098810eb.gif?imageMogr2/auto-orient/strip)
高度不对
这个问题是因为约束没有满足自上而下，从而系统不知道怎么去计算。解决办法就是去修改约束，直到满足为止。一定要好好理解约束啊！
2.点击状态栏无法滚动到顶部
我们知道，如果界面中有UIScrollView的话，点击状态栏会让其滚动到顶部，就像这样：
![](http://upload-images.jianshu.io/upload_images/1608265-d8ee37f4ebea29bd.gif?imageMogr2/auto-orient/strip)
点击状态栏会滚动到顶部
但是如果我们用了自动计算高度的方法，又调用了tableView的reloadData方法（例如我们的数据有分页的时候，加载完下一页的数据后会去刷新tableView）。这时候就会出现问题，点击状态栏就有几率不能精确滚动到顶部了：
![](http://upload-images.jianshu.io/upload_images/1608265-0144754f8dff68eb.gif?imageMogr2/auto-orient/strip)
Untitled.gif
解决这个问题的办法是去缓存cell的高度，代码如下：
`@property (nonatomic, strong) NSMutableDictionary *heightAtIndexPath;//缓存高度所用字典`
```
#pragma mark - UITableViewDelegate
-(CGFloat)tableView:(UITableView *)tableView estimatedHeightForRowAtIndexPath:(NSIndexPath *)indexPath
{
    NSNumber *height = [self.heightAtIndexPath objectForKey:indexPath];
    if(height)
    {
        return height.floatValue;
    }
    else
    {
        return 100;
    }
}
- (void)tableView:(UITableView *)tableView willDisplayCell:(UITableViewCell *)cell forRowAtIndexPath:(NSIndexPath *)indexPath
{
    NSNumber *height = @(cell.frame.size.height);
    [self.heightAtIndexPath setObject:height forKey:indexPath];
}
```
解释一下，就是用一个字典做容器，在cell将要显示的时候在字典中保存这行cell的高度。然后在调用estimatedHeightForRowAtIndexPath方法时，先去字典查看有没有缓存高度，有就返回，没有就返回一个大概高度。
缓存高度之后，在demo里面多试几次，发现点击状态栏已经可以精确滚动回顶部了：
![](http://upload-images.jianshu.io/upload_images/1608265-b09cef057ef8ba9a.gif?imageMogr2/auto-orient/strip)
用缓存高度解决了问题
这段代码其实可以写在viewController的基类里面，这样写一遍就可以每个地方都能缓存cell的高度了。详见demo。这样就完美了！
