# iOS文档补完计划--UIView - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2018年12月14日 13:28:40[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：35
![](https://upload-images.jianshu.io/upload_images/1552225-d148da8b556a5ab2.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1000/format/webp)
> 
UIView可以说是我们日常工作中接触最多的一个对象、是所有视图控件(不包括视图控制器)的基类。
主要的功能包括视图样式、层级、约束、自动布局、渲染、手势、动画、坐标转换等等。
其中有些东西(比如原生自动布局、而我们平时都用mas/sd)并不常用、所以只筛选了一部分平时可能用得到的地方。
由于内容实在太多、所以有些复杂的地方只是简单总结一下并且给出一些参考链接方便查阅
目录主要分为以下几个样式:
**常用**、会用、*了解*
## [目录]()
- **创建视图对象**
- **配置视图的视觉外观**- **backgroundColor**
- **hidden**
- **alpha**
- *opaque*
- tintColor
- *tintAdjustmentMode*
- **clipsToBounds**
- *clearsContextBeforeDrawing*
- **maskView**
- layerClass
- layer
- **配置与事件相关的行为**- **userInteractionEnabled**
- multipleTouchEnabled
- exclusiveTouch
- **配置边界和框架矩形**- **frame**
- **bounds**
- center
- **transform**
- **管理视图层次结构**- **superview**
- **subviews**
- *window*
- **addSubview**
- **removeFromSuperview**
- bringSubviewToFront
- sendSubviewToBack
- insertSubview:atIndex:
- insertSubview:aboveSubview:
- insertSubview: belowSubview:
- exchangeSubviewAtIndex:withSubviewAtIndex:
- **isDescendantOfView:**
- 观察与视图层级的更改
	- didAddSubview/willRemoveSubview等六个方法
- 配置内容边距
	- *LayoutMargins相关*
- **屏幕的安全区域**- **safeAreaInsets**
- **safeAreaLayoutGuide**
- safeAreaInsetsDidChange
- insetsLayoutMarginsFromSafeArea
- **测量Auto Layout**- **systemLayoutSizeFittingSize**
- systemLayoutSizeFittingSize:withHorizontalFittingPriority:verticalFittingPriority
- **intrinsicContentSize**
- **invalidateIntrinsicContentSize**
- **Compression Resistance priority(抗压缩)**
- **Hugging priority(抗拉伸)**
- **触发自动布局**- needsUpdateConstraints
- **setNeedsUpdateConstraints**
- **updateConstraints**
- **updateConstraintsIfNeeded**
- **配置调整大小行为**- **contentMode**
- **UIViewContentMode**
- **sizeThatFits**
- sizeToFit
- autoresizesSubviews
- autoresizingMask
- **布局子视图**- **layoutSubviews**
- **setNeedsLayout**
- **layoutIfNeeded**
- *requiresConstraintBasedLayout*
- *translatesAutoresizingMaskIntoConstraints*
- **绘制和更新视图**- **drawRect**
- **setNeedsDisplay**
- **setNeedsDisplayInRect**
- *contentScaleFactor*
- tintColorDidChange
- **管理手势识别器**- **添加删除和获取**
- **gestureRecognizerShouldBegin**
- *观察焦点*- *canBecomeFocused*
- *focused*
- *运动视觉效果*- *添加删除和获取*
- **后台启动恢复**
- 捕获视图快照
- **识别视图**- **tag**
- **viewWithTag**
- **坐标系转换**- **convertPoint**
- **convertRect**
- **超出父视图的View可以被点击**
- **命中测试(Hit-Testing)**- **hitTest:withEvent**
- **pointInside:withEvent**
- **为响应链寻找最合适的FirstView**
- **结束视图编辑**- **endEditing:**
- Block动画
- 首尾式动画
### UIView
> 
包含了UIView的基本功能
- 
userInteractionEnabled
> 
设置用户交互，默认YES允许用户交互
```
@property(nonatomic,getter=isUserInteractionEnabled) BOOL userInteractionEnabled;
```
- 
tag
> 
控件标记(父控件可以通过tag找到对应的子控件)，默认为0
```
@property(nonatomic)  NSInteger tag;
```
- 
观察焦点
- 
管理用户界面方向
**semanticContentAttribute(翻转效果)**
> 
是否翻转视图
```
@property (nonatomic) UISemanticContentAttribute semanticContentAttribute NS_AVAILABLE_IOS(9_0);
```
[世上总是有很多奇人异事、比如阿拉伯人的阅读顺序。](https://www.jianshu.com/p/34b5a8d9a77e)
可以让布局自动左右翻转、不过前提是布局时使用Leading以及Trailing两个约束条件而不是Left和Right。
**获取视图方向**
```
/** 获取视图的方向 */
+ (UIUserInterfaceLayoutDirection)userInterfaceLayoutDirectionForSemanticContentAttribute:(UISemanticContentAttribute)attribute NS_AVAILABLE_IOS(9_0);
 
/** 获取相对于指定视图的界面方向 */
+ (UIUserInterfaceLayoutDirection)userInterfaceLayoutDirectionForSemanticContentAttribute:(UISemanticContentAttribute)semanticContentAttribute relativeToLayoutDirection:(UIUserInterfaceLayoutDirection)layoutDirection NS_AVAILABLE_IOS(10_0);
 
/** 返回即时内容的布局的方向 */
@property (readonly, nonatomic) UIUserInterfaceLayoutDirection effectiveUserInterfaceLayoutDirection NS_AVAILABLE_IOS(10_0);
```
### UIViewGeometry(几何分类)
- 
multipleTouchEnabled
> 
是否允许多点触摸 默认NO
```
@property(nonatomic,getter=isMultipleTouchEnabled) BOOL multipleTouchEnabled;
```
### 创建视图对象
```
/** 通过Frame初始化UI对象 */
- (instancetype)initWithFrame:(CGRect)frame NS_DESIGNATED_INITIALIZER;
/** 用于xib初始化 */
- (nullable instancetype)initWithCoder:(NSCoder *)aDecoder NS_DESIGNATED_INITIALIZER;
```
### 配置视图的视觉外观
- 
backgroundColor
> 
视图背景色
```
@property(nonatomic, copy) UIColor *backgroundColor;
```
默认值nil、也就是透明。
- 
hidden
> 
是否隐藏视图
```
@property(nonatomic, getter=isHidden) BOOL hidden;
```
父视图的隐藏会导致子视图被隐藏。并且不能被点击、不能成为第一响应者
- 
alpha
> 
视图透明度
```
@property(nonatomic) CGFloat alpha;
```
父视图的透明度会应用到子视图上。小于0.01则等于被隐藏了。
- 
opaque
> 
视图是否不透明。主要是用于视图混合
```
@property(nonatomic, getter=isOpaque) BOOL opaque;
```
UIView的默认值是YES、但UIButton等子类的默认值都是NO。
需要注意他并不是我们肉眼层面的透明。
[这和视图合成机制有关](https://blog.csdn.net/wzzvictory/article/details/10076323)
简而言之如果你的视图`alpha=1`、完全可以将`opaque=YES`。让GPU在混合视图时不必考虑下方视图颜色。
- 
tintColor
> 
色调颜色
```
@property(nonatomic, strong) UIColor *tintColor;
```
- `父视图`更改了tintColor为red，那么它所有的一级`子视图`tintColor全部为red。下一级也会根据前一级进行设置。
	除非你主动设置了子视图的tintColor。
- `原生控件`基本都有`默认的tintColor`、比如`UIButton为蓝色`。
	当然、也和创建的方式有关。`UIButtonTypeCustom`方式是没有的。
- 当tintColor被修改、将会调用对象的`tintColorDidChange:`方法。
	个人感觉就这个玩意比较有用、毕竟我们更多的是自定义控件着色。不可能希望全屏幕变成一个颜色。
想进一步了解的话。这里推荐一个博客可以看一看: [《iOS tintColor解析》](https://www.cnblogs.com/wfwenchao/p/4884833.html)
- 
tintAdjustmentMode
> 
色调(tintColor)模式
```
typedef NS_ENUM(NSInteger, UIViewTintAdjustmentMode) {
    UIViewTintAdjustmentModeAutomatic,//视图的着色调整模式与父视图一致
    
    UIViewTintAdjustmentModeNormal,//默认值
    UIViewTintAdjustmentModeDimmed,//暗色
} NS_ENUM_AVAILABLE_IOS(7_0);
@property(nonatomic) UIViewTintAdjustmentMode tintAdjustmentMode NS_AVAILABLE_IOS(7_0);
```
改变这个属性、也会调用`tintColorDidChange:`方法。
- 
clipsToBounds
> 
是否截取掉超过子视图超过自身的部分、默认为NO
```
@property(nonatomic)  BOOL  clipsToBounds;
```
最大的用处还是切圆角和图片吧。
需要注意的是`layer`有一个方法`maskToBounds`也是一个作用、`clipsToBounds`内部就是调用了`maskToBounds`。
其实效果一样、只不过从语义上来讲分成`View`和`layer`两个方法。
- 
clearsContextBeforeDrawing
> 
视图重绘前是否先清理以前的内容，默认YES
```
@property(nonatomic)  BOOL   clearsContextBeforeDrawing;
```
如果你把这个属性设为NO、那么你要保证能在 drawRect：方法中正确的绘画。
如果你的代码已经做了大量优化、那么设为NO可以提高性能、尤其是在滚动时可能只需要重新绘画视图的一部分。
所以说、通常用不到。
- 
maskView
> 
遮罩层
```
@property(nullable, nonatomic,strong) UIView *maskView NS_AVAILABLE_IOS(8_0);
```
- 虽说是遮罩层、但实际上不会多出一个View。
	只是对颜色的混合有影响。
- 只会显示出与maskView可见(不透明)部分重叠的部分。
- maskView的对应点的alpha会赋值给View对应的point。
- 与layer.mask基本相同、只是需要8.0的支持。
举一个简单的例子：
```
UIView * view1 = [[UIView alloc]initWithFrame:CGRectMake(0, 100, 200, 200)];
view1.backgroundColor = [UIColor blueColor];
UIView * maskView = [[UIView alloc]initWithFrame:view1.bounds];
maskView.backgroundColor = [UIColor clearColor];
UIView * view_1 = [[UIView alloc]initWithFrame:CGRectMake(0, 0, 100, 200)];
view_1.backgroundColor = [UIColor whiteColor];
[maskView addSubview:view_1];
UIView * view_2 = [[UIView alloc]initWithFrame:CGRectMake(100, 0, 100, 200)];
view_2.backgroundColor = [UIColor whiteColor];
view_2.alpha = 0.5;
[maskView addSubview:view_2];
view1.maskView = maskView;
[self.view addSubview:view1];
```
![](https://upload-images.jianshu.io/upload_images/1552225-ad708d8c7eec459b.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/450/format/webp)
你还可以通过layer或者图片来设计出很多有趣的效果:[《使用 maskView 设计动画》](https://www.jianshu.com/p/6e360516e3bc)、[《还有一个有趣的动画库》](https://github.com/rounak/RJImageLoader)(layer.mask)
- 
layerClass
> 
返回当前View所使用的根Layer类型
```
#if UIKIT_DEFINE_AS_PROPERTIES
@property(class, nonatomic, readonly) Class layerClass;                        // default is [CALayer class]. Used when creating the underlying layer for the view.
#else
+ (Class)layerClass;                        // default is [CALayer class]. Used when creating the underlying layer for the view.
#endif
```
layer有很多种、比如`CATextLayer`适合文本、`CAGradientLayer`适合处理渐变、`CAReplicatorLayer`适合处理很多相似的图层。
当然这些我都不太了解~你可以参阅[《[iOS Animation]-CALayer 专用图层》](https://www.cnblogs.com/daxiaxiaohao/p/4272722.html)
- 
layer
> 
layer视图图层(可以用来设置圆角效果/阴影效果)
```
@property(nonatomic,readonly,strong) CALayer  *layer;
```
### 配置与事件相关的行为
- 
userInteractionEnabled
> 
设置用户交互、默认YES允许用户交互。
```
@property(nonatomic,getter=isUserInteractionEnabled) BOOL userInteractionEnabled;
```
这个属性直接影响到控件能否进入响应链或者成为第一响应者。
[《iOS文档补完计划--UIResponder》](https://www.jianshu.com/p/0af63fe03f76)
- 
multipleTouchEnabled
> 
是否允许多指触控。默认NO
```
@property(nonatomic,getter=isMultipleTouchEnabled) BOOL multipleTouchEnabled __TVOS_PROHIBITED;
```
- 
exclusiveTouch
> 
是否让View独占Touch事件
```
@property(nonatomic,getter=isExclusiveTouch) BOOL       exclusiveTouch;
```
默认是NO。设置成YES避免在一个界面上同时点击多个button。
### 配置边界和框架矩形
- 
frame
> 
位置和尺寸
```
@property(nonatomic) CGRect  frame;
```
以父控坐标系的左上角为坐标原点(0, 0)
- 
bounds
> 
位置和尺寸
```
@property(nonatomic) CGRect    bounds;
```
以自身标系的左上角为坐标原点(0, 0)
- 
center
> 
中心点
```
@property(nonatomic) CGPoint   center;
```
以父控件的左上角为坐标原点(0, 0)
- 
transform
> 
形变
```
@property(nonatomic) CGAffineTransform transform;
```
可以做一些Transform动画、大概就是形变、缩放、位移咯。
**比如你可以给cell做一些小动画**
```
NSArray *cells = tableView.visibleCells;
for (int i = 0; i < cells.count; i++) {
    UITableViewCell *cell = [cells objectAtIndex:i];
    if (i%2 == 0) {
        cell.transform = CGAffineTransformMakeTranslation(-BSScreen_Width,0);
    }else {
        cell.transform = CGAffineTransformMakeTranslation(BSScreen_Width,0);
    }
    [UIView animateWithDuration:duration delay:i*0.03 usingSpringWithDamping:0.75 initialSpringVelocity:1/0.75 options:0 animations:^{
        cell.transform = CGAffineTransformIdentity;
    } completion:^(BOOL finished) {
        
    }];
}
```
### 管理视图层次结构
- 
superview
> 
获取父视图
```
@property(nullable, nonatomic,readonly) UIView *superview;
```
- 
subviews
> 
获取所有子视图
```
@property(nonatomic,readonly,copy) NSArray<__kindof UIView *> *subviews;
```
数组的顺序等于添加到父视图上的顺序。
你也可以尝试用递归的方式遍历所有子视图嗯。
- 
window
> 
获取视图所在的Window
```
@property(nullable, nonatomic,readonly) UIWindow  *window;
```
这个我也不太懂干嘛的。只知道如果父视图是UIWindow一定有值、否则(*我测试的都是*)为空。
- 
- addSubview:
> 
添加子视图
```
- (void)addSubview:(UIView *)view;
```
会被添加在subviews的末尾、视图层级的最上方。
- 
- removeFromSuperview
> 
从父视图上移除
```
- (void)removeFromSuperview;
```
- 
- bringSubviewToFront:
> 
移动指定的子视图，使其显示在其所以兄弟节点之上
```
- (void)bringSubviewToFront:(UIView *)view;
```
- 
- sendSubviewToBack:
> 
移动指定的子视图，使其显示在其所有兄弟节点之下
```
- (void)sendSubviewToBack:(UIView *)view;
```
自己试去吧~
```
UIView *view1 = [[UIView alloc] initWithFrame:CGRectMake(10, 150, 100, 50)];
view1.backgroundColor = [UIColor blueColor];
[self.view addSubview:view1];
UIView *view2 = [[UIView alloc] initWithFrame:CGRectMake(15, 155, 100, 50)];
view2.backgroundColor = [UIColor grayColor];
[self.view addSubview:view2];
UIView *view3 = [[UIView alloc] initWithFrame:CGRectMake(20, 160, 100, 50)];
view3.backgroundColor = [UIColor yellowColor];
[self.view addSubview:view3];
//如果将下面两行代码都注释掉   view1 会在下面   view2会在上面
//  下面这行代码能够将view2  调整到父视图的最下面
//    [self.view sendSubviewToBack:view2];
//将view调整到父视图的最上面
[self.view bringSubviewToFront:view1];
```
- 
- insertSubview:atIndex:
> 
插入子视图(将子视图插入到subviews数组中index这个位置)
```
- (void)insertSubview:(UIView *)view atIndex:(NSInteger)index;
```
- 
- insertSubview:aboveSubview:
> 
插入子视图(将子视图插到siblingSubview之上)
```
- (void)insertSubview:(UIView *)view aboveSubview:(UIView *)siblingSubview;
```
- 
- insertSubview: belowSubview:
> 
插入子视图(将子视图插到siblingSubview之下)
```
- (void)insertSubview:(UIView *)view belowSubview:(UIView *)siblingSubview;
```
- 
- exchangeSubviewAtIndex:withSubviewAtIndex:
> 
交换两个子视图
```
- (void)exchangeSubviewAtIndex:(NSInteger)index1 withSubviewAtIndex:(NSInteger)index2;
```
- 
- isDescendantOfView:
> 
检测一个视图是否属于另一个的子视图
```
- (BOOL)isDescendantOfView:(UIView *)view;
```
举个例子:
```
(lldb) po [self.view isDescendantOfView:view1]
NO
(lldb) po [view1 isDescendantOfView:self.view]
YES
```
需要注意的是、这个判定并不局限于一级结构。
### 观察与视图层级的更改
> 
包含子视图的成功添加/移除、本视图添加到新父视图的开始/结束。
一下方法默认不做任何操作。需要注意的是在`removeFromSuperview`时、也会调用。只不过`newSuperview`为空。
```
/** 添加自视图完成后调用 */
- (void)didAddSubview:(UIView *)subview;
/** 将要移除自视图时调用 */
- (void)willRemoveSubview:(UIView *)subview;
 
/** 将要移动到新父视图时调用 */
- (void)willMoveToSuperview:(nullable UIView *)newSuperview;
/** 移动到新父视图完成后调用 */
- (void)didMoveToSuperview;
/** 将要移动到新Window时调用 */
- (void)willMoveToWindow:(nullable UIWindow *)newWindow;
/** 移动到新Window完成后调用 */
- (void)didMoveToWindow;
```
### 配置内容边距
> 
[官方文档对Content Margin的解释](https://developer.apple.com/documentation/uikit/uiview/positioning_content_within_layout_margins?language=objc)
- 
directionalLayoutMargins
> 
iOS11 开始引入，可以根据语言的方向进行前后布局，与 layoutMargins 相比，能更好的适配 RTL 语言。
```
@property (nonatomic) NSDirectionalEdgeInsets directionalLayoutMargins API_AVAILABLE(ios(11.0),tvos(11.0));
```
但和我们关系不大、除非某天我们进军中东的某些国家了。
- 
layoutMargins
> 
自动布局时。用于指定视图和它的子视图之间的边距。
```
@property (nonatomic) UIEdgeInsets layoutMargins NS_AVAILABLE_IOS(8_0);
```
iOS11之后请使用`directionalLayoutMargins`属性进行布局。他将左右的概念替换成了前后。而且这两个属性会互相同步
默认为8个单位。如果视图并不是完全处于安全区域内或者设置了`preservesSuperviewLayoutMargins`则可能更大。
不过说实话如果用masonry的话感觉这个属性意义不大
- 
preservesSuperviewLayoutMargins
> 
是否将当前视图的间距和父视图相同。
```
@property (nonatomic) BOOL preservesSuperviewLayoutMargins NS_AVAILABLE_IOS(8_0);
```
设置一个视图的边距（视图边缘与其子视图边缘的距离）、防止其子视图和父视图边缘重合。
- 
-layoutMarginsDidChange
> 
改变view的layoutMargins这个属性时，会触发这个方法
```
- (void)layoutMarginsDidChange NS_AVAILABLE_IOS(8_0);
```
用原生Layout来布局说实话我没用过。可以看看这篇帖子:[《layoutMargins和preservesSuperviewLayoutMargins》](https://www.jianshu.com/p/4237bd89f521)
### 屏幕的安全区域
> 
[官方文档对的《Safe Area》解释](https://developer.apple.com/documentation/uikit/uiview/positioning_content_relative_to_the_safe_area?language=objc)
**iOS11之后、(为了帮助适配iPhoneX？)苹果给我们引入了一个安全区域的概念。**
安全区域帮助我们将view放置在整个屏幕的可视的部分。即使把navigationbar设置为透明的，系统也认为安全区域是从navigationbar的bottom开始，保证不被系统的状态栏、或导航栏覆盖。
- 
safeAreaInsets
> 
反映了一个view距离该view的安全区域的边距
```
@property (nonatomic,readonly) UIEdgeInsets safeAreaInsets API_AVAILABLE(ios(11.0),tvos(11.0));
```
这个属性会被系统在布局后自动设置。比如你可以在`UIViewController`的`viewSafeAreaInsetsDidChange`方法下检测横屏以及竖屏下的变化。
对于`UIView`也有对应的方法`safeAreaInsetsDidChange`
- 
safeAreaLayoutGuide
> 
safeAreaLayoutGuide是一个相对抽象的概念，为了便于理解，我们可以把safeAreaLayoutGuide看成是一个“view”，这个“view”系统自动帮我们调整它的bounds，让它不会被各种奇奇怪怪的东西挡住，包括iPhone X的刘海区域和底部的一道杠区域，可以认为在这个“view”上一定能完整显示所有内容。
结合Masonry的用法:将四周与安全区域贴合
```
[view mas_makeConstraints:^(MASConstraintMaker *make) {
    make.top.equalTo(self.view.mas_safeAreaLayoutGuideTop);
    make.left.equalTo(self.view.mas_safeAreaLayoutGuideLeft);
    make.right.equalTo(self.view.mas_safeAreaLayoutGuideRight);
    make.bottom.equalTo(self.view.mas_safeAreaLayoutGuideBottom);
}];
```
- 
- safeAreaInsetsDidChange
> 
当View的`safeAreaInsets`发生变化时自动调用
```
- (void)safeAreaInsetsDidChange API_AVAILABLE(ios(11.0),tvos(11.0));
```
- 
insetsLayoutMarginsFromSafeArea
> 
决定在自动布局时是否考虑`safeAreaInsets`的限制
```
@property (nonatomic) BOOL insetsLayoutMarginsFromSafeArea
```
示例代码:
```
self.view.insetsLayoutMarginsFromSafeArea = NO;
self.tableView = [[UITableView alloc] initWithFrame:CGRectZero style:UITableViewStylePlain];
self.tableView.backgroundColor = [UIColor orangeColor];
self.tableView.translatesAutoresizingMaskIntoConstraints = NO;
[self.view addSubview:self.tableView];
NSArray<__kindof NSLayoutConstraint *> *constraints = [NSLayoutConstraint constraintsWithVisualFormat:@"V:|-[tableView]-|" options:0 metrics:nil views:@{@"tableView" : self.tableView}];
[self.view addConstraints:constraints];
constraints = [NSLayoutConstraint constraintsWithVisualFormat:@"H:|[tableView]|" options:0 metrics:nil views:@{@"tableView" : self.tableView}];
[self.view addConstraints:constraints];
```
关于iOS11安全区域的特性、推荐一篇Bugly的帖子[《iOS 11 安全区域适配总结》](https://mp.weixin.qq.com/s/W1_0VrchCO50owhJNmJnuQ)
### 测量Auto Layout
- 
- systemLayoutSizeFittingSize:
> 
返回Auto Layout后内容高度。
```
- (CGSize)systemLayoutSizeFittingSize:(CGSize)targetSize NS_AVAILABLE_IOS(6_0);
```
通常用于包含多层视图的控件计算(比如cell)。这里注意与`sizeThatFits`和`sizeToFits`进行区分。
```
UILabel *label = [[UILabel alloc] init];
label.text = @"SafeAreaS";
label.numberOfLines = 0;
label.textAlignment = NSTextAlignmentCenter;
label.backgroundColor = [UIColor greenColor];
[self.view addSubview:label];
[label mas_makeConstraints:^(MASConstraintMaker *make) {
    if (@available(iOS 11, *)) {
        make.top.equalTo(self.view.mas_safeAreaLayoutGuideTop);
        make.left.equalTo(self.view.mas_safeAreaLayoutGuideLeft);
    } else {
        make.top.equalTo(self.mas_topLayoutGuideBottom);
        make.left.mas_equalTo(0);
    }
}];
[self.view layoutIfNeeded];
CGSize layoutSize =  [label systemLayoutSizeFittingSize:UILayoutFittingExpandedSize];
CGSize labelSize = label.frame.size;
//(lldb) po layoutSize
//(width = 79.666666666666671, height = 20.333333333333332)
//
//(lldb) po labelSize
//(width = 79.666666666666671, height = 20.333333333333332)
```
利用这个方法、我们可以对是适应高度的Cell高度进行缓存来适配iOS8以下的情况。需要注意的是要对`cell.contentView`进行计算。
```
model.cellHeight = [cell.contentView systemLayoutSizeFittingSize:UILayoutFittingCompressedSize].height;
```
- 
systemLayoutSizeFittingSize:withHorizontalFittingPriority:verticalFittingPriority:
> 
和上一个方法一样、但是增加了宽高的优先级。使结果更加准确
```
- (CGSize)systemLayoutSizeFittingSize:(CGSize)targetSize withHorizontalFittingPriority:(UILayoutPriority)horizontalFittingPriority verticalFittingPriority:(UILayoutPriority)verticalFittingPriority NS_AVAILABLE_IOS(8_0);
```
- 
intrinsicContentSize
> 
返回控件的固有大小
```
#if UIKIT_DEFINE_AS_PROPERTIES
@property(nonatomic, readonly) CGSize intrinsicContentSize NS_AVAILABLE_IOS(6_0);
#else
- (CGSize)intrinsicContentSize NS_AVAILABLE_IOS(6_0);
#endif
```
在自动布局时、有些控件(UILabel/UIButton/UIImageView等)`只需要设置位置而不需要设置大小`、就是利用这个属性。
当内容改变时、调用`invalidateIntrinsicContentSize`通知系统、并且自定义`intrinsicContentSize`的实现、来返回合适的宽高。
举个例子、你可以让你的UITextField能够自适应宽度[《一个随输入文字宽度变化的自定义UITextField》](https://www.jianshu.com/p/a5eaba3c24f7)。
- 
- invalidateIntrinsicContentSize
> 
废除视图原本内容的`intrinsicContentSize`
```
- (void)invalidateIntrinsicContentSize NS_AVAILABLE_IOS(6_0);
```
上面已经说过用法了
- 
抗压缩与抗拉伸
Compression Resistance priority(抗压缩)
> 
有多大的优先级阻止自己变小
```
/* 返回某个方向的抗压缩等级 */
- (UILayoutPriority)contentCompressionResistancePriorityForAxis:(UILayoutConstraintAxis)axis NS_AVAILABLE_IOS(6_0);
/* 设置某个方向的抗压缩等级*/
- (void)setContentCompressionResistancePriority:(UILayoutPriority)priority forAxis:(UILayoutConstraintAxis)axis NS_AVAILABLE_IOS(6_0);
```
Hugging priority(抗拉伸)
> 
有多大的优先级阻止自己变大
```
/* 返回某个方向上的抗拉伸等级 */
- (UILayoutPriority)contentHuggingPriorityForAxis:(UILayoutConstraintAxis)axis NS_AVAILABLE_IOS(6_0);
/* 设置某个方向上的抗拉伸等级*/
- (void)setContentHuggingPriority:(UILayoutPriority)priority forAxis:(UILayoutConstraintAxis)axis NS_AVAILABLE_IOS(6_0);
```
通常是给UILabel用的[《HuggingPriority和CompressionResistance 一个例子教你理解》](https://www.jianshu.com/p/5cf559435eb9)
### 触发自动布局
- 
- needsUpdateConstraints
> 
通知用户是否有需要更新的约束
```
- (BOOL)needsUpdateConstraints NS_AVAILABLE_IOS(6_0);
```
这个可以作为你判断是否应该在后续代码里主动调用`updateConstraints`的前提。不过通常我们都不太需要、因为布局的修改都是自己写的自然清楚何时调用。
还有就是当View尚未展示时、更新的标记一直会返回YES。
- 
- setNeedsUpdateConstraints
> 
标记准备更改约束
```
- (void)setNeedsUpdateConstraints NS_AVAILABLE_IOS(6_0);
```
通常是用作批量修改约束时的优化、避免系统多次计算。
- 
- updateConstraints
> 
当View本体的布局被修改时被自动调用
```
- (void)updateConstraints NS_AVAILABLE_IOS(6_0) NS_REQUIRES_SUPER;
```
你可以在这里自己更新View内部控件的布局、需要调用super实现。
会被调用的情况:
- 视图被添加到父视图上
- 主动调用`updateConstraintsIfNeeded`
- 更新约束时
这里需要注意的是更新约束后自动调用的时机:
- 如果你使用`NSLayoutConstraint`对约束进行更新
`updateConstraints`方法会`自动调用`。
- 如果使用`Masonry`对约束进行更新
`updateConstraints`方法`不会自动调用`。
	你需要手动`setNeedsUpdateConstraints`然后`updateConstraintsIfNeeded`
- 
- updateConstraintsIfNeeded
> 
立即触发约束更新，自动更新布局
```
- (void)updateConstraintsIfNeeded NS_AVAILABLE_IOS(6_0);
```
会把之前`setNeedsUpdateConstraints`标记之后的所有约束修改、同时更新。
并且(如果控件已经被`setNeedsUpdateConstraints`标记)自动调用控件的`updateConstraints`方法。
所以、方法调用的顺序为：
视图的布局改变时:`updateConstraints`被执行。
视图布局修改:`setNeedsUpdateConstraints`-->`updateConstraintsIfNeeded`
而当`setNeedsUpdateConstraints`被调用`needsUpdateConstraints`也会返回YES作为标记。
贴一个Masoney动画的例子:[《Masonry自动布局详解二：动画更新约束》](https://blog.csdn.net/woaifen3344/article/details/50114415)
但是这个帖子有些问题
```
// 告诉self.view约束需要更新
[self.view setNeedsUpdateConstraints];
// 调用此方法告诉self.view检测是否需要更新约束，若需要则更新，下面添加动画效果才起作用
[self.view updateConstraintsIfNeeded];
```
这两句话并没什么必要、因为我并没有更新self.view的约束。直接修改成这样、是比较好的。
```
- (void)onGrowButtonTaped:(UIButton *)sender {
    self.scacle += 0.5;
    [self.growingButton mas_updateConstraints:^(MASConstraintMaker *make) {
        make.center.mas_equalTo(self.view);
        
        // 初始宽、高为100，优先级最低
        make.width.height.mas_equalTo(100 * self.scacle);
        // 最大放大到整个view
        make.width.height.lessThanOrEqualTo(self.view);
    }];
    
    [UIView animateWithDuration:0.3 animations:^{
        [self.view layoutIfNeeded];
    }];
}
```
这也佐证了一个问题。`layoutIfNeeded`执行动画与`setNeedsUpdateConstraints`/`updateConstraintsIfNeeded`并没什么直接关系。
### 配置调整大小行为
- 
contentMode
> 
内容显示的模式。默认`UIViewContentModeScaleToFill
```
@property(nonatomic) UIViewContentMode contentMode;
```
我们平时调整图片显示状态用的就是这个属性
- 
UIViewContentMode
> 
内容具体的显示模式
```
typedef NS_ENUM(NSInteger, UIViewContentMode) {
    UIViewContentModeScaleToFill,       //!< 缩放内容到合适比例大小.
    UIViewContentModeScaleAspectFit,    //!< 缩放内容到合适的大小，边界多余部分透明.
    UIViewContentModeScaleAspectFill,   //!< 缩放内容填充到指定大小，边界多余的部分省略.
    UIViewContentModeRedraw,            //!< 重绘视图边界 (需调用 -setNeedsDisplay).
    UIViewContentModeCenter,            //!< 视图保持等比缩放.
    UIViewContentModeTop,               //!< 视图顶部对齐.
    UIViewContentModeBottom,            //!< 视图底部对齐.
    UIViewContentModeLeft,              //!< 视图左侧对齐.
    UIViewContentModeRight,             //!< 视图右侧对齐.
    UIViewContentModeTopLeft,           //!< 视图左上角对齐.
    UIViewContentModeTopRight,          //!< 视图右上角对齐.
    UIViewContentModeBottomLeft,        //!< 视图左下角对齐.
    UIViewContentModeBottomRight,       //!< 视图右下角对齐.
};
```
- 
- sizeThatFits:
> 
计算内容最合适的大小、但并不改变view的size
```
- (CGSize)sizeThatFits:(CGSize)size;
```
通常用于leaf-level views、这里注意与`systemLayoutSizeFittingSize`进行区分。
这里的参数size、类似于UILabel的`preferredMaxLayoutWidth`属性用于限制计算范围。
你可以自己试试
```
UITextView * textView = [[UITextView alloc]initWithFrame:CGRectMake(0, 100, 20, 20)];
[self.view addSubview:textView];
//textView.text = @"asdasdasdasdasdasdasda\nasasdasdasdasdasdasdasda\nasasdasdasdasdasdasdasda\nasasdasdasdasdasdasdasda\nasasdasdasdasdasdasdasda\nasasdasdasdasdasdasdasda\nasasdasdasdasdasdasdasda\nas";
textView.text = @"asdasdasdasdasdasdasdaasdasdasdasdasdasdasdaasdasdasdasdasdasdasdaasdasdasdasdasdasdasdaasdasdasdasdasdasdasdaasdasdasdasdasdasdasdaasdasdasdasdasdasdasdaasdasdasdasdasdasdasdaasdasdasdasdasdasdasdaasdasdasdasdasdasdasdaasdasdasdasdasdasdasdaasdasdasdasdasdasdasdaasdasdasdasdasdasdasdaasdasdasdasdasdasdasdaasdasdasdasdasdasdasdaasdasdasdasdasdasdasdaasdasdasdasdasdasdasdaasdasdasdasdasdasdasda";
textView.backgroundColor = [UIColor orangeColor];
CGSize s = [textView sizeThatFits:CGSizeMake(self.view.bounds.size.width, 999)];
textView.frame = CGRectMake(textView.frame.origin.x, textView.frame.origin.y, s.width, s.height);
```
如果我们想让UITextField等控件也自适应、重写`intrinsicContentSize`内部用`sizeThatFits:CGSizeMake`计算一下就好了呗。
- 
- sizeToFit
> 
计算内容最合适的大小、并改变view的size
```
- (void)sizeToFit;
```
会使用控件原有的宽高进行计算、等价于:
```
CGSize s = [textView sizeThatFits:textView.frame.size];
textView.frame = CGRectMake(textView.frame.origin.x, textView.frame.origin.y, s.width, s.height);
```
注意如果修改了size、会调用`layoutSubviews`
- 
autoresizesSubviews
> 
当本身大小发生改变时、是否自动布局子视图
```
@property(nonatomic) BOOL  autoresizesSubviews;
```
如果视图的autoresizesSubviews属性声明被设置为YES，则其子视图会根据autoresizingMask属性的值自动进行尺寸调整。
- 
autoresizingMask
> 
当父视图`autoresizesSubviews`为`YES`并且改变了大小时、该子视图的布局规则。
```
@property(nonatomic) UIViewAutoresizing autoresizingMask;
```
现在基本都是Auto Layout、很少用到这两个东西了。
可以了解一下[《IOS-AutoresizesSubviews》](https://blog.csdn.net/eward9888/article/details/8250556)
### 布局子视图
- 
- layoutSubviews
> 
当控件被(系统)赋予了一个新的大小时触发。
```
- (void)layoutSubviews;
```
- 
添加到屏幕时触发
	必须有指定的rect
- 
调用`setNeedsLayout`时触发
- 
size发生改变时触发。
	触发次数需要满足最后两点规则
- 
滑动scrollview时触发
- 
旋转屏幕时触发
- 
**系统赋予大小**
	我们都知道用户所设置的frame/layout并不会直接修改控件frame、而是会在特定的周期由系统进行布局绘制。也就是说、`我们在一个周期内连续设置多次frame/layout、系统也只会在周期结束时布局一次、并触发一次layoutSubviews`。
- 
控件只有被添加到屏幕上、才能触发layoutSubviews。
	通常都会触发两次、因为你还得给他设置frame/layout。
	不过如果先设置frame然后隔了很久才添加到屏幕上、就是一次。
- 
- setNeedsLayout
> 
为该控件设置标记。等待更新布局
```
- (void)setNeedsLayout;
```
和`setNeedsUpdateConstraints`的机制一样、他允许你对其多个子View进行布局后统一更新。
注意它并不是实时更新、而会在下一次布局周期中进行统一更新。
- 
- layoutIfNeeded
> 
立即更新该View所有子视图的布局
```
- (void)layoutIfNeeded;
```
它会将所有尚未更新的布局立即进行更新。
通常的用处有两个（欢迎补充）：
1.布局尚未完成、但我们需要获取具体的Frame(当然、根据情况你也可以使用`systemLayoutSizeFittingSize`以节省性能)
```
UIView * view = [UIView new];
[self.view addSubview:view];
[view mas_makeConstraints:^(MASConstraintMaker *make) {
    make.top.left.right.equalTo(self.view);
    make.height.mas_equalTo(50);
}];
CGSize s = [view systemLayoutSizeFittingSize:UILayoutFittingCompressedSize];
NSLog(@"%lf",s.height);//50.000000
NSLog(@"%lf",view.frame.size.height);//0.000000
[self.view layoutIfNeeded];
NSLog(@"%lf",view.frame.size.height);//50.000000
```
- 让约束通过动画更新
	贴一个Masoney动画的例子:[《Masonry自动布局详解二：动画更新约束》](https://blog.csdn.net/woaifen3344/article/details/50114415)
- 
requiresConstraintBasedLayout
> 
标记View是否需要用AutoLayout进行布局
```
#if UIKIT_DEFINE_AS_PROPERTIES
@property(class, nonatomic, readonly) BOOL requiresConstraintBasedLayout NS_AVAILABLE_IOS(6_0);
#else
+ (BOOL)requiresConstraintBasedLayout NS_AVAILABLE_IOS(6_0);
#endif
```
这个说实话不太懂、看文档和网上的意思都是如果不返回YES、有可能不调用`updateConstraints`方法。但是我自己写起来总会调用、希望有大神指正。
至于为什么要调用`updateConstraints`:[《masonry小问题之requiresConstraintBasedLayout》](https://www.jianshu.com/p/b58233a2c640)
- 
translatesAutoresizingMaskIntoConstraints
> 
是否将`AutoresizingMask`转化成`Constraints`约束。默认为YES
```
@property(nonatomic) BOOL translatesAutoresizingMaskIntoConstraints NS_AVAILABLE_IOS(6_0);
```
名字已经把这个属性的作用说的很明白了、但我们还可以再解释一下:
- 当我们用代码添加视图时:
	视图的`translatesAutoresizingMaskIntoConstraints`属性默认为`YES`，可是`AutoresizingMask`属性默认会被设置成`None`。也就是说如果我们不去动`AutoresizingMask`，那么`AutoresizingMask`就不会对约束产生影响。
- 当我们使用`interface builder`添加视图时:
`AutoresizingMask`虽然会被设置成非`None`，但是`translatesAutoresizingMaskIntoConstraints`默认被设置成了`NO`。所以也不会有冲突。
- 会出现问题的情况:
	当有一个视图是靠`AutoresizingMask`布局的，而我们修改了`translatesAutoresizingMaskIntoConstraints`后会让视图失去约束，走投无路。例如我自定义转场时就遇到了这样的问题，转场后的视图并不在视图的正中间。
不过这些问题似乎只有用`NSLayoutConstraint`才能体现出问题？
参考[《AutoLayout的那些事儿》](http://www.cocoachina.com/ios/20160530/16522.html)、[《代码添加constraint，设置translatesAutoresizingMaskIntoConstraints为NO的原因》](https://blog.csdn.net/u010140921/article/details/40627983)
### 绘制和更新视图
- 
- drawRect:
> 
自定义的绘制内容
```
- (void)drawRect:(CGRect)rect;
```
1.如果是UIView则不需要调用super、UIView子类需要调用super实现。
- 不要手动调用。
	可以通过`setNeedsDisplay`或者`setNeedsDisplayInRect`让系统自己调用。
- UIImageView不允许使用重写draw绘制
	因为他本身也不使用draw绘制、仅仅是使用内部的image view来显示图像
- 如果自己实现了drawRect、那务必每次都实现它
`setNeedsDisplay`会将绘制全部清空。系统自动调用时也是。
`setNeedsDisplayInRect`则会清空指定的rect。
![](https://upload-images.jianshu.io/upload_images/1552225-a30d323f8b72db24.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/542/format/webp)
调用的时机:
**1. 视图第一次被添加到父视图上的时候、由系统自动调用**
需要注意及时`hidden = YES`或者`alpha = 0`也会调用、但`remove`后再`add`并不会。
**2. 添加到父视图时必须有给定的rect、才会被自动调用**
也就是size必须不为{0,0}
**3. 修改了rect被调用自动调用**
**4.`setNeedsDisplay`+`setNeedsDisplayInRect`**
当然、必须有rect
具体使用:
在iOS中使用drawRect绘图一般分为以下5个步骤：
1、获取绘图上下文
CGContextRef context = UIGraphicsGetCurrentContext();
2、创建并设置路径
3、将路径添加到上下文
如：线宽、线条颜色、填充颜色等
4、设置上下文状态
5、绘制路径
6、释放路径
具体可以参考:[《drawRect的绘制的使用（绘制文本字符、绘制图片、绘制图形）》](https://blog.csdn.net/potato512/article/details/56845385)
使用DrawRect是会有一定性能问题的:
- contents寄宿图
[《内存恶鬼drawRect》](https://blog.csdn.net/sandyloo/article/details/51063799)
- 离屏渲染
[《深刻理解移动端优化之离屏渲染》](http://www.cocoachina.com/ios/20161114/18072.html)
这块我有空准备补一下、先打个卡:
[《iOS绘制和渲染》](https://www.jianshu.com/p/2bbed48dbfd0)
- 
- setNeedsDisplay
> 
标记全部重绘
```
- (void)setNeedsDisplay;
```
需要注意的是并不会立即重绘、而是等到下一个周期
- 
- setNeedsDisplayInRect:
> 
标记指定rect重绘
```
- (void)setNeedsDisplayInRect:(CGRect)rect;
```
需要注意的是并不会立即重绘、而是等到下一个周期。
![](https://upload-images.jianshu.io/upload_images/1552225-cb6254eb7566b084.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/428/format/webp)
- 
contentScaleFactor
> 
视图内容的缩放比例
```
@property(nonatomic) CGFloat  contentScaleFactor NS_AVAILABLE_IOS(4_0);
```
修改contentScaleFactor可以让UIView的渲染精度提高，这样即使在CGAffineTransform放大之后仍然能保持锐利
- 
- tintColorDidChange
> 
当`tintColor`或者`tintAdjustmentMode`被修改时系统调用
```
- (void)tintColorDidChange NS_AVAILABLE_IOS(7_0);
```
你可以把他当成一个监听来使用
```
- (void)tintColorDidChange {
  _tintColorLabel.textColor = self.tintColor;
  _tintColorBlock.backgroundColor = self.tintColor;
}
```
### 管理手势识别器
- 
添加删除和获取
```
** 当前视图所附加的所有手势识别器 */
@property(nullable, nonatomic,copy) NSArray<__kindof UIGestureRecognizer *> *gestureRecognizers NS_AVAILABLE_IOS(3_2);
 
/** 添加一个手势识别器 */
- (void)addGestureRecognizer:(UIGestureRecognizer*)gestureRecognizer NS_AVAILABLE_IOS(3_2);
/** 移除一个手势识别器 */
- (void)removeGestureRecognizer:(UIGestureRecognizer*)gestureRecognizer NS_AVAILABLE_IOS(3_2);
```
- 
- gestureRecognizerShouldBegin
> 
是否继续识别手势。
```
- (BOOL)gestureRecognizerShouldBegin:(UIGestureRecognizer *)gestureRecognizer NS_AVAILABLE_IOS(6_0);
```
此方法在gesture recognizer视图转出UIGestureRecognizerStatePossible状态时调用，如果返回NO,则转换到UIGestureRecognizerStateFailed;如果返回YES,则继续识别触摸序列.(默认情况下为YES)。
你可以用来在控件指定的位置使用手势识别
```
- (BOOL)gestureRecognizerShouldBegin:(UIGestureRecognizer *)gestureRecognizer{
    if ([gestureRecognizer isKindOfClass:[UIPanGestureRecognizer class]]) {
        UIPanGestureRecognizer *pan = (UIPanGestureRecognizer *)gestureRecognizer;
        CGPoint translation = [pan translationInView:self.view];
        CGFloat offsetX = self.scrollView.contentOffset.x;
        if (translation.x > 0 && offsetX == 0.0 ) {
               return NO;
        }
    }
    return YES;
}
```
### 观察焦点
canBecomeFocused
> 
返回是否可以成为焦点, 默认NO
```
#if UIKIT_DEFINE_AS_PROPERTIES
@property(nonatomic,readonly) BOOL canBecomeFocused NS_AVAILABLE_IOS(9_0); // NO by default
#else
- (BOOL)canBecomeFocused NS_AVAILABLE_IOS(9_0); // NO by default
#endif
```
焦点是给AppleTV用的、用遥控器选择屏幕上的控件。
[在一个以焦点为基础的交互模型中，在屏幕上的单一视图可以得到焦点，并且用户可以通过浏览屏幕上不同的UI选项将焦点移动到其他视图，从而引起焦点更新。得到焦点的视图被用作任何用户操作的目标事件。例如，如果一个屏幕上的按钮被选中，当由遥控器发送按钮选择事件时，目标事件将被触发。](http://www.cocoachina.com/game/20151123/14389.html)
focused
> 
返回是否已经被聚焦
```
@property (readonly, nonatomic, getter=isFocused) BOOL focused NS_AVAILABLE_IOS(9_0);
```
### 运动视觉效果
就是王者荣耀那种可以晃手机看背景图的效果吧
- 
添加删除和获取
> 
添加、删除、查看、我没用过知道就得了
```
/** 添加运动效果，当倾斜设备时视图稍微改变其位置 */
- (void)addMotionEffect:(UIMotionEffect *)effect NS_AVAILABLE_IOS(7_0);
 
/** 移除运动效果 */
- (void)removeMotionEffect:(UIMotionEffect *)effect NS_AVAILABLE_IOS(7_0);
 
/** 所有添加的运动效果 */
@property (copy, nonatomic) NSArray<__kindof UIMotionEffect *> *motionEffects NS_AVAILABLE_IOS(7_0);
```
### 后台启动恢复
> 
你可以让视图在后台恢复时候仍然保持原有的样子
```
/** 标示是否支持保存,恢复视图状态信息 */
@property (nullable, nonatomic, copy) NSString *restorationIdentifier NS_AVAILABLE_IOS(6_0);
/** 保存视图状态相关的信息 */
- (void) encodeRestorableStateWithCoder:(NSCoder *)coder NS_AVAILABLE_IOS(6_0);
/** 恢复和保持视图状态相关信息 */
- (void) decodeRestorableStateWithCoder:(NSCoder *)coder NS_AVAILABLE_IOS(6_0);
```
UIView的例子就不举了、看看UIViewController的挺好[《iOS的App实现状态恢复》](https://www.jianshu.com/p/7f2fe9361f07)
与添加定位或者播放音频等真后台方式不同、这个仅仅是恢复页面。
### 捕获视图快照
- 
- snapshotViewAfterScreenUpdates:
> 
对某个视图进行快照
```
- (UIView *)snapshotViewAfterScreenUpdates:(BOOL)afterUpdates;
```
该方法有一个BOOL类型的参数、这个参数表示是否立即生成快照、还是在需要更新视图的时候生成。
```
UIView *showView = [[UIView alloc] initWithFrame:CGRectMake(0, 100, 100, 100)];
showView.backgroundColor = [UIColor redColor];
[self.view addSubview:showView];
self.vvvv = showView;
UIView *snap1 = [showView snapshotViewAfterScreenUpdates:NO];
snap1.center = self.view.center;
[self.view addSubview:snap1];
```
设置YES、会等到当前队列的所有方法完成之后、才会生成快照。
在设置NO的情况、延时生成快照、也能达到YES的效果、原理是一样的。
- 
- resizableSnapshotViewFromRect:afterScreenUpdates:withCapInsets:
> 
比上面的方法多了两个参数、意味着你可以把视图进行分割操作
```
- (UIView *)resizableSnapshotViewFromRect:(CGRect)rect 
                       afterScreenUpdates:(BOOL)afterUpdates 
                            withCapInsets:(UIEdgeInsets)capInsets;
```
- 
- drawViewHierarchyInRect:afterScreenUpdates:
> 
比之前的多了一个rect参数、其他并没发现什么去区别~
```
- (BOOL)drawViewHierarchyInRect:(CGRect)rect 
             afterScreenUpdates:(BOOL)afterUpdates;
```
关于`afterUpdates`参数：
尽量设置为NO、否则如果视图中途被释放掉会殷勤crash。
### 识别视图
- 
tag
> 
识别标识、默认为0
```
@property(nonatomic) NSInteger tag;
```
- 
- viewWithTag
> 
范围子View中某个tag的View
```
- (__kindof UIView *)viewWithTag:(NSInteger)tag;
```
- 搜索包括二级子视图
- 以队列的形式搜索、搜索到一个则返回。
### 坐标系转换
> 
将一个View中的Rect或Point转化到另一个View的坐标系中
```
/** 将point由point所在视图转换到目标视图view中，返回在目标视图view中的point值 */
- (CGPoint)convertPoint:(CGPoint)point toView:(nullable UIView *)view;
/** 将point由point所在视图转换到目标视图view中，返回在目标视图view中的point值 */
- (CGPoint)convertPoint:(CGPoint)point fromView:(nullable UIView *)view;
/** 将rect由rect所在视图转换到目标视图view中，返回在目标视图view中的rect */
- (CGRect)convertRect:(CGRect)rect toView:(nullable UIView *)view;
/** 将rect从view中转换到当前视图中，返回在当前视图中的rect */
- (CGRect)convertRect:(CGRect)rect fromView:(nullable UIView *)view;
```
每种转换都有两个方法、但实际向作用都一样。
无非是参数位置的区别~
**将view1中的一个视图、转化到view2的坐标系中**
```
[view1 convertRect:view1.childView toView:view2];
[view2 convertRect:view1.childView fromView:view1];
```
需要注意的是`view`参数如果为nil、则会返回基于当前window的坐标。
并且、两个view必须归属于同一个window。
比如让超出父视图的View可以被点击
```
-(UIView *)hitTest:(CGPoint)point withEvent:(UIEvent *)event
{
    UIView *view = [super hitTest:point withEvent:event];
    if (view == nil) {
        for (UIView *subView in self.subviews) {
            //将point从self的坐标系、映射给subView的坐标系
            CGPoint myPoint = [subView convertPoint:point fromView:self];
            //判断point是否在subView上
            if (CGRectContainsPoint(subView.bounds, myPoint)) {
                return subView;
            }
        }
    }
    return view;
}
```
需要注意的是
如果你想让某个控件拦截某个事件、为了保险起见尽量修改父视图的`hitTest`方法、以免被其他原本能够响应的控件捷足先登。
### 命中测试(Hit-Testing)
- 
- hitTest:withEvent:
> 
询问事件在当前视图中的响应者，同时又是作为事件传递的桥梁
```
- (UIView *)hitTest:(CGPoint)point 
          withEvent:(UIEvent *)event;
```
上面例子中重载的方法便是这个。
以下几种状态的视图无法响应事件：
- 不允许交互
	userInteractionEnabled = NO
- 隐藏
	hidden = YES 如果父视图隐藏，那么子视图也会隐藏，隐藏的视图3. 无法接收事件
	透明度
	alpha < 0.01 如果设置一个视图的透明度<0.01，会直接影响子视图的透明度。alpha：0.0~0.01为透明。
默认情况下:
- 若当前视图无法响应事件
	则返回nil
- 若当前视图可以响应事件
	但无子视图可以响应事件、则返回自身作为当前视图层次中的事件响应者
- 若当前视图可以响应事件
	同时有子视图可以响应、则返回子视图层次中的事件响应者
其内部实现大致为:
- 看自身能否响应时间
- 看点是否在自身上
- 看子视图是否能够响应
```
- (UIView *)hitTest:(CGPoint)point withEvent:(UIEvent *)event{
    //3种状态无法响应事件
     if (self.userInteractionEnabled == NO || self.hidden == YES ||  self.alpha <= 0.01) return nil; 
    //触摸点若不在当前视图上则无法响应事件
    if ([self pointInside:point withEvent:event] == NO) return nil; 
    //从后往前遍历子视图数组 
    int count = (int)self.subviews.count; 
    for (int i = count - 1; i >= 0; i--) 
    { 
        // 获取子视图
        UIView *childView = self.subviews[i]; 
        // 坐标系的转换,把触摸点在当前视图上坐标转换为在子视图上的坐标
        CGPoint childP = [self convertPoint:point toView:childView]; 
        //询问子视图层级中的最佳响应视图
        UIView *fitView = [childView hitTest:childP withEvent:event]; 
        if (fitView) 
        {
            //如果子视图中有更合适的就返回
            return fitView; 
        }
    } 
    //没有在子视图中找到更合适的响应视图，那么自身就是最合适的
    return self;
}
```
- 
- pointInside:withEvent:
> 
判断触摸点是否在自身坐标范围内
```
- (BOOL)pointInside:(CGPoint)point 
          withEvent:(UIEvent *)event;
```
默认实现是若在坐标范围内则返回YES，否则返回NO。
所以之前那个超出范围点击的方法中
```
if (CGRectContainsPoint(subView.bounds, myPoint)) {
    return subView;
}
```
换成
```
if ([subView pointInside:myPoint withEvent:event]) {
    return subView;
}
```
也是一样可行的。
通过修改`pointInside`的判定[《扩大UIButton的点击范围》](https://www.jianshu.com/p/e368d3fea803)
- 
为响应链寻找最合适的FirstView
从事件传递到APP中开始、寻找最合适的View
UIApplication -> UIWindow -> 父View -> 子view
- 逐级调用`hitTest:withEvent`方法
- `hitTest:withEvent`方法内部通过`pointInside:withEvent:`进行判断。
	通过则返回自身
### 结束视图编辑
- 
- endEditing:
> 
强制让自身或者子视图上的`UIResponder`放弃第一响应者。
```
- (BOOL)endEditing:(BOOL)force;
```
更多收起键盘的方式可以看看[《iOS开发－关闭/收起键盘方法总结》](https://www.jianshu.com/p/7db20da82c4a)
### Block动画
> 
传统的Block动画
但是苹果文档中更推荐使用[《UIViewPropertyAnimator》](https://developer.apple.com/documentation/uikit/uiviewpropertyanimator?language=objc)来做动画了
```
/** 用于对一个或多个视图的改变的持续时间、延时、选项动画完成时的操作 */
+ (void)animateWithDuration:(NSTimeInterval)duration delay:(NSTimeInterval)delay options:(UIViewAnimationOptions)options animations:(void (^)(void))animations completion:(void (^ __nullable)(BOOL finished))completion NS_AVAILABLE_IOS(4_0);
 
/** 用于对一个或多个视图的改变的持续时间、选项动画完成时的操作，默认：delay = 0.0, options = 0 */
+ (void)animateWithDuration:(NSTimeInterval)duration animations:(void (^)(void))animations completion:(void (^ __nullable)(BOOL finished))completion NS_AVAILABLE_IOS(4_0);
 
/** 用于对一个或多个视图的改变的持续时间内动画完成时的操作，默认：delay = 0.0, options = 0, completion = NULL */
+ (void)animateWithDuration:(NSTimeInterval)duration animations:(void (^)(void))animations NS_AVAILABLE_IOS(4_0);
 
/** 使用与物理弹簧运动相对应的定时曲线执行视图动画 */
+ (void)animateWithDuration:(NSTimeInterval)duration delay:(NSTimeInterval)delay usingSpringWithDamping:(CGFloat)dampingRatio initialSpringVelocity:(CGFloat)velocity options:(UIViewAnimationOptions)options animations:(void (^)(void))animations completion:(void (^ __nullable)(BOOL finished))completion NS_AVAILABLE_IOS(7_0);
 
/** 为指定的容器视图创建转换动画 */
+ (void)transitionWithView:(UIView *)view duration:(NSTimeInterval)duration options:(UIViewAnimationOptions)options animations:(void (^ __nullable)(void))animations completion:(void (^ __nullable)(BOOL finished))completion NS_AVAILABLE_IOS(4_0);
 
/** 使用给定的参数在指定视图之间创建转换动画 */
+ (void)transitionFromView:(UIView *)fromView toView:(UIView *)toView duration:(NSTimeInterval)duration options:(UIViewAnimationOptions)options completion:(void (^ __nullable)(BOOL finished))completion NS_AVAILABLE_IOS(4_0); // toView added to fromView.superview, fromView removed from its superview
 
/** 在一个或多个视图上执行指定的系统提供的动画，以及定义的可选并行动画 */
+ (void)performSystemAnimation:(UISystemAnimation)animation onViews:(NSArray<__kindof UIView *> *)views options:(UIViewAnimationOptions)options animations:(void (^ __nullable)(void))parallelAnimations completion:(void (^ __nullable)(BOOL finished))completion NS_AVAILABLE_IOS(7_0);
```
### 首尾式动画
> 
通过上下文的方式对动画进行配置
但是苹果文档中更推荐使用[《UIViewPropertyAnimator》](https://developer.apple.com/documentation/uikit/uiviewpropertyanimator?language=objc)来做动画了
```
/** 开始动画 */
+ (void)beginAnimations:(nullable NSString *)animationID context:(nullable void *)context;
/** 提交动画 */
+ (void)commitAnimations;
 
/** 设置动画代理, 默认nil */
+ (void)setAnimationDelegate:(nullable id)delegate;
/** 动画将要开始时执行方法（必须要先设置动画代理）, 默认NULL */
+ (void)setAnimationWillStartSelector:(nullable SEL)selector;
/** 动画已结束时执行方法（必须要先设置动画代理）, 默认NULL */
+ (void)setAnimationDidStopSelector:(nullable SEL)selector;
/** 设置动画时长, 默认0.2秒 */
+ (void)setAnimationDuration:(NSTimeInterval)duration;
/** 动画延迟执行时间, 默认0.0秒 */
+ (void)setAnimationDelay:(NSTimeInterval)delay;
/** 设置在动画块内部动画属性改变的开始时间, 默认now ([NSDate date]) */
+ (void)setAnimationStartDate:(NSDate *)startDate;
/** 设置动画曲线, 默认UIViewAnimationCurveEaseInOut */
+ (void)setAnimationCurve:(UIViewAnimationCurve)curve;
/** 动画的重复播放次数， 默认0 */
+ (void)setAnimationRepeatCount:(float)repeatCount;
/** 设置是否自定翻转当前的动画效果, 默认NO */
+ (void)setAnimationRepeatAutoreverses:(BOOL)repeatAutoreverses;
/** 设置动画从当前状态开始播放, 默认NO */
+ (void)setAnimationBeginsFromCurrentState:(BOOL)fromCurrentState;
 
/** 在动画块中为视图设置过渡动画 */
+ (void)setAnimationTransition:(UIViewAnimationTransition)transition forView:(UIView *)view cache:(BOOL)cache;
 
/** 设置是否激活动画 */
+ (void)setAnimationsEnabled:(BOOL)enabled;
```
### 最后
本文主要是自己的学习与总结。如果文内存在纰漏、万望留言斧正。如果愿意补充以及不吝赐教小弟会更加感激。
### 参考资料
[官方文档--UIView](https://developer.apple.com/documentation/uikit/uiview?language=objc)
[iOS UIView非常用方法及属性详解](https://www.cnblogs.com/snake-hand/p/3190021.html)
[详解UICoordinateSpace和UIScreen在iOS 8上的坐标问题](https://blog.csdn.net/jgh609445029/article/details/41866807)
[[iOS动画] -Calayer专用图层](https://www.cnblogs.com/daxiaxiaohao/p/4272722.html)
[iOS冷门知识-RightToLeft](https://www.jianshu.com/p/34b5a8d9a77e)
[iOS形变之CGAffineTransform](https://www.jianshu.com/p/ca7f9bc62429)
[UIView的alpha，hidden和opaque属性之间的关系和区别](https://blog.csdn.net/wzzvictory/article/details/10076323)
[的iOS tintColor解析](https://www.cnblogs.com/wfwenchao/p/4884833.html)
[IOS开发之UIView的详解](https://blog.csdn.net/zeng_zhiming/article/details/73469182)
[使用maskView设计动画](https://www.jianshu.com/p/6e360516e3bc)
[layoutMargins和preservesSuperviewLayoutMargins](https://www.jianshu.com/p/4237bd89f521)
[iOS11适配-安全区](https://blog.csdn.net/u011656331/article/details/78365326)
[一个随输入文字宽度变化的自定义的UITextField](https://www.jianshu.com/p/a5eaba3c24f7)
[HuggingPriority和CompressionResistance一个例子教你理解](https://www.jianshu.com/p/5cf559435eb9)
[砌体约束立即生效跟约束更新/绘制/ layout](https://blog.csdn.net/zpz5789/article/details/50922469)
[深入理解Auto Layout第一弹](https://blog.csdn.net/rjh_xiaoxiao/article/details/50547205)
[IOS-AutoresizesSubviews](https://blog.csdn.net/eward9888/article/details/8250556)[AutoLayout的那些事儿](http://www.cocoachina.com/ios/20160530/16522.html)
[代码添加约束，设置translatesAutoresizingMaskIntoConstraints为NO的原因](https://blog.csdn.net/u010140921/article/details/40627983)
[drawRect的绘制的使用（绘制文本字符，绘制图片，绘制图形）](https://blog.csdn.net/potato512/article/details/56845385)
[内存恶鬼drawRect中](https://blog.csdn.net/sandyloo/article/details/51063799)
[深刻理解移动端优化之离屏渲染](http://www.cocoachina.com/ios/20161114/18072.html)
[iOS的绘制和渲染](https://www.jianshu.com/p/2bbed48dbfd0)
[的iOS应用程序的实现状态恢复](https://www.jianshu.com/p/7f2fe9361f07)
作者：kirito_song 
链接：HTTPS：//www.jianshu.com/p/ea3d2970a892 
来源：书繁简
繁简书著作权归作者所有，任何形式的转载都请联系作者获得授权并注明出处。
