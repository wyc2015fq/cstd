# ios 之CGRectMake----包括CGPoint、CGSize、CGRect - xqhrs232的专栏 - CSDN博客
2019年02月22日 15:14:26[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：30
原文地址::[https://www.cnblogs.com/yulang314/p/3551304.html](https://www.cnblogs.com/yulang314/p/3551304.html)
首先要弄懂几个基本的概念。
一）三个结构体：CGPoint、CGSize、CGRect
1.  CGPoint
[plain] [view plain](http://blog.csdn.net/iukey/article/details/7083165)[copy](http://blog.csdn.net/iukey/article/details/7083165)
- /* Points. */  
- 
- struct CGPoint {  
-   CGFloat x;  
-   CGFloat y;  
- };  
- typedef struct CGPoint CGPoint;  
看到这个想必你已经懂了，不再解释。
2.  CGSize
[plain] [view plain](http://blog.csdn.net/iukey/article/details/7083165)[copy](http://blog.csdn.net/iukey/article/details/7083165)
- /* Sizes. */  
- 
- struct CGSize {  
-   CGFloat width;  
-   CGFloat height;  
- };  
- typedef struct CGSize CGSize;  
不解释。
3.CGRect
[plain] [view plain](http://blog.csdn.net/iukey/article/details/7083165)[copy](http://blog.csdn.net/iukey/article/details/7083165)
- /* Rectangles. */  
- 
- struct CGRect {  
-   CGPoint origin;//偏移是相对父窗口的  
-   CGSize size;  
- };  
- typedef struct CGRect CGRect;  
同样 不解释。
这三个结构体均在一个头文件里：CGGeometry.h
二）几个方法
1.CGRectMake
[plain] [view plain](http://blog.csdn.net/iukey/article/details/7083165)[copy](http://blog.csdn.net/iukey/article/details/7083165)
- CG_INLINE CGRect  
- CGRectMake(CGFloat x, CGFloat y, CGFloat width, CGFloat height)  
- {  
-   CGRect rect;  
-   rect.origin.x = x; rect.origin.y = y;  
-   rect.size.width = width; rect.size.height = height;  
-   return rect;  
- }  
没错，这个方法就是make一个rect，定好origin（起点，左上角），宽与高，就可以画出一个位置与大小确定的rect（矩形）这个函数被声明为内联函数，一是因为它比较小，二是因为在画界面时我们要求一定的效率。这个函数还是藏在刚刚那个头文件里面：CGGeometry.h
三）几个基本界面元素：window（窗口）、视图（view）
要在屏幕上显示内容首先要创建一个窗口承载内容，要创建一个窗口，需要一个边框（frame），含有边框信息的底层 结构就CGRect。每个能够在屏幕上显示自己的对象都有一个边框，定义了他的显示区域，不过许多高层的视图类会自动计算这一信息。其他的那些类则在视图初始化时通过一个initWithFrame的初始化方法来设置。
再来认识一个类：UIScreen。UIScreen类代表了屏幕，通过这个类我们可以获取一些想要的东东。
[plain] [view plain](http://blog.csdn.net/iukey/article/details/7083165)[copy](http://blog.csdn.net/iukey/article/details/7083165)
- CGrect screenBounds = [ [UIScreen mainScreen]bounds];//返回的是带有状态栏的Rect  
- CGRect viewBounds = [ [UIScreen mainScreen]applicationFrame];//不包含状态栏的Rect  
- //screenBounds 与 viewBounds 均是相对于设备屏幕来说的  
- //所以 screenBounds.origin.x== 0.0 ;   screenBounds.oringin.y = 0.0;   screenBounds.size.width == 320;  screenBounds.size.height == 480(或者其他分辨率有所差异)  
- //所以 screenBounds.origin.x== 0.0 ;   screenBounds.oringin.y = 20.0;(因为状态栏的高度是20像素)   screenBounds.size.width == 320;  screenBounds.size.height == 480  
UIView
下面来认识一下UIView类，这个类继承自UIResponder,看这个名字我们就知道它是负责显示的画布，如果说把window比作画框的话。我们就是不断地在画框上移除、更换或者叠加画布，或者在画布上叠加其他画布，大小当然 由绘画者来决定了。有了画布，我们就可以在上面任意施为了。很多简单的东西我会把库里面的内容贴出来，如果东西太多贴出来就不太好，朋友们自己去库文件里面看吧。这个类在UIView.h里面。下面我们先学习一些基础的东西，其他的东东会在以后慢慢展开。
[plain] [view plain](http://blog.csdn.net/iukey/article/details/7083165)[copy](http://blog.csdn.net/iukey/article/details/7083165)
- 
- UIView* myView =[[ UIView alloc]initWithFrame:CGRectMake(0.0,0.0,200.0,400.0)];//这里创建了一块画布，定义了相对于父窗口的位置， 以及大小。  
我们可以把这块画布加到其他画布上，具体方法后面会讲到。我们还可以在这块画布上画上其它好玩的东东，具体情形后面会一一讲解。
UIWindow
UIWindow继承自UIView，关于这一点可能有点逻辑障碍，画框怎么继承自画布呢？不要过于去专牛角尖，画框的形状不就是跟画布一样吗？拿一块画布然后用一些方法把它加强，是不是可以当一个画框用呢？这也是为什么 一个view可以直接加到另一个view上去的原因了。
看一下系统的初始化过程（在application didFinishLauchingWithOptions里面）：
[plain] [view plain](http://blog.csdn.net/iukey/article/details/7083165)[copy](http://blog.csdn.net/iukey/article/details/7083165)
- self.window = [[[UIWindow alloc] initWithFrame:[[UIScreen mainScreen] bounds]] autorelease];  
- self.window.backgroundColor = [UIColor grayColor];//给window设置一个背景色  
- 
