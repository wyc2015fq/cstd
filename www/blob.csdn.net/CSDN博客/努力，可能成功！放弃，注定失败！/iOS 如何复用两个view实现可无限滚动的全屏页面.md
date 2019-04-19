# iOS:如何复用两个view实现可无限滚动的全屏页面 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2018年03月28日 09:49:48[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：451
学iOS的应该没有没用过`tableView`的吧，`tableView`里面会对`dequeueReusableCellWithIdentifier`申明的`cell`根据identifier进行复用，而cell的复用相信会让一些初学者有一些困扰，它是怎么实现复用的，为什么我这么写，`cell`会变得好乱，完全不是我想要的。
而在平常的需求里面也会遇到一些需要横向的可循环使用的视图的需求
如下图，
![](https://upload-images.jianshu.io/upload_images/302190-21ffcef03744580d.gif?imageMogr2/auto-orient/strip%7CimageView2/2/w/684)
headTabbarWithViews.gif
如果每需要一个就重新创建一个`view`的话，这么多`view`会占用太多的内存了，而且其实很多`view`，完全是占着茅坑不拉屎。所以，我们一般都会用复用其中的`view`，如果我们实现了这样一个有复用视图，***对于以后多个`imageView`、`tableView`等就有一个比较好的处理方法了***。
以上就是我写这个`pageListView`的原因， **1**.增强对于复用的理解 **2**.便于平常项目里面对于一些可重复使用的`view`做一个简单地集成封装。
其实我们要实现的`pageListView`复用的原理和`tableView`对于cell的复用的原理基本一致，只是简单了很多。因为我们目前考虑的都是一页一页的切换。所以不必考虑这个`view`的`size`和滑动完之后停留在两个`view`中间的情况。
简单说一下我们目前的思路，其实和`tableViewCell`复用的机制类似，创建两个池子，一个放可视区域的`views`，另一个放可复用`views`。当需要展示一个`view`，而复用池为空时，就创建一个新的`view`，并将这个`view`放在可视`views`池，如果一个`view`已经完全出了可视区域，就把它放入复用池，每次加载`view`的时候给`view`设置一下`frame`。
上面讲了最基本的思路，下面让我们来实现看看。
[https://github.com/redihd/PageListView](https://link.jianshu.com?t=https://github.com/redihd/PageListView)
这个是我实现一个可复用的`pageListView`的代码，想看的可以拉下来看一下。
下面我会用我实现的代码来做简单的讲解。
![](https://upload-images.jianshu.io/upload_images/302190-c00522a72e9caed5.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/616)
pageView.h
因为要实现的`view`很简单，所以我们在.h文件中也没有太多的属性与方法，主要就是有个三个`block`来实现类似`tableView`的`datasource` 与`delegate`。其中`loadViewAtIndexBlock` 需要两个传入参数，会有一个返回的view，这个`block`实现了类似`- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath;`这个方法。在接下来我们会细讲这个`block`的功能。
再来看.m文件里面的属性
![](https://upload-images.jianshu.io/upload_images/302190-36cb8f75b0afa70b.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/591)
pageview.m头
我们将一些不需要对外暴露的都放在了.m中，其中两个属性`visibleListViewsItems`,`dequeueViewPool`是实现能够复用`view`的`pageView`的关键。
`visibleListViewsItems` :这个可变字典用来存储可视区域的视图及其对应的index。
`dequeueViewPool`：这个可变set用来存储可复用的视图。
.m文件中大多数方法都是设置frame，位移等，我们这里不谈，主要介绍其中几个关键方法
![](https://upload-images.jianshu.io/upload_images/302190-416b5eb5cc40c8be.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/639)
loadDequeueViewAndVisibleViewsIfNeeded.png
`loadDequeueViewAndVisibleViewsIfNeeded`该方法主要是在scrollview的contentOffset.x变化时，计算应该要在可视视图字典中的视图，把旧的可视视图字典里面的视图移动到可重用视图池，然后将要新的展示的视图取出来，加入可视视图字典。
![](https://upload-images.jianshu.io/upload_images/302190-18808cd87a658ac7.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/512)
loadViewAtIndex
`loadViewAtIndex`该方法主要是从我们`datasource`的`block`获取要展示的view，这个view默认是从复用池即`visibleListViewsItems`任意取一个`view`来当做我们将要展示的`view`，并将其与index加入`visibleListViewsItems`，如果`block`提供的`view`为空，再新建一个`view`。然后将原来在`view`要展示的位置的视图（如果还有的话）移除，并加入`visibleListViewsItems`。
基本思路就是这样了
然后关于如何使用这个view
![](https://upload-images.jianshu.io/upload_images/302190-e1f8a292db3795b8.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/700)
initPageView.png
`totalPagesCountBlock` 这个`block`设置有多少个view要展示
`loadViewAtIndexBlock` 这个`block`处理可重用的视图
`pageViewClickBlock` 这个`block`处理点击事件
个人比较喜欢用block，所以把delegate和datasource都用block来做了。
最后跑一下，可以看到，即使设置了60000个，但是cpu和memory都还是很小。
![](https://upload-images.jianshu.io/upload_images/302190-287dffb95b55c131.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/700)run
最后，其实我们现在要实现这种视图，用横向的collectionView来做就好了。但是，写一个这种对于个人理解一些东西的实现还是有帮助的。接下来可能会把这个写的更详细，功能再多一些。
另外再就是自己用uiview实现一个缺一部分功能的scrollView。
作者：redihd
链接：https://www.jianshu.com/p/3e7c49833bb2
來源：简书
著作权归作者所有。商业转载请联系作者获得授权，非商业转载请注明出处。

