# 做一个广告无限轮播的UICollectionView组件吧 - weixin_33985507的博客 - CSDN博客
2016年03月11日 00:01:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：3
## 前言
广告无限轮播，随处可见的功能，也有很多iOS Developer实现过这个组件，在这说一下我的实现方式，算是一个总结吧。
这个功能有两种实现方式，老的做法是使用UIScrollView，这个方法可能需要多写一些逻辑代码；新的方法就是使用UICollectionView，因为UICollectionView的一些特性，相对于UIScrollView在代码和逻辑上能够省去不少功夫。本文着重介绍的就是使用UICollectionView实现无限轮播的功能。
## 原理
在最前面多添加最后一张图片，在最后面多添加第一张图片，图片和cell个数都+2，然后在滑动到cell（0）时换到cell（n-1），滑动到cell（n）时换到cell（1），因为对应的cell上的图片是相同的，在切换的时候取消动画效果，这样在视觉上是看不出来的。
## 实现过程
原理其实很简单。正常情况下，应该是多少张图片对应多少个cell，也就是说图片的数量和cell的数量保持一致，但是这样就无法实现轮播的效果了。我们采取的办法是，先处理图片，在第一张图片前面添加一张最后一张图片，然后在最后一张图片后面添加一张第一张图片，这样图片的个数就+2了，创建的cell响应的也+2，假设我们要展示2张图片，经过处理后，应该是这个样子的：
![1647894-7d6c4518927ea334.png](https://upload-images.jianshu.io/upload_images/1647894-7d6c4518927ea334.png)
2张图片要对应4个cell
对于初始数组的处理代码，我们可以这么写：
- (NSArray *)reloadDataArrayWithDataArray:(NSArray *)dataArray {
NSMutableArray *resultArray = [[NSMutableArray alloc] initWithCapacity:dataArray.   count];
[resultArray addObject:dataArray.lastObject];
```
for (id object in dataArray) {
        [resultArray addObject:object];
    }
    
    [resultArray addObject:dataArray.firstObject];
    return resultArray;
}
```
如果这样的话，默认显示的就是image2了，而不是我们想要的image1，这时候在初始化的时候我们需要做点小手脚，通过下面的方法换到cell1就可以了
```
[self.carouselCollectionView scrollToItemAtIndexPath:[NSIndexPath indexPathForItem:_currentItem inSection:0]
                                    atScrollPosition:UICollectionViewScrollPositionNone
                                            animated:NO];
```
经过处理后，应该是这个样子的：
![1647894-cfeddb9b7a85af9f.png](https://upload-images.jianshu.io/upload_images/1647894-cfeddb9b7a85af9f.png)
红框中的cell为默认显示内容
OK！按照之前的过程，我们已经可以看到这样的效果了，默认进来显示的image1，无论向左还是向右滑动，显示的都是image2，貌似是我们想要的轮播效果，但是继续滑动总会有到头的时候，而我们想要有轮播效果，就必须保证当前显示的cell不是第一个或者最后一个，也就是说当前显示的cell前后必须都有cell，这样才可以在滑动的时候不会出现到边缘无法滑动的情况。上图中，只有cell1、cell2才是可以轮播的位置，那么我们只需要在滑动到cell0的时候换到cell2、滑动到cell3的时候换到cell1，还是之前用过的方法来实现：
```
if (self.currentItem == 0) {
    toItem = self.dataArray.count-2;
    NSIndexPath *indexPath = [NSIndexPath indexPathForItem:toItem inSection:0];
    [self.carouselCollectionView scrollToItemAtIndexPath:indexPath
                                        atScrollPosition:UICollectionViewScrollPositionNone
                                                animated:NO];
    self.currentItem = toItem;
}
if (self.currentItem == self.dataArray.count-1) {
    toItem = 1;
    NSIndexPath *indexPath = [NSIndexPath indexPathForItem:toItem inSection:0];
    [self.carouselCollectionView scrollToItemAtIndexPath:indexPath
                                        atScrollPosition:UICollectionViewScrollPositionNone
                                                animated:NO];
    self.currentItem = toItem;
}
```
那么我们在什么时候进行这个交换cell的操作呢？UIScrollViewDelegate的代理方法：
- (void)scrollViewDidEndDecelerating:(UIScrollView *)scrollView {
[self reloadItemAction];
}
```
- (void)reloadItemAction {
    NSIndexPath *indexPath = [[self.carouselCollectionView indexPathsForVisibleItems]   lastObject];
    self.currentItem = indexPath.item;
    NSInteger toItem = 0;
    
    if (self.currentItem == 0) {
        toItem = self.dataArray.count-2;
        NSIndexPath *indexPath = [NSIndexPath indexPathForItem:toItem inSection:0];
        [self.carouselCollectionView scrollToItemAtIndexPath:indexPath
                                            atScrollPosition:UICollectionViewScrollPositi   onNone
                                                    animated:NO];
        self.currentItem = toItem;
    }
    
    if (self.currentItem == self.dataArray.count-1) {
        toItem = 1;
        NSIndexPath *indexPath = [NSIndexPath indexPathForItem:toItem inSection:0];
        [self.carouselCollectionView scrollToItemAtIndexPath:indexPath
                                            atScrollPosition:UICollectionViewScrollPositi   onNone
                                                    animated:NO];
        self.currentItem = toItem;
    }
    
    self.pageControl.currentPage = self.currentItem-1;
}
```
看完刚才的代码，你会有疑问，`self.pageControl`，这个什么鬼？对于这个组件来说，只显示图片肯定是不行的，我们还需要`UIPageControl`来显示当前图片的位置，也就是说当前显示的是第几张图片。因为我们的cell数量实际上是比传入的图片数量+2的，所以我们在操作`UIPageControl`的`currentPage`时，需要注意这一点。
还有我们会需要点击某一张图片进行详情的查看或者跳转到一个其他的网址之类的，这时需要实现一个点击事件，我采用的传入自定义的block参数，然后在`UICollectionView`的`didSelectItemAtIndexPath`方法中进行回调，和`UIPageControl`的`currentPage`一样，我们需要注意`indexPath.item`的值：
- (void)collectionView:(UICollectionView *)collectionView didSelectItemAtIndexPath:(    NSIndexPath *)indexPath {
```
if (self.didSelectItemBlock) {
        self.didSelectItemBlock(indexPath.item-1);
    }
}
```
通过上面的处理，我们就可以实现无限轮播的功能了。
**但是**
只有轮播是不够的，有的时候我们还需要它能自己进行轮播，而不是人为的滑动，在之前基础上，初始化时我们加一个定时器就可以了
- (void)loadTimer {
self.timer = [NSTimer timerWithTimeInterval:self.timeInterval
target:self
selector:@selector(timerChanged)
userInfo:nil
repeats:YES];
[[NSRunLoop currentRunLoop] addTimer:self.timer
forMode:NSRunLoopCommonModes];
}
定时器事件方法中这么实现：
- (void)timerChanged {
if (self.currentItem == self.dataArray.count-2) {
NSInteger toItem = 0;
NSIndexPath *indexPath = [NSIndexPath indexPathForItem:toItem inSection:0];
[self.carouselCollectionView scrollToItemAtIndexPath:indexPath
atScrollPosition:UICollectionViewScrollPositi   onNone
animated:NO];
self.currentItem = toItem;
}
```
self.currentItem++;
    
    NSIndexPath *indexPath = [NSIndexPath indexPathForItem:self.currentItem inSection:0];
    [self.carouselCollectionView scrollToItemAtIndexPath:indexPath
                                        atScrollPosition:UICollectionViewScrollPositionLe   ft
                                                animated:YES];
    self.pageControl.currentPage = self.currentItem-1;
}
```
OK！广告可以自动进行轮播了，伸手上去滑动两下，发现问题了，手势和定时器发生冲突了，我们可以在手势开始时停止定时器，手势停止时重新开启定时器，这样就可以解决冲突了UIScrollViewDelegate中的两个方法：
- (void)scrollViewWillBeginDecelerating:(UIScrollView *)scrollView {
if (self.timer) {
[self.timer invalidate];
}
}
```
- (void)scrollViewDidEndDecelerating:(UIScrollView *)scrollView {
    [self reloadItemAction];
    
    if (self.timer) {
        [self loadTimer];
    }
}
```
现在应该是完整的广告无限轮播组件了。对外两个简答的接口方法，一个带定时器的，一个不带定时器的：
- (instancetype)initWithFrame:(CGRect)frame
dataArray:(NSArray *)dataArray
didSelectItemBlock:(void (^)(NSInteger didSelectItem))block;
```
- (instancetype)initWithFrame:(CGRect)frame
                    dataArray:(NSArray *)dataArray
                 timeInterval:(CGFloat)timeInterval
           didSelectItemBlock:(void (^)(NSInteger didSelectItem))block;
```
使用的时候，可以这样：
#define __LOADIMAGE(file, type) [UIImage imageWithContentsOfFile:[[NSBundle mainBundle]pathForResource:file ofType:type]]
```
- (void)loadCarouselCollectionView {
    // 不使用定时器初始化
    UCCarouselView *carouselView = [[UCCarouselView alloc] initWithFrame:({
        CGRectMake(0.f, 20.f,
                   CGRectGetWidth([UIScreen mainScreen].bounds),
                   200.f);
    }) dataArray:[self loadData] didSelectItemBlock:^(NSInteger didSelectItem) {
        
        NSLog(@"didSelectItem is :%ld", (long)didSelectItem);
        
    }];
    [self.view addSubview:carouselView];
    
    // 使用定时器初始化
//    UCCarouselView *carouselView = [[UCCarouselView alloc] initWithFrame:({
//        CGRectMake(0.f, 20.f,
//                   CGRectGetWidth([UIScreen mainScreen].bounds),
//                   200.f);
//    }) dataArray:[self loadData] timeInterval:2.f didSelectItemBlock:^(NSInteger    didSelectItem) {
//        
//        NSLog(@"didSelectItem is :%ld", (long)didSelectItem);
//    
//    }];
//    [self.view addSubview:carouselView];
}
// Demo 数据
- (NSArray *)loadData {
    NSArray *array = @[__LOADIMAGE(@"dota2_0", @"jpg"),
                       __LOADIMAGE(@"dota2_1", @"jpg"),
                       __LOADIMAGE(@"dota2_2", @"jpg"),
                       __LOADIMAGE(@"dota2_3", @"jpg")];
    return array;
}
```
欢迎各位iOS Developer踊跃指出Bug、意见！希望本文能够对您有所帮助！
[完整的Demo源码看这里！](https://link.jianshu.com?t=https://github.com/XiaoyueWang/UCCarouselView.git)如果感觉还可以来个star也是不错的！
