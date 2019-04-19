# iOS UIKit框架注解 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2017年04月13日 08:55:15[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：1994
#### 本文来自简书，原文地址:[http://www.jianshu.com/p/e5b995ecf44d](http://www.jianshu.com/p/e5b995ecf44d)#### 导语> 我们追求技术的提升，关注技术的发展历程；作为从事技术工作的伐码猿，有个想法（`仅是个人想法`），这个想法工程量之大（`遇到，用到，最新技术点，点点积累，及时补充吧`），从事技术多年也算是对所花费时间的一个总结（`算是对技术这块的一个目标吧`）。这篇文章的内容仅是【UIKit框架 各分类文件的一个简明介绍】，重点 -> 重点 -> 重点（```重要的事情说 2+1 遍```），我的「想法和行动」都在文章末尾![](http://upload-images.jianshu.io/upload_images/2230763-0fd6bbb32d5a58aa.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)「UIKit Framework Annotation  Practical Case」我们通常称为 “Cocoa框架”，事实上 Cocoa 本身是一个框架的集合，它包含了众多子框架，其中最重要的要数 “Foundation” 和 “UIKit” 。前者是框架的基础，和界面无关，其中包含了大量常用的API；后者是基础的UI类库，以后我们在 iOS 开发中会经常用到。这两个框架在系统架构中的位置如下图：![](http://upload-images.jianshu.io/upload_images/2230763-75296d35dc3f2f7c.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240) Cocoa框架上图注解：- 1、Core OS 层位于 `iOS` 框架的最底层，主要包含内核、文件系统、网络基础架构、安全管理、电源管理、设备驱动、线程管理、内存管理等。简而言之，该层提供了最低级的、系统级的服务。- 2、Core Services 层可称之为核心服务层，顾名思义，它提供诸如字符串管理、集合管理、网络操作、`URL`实用工具、联系人管理、偏好设置等服务。除此之外，它还提供很多基于硬件特性的服务，如`GPS`、加速仪、陀螺仪等。该层包含了```Core Location```、`Core Motion`、`SystemConfiguration`、`Foundation`与```Core Foundation```子模块。其中`Foundation`与```Core Foundation```子模块提供了对公共数据类型(字符串、集合等)的抽象，`Core Foundation`中的```Core Data```子模块可以实现对象图形管理与对象持久化。- 3、Media 层：依赖于Core `Services`层提供的功能，主要负责图形与多媒体服务。它包含了`CoreGraphics`、```Core Text```、`OpenGL ES`、```Core Animation```、`AVFoundation`、```Core Audio```等与图形、视频和音频相关的功能模块。- 4、 Cocoa Touch 层 是直接向`iOS`应用程序提供各种基础功能的支持。其中的`UIKit`框架提供各种可视化控件供应用程序使用，如窗口、视图、视图控制器与各种用户控件等。另外`UIKit`也定义了应用程序的默认行为和事件处理结构。UIKit 框架 是 Objective-C 语言应用于苹果移动设备的界面呈现、管理功能库，功能之强大，本专题 致力于将苹果官网的海量的文档进行 归类整理，框架各分类文件注解，并对每一类中常用的功能进行案例渐进式解析，以 简化初学者入门和老手回顾的繁索过程，尽快切入主题，快速应用起来。在「时间 & 知识 」有限内，总结的文章难免有「未全、不足 」的地方，还望各位好友指出，以提高文章质量。这里我们不妨也看一下 UIKit 类结构：![](http://upload-images.jianshu.io/upload_images/2230763-0a6913fa199b11c5.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240) UIKit 框架- 1. UIAcceleration:被叫做加速事件的一个`UIAcceleration`类的实例是用来代表即时的三维加速数据。为了接收重力加速度，要注册一个应用应用程序作为一个共享`UIAccelerater`对象的委托对象（参考`UIAcceleromete`类）。- 2. UIAccelerater:`UIAccelerater`类可以让你的寄存器接收到从板载硬件上得到的加速相关数据。当设备移动时，它的硬件能够报告沿主线在三维空间中的线性加速度变化。你可以利用这些数据来检测器件的电流方向和该方向的瞬时变化。你可以使用这瞬时变化输入到一个游戏或者启动应用程序中的一些行动。- 3. UIaccessibilityElement:`UIAccessibilityElement`类封装的项目信息对一些特殊的人可见，默认情况下是不可访问的。例如，一个图标或文字图像不会自动访问，因为它没有继承的`UIView`（或`UIControl`）。一个包含等非视图的项目，创建一个实例来表示每个项目需要访问的`UIAccessibilityElement`。- 4. UIBarItem:`UIBarItem`是一个抽象的超类用来在屏幕的底部给项目加上一栏。带栏的条目类似于一个按钮的方式。他们有标题，有图片，动作和目标。你可以在项目中启用和禁用一个带栏条目。- 4.1 UIBarButtonItem:一个栏按钮项目是放置在一个`UIToolbar`或`UINavigation`对象专门的按钮。它从它的抽象父类，`UIBarItem`继承的基本按钮的行为。`UIBarButtonItem`定义额外的初始化方法和属性，使用工具栏和导航栏。- 4.2 UITabBarItem:`UITabBarItem`类实现了一个带标签栏的项目，`UITabBar`类的实例。一个标签栏严格地以收音机模式来运作，其中一个项目是在选定的时间，点击标签栏项目切换上面的标签栏。您还可以指定在标签栏上项目的信息，例如加入额外的视觉徽章值，电话应用程序上使用该项目的徽章，以显示新邮件的数量。这个类还提供了一些系统默认创建的项目。- 5. UIBezierPath:`UIBezierPath`类，您可以定义一个路径由直线和曲线的线段组成，并呈现在您的自定义视图路径。您可以使用这个类，最初只是为您的路径中指定的几何形状。路径可以定义简单的形状，如矩形，椭圆形和圆弧，或他们可以定义复杂的多边形，采用直线和曲线线段的混合物。定义的形状后，您可以使用这个类的其他方法来呈现当前图形上下文的路径。- 6. UIColor:一个`UIColor`对象代表的颜色，有时不透明（`alpha`值）。您可以使用`UIColor`对象来存储颜色数据，并在绘画过程中，你可以用它们来设置当前填充和笔触颜色。- 7. UIDecive:`UIDevice`类提供了一个单个实例代表当前设备。从这个实例中，可以获取有关设备的唯一的`ID`，分配名称，设备型号，和操作系统名称和版本等信息。您也可以使用的`UIDevice`实例，检测设备的特点，如物理方向的变化。使用方向属性得到当前的方向，或注册`UIDeviceOrientationDidChangeNotification`通知收到更改通知。在使用这些技术来获得定位数据之前，你必须使数据传递，使用的`beginGeneratingDeviceOrientationNotifications`方法。当您不再需要跟踪设备的方向，调用`endGeneratingDeviceOrientationNotifications`方法来禁用交付的通知..- 8. UIDocumentInteractionController:一个文件交互控制器一起委托对象，提供应用程序管理与本地系统中的文件的用户交互的支持。例如，一个电子邮件程序可能使用这个类，允许用户预览附件和其他应用程序中打开它们。使用这个类，目前预览相应的用户界面，打开，复制或打印指定的文件。- 9. UIEvent:一个`UIEvent`对象（或者简单地说，一个事件对象）代表在`iOS`的事件。一般有三种类型的事件：触摸事件，运动事件和远程控制的事件，。远程控制的事件使一个`Responder`对象来接收来自外部的附件或耳机的命令，以便它可以管理管理音频和视频，例如，播放视频或跳过到下一音轨。- 10. UIFont:`UIFont`类提供了用于获取和设置字体信息的接口。类提供了访问字体的特点，还提供了访问，这是在布局过程中中使用的字体的字形信息系统。他们通过接受它们作为参数的方法，您可以使用字体对象。- 11. UIGestureRecognizer:`UIGestureRecognizer`是一个具体的手势识别类的抽象基类。一个手势识别对象（或简单地说，一个手势识别）中分离出来承认这种认识上的姿态和行动的逻辑。当这些对象之一承认一个共同的姿态，或者，在某些情况下，改变的姿态，它发送一个动作消息，每一个指定的目物镜。以下胃具体的子类：- 11.1 UITapGestureRecognizer:点击手势。指定数量的手指必须要承认的姿态，挖掘查看指定的次数。- 11.2 UIPinchGestureRecognizer:看起来捏的手势，涉及两个接触。当用户将两个手指，向对方的传统意义是缩小;当用户将两个手指从彼此远离，传统意义变焦。- 11.3 UIRotationGestureRecognizer:看起来轮换涉及两个触摸手势。当用户移动手指对面对方的圆周运动，基本的观点应该在相应的方向和速度旋转。- 11.4 UISwipeGestureRecognizer:看起来刷卡在一个或多个方向的手势。抨击是一个独立的姿态，因此，相关的操作的消息发送每个手势只有一次。- 11.5 UIPanGestureRecognizer:看起来平移（拖动）的手势。用户必须按查看上一个或更多的手指，而他们平移。实施这个手势识别动作方法的客户端可以要求它目前的翻译和手势的速度。- 11.6 UILongPressGestureRecognizer:看起来长按手势。用户必须按下一个或更多的手指行动讯息传送至少指定期限。此外，手指可能要承认的姿态移动唯一指定的距离;如果他们超越这个限制的姿态失败。- 12. UIImage:一个`UIImage`对象是一个高层次的的方式来显示图像数据。您可以从文件中创建的图像，从石英图像中的物体，或从您收到的原始图像数据。`UIImage`的类还提供图像绘制到当前图形的上下文中，使用不同的混合模式和不透明度值的几种选择。- 13. UILocalizedIndexedCollation:`UILocalizedIndexedCollation`类方便为有部分指数的图表进行组织，整理，以及数据本地化。表视图的数据源，然后使用排序对象提供的输入节的标题和节索引标题的表视图。- 14. UILocalNotification:`UILocalNotification`实例表示，应用程序可以提交给它的用户在特定的日期和时间安排的通知。操作系统是负责在适当的时间提供通知;没有要运行发生的应用程序。虽然本地的通知类似，因为它们是用于显示警报，播放声音和徽章的应用程序图标，远程通知，它们是组成和本地传递，不需要与远程服务器连接。- 15. UIMenuController:他单身`UIMenuController`实例介绍了菜单界面的剪切，复制，粘贴，选择，选择，和删除命令。- 16. UINavigationItem:`UINavigationItem`类封装了一个`UINavigationBar`对象的堆栈上推导航项目的信息。一个导航栏是用于控制导航层次的内容。一个`UINavigationItem`指定什么是导航栏上的显示，当它是顶部的项目，也代表它是如何时，后面的项目。- 17. UINib:`UINib`类的实例作为对象包装或容器，`InterfaceBuilder`的`nib`文件。一个`UINib`对象在内存中缓存的一个`nib`文件的内容，解档和实例。当您的应用程序需要实例化的`nib`文件的内容，它可以这样做，而不必从`nib`文件加载数据，从而提高性能。`UINib`对象可以自动释放该缓存笔尖数据免费为您的应用程序内存在低内存条件下，重新装载数据下一次你的应用程序实例笔尖。您的应用程序应该使用`UINib`对象时，它需要反复实例相同的笔尖数据。例如，如果您的表视图使用`nib`文件实例表视图细胞，缓存中`UINib`对象的笔尖可以提供显着的性能改善。- 18. UIPasteboard:`UIPasteboard`类允许应用程序共享应用程序内部或与其他应用程序使用的全系统或应用程序特定的黏贴板的数据。- 19. UIPopoverController:`UIPopoverController`类是用于管理在`popover`中内容的呈现。你使用`popovers`暂时地呈现目前信息暂时而且用一种不超过整个屏幕就像一个模态视图的方式。`popover`内容是你在一个特殊类型的窗口的现有内容上分层。`popover`仍然可见，直到用户【点击】以外的`popover`窗口或你明确驳回。`Popover`控制器为`iPad`等设备的提供使用仅限。尝试在其他设备结果出现异常时进行新的一个创建。- 20. UIPrintFormatter:`UIPrintFormatter`时打印格式化的抽象基类：展示了传统的可打印的内容对象可以跨页边界。由于打印格式化，打印系统，可以自动打印与打印格式化的内容相关联的类型。以下为`UIPrintFormatter`的子类：- 20.1 UISimpleTextPrintFormatter:`UISimpleTextPrintFormatter`类的实例进行布局打印纯文本，可能是多个页面。类允许您指定的印刷文字全球的字体，颜色，对齐线，和换行符的模式属性。- 20.2 UIMarkupTextPrintFormatter:`UIMarkupTextPrintFormatter`类的实例展示了一个多页打印作业的HTML标记文本。- 20.3 UIViewPrintFormatter:`UIViewPrintFormatter`类的一个实例，勾画出用于打印的视图绘制的内容。视图的内容可以跨越多个页面。- 21. UIPrintInfo:一个`UIPrintInfo`对象封装了有关打印作业的信息，包括打印机标识，作业名称，输出类型（照片，正常，灰阶），方向（纵向或横向），和任何选定的双工模式，。打印系统打印时，使用此信息。- 22. UIPrintInteractionController:`UIPrintInteractionController`类的共享实例，提出了印刷的用户界面和管理的文件，图像和其他可打印的内容在`iOS`打印。`UIPrintInteractionController`是`iOS`中央印刷类。它的共享实例代表一个打印作业。打印作业，包括其印刷相关的信息和选项，如输出类型，作业名称，纸张大小和方向，要打印的内容。- 23. UIPrintPageRenderer:一个`UIPrintPageRenderer`对象绘制要打印页的内容带或不带打印格式化.A页面的渲染就是`UIPrintPageRenderer`自定义子类的一个实例。当您撰写打印作业使用的`UIPrintInteractionController`共享实例，您指定的页面渲染到该实例的`printPageRenderer`属性。- 24. UIPrintPaper:`UIPrintPaper`类的一个实例封装使用的纸张尺寸，打印作业，并在其中的内容可以打印的矩形。- 25. UIResponder:`UIResponder`类定义了一个接口，应对和处理事件的对象。它是`UIApplication`，`UIView`和它的子类（包括`UIWindow`中）的超类。这些类的实例有时被称为作为响应者对象或简单地说，应答。- 25.1 UIApplication:`UiApplication`类，提供了一个在`iOS`上运行的应用程序的控制和协调集中点。每个应用程序必须具有完全的`UIApplication`的一个实例（或子类`UIApplication`）。当一个应用程序启动时，`UIApplicationMain`函数被调用，在其他任务中，这个函数创建了一个单身的`UIApplication`对象。此后，您可以通过这个对象调用`sharedApplication`类方法。- 25.2 UIViewController:`UIViewController`类提供`iPhone`应用程序的基本观点的管理模式。基本视图控制器类支持相关联的视图的演示，为管理模式视图提供支持，并支持在响应设备方向变化旋转的意见。象`UINavigationController`和`UITabBarController`的这样的子类，如提供管理复杂的层次结构视图控制器和视图的其他行为。- 25.2.1 UITabBarController:`UITabBarController`类实现了一个专门的视图控制器管理一个广播式的选择界面。不适用于这个类的子类。相反，您可以使用它的实例去呈现允许用户在不同的操作模式之间进行选择的接口。这个标签栏界面显示在窗口底部的制表符之间的不同模式从而用来显示模式的意见选择。- 25.2.2 UITableViewController:`UITableViewController`类创建一个控制器对象，管理表视图。- 25.2.3 UINavigationController:`UINavigationController`的类实现了一个专门的视图控制器管理层次的内容导航。不适用于这个类的子类。相反，您可以使用实例当你希望你的应用程序的用户界面能够反映您的内容的层次性。此导航界面使你更有效的展现数据而且更容易让用户浏览内容。- 25.2.3-1 UIImagePickerController:`UIImagePickerController`类管理定制的和系统提供的用户界面，支持的设备上的图片和电影，并选择把使用的图片和电影保存在您的应用程序中。的图像选取器控制器管理用户交互和委托对象提供这些相互作用的结果。- 25.2.3-1 UIVideoEditorController:一个`UIVideoEditorController`对象，或视频编辑器，通过修剪从以前录制的电影的开始和结束的视频帧，以及重新编码降低质量来管理系统提供的用户界面。对象管理的用户交互，并提供您的委托对象编辑的电影文件系统路径。`UIVideoEditorController`类的特点是仅适用于设备，支持视频录制。- 25.3 UIView:`UIView`类通过定义一个在屏幕和界面上的矩形区域来管理这块区域的内容。在运行时，视图对象处理其区域内的任何内容渲染，还处理与该内容的任何相互作用。`UIView`类本身提供了基本行为的背景颜色填充矩形区域。更加复杂的内容，可以通过继承`UIView`和实施必要的绘图和自身事件处理代码来展现。`UIKit`框架还包括一个准的子类可以使用的范围从简单的按钮到复杂的表集。例如，一个`UILabel`对象绘制一个文本字符串和一个`UIImageView`对象绘制一个图像。- 25.3-1 UIWindow:`UIWindow`类定义，管理和协调的`Windows`应用程序显示在屏幕上的对象（如`Windows`）。一个窗口的两个主要职能是，为显示其意见面积和分发活动的意见。窗口是在视图层次的根。一个窗口属于一个级别；一个级别的窗口出现另一个层面以上。例如，警报出现高于正常的窗口。通常情况下，只有一个在`iOS`应用程序的窗口。- 25.3-2 UILabel:`UILabel`类实现一个只读的文本视图。您可以使用这个类的静态文本，如你可能会使用它来识别你的用户界面的其他部分，绘制一个或多个行。基本`UILabel`类提供控制文本的外观，包括它是否使用了一层阴影，或汲取的一大亮点。如果需要，您可以自定义文本的外观进一步通过继承。- 25.3-3 UIPickerView:`UIPickerView`类实现对象，所谓的选择器的看法，即使用一个纺车或老虎机的比喻来显示一个或多个值集。用户可以选择旋转的车轮，使所需的行的值与选择的值之间的对应关系值。`UIDatePicker`类使用一个自定义子类的`UIPickerView`显示日期和时间。为了看一个例子，挖掘在时钟应用程序报警窗格添加(“+”)按钮。- 25.3-4 UIProgressView:当一个邮件应用程序下载消息时它的进度条会在应用程序的底部显示。`UIProgressView`类提供了管理风格的进度条，用于获取和设置值是固定的任务的进度的属性。- 25.3-5 UIActivityIndicatorView:`UIActivityIndicatorView`类创建和管理一个指标用来显示不确定的任务进度。从外观上看，这个指是一个“齿轮”是动画旋转。- 25.3-6 UIImageView:图像的视图对象提供了一个用于显示单个图像或者一系列动画图像的基于视图的容器。对于动画图像，`UIImageView`的类提供设置动画的持续时间和频率的控制。您还可以启动和停止动画自由。- 25.3-7 UITabBar:`UITabBar`类实现了一个选择两个或多个按钮的控制，并称之为项目。一个标签栏最常见的用途是实现一个模态接口攻一个项目的变化选择。如果你想暂时突出或不会改变某个项目的外观时，轻按按钮，使用一个`UIToolbar`对象。`UITabBar`类提供为用户定制能力重新排列标签栏，删除和酒吧中添加项目。您可以使用标签栏的委托，以增加这种行为。- 25.3-8 UIToolBar:一个工具栏是显示一个或多个按钮被称之为工具栏项一种控制。当被轻按时工具栏瞬间突出或不改变某个项目的外观。- 25.3-9 UINavigationBar:`UINavigationBar`类实现了一个层次的内容导航控制。它的一个条形栏，通常显示在屏幕上方，包含用于导航的向上和向下一个层次的按钮。主要属性是左（后退）按钮，中心的名称和一个可选的右边的按钮。您可以指定这些自定义视图。- 25.3-10 UITableViewCell:`UITableViewCell`类定义的细胞出现在`UITableView`的对象的属性和行为中。- 25.3-11 UIActionSheet:使用`UIActionSheet`类可以把一套如何继续给定任务的替代品给用户。您还可以使用行动表，以提示用户确认是否有潜在危险的行动。该行动表包含一个可选的标题和一个或多个按钮，其中每个对应采取的行动。- 25.3-12 UIAlterView:使用`UIAlertView`类向用户显示一条警告消息。警报视图的功能相似，但在外观上不同于一个动作表（`UIActionSheet`的一个实例）。- 25.3-13 UIScrollView:`UIScrollView`的类提供支持显示的内容是大于应用程序的窗口大小。它可以使用户内滚动的内容，通过刷卡手势，和捏的手势放大和部分内容。- 25.3-13.1 UITextView:`UITextView`的类实现一个滚动的，多行文本区域的行为。类支持使用自定义字体，颜色，和对齐的文本的显示，同时还支持文本编辑。通常可以使用一个文本视图，显示多行文本，如显示一个大的文本文件的身体时，。- 25.3-13.2 UITableView:`UITableView`（或简单地说，表视图）的一个实例是用于显示和编辑分层列出的信息的一种手段。- 25.3-13.3 UICollectionView:`UICollectionView`（综合视图）是一种新的数据展示方式，简单来说可以把他理解成多列的`UITableView`，可以做九宫格布局的一种`view`。- 25.3-14 UISearchBar:`UISearchBar`类实现一个基于文本的搜索的文本字段控制。该控件提供了一个搜索按钮，输入文字的文本字段书签“按钮，一个取消按钮。`UISearchBar`对象实际上并不执行任何搜索。您可以使用委托，符合`UISearchBarDelegate`协议的对象，实施行动时，输入文本和按钮被点击。- 25.3-15 UIWebView:您使用了`UIWebView`类，在您的应用程序中嵌入网页内容。要做到这一点，你只需创建一个`UIWebView`对象，将它附加到一个窗口，它发送一个请求加载网页内容。您也可以使用这个类在网页的历史向前和向后移动，你甚至可以设置一些网页内容属性的编程。- 25.3-16 UIControl:`UIControl`是为控制对象，如用户的意图传达到应用程序的按钮和滑块的基类。你不能直接使用`UIControl`类实例化控制。相反，它定义了共同的界面和它的所有子类的行为结构。- 25.3-16.1 UIButton:`UIButton`的类的一个实例，实现了在触摸屏上的按钮。触摸一个按钮拦截事件和动作消息发送到目标对象时。设定的目地和行动方法都继承自`UIControl`。这个类提供了方法来设置标题，图像，按钮等外观属性。通过这些访问，您可以为每个按钮状态指定一个不同的外观。- 25.3-16.2 UIDatePicker:`UIDatePicker`类实现了一个对象，它使用多个旋转的车轮，以允许用户选择日期和时间。`iPhone`的例子是一个日期选择器，定时器和闹钟设置闹钟的时钟应用程序中的窗格。您也可以使用日期选择器作为一个倒数计时器。- 25.3-16.3 UIPageControl:您可以使用`UIPageControl`类来创建和管理页面控制。一个页面控制是在控制中心的点继承。每个点对应一个应用程序的文档（或其他实体的数据模型）页面，白色圆点表示当前浏览的网页。- 25.3-16.4 UISegmentedControl:一个`UISegmentedControl`对象是横向控制多个段，每个段作为一个独立的按钮运作。分段控制提供一个紧凑的手段，组合到一起的控制。- 25.3-16.5 UITextField:`UITextField`对象是一个显示可编辑的文字和动作消息发送到目标对象，当用户按下返回按钮控制。通常可以使用这个类从用户收集少量的文字，并执行一些立即采取行动，如搜索操作，根据该文本。- 25.3-16.6 UISlider:一个`UISlider`对象是一个可视化的控制，用来选择一个连续值范围从单一的值。滑块始终显示为单杠。拇指，注意到滑块的当前值，并更改设置的用户可以通过移动。- 25.3-16.7 UISwitch:您可以使用`UISwitch`类来创建和管理```ON/ OFF```按钮，您会看到，例如在飞行模式等服务的偏好设定（设定）。这些对象被称为开关。- 26. UIScreen:一个`UIScreen`对象包含设备的整个屏幕的边界矩形。当设置应用程序的用户界面，你应该使用这个对象的属性，能为您的应用程序的窗口的建议的框架矩形。- 27. UIScreenMode:一个`UIScreenMode`对象表示的，可以应用到一个`UIScreen`对象的属性可能。该对象封装了有关屏幕的底层显示缓冲区的大小和使用单个像素的宽高比信息。- 28. UISearchDisplayController:搜索显示控制器管理一个搜索栏，显示表视图显示另一种观点认为控制器管理的数据搜索的结果。- 29. UITextChecker:您使用的`UITextChecker`类的实例来检查拼写错误的单词字符串（通常是文档中的文本）。- 30. UITextPosition:一个`UITextPosition`对象代表一个文本容器中的位置，换句话说，它是一个到在文本显示的支持字符串的索引。- 31. UITextRange:一个`UITextRange`对象代表一个字符在文本容器的范围，换句话说，它确定一个起始索引和结束索引一个字符串，支持一个文本输入对象。- 32. UITouch:一个`UITouch`对象表示存在一个特定事件的屏幕上用手指或运动。你可以通过`UIEvent`对象传递到事件处理响应对象`UITouch`对象。[上面内容参考 http://www.jianshu.com/p/ae3ab9fdc23c](http://www.jianshu.com/p/ae3ab9fdc23c)总之，学习一门新的框架类库的时侯，需要追根溯源，才好提纲切领，即能掌握全局，又能深入细节。从系统根类出发，结合事件响应处理的机制原理，再深入到各个界面组件或自定义，就会胸有成竹而不乱阵脚了。说明：原文章名称 “iOS UIKit框架开源”，今日改为 “iOS UIKit框架注解” 。文章注解编写周期也许会很长一些，心有余而力有限，以后遇到用到会有空闲时间及时补充吧，也欢迎你的加入 & Fork，这样你也学习了系统的API。#### UIKit框架注解演示![](http://upload-images.jianshu.io/upload_images/2230763-9dbd82586460362c.gif?imageMogr2/auto-orient/strip) UIKit框架注解演示.gif#### GitHub工程地址- [UIKit 框架，归类整理，各分类文件注解，并对每一类中常用的功能进行案例渐进式解析，从零开始，由浅入深；以简化初学者入门和老司机回顾的繁索过程，尽快切入主题，快速使用起来。这里只等你的到来 ！Star & Fork](https://github.com/CustomPBWaters/UIKit-Framework-OpenSource)- [官方译文框架源码注解，当你「了解权威 & 进阶原理」的时候，网搜的众多中 ~ ~（自行脑补）。做只有思想的伐码猿，做到「实用概念，不文言」&「实现原理，简化详解」，从零开始，由浅入深。 以简化初学者入门和老司机回顾的繁索过程，尽快切入主题，快速使用起来。这里只等你的到来 ！Star & Fork](https://github.com/CustomPBWaters/OfficialTranslation-SourceAnnotation)#### 期待- 如果在阅读过程中遇到 error，希望你能 Issues 我，谢谢。- 如果你想为【UIKit 框架注解】分享点什么，也希望你能 Issues 我，我非常想为这篇文章增加更多实用的内容，谢谢。- [「博客原文」](https://custompbwaters.github.io/2015/06/01/%E6%BA%90%E7%A0%81%E6%B3%A8%E8%A7%A3+%E6%B4%BB%E7%94%A8/iOS%20UIKit%E6%A1%86%E6%9E%B6/)对本专题我会【不定时、持续更新、一些 学习心得与文章、实用才是硬道理】^_^.- [你关注的人，决定了你看到的世界！「@ ：️Plain boiled water」^_^.](https://github.com/CustomPBWaters/UIKit-Framework-OpenSource)#### 后续 & About me【我也是对所花费时间的一个总结】About me：[@「伐码猿」](http://www.jianshu.com/u/fd745d76c816)、[@「白开水ln」](http://upload-images.jianshu.io/upload_images/2230763-f8d3826ef73b7e4b.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)我只是个【有思想的伐码猿】加上【自己的学习总结️】写出来的文章。![](http://upload-images.jianshu.io/upload_images/2230763-b1ec9ba2cfa2e2bb.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)↑ @伐码猿 --> 分享和开源精神 ↑
#### 导语
> 
我们追求技术的提升，关注技术的发展历程；作为从事技术工作的伐码猿，有个想法（`仅是个人想法`），这个想法工程量之大（`遇到，用到，最新技术点，点点积累，及时补充吧`），从事技术多年也算是对所花费时间的一个总结（`算是对技术这块的一个目标吧`）。
这篇文章的内容仅是【UIKit框架 各分类文件的一个简明介绍】，重点 -> 重点 -> 重点（
```
重要的事情说
 2+1 遍
```
），
我的「想法和行动」都在文章末尾
![](http://upload-images.jianshu.io/upload_images/2230763-0fd6bbb32d5a58aa.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
「UIKit Framework Annotation  Practical Case」
我们通常称为 “Cocoa框架”，事实上 Cocoa 本身是一个框架的集合，它包含了众多子框架，其中最重要的要数 “Foundation” 和 “UIKit” 。前者是框架的基础，和界面无关，其中包含了大量常用的API；后者是基础的UI类库，以后我们在 iOS
 开发中会经常用到。这两个框架在系统架构中的位置如下图：
![](http://upload-images.jianshu.io/upload_images/2230763-75296d35dc3f2f7c.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
 Cocoa框架
上图注解：
- 
1、Core OS 层
位于 `iOS` 框架的最底层，主要包含内核、文件系统、网络基础架构、安全管理、电源管理、设备驱动、线程管理、内存管理等。简而言之，该层提供了最低级的、系统级的服务。
- 
2、Core Services 层
可称之为核心服务层，顾名思义，它提供诸如字符串管理、集合管理、网络操作、`URL`实用工具、联系人管理、偏好设置等服务。除此之外，它还提供很多基于硬件特性的服务，如`GPS`、加速仪、陀螺仪等。该层包含了
```
Core
 Location
```
、`Core Motion`、`SystemConfiguration`、`Foundation`与
```
Core
 Foundation
```
子模块。其中`Foundation`与
```
Core
 Foundation
```
子模块提供了对公共数据类型(字符串、集合等)的抽象，`Core Foundation`中的
```
Core
 Data
```
子模块可以实现对象图形管理与对象持久化。
- 3、Media 层：依赖于Core 
`Services`层提供的功能，主要负责图形与多媒体服务。它包含了`CoreGraphics`、
```
Core
 Text
```
、`OpenGL ES`、
```
Core
 Animation
```
、`AVFoundation`、
```
Core
 Audio
```
等与图形、视频和音频相关的功能模块。
- 4、 Cocoa Touch 层 
是直接向`iOS`应用程序提供各种基础功能的支持。其中的`UIKit`框架提供各种可视化控件供应用程序使用，如窗口、视图、视图控制器与各种用户控件等。另外`UIKit`也定义了应用程序的默认行为和事件处理结构。
UIKit 框架 是 Objective-C 语言应用于苹果移动设备的界面呈现、管理功能库，功能之强大，本专题 致力于将苹果官网的海量的文档进行 归类整理，框架各分类文件注解，并对每一类中常用的功能进行案例渐进式解析，以 简化初学者入门和老手回顾的繁索过程，尽快切入主题，快速应用起来。
在「时间 & 知识 」有限内，总结的文章难免有「未全、不足 」的地方，还望各位好友指出，以提高文章质量。
这里我们不妨也看一下 UIKit 类结构：
![](http://upload-images.jianshu.io/upload_images/2230763-0a6913fa199b11c5.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
 UIKit 框架
- 1. UIAcceleration:
被叫做加速事件的一个`UIAcceleration`类的实例是用来代表即时的三维加速数据。为了接收重力加速度，要注册一个应用应用程序作为一个共享`UIAccelerater`对象的委托对象（参考`UIAcceleromete`类）。
- 2. UIAccelerater:
`UIAccelerater`类可以让你的寄存器接收到从板载硬件上得到的加速相关数据。当设备移动时，它的硬件能够报告沿主线在三维空间中的线性加速度变化。你可以利用这些数据来检测器件的电流方向和该方向的瞬时变化。你可以使用这瞬时变化输入到一个游戏或者启动应用程序中的一些行动。
- 3. UIaccessibilityElement:
`UIAccessibilityElement`类封装的项目信息对一些特殊的人可见，默认情况下是不可访问的。例如，一个图标或文字图像不会自动访问，因为它没有继承的`UIView`（或`UIControl`）。一个包含等非视图的项目，创建一个实例来表示每个项目需要访问的`UIAccessibilityElement`。
- 
4. UIBarItem:
`UIBarItem`是一个抽象的超类用来在屏幕的底部给项目加上一栏。带栏的条目类似于一个按钮的方式。他们有标题，有图片，动作和目标。你可以在项目中启用和禁用一个带栏条目。
- 4.1 UIBarButtonItem:
一个栏按钮项目是放置在一个`UIToolbar`或`UINavigation`对象专门的按钮。它从它的抽象父类，`UIBarItem`继承的基本按钮的行为。`UIBarButtonItem`定义额外的初始化方法和属性，使用工具栏和导航栏。
- 4.2 UITabBarItem:
`UITabBarItem`类实现了一个带标签栏的项目，`UITabBar`类的实例。一个标签栏严格地以收音机模式来运作，其中一个项目是在选定的时间，点击标签栏项目切换上面的标签栏。您还可以指定在标签栏上项目的信息，例如加入额外的视觉徽章值，电话应用程序上使用该项目的徽章，以显示新邮件的数量。这个类还提供了一些系统默认创建的项目。
- 5. UIBezierPath:
`UIBezierPath`类，您可以定义一个路径由直线和曲线的线段组成，并呈现在您的自定义视图路径。您可以使用这个类，最初只是为您的路径中指定的几何形状。路径可以定义简单的形状，如矩形，椭圆形和圆弧，或他们可以定义复杂的多边形，采用直线和曲线线段的混合物。定义的形状后，您可以使用这个类的其他方法来呈现当前图形上下文的路径。
- 6. UIColor:
一个`UIColor`对象代表的颜色，有时不透明（`alpha`值）。您可以使用`UIColor`对象来存储颜色数据，并在绘画过程中，你可以用它们来设置当前填充和笔触颜色。
- 7. UIDecive:
`UIDevice`类提供了一个单个实例代表当前设备。从这个实例中，可以获取有关设备的唯一的`ID`，分配名称，设备型号，和操作系统名称和版本等信息。您也可以使用的`UIDevice`实例，检测设备的特点，如物理方向的变化。使用方向属性得到当前的方向，或注册`UIDeviceOrientationDidChangeNotification`通知收到更改通知。在使用这些技术来获得定位数据之前，你必须使数据传递，使用的`beginGeneratingDeviceOrientationNotifications`方法。当您不再需要跟踪设备的方向，调用`endGeneratingDeviceOrientationNotifications`方法来禁用交付的通知..
- 8. UIDocumentInteractionController:
一个文件交互控制器一起委托对象，提供应用程序管理与本地系统中的文件的用户交互的支持。例如，一个电子邮件程序可能使用这个类，允许用户预览附件和其他应用程序中打开它们。使用这个类，目前预览相应的用户界面，打开，复制或打印指定的文件。
- 9. UIEvent:
一个`UIEvent`对象（或者简单地说，一个事件对象）代表在`iOS`的事件。一般有三种类型的事件：触摸事件，运动事件和远程控制的事件，。远程控制的事件使一个`Responder`对象来接收来自外部的附件或耳机的命令，以便它可以管理管理音频和视频，例如，播放视频或跳过到下一音轨。
- 10. UIFont:
`UIFont`类提供了用于获取和设置字体信息的接口。类提供了访问字体的特点，还提供了访问，这是在布局过程中中使用的字体的字形信息系统。他们通过接受它们作为参数的方法，您可以使用字体对象。
- 
11. UIGestureRecognizer:
`UIGestureRecognizer`是一个具体的手势识别类的抽象基类。一个手势识别对象（或简单地说，一个手势识别）中分离出来承认这种认识上的姿态和行动的逻辑。当这些对象之一承认一个共同的姿态，或者，在某些情况下，改变的姿态，它发送一个动作消息，每一个指定的目物镜。以下胃具体的子类：
- 
11.1 UITapGestureRecognizer:
点击手势。指定数量的手指必须要承认的姿态，挖掘查看指定的次数。
- 
11.2 UIPinchGestureRecognizer:
看起来捏的手势，涉及两个接触。当用户将两个手指，向对方的传统意义是缩小;当用户将两个手指从彼此远离，传统意义变焦。
- 
11.3 UIRotationGestureRecognizer:
看起来轮换涉及两个触摸手势。当用户移动手指对面对方的圆周运动，基本的观点应该在相应的方向和速度旋转。
- 
11.4 UISwipeGestureRecognizer:
看起来刷卡在一个或多个方向的手势。抨击是一个独立的姿态，因此，相关的操作的消息发送每个手势只有一次。
- 
11.5 UIPanGestureRecognizer:
看起来平移（拖动）的手势。用户必须按查看上一个或更多的手指，而他们平移。实施这个手势识别动作方法的客户端可以要求它目前的翻译和手势的速度。
- 
11.6 UILongPressGestureRecognizer:
看起来长按手势。用户必须按下一个或更多的手指行动讯息传送至少指定期限。此外，手指可能要承认的姿态移动唯一指定的距离;如果他们超越这个限制的姿态失败。
- 
12. UIImage:
一个`UIImage`对象是一个高层次的的方式来显示图像数据。您可以从文件中创建的图像，从石英图像中的物体，或从您收到的原始图像数据。`UIImage`的类还提供图像绘制到当前图形的上下文中，使用不同的混合模式和不透明度值的几种选择。
- 13. UILocalizedIndexedCollation:
`UILocalizedIndexedCollation`类方便为有部分指数的图表进行组织，整理，以及数据本地化。表视图的数据源，然后使用排序对象提供的输入节的标题和节索引标题的表视图。
- 14. UILocalNotification:
`UILocalNotification`实例表示，应用程序可以提交给它的用户在特定的日期和时间安排的通知。操作系统是负责在适当的时间提供通知;没有要运行发生的应用程序。虽然本地的通知类似，因为它们是用于显示警报，播放声音和徽章的应用程序图标，远程通知，它们是组成和本地传递，不需要与远程服务器连接。
- 15. UIMenuController:
他单身`UIMenuController`实例介绍了菜单界面的剪切，复制，粘贴，选择，选择，和删除命令。
- 16. UINavigationItem:
`UINavigationItem`类封装了一个`UINavigationBar`对象的堆栈上推导航项目的信息。一个导航栏是用于控制导航层次的内容。一个`UINavigationItem`指定什么是导航栏上的显示，当它是顶部的项目，也代表它是如何时，后面的项目。
- 17. UINib:
`UINib`类的实例作为对象包装或容器，`InterfaceBuilder`的`nib`文件。一个`UINib`对象在内存中缓存的一个`nib`文件的内容，解档和实例。当您的应用程序需要实例化的`nib`文件的内容，它可以这样做，而不必从`nib`文件加载数据，从而提高性能。`UINib`对象可以自动释放该缓存笔尖数据免费为您的应用程序内存在低内存条件下，重新装载数据下一次你的应用程序实例笔尖。您的应用程序应该使用`UINib`对象时，它需要反复实例相同的笔尖数据。例如，如果您的表视图使用`nib`文件实例表视图细胞，缓存中`UINib`对象的笔尖可以提供显着的性能改善。
- 18. UIPasteboard:
`UIPasteboard`类允许应用程序共享应用程序内部或与其他应用程序使用的全系统或应用程序特定的黏贴板的数据。
- 19. UIPopoverController:
`UIPopoverController`类是用于管理在`popover`中内容的呈现。你使用`popovers`暂时地呈现目前信息暂时而且用一种不超过整个屏幕就像一个模态视图的方式。`popover`内容是你在一个特殊类型的窗口的现有内容上分层。`popover`仍然可见，直到用户【点击】以外的`popover`窗口或你明确驳回。`Popover`控制器为`iPad`等设备的提供使用仅限。尝试在其他设备结果出现异常时进行新的一个创建。
- 
20. UIPrintFormatter:
`UIPrintFormatter`时打印格式化的抽象基类：展示了传统的可打印的内容对象可以跨页边界。由于打印格式化，打印系统，可以自动打印与打印格式化的内容相关联的类型。以下为`UIPrintFormatter`的子类：
- 
20.1 UISimpleTextPrintFormatter:
`UISimpleTextPrintFormatter`类的实例进行布局打印纯文本，可能是多个页面。类允许您指定的印刷文字全球的字体，颜色，对齐线，和换行符的模式属性。
- 
20.2 UIMarkupTextPrintFormatter:
`UIMarkupTextPrintFormatter`类的实例展示了一个多页打印作业的HTML标记文本。
- 
20.3 UIViewPrintFormatter:
`UIViewPrintFormatter`类的一个实例，勾画出用于打印的视图绘制的内容。视图的内容可以跨越多个页面。
- 21. UIPrintInfo:
一个`UIPrintInfo`对象封装了有关打印作业的信息，包括打印机标识，作业名称，输出类型（照片，正常，灰阶），方向（纵向或横向），和任何选定的双工模式，。打印系统打印时，使用此信息。
- 22. UIPrintInteractionController:
`UIPrintInteractionController`类的共享实例，提出了印刷的用户界面和管理的文件，图像和其他可打印的内容在`iOS`打印。`UIPrintInteractionController`是`iOS`中央印刷类。它的共享实例代表一个打印作业。打印作业，包括其印刷相关的信息和选项，如输出类型，作业名称，纸张大小和方向，要打印的内容。
- 23. UIPrintPageRenderer:
一个`UIPrintPageRenderer`对象绘制要打印页的内容带或不带打印格式化.A页面的渲染就是`UIPrintPageRenderer`自定义子类的一个实例。当您撰写打印作业使用的`UIPrintInteractionController`共享实例，您指定的页面渲染到该实例的`printPageRenderer`属性。
- 24. UIPrintPaper:
`UIPrintPaper`类的一个实例封装使用的纸张尺寸，打印作业，并在其中的内容可以打印的矩形。
- 
25. UIResponder:
`UIResponder`类定义了一个接口，应对和处理事件的对象。它是`UIApplication`，`UIView`和它的子类（包括`UIWindow`中）的超类。这些类的实例有时被称为作为响应者对象或简单地说，应答。
- 
25.1 UIApplication:
`UiApplication`类，提供了一个在`iOS`上运行的应用程序的控制和协调集中点。每个应用程序必须具有完全的`UIApplication`的一个实例（或子类`UIApplication`）。当一个应用程序启动时，`UIApplicationMain`函数被调用，在其他任务中，这个函数创建了一个单身的`UIApplication`对象。此后，您可以通过这个对象调用`sharedApplication`类方法。
- 
25.2 UIViewController:
`UIViewController`类提供`iPhone`应用程序的基本观点的管理模式。基本视图控制器类支持相关联的视图的演示，为管理模式视图提供支持，并支持在响应设备方向变化旋转的意见。象`UINavigationController`和`UITabBarController`的这样的子类，如提供管理复杂的层次结构视图控制器和视图的其他行为。
- 
25.2.1 UITabBarController:
`UITabBarController`类实现了一个专门的视图控制器管理一个广播式的选择界面。不适用于这个类的子类。相反，您可以使用它的实例去呈现允许用户在不同的操作模式之间进行选择的接口。这个标签栏界面显示在窗口底部的制表符之间的不同模式从而用来显示模式的意见选择。
- 
25.2.2 UITableViewController:
`UITableViewController`类创建一个控制器对象，管理表视图。
- 
25.2.3 UINavigationController:
`UINavigationController`的类实现了一个专门的视图控制器管理层次的内容导航。不适用于这个类的子类。相反，您可以使用实例当你希望你的应用程序的用户界面能够反映您的内容的层次性。此导航界面使你更有效的展现数据而且更容易让用户浏览内容。
- 
25.2.3-1 UIImagePickerController:
`UIImagePickerController`类管理定制的和系统提供的用户界面，支持的设备上的图片和电影，并选择把使用的图片和电影保存在您的应用程序中。的图像选取器控制器管理用户交互和委托对象提供这些相互作用的结果。
- 
25.2.3-1 UIVideoEditorController:
一个`UIVideoEditorController`对象，或视频编辑器，通过修剪从以前录制的电影的开始和结束的视频帧，以及重新编码降低质量来管理系统提供的用户界面。对象管理的用户交互，并提供您的委托对象编辑的电影文件系统路径。`UIVideoEditorController`类的特点是仅适用于设备，支持视频录制。
- 
25.3 UIView:
`UIView`类通过定义一个在屏幕和界面上的矩形区域来管理这块区域的内容。在运行时，视图对象处理其区域内的任何内容渲染，还处理与该内容的任何相互作用。`UIView`类本身提供了基本行为的背景颜色填充矩形区域。更加复杂的内容，可以通过继承`UIView`和实施必要的绘图和自身事件处理代码来展现。`UIKit`框架还包括一个准的子类可以使用的范围从简单的按钮到复杂的表集。例如，一个`UILabel`对象绘制一个文本字符串和一个`UIImageView`对象绘制一个图像。
- 
25.3-1 UIWindow:
`UIWindow`类定义，管理和协调的`Windows`应用程序显示在屏幕上的对象（如`Windows`）。一个窗口的两个主要职能是，为显示其意见面积和分发活动的意见。窗口是在视图层次的根。一个窗口属于一个级别；一个级别的窗口出现另一个层面以上。例如，警报出现高于正常的窗口。通常情况下，只有一个在`iOS`应用程序的窗口。
- 
25.3-2 UILabel:
`UILabel`类实现一个只读的文本视图。您可以使用这个类的静态文本，如你可能会使用它来识别你的用户界面的其他部分，绘制一个或多个行。基本`UILabel`类提供控制文本的外观，包括它是否使用了一层阴影，或汲取的一大亮点。如果需要，您可以自定义文本的外观进一步通过继承。
- 
25.3-3 UIPickerView:
`UIPickerView`类实现对象，所谓的选择器的看法，即使用一个纺车或老虎机的比喻来显示一个或多个值集。用户可以选择旋转的车轮，使所需的行的值与选择的值之间的对应关系值。`UIDatePicker`类使用一个自定义子类的`UIPickerView`显示日期和时间。为了看一个例子，挖掘在时钟应用程序报警窗格添加(“+”)按钮。
- 
25.3-4 UIProgressView:
当一个邮件应用程序下载消息时它的进度条会在应用程序的底部显示。`UIProgressView`类提供了管理风格的进度条，用于获取和设置值是固定的任务的进度的属性。
- 
25.3-5 UIActivityIndicatorView:
`UIActivityIndicatorView`类创建和管理一个指标用来显示不确定的任务进度。从外观上看，这个指是一个“齿轮”是动画旋转。
- 
25.3-6 UIImageView:
图像的视图对象提供了一个用于显示单个图像或者一系列动画图像的基于视图的容器。对于动画图像，`UIImageView`的类提供设置动画的持续时间和频率的控制。您还可以启动和停止动画自由。
- 
25.3-7 UITabBar:
`UITabBar`类实现了一个选择两个或多个按钮的控制，并称之为项目。一个标签栏最常见的用途是实现一个模态接口攻一个项目的变化选择。如果你想暂时突出或不会改变某个项目的外观时，轻按按钮，使用一个`UIToolbar`对象。`UITabBar`类提供为用户定制能力重新排列标签栏，删除和酒吧中添加项目。您可以使用标签栏的委托，以增加这种行为。
- 
25.3-8 UIToolBar:
一个工具栏是显示一个或多个按钮被称之为工具栏项一种控制。当被轻按时工具栏瞬间突出或不改变某个项目的外观。
- 
25.3-9 UINavigationBar:
`UINavigationBar`类实现了一个层次的内容导航控制。它的一个条形栏，通常显示在屏幕上方，包含用于导航的向上和向下一个层次的按钮。主要属性是左（后退）按钮，中心的名称和一个可选的右边的按钮。您可以指定这些自定义视图。
- 
25.3-10 UITableViewCell:
`UITableViewCell`类定义的细胞出现在`UITableView`的对象的属性和行为中。
- 
25.3-11 UIActionSheet:
使用`UIActionSheet`类可以把一套如何继续给定任务的替代品给用户。您还可以使用行动表，以提示用户确认是否有潜在危险的行动。该行动表包含一个可选的标题和一个或多个按钮，其中每个对应采取的行动。
- 
25.3-12 UIAlterView:
使用`UIAlertView`类向用户显示一条警告消息。警报视图的功能相似，但在外观上不同于一个动作表（`UIActionSheet`的一个实例）。
- 
25.3-13 UIScrollView:
`UIScrollView`的类提供支持显示的内容是大于应用程序的窗口大小。它可以使用户内滚动的内容，通过刷卡手势，和捏的手势放大和部分内容。
- 
25.3-13.1 UITextView:
`UITextView`的类实现一个滚动的，多行文本区域的行为。类支持使用自定义字体，颜色，和对齐的文本的显示，同时还支持文本编辑。通常可以使用一个文本视图，显示多行文本，如显示一个大的文本文件的身体时，。
- 
25.3-13.2 UITableView:
`UITableView`（或简单地说，表视图）的一个实例是用于显示和编辑分层列出的信息的一种手段。
- 
25.3-13.3 UICollectionView:
`UICollectionView`（综合视图）是一种新的数据展示方式，简单来说可以把他理解成多列的`UITableView`，可以做九宫格布局的一种`view`。
- 
25.3-14 UISearchBar:
`UISearchBar`类实现一个基于文本的搜索的文本字段控制。该控件提供了一个搜索按钮，输入文字的文本字段书签“按钮，一个取消按钮。`UISearchBar`对象实际上并不执行任何搜索。您可以使用委托，符合`UISearchBarDelegate`协议的对象，实施行动时，输入文本和按钮被点击。
- 
25.3-15 UIWebView:
您使用了`UIWebView`类，在您的应用程序中嵌入网页内容。要做到这一点，你只需创建一个`UIWebView`对象，将它附加到一个窗口，它发送一个请求加载网页内容。您也可以使用这个类在网页的历史向前和向后移动，你甚至可以设置一些网页内容属性的编程。
- 
25.3-16 UIControl:
`UIControl`是为控制对象，如用户的意图传达到应用程序的按钮和滑块的基类。你不能直接使用`UIControl`类实例化控制。相反，它定义了共同的界面和它的所有子类的行为结构。
- 
25.3-16.1 UIButton:
`UIButton`的类的一个实例，实现了在触摸屏上的按钮。触摸一个按钮拦截事件和动作消息发送到目标对象时。设定的目地和行动方法都继承自`UIControl`。这个类提供了方法来设置标题，图像，按钮等外观属性。通过这些访问，您可以为每个按钮状态指定一个不同的外观。
- 
25.3-16.2 UIDatePicker:
`UIDatePicker`类实现了一个对象，它使用多个旋转的车轮，以允许用户选择日期和时间。`iPhone`的例子是一个日期选择器，定时器和闹钟设置闹钟的时钟应用程序中的窗格。您也可以使用日期选择器作为一个倒数计时器。
- 
25.3-16.3 UIPageControl:
您可以使用`UIPageControl`类来创建和管理页面控制。一个页面控制是在控制中心的点继承。每个点对应一个应用程序的文档（或其他实体的数据模型）页面，白色圆点表示当前浏览的网页。
- 
25.3-16.4 UISegmentedControl:
一个`UISegmentedControl`对象是横向控制多个段，每个段作为一个独立的按钮运作。分段控制提供一个紧凑的手段，组合到一起的控制。
- 
25.3-16.5 UITextField:
`UITextField`对象是一个显示可编辑的文字和动作消息发送到目标对象，当用户按下返回按钮控制。通常可以使用这个类从用户收集少量的文字，并执行一些立即采取行动，如搜索操作，根据该文本。
- 
25.3-16.6 UISlider:
一个`UISlider`对象是一个可视化的控制，用来选择一个连续值范围从单一的值。滑块始终显示为单杠。拇指，注意到滑块的当前值，并更改设置的用户可以通过移动。
- 
25.3-16.7 UISwitch:
您可以使用`UISwitch`类来创建和管理
```
ON/
 OFF
```
按钮，您会看到，例如在飞行模式等服务的偏好设定（设定）。这些对象被称为开关。
- 
26. UIScreen:
一个`UIScreen`对象包含设备的整个屏幕的边界矩形。当设置应用程序的用户界面，你应该使用这个对象的属性，能为您的应用程序的窗口的建议的框架矩形。
- 27. UIScreenMode:
一个`UIScreenMode`对象表示的，可以应用到一个`UIScreen`对象的属性可能。该对象封装了有关屏幕的底层显示缓冲区的大小和使用单个像素的宽高比信息。
- 28. UISearchDisplayController:
搜索显示控制器管理一个搜索栏，显示表视图显示另一种观点认为控制器管理的数据搜索的结果。
- 29. UITextChecker:
您使用的`UITextChecker`类的实例来检查拼写错误的单词字符串（通常是文档中的文本）。
- 30. UITextPosition:
一个`UITextPosition`对象代表一个文本容器中的位置，换句话说，它是一个到在文本显示的支持字符串的索引。
- 31. UITextRange:
一个`UITextRange`对象代表一个字符在文本容器的范围，换句话说，它确定一个起始索引和结束索引一个字符串，支持一个文本输入对象。
- 32. UITouch:
一个`UITouch`对象表示存在一个特定事件的屏幕上用手指或运动。你可以通过`UIEvent`对象传递到事件处理响应对象`UITouch`对象。
[上面内容参考 http://www.jianshu.com/p/ae3ab9fdc23c](http://www.jianshu.com/p/ae3ab9fdc23c)
总之，学习一门新的框架类库的时侯，需要追根溯源，才好提纲切领，即能掌握全局，又能深入细节。从系统根类出发，结合事件响应处理的机制原理，再深入到各个界面组件或自定义，就会胸有成竹而不乱阵脚了。
说明：原文章名称 “iOS UIKit框架开源”，今日改为 “iOS UIKit框架注解” 。文章注解编写周期也许会很长一些，心有余而力有限，以后遇到用到会有空闲时间及时补充吧，也欢迎你的加入 & Fork，这样你也学习了系统的API。
#### UIKit框架注解演示
![](http://upload-images.jianshu.io/upload_images/2230763-9dbd82586460362c.gif?imageMogr2/auto-orient/strip)
 UIKit框架注解演示.gif
#### GitHub工程地址
- 
[UIKit 框架，归类整理，各分类文件注解，并对每一类中常用的功能进行案例渐进式解析，从零开始，由浅入深；以简化初学者入门和老司机回顾的繁索过程，尽快切入主题，快速使用起来。这里只等你的到来
 ！Star & Fork](https://github.com/CustomPBWaters/UIKit-Framework-OpenSource)
- 
[官方译文框架源码注解，当你「了解权威 & 进阶原理」的时候，网搜的众多中 ~ ~（自行脑补）。做只有思想的伐码猿，做到「实用概念，不文言」&「实现原理，简化详解」，从零开始，由浅入深。
 以简化初学者入门和老司机回顾的繁索过程，尽快切入主题，快速使用起来。这里只等你的到来 ！Star & Fork](https://github.com/CustomPBWaters/OfficialTranslation-SourceAnnotation)
#### 期待
- 
如果在阅读过程中遇到 error，希望你能 Issues 我，谢谢。
- 
如果你想为【UIKit 框架注解】分享点什么，也希望你能 Issues 我，我非常想为这篇文章增加更多实用的内容，谢谢。
- 
[「博客原文」](https://custompbwaters.github.io/2015/06/01/%E6%BA%90%E7%A0%81%E6%B3%A8%E8%A7%A3+%E6%B4%BB%E7%94%A8/iOS%20UIKit%E6%A1%86%E6%9E%B6/)对本专题我会【不定时、持续更新、一些
 学习心得与文章、实用才是硬道理】^_^.
- 
[你关注的人，决定了你看到的世界！「@ ：️Plain boiled water」^_^.](https://github.com/CustomPBWaters/UIKit-Framework-OpenSource)
#### 后续 & About me
【我也是对所花费时间的一个总结】
About me：[@「伐码猿」](http://www.jianshu.com/u/fd745d76c816)、[@「白开水ln」](http://upload-images.jianshu.io/upload_images/2230763-f8d3826ef73b7e4b.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
我只是个【有思想的伐码猿】加上【自己的学习总结️】写出来的文章。
![](http://upload-images.jianshu.io/upload_images/2230763-b1ec9ba2cfa2e2bb.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
↑ @伐码猿 --> 分享和开源精神 ↑
