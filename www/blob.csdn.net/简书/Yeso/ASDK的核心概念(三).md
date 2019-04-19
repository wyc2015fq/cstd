# ASDK的核心概念(三) - 简书
# ASDK的核心概念(三)
**ASDK的核心概念**
> 
一: 预加载
二: Node Containers
三: Node Subclasses
#### 一: 预加载
`Texture`的异步渲染、计算和测量特性使其非常强大，另外对于`textuer`来说另一个至关重要是智能预加载。
正如“[入门指南](http://texturegroup.org/docs/getting-started.html)”中指出的那样，`node`脱离'node containers'将变的一无是处。这是由于所有`node`都具备接口状态的概念。
所有容器都在内部创建和维护的`ASRangeController`用于更新`node`的状态
在容器外部使用的节点不会由任何范围控制器更新其状态。这有时会导致闪烁，我们无法捕捉每个`node`的状态
###### 1.1: Interface State Ranges(接口状态范围)
当节点添加到滚动或分页界面时，它们通常位于以下范围之一中。这意味着随着滚动视图的滚动，它们的界面状态将在它们移动过程中更新。
![](https://upload-images.jianshu.io/upload_images/2092929-4c0ea17d1ca12260.png)
接口状态范围
**节点将处于以下范围之一中:**
|接口状态|描述|
|----|----|
|Preload|远离可见的最远距离。这是从外部来源收集内容的地方，无论是API还是本地磁盘。|
|Display|在这里，显示任务，如文本光栅化和图像解码。|
|Visible|该节点在屏幕上至少有一个像素。|
###### 1.2： ASRangeTuningParameters(范围的调节参数)
每个这些范围的大小以“screenfuls”来衡量。尽管默认大小在很多用例中都能很好地工作，但可以通过在滚动节点上为范围类型设置调整参数来轻松调整它们。
![](https://upload-images.jianshu.io/upload_images/2092929-c0abc91977f1b79f.png)
image
在上述滚动`collection`的可视化中，用户正在向下滚动。如您所见，将要展示方向上的范围大小比用户远离的内容（拖尾方向）大得多。 如果用户要改变方向，则前端和后端将动态交换以保持内存使用最佳。 这使您可以担心定义前导和尾随尺寸，而无需担心对用户变化的滚动方向作出反应。
智能预加载也可以在多个方面工作。
###### 1.3： Interface State Callbacks(接口状态的回调)
当用户滚动时，`node`在范围内移动并通过加载数据，渲染等方式做出适当反应。您自己的节点子类可以通过实现相应的回调方法轻松访问此机制。
- 1: Visible Range(可见区域)
- `-didEnterVisibleState`
- `-didExitVisibleState`
- 2: Display Range(将要呈现)
- `-didEnterDisplayState`
- `-didExitDisplayState`
- 3: Preload Range(远端加载)
- `-didEnterPreloadState`
- `-didExitPreloadState`
#### 二: Node Containers(节点容器)
###### 2.1: Use Nodes in Node Containers(在节点容器中使用节点)
**强烈建议您在节点容器中使用Texture的节点。 Texture提供以下节点容器。**
|Texture节点容器|UIKit等效|
|----|----|
|[ASCollectionNode](http://texturegroup.org/docs/containers-ascollectionnode.html)|UICollectionView|
|[ASPagerNode](http://texturegroup.org/docs/containers-aspagernode.html)|UIPageViewController|
|[ASTableNode](http://texturegroup.org/docs/containers-astablenode.html)|UITableView|
|[ASViewController](http://texturegroup.org/docs/containers-asviewcontroller.html)|UIViewController|
|ASNavigationController|UINavigationController 实现`ASVisibility`协议。|
|ASTabBarController|UITabBarController 实现`ASVisibility`协议。|
示例代码和特定示例项目在每个节点容器的文档中突出显示。
###### 2.2: 通过使用节点容器获得什么？
节点容器自动管理其节点的智能预加载。这意味着所有节点的布局测量，数据读取，解码和渲染都将以异步方式完成。这就是为什么建议使用容器节点内的节点的原因。
**请注意，虽然可以直接使用节点（不使用Texture节点容器），除非添加其他调用，否则只有在屏幕上显示时才会开始显示（如UIKit所做的那样）。 这可能导致性能下降和内容闪烁。**
#### 三: Node Subclasses(节点子类)
Texture提供以下节点。
与UIKit组件相比，使用节点的一个关键优势是 **突破了UIKit组件必须要在主线程进行布局和显示的限制**，因此使用`Texture`可以保证主线程可以及时响应用户交互事件。
|Texture Node|UIKit Equivalent|
|----|----|
|[ASDisplayNode](http://texturegroup.org/docs/display-node.html)|UIView 所有其他节点都从其继承的根Texture节点|
|[ASCellNode](http://texturegroup.org/docs/cell-node.html)|UITableViewCell & UICollectionViewCell **ASCellNode用于ASTableNode，ASCollectionNode和ASPagerNode**|
|[ASScrollNode](http://texturegroup.org/docs/scroll-node.html)|UIScrollView  此节点对于创建包含其他节点的自定义可滚动区域很有用|
|[ASEditableTextNode](http://texturegroup.org/docs/editable-text-node.html)|UITextView|
|[ASTextNode](http://texturegroup.org/docs/text-node.html)|UILabel|
|[ASImageNode](http://texturegroup.org/docs/image-node.html)、    [ASNetworkImageNode](http://texturegroup.org/docs/network-image-node.html)、   [ASMultiplexImageNode](http://texturegroup.org/docs/multiplex-image-node.html)|UIImage|
|[ASVideoNode](http://texturegroup.org/docs/video-node.html)|AVPlayerLayer|
|ASVideoPlayerNode|UIMoviePlayer|
|[ASControlNode](http://texturegroup.org/docs/control-node.html)|UIControl|
|[ASButtonNode](http://texturegroup.org/docs/button-node.html)|UIButton|
|[ASMapNode](http://texturegroup.org/docs/map-node.html)|MKMapView|
`Texure`无缝的对接UIKit，除此之外`Texture`节点提供更高级的功能和便利。 例如，`ASNetworkImageNode`会自动加载和缓存管理，甚至支持渐进式jpeg和动画gif。
[AsyncDisplayKitOverview](https://github.com/texturegroup/texture/tree/master/examples/AsyncDisplayKitOverview)示例应用程序给出了上面列出的每个节点的基本实现。
#### 三: Node Inheritance Hierarchy(节点继承层次结构)
**所有texture节点都从ASDisplayNode继承。**
![](https://upload-images.jianshu.io/upload_images/2092929-30403b82ed32351f.png)
Node Inheritance Hierarchy
以蓝色突出显示的节点是UIKit元素的同步包装。例如，`ASScrollNode`包装一个`UIScrollView`，`ASCollectionNode`包装一个`UICollectionView`。 `liveMapMode中的ASMapNode`是`UIMapView`的同步包装器。
#### 四: Subclassing(子类)
创建子类时最重要的区别在于是`ASViewController`还是`ASDisplayNode`的子类。这听起来很明显，但由于其中的一些差异很微妙，因此一定注意区分。
###### 4.1： ASDisplayNode
虽然子类化节点类似于编写UIView子类，但还是有一些指导原则需要遵循，以确保您充分利用该框架的潜力，并确保您的节点按预期行事。
**4.1.1: -init**
使用`nodeBlocks`时，在后台线程上调用此方法。但是，因为在-init完成之前没有其他方法可以运行，所以在此方法中不应该有锁。
要记住的最重要的事情是你的init方法必须能够在任何队列上被调用。 最值得注意的是，**这意味着您不应该初始化任何UIKit对象，触摸节点的视图或图层（例如`node.layer.X`或`node.view.X`），或者在初始化程序中添加任何手势识别器**。 取而代之的是在`-dldLoad`中执行这些操作。
**4.1.2: -didLoad**
这个方法在概念上类似于`UIViewController`的`-viewDidLoad`方法;它被调用一次，并且是加载后台视图的点。它可以保证在主线程中调用，并且是执行任何UIKit事物（例如添加手势识别器，触摸视图/图层，初始化UIKit对象）的适当位置。
**4.1.3： -layoutSpecThatFits:**
该方法定义了布局，并在后台线程上执行繁重的计算。**此方法用于构建将生成节点大小的布局规范对象，以及所有子节点的大小和位置。 这是您将放置大部分布局代码的地方**。
您创建的布局规范对象具有延展性，直到它在此方法中返回为止。 在这之后，它将是不可变的。 记住不要缓存布局规格供以后使用，而是在必要时重新创建它们。
由于它在后台线程上运行，因此不应在此处设置任何node.view或node.layer属性。另外，除非您知道自己在做什么，否则不要在此方法中创建任何节点。此外，与其他方法覆盖不同，调用super的方法并不是必需的。
**4.1.4： -layout**
在这种方法中super调用是layoutSpec的结果应用的地方;在此方法调用super之后，布局规格将被计算并且所有子节点将被测量和定位。
-layout在概念上与`UIViewController`的`-viewWillLayoutSubviews`类似。 这是更改隐藏属性的好地方，如果需要（不可布局的属性）设置基于视图的属性或设置背景颜色。 您可以将背景颜色设置放在`-layoutSpecThatFits:`中，但可能存在计时问题。 如果你碰巧使用任何UIViews，你可以在这里设置它们的框架。 但是，您始终可以使用`-initWithViewBlock`创建节点包装：然后在其他位置的后台线程上调整大小。
这个方法在 **主线程**中调用。但是，如果您使用`layout Specs`，您将不会过多依赖此方法，因为最好远离主线程进行布局。 除非使用1到10个子元素你可以使用这个方法。
对于你希望子节点成为你的确切大小的特定情况，一个很好的使用 `-layout` 布局。例如: 当你想要一个`collectionNode`占据整个屏幕,这种情况不受布局规范的支持，通常在这种方法中用一行手动设置框架最简单：
```
subnode.frame = self.bounds;
```
如果你希望在`ASViewController`中有同样的效果，你可以在`-viewWillLayoutSubviews`中做同样的事情，除非你的节点是`initWithNode`中的节点：在这种情况下，它会自动完成。
###### 4.2： ASViewController
`ASViewController`是一个常规的`UIViewController`子类，它具有管理节点的特殊功能。 **由于它是一个`UIViewController`子类，因此所有方法都在主线程上调用（并且您应该始终在主线程上创建一个`ASViewController`)**
**4.2.1：-init**
该方法在`ASViewController`生命周期的最初阶段被调用一次。 和`UIViewController`初始化一样，最好的做法是不要在这个方法中访问`self.view`或`self.node.view`，因为它会强制视图被提前创建。 相反，在`-viewDidLoad`中执行任何视图访问。
`ASViewController`的指定初始化程序是`initWithNode :`一个典型的初始化程序看起来像下面的代码。请注意在调用`super`之前如何创建`ASViewController`的节点。 `ASViewController`类似于`UIViewController`管理视图来管理节点，但初始化稍有不同。
```
OC
- (instancetype)init
{
  _pagerNode = [[ASPagerNode alloc] init];
  self = [super initWithNode:_pagerNode];
  // setup any instance variables or properties here
  if (self) {
    _pagerNode.dataSource = self;
    _pagerNode.delegate = self;
  }
  
  return self;
}
```
```
swift
init() {
  let pagerNode = ASPagerNode()
  super.init(node: pagerNode)
  pagerNode.setDataSource(self)
  pagerNode.setDelegate(self)
}
```
**4.2.2：-loadView**
我们建议您不要使用这种方法，因为它与`-viewDidLoad`相比没有什么特别的优势，并且有一些缺点。 但是，只要不将`self.view`属性设置为不同的值，就可以安全使用。 对`[super loadView]`的调用会将其设置为`node.view`。
**4.2.3： -viewDidLoad**
这个方法在`ASViewController`的生命周期中被调用一次，紧接在`-loadView`之后。 这是您访问节点视图的最早时间。 这是放置任何设置代码的好地方，它只能运行一次，并需要访问视图/图层，例如添加手势识别器。
布局代码不应该放在这个方法中，因为当视图的几何变化时它不会再被调用。 注意这对UIViewController同样适用;即使您目前不期望元素的几何变化，在此方法中放置布局代码也是不好的做法。
**4.2.4 -viewWillLayoutSubviews**
该方法在与节点的`-layout`方法完全相同的时间被调用，并且可以在`ASViewController`的生命周期中多次调用该方法; 只要`ASViewController`节点的边界发生变化（包括旋转，分割屏幕，键盘显示）以及层次结构发生变化（儿童被添加，删除或更改大小），就会调用它。
为了一致性，最好的做法是将所有布局代码放入此方法中。因为它不是非常频繁地调用，所以即使代码不直接依赖于大小，也属于这里。
**4.2.5 -viewWillAppear: / -viewDidDisappear:**
这些方法在`ASViewController`的节点出现在屏幕上（最早可见时）以及从视图层次结构（最早不再可见的时间）之后立即调用。 这些方法提供了一个很好的机会来启动或停止与呈现或解除控制器相关的动画。 这也是一个记录用户操作的好地方。
尽管这些方法可能被多次调用，并且几何信息可用，但它们不会被调用用于所有几何变化，因此不应该用于核心布局代码（超出特定动画所需的设置）。
#### 五: FAQ
###### 5.1: 常见的开发者错误
- 1: 不要在`-init:`中访问节点的视图。
- 2: 确保您访问`nodeBlock`外部的数据源。
- 3: 采取措施避免`viewBlocks`中的保留周期。
###### 5.2: 常见的概念误解
- 1: `ASCellNodes`不可重用。
- 2: 布局规格每次调用布局时都会重新生成。
- 3: 我们强大的`Layout API`中使用的所有尺寸之间的差异。
###### 5.3: 常见问题
- 如果您关心性能，请勿使用`CALayer`的`.cornerRadius`属性（或`shadowPath`，`边框`或`遮罩`）。
- `Texture`不支持UIKit自动布局。
- 我可以在`Texture`中使用我的`UICollectionViewCells`吗？
- `ASDisplayNode`保持活动参考。
**在加载之前访问节点的视图**
Node的`-init`方法通常非主线程被调用，因此必须不要访问UIKit对象。常见错误的示例包括访问节点的视图或创建手势识别器。取而代之的是, 这些操作非常适合在`-dldLoad`中执行。
**确保您访问节点块外的数据源**
`indexPath`参数仅在`nodeBlockForItemAtIndexPath：`或`nodeBlockForRowAtIndexPath：`中返回的节点块外有效。由于这些块是在后台线程上执行的，由于数据源中的其他更改，indexPath可能会因执行时间而无效
查看如何在`ASTableNode`页面中正确编码节点块的示例。就像使用`UIKit`一样，如果从任何`ASCellNode`的块返回`Nil`，它将导致异常。
**采取措施避免viewBlocks中的保留周期**
使用`initWithViewBlock`时：通过捕获对自身的强引用来防止保留周期很重要. 循环可以创建的两种方式是使用块内的任何实例变量或直接引用自身而不使用弱指针
只要通过指向自我的弱指针访问属性，就可以使用属性而不是实例变量。
因为`viewBlocks`总是在主线程上执行，所以执行UIKit操作（包括手势识别器的创建和添加）是安全的。
虽然该块在创建视图后被销毁，但如果该块从未运行并且该视图永远不会创建，那么一个循环可以持续，防止释放内存。
**ASCellNode可重用性**
`Texture`不使用单元重用，由于许多特定的原因，其中一个副作用就是消除了与细胞再利用相关的大量错误。
**重新生成LayoutSpecs**
每次调用`layoutThatFits:`方法时，都会重新生成一个节点的`layoutSpec`
**布局API大小调整**
如果您对`ASRelativeDimension`，`ASRelativeSize`，`ASRelativeSizeRange`和`ASSizeRange`感到困惑，请查看我们的`Layout API`调整[指南](http://texturegroup.org/docs/layout-api-sizing.html)。
**CALayer的.cornerRadius属性杀死性能**
`CALayer`的`.cornerRadius`属性是一个灾难性的昂贵的属性，只能在没有其他选择的情况下使用.
它是CALayer上效率最低，渲染密集度最高的属性之一（与`shadowPath`，`遮罩`，`边框`等一起,
这些属性会触发离屏渲染，以在每个帧上执行剪裁操作.滚动期间60FPS！ - 即使该区域的内容没有变化。
使用`cornerRadius`会在`iPhone 4`，`4S`和`5 / 5C`（以及可比的`iPad / iPod`）上视觉上降低性能，并降低头部空间，并使5S和更新设备上的帧丢失更可能。
如需更长时间的讨论和简单的替代圆角解决方案，请阅读我们全面的[圆角圆角指南](http://texturegroup.org/docs/corner-rounding.html)。
**纹理不支持UIKit Auto Layout或InterfaceBuilder**
Texture不支持`UIKit Auto Layout`和`InterfaceBuilder`。值得注意的是，这些技术都不允许在已建立和有纪律的iOS开发团队中使用，例如`Facebook`，`Instagram`和`Pinterest`
但是，`Texture`的[Layout API](http://texturegroup.org/docs/automatic-layout-basics.html)提供了各种[ASLayoutSpec object](http://texturegroup.org/docs/automatic-layout-containers.html)，允许实现更高效的自动布局（多线程，离开主线程), 更易于调试（可以进入代码并查看所有值来自哪里，因为它是开源的），并且可重用（可以构建可与UI的不同部分共享的可组合布局）。
**ASDisplayNode保持活动参考**
该保留周期是故意创建的，因为节点处于“活动”视图层次结构中（它位于屏幕上的UIWindow内）。
要明白为什么这是必要的，请考虑Apple也在UIView和CALayer之间创建了这个保留循环,如果您创建一个UIView并将其图层添加到一个超级图层，然后释放UIView，它将保持活动状态，即使指向它的CALayer委托很弱
出于同样的原因，如果节点的视图是窗口的后代，但没有对节点的引用，我们使节点保持活动状态，并从视图中引用强引用。
良好的应用程序设计不应该依赖这种行为，因为对节点的强引用应该由子节点数组或实例变量维护。 但是，这种情况偶尔会发生，例如使用UIView动画API时。这个循环不应该造成泄漏，甚至不会延长节点的生命周期，而不会超过绝对必要的时间。
**UICollectionViewCell兼容性**
Texture支持使用`UICollectionViewCells`和本地`ASCellNodes`。
请注意，即使在同一个`ASCollectionNode`中混合使用，**这些UIKit单元也不具备ASCellNodes的性能优势（如预加载，异步布局和异步绘制）**。
但是，这种互操作性使开发人员能够灵活地测试出框架，而无需一次转换所有的单元。在这里阅读[更多](http://texturegroup.org/docs/uicollectionviewinterop.html)。
[iOS 开发一定要尝试的 Texture(ASDK)](https://didee.cn/2018/01/29/iOS-ASDK/)
[Texture 布局篇](https://bawn.github.io/2017/12/Texture-Layout/?utm_source=tuicool&utm_medium=referral)
[iOS性能优化探讨](https://mp.weixin.qq.com/s/7rJcRErxIAFRRD6czhguKQ)
[新大陆：AsyncDisplayKit](https://segmentfault.com/a/1190000007991853)
[预加载与智能预加载（iOS）](https://draveness.me/preload)
[AsyncDisplayKit 2.0 Tutorial: Getting Started](https://www.raywenderlich.com/124311/asyncdisplaykit-2-0-tutorial-getting-started)
[AsyncDisplayKit 2.0 Tutorial: Automatic Layout](https://www.raywenderlich.com/124696/asyncdisplaykit-2-0-tutorial-automatic-layout)
