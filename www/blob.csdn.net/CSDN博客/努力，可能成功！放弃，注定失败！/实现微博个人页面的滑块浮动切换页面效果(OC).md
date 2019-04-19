# 实现微博个人页面的滑块浮动切换页面效果(OC) - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2016年08月29日 08:45:11[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：1058
#### 前言
> 
这篇文章真的是不想写的, 因为之前分享过一篇用swift实现的相同的效果, 无奈很多的朋友总是问有没有oc版的, 好吧, 也可以理解在swift不太普及的情况下出现这样的事情, 看来最近真的是闲的慌, 每天在交流群里跟大家吹牛不说, 还把这个用oc写了一遍, 不过实现的过程和原来的swift有一点不一样, 因为对应的oc版的ZJScrollPageView更新了, 但是swift的并没有更新.[demo地址](https://github.com/jasnig/ZJScrollPageView)
## 最终效果
![](http://upload-images.jianshu.io/upload_images/1271831-26182de246fd424d.gif?imageMogr2/auto-orient/strip)
示例效果.gif
一` 实现的思路
, 大多和swift中一样只是增加了一个scrollView
## 构思: 利用监控和设置UIScrollView的偏移量来实现
![](http://upload-images.jianshu.io/upload_images/1271831-8f8e270d988a7f1c.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
层级结构
.
- 
层级结构, 第一层为控制器的view, 第二层为containerView, 第三层为contentView, 第四层为 headView(为了实现header也能滑动, 添加了一个scrollView在他的下面)和segmentView
- 
第三层的contentView,设置contentView的大小和控制器的view的大小一样, 来显示子控制器的view的内容, 需要设置他的子控制器view的tableView(collectionVie)的初始偏移量为segmentView和headView的高度之和,同时将segmentView和headView添加到view上面, 以实现tableView在滚动的时候segmentView和headView可以同步滚动
- 
subview的添加顺序, 先添加contentView, 然后再添加segmentView和headView(因为contentView和view大小一样, 若是在后面添加就将segmentView和headView遮住了)
- 
同步滚动原理: 监控子控制器的scrollView的滚动偏移量(contentOffset.y), 根据偏移量的改变量来同步的调整segmentView和headView的frame, 这里的监控之前我是使用Closure来实现,但是后面需要更新tableView的偏移量有需要一个Closure, 所以就改为了delegate来实现.
- 
使segmentView浮动: 通过监控子控制器的scrollView的滚动偏移量(contentOffset.y), 根据偏移量的改变量来同步的调整segmentView和headView的frame, 当segmentView同步"滚动"到顶部的时候, 通过判断scrollView的滚动偏移量的范围来固定segmentView和headView的frame, 即达到浮动效果
- 
在segmentView和headView的frame随着scrollView滚动到下方原始位置的时候, 通过判断scrollView的滚动偏移量的范围来固定segmentView和headView的frame
二. 实现部分 .
- 这些懒加载中设置了具体的ZJScrollPageView的属性和初始化的frame
![](http://upload-images.jianshu.io/upload_images/1271831-5f8cf7111703b2a6.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
Snip20160706_7.png
2, #pragma ZJScrollPageViewDelegate 代理方法
```
- (NSInteger)numberOfChildViewControllers {
    return self.titles.count;
}
- (UIViewController<ZJScrollPageViewChildVcDelegate> *)childViewController:(UIViewController<ZJScrollPageViewChildVcDelegate> *)reuseViewController forIndex:(NSInteger)index {
    UIViewController<ZJScrollPageViewChildVcDelegate> *childVc = reuseViewController;
    if (!childVc) {
        childVc = [[ZJPageViewController alloc] init];
    }
    if (index%2==0) {
        childVc.view.backgroundColor = [UIColor blueColor];
    } else {
        childVc.view.backgroundColor = [UIColor redColor];
    }
    // 设置代理, 用于处理子控制器的滚动
    _currentChildVc = (ZJPageViewController *)childVc;
    _currentChildVc.delegate = self;
    return childVc;
}
```
3, 相应子控制器的scrollView的滚动, 在这里面调整segmentView等的位置,
这里面会处理三种情况,
- 第一种是向上滚到滑块的位置在navigationBar的时候, 需要使得继续向上滚的时候滑块停在这里
- 第二种是向下滚动直到headView完全显示出来的时候, 需要处理
- 第三种就是上面两种之间的时候, 需要让segmentView等同步滚动
- 下面的代码就是分别处理这三种情况
```
- (void)scrollViewIsScrolling:(UIScrollView *)scrollView {
    _childOffsetY = scrollView.contentOffset.y;
    self.currentOffsetY = _childOffsetY + defaultOffSetY;
//    NSLog(@"%f", _currentOffsetY);
    if (self.currentOffsetY <= 0 ) {
// 让headView停在navigationBar下面
        self.segmentView.zj_y = -_childOffsetY - segmentViewHeight;
        self.scrollView.zj_y = self.segmentView.zj_y - headViewHeight;
    }
    else if (self.currentOffsetY>=headViewHeight) {
        // 使滑块停在navigationBar下面
        self.scrollView.zj_y = naviBarHeight - headViewHeight;
        self.segmentView.zj_y = naviBarHeight;
    }
    else {
        // 这里是让滑块和headView随着上下滚动
        self.segmentView.zj_y = -_childOffsetY - segmentViewHeight;
        self.scrollView.zj_y = self.segmentView.zj_y - headViewHeight;
        // "递归"
        if (self.scrollView.contentOffset.y == self.currentOffsetY) {
            return;
        }
        [self.scrollView setContentOffset:CGPointMake(0, self.currentOffsetY)];
    }
}
```
4, 处理headView的滚动
```
- (void)scrollViewDidScroll:(UIScrollView *)scrollView {
    self.currentOffsetY = scrollView.contentOffset.y;
// 这样的方式达到, 底部的scrollView滚动,但是headView在屏幕上的位置不变, 
    self.headView.zj_y = self.currentOffsetY;
    if (self.currentOffsetY < 0) {
        self.containerView.zj_y = -self.currentOffsetY;
        return;
    } else {
        self.containerView.zj_y = 0;
    }
    if (_currentChildVc.tableView.contentOffset.y == self.currentOffsetY - defaultOffSetY) {
        return;
    }
  // 同步滚动子控制器的scrollView
    [_currentChildVc.tableView setContentOffset:CGPointMake(0, self.currentOffsetY - defaultOffSetY)];
}
```
5, 当切换页面的时候, 需要根据当前页面的scrollView的偏移量以及当前的segmentView等的位置, 来整体的调整, 同时在悬停的时候保留每个页面的滚动偏移量, 这里处理两种情况
- 新页面出来的时候滑块并没有到上面悬停的位置, 这个时候, 将新页面的scrollView偏移量置为初始的
- 新页面出来的时候滑块在上面悬停的位置, 这个时候, 使用新页面的scrollView偏移量, 并且调整各个控件的位置
```
- (void)setupScrollViewOffSetYWhenViewWillAppear:(UIScrollView *)scrollView {
    dispatch_block_t setHeight = ^ {
        _childOffsetY = scrollView.contentOffset.y;
        [scrollView setContentSize:CGSizeMake(0, MAX(scrollView.bounds.size.height - naviBarHeight - segmentViewHeight, scrollView.contentSize.height))];
    };
    if (_childOffsetY < -(naviBarHeight + segmentViewHeight)) {
        [scrollView setContentOffset:CGPointMake(0, _childOffsetY)];
        setHeight();
        return;
    } else {
        if (scrollView.contentOffset.y < -(naviBarHeight + segmentViewHeight)) {
            [scrollView setContentOffset:CGPointMake(0, -(naviBarHeight + segmentViewHeight))];
            // 使滑块停在navigationBar下面
            self.scrollView.zj_y = naviBarHeight - headViewHeight;
            self.segmentView.zj_y = naviBarHeight;
            setHeight();
            return;
        }
        setHeight();
        return;
    }
}
```
6, 子控制器中, 需要使用代理, 通知, kvo, block等方式让父控制器知道当前的scrollView(tableView, collectionView均可)的偏移量即可, 这里我使用了代理来实现
```
#pragma ZJScrollPageViewChildVcDelegate
// 每次页面出现的时候会调用, 这个时候传递当前的偏移量
- (void)setUpWhenViewWillAppearForTitle:(NSString *)title forIndex:(NSInteger)index firstTimeAppear: (BOOL)isFirstTime {
    [self.delegate setupScrollViewOffSetYWhenViewWillAppear:self.tableView];
    if(isFirstTime) {
        // 加载数据
    } else {
        //刷新...
    }
}
#pragma UIScrollViewDelegate 传递滚动的偏移量
- (void)scrollViewDidScroll:(UIScrollView *)scrollView {
    [self.delegate scrollViewIsScrolling:scrollView];
}
- (void)viewDidLoad {
    [super viewDidLoad];
    self.tableView = [[UITableView alloc] initWithFrame:self.view.bounds style:UITableViewStylePlain];
    self.tableView.delegate = self;
    self.tableView.dataSource = self;
    [self.tableView registerClass:[UITableViewCell class] forCellReuseIdentifier:cellId];
// 这里设置初始的内容便宜 200+64+44 这个数字是父控制器中的常量, 这里偷个懒
    [self.tableView setContentInset:UIEdgeInsetsMake(200+64+44, 0, 0, 0)];
    [self.view addSubview:self.tableView];
}
```
> 
#### 然后呢就简单的实现了这种效果, 其中可能会有问题, 欢迎交流, [demo地址](https://github.com/jasnig/ZJScrollPageView)
文／ZeroJ（简书作者）
原文链接：http://www.jianshu.com/p/e6d2bd624e85
著作权归作者所有，转载请联系作者获得授权，并标注“简书作者”。
