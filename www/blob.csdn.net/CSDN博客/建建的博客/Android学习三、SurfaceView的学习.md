# Android学习三、SurfaceView的学习 - 建建的博客 - CSDN博客
2017年06月01日 11:58:57[纪建](https://me.csdn.net/u013898698)阅读数：145
想要学习SurfaceView，必须了解SurfaceView、Surface、SurfaceHolder。
一、Surface
下面是Google提供的关于Surface的API的概述：
Handle onto a raw buffer that is being managed by the screen compositor.
直接从raw缓冲区中读取数据到屏幕。
surface的意思就是界面，可以理解为我们要显示的画面，不过知识先将它存储到内存中，完成后，才显示到屏幕中，也就是SurfaceView中。
几个重要的方法
![](https://img-my.csdn.net/uploads/201204/21/1335003494_8087.jpg)

二、SurfaceHolder
下面是Google提供的关于SurfaceHolder的API的概述：
Abstract interface to someone holding a display surface. Allows you to control the surface size and format, edit the pixels in the surface, and monitor changes to the surface. This interface is typically available through the`SurfaceView` class.
When using this interface from a thread other than the one running its `SurfaceView`, you will want to carefully read the methods`lockCanvas()` and`Callback.surfaceCreated()`.
SurfaceHolder是一个抽象的接口，用来支持显示一个surface。它允许你控制这个surface的大小和格式，允许你编辑这个surface，并且监测这个surface的变化。这个接口
的典型应用是通过SurfaceView类来实现的。
当你在一个线程中使用这个接口，而不是通过运行一个SurfaceView得到，那么，你就要自己掌握lockCanvas()和`Callback.surfaceCreated()`.
也就是说SurfaceHolder是操作Surface的接口。
SurfaceHolder的方法
![](https://img-my.csdn.net/uploads/201204/21/1335004022_5926.jpg)
关于SurfaceHolder的setType()方法的参数：
SURFACE_TYPE_NORMAL：用RAM缓存原生数据的普通Surface
SURFACE_TYPE_HARDWARE：适用于DMA(Direct memory access )引擎和硬件加速的Surface
SURFACE_TYPE_GPU：适用于GPU加速的Surface
SURFACE_TYPE_PUSH_BUFFERS：表明该Surface不包含原生数据，Surface用到的数据由其他对象提供，在Camera图像预览中就使用该类型的Surface，有Camera 负责提供给预览Surface数据，这样图像预览会比较流畅。如果设置这种类型则就不能调用lockCanvas来获取Canvas对象了。
虽然setType()方法已经被声明deprecated，但是必要的时候还是设置下比较好。
几个需要注意的方法：
(1)、abstract void addCallback(SurfaceHolder.Callback callback);
// 给SurfaceView当前的持有者一个回调对象。
(2)、abstract Canvas lockCanvas();
// 锁定画布，一般在锁定后就可以通过其返回的画布对象Canvas，在其上面画图等操作了。
(3)、abstract Canvas lockCanvas(Rect dirty);
// 锁定画布的某个区域进行画图等..因为画完图后，会调用下面的unlockCanvasAndPost来改变显示内容。
// 相对部分内存要求比较高的游戏来说，可以不用重画dirty外的其它区域的像素，可以提高速度。
(4)、abstract void unlockCanvasAndPost(Canvas canvas);
// 结束锁定画图，并提交改变。

三、SurfaceView
下面是Google提供的关于SurfaceView的API的概述：
Provides a dedicated drawing surface embedded inside of a view hierarchy. You can control the format of this surface and, if you like, its size; the SurfaceView takes care of placing the surface at the correct location on the screen
The surface is Z ordered so that it is behind the window holding its SurfaceView; the SurfaceView punches a hole in its window to allow its surface to be displayed. The view hierarchy will take care of correctly compositing with the Surface any siblings of
 the SurfaceView that would normally appear on top of it. This can be used to place overlays such as buttons on top of the Surface, though note however that it can have an impact on performance since a full alpha-blended composite will be performed each time
 the Surface changes.
Access to the underlying surface is provided via the SurfaceHolder interface, which can be retrieved by calling`getHolder()`.
The Surface will be created for you while the SurfaceView's window is visible; you should implement`surfaceCreated(SurfaceHolder)` and`surfaceDestroyed(SurfaceHolder)` to
 discover when the Surface is created and destroyed as the window is shown and hidden.
One of the purposes of this class is to provide a surface in which a secondary thread can render into the screen. If you are going to use it this way, you need to be aware of some threading semantics:
- All SurfaceView and `SurfaceHolder.Callback` methods will be called from the thread running the SurfaceView's window (typically the main thread of the application). They thus need
 to correctly synchronize with any state that is also touched by the drawing thread.
- You must ensure that the drawing thread only touches the underlying Surface while it is valid -- between`SurfaceHolder.Callback.surfaceCreated()` and`SurfaceHolder.Callback.surfaceDestroyed()`.
翻译(Copy别人的，唉，英语太差了，伤不起啊！![奋斗](http://static.blog.csdn.net/xheditor/xheditor_emot/default/struggle.gif)，努力学习英文！)
SurfaceView是视图(View)的继承类，这个视图里内嵌了一个专门用于绘制的Surface。你可以控制这个Surface的格式和尺寸。Surfaceview控制这个Surface的绘制位置。
surface是纵深排序(Z-ordered)的，这表明它总在自己所在窗口的后面。surfaceview提供了一个可见区域，只有在这个可见区域内 的surface部分内容才可见，可见区域外的部分不可见。surface的排版显示受到视图层级关系的影响，它的兄弟视图结点会在顶端显示。这意味者 surface的内容会被它的兄弟视图遮挡，这一特性可以用来放置遮盖物(overlays)(例如，文本和按钮等控件)。注意，如果surface上面 有透明控件，那么它的每次变化都会引起框架重新计算它和顶层控件的透明效果，这会影响性能。
你可以通过SurfaceHolder接口访问这个surface，getHolder()方法可以得到这个接口。
surfaceview变得可见时，surface被创建；surfaceview隐藏前，surface被销毁。这样能节省资源。如果你要查看 surface被创建和销毁的时机，可以重载surfaceCreated(SurfaceHolder)和 surfaceDestroyed(SurfaceHolder)。
surfaceview的核心在于提供了两个线程：UI线程和渲染线程。这里应注意：
1> 所有SurfaceView和SurfaceHolder.Callback的方法都应该在UI线程里调用，一般来说就是应用程序主线程。渲染线程所要访问的各种变量应该作同步处理。
2> 由于surface可能被销毁，它只在SurfaceHolder.Callback.surfaceCreated()和 SurfaceHolder.Callback.surfaceDestroyed()之间有效，所以要确保渲染线程访问的是合法有效的surface。
1、定义
可以直接从内存或者DMA等硬件接口取得图像数据,是个非常重要的绘图容器。
它的特性是：可以在主线程之外的线程中向屏幕绘图上。这样可以避免画图任务繁重的时候造成主线程阻塞，从而提高了程序的反应速度。在游戏开发中多用到SurfaceView，游戏中的背景、人物、动画等等尽量在画布canvas中画出。
2、实现
首先继承SurfaceView并实现SurfaceHolder.Callback接口
使用接口的原因：因为使用SurfaceView 有一个原则，所有的绘图工作必须得在Surface 被创建之后才能开始(Surface—表面，这个概念在 图形编程中常常被提到。基本上我们可以把它当作显存的一个映射，写入到Surface 的内容
                      可以被直接复制到显存从而显示出来，这使得显示速度会非常快)，而在Surface 被销毁之前必须结束。所以Callback 中的surfaceCreated 和surfaceDestroyed 就成了绘图处理代码的边界。
需要重写的方法
（1）public void surfaceChanged(SurfaceHolder holder,int format,int width,int height){}
　    //在surface的大小发生改变时激发
　（2）public void surfaceCreated(SurfaceHolder holder){}
　    //在创建时激发，一般在这里调用画图的线程。
　（3）public void surfaceDestroyed(SurfaceHolder holder) {}
　    //销毁时激发，一般在这里将画图的线程停止、释放。
整个过程：继承SurfaceView并实现SurfaceHolder.Callback接口 ----> SurfaceView.getHolder()获得SurfaceHolder对象 ---->SurfaceHolder.addCallback(callback)添加回调函数---->SurfaceHolder.lockCanvas()获得Canvas对象并锁定画布----> Canvas绘画 ---->SurfaceHolder.unlockCanvasAndPost(Canvas
 canvas)结束锁定画图，并提交改变，将图形显示。
下面是SurfaceView的一个简单[测试](http://lib.csdn.net/base/softwaretest)，特别提醒，最好设置SurfaceHolder.addCallBack，重写里面的方法，然后在public void surfaceCreated(SurfaceHolder
 holder) 里面，在再SurfaceView中画图，防止SurfaceView还没有创建好，而导致的异常。
**[html]**[view
 plain](http://blog.csdn.net/lonelyroamer/article/details/7484639#)[copy](http://blog.csdn.net/lonelyroamer/article/details/7484639#)
- import java.util.Timer;  
- import java.util.TimerTask;  
- 
- import android.app.Activity;  
- import android.graphics.Canvas;  
- import android.graphics.Color;  
- import android.graphics.Paint;  
- import android.graphics.Rect;  
- import android.os.Bundle;  
- import android.util.Log;  
- import android.view.SurfaceHolder;  
- import android.view.SurfaceView;  
- 
- /**  
-  * 画一个布满屏幕的格子  
-  *   
-  */  
- public class SurfaceViewTest2Activity extends Activity {  
-     /** Called when the activity is first created. */  
-     private SurfaceView surfaceView;  
-     private SurfaceHolder holder;  
-     private int screenWidth;  
-     private int screenHeight;  
-     private Timer timer;  
-     private MyTimerTask1 task1;  
-     private float currentX = 0.0f;// 记录当前的横向绘制直线的进度  
-     private float currentY = 0.0f;// 记录当前的纵向绘制直线的进度  
-     private Paint paint;// 画笔  
- 
-     private boolean reversion = false;// 控制绘制线条的反转绘制  
-     private int countLine = 0;// 记录画的X是第几条线  
- 
-     @Override  
-     public void onCreate(Bundle savedInstanceState) {  
-         super.onCreate(savedInstanceState);  
-         setContentView(R.layout.main);  
-         // 计算屏幕的宽高  
- screenWidth = getWindowManager().getDefaultDisplay().getWidth();  
- screenHeight = getWindowManager().getDefaultDisplay().getHeight();  
-         // SurfaceView和holder的初始化  
- surfaceView = (SurfaceView) findViewById(R.id.sv);  
- holder = surfaceView.getHolder();  
-         holder.addCallback(new SurfaceCallBack());  
-         // 计时器和任务的初始化  
- timer = new Timer();  
- task1 = new MyTimerTask1();  
-         // 画笔的设置  
- paint = new Paint();  
-         paint.setColor(Color.GREEN);// 画笔为绿色...  
-         paint.setStrokeWidth(2);// 设置画笔粗细  
-     }  
- 
-     public class SurfaceCallBack implements SurfaceHolder.Callback {  
-         public void surfaceCreated(SurfaceHolder holder) {  
-             timer.schedule(task1, 1000, 5);  
-         }  
- 
-         public void surfaceChanged(SurfaceHolder holder, int format, int width,  
-                 int height) {  
- 
-         }  
- 
-         public void surfaceDestroyed(SurfaceHolder holder) {  
- 
-         }  
- 
-     }  
- 
-     class MyTimerTask1 extends TimerTask {  
-         @Override  
-         public void run() {  
-             // 开始绘制  
-             drawGrid(currentX, currentY);  
-             // 如果不反转的话，那么currentX++，否侧--  
-             if (!reversion)  
-                 currentX++;  
-             else  
-                 currentX--;  
-             // 保持绘制x和y的同步，不过不知道为什么，在模拟器上，X到顶了，而Y没有  
-             // 不过从输出上看，是同步的，可能是模拟器上的问题  
- currentY = ((float) screenHeight / screenWidth) * currentX;  
-             Log.i("test", currentX + "  " + currentY);  
-             // 如果超过屏幕，那么线条就要反转，并且线条的个数要加1  
-             if (currentX >= screenWidth) {  
- reversion = true;  
-                 countLine++;  
-             }  
-             if (currentX <= 0) {  
- reversion = false;  
-                 countLine++;  
-             }  
-         }  
-     }  
- 
-     public void drawGrid(float x, float y) {  
-         Canvas canvas = holder.lockCanvas(new Rect(0, 0, screenWidth,  
-                 screenHeight));  
- 
-         if (!reversion) {//没有反转的绘制情况  
-             canvas.drawLine(0, countLine * 15, x, countLine * 15, paint);  
-             canvas.drawLine(countLine * 15, 0, countLine * 15, y, paint);  
-         } else {//反转的绘制情况  
-             canvas.drawLine(screenWidth, countLine * 15, x, countLine * 15,  
-                     paint);  
-             canvas.drawLine(countLine * 15, screenHeight, countLine * 15, y,  
-                     paint);  
-         }  
-         holder.unlockCanvasAndPost(canvas);  
-     }  
- }  
