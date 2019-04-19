# 深入了解UIViewController控制器与对应的View类的详解 - xqhrs232的专栏 - CSDN博客
2019年03月14日 17:36:08[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：15
个人分类：[Mac OS/i OS技术](https://blog.csdn.net/xqhrs232/article/category/1252162)
原文地址::[https://blog.csdn.net/hbblzjy/article/details/53066167](https://blog.csdn.net/hbblzjy/article/details/53066167)
相关文章
1、[关于View和VIewController的关系和理解](https://www.cnblogs.com/wisejoker/p/3399846.html)----[https://www.cnblogs.com/wisejoker/p/3399846.html](https://www.cnblogs.com/wisejoker/p/3399846.html)
2、ViewModel，多个ViewController的动态更换----[https://www.jianshu.com/p/37c0f3d39db6](https://www.jianshu.com/p/37c0f3d39db6)
iewController是iOS开发中MVC模式中的C(视图控制器)，ViewController是view的controller，ViewController的职责主要包括管理内部各个view的加载显示和卸载，同时负责与其他ViewController的通信和协调。
- 在IOS中，有两类ViewController
	- 一类主要用于展示内容，比如UIViewController、UITableViewController等，同时还可以自定义继承自UIViewController的ViewController；
- 另一类是ViewController容器，UINavigationViewController和UITabBarController等，UINavigationController是以Stack的形式来存储和管理ViewController，UITabBarController是以Array的形式来管理ViewController。
**不管是哪类ViewController,都继承自UIViewController**
### UIViewController --> 生命周期方法
> 
控制器从创建到销毁方法的执行顺序
load->initialize->init(initWithNibName)—>loadView—>viewDidLoad—>viewWillApper—>viewDidApper—>viewWillDisapper—>viewDidDisapper—>viewWillUnload->viewDidUnload—>dealloc
其中viewWillUnload跟viewDidUnLoad 在iOS6以后就过期了.
收到low-memory时系统不会释放view，而只是释放controller的resource。
- LoadView: 控制器调用loadView方法创建控制器的view.当控制器的view存在了就不会调用.
- 不要再LoadView中调用[super loadView],会影响CPU性能
- 苹果官方文档
	- You should never call this method directly. The view controller calls this method when its view property is requested but is currently nil. This method loads or creates a view and assigns it to the view property.
- 当系统要展示这个控制器view的时候,会先去view的getter方法中寻找有没有返回view,如果view == nil,系统就会主动去调用这个方法.
> 
控制器的view都是懒加载,当需要展示的时候才会去创建
- 懒加载:重写getter方法
	- 好处:不用管什么时候需要创建,做到要用时再创建
**view的加载过程**
![](http://upload-images.jianshu.io/upload_images/2996699-8770cb8869127531.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
view的加载过程
**从代码中加载view**
![](http://upload-images.jianshu.io/upload_images/2996699-369abaa1787f4856.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
代码创建
**从storyboard/xib中加载view**
![](http://upload-images.jianshu.io/upload_images/2996699-a0cc9d4d667b034f.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
从storyboard/xib中加载view
- **实现过程**- 先去判断当前控制器是不是从StoryBoard当中加载的,如果是,那么它就会从StoryBoard当中加载控制器的View.
- 如果不是从StoryBoard当中加载的, 那么它还会判断是不是从Xib当中创建的控制器.
		如果是,那么它就会从xib加载控制器的View.
- 如果也不是从Xib加载的控制器.那么它就会创建一个空的UIView.设为当前控制器的View.
		注意点:
		- 一旦重写了loadView,表示需要自己创建控制器的View.
- 3.2 如果控制器的View还没有赋值,就不能调用控制器View的get方法.会造成死循环.
			因为控制器View的get方法底层会调用loadView方法.
- 
**viewDidLoad:** 当控制器View加载完毕时调用
- 我们经常会在这个方法里面做一些控件的初始化操作,其中可能会给一些控件设置尺寸位置,但是需要注意的是在这个方法内只能参照控制器的view的尺寸,不能参照其他子控件的尺寸
- 
**viewWillAppear:** 视图将要显示的时候调用
- 
**viewWillLayoutSubviews:** viewWillLayoutSubviews即将对子控件进行布局
- **viewDidLayoutSubviews:**当控制器View布局子控件完毕时调用
	- 可以在此方法中可以用子控件的尺寸做参考
- 在这个方法内真正做一些控件布局的操作,可以拿到控件尺寸.
- 不管是从xib,strongBoard,代码中加载的控件,willAppear之后都会调用这个方法,方法调用很频繁
- 
**viewDidAppear:**当控制器View显示完毕时调用
- 
**viewWillDisappear:**当控制器View既将消失时调用
- 
**viewDidDisappear:**当控制器View消失完毕时调用
- 
**dealloc:** 视图被销毁，此处需要对你在init和viewDidLoad中创建的对象进行释放
- 在这个方法内,我们通常会把添加在控制器内的一些东西给移出.例如:KVO监听者,HUD,定时器等
- 
**didReceiveMemoryWarning:** 当收到系统发出的内存警告时调用,我们可以截获系统发出的内存警告然后做一些是自定义事情
收到内存警告系统执行步骤
![](http://upload-images.jianshu.io/upload_images/2996699-54788249191211f2.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
Paste_Image.png
### 类
- 
**load:**当一个类被加载时调用,只加载一次
- 
**initialize:**当本类或者子类被加载时调用,可能调用多次
- 
**init:**用代码创建类的时候调用,只能做一些初始化操作,不能设置控件的frame,init其实是去调用initWithFrame,只不过frame为CGRectZero
- 
**initWithFrame:**用代码创建类的时候调用,只能做一些初始化操作,不能在这设置控件的frame,如果已经知道了frame,那么在这里设置子控件的frame是没有问题的,但是如果外界使用init的方式创建,最终也会调用initWithFrame方法,此时的frame传进来是0,那么,在这个方法里面设置的子控件的frame也会为0.所以,为了严谨起见,最好不要在这个方法里面设置子控件的frame.
- 
**initWithCoder:**从xib/storyboard中加载就会调用此方法,只能在这个方法做一些一次性设置,不能设置控件的frame
- 
**awakeFromNib:**从文件中加载.就会调用此方法,可以在这个方法中设置frame
- **layoutSubviews:**布局本类或者子控件时调用,调用频率频繁,通常在这里设置子控件的位置
	- 注意:一定要在[super layoutSubviews];之后进行设置控件的frame
- 苹果官方文档
		- You should override this method only if the autoresizing and constraint-based behaviors of the subviews do not offer the behavior you want.
- 当我们在某个类的内部调整子视图位置时，需要调用。反过来的意思就是说：如果你想要在外部设置subviews的位置，就不要重写
- 如果没有先调用[super layoutSubviews];可能设置了frame,控件也不会显示
- 可能触发layoutSubviews的情况
		- addSubviews
- 滚动scrollView
- 旋转Screen会触发父类的layouSubviews
- ....
转载自：[http://www.jianshu.com/p/ab39683f94ae](http://www.jianshu.com/p/ab39683f94ae)
