# iOS开发之UIViewController - 三少GG - CSDN博客
2016年02月15日 18:38:07[三少GG](https://me.csdn.net/scut1135)阅读数：803
个人分类：[IOS 常规](https://blog.csdn.net/scut1135/article/category/6071587)

http://blog.csdn.net/xyz_lmn/article/details/9022399
  ViewController是iOS应用程序中重要的部分，是应用程序数据和视图之间的重要桥梁，ViewController管理应用中的众多视图。iOS的SDK中提供很多原生ViewController，以支持标准的用户界面，例如表视图控制器（UITableViewController）、导航控制器（UINavigationController）、标签栏控制器（UITabbarController）和iPad专有的UISplitViewController等。
按结构可以对iOS的所有ViewController分成两类：
        1、主要用于展示内容的ViewController，这种ViewController主要用于为用户展示内容，并与用户交互，如UITableViewController，UIViewController。
        2、用于控制和显示其他ViewController的ViewController。这种ViewController一般都是一个ViewController的容器。如UINavigationController，UITabbarController。它们都有一个属性：viewControllers。其中UINavigationController表示一种Stack式结构，push一个ViewController或pop一次，因此后一个ViewController一般会依赖前一个ViewController。而UITabbarController表示一个Array结构，各个ViewController是并列的。
## ViewController使用
UIViewController可以使用两种方式创建，1、xib方式，2、代码方式
        1）xib方式
        Command+N　新建文件，选Cocoa Touch UIViewController subclass,SubClass of UIViewController,勾选with
 XIB for user interface。定义ViewController名字是MainViewController,最终生成MainViewController.h MainViewController.m MainViewController.xib三个文件。在AppDelegate.m文件的
didFinishLaunchingWithOptions方法中加载xib文件。
加载xib文件后，在xib文件拖拽几个控件可看到效果。
     2）代码创建
       Command+N　新建文件UIViewController文件，在AppDelegate.m文件的didFinishLaunchingWithOptions方法中使用。在UIViewController的loadView方法中使用代码创建view。
## ViewController的生命周期
       前面写了[iOS应用程序的生命周期](http://blog.csdn.net/xyz_lmn/article/details/8916752)，这里会写到ViewController的生命周期，这个更像Android的Activity的生命周期（见文章最后的图）。ViewController生命周期会经历初始化、加载视图、销毁视图、生命结束等过程。
1）init方法
      初始化ViewController本身。
2）loadView方法
当view需要被展示而它却是nil时，viewController会调用该方法。
      如果代码维护View的话需要重写此方法，使用xib维护View的话不用重写。
3）viewDidLoad方法
执行完loadView后继续执行viewDidLoad，loadView时还没有view，而viewDidLoad时view已经创建好了。
4）viewDidUnload方法
     当系统内存吃紧的时候会调用该方法，内存吃紧时，在iPhone OS 3.0之前didReceiveMemoryWarning是释放无用内存的唯一方式，但是OS 3.0及以后viewDidUnload方法是更好的方式。
      在该方法中将所有IBOutlet（无论是property还是实例变量）置为nil（系统release view时已经将其release掉了）。
      在该方法中释放其他与view有关的对象、其他在运行时创建（但非系统必须）的对象、在viewDidLoad中被创建的对象、缓存数据等。
      一般认为viewDidUnload是viewDidLoad的镜像，因为当view被重新请求时，viewDidLoad还会重新被执行。
5）dealloc
      释放其他资源或内存。
![](https://img-blog.csdn.net/20130621112948812?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveHl6X2xtbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
viewController的生命周期图
ViewController加载view过程，见下图（loadView）
### 1）loadView
![](https://img-blog.csdn.net/20130621105945796?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveHl6X2xtbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
ViewController卸载View过程见（unLoadView）
### 2）unLoadView
![](https://img-blog.csdn.net/20130621105956156?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveHl6X2xtbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

http://www.jianshu.com/p/99f37dac2e8c
对于初学者来说往往无法弄清iOS中各种各样的Controller之间的关系和使用场景。这篇文章将尝试整理各个Controller的用法以及注意事项。
> 
主要介绍的Controller有：
- UIViewController
- UINavigationController
- UITabBarController
- ModalViewController
- ChildViewController
## UIXXXController之间的关系
对于各个Controller之间的关系我想从下图开始讲解：
![img](http://lisi1987-blog-images.qiniudn.com/blog_uiviewcontroller_001_v004.png)
img
程序的入口是main()这个大家都知道，iOS也不例外也是在main
里面：
```
int main(int argc, char * argv[]) {
    @autoreleasepool {
        return UIApplicationMain(argc, argv, nil, NSStringFromClass([AppDelegate class]));
    }
}
```
对于这个main里面干了什么事情与这篇文章的主题不太符合，初学者可能也不太能理解，感兴趣的可以去了解一下`runloop`，我们只要知道在这之后，我们的APP将要执行的类就到了AppDelegate，并且我们的代码也是从这里开始。
与我们今天主题最相关的就是下面一段了：
```
- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions {
    // Override point for customization after application launch.
    self.rootViewController = [[UIViewController alloc] init];
    [self makeKeyAndVisible];
    return YES;
}
```
这段代码设置就是iOS程序里启动后显示的第一个ViewController，接下来的流程就全由你自己控制了。
> 
PS：在Xcode5 之后默认使用storyboard不需要在这里写任何代码就会有显示默认的ViewController，那时因为在info.plist中选择使用Main.storyboard的initialViewController作为初始ViewController。
对于一个iOS的APP来讲第一个UIViewController的类型是非常重要的，它将决定这个APP的UI架构和层级。
我们就从Xcode自带的3个典型模板工程去分析和讲解。
![img](http://lisi1987-blog-images.qiniudn.com/blog_uiviewcontroller_002.png)
img
如图，Xcode创建工程时有几组模板工程可以选择：`Master-Detail Application`、
```
Single
 View Application
```
、`Tabbed Application`。其中创建后对应的rootViewController分别为：
- Master-Detail Application ---> UINavigationController
- Single View Application ---> UIViewController
- Tabbed Application ---> UITabbarController
> 
PS：Page-Based Application在今天的主题中暂不讨论

http://wongkoo.github.io/2015/07/28/UIViewController/
### - (void)loadView;
*This is where subclasses should create their custom view hierarchy if they aren’t using a nib. Should never be called directly.如果没有使用Nib，那么子类应该实现该方法创建视图层。永远不要直接调用它！*
使用IB的时候，不要覆写该方法。
#### 调用
不应该被直接调用。那什么时候被调用？
每次访问UIViewController的view(比如controller.view、self.view)而且view为nil，loadView方法就会被调用。
#### 作用
loadView方法是用来负责创建UIViewController的view
#### 默认实现
1、它会先去查找与UIViewController相关联的xib文件，通过加载xib文件来创建UIViewController的view
初始化中指定某xib，显式初始化
```
[[MJViewController alloc] initWithNibName:@"MJViewController" bundle:nil];
```
如果没有明显的传xin文件名，就会加载跟UIViewController相同文件名的xib文件。
```
[[MJViewController alloc] init]; // 加载MJViewController.xib
```
2、如果没有找到相关联的xib文件，就会创建一个空白的UIView，然后赋值给UIViewController的view属性，大致如下
```
self.view = [[[UIView alloc] initWithFrame:[UIScreen mainScreen].applicationFrame] autorelease];
//applicationFrame的值是：{{x = 0, y = 20}, {width = 320, height = 460}}
```
#### 使用
如果想通过代码来创建UIViewController的View，就要重写loadView方法，并且不需要调用[super loadView]，因为若没有xib文件，[super loadView]默认会创建一个空白的UIView。
我们既然要通过代码来自定义UIView，那么就没必要事先创建一个空白的UIView，以节省不必要的开销。正确的做法应该是这样：
```
- (void)loadView {  
     self.view = [[[UIWebView alloc] initWithFrame:[UIScreen mainScreen].applicationFrame] autorelease];  
}
```
不需要调用[super loadView]，你调用了也不会出错，只是造成了一些不必要的开销。
总结一句话，苹果设计这个方法就是给我们自定义UIViewController的View用的。
# [Android程序员 如何入门iOS ——UIViewController 和 Layout System](https://segmentfault.com/a/1190000004334654)
https://segmentfault.com/a/1190000004334654
