# ios开发-3D折叠菜单的简单实现 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2016年09月01日 22:34:01[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：1206
> 
在Github看到一个老外写的一个3D折叠效果的侧滑菜单demo，效果很不错，想收藏起来为以后项目开发所用。老外的demo是Swift版的，趁最近比较有时间安静下来学习，于是，我决定把它封装成OC版的。
先看看效果 
![](http://upload-images.jianshu.io/upload_images/1528347-d2c5f03455d0cc54.gif?imageMogr2/auto-orient/strip)
LeftMenuGif.gif
#### 原理
> 
1、使用storyboard来创建整个UI框架，能比较直观地看到这个结构其实很简单
![](http://upload-images.jianshu.io/upload_images/1528347-56b868631b65a267.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
UI框架.png
> 
2、主要由左侧列表控制器`MenuViewController` 和右边的内容控制器`DetailViewController`构成，这两个控制器分别接上一个导航控制器后分别连到`ContainerviewController`上。
- 下面是`ContainerviewController`视图布局，通过`scrollView`作为容器来操作左右滑动
![](http://upload-images.jianshu.io/upload_images/1528347-0dda65ee3a25c673.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
主视图.png
- *scrollView*记得打开*PagingEnabled*
![](http://upload-images.jianshu.io/upload_images/1528347-a522d40ce55f513c.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
PagingEnabled
- 其中右边的Container宽度为屏幕的宽度，这样就能通过左右滑动改变`scrollView`的`contentOffset`值来显示或者隐藏左边的菜单
![](http://upload-images.jianshu.io/upload_images/1528347-2b3a77659bbb8313.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
ContainerviewController.png
#### 代码（部分关键代码）
##### 1、ContainerViewController.m
> 
在storyboard里面拖出`scrollview`和左边的`menuContainerView`这个两个属性，在`viewDidLayoutSubviews`里面设置`menuContainerView`的[anchorPoint](http://blog.csdn.net/yongyinmg/article/details/37927833)
```
-(void)viewDidLayoutSubviews
{
    [super viewDidLayoutSubviews];
    self.menuContainView.layer.anchorPoint = CGPointMake(1.0, 0.5);
    [self hideOrShowMenu:NO animated:NO]; //初始化为隐藏菜单
}
```
> 
控制`menuContainerView`展开或者隐藏，并且在上面的`viewDidLayoutSubviews`方法里面初始化为隐藏状态
```
- (void)hideOrShowMenu:(BOOL)isShow animated:(BOOL)isAnimated
{
    CGFloat xOffset = CGRectGetWidth(self.menuContainView.bounds);
    CGPoint contentOffset = isShow?CGPointZero:CGPointMake(xOffset, 0);
    //通过设置contentOffset来隐藏或者显示菜单
    [self.scorllView setContentOffset:contentOffset animated:isAnimated];
    _isShow = isShow;
}
```
> 
在scrollView的`scrollViewDidScroll`代理方法里面设置左右滑动的判断以及3D效果
```
-(void)scrollViewDidScroll:(UIScrollView *)scrollView
{
    CGFloat multiplier = 1.0 / CGRectGetWidth(self.menuContainView.bounds);
    CGFloat offset = scrollView.contentOffset.x * multiplier;
    CGFloat fraction = 1.0 - offset;
    self.menuContainView.layer.transform = [self transformForFraction:fraction];
    self.menuContainView.alpha = fraction;
    HamburgerView *rotatingView = (HamburgerView *)_detailVC.hamburger;
    [rotatingView rotateWithFraction:fraction];
    scrollView.pagingEnabled = scrollView.contentOffset.x < (scrollView.contentSize.width - CGRectGetWidth(scrollView.frame));
    CGFloat menuOffset = CGRectGetWidth(self.menuContainView.bounds);
    _isShow = !CGPointEqualToPoint(CGPointMake(menuOffset, 0), scrollView.contentOffset);
}
```
```
- (CATransform3D)transformForFraction:(CGFloat)fraction
{
    CATransform3D identity = CATransform3DIdentity;
    identity.m34 = -1.0 / 1000.0;
    double angle = (1.0 - fraction) * -M_PI_2;
    CGFloat xOffset = CGRectGetWidth(self.menuContainView.bounds) * 0.5;
    CATransform3D rotateTransform = CATransform3DRotate(identity, angle, 0.0, 1.0, 0.0);
    CATransform3D translateTransform = CATransform3DMakeTranslation(xOffset, 0.0, 0.0);
    return CATransform3DConcat(rotateTransform, translateTransform);
}
```
##### 2、MenuViewController.m
> 
这是菜单tableview控制器，主要控制菜单每一项的背景图片和颜色以及点击事件
```
///加载plist文件中的数据资源
- (void)viewDidLoad {
    [super viewDidLoad];
    NSString *path = [[NSBundle mainBundle]pathForResource:@"MenuItems" ofType:@"plist"];
    _dataArray = [NSArray arrayWithContentsOfFile:path];
}
```
```
///tableViewCell点击响应，这里用通知的方法来通知detailView控制器
-(void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath
{
    [tableView deselectRowAtIndexPath:indexPath animated:YES];
    NSDictionary *dict = _dataArray[indexPath.row];
    [[NSNotificationCenter defaultCenter]postNotificationName:@"KCallToShowInDetailViewNotification" object:dict];
}
```
##### 3、HamburgerView.m
> 
展开菜单的点击按钮，这里叫‘汉堡包’，哈。这个view里面需要创建一个imageView视图，还需要一个控制imageView旋转的方法，如下, （这个方法在`scrollViewDidScroll`中会调用到）
```
- (void)rotateWithFraction:(CGFloat)fraction
{
    CGFloat angle = fraction * M_PI_2;
    _imageView.transform = CGAffineTransformMakeRotation(angle);
}
```
#### 小结
> 
小结一下：这个侧滑菜单的demo，个人感觉效果还可以，灵活运用scrollview的特性，实现起来也不复杂，以后的项目中可以参照这种UI框架来实现该功能。
附上项目代码：
老外的demo：[https://github.com/chris-pilcher/3D-Sidebar-Animation](https://github.com/chris-pilcher/3D-Sidebar-Animation)
我的demo：[https://github.com/RanYeee/RNLeftSildeMenu](https://github.com/RanYeee/RNLeftSildeMenu)
*请各位大神多多指导！
文／Rany_IOS（简书作者）
原文链接：http://www.jianshu.com/p/188b8b40aa15
著作权归作者所有，转载请联系作者获得授权，并标注“简书作者”。
