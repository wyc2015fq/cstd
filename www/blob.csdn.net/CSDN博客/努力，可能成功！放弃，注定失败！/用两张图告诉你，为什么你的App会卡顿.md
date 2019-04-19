# 用两张图告诉你，为什么你的App会卡顿? - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2017年04月18日 15:04:54[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：1132
![](http://upload-images.jianshu.io/upload_images/1869462-e45f4cf10981b323.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
Cover
# 本文来自简书，原文地址:[http://www.jianshu.com/p/df4d5ec779c8](http://www.jianshu.com/p/df4d5ec779c8)
# 有什么料？
从这篇文章中你能获得这些料：
- 知道setContentView()之后发生了什么？
- 知道Android究竟是如何在屏幕上显示我们期望的画面的？
- 对Android的视图架构有整体把握。
- 学会从根源处分析画面卡顿的原因。
- 掌握如何编写一个流畅的App的技巧。
- 从源码中学习Android的细想。
- 收获两张自制图，帮助你理解Android的视图架构。
![](http://upload-images.jianshu.io/upload_images/1869462-59ab533d5276864e.JPG?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
link
# 从setContentView()说起
```
public class AnalyzeViewFrameworkActivity extends Activity {
  @Override
  protected void onCreate(Bundle savedInstanceState) {
    super.onCreate(savedInstanceState);
    setContentView(R.layout.activity_analyze_view_framwork);
  }
}
```
上面这段代码想必Androider们大都已经不能再熟悉的更多了。但是你知道这样写了之后发生什么了吗？这个布局到底被添加到哪了？我的天，知识点来了！
可能很多同学也知道这个布局是被放到了一个叫做DecorView的父布局里，但是我还是要再说一遍。且看下图✌️
![](http://upload-images.jianshu.io/upload_images/1869462-26584be9906690ef.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
image
这个图可能和伙伴们在书上或者网上常见的不太一样，为什么不太一样呢？因为是我自己画的，哈哈哈...
下面就来看着图捋一捋Android最基本的视图框架。
## PhoneWindow
估计很多同学都知道，每一个Activity都拥有一个Window对象的实例。这个实例实际是PhoneWindow类型的。那么PhoneWindow从名字很容易看出，它应该是Window的儿子（即子类）！
知识点：每一个Activity都有一个PhoneWindow对象。
那么，PhoneWindow有什么用呢？它在Activity充当什么角色呢？下面我就姑且把PhoneWindow等同于Window来称呼吧。
![](http://upload-images.jianshu.io/upload_images/1869462-2bf58e5125348273.PNG?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
image
Window从字面看它是一个窗口，意思和PC上的窗口概念有点像。但也不是那么准确。看图说。可以看到，我们要显示的布局是被放到它的属性mDecor中的，这个mDecor就是DecorView的一个实例。下面会专门撸DecorView，现在先把关注点放到Window上。Window还有一个比较重要的属性mWindowManager，它是WindowManager(这是个接口)的一个实现类的一个实例。我们平时通过getWindowManager()方法获得的东西就是这个mWindowManager。顾名思义，它是Window的管理者，负责管理着窗口及其中显示的内容。它的实际实现类是WindowManagerImpl。可能童鞋们现在正在PhoneWindow中寻找着这个mWindowManager是在哪里实例化的，是不是上下来回滚动着这个类都找不见？STOP！mWindowManager是在它爹那里就实例化好的。下面代码是在`Window.java`中的。
```
public void setWindowManager(WindowManager wm, 
    IBinder appToken, 
    String appName, 
    boolean hardwareAccelerated) {
        ...
        if (wm == null) {
            wm = (WindowManager)mContext.getSystemService(Context.WINDOW_SERVICE);
            //获取了一个WindowManager
        }
        mWindowManager = ((WindowManagerImpl)wm).createLocalWindowManager(this);
        //通过这里我们可以知道，上面获取到的wm实际是WindowManagerImpl类型的。
    }
```
通过上面的介绍，我们已经知道了Window中有负责承载布局的DecorView，有负责管理的WindowManager（事实上它只是个代理，后面会讲它代理的是谁）。
## DecorView
前面提到过，在Activity的onCreate()中通过setContentView()设置的布局实际是被放到DecorView中的。我们在图中找到DecorView。
从图中可以看到，DecorView继承了FrameLayout，并且一般情况下，它会在先添加一个预设的布局。比如DecorCaptionView，它是从上到下放置自己的子布局的，相当于一个LinearLayout。通常它会有一个标题栏，然后有一个容纳内容的mContentRoot，这个布局的类型视情况而定。我们希望显示的布局就是放到了mContentRoot中。
知识点：通过setContentView()设置的布局是被放到DecorView中，DecorView是视图树的最顶层。
## WindowManager
前面已经提到过，WindowManager在Window中具有很重要的作用。我们先在图中找到它。这里需要先说明一点，在PhoneWindow中的mWindowManager实际是WindowManagerImpl类型的。WindowManagerImpl自然就是接口WindowManager的一个实现类喽。这一点是我没有在图中反映的。
WindowManager是在Activity执行attach()时被创建的，attach()方法是在onCreate()之前被调用的。关于Activity的创建可以看看我的这篇：[【可能是史上最简单的！一张图3分钟让你明白Activity启动流程，不看后悔！http://www.jianshu.com/p/9ecea420eb52】](http://www.jianshu.com/p/9ecea420eb52)。
Activity.java
```
final void attach(Context context, ActivityThread aThread,
    Instrumentation instr, IBinder token, int ident,
    Application application, Intent intent, ActivityInfo info,
    CharSequence title, Activity parent, String id,
    NonConfigurationInstances lastNonConfigurationInstances,
    Configuration config, String referrer, IVoiceInteractor voiceInteractor,
    Window window){
        ...
        mWindow = new PhoneWindow(this, window);
        //创建Window
        ...
        mWindow.setWindowManager(
         (WindowManager)context.getSystemService(Context.WINDOW_SERVICE),
         mToken, mComponent.flattenToString(),
         (info.flags & ActivityInfo.FLAG_HARDWARE_ACCELERATED) != 0);
        //注意！这里就是在创建WindowManager。
        //这个方法在前面已经说过了。
        if (mParent != null) {
           mWindow.setContainer(mParent.getWindow());
        }
        mWindowManager = mWindow.getWindowManager();
            }
```
继续看图。WindowManagerImpl持有了PhoneWindow的引用，因此它可以对PhoneWindow进行管理。同时它还持有一个非常重要的引用mGlobal。这个mGlobal指向一个WindowManagerGlobal类型的单例对象，这个单例每个应用程序只有唯一的一个。在图中，我说明了WindowManagerGlobal维护了本应用程序内所有Window的DecorView，以及与每一个DecorView对应关联的ViewRootImpl。这也就是为什么我前面提到过，WindowManager只是一个代理，实际的管理功能是通过WindowManagerGlobal实现的。我们来看个源码的例子就比较清晰了。开始啦！
![](http://upload-images.jianshu.io/upload_images/1869462-f784ff4183bd92c5.JPG?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
image
WimdowManagerImpl.java
```
public void addView(@NonNull View view, @NonNull ViewGroup.LayoutParams params) {
    ...
    mGlobal.addView(view, params, mContext.getDisplay(), mParentWindow);
    //实际是通过WindowManagerGlobal实现的。
}
```
从上面的代码可以看出，WindowManagerImpl确实只是WindowManagerGlobal的一个代理而已。同时，上面这个方法在整个Android的视图框架流程中十分的重要。我们知道，在Activity执行onResume()后界面就要开始渲染了。原因是在onResume()时，会调用WindowManager的addView()方法(实际最后调用的是WindowManagerGlobal的addView()方法)，把视图添加到窗口上。结合我的这篇[【可能是史上最简单的！一张图3分钟让你明白Activity启动流程，不看后悔！http://www.jianshu.com/p/9ecea420eb52】](http://www.jianshu.com/p/9ecea420eb52)看，可以帮助你更好的理解Android的视图框架。
ActivityThread.java
```
final void handleResumeActivity(IBinder token,
    boolean clearHide, boolean isForward, boolean reallyResume, int seq, String reason) {
    ...
    ViewManager wm = a.getWindowManager();
    //获得WindowManager，实际是WindowManagerImpl
    ...
    wm.addView(decor, l);
    //添加视图
    ...
    wm.updateViewLayout(decor, l);
    //需要刷新的时候会走这里
    ...
}
```
从上面可以看到，当Activity执行onResume()的时候就会添加视图，或者刷新视图。需要解释一点：WindowManager实现了ViewManager接口。
如图中所说，WindowManagerGlobal调用addView()的时候会把DecorView添加到它维护的数组中去，并且会创建另一个关键且极其重要的ViewRootImpl(这个必须要专门讲一下)类型的对象，并且也会把它存到一个数组中维护。
WindowManagerGlobal.java
```
public void addView(View view, ViewGroup.LayoutParams params,
    Display display, Window parentWindow) {
    ...
    root = new ViewRootImpl(view.getContext(), display);
    //重要角色登场
    view.setLayoutParams(wparams);
    mViews.add(view);
    mRoots.add(root);
    //保存起来维护
    mParams.add(wparams);
    ...
    root.setView(view, wparams, panelParentView);
    //设置必要属性view是DecorView，panelParentView是PhoneWindow
    ...
}
```
可以看出ViewRootImpl是在Activity执行onResume()的时候才被创建的，并且此时才把DecorView传进去让它管理。
知识点：WindowManager是在onCreate()时被创建。它对窗口的管理能力实际是通过WindowManagerGlobal实现的。在onResume()是视图才通过WindowManager被添加到窗口上。
## ViewRootImpl
ViewRootImpl能够和系统的WindowManagerService进行交互，并且管理着DecorView的绘制和窗口状态。非常的重要。赶紧在图中找到对应位置吧！
ViewRootImpl并不是一个View，而是负责管理视图的。它配合系统来完成对一个Window内的视图树的管理。从图中也可以看到，它持有了DecorView的引用，并且视图树它是视图树绘制的起点。因此，ViewRootImpl会稍微复杂一点，需要我们更深入的去了解，在图中我标出了它比较重要的组成Surface和Choreographer等都会在后面提到。
到此，我们已经一起把第一张图撸了一遍了，现在童鞋们因该对Android视图框架有了大致的了解。下面将更进一步的去了解Android的绘制机制。
# App总是卡顿到底是什么原因？
下面将会详细的讲解为什么我们设置的视图能够被绘制到屏幕上？这中间究竟隐藏着怎样的离奇？看完之后，你自然就能够从根源知道为什么你的App会那么卡，以及开始有思路着手解决这些卡顿。
![](http://upload-images.jianshu.io/upload_images/1869462-c71f7527c48dac86.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
image
同样用一张图来展示这个过程。由于Android绘制机制确实有点复杂，所以第一眼看到的时候你的内心中可能蹦腾了一万只草泥马
