# iOS-UINavigationController官方文档分析大总结 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2016年07月29日 20:20:53[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：2199

# iOS-UINavigationController官方文档分析大总结
字数3131
阅读114评论0喜欢6
# 一、概述
- 类继承关系如下:**UINavigationController<- UIViewcontroller <- UIResponder <- NSObject**.
- 它是一个容器类视图控制器,以栈的形式管理一组视图控制器，位于栈底的视图是其`rootViewController`,管理视图控制器个数理论上不受限制(实际受内存限制)，push和pop方法来弹入弹出控制器，最多只能显示一个视图控制器，那就是处于栈顶的视图控制器.
- 如果不给它添加视图控制器也不会报错，界面上也有视图，因为UINavigationController继承自UIViewController，也有自己的view，只不过默认情况下.view.backgroundColor为nil，即透明的。
- 若不给除根视图控制器之外的其他视图控制器设置**返回按钮**,UINavigationController会给他们设置**返回按钮(<返回)**.
- UINavigationController管理的对象有:UINavigationBar,UIToolBar.UIToolBar默认是隐藏的.
- UINavigationController允许开发者自定义UINavigationBar外观相关的属性,但是其`frame bounds alpha`不允许直接自定义,除非使用UINavigationBar的子类,
初始化UINavigationController时,方法为:
[initWithNavigationBarClass:toolbarClass:](https://developer.apple.com/library/ios/documentation/UIKit/Reference/UINavigationController_Class/#//apple_ref/occ/instm/UINavigationController/initWithNavigationBarClass:toolbarClass:)
- 自定义UINavigationBar,可使用 [UIAppearance](https://developer.apple.com/library/ios/documentation/UIKit/Reference/UIAppearance_Protocol/index.html#//apple_ref/occ/intf/UIAppearance),该API是全局修改,常在APPDelegate方法里面使用,比如全局设置标题的字体大小 颜色等.
# 二、常用函数
## 初始化函数:
[- initWithRootViewController:](https://developer.apple.com/library/ios/documentation/UIKit/Reference/UINavigationController_Class/#//apple_ref/occ/instm/UINavigationController/initWithRootViewController:)
[- initWithNavigationBarClass:toolbarClass:](https://developer.apple.com/library/ios/documentation/UIKit/Reference/UINavigationController_Class/#//apple_ref/occ/instm/UINavigationController/initWithNavigationBarClass:toolbarClass:)
## 跟导航栏相关的函数和属性
使用push方法能将某个控制器压入栈
`- (void)pushViewController:(UIViewController *)viewController animated:(BOOL)animated;`
使用`setViewControllers`一次压入多个控制器vc1->vc2->vc3，会显示最后的控制器vc3(处于栈顶)，代码如下：
```
UINavigationController *nav = [[UINavigationController alloc] init];
window.rootViewController = nav;// 创建3个测试控制器UIViewController *vc1 = [[UIViewController alloc] init];
vc1.view.backgroundColor = [UIColor blueColor];
UIViewController *vc2 = [[UIViewController alloc] init];
vc2.view.backgroundColor = [UIColor redColor];
UIViewController *vc3 = [[UIViewController alloc] init];
vc3.view.backgroundColor = [UIColor greenColor];// 最终会显示vc3[nav setViewControllers:@[vc1,vc2,vc3] animated:YES];
```
使用pop方法可以移除栈顶控制器当一个控制器被pop后，控制器内存就被释放了(会调用deinit/dealloc函数)：
``- (UIViewController *)popViewControllerAnimated:(BOOL)animated;``
一层一层的返回不方便，可以直接回到指定的控制器VC_A(处与VC_A与栈顶之间的控制器全被释放),下面代码执行后，VC_A处于栈顶：
``- (NSArray *)popToViewController:VC_A animated:(BOOL)animated;``
回到根控制器（栈底控制器）：
``-(NSArray *)popToRootViewControllerAnimated:(BOOL)animated;``
获取被管理的控制器
```
/// 当前管理的所有的控制器
@property(nonatomic,copy) NSArray<__kindof UIViewController *> *viewControllers;/// 栈顶控制器
@property(nullable, nonatomic,readonly,strong) UIViewController *topViewController;
/// 当前可见的VC，可能是topViewController，也可能是当前topViewController present(modal)出来的VC，总而言之就是可见的VC
@property(nullable, nonatomic,readonly,strong) UIViewController *visibleViewController;
```
注意，topViewController与visibleViewController大部分情况一样，也有可能不同
## NavigationBar不常用属性却非常有用的属性
[interactivePopGestureRecognizer](https://developer.apple.com/library/ios/documentation/UIKit/Reference/UINavigationController_Class/#//apple_ref/occ/instp/UINavigationController/interactivePopGestureRecognizer)
**关于这个属性的小结:**
- **注意:从iOS7开始，系统为UINavigationController提供了一个interactivePopGestureRecognizer用于右滑返回(pop),但是，如果自定了back button或者隐藏了navigationBar，该手势就失效了。**
- 所以如果若想使用这个功能,则返回按钮最好使用系统的.
**若自定义了返回按钮,又想"右滑返回的手势有效"的解决办法:**
- 方法一:在视图基类中自定义手势返回,但是没有系统的渐变的效果.博主不推荐使用该方法.
- 网上查的解决办法是:[UINavigationController返回手势失效问题](http://chisj.github.io/blog/2015/05/27/uinavigationcontrollerfan-hui-shou-shi-shi-xiao-wen-ti/)
```
在UINavigationController基类中添加代码如下:
遵守协议是 <UIGestureRecognizerDelegate>
切记:使用此方法时  self.hidesBarsOnSwipe = YES;不能使用,即默认是关闭的,否则会造成冲突,在右滑返回时 会出现navigationBar不正常显示的问题,如有兴趣可自己尝试一下.
- (void)viewDidLoad {
    [super viewDidLoad];
//    self.hidesBarsOnSwipe = YES;
//    self.toolbarHidden = NO;
    self.hidesBottomBarWhenPushed = YES;
    self.interactivePopGestureRecognizer.delegate =  self;
}
- (BOOL)gestureRecognizerShouldBegin:(UIGestureRecognizer *)gestureRecognizer {
    if (self.viewControllers.count <= 1 ) {
        return NO;
    }
    return YES;
}
```
- **关于右滑返回的总结是:若想使用该属性,则最好使用系统返回键.**
下面这些属性在某些情况下会使用到
- 在小屏4s 5s上有些PM可能会要求在浏览内容时隐藏导航栏,这个时候就可使用此属性.
[hidesBarsOnTap](https://developer.apple.com/library/ios/documentation/UIKit/Reference/UINavigationController_Class/#//apple_ref/occ/instp/UINavigationController/hidesBarsOnTap)(点击隐藏导航栏,再点击显现,皆有向上/下过度的过程)
[hidesBarsOnSwipe](https://developer.apple.com/library/ios/documentation/UIKit/Reference/UINavigationController_Class/#//apple_ref/occ/instp/UINavigationController/hidesBarsOnSwipe)(经试验是向上轻扫隐藏,向下轻扫则显现,皆有向上/下过度的过程)
[hidesBarsWhenVerticallyCompact](https://developer.apple.com/library/ios/documentation/UIKit/Reference/UINavigationController_Class/#//apple_ref/occ/instp/UINavigationController/hidesBarsWhenVerticallyCompact)
[hidesBarsWhenKeyboardAppears](https://developer.apple.com/library/ios/documentation/UIKit/Reference/UINavigationController_Class/#//apple_ref/occ/instp/UINavigationController/hidesBarsWhenKeyboardAppears)
[navigationBarHidden](https://developer.apple.com/library/ios/documentation/UIKit/Reference/UINavigationController_Class/#//apple_ref/occ/instp/UINavigationController/navigationBarHidden)
[barHideOnTapGestureRecognizer](https://developer.apple.com/library/ios/documentation/UIKit/Reference/UINavigationController_Class/#//apple_ref/occ/instp/UINavigationController/barHideOnTapGestureRecognizer)
[barHideOnSwipeGestureRecognizer](https://developer.apple.com/library/ios/documentation/UIKit/Reference/UINavigationController_Class/#//apple_ref/occ/instp/UINavigationController/barHideOnSwipeGestureRecognizer)
## ToolBar属性
[- setToolbarHidden:animated:](https://developer.apple.com/library/ios/documentation/UIKit/Reference/UINavigationController_Class/#//apple_ref/occ/instm/UINavigationController/setToolbarHidden:animated:)
[toolbarHidden](https://developer.apple.com/library/ios/documentation/UIKit/Reference/UINavigationController_Class/#//apple_ref/occ/instp/UINavigationController/toolbarHidden)
# 三、导航条
UINavigationController是做导航用的，具体的操作大部是由导航条来完成，导航条的使用就显得很重要。导航条的内容由控制器的navigationItem属性决定。
1 navigationItem的属性
一般使用self.navigationItem.对应属性来获取属性，或者设置属性。或者使用self.navigationController获取到navigationController，再通过navigationController获取到想要设置的viewController
中间的标题文字
`@property(nullable, nonatomic,copy) NSString *title;`
中间标题视图
`@property(nullable, nonatomic,strong) UIView *titleView;`
项目中的常用方法是:
```cpp
```cpp
UIImageView *titleView = [[UIImageView alloc]initWithFrame:CGRectMake(SCREEN_WIDTH/2 - 100, 0, 200, 44)];
titleView.image = 图片.
[self.navigationItem.titleView addSubview:titleView];
```
```
导航栏附加解释说明，如果设置了此字段，导航栏会高出30个点显示此字段在title正上方
@property(nullable,nonatomic,copy) NSString *prompt;
prompt显示位置
![](http://upload-images.jianshu.io/upload_images/639412-90f8246e8b0dff0a.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
自定义左上角的返回按钮
```
/// 直接设置
@property(nullable, nonatomic,strong) UIBarButtonItem *leftBarButtonItem;
```
大部分情况下，我们需要指定左边返回按钮距离左边框的距离，可以如下设定：
```
//自己项目中的方法
 UIButton *backBt = [UIButton buttonWithType:UIButtonTypeSystem];
 backBt.frame = CGRectMake(0, 0, LabelWidth_20, LabelWidth_20);
 [backBt setBackgroundImage:[UIImage imageNamed:@"回退"] forState:UIControlStateNormal];
 [backBt addTarget:self action:@selector(back) forControlEvents:UIControlEventTouchUpInside];
 self.navigationItem.leftBarButtonItem = [[UIBarButtonItem alloc]initWithCustomView:backBt];
//系统推荐方法如下
UIBarButtonItem *leftItem = [[UIBarButtonItem alloc] initWithImage:[UIImage imageNamed:@"gobackItem.png"] style:UIBarButtonItemStylePlain target:self action:@selector(backViewcontroller)];
UIBarButtonItem *fixedItem = [[UIBarButtonItem alloc] initWithBarButtonSystemItem:UIBarButtonSystemItemFixedSpace target:nil action:nil];
// 设置边框距离，个人习惯设为-16，可以根据需要调节fixedItem.width = -16;
self.navigationItem.leftBarButtonItems = @[fixedItem, leftItem];
```
下图为设置边框距离前后的差别:
没有设置边框距离
![](http://upload-images.jianshu.io/upload_images/639412-94d6c2a0b3206ae7.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
设置边框距离后
![](http://upload-images.jianshu.io/upload_images/639412-be69037734bc54ec.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
**忽略点:(关于系统返回按钮)**
- 子导航条后退按钮，假设通过VC1 push VC2，那么如果设置VC1.navigationItem.backBarButtonItem就会显示在VC2的左上角返回按钮；
如果再设置VC2.navigationItem.leftBarButtonItem则会覆盖VC1的设置；
如果VC1和VC2都没有设置，则会显示默认的backBarButtonItem。
`@property(nullable,nonatomic,strong) UIBarButtonItem *backBarButtonItem;`
- push到下一个页面时,若不自定义返回按钮,则会显示默认的返回按钮 **"<返回"** 字体颜色为系统字体颜色,即backBarButtonItem,且系统的向左平移pop的功能会自己实现.
- 若自定义了返回按钮 则系统自带的向左平移pop的功能会失效.
- title的长度不可过长,否则会造成系统返回按钮的 **返回** 字体消失,只剩余一个**<**
自定义右上角的按钮，或多个按钮
```
@property(nullable, nonatomic,strong) UIBarButtonItem *rightBarButtonItem;/// 一次设置多个按钮
@property(nullable,nonatomic,copy) NSArray<UIBarButtonItem *> *rightBarButtonItems;
```
2 设置navigationItem的字体格式
```
// 字体大小19，颜色为白色
[nav.navigationBar setTitleTextAttributes:@{NSFontAttributeName:[UIFont systemFontOfSize:19],NSForegroundColorAttributeName:[UIColor whiteColor]}];
```
四、UIToolBar
UINavigationController自带了一个工具栏，通过[self.navigationController setToolbarHidden:NO];来显示工具栏，工具栏中的内容可以通过viewController的toolbarItems来设置，显示的顺序和设置的NSArray中存放的顺序一致，每一个UIBarButtonItem对象都可以设定点击事件，可以使用系统提供的很多常用风格的对象，也可以根据需求进行自定义，下面举例使用系统提供的样式。
```
// 1 显示工具条
[self.navigationController setToolbarHidden:NO];
// 2 创建四个UIBarButtonItem
UIBarButtonItem *itemOne = [[UIBarButtonItem alloc] initWithBarButtonSystemItem:UIBarButtonSystemItemAdd target:nil action:nil];
UIBarButtonItem *itemTwo = [[UIBarButtonItem alloc] initWithBarButtonSystemItem:UIBarButtonSystemItemSave target:nil action:nil];
UIBarButtonItem *itemThree = [[UIBarButtonItem alloc] initWithBarButtonSystemItem:UIBarButtonSystemItemAction target:nil action:nil];
UIBarButtonItem *itemFour = [[UIBarButtonItem alloc] initWithBarButtonSystemItem:UIBarButtonSystemItemSearch target:nil action:nil];
// 间隙
UIBarButtonItem *flexibleItem = [[UIBarButtonItem alloc] initWithBarButtonSystemItem:UIBarButtonSystemItemFlexibleSpace target:nil action:nil];
// 3 添加到toolbarItemsvc.
toolbarItems = @[itemOne,flexibleItem,itemTwo,flexibleItem,itemThree,flexibleItem,itemFour];
```
效果如下：
![](http://upload-images.jianshu.io/upload_images/639412-d7b36a5f41b0d039.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
另外，UIToolBar使用的比较少，大部分情况下而是使用另一个导航控制器UITabBarController
# 五 UINavigationControllerDelegate
有两个常用的方法
```
// 一般用于传递参数，或者做一些其它处理
- (void)navigationController:(UINavigationController *)navigationController willShowViewController:(UIViewController *)viewController animated:(BOOL)animated;
- (void)navigationController:(UINavigationController *)navigationController didShowViewController:(UIViewController *)viewController animated:(BOOL)animated;
```
参考：[http://www.cnblogs.com/mddblog/p/4556974.html](http://www.cnblogs.com/mddblog/p/4556974.html)
# 六 UINavigationBar、UINavigationItem、UIToolbar与UIBarButtonItem四者关系
补充:
## 1 三者概述
- 
UITabBar 上展示的是: UITabBarItem( 其类继承关系为--> UIBarItem --> NSObject)
- 
UIToolBar 上展示的是: UIToolBarItem (其类继承关系为-->UIBarButtonItem --> UIBarItem--> NSObject)
- 
UINavigationBar 上展示的是: UINavigationItem(包括:titleView leftBarButtonItem leftBarButtonItems rightBarButtonItem rightBarButtonItems) (其类继承关系为--> UIBarButtonItem --> UIBarItem--> NSObject)
## 2 三者区别
|名称|位置|作用|高度|appearance|
|----|----|----|----|----|
|UITabBar|任意位置(常见底部)|点击前后item发生变化,管理一组视图|49|全局修改UI|
|UIToolBar|任意位置(常见底部)|一般用作功能性点击|44|全局修改UI|
|UINavigationBar|顶部|一般初始化UINavigationController时,自动生成UIUINavigationBar,也可以单独使用|44|全局修改UI|
**NavigaitonBar**
是导航栏，位于屏幕的上方，管理整个NavigationController的navigationItem，它类似navigationcontroller一样提供了一个栈来管理UINavigationItem，在编程时，一般只设置每个控制器的navigationItem属性.一个导航控制器管理多个视图控制器（多个视图控制器共享一个导航控制器），而一个导航控制器只有一个UINavigationBar，被管理的多个视图控制器共享这一个UINavigationBar，只要一个视图控制器改变了UINavigationBar的属性则影响是全局的。每个视图控制器都会有属于自己的UINavigationItem，系统会以懒加载的方式创建一个UINavigationItem显示在UINavigationBar中，改变UINavigationItem
只会在当前控制器起作用，不会影响其它控制器。
**Toolbar**
显示在屏幕底部，是导航控制器的工具栏，一个导航控制器只有一个，在任何被管理的视图控制器地方改变则会都改变。可以一次性添加多个UIBarButtonItem或按钮（包装成UIBarButtonItem后添加），有一个items数组属性。
**UIBarButtonItem**
是UINavigationItem或者Toolbar具体的一个按钮。
**UITabBar外观相关的属性和方法:**
[自定义TabBar的方法参考:](http://blog.csdn.net/xn4545945/article/details/35994863)
[barStyle](https://developer.apple.com/reference/uikit/uitabbar/1623454-barstyle?language=objc)The tab bar style that specifies its appearance.
[translucent](https://developer.apple.com/reference/uikit/uitabbar/1623458-translucent?language=objc)A Boolean value that indicates whether the tab bar is translucent.
[barTintColor](https://developer.apple.com/reference/uikit/uitabbar/1623445-bartintcolor?language=objc)The tint color to apply to the tab bar background.
[itemPositioning](https://developer.apple.com/reference/uikit/uitabbar/1623468-itempositioning?language=objc)The positioning scheme for the tab bar items in the tab bar.
[itemSpacing](https://developer.apple.com/reference/uikit/uitabbar/1623446-itemspacing?language=objc)The amount of space (in points) to use between tab bar items.
[itemWidth](https://developer.apple.com/reference/uikit/uitabbar/1623465-itemwidth?language=objc)The width (in points) of tab bar items.
[tintColor](https://developer.apple.com/reference/uikit/uitabbar/1623460-tintcolor?language=objc)The tint color to apply to the tab bar items.
[backgroundImage](https://developer.apple.com/reference/uikit/uitabbar/1623469-backgroundimage?language=objc)The custom background image for the tab bar.
[shadowImage](https://developer.apple.com/reference/uikit/uitabbar/1623452-shadowimage?language=objc)The shadow image to use for the tab bar.
[selectionIndicatorImage](https://developer.apple.com/reference/uikit/uitabbar/1623456-selectionindicatorimage?language=objc)The image to use for the selection indicator.
**UINavigationBar外观相关的属性和方法:**
[backIndicatorImage](https://developer.apple.com/reference/uikit/uinavigationbar/1624942-backindicatorimage?language=objc)The image shown beside the back button.
[backIndicatorTransitionMaskImage](https://developer.apple.com/reference/uikit/uinavigationbar/1624938-backindicatortransitionmaskimage?language=objc)The image used as a mask for content during push and pop transitions.
[barStyle](https://developer.apple.com/reference/uikit/uinavigationbar/1624955-barstyle?language=objc)The navigation bar style that specifies its appearance.
[barTintColor](https://developer.apple.com/reference/uikit/uinavigationbar/1624931-bartintcolor?language=objc)The tint color to apply to the navigation bar background.
[shadowImage](https://developer.apple.com/reference/uikit/uinavigationbar/1624963-shadowimage?language=objc)The shadow image to be used for the navigation bar.
[tintColor](https://developer.apple.com/reference/uikit/uinavigationbar/1624937-tintcolor?language=objc)The tint color to apply to the navigation items and bar button items.
[translucent](https://developer.apple.com/reference/uikit/uinavigationbar/1624928-translucent?language=objc)A Boolean value indicating whether the navigation bar is translucent ([YES](https://developer.apple.com/reference/objectivec/yes?language=objc)) or not ([NO](https://developer.apple.com/reference/objectivec/no?language=objc)).
[- backgroundImageForBarMetrics:](https://developer.apple.com/reference/uikit/uinavigationbar/1624962-backgroundimageforbarmetrics?language=objc)Returns the background image for given bar metrics.
[- setBackgroundImage:forBarMetrics:](https://developer.apple.com/reference/uikit/uinavigationbar/1624926-setbackgroundimage?language=objc)Sets the background image for given bar metrics.
[- backgroundImageForBarPosition:barMetrics:](https://developer.apple.com/reference/uikit/uinavigationbar/1624940-backgroundimageforbarposition?language=objc)Returns the background image to use for a given bar position and set of metrics.
[- setBackgroundImage:forBarPosition:barMetrics:](https://developer.apple.com/reference/uikit/uinavigationbar/1624968-setbackgroundimage?language=objc)Sets the background image to use for a given bar position and set of metrics.
[- titleVerticalPositionAdjustmentForBarMetrics:](https://developer.apple.com/reference/uikit/uinavigationbar/1624966-titleverticalpositionadjustmentf?language=objc)Returns the title’s vertical position adjustment for given bar metrics.
[- setTitleVerticalPositionAdjustment:forBarMetrics:](https://developer.apple.com/reference/uikit/uinavigationbar/1624959-settitleverticalpositionadjustme?language=objc)Sets the title’s vertical position adjustment for given bar metrics.
[titleTextAttributes](https://developer.apple.com/reference/uikit/uinavigationbar/1624953-titletextattributes?language=objc)Display attributes for the bar’s title text.
**UIToolBar外观相关的属性和方法:**
[barStyle](https://developer.apple.com/reference/uikit/uitoolbar/1617994-barstyle?language=objc)The toolbar style that specifies its appearance.
[barTintColor](https://developer.apple.com/reference/uikit/uitoolbar/1618002-bartintcolor?language=objc)The tint color to apply to the toolbar background.
[tintColor](https://developer.apple.com/reference/uikit/uitoolbar/1617995-tintcolor?language=objc)The tint color to apply to the bar button items.
[translucent](https://developer.apple.com/reference/uikit/uitoolbar/1618001-translucent?language=objc)A Boolean value that indicates whether the toolbar is translucent ([YES](https://developer.apple.com/reference/objectivec/yes?language=objc)) or not ([NO](https://developer.apple.com/reference/objectivec/no?language=objc)).
[- backgroundImageForToolbarPosition:barMetrics:](https://developer.apple.com/reference/uikit/uitoolbar/1617998-backgroundimagefortoolbarpositio?language=objc)
[- setBackgroundImage:forToolbarPosition:barMetrics:](https://developer.apple.com/reference/uikit/uitoolbar/1618003-setbackgroundimage?language=objc)
[- shadowImageForToolbarPosition:](https://developer.apple.com/reference/uikit/uitoolbar/1618000-shadowimagefortoolbarposition?language=objc)
[- setShadowImage:forToolbarPosition:](https://developer.apple.com/reference/uikit/uitoolbar/1617991-setshadowimage?language=objc)
关于详细使用方法可参考:[UINavigationBar 使用总结:](http://www.jianshu.com/p/f0d3df54baa6)写的很详细.
