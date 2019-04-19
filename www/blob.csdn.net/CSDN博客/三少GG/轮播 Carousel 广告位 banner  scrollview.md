# 轮播/Carousel/广告位/banner***scrollview - 三少GG - CSDN博客
2016年02月18日 16:30:21[三少GG](https://me.csdn.net/scut1135)阅读数：1029
个人分类：[IOS 常规](https://blog.csdn.net/scut1135/article/category/6071587)

开源方案候选：
https://github.com/gsdios/SDCycleScrollView       “iOS第一图片轮播器” 
**实现原理解释：**
http://shengpan.net/pscarouselview/
# 谈谈组件封装的思路和实现--PSCarouselView
前两天面试了一个应聘者，他的演示项目里有广告轮播功能。恰好之前我封装过一个实现了此功能的控件，于是就顺着他广告轮播的实现一直往下聊，从需求的抽象一直聊到各种实现的细节和需要考虑的问题等等。组件的封装是开发中比较有趣的一件事。今天我们就拿轮播控件举例，聊聊组件的封装。
授人予渔先要授人予鱼。[先给出鱼(PSCarouselView)](https://github.com/DeveloperPans/PSCarouselView)，再慢慢讲渔。
## 分析需求
一般来说，我们在封装组件的时候，会先思考以下几点
- 
这个组件要做什么
- 
这个组件至少需要知道哪些信息
- 
这个组件会反馈哪些信息
这个阶段，我们还不会考虑组件的具体实现，仅仅对其做抽象。我们要做的东西不仅仅是适用于单个项目的，而应该是通用的，可以适应大部分同种类需求的。
#### Scene I
BA说，客户要在首页加个轮播。于是半天后设计师出了张图，看起来大概像这样：
![](http://shengpan.net/content/images/2015/11/-----1.jpg)
我们按照上面的思路，拆分一下这个需求。
### 这个组件要做什么
显而易见的：
> 
1.展示多张图片
2.可以向左向右翻页
3.PageControl的状态会根据图片的滚动而相应改变
隐含可能要做的：
> 
1.支持左右两侧无限循环滚动
2.自动轮播
3.支持手动滑动
4.支持点击并进行相关的响应
5.图片的缓存
### 这个组件至少需要知道哪些信息
一个封装得优秀的复杂度不高组件就像一个魔法盒子，只需要触发启动开关，就可以达到你期待的效果。极简的触发参数和条件是组件封装的精髓。在内容型App中，轮播图一般会用作推荐内容展示区域。在O2O类App中，轮播图一般会用作广告位。因此轮播区域图片的内容绝大多数都是动态的。在一般的C/S开发中，客户端要获取存储在服务器上的图片会获取它的URL,然后在需要的时候根据URL异步地加载这些图片。因此，我们找出了的轮播空间第一个必备条件：
一个图片URL地址数组。
```
@property (nonatomic, strong) NSArray *imageURLs;/**< 必须赋值。只要给这个imageURL赋值，会自动获取图片*/
```
这个时候我们已经可以根据数组内的URL,数组内URL的数量，完成轮播效果了。但还不够完善。试想，当数组为空的时候，我们的轮播控件是一个什么状态？网络比较慢，图片还在加载中的时候，我们的轮播控件是什么状态？就目前来说----空白一片。
每个App都有每个App的设计，但无论如何谁也不会容忍首页最醒目的部分出现一大片空白，因此占位图是必须的。我们告诉我们的控件，“没有图片的时候别傻愣着，显示这张图”。必备条件二：
占位图
```
@property (nonatomic, strong) UIImage *placeholder;/**< 没有轮播图时的占位图*/
```
有了以上信息我们的轮播控件已经可以在运行得比较好了。但是缺少了一定的定制性，能满足的需求面还不够广。比如说，让控件在需要自动轮播的地方自动轮播，在不需要自动轮播的地方保留无限滚动的特性，可以手动拨动？（之前有个产品提过这样的需求）比如说,这个控件是否应该提供可选的PageControl实现？依据设计师给的图不同，我们可能需要实现不同的PageControl，不管是颜色，布局，还是其可见隐藏都可能会有不同。这一部分复杂度不应该归到轮播控件中。但考虑到一些快速开发的需求,控件还是提供了一个默认选项。这些都是让控件变得更加可配置和灵活（同时也变得复杂）的可选信息。综上我们大概可以整理出以下属性：
```
@property (nonatomic,getter=isAutoMoving)      BOOL autoMoving;/**< 是否自动轮播,默认为NO*/
@property (nonatomic,getter=isShowPageControl) BOOL showPageControl;/**< 是否展示默认的PageControl，默认为YES*/
```
### 这个组件会反馈哪些信息
上一点中，如果使用者需要自己实现PageControl，那么当前控件滚动到哪一页了，应该给出反馈。
不论在O2O的App中，还是资讯类App中，用户点击轮播必定会有相对应的响应，例如之乎日报点击跳转到对应的文章，淘宝点击查看大图等。因此用户点击的信息也必须反馈出来，以让控制器根据用户的操作进行不同的响应。
View层对Controller层的反馈一般有两种，一种是Target-Action，一种是Delegate。此处我们类似UITableView,给出一定的代理方法。遵循苹果的设计规范，让使用的开发者容易上手。
```java
@protocol PSCarouselDelegate <NSObject>
@optional
/**
 *  告诉代理滚动到哪一页了
 *
 *  @param carousel self
 *  @param page     已经计算好，直接使用即可
 */
- (void)carousel:(PSCarouselView *)carousel didMoveToPage:(NSUInteger)page;
/**
 *   告诉代理用户点击了某一页
 *
 *  @param carousel
 *  @param index  imageURL的index  
 */
- (void)carousel:(PSCarouselView *)carousel didTouchPage:(NSUInteger)page;
@end
```
至此，轮播组件的架子已经明晰，剩下的就是如何实现，以及实现的时候需要考虑的一些细节。
## 实现过程
### 选择基类
苹果的UIKit提供了非常多优秀的类，可以作为我们轮播视图的基类。根据以上的分析，我们根据直觉，立马可以联想到以下三个类：`UIPageViewController`、`UIScrollView`、`UICollectionView`。
`UIPageViewController`提供了非常好的翻页封装，仅需要指定翻页的方向，和子ViewController的数组，就可以提供类似轮播的功能。并且`UIPageViewController`提供了供翻页的接口和必要的反馈信息，定义在`UIPageViewControllerDelegate`协议中。用UIPageViewController,我们只需要将每个轮播页封装成一个仅包含一个UIImageView的ViewController，并将其设为UIPageViewController的ViewControllers，并实现它的代理方法就很容易实现我们的轮播图。
`UIScrollView`提供了最基本的滚动封装。采用UIScrollView作为基类，需要自己根据图片的数量计算ContentSize,并在ScrollView相对应的代理方法中，根据Frame相关信息来计算页码，会比UIPageViewController稍微复杂。但好在，它是一个`View`。
`UICollectionView`做为UIScrollView的子类提供了比UIScrollView更好的封装，也提供了"翻页"的接口，并提供了一系列定位CollectionView的状态信息。相比UIScrollView,他提供了更深层的封装。同UIPageViewController一样，非常适合本作为本组件的基类。同时，当UICollectionView的一些代理方法不足以提供相关信息时，还可以通过UIScrollView的代理方法来解决。
采用`UIPageViewController`作为子类，考虑到使用者需要将其作为subViewController添加到它们的项目中，没有将一个View添加到ViewController中来得直观，综合考虑，PSCarouselView选择了UICollectionView作为其基类。
### 实现功能
如何利用UICollectionView在有限的图片数量下实现无限轮播？我们只需要N+2个Cell就可以实现无限轮播。如图
![](http://shengpan.net/content/images/2015/11/----001.jpeg)
将imageURL数组中的URL,扩充为N+2。同时，将imageURLs的最后一个URL作为新数组的第一个URL,将imageURLs的第一个URL作为新数组的最后一个URL。然后在我们的CollectionView滚动到最后一个Cell时，跳转到第二个URL表示的IndexPath即可。自动轮播采用计时器来完成。在实现的时候，我们需要注意一下的几点
1.计时器的开启与暂停
自动轮播，通常我们会使用到计时器NSTimer。当我们在页面切换的过程中，需要注意计时器的开启与暂停，不然可能会出现一些不可预料的BUG。而一个ViewController的生命周期，View是不可能知道的。因此我们需要提供两个接口，供使用者开启/暂停计时器。
```cpp
- (void)startMoving;
- (void)stopMoving;
```
另外，我们还需要注意App的生命周期，当App进入不活动的状态时，我们也需要将计时器暂停，并在回到活动状态时相应地启动。
```
#pragma mark - Notification
//程序被暂停的时候，应该停止计时器
- (void)applicationWillResignActive
{
    [self stopMoving];
}
//程序从暂停状态回归的时候，重新启动计时器
- (void)applicationDidBecomeActive
{
    if (self.isAutoMoving)
    {
        [self startMoving];
    }
}
```
2.用户操作与自动轮播的冲突 
用户操作时如果轮播还是继续滚动，会导致用户产生不可控的错觉。《iOS Human Interface Guide》中强调过App需要给于用户控制感。因此，在用户手动"翻页"的时候，我们需要暂时暂停一下自动轮播，并在用户手动"翻页"完成后，重新进行轮播。
```
//用户手动拖拽，暂停一下自动轮播
- (void)scrollViewWillBeginDragging:(UIScrollView     *)scrollView
{
    [self removeTimer];
}
//用户拖拽完成，恢复自动轮播（如果需要的话）
- (void)scrollViewDidEndDecelerating:(UIScrollView *)scrollView
{
    if (self.isAutoMoving)
    {
        [self addTimer];
    }
}
```
3.图片的缓存与展示 
如何通过图片的URL来获得相对应的图片，并做好图片的缓存，以实现最大限度地节省流量？这个模块其实应该单独做一个有关图片缓存的库，而不应该包含到View层中来。本作采用了SDWebImage来实现这一功能。有兴趣的朋友可以尝试着实现一个图片缓存库。
以上，一个轮播组件的封装基本上完成了。更多实现上的细节请参看GitHub上的[Demo](https://github.com/DeveloperPans/PSCarouselView)。
That's all.Hope you enjoy it :)
