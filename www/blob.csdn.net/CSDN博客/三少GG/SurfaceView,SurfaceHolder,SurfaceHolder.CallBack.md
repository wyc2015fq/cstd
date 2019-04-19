# SurfaceView,SurfaceHolder,SurfaceHolder.CallBack - 三少GG - CSDN博客
2012年01月01日 14:02:18[三少GG](https://me.csdn.net/scut1135)阅读数：1699标签：[android																[游戏																[ophone																[exception																[hierarchy																[callback](https://so.csdn.net/so/search/s.do?q=callback&t=blog)](https://so.csdn.net/so/search/s.do?q=hierarchy&t=blog)](https://so.csdn.net/so/search/s.do?q=exception&t=blog)](https://so.csdn.net/so/search/s.do?q=ophone&t=blog)](https://so.csdn.net/so/search/s.do?q=游戏&t=blog)](https://so.csdn.net/so/search/s.do?q=android&t=blog)
个人分类：[Android 常规](https://blog.csdn.net/scut1135/article/category/1057214)
## [**SurfaceView 教程**](http://www.ophonesdn.com/article/show/165)
SurfaceView介绍
      通常情况程序的View和用户响应都是在同一个线程中处理的，这也是为什么处理长时间事件（例如访问网络）需要放到另外的线程中去（防止阻塞当前UI线程的操作和绘制）。但是在其他线程中却不能修改UI元素，例如用后台线程更新自定义View（调用View的在自定义View中的onDraw函数）是不允许的。
** 如果需要在另外的线程绘制界面、需要迅速的更新界面或则渲染UI界面需要较长的时间，这种情况就要使用SurfaceView了。SurfaceView中包含一个Surface对象，而Surface是可以在后台线程中绘制的。**Surface属于OPhone底层显示系统，关于这方面的介绍请参考附录中的资料[1]。**SurfaceView的性质决定了其比较适合一些场景：需要界面迅速更新、对帧率要求较高的情况。使用SurfaceView需要注意以下几点情况：**
- SurfaceView和SurfaceHolder.Callback函数都从当前SurfaceView窗口线程中调用（一般而言就是程序的主线程）。有关资源状态要注意和绘制线程之间的同步。
- 在绘制线程中必须先合法的获取Surface才能开始绘制内容，在SurfaceHolder.Callback.surfaceCreated() 和SurfaceHolder.Callback.surfaceDestroyed()之间的状态为合法的，另外在Surface类型为SURFACE_TYPE_PUSH_BUFFERS时候是不合法的。
- 额外的绘制线程会消耗系统的资源，在使用SurfaceView的时候要注意这点。
**使用SurfaceView**
    只要继承SurfaceView类并实现SurfaceHolder.Callback接口就可以实现一个自定义的SurfaceView了，SurfaceHolder.Callback在底层的Surface状态发生变化的时候通知View，SurfaceHolder.Callback具有如下的接口：
- surfaceCreated(SurfaceHolder holder)：当Surface第一次创建后会立即调用该函数。程序可以在该函数中做些和绘制界面相关的初始化工作，一般情况下都是在另外的线程来绘制界面，所以不要在这个函数中绘制Surface。
- surfaceChanged(SurfaceHolder holder, **int** format,
**int** width,**int** height)：当Surface的状态（大小和格式）发生变化的时候会调用该函数，在surfaceCreated调用后该函数至少会被调用一次。
- surfaceDestroyed(SurfaceHolder holder)：当Surface被摧毁前会调用该函数，该函数被调用后就不能继续使用Surface了，一般在该函数中来清理使用的资源。
 通过SurfaceView的getHolder()函数可以获取SurfaceHolder对象，Surface 就在SurfaceHolder对象内。虽然Surface保存了当前窗口的像素数据，但是在使用过程中是不直接和Surface打交道的，由SurfaceHolder的Canvas lockCanvas()或则Canvas lockCanvas(Rect dirty)函数来获取Canvas对象，通过在Canvas上绘制内容来修改Surface中的数据。如果Surface不可编辑或则尚未创建调用该函数会返回null，在
 unlockCanvas() 和 lockCanvas()中Surface的内容是不缓存的，所以需要完全重绘Surface的内容，为了提高效率只重绘变化的部分则可以调用lockCanvas(Rect dirty)函数来指定一个dirty区域，这样该区域外的内容会缓存起来。在调用lockCanvas函数获取Canvas后，SurfaceView会获取Surface的一个同步锁直到调用unlockCanvasAndPost(Canvas canvas)函数才释放该锁，这里的同步机制保证在Surface绘制过程中不会被改变（被摧毁、修改）。
当在Canvas中绘制完成后，调用函数unlockCanvasAndPost(Canvas canvas)来通知系统Surface已经绘制完成，这样系统会把绘制完的内容显示出来。为了充分利用不同平台的资源，发挥平台的最优效果可以通过SurfaceHolder的setType函数来设置绘制的类型，目前接收如下的参数：
- *SURFACE_TYPE_NORMAL*：用RAM缓存原生数据的普通Surface
- *SURFACE_TYPE_HARDWARE*：适用于DMA(*Direct memory access* )引擎和硬件加速的Surface
- *SURFACE_TYPE_GPU*：适用于GPU加速的Surface
- *SURFACE_TYPE_PUSH_BUFFERS*：表明该Surface不包含原生数据，Surface用到的数据由其他对象提供，在Camera图像预览中就使用该类型的Surface，有Camera负责提供给预览Surface数据，这样图像预览会比较流畅。如果设置这种类型则就不能调用lockCanvas来获取Canvas对象了。
         目前OPhone还不支持GIF动画图片的显示，这里就通过一个SurfaceView来展示如何定制一个支持GIF动画的View，同时从该示例（注释）中也可以看出如何使用SurfaceView。
         首先创建一个GifView继承在SurfaceView，代码如下：
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# [android大扫盲之SurfaceView,SurfaceHolder,SurfaceHolder.CallBack.](http://www.cnblogs.com/bausch/archive/2011/10/20/2219068.html)
# [你hold住了吗](http://www.cnblogs.com/bausch/archive/2011/10/20/2219068.html)
最近接触到了SurfaceView,SurfaceHolder,SurfaceHolder.CallBack,一直不求其解，现在来粗浅认识一下它们。
先看一下官方的定义：
1.SurfaceView
SurfaceView是视图(View)的继承类，这个视图里内嵌了一个专门用于绘制的Surface。你可以控制这个Surface的格式和尺寸。Surfaceview控制这个Surface的绘制位置。
surface是纵深排序(Z-ordered)的，这表明它总在自己所在窗口的后面。surfaceview提供了一个可见区域，只有在这个可见区域内 的surface部分内容才可见，可见区域外的部分不可见。surface的排版显示受到视图层级关系的影响，它的兄弟视图结点会在顶端显示。这意味者 surface的内容会被它的兄弟视图遮挡，这一特性可以用来放置遮盖物(overlays)(例如，文本和按钮等控件)。注意，如果surface上面 有透明控件，那么它的每次变化都会引起框架重新计算它和顶层控件的透明效果，这会影响性能。
  你可以通过SurfaceHolder接口访问这个Surface.用getHolder()方法可以得到这个接口。
surfaceview变得可见时，surface被创建；surfaceview隐藏前，surface被销毁。这样能节省资源。如果你要查看 surface被创建和销毁的时机，可以重载surfaceCreated(SurfaceHolder)和 surfaceDestroyed(SurfaceHolder)。
surfaceview的核心在于提供了两个线程：UI线程和渲染线程。这里应注意：
1> 所有SurfaceView和SurfaceHolder.Callback的方法都应该在UI线程里调用，一般来说就是应用程序主线程。渲染线程所要访问的各种变量应该作同步处理。
2> 由于surface可能被销毁，它只在SurfaceHolder.Callback.surfaceCreated()和 SurfaceHolder.Callback.surfaceDestroyed()之间有效，所以要确保渲染线程访问的是合法有效的surface。
2.SurfaceHolder
显示一个surface的抽象接口，使你可以控制surface的大小和格式， 以及在surface上编辑像素，和监视surace的改变。这个接口通常通过SurfaceView类实现。
3. SurfaceHolder.Callback
用户可以实现此接口接收surface变化的消息。当用在一个SurfaceView 中时， 它只在SurfaceHolder.Callback.surfaceCreated()和SurfaceHolder.Callback.surfaceDestroyed()之间有效。设置Callback的方法是SurfaceHolder.addCallback.
看完官方的介绍，明白了许多，surfaceView带z-order的，还可以在上面操作像素有木有，还有单独的渲染线程哦亲。如果做游戏开发，当然选SurfaceView了。
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
[android中用SurfaceHolder处理SurfaceView的画图](http://hi.baidu.com/xxw8393/blog/item/dea5d773126546168601b062.html)
在用SurfaceView进行游戏开发过程中，用到SurfaceHolder来处理它的Canvas上画的效果和动画是必不可少的。用于控制表面，大小，像素等。
Abstract interface to someone holding a display surface. Allows you to control the surface size and format,
edit the pixels in the surface, and monitor changes to the surface. This interface is typically available
through the SurfaceView class. 
其中特别要注意以下的几个函数：
abstract void addCallback(SurfaceHolder.Callback callback);
// 给SurfaceView当前的持有者一个回调对象。
abstract Canvas lockCanvas();
// 锁定画布，一般在锁定后就可以通过其返回的画布对象Canvas，在其上面画图等操作了。
abstract Canvas lockCanvas(Rect dirty);
// 锁定画布的某个区域进行画图等..因为画完图后，会调用下面的unlockCanvasAndPost来改变显示内容。
// 相对部分内存要求比较高的游戏来说，可以不用重画dirty外的其它区域的像素，可以提高速度。
abstract void unlockCanvasAndPost(Canvas canvas);
// 结束锁定画图，并提交改变。
例子：
// 请参考上一篇文章：[**android中用SurfaceView做游戏开发**](http://cg3a20.mail.163.com/a/f/js3/editor/0904231044/androidsurfaceview.html)
    class DrawThread extends Thread {
        private SurfaceHolder holder;
        private boolean running = true;
        protected DrawThread(SurfaceHolder holder) {this.holder = holder;}
        protected void doStop() { running = false; }
        public void run() {
            Canvas c = null;
            while( running ) {
                c = holder.lockCanvas(null);
                // 锁定整个画布，在内存要求比较高的情况下，建议参数不要为null
                try {
                    synchronized(holder) {
                        bGrid.drawGrid(c);//画游戏中的网格
                        BBoom.drawBooms(c, booms); //画游戏中的炸弹
                        bFairy.drawFairy(c);//画游戏中的主角
                        // 画的内容是z轴的，后画的会覆盖前面画的。
                    }
                } catch(Exception ex) {}
                finally {
                    holder.unlockCanvasAndPost(c);
                    //更新屏幕显示内容
                }
    
            }
        }
    };
**在android中开发游戏，一般来说，或想写一个复杂一点的游戏，是必须用到SurfaceView来开发的。**经过这一阵子对android的学习，我找到了自已在android中游戏开发的误区，**不要老想着用Layout和view去实现，不要将某个游戏中的对象做成一个组件来处理**。应该尽量想着在Canvas(画布)中画出游戏戏中的背景、人物、动画等...
**SurfaceView提供直接访问一个可画图的界面，可以控制在界面顶部的子视图层。SurfaceView是提供给需要直接画像素而不是使用窗体部件的应用使用的。**Android图形系统中一个重要的概念和线索是surface。View及其子类（如TextView, Button）
要画在surface上。每个surface创建一个Canvas对象（但属性时常改变），用来管理view在surface上的绘图操作，如画点画线。
还要注意的是，使用它的时候，一般都是出现在最顶层的：The view hierarchy will take care of correctly compositing 
with the Surface any siblings of the SurfaceView that would normally appear on top of it.
使用的SurfaceView的时候，一般情况下还要对其进行创建，销毁，改变时的情况进行监视，这就要用到SurfaceHolder.Callback.
class BBatt extends SurfaceView implements SurfaceHolder.Callback {
    public void surfaceChanged(SurfaceHolder holder,int format,int width,int height){}
//看其名知其义，在surface的大小发生改变时激发
    public void surfaceCreated(SurfaceHolder holder){}
//同上，在创建时激发，一般在这里调用画图的线程。
    public void surfaceDestroyed(SurfaceHolder holder) {}
//同上，销毁时激发，一般在这里将画图的线程停止、释放。
}
例子：
public class BBatt extends SurfaceView implements 
                 SurfaceHolder.Callback, OnKeyListener {
             private BFairy bFairy;
             private DrawThread drawThread;
             public BBatt(Context context) {
                 super(context);
                 this.setLayoutParams(
                     new ViewGroup.LayoutParams(
                         Global.battlefieldWidth, Global.battlefieldHeight));
                 this.getHolder().addCallback( this );
                 this.setFocusable( true );
                 this.setOnKeyListener( this );
                 bFairy = new BFairy(this.getContext());
             }
             public void surfaceChanged(SurfaceHolder holder,
                  int format,int width,int height) {
                  drawThread = new DrawThread(holder);
                  drawThread.start();
             }
             public void surfaceDestroyed(SurfaceHolder holder) {
                  if( drawThread != null ) {
                        drawThread.doStop();
                        while (true) try {
                             drawThread.join();
                             break ;
                        } catch(Exception ex) {}
                  }
             }
             public boolean onKey(View view, int keyCode, KeyEvent event) {}
}
