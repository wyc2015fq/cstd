# iOS 基础知识点2 - weixin_33985507的博客 - CSDN博客
2019年01月08日 18:01:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：7
简单说一下App的启动过程，从main文件开始说起
```
程序启动分为两类：1.有stroyboard 2.没有storyboard
有storyboard情况下：
1.mian函数
2.UIApplicationMain
* 创建UIApplication对象
* 创建UIApplication的delegate对象
3.根据Info.plist获得Main.storyboard的文件名，加载Main.storyboard(有sttoryboard)
* 创建UIWindow
* 创建和设置UIWindow的rootViewController
* 显示窗口
没有storyboard情况下：
1.main函数
2.UIApplicationMain
* 创建UIApplication对象
* 创建UIApplication的delegate对象
3.delegate对象开始处理（监听）系统事件（没有storyboard）
* 程序启动完毕的时候，就会调用代理的application:didFinishLauchingWithOptions:方法
* 在application:didFinishLauchingWIthOptions:中创建UIWindow
* 创建和设置UIWindow的rootViewController
* 显示窗口
```
UIButton与UITableView的层级结构
```
- 继承结构，属于内部的子控件结构
- UIButton为：UIButton>UIControl>UIView>UIResponder>NSObject
- UITableView为 ：UITableView>UIScrollView>UIView>UIResponder>NSObject
```
设置scrollView的contentsize能在viewDidLoad里设置么，为什么
```
- 一般情况下可以设置在viewDidLoad 中，但在autoLayout下，系统会在viewDidAppear之前根据根据subview的constraint重新计算scrollview的contentsize。这就是为什么，在viewDidLoad里面设置了contentsize没用。因为在后面，会在重新计算一次，前面手动设置的值会被覆盖掉。
- 解决办法就是：
   -去除autoLayout选项，自己手动设置contentsize
   - 如果要使用autoLayout,要么自己设置完subview的constraint,然后让系统自动根据constraint计算出contentsize.要么就在viewDiaAppear里面自己手动设置contentsize.
```
简述你对UIView、UIWindow和CALayer的理解
```
- UIView:属于UIKit.framework框架，负责渲染矩形区域的内容，为矩形区域添加动画，响应区域的触摸事件，布局和管理一个或多个子视图。
- UIWindow:属于UIKit.framework框架，是一种特殊的UIView，通常在一个程序中只会有一个UIWindow,但可以手动创建多个UIWindow,同时加到程序里面。UIWindow在程序中主要起到三个作用：
  *作为容器，包含app所要显示的所有视图
  *传递触摸消息到程序中view和其他对象
  *与UIViewController协同工作，方便完成设备方向旋转的支持
- CALayer：属于QuartzCore.framework,是用来绘制内容的，对内容进行动画处理依赖与UIView来进行显示，不能处理用户事件
- UIView和CALayer是相互依赖的，UIView依赖CALayer提供内容，CALayer依赖UIView的容器显示绘制内容。
- （补充）UIViewController:每个视图控制器都有一个自带的视图，并且负责这个视图相关的一切事务。方便管理视图中的子视图，负责model与view的通信，检测设备旋转以及内存警告，是所有视图控制类的基类，定义了控制器的基本功能。
```
frame 和bounds有什么不同？
```
frame指的是:该view在父view坐标系统中的位置和大小（参照点是父亲的坐标系统）
bounds指的是:该view在本身坐标系统中的位置和大小（参照点是本身坐标系统）
```
响应者链条
```
· 事件响应链。包括点击事件、画面刷新事件等。在视图栈内从上至下，或者从下至上传播，可以说点击事件的分发，传递以及处理。
· 首先解释响应者链的概念
  ⑴ UIResponder类，是UIKit中一个用于处理事件响应的基类。窗口上的所有事件触发，都由该类响应（即事件处理入口）。所以，窗口上的view以及控制器都是派生于该类的，例如UIView、UIController等。
⑵ 调用UIResponder类提供的方法或属性，我们就可以捕捉到窗口上的所有响应时间，并进行处理。
⑶ 响应者链条是由多个响应者对象连接起来的链条，其中响应者对象是能处理事件的对象，所有view和viewController都是响应者的对象，利用响应者链条能让多个控件处理同一个触摸事件。
· 事件传递机制
如果当前view不能处理当前事件，那么事件将会沿着响应者链（Responder Chain）进行传递，直到能遇到能处理该事件的响应者。
- 接收事件的initial view如果不能处理该事件并且它不是顶层的view,则事件会往它的父view进行传递。
- initial view的父view获取事件后如果仍不能处理，则继续往上传递，循环这个过程。如果顶层的view还是不能处理这个事件的话，则会将时间传递给他们的viewController
- 如果viewController也不能处理，则传递给windown(UIWindown),此时windown不能处理的话就将事件传递UIAppliaction,最后如果连Application也不能处理，则废弃该事件。
```
