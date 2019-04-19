# iOS中视图切换的四种方式 - xqhrs232的专栏 - CSDN博客
2019年02月27日 16:38:02[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：39
个人分类：[Mac OS/i OS技术](https://blog.csdn.net/xqhrs232/article/category/1252162)
原文地址::[https://blog.csdn.net/u011774517/article/details/54948373](https://blog.csdn.net/u011774517/article/details/54948373)
相关文章
1、比较IOS开发中常用视图的四种切换方式----[https://blog.csdn.net/thanklife/article/details/78069921](https://blog.csdn.net/thanklife/article/details/78069921)
2、iOS如何创建多视图及视图间切换(学习记录)----[https://blog.csdn.net/waitaction/article/details/79558363](https://blog.csdn.net/waitaction/article/details/79558363)
3、IOS视图切换的使用----[http://www.runoob.com/ios/att-ios-ui-view-transition.html](http://www.runoob.com/ios/att-ios-ui-view-transition.html)
4、[iOS 模态视图，视图之间的切换](https://www.cnblogs.com/zhaochaobin/p/5276977.html)----[https://www.cnblogs.com/zhaochaobin/p/5276977.html](https://www.cnblogs.com/zhaochaobin/p/5276977.html)
5、[iOS Controller中视图切换动画效果](https://www.cnblogs.com/zimou47/p/6432810.html)----[https://www.cnblogs.com/zimou47/p/6432810.html](https://www.cnblogs.com/zimou47/p/6432810.html)
iOS中常用的四种视图切换的方式有UITabBarController、UINavigationController、模态窗口以及添加为子视图的方式 
一、简单分析一下各自的原理： 
* UITabBarController：以平行的方式管理视图，各个视图之间往往关系并不大，每个加入到UITabBarController的视图都会进行初始化即使当前不显示在界面上，相对比较占用内存。 
* UINavigationController：以栈的方式管理视图，各个视图的切换就是压栈和出栈操作，出栈后的视图会立即销毁。 
* UIModalController：以模态窗口的形式管理视图，当前视图关闭前其他视图上的内容无法操作。 
* 添加为子视图的方式 
二、详细介绍 
UITabBarController 
UITabBarController是Apple专门为了利用页签切换视图而设计的，在这个视图控制器中有一个UITabBar控件，用户通过点击tabBar进行视图切换。我们知道在UIViewController内部有一个视图，一旦创建了UIViewController之后默认就会显示这个视图，但是UITabBarController本身并不会显示任何视图，如果要显示视图则必须设置其viewControllers属性（它默认显示viewControllers[0]）。这个属性是一个数组，它维护了所有UITabBarController的子视图。
对于UITabBarController简单总结如下： 
* UITabBarController会一次性初始化所有子控制器，但是默认只加载第一个控制器视图，其他视图控制器只初始化默认不会加载,为了能够将其他子控制器也正常显示在Tab bar中我们访问了每个子视图控制器的视图以便调用其视图加载方法（viewDidLoad）;当然，既然会调用子视图的初始化方法，当然也可以将视图控制器的tabBarItem属性设置放到init方法中设置，如此则不用再遍历其视图属性了。 
* tabBarController与子视图的交流：每个视图控制器都有一个tabBarController属性，通过它可以访问所在的UITabBarController 
* 每个视图控制器都有一个tabBarItem属性，通过它控制视图在UITabBarController的tabBar中的显示信息。 tabBarItem的image属性必须是png格式（建议大小32*32）并且打开alpha通道否则无法正常显示。 
* 
UINavigationController 
UINavigationController是一个导航控制器，它用来组织有层次关系的视图，在UINavigationController中子控制器以栈的形式存储，只有在栈顶的控制器能够显示在界面中，一旦一个子控制器出栈则会被销毁。UINavigationController默认也不会显示任何视图（这个控制器自身的UIView不会显示），它必须有一个根控制器rootViewController，而且这个根控制器不会像其他子控制器一样被销毁。
简单总结 
* UINavigationController默认显示一个根控制器，，通过根控制器导航到其他下一级子视图。 
* 与子视图的沟通：在子视图中可以通过navigationController访问导航控制器，同时可以通过navigationController的childViewControllers获得当前栈中所有的子视图（注意每一个出栈的子视图都会被销毁）。 
* UINavigationController导航是通过上方导航栏进行的，每个放到UINavigationController栈中的子视图都会显示一个导航栏，可以通过子控制器（包括根控制器）的navigationItem访问这个导航栏，修改其左右两边的按钮内容。 默认情况下除了根控制器之外的其他子控制器都会在导航栏左侧显示返回按钮，点击可以返回上一级视图，同时按钮标题默认为上一级视图的标题,可以通过backBarButtonItem修改。
模态窗口(模式－视图－控制器) 
模态窗口只是视图控制器显示的一种方式（在iOS中并没有专门的模态窗口类），模态窗口不依赖于控制器容器直接通过使用视图控制器来管理不涉及其他视图，通常用于显示独立的内容，在模态窗口显示的时其他视图的内容无法进行操作。
其流程是一次显示一个视图控制器然后从一个视图控制器迁移到另一个。流程如图所示 
模态窗口使用起来比较容易，一般的视图控制器只要调用- (void)presentViewController:(UIViewController *)viewControllerToPresent animated: (BOOL)flag completion:(void (^)(void))completion NS_AVAILABLE_IOS(5_0);方法那么参数中的视图控制器就会以模态窗口的形式展现，同时调用- (void)dismissViewControllerAnimated: (BOOL)flag completion: (void (^)(void))completion NS_AVAILABLE_IOS(5_0);方法就会关闭模态窗口。
添加为子视图 
从另一个视图控制器把视图添加到当前视图控制器，作为子视图。如果再添加之前不把前一个删除的话，整个栈上的子视图将难以管理。流程图如下 
个人认为如果不需要花哨的效果，又不想让导航条占去屏幕的空间，推荐使用模态窗口
1
参考：http://www.cnblogs.com/kenshincui/p/3940746.html#autoid-4-0-0 
《设计模式》
