# Storyboard的爱与恨 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2016年02月25日 09:22:18[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：1297

尽管现在已经是Apple将Storyboard整合进Xcode中的第四个年头，大家对于Storyboard的评价仍然褒贬不一。有[早期就选择转向Storyboard](http://onevcat.com/2013/12/code-vs-xib-vs-storyboard/)用于UI开发的国内业界领头人物，也有[创建项目就立马删除Storyboard](http://martinnormark.com/the-story-of-no-storyboard-in-ios-development/)的大牛。我经历过纯代码布局，同时也在多个多人合作项目中使用Storyboard开发界面。在初期绕过各种坑后，Storyboard将会是快速构建UI界面的好帮手，特别是在现如今设备分辨率与尺寸日益增加的情况下，它可以帮助工程师们节约大量的界面代码书写时间。Storyboard存在的一大意义在于为UI提供了可视化开发方式，另一方面提供了一种更好的MVC的View层实现方式，让你的ViewController代码更简洁。当然，Storyboard的不足仍然不可忽视，错误的难以定位经常让刚上手的开发者们手足无措，相比于代码更不容易阅读的XML源文件所导致多人合作中的冲突不易解决等问题仍然有待完善。本文从各个方面介绍一下Storyboard，分享一下Storyboard的一些使用心得。
**历史**
1986年Jean-Marie Hullot发明了IB(Interface Build--Storyboard的前身),并且和Macintosh的工具箱无缝融合，这一工具被Denison Bollay发现了。第二年， Denison Bollay带着Hullot和他的IB到NeXT,将IB演示给Steve Jobs看。老乔立意识到了IB的价值，并将其纳入到了NeXTSTEP中。之后Steve 带着NeXT的技术结晶(当然也包括IB)重新回归Apple，并将之整合到了Apple的体系中。2008年第一代iPhone
 SDK发布的时候，IB就已经捆绑在其中。到了Xcode4,Apple更是直接将其集成进IDE里。随后随着不断地改进，更新，演变，最终变成了我们今天所看到的Storyboard。从某种角度来说，Storyboard也是老乔留给我们的众多礼物之一。
**故事板能做什么**
故事板主要为我们提供了以下的功能：(这些功能都是可视化的)
- 
Auto Layout
- 
Size Classes
- 
Secnce的跳转
- 
代码可视化
**Auto Layout**
自动布局颠覆了之前直接操作Frame的布局方式，从思考View应该在哪个位置,变成了考虑在特定条件下，View的所处的位置需要满足哪些条件。通过这些条件来确定View的Frame。自动布局在实际应用中大体上可以将分为三组:
View与Super View的约束
![QQ截图20160224113505.png](http://cc.cocimg.com/api/uploads/20160224/1456284915722067.png)
View自身的约束
![QQ截图20160224113548.png](http://cc.cocimg.com/api/uploads/20160224/1456284955364097.png)
View与Other View的约束
![QQ截图20160224113613.png](http://cc.cocimg.com/api/uploads/20160224/1456284979459075.png)
假如我们需要在代码中使用自动布局可以使用 Visual Format Language或者NSLayoutConstraint的简单工厂方法来生成约束，然后添加到View上。我们来看一个例子：
```
`//用代码来实现上图中View与Super View的约束`
`    ``UIView *superView = self.view;`
`    ``UIView *subView = [[UIView alloc] init];`
`    ``NSLayoutConstraint *leadingConstraint = [NSLayoutConstraint constraintWithItem:superView`
`                                                           ``attribute:NSLayoutAttributeLeading`
`                                                           ``relatedBy:NSLayoutRelationEqual`
`                                                              ``toItem:subView`
`                                                           ``attribute:NSLayoutAttributeLeading`
`                                                          ``multiplier:1`
`                                                            ``constant:15];`
`    ``NSLayoutConstraint *TrailingConstraint = [NSLayoutConstraint constraintWithItem:superView`
`                                                                         ``attribute:NSLayoutAttributeTrailing`
`                                                                         ``relatedBy:NSLayoutRelationEqual`
`                                                                            ``toItem:subView`
`                                                                         ``attribute:NSLayoutAttributeTrailing`
`                                                                        ``multiplier:1`
`                                                                          ``constant:15];`
`                                                                          ``//topConstraint init...`
`//bottomConstraint init...`
`    ``[superView addConstraint:leadingConstraint];`
`    ``[superView addConstraint:TrailingConstraint];`
`    ``[superView addConstraint:topConstraint];`
`    ``[superView addConstraint:bottomConstraint];`
`    `
`    ``// 如果是iOS8+ 则使用下面的方式来激活Constraint`
`    ``// leadingConstraint.active = YES;`
`    ``// leadingConstraint.active = YES;`
`    ``// leadingConstraint.active = YES;`
`    ``// leadingConstraint.active = YES;`
```
是不是一大团乱糟糟的代码?Visual Format Language用起来更加令人崩溃。好在业界已经有比较好的代码自动布局的第三方解决方案。但是仍然会有大堆的简单界面布局代码残留在你的代码中。
为了让你的生活更轻松(也为了让代码更清爽)，Storyboard就包含了非常优雅的可视化自动布局解决方案。以上一切，在Storyboard中都被浓缩成了两个按钮（下图红圈中的椭圆按钮）。
![1111.png](http://cc.cocimg.com/api/uploads/20160224/1456285168598907.png)
- 
红框1:为被选中View和离他最近的View(可能是SuperView,也可能是另一个同层级的View,看哪个离它更近)添加Leading、Training、Top、Bottom四个属性约束。
- 
红框2:为View添加自身宽和高约束
- 
红色椭圆左侧按钮:当选中多个View时，为多个View添加约束
只需要点击几下鼠标，Storyboard就可以帮你轻松完成视图布局。
**Auto Layout Debug**
使用代码来对Auto Layout布局的另一个缺点在于debug的困难。当添加了多余的约束，往往只能在运行时才能发现错误。同时，要寻找出是哪一行代码添加了错误的约束也比较费力(往往连控制台都没有错误输出)。
而Storyboard却为此提供了非常友好的静态检查。主要针对View的约束、布局提供警告和Error,甚至是解决方案。
![-----2016-02-19-16-30-51.png](http://cc.cocimg.com/api/uploads/20160224/1456285225845088.png)
上图的例子是：我们为Label添加了多余的约束，Storyboard用红色标记出冲突的约束，并给出修改建议:删除其中一个约束以保证约束的正确性。是不是很友好？ ：）
**Size Classes**
Apple 与iOS 8推出了Size Classes的概念。意在解决因设备尺寸造成的适配问题。Size Classes通过将界面的宽度和高度抽象为正常和紧凑两种概念，通过合理的组合，可以将现有设备(以及未来将要出现的设备)划分到不同的Size中。因此，无论是代码还是界面布局，只需要针对Size进行，而不用再拘泥于分辨是iPhone还是iPad,是横屏还是竖屏的问题了。Size Classes的推出是具有前瞻性的，无论是Apple Watch还是iOS 9推出的的iPad 分屏模式，都可以用Size Classes完美解决适配的问题。
Size Classes和现有设备的对照表如下：
![QQ截图20160224114047.png](http://cc.cocimg.com/api/uploads/20160224/1456285259294768.png)
在之前，我们要对横屏竖屏的界面进行区分，代码一般是这样的：
```
`if``(IPAD_PORTRAIT)  `
`{`
`    ``//TODO:modify something portrait`
`}`
`else`
`{`
`    ``//TODO:modify something landscape`
`}`
```
在Size Classes时代，Apple引入了一个新的类UITraitCollection来封装水平和垂直方向的Size信息。现在我们通过代码来改变界面是这样的：
```
`- (void)willTransitionToTraitCollection:(UITraitCollection *)newCollection `
`              ``withTransitionCoordinator:(id )coordinator`
`{`
`    ``[``super``willTransitionToTraitCollection:newCollection `
`                 ``withTransitionCoordinator:coordinator];`
`    ``[coordinator animateAlongsideTransition:^(id  context) `
`    ``{`
`        ``if``(newCollection.verticalSizeClass == UIUserInterfaceSizeClassCompact) {`
`            ``//To Do: modify something for compact vertical size`
`        ``} ``else``{`
`            ``//To Do: modify something for other vertical size`
`        ``}`
`        ``[self.view setNeedsLayout];`
`    ``} completion:nil];`
`}`
```
在TODO中做相对应Size的事。
可以想见的是，仍然会有非常多的布局代码占据着你的源文件。但在Storyboard中，一切变得异常简单。
使用Size Classes，我们只需要选择相对应的size，在那个Size下进行布局。运行时，就会根据设备的尺寸，自动地展示相对应Size的布局。比如iPhone竖屏就展示width Compact height RegularSize下的信息。当手机横屏，系统会自动添加一个过渡动画(虽然有点生硬),并转到width Regular height Compact的Size。这一切不需要一行代码。
能不能再给力点?
Sure.有这么一种情景:iPhone横屏下，拥有一个avatarView,竖屏下拥有一个相同的avatar View。这种情况下我们只需要在一个Size中完成这个View,然后在Storyboard的attributed inspector中做一些勾选，将其"install"进相对应的Size中，就可以达到复用的目的。如果有差异，则在对应的Size中定制即可。（如下图）
![QQ截图20160224114232.png](http://cc.cocimg.com/api/uploads/20160224/1456285370637300.png)
能不能再给力点儿？
Of Course！除了View,约束也可以不同Size配置不同。最厉害的是，图片文件也可以根据Size来区分。我们只需要对.xcassets文件勾选Size Classes,就可以为不同Size配置不同图片.这意味着，在同一个安装包下，通过Size Classes，我们甚至可以为横屏iPhone和竖屏iPhone做出完全不同的App!
![QQ截图20160224114320.png](http://cc.cocimg.com/api/uploads/20160224/1456285408336343.png)
Scene的转场
如我们所料，Storyboard也可以通过可视化的操作来实现Scene的转场。
故事板的转场有两种，可以分为手动触发和自动触发。自动触发完全由Storyboard实现，而手动触发则需要配合代码。前者简单易用，后者适用于配合业务逻辑，进行不同转场的触发。自动触发的转场非常简单，我们只需选择一个UIControl(比如UIButton),按住Control+左键，拖线至目标Scene,选择Action类型，即可在触发UIControl的某些事件的时候，自动执行转场。
![ooooo-1.gif](http://cc.cocimg.com/api/uploads/20160224/1456281137433775.gif)
例如利用UIButton转场，实际上是在触发TouchUpInside事件时执行。这一简单的操作实际上相当于如下代码:
```
`- (void)viewDidLoad`
`{`
`    ``[self.button addTarget:self`
`                    ``action:@selector(showPSViewControllerB)`
`          ``forControlEvents:UIControlEventTouchUpInside];`
`          `
`}`
`- (void)showPSViewControllerB`
`{`
`    ``PSViewControllerB *viewController = [[PSViewControllerB alloc]init];`
`    ``//配置..传值...`
`    ``[self.navigationController pushViewController:viewController animated:YES];`
`}`
```
Storyboard将Scene转场变成了可视化的操作又引入了一个新的问题，需要如何传递参数给目标ViewController？
解决方法就是，我们需要在Storyboard中给Segue一个Identifier，然后在源ViewController中重写如下方法即可:
```
`- (void)prepareForSegue:(UIStoryboardSegue *)segue sender:(id)sender`
`{`
`    ``if``([segue.identifier isEqualToString:[PSViewControllerB description]])`
`    ``{`
`        ``PSViewControllerB *vc = segue.destinationViewController;`
`        ``//配置..传值..`
`    ``}`
`}`
```
手动触发则需要代码配合。不同的是，拖线的对象从UIControl变成了UIViewController(不要忘了在Storyboard中填写Segue Identifier)。
![aaa.gif](http://cc.cocimg.com/api/uploads/20160224/1456281217784489.gif)
然后在代码中需要转场的地方，加上performSegueWithIdentifier:sender:即可。例子如下：
```
`//self:PSViewControllerA`
`    ``if``(isBizSuccess){`
`        ``[self performSegueWithIdentifier:[PSViewControllerB description] sender:parameter];`
`    ``} ``else``{`
`        ``[self showTips:@``"some failure reason"``];`
`    ``}`
```
你可以利用performSegueWithIdentifier:sender:来进行任何形式的转场。Segue为我们的转场提供了不同的Action，囊括了常见的UINavigationViewController的push，或者所有ViewController都可以执行的Modelly Presentation。
事实上，在iOS 8以后，我们就可以利用Storyboard结合代码实现自定义的转场，无论是在哪一种上下文环境中。
采用Storyboard进行Scene转场的好处在于，一个ViewController的所有转场代码，都集中到了prepareForSegue:sender: 方法中，debug或者添加新功能时，可以很容易顺藤摸瓜。但缺点同样明显。每次转场的修改/删除需要同时修改Storyboard和代码文件。同时，随着项目的进行，越来越多的Scene和业务逻辑，导致Storyboard中Segue的数量剧增，难以维护。
巨量的Segue(仅仅是部分截图)
![-----2016-02-22-17-04-05.png](http://cc.cocimg.com/api/uploads/20160224/1456281261411270.png)
**多Storyboard协作**
解决如上问题的方法就是，尽量将项目的界面分割在多个Storyboard文件中。一个最佳实践是，按照项目功能模块来区分故事板，例如Login.Storyboard,Chat.Storyboard,Person.Storyboard等。尽量把每个Storyboard的Scene数量控制在20个以内。
同时，Scene间的转场我们依然可以采用Segue,并且使用起来和单个Storyboard无异。这要多亏Apple在iOS 9新推出的[UIStoryboard Reference](http://shengpan.net/top-tips-for-ios9/#StoryboardReference)。
**代码可视化**
还有什么能比代码可视化更加炫酷的呢？作为前端工程师，最享受的时候，就是枯燥的代码和算法变成了优美的动画。但这一切都只在按下command+R之后。
现在，通过Storyboard,我们也可以在编译时实时预览我们的代码所产生的效果。
![IB_DESIGNABLE.gif](http://cc.cocimg.com/api/uploads/20160224/1456281330792796.gif)
通过为自定义的View添加IB_DESIGNABLE关键字（注意图中关键字的位置）,我们让Storyboard为我们自定义的视图进行实时渲染。有的人可能会担心实时渲染造成的性能问题。这点大可放心，Xcode有一套非常优秀的缓存机制(优秀到有些时候必须要clean一下，某些小改动才会在真机上生效),只需要编译一次，视图就会被缓存，不会造成每次在Storyboard、代码文件中切换时多次渲染的问题。
在swift中则为@IBDesignable，放在class关键字之前
到这里令人惊叹的类似Playground的事实渲染功能，已经可以动态地应用在项目中了。我们可以利用IB_DESIGNABLE和IBInspectable来制作图表等高度自定义的、独特的视图。
当然，故事板狂魔对故事板的使用不会就此罢手的，本着一切能用Storyboard配置就不写代码的原则，我们也希望可以在故事板中配置自定义控件的属性。幸运的是，Apple再次为我们的想法提供了可能。
**IBInspectable**
![IBInspectable.gif](http://cc.cocimg.com/api/uploads/20160224/1456281374553319.gif)
通过为自定义View的属性添加IBInspectable关键字（注意图中关键字的位置），我们可以将原本需要代码配置的属性，放到故事板中。IBInspectable支持以下类型的属性：
- 
BOOL
- 
NSString
- 
NSNumber
- 
CGPoint
- 
CGSize
- 
CGRect
- 
UIColor
- 
NSRange
- 
UIImage
在swift中则为@IBInspectable，放在var关键字之前
**为系统控件添加IBInspectable**
不少设计设都喜欢设计圆角。通常我们需要写如下代码:
```
`view.layer.cornerRadius = 5;`
`view.layer.masksToBounds = YES;`
```
为了解决这些重复代码的问题，有的人喜欢为View写Category，一行代码实现圆角。然而这需要在不同的ViewController中不断引入这个Category,不够优雅。当然，这种小事情我们也肯定不会愿意采用继承的。
实际上，我们只需要为项目添加一个View的Category，在其中声明一个@property并加上IBInspectable关键字,然后在实现文件中的getter&&setter方法中实现具体的逻辑。不用import头文件，也不需要运行,Storyboard中将自动出现这个属性以供配置。这不正是我们梦寐以求的完全解耦吗！？
```
`//UIView+CornerRadius.h`
`@interface UIView (CornerRadius)`
`@property (nonatomic, assign) IBInspectable CGFloat cornerRadius;`
`@end`
```
```
`//UIView+CornerRadius.m`
`@implementation UIView (CornerRadius)`
`- (void)setCornerRadius:(CGFloat)cornerRadius`
`{`
`    ``self.layer.cornerRadius = cornerRadius;`
`    ``self.layer.masksToBounds = cornerRadius > 0;`
`}`
`- (CGFloat)cornerRadius`
`{`
`    ``return``self.layer.cornerRadius;`
`}`
`@end`
```
![CornerRadius.gif](http://cc.cocimg.com/api/uploads/20160224/1456281543179496.gif)
实际上，IBInspectable是对运行时属性进行的一种拓展，你在Attributed Inspector中进行的自定义属性配置，都会在Identity Inspector的运行时属性中得到体现。
**Storyboard的弊端**
Storyboard也并非十全十美的。它依然有许多的问题亟待解决，有些致命的问题，更是成为导致许多开发者放弃Storyboard的原因。在iOS9普及率已经达到[77%](https://developer.apple.com/support/app-store/)的今天，Storyboard仍然有很多问题需要完善。
**难以维护**
Storyboard在某些角度上，是难以维护的。我所遇到过的实际情况是，公司一个项目的2.0版本，设计师希望替换原有字体。然而原来项目的每一个Label都是采用Storyboard来定义字体的，因此替换新字体需要在Storyboard中更改每一个Label。
幸亏我们知道Storyboard的源文件是XML，最终写了一个读取-解析-替换脚本来搞定这件事。
**性能瓶颈**
当项目达到一定的规模，即使是高性能的MacBook Pro,在打开Storyboard是也会有3-5秒的读取时间。无论是只有几个Scene的小东西，还是几十个Scene的庞然大物，都无法避免。Scene越多的文件，打开速度越慢(从另一个方面说明了分割大故事板的重要性)。
让人沮丧的是，这个造成卡顿的项目规模并不是太难达到。
我猜想是由于每一次打开都需要进行I/O操作造成的，Apple对这一块的缓存优化没有做到位。可能是由于Storyboard占用了太多内存，难以在内存中进行缓存。Whatever,这个问题总是让人困扰的。
然而需要指出的是，采用Storyboard开发或采用纯代码开发的App，在真机的运行效率上，并没有太大的区别。
**错误定位困难**
Storyboard的初学者应该对此深有体会。排除BAD_EXCUSE错误不说，单单是有提示的错误，就足以让人在代码和Storyboard之间来回摸索，却无法找到解决方案。
一个典型的例子是，在代码中删除了IBOUTLET属性或者IBAction方法，但是却忘了在Storyboard中删除对应的连接，运行后crash。然而控制台只会输出一些模糊其词的错误描述。
```
`*** Terminating app due to uncaught exception ``'NSUnknownKeyException'``, `
`reason: '[ setValue:forUndefinedKey:]:  `
`this``class is not key value coding-compliant ``for``the key drawButton.'`
```
有经验的开发者可以从drawButton这个关键字中找到突破口，但大部分刚接触Storyboard的开发者，会被困在其中。
**最后**
综合其利弊，毅然选择了站在Storyboard这边。一方面是其提供的便利，另一方面是Apple对Storyboard的大力支持。这一点宏观上看，可以在以往对Storyboard的改进和增强上看出，微观上看，几乎所有iOS 8之后的simple code都或多或少采用了Storyboard作为界面开发工具。有理由相信，Storyboard的未来是光明的。
愿大家在Storyboard的路(keng)上，越走越远。
