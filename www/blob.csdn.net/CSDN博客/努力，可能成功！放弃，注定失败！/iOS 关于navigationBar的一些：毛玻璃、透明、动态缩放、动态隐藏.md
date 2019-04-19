# iOS 关于navigationBar的一些：毛玻璃、透明、动态缩放、动态隐藏 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2016年03月23日 09:45:44[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：1951

# 前言
本文整理了一些关于navigationBar的非常规的但是较为实用的操作，包括利用毛玻璃、动态透明、动态item颜色、动态隐藏，以及头视图的动态缩放，并同时涉及了statusBar的动态设置（换色）。
先预览下整体效果：
![](http://upload-images.jianshu.io/upload_images/1468630-bece03c5d053b4f3.gif?imageMogr2/auto-orient/strip)
> 
Demo详见GitHub：[Demo_AboutNavigationBar](https://github.com/kukumaluCN/JXT_iOS_Demos)
为了便于展示，类没有复用，也没有继承共有的父类，所有存在大量“有丝分裂”的重复代码。。。说白了就是懒。。。千万别学我就是了。
# 1.不要“浪费”了这块毛玻璃
这里所谓的不要浪费，只是个人的偏好，当然也是顺遂了苹果的UI特色之一：毛玻璃穿透效果。
一般界面上是有两块毛玻璃的：navigationBar和tabBar，很多APP都会自定义这两块Bar，不仅是颜色，甚至是控件本身。单考虑利用系统的Bar的话，如果不自定义颜色，就是利用毛玻璃效果本身了，然而很多APP，也并未利用起这个毛玻璃效果，简书的就是这样，不过简书应该是自定义了navigationBar？或许。
（这里插一句，就是QQ的某次更新，更新之后，navigationBar的蓝色变成了毛玻璃，之后的一次更新又换了回来……可能是被吐槽太多？这里应该是用户习惯先入为主的问题了；还有就是微信，微信应该算得上是个十分纯粹的iOS风格的APP，有留意过的话，也会发现其navigationBar是黑色的毛玻璃，或许是`barStyle = UIBarStyleBlack`，因为微信也是有很多非原生的处理的。）
下面结合tableView介绍下其与navigationBar的毛玻璃的作用效果。知道我可能要啰嗦什么的朋友可自行跳过这部分内容。
先对比下这两种效果（图不太清晰，仔细看navigationBar，还是很容易对比出来差别的）：
![](http://upload-images.jianshu.io/upload_images/1468630-2c4bd0620fa6cc7a.gif?imageMogr2/auto-orient/strip)
有毛玻璃穿透效果的效果
![](http://upload-images.jianshu.io/upload_images/1468630-45eda0865e87e2f3.gif?imageMogr2/auto-orient/strip)
没有毛玻璃穿透效果
## 1.1.一般的处理
```
- (UITableView *)tableView
  {
      if (!_tableView) {
          _tableView = [[UITableView alloc] initWithFrame:CGRectMake(0, 64, ScreenWidth, ScreenHeight - 64)];
          _tableView.delegate = self;
          _tableView.dataSource = self;
          _tableView.backgroundColor = [UIColor whiteColor];
          _tableView.separatorStyle = UITableViewCellSeparatorStyleSingleLine;
      }
      return _tableView;
  }
```
相信有很多人大概是以与这样类似的方式去初始化tableView的，origin.y=64，总高度-64，都是为了防止navigationBar的遮挡，然而这样的处理会产生一个bug，就是tableView整体居然下移了64……，然后就有了这段解决代码：
```
if ([self respondsToSelector:@selector(automaticallyAdjustsScrollViewInsets)]) {
      self.automaticallyAdjustsScrollViewInsets = NO;
  }
```
这是iOS7的特性之一，navigationBar自适应scrollView滑动视图，然而因为我们的+64的处理，导致系统的这个默认开启的功能就成了自作多情。如果
`_tableView = [[UITableView alloc] initWithFrame:CGRectMake(0, 0, ScreenWidth, ScreenHeight)];`
这样做，之前的“bug”自然就会没有，而且我们想要的毛玻璃效果就会自然的展现出来（如上图一），但是因为系统的自适应，在某些特殊情况下，依旧会产生不可控的bug，所以刚才处理bug的代码，还是有必要的。
## 1.2.“释放”系统的毛玻璃效果的处理
配合：
```
if ([self respondsToSelector:@selector(automaticallyAdjustsScrollViewInsets)]) {
      self.automaticallyAdjustsScrollViewInsets = NO;
  }
```
这样初始化tableView：
```cpp
```cpp
- (UITableView *)tableView
  {
      if (!_tableView) {
          _tableView = [[UITableView alloc] initWithFrame:CGRectMake(0, 0, ScreenWidth, ScreenHeight)];
          _tableView.delegate = self;
          _tableView.dataSource = self;
          _tableView.backgroundColor = [UIColor whiteColor];
          _tableView.contentInset = UIEdgeInsetsMake(64, 0, 0, 0);
          _tableView.scrollIndicatorInsets = UIEdgeInsetsMake(64, 0, 0, 0);
          _tableView.separatorStyle = UITableViewCellSeparatorStyleSingleLine;
      }
      return _tableView;
  }
```
```
这里利用了`edgeInset`内嵌边距，tableView的frame依旧是全屏尺寸的，只是设置了下`contentInset`使得其内容视图的尺寸改变了，`scrollIndicatorInsets`是右侧的滑动指示器，要跟着一起才自然。如果当前页还有tabBar的话，只需要设置`UIEdgeInsetsMake`的`bottom`值就行。这样，上图一的毛玻璃穿透效果就自然的释放了出来。
## 1.3.设置`contentInset`带来的小问题
- **1.webView初始加载时底部的黑条**
如果使用上述方式初始化webView（同样是滑动视图，有scrollView属性，可设置`scrollView.contentInset`），至于所谓的“黑条”，大致类似于下面的效果：
![](http://upload-images.jianshu.io/upload_images/1468630-96d1fa4673e5ec14.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
黑条出现在视图底部，一般高度为64，也就是`contentInset`的`top`值，上图的因为设置关系要大很多。相信很多人都在浏览APP的webView时，在web加载完之前的空白页时，见到过这一现象。
关于这以现象的解决方法网上也有很多，我一般是在初始化时，将`contentOffset.y`设置一个（-64）的偏移量，这样，初始进入webView时，在不滑动页面的情况下，是不可见的，这算是较为温和的解决方法吧，还有一种就是，初始化时设置`opaque=NO`，也就是使得view透明，但是在加载之后，要设置`opaque=YES`，也就是默认不透明，这样，黑条是彻底不可见的。
- **2.滑动偏移量改变**
在实现一些滑动视图的动态变化的处理中，例如稍后提到的动态缩放，一般会用代理检测`contentOffset.y`值，但是因为设置了`contentInset.top`，此时`contentOffset.y`值也就发生了变化：
```
- (void)scrollViewDidScroll:(UIScrollView *)scrollView
{
    CGFloat offsetY = scrollView.contentOffset.y + self.tableView.contentInset.top;//注意
}
```
初始不再是0，而是-64，也就是`-top`。
## 1.4.为什么需要毛玻璃效果
费了一些功夫，都只是为了实现毛玻璃效果，至于为什么要这个毛玻璃穿透效果，我也说不好，喜好也罢，设计风格也罢，至少，感觉这样做出的APP，不用专门考虑各种Bar的配色什么的，动态模糊穿透的彩色，也的确很漂亮，至少，iOS的UI风格有这样的趋势。也不要小瞧这个毛玻璃效果，真正实现一个高效的毛玻璃，也就是动态模糊渲染，也是很麻烦的，一些三方库，为了版本适配（适配iOS7），自定义的毛玻璃，也是利用了系统的toolBar。看过一些文章，介绍来介绍去，还是说，用系统在iOS8之后提供的API实现毛玻璃是最好的方式。
下图（渣图……）是我最近封装了有一阵子的一个自定义控件，同样是为了效果，用toolBar自定义了一块毛玻璃，这个控件还未彻底完善，稍后会开源，下图只是其中一种样式，这个控件的自定义程度还是非常高的。
![](http://upload-images.jianshu.io/upload_images/1468630-5cf9da933daba493.gif?imageMogr2/auto-orient/strip)
# 2.navigationBar的透明
有时候，我们需要将navigationBar设置透明，但不是隐藏，因为还需要其item控件（返回键什么的），虽然navigationBar是继承于UIView的，但是直接设置其`alpha`是无效的，应该是因为navigationBar复合的视图层级：
![](http://upload-images.jianshu.io/upload_images/1468630-d3a5599d2f540d4b.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
![](http://upload-images.jianshu.io/upload_images/1468630-b7cd8c1f2b5aae13.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
根据视图层级关系，我们用这个十分简单的方法来设置navigationBar的透明： 
``[[[self.navigationController.navigationBar subviews] objectAtIndex:0] setAlpha:0];``
关于navigationBar设置透明的方法，不知上述一种，还有许多，详见此文，非常具有参考价值：
> 
[怎么把顶部这个navigationbar设置为透明呢，能够让下面的图片显示出来，但是返回按钮不透明？](http://www.zhihu.com/question/36659843)
如果想像QQ空间或者微博那样动态的改变透明度，只需要在`scrollViewDidScroll`方法中，动态去设置alpha值就行，何时开始改变、变化率全凭自定义的参数控制，具体详见Demo中的代码。
# 3.动态statusBar颜色
改变item的颜色很简单，只需设置：
`self.navigationController.navigationBar.tintColor = [UIColor blackColor];`
动态设置的方式同刚才的alpha。这里主要说的是statusBar的颜色或者说样式的改变。
iOS7之后，statusBar不再是全局属性，每个VC都可自行控制statusBar的样式，虽然样式只有简单的字体黑或白两种，但是在很多情况下都是很有用的，尤其是上面的navigationBar的alpha动态改变，在QQ空间中就有这个效果。
在设置statusBar样式之前，需要做一个处理，而且是针对navigationBar的处理，在使用了navigationController之后，直接设置某一个VC的statusBar的样式是无效的，因为navigationBar是唯一的，所有压栈推出的VC，都是navigationController的子控制器，这就需要指定statusBar样式改变的VC为当前的topVC，具体方式网上也有很多，这里只介绍个人使用的一种。
首先创建一个继承于UINavigationController的子类，在这个类中实现下面的方法：
```
- (UIViewController *)childViewControllerForStatusBarStyle
  {
      return self.topViewController;
  }
```
或者是同样效果的这个方法：
```
- (UIStatusBarStyle)preferredStatusBarStyle
  {
      UIViewController * topVC = self.topViewController;
      return [topVC preferredStatusBarStyle];
  }
```
之后，只需在要改变statusBar样式的VC类中实现：
```
#ifdef __IPHONE_7_0
  - (UIStatusBarStyle)preferredStatusBarStyle
  {
      if (_statusBarStyleControl) {
          return UIStatusBarStyleDefault;
      }
      else {
          return UIStatusBarStyleLightContent;
      }
  }
  - (BOOL)prefersStatusBarHidden
  {
      return NO;
  }
  #endif
```
`__IPHONE_7_0`是系统的宏，这里用来版本适配，这个不写貌似没有关系？因为之前试了iOS7以下的系统没有崩溃，iOS7之前没有这个方法，应该是不会执行的，也就不会崩溃。
`preferredStatusBarStyle`就是控制用来控制statusBar颜色或者说样式的，`_statusBarStyleControl`是自定义的一个用来动态控制的BOOL属性。
`prefersStatusBarHidden`这个控制statusBar的显示隐藏，建议NO或直接默认不写，如果设置隐藏，视图会整体上移20，效果不太好，看具体需求。
至于控制statusBar的改变，也是在`scrollViewDidScroll`代理中动态实现，例如某一情况下触发如下：
```
_statusBarStyleControl = YES;
if ([self respondsToSelector:@selector(setNeedsStatusBarAppearanceUpdate)]) {
    [self setNeedsStatusBarAppearanceUpdate];
}
```
第一步是设置之前提到的`_statusBarStyleControl`标志位的值的切换，第二部是最重要的`setNeedsStatusBarAppearanceUpdate`，这个系统的方法是在改变statusBar显示样式之前必须执行的，否则`preferredStatusBarStyle`不会再当前视图加载完成后再次执行。
# 4.navigationBar的动态隐藏
navigationBar的隐藏很简单：
`[self.navigationController setNavigationBarHidden:YES animated:YES];`
这个方法可以使动态隐藏时有动画效果，不会显得突兀。
动态隐藏的效果有两个场景：一个就是例如简书这样的，在浏览时，上滑，navigationBar隐藏，下滑navigationBar显示，在这期间，手指是不松开的，这需要实时检测当前是上滑还是下滑；第二个场景是Safari浏览器那样的，滑动后松手，根据上滑还是下滑设置隐藏（Safari的navigationBar不是隐藏，只是变化）。这样的两种场景虽然很相似，但就是松不松手的问题，处理方式和体验也是完全不同的。
- 1.第二场景，松手
这个处理十分简单：
```
- (void)scrollViewWillEndDragging:(UIScrollView *)scrollView withVelocity:(CGPoint)velocity targetContentOffset:(inout CGPoint *)targetContentOffset{
    NSLog(@"======== %lf", velocity.y);
    if(velocity.y > 0) {
        [self.navigationController setNavigationBarHidden:YES animated:YES];
    }
    else {
        [self.navigationController setNavigationBarHidden:NO animated:YES];
    }
}
```
velocity.y这个量，在上滑和下滑时，变化极小（小数），但是因为方向不同，有正负之分，这就很好处理了。
- 
2.第一场景，不松手
```
- (void)scrollViewDidScroll:(UIScrollView *)scrollView
{
    CGFloat offsetY = scrollView.contentOffset.y + _tableView.contentInset.top;//注意
    CGFloat panTranslationY = [scrollView.panGestureRecognizer translationInView:self.tableView].y;
    if (offsetY > 64) {
        if (panTranslationY > 0) { //下滑趋势，显示
            [self.navigationController setNavigationBarHidden:NO animated:YES];
        }
        else {  //上滑趋势，隐藏
            [self.navigationController setNavigationBarHidden:YES animated:YES];
        }
    }
    else {
        [self.navigationController setNavigationBarHidden:NO animated:YES];
    }
}
```
这里的`offsetY > 64`只是为了在视图滑过navigationBar的高度之后才开始处理，防止影响展示效果。
`panTranslationY`是scrollView的pan手势的手指位置的y值，这个方法是个人自己想到的，可能不是太好，因为`panTranslationY`这个值在较小幅度上下滑动时，可能都为正或都为负，这就使得这一方式不太灵敏，如果有更好的方法，欢迎留言交流。
# 5.动态缩放
这个效果纯粹是为了仿简书的个人信息的头像的缩放效果。
头像视图的布局直接利用了navigationBar的titleView，代码还是很好理解的，视图层级关系如下：
![](http://upload-images.jianshu.io/upload_images/1468630-016b294e7d7b3eb9.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
头像的一半是“悬空”的。
缩放只是滑动代理监听时的一个动态缩放的过程，缩放率同之前说的alpha的动态变化一样，自行设置常量，这里注意缩放过程的分段执行，为了效果，这个有下拉放大回弹的效果，这部分不是简书的效果：
```cpp
```cpp
_topImageView.transform = CGAffineTransformMakeScale(1 - offsetY/300, 1 - offsetY/300);
```
```
如果只是设置缩放，效果如下：
![](http://upload-images.jianshu.io/upload_images/1468630-badaebe491bbda11.gif?imageMogr2/auto-orient/strip)
为了使缩放过程中位置相对保持，之前想到了改变锚点的方式，然而效果很差，最终还是以简单的方式实现：
```
CGRect frame = _topImageView.frame;
frame.origin.y = 5;
_topImageView.frame = frame;
```
就是保持y的坐标为初始值。
下面的这个仿微博的效果，也是基于同样的原理，只是要注意图层关系，详细参考代码，不再赘述。
![](http://upload-images.jianshu.io/upload_images/1468630-e88b5640d8ad76b8.gif?imageMogr2/auto-orient/strip)
# 6.侧滑返回
这算是个比较大的话题了，前面提到的那篇文章中有关于此的详细论述，这里只是简单提及，稍后有机会，会专门写一篇关于侧滑返回的自定义实现。
iOS7之后，navigationController推出的视图，只要返回按钮不自定义覆盖，或者相关属性默认，右滑屏幕左边缘，可以直接pop返回，这是个十分方便的功能。关于实现和自定义实现，网上也有太多的文章。这里只是说明一点，就是在设置navigationBar透明之后，使用侧滑返回的过程中，navigationBar会突然出现，显得十分突兀，这也是目前微博APP的效果，QQ和微信都针对此做了不同的处理，本文的Demo中也尝试做了一定的处理，但是效果并不是十分理想的，就是中断侧滑返回动作时，navigationBar闪的问题。这个问题已经超出本文内容的范围，有机会会在今后的文章中介绍。
![](http://upload-images.jianshu.io/upload_images/1468630-404d3bf3dad45d2f.gif?imageMogr2/auto-orient/strip)
**参考文章：**
1.[iOS开发的一些奇巧淫技](http://www.cocoachina.com/ios/20141229/10783.html)
2.[自定义iOS7导航栏背景,标题和返回按钮文字颜色](http://blog.csdn.net/mad1989/article/details/41516743)
3.[Removing the title text of an iOS 7 UIBarButtonItem](http://stackoverflow.com/questions/19078995/removing-the-title-text-of-an-ios-7-uibarbuttonitem)
4.[iOS 实现ScrollView 上滑隐藏Navigationbar,下滑显示](http://www.jianshu.com/p/b43113256ce1)
5.[iOS 7 改变 app 的外观（NavigationBar，TabBar，StatusBar）](http://www.jianshu.com/p/972679ec8907)
6.[iOS不同版本适配问题(#ifdef __IPHONE_7_0)](http://blog.csdn.net/xyxjn/article/details/40425895)
7.[__IPHONE_OS_VERSION_MIN_REQUIRED](http://blog.csdn.net/maoxunxing/article/details/43969597)
8.[IOS开发之不同版本适配问题3(#ifdef __IPHONE_7_0 BaseSDK Development Target)](http://www.2cto.com/kf/201403/287164.html)
9.[IOS开发之不同版本适配问题2(#ifdef __IPHONE_7_0)](http://www.cnblogs.com/ios8/p/ios-version-com.html)
10.[ios7 statusBar的字体颜色怎么设置为白色的呢](http://www.cocoachina.com/bbs/read.php?tid=243110)
11.[IOS上 关于状态栏的相关设置(UIStatusBar)](http://doc.okbase.net/shede333/archive/94517.html)
12.[IOS7怎么修改Navigation Bar上的返回按钮文本颜色,箭头颜色以及导航栏按钮的颜色](http://www.itstrike.cn/Question/e0fd5e29-34ef-49fe-9a55-a018878199a2.html)
13.[怎么把顶部这个navigationbar设置为透明呢，能够让下面的图片显示出来，但是返回按钮不透明？](http://www.zhihu.com/question/36659843)
14.[iOS navigationbar全透明的方法](http://www.jianshu.com/p/2aba3be8328e)
15.[IOS中设置UINavigationBar的各种样式（图片/透明效果/下方内容显示情况)](http://www.2cto.com/kf/201412/358997.html)
16.[iOS开发UI篇—CAlayer层的属性](http://www.cnblogs.com/wendingding/p/3800736.html)
17.[iOS - UINavigationController简介](http://www.jianshu.com/p/3806f2bf4172)
[](http://www.jianshu.com/p/3806f2bf4172)
