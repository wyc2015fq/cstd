# Android-SurfaceView与SurfaceHolder对象 - 建建的博客 - CSDN博客
2017年06月02日 15:00:22[纪建](https://me.csdn.net/u013898698)阅读数：126
个人分类：[Android](https://blog.csdn.net/u013898698/article/category/6941721)
调试Media播放时，不时用到SurfaceView与SurfaceHolder对象，写case测试及实际运行效果，
基本上搞清楚这两个对象的用法及区别
1、SurfaceView 
public class SurfaceView extends View
SurfaceView是视图(View)的继承类，这个视图里内嵌了一个专门用于绘制的Surface。你可以控制这个Surface的格式和尺寸。Surfaceview控制这个Surface的绘制位置。
surface是纵深排序(Z-ordered)的，这表明它总在自己所在窗口的后面。surfaceview提供了一个可见区域，只有在这个可见区域内 的surface部分内容才可见，可见区域外的部分不可见。surface的排版显示受到视图层级关系的影响，它的兄弟视图结点会在顶端显示。这意味者 surface的内容会被它的兄弟视图遮挡，这一特性可以用来放置遮盖物(overlays)(例如，文本和按钮等控件)。注意，如果surface上面 有透明控件，那么它的每次变化都会引起框架重新计算它和顶层控件的透明效果，这会影响性能。你可以通过SurfaceHolder接口访问这个Surface.用getHolder()方法可以得到这个接口。
surfaceview变得可见时，surface被创建；surfaceview隐藏前，surface被销毁。这样能节省资源。
如果你要查看 surface被创建和销毁的时机，可以重载surfaceCreated(SurfaceHolder)和 surfaceDestroyed(SurfaceHolder)。
surfaceview的核心在于提供了两个线程：UI线程和渲染线程。
这里应注意：
1> 所有SurfaceView和SurfaceHolder.Callback的方法都应该在UI线程里调用，一般来说就是应用程序主线程。
   渲染线程所要访问的各种变量应该作同步处理。
2> 由于surface可能被销毁，它只在SurfaceHolder.Callback.surfaceCreated()和 SurfaceHolder.Callback.surfaceDestroyed()之间有效，所以要确保渲染线程访问的是合法有效的surface。
可以在主线程之外的线程中向屏幕绘图，这样可以避免画图任务繁重的时候造成主线程阻塞，从而提高了程序的反应速度。
在游戏开发中多用到SurfaceView，游戏中的背景、人物、动画等等尽量在画布canvas中画出。
2、SurfaceHolder
public interface SurfaceHolder
显示一个surface的抽象接口，使你可以控制surface的大小和格式， 以及在surface上编辑像素，和监视surace的改变。
这个接口通常通过SurfaceView类实现。
surface的控制器，用来操纵surface，处理它的Canvas上画的效果和动画，控制表面，大小，像素等。
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
3、SurfaceHolder.Callback
用户可以实现此接口接收surface变化的消息。当用在一个SurfaceView 中时， 它只在SurfaceHolder.Callback.surfaceCreated()和SurfaceHolder.Callback.surfaceDestroyed()之间有效。设置Callback的方法是SurfaceHolder.addCallback.
实现上一般继承SurfaceView并实现SurfaceHolder.Callback接口
下面我们举个例子说明一下这几个对象的用法：
**[java]**[view
 plain](http://blog.csdn.net/andyhuabing/article/details/7657069#)[copy](http://blog.csdn.net/andyhuabing/article/details/7657069#)
[print](http://blog.csdn.net/andyhuabing/article/details/7657069#)[?](http://blog.csdn.net/andyhuabing/article/details/7657069#)
- <span style="font-size:16px;">package com.test.surfaceview;  
- 
- import android.app.Activity;  
- import android.content.Context;  
- import android.graphics.Canvas;  
- import android.graphics.Color;  
- import android.graphics.Paint;  
- import android.graphics.Rect;  
- import android.os.Bundle;  
- import android.util.Log;  
- import android.view.SurfaceHolder;  
- import android.view.SurfaceView;  
- 
- publicclass TestsurfaceviewActivity extends Activity {  
- privatefinalstatic String TAG = "TestsurfaceviewActivity";  
- 
- /** Called when the activity is first created. */
- @Override
- publicvoid onCreate(Bundle savedInstanceState) {  
- super.onCreate(savedInstanceState);  
- //setContentView(R.layout.main);
-         setContentView(new MySurfaceView(this)); // 这里以MySurfaceView作为显示View
-     }  
- 
- class MySurfaceView extends SurfaceView implements SurfaceHolder.Callback{  
- private SurfaceHolder holder;  
- private MyThread mThread ;  
- 
- public MySurfaceView(Context context){  
- super(context);  
-             holder = this.getHolder(); //获取holder对象
-             holder.addCallback(this); // 添加surface回调函数
-             mThread = new MyThread(holder); //创建一个绘图线程
-         }  
- 
- @Override
- publicvoid surfaceChanged(SurfaceHolder holder, int format, int width,  
- int height) {  
- // TODO Auto-generated method stub
-             Log.i(TAG,"surfaceChanged is called");  
-         }  
- 
- @Override
- publicvoid surfaceCreated(SurfaceHolder holder) {  
- // TODO Auto-generated method stub
-             Log.i(TAG,"surfaceCreated is called");  
- 
-             mThread.isRun = true;  
-             mThread.start();  
-         }  
- 
- @Override
- publicvoid surfaceDestroyed(SurfaceHolder holder) {  
- // TODO Auto-generated method stub
-             Log.i(TAG,"surfaceDestroyed is called");  
- 
-             mThread.isRun = false;  
-             mThread.stop();  
-         }  
-     }  
- 
- class MyThread extends Thread{  
- private SurfaceHolder holder ;  
- publicboolean isRun = false;  
- 
- public MyThread(SurfaceHolder holder){  
- this.holder = holder;  
-             isRun = true;  
-             Log.i(TAG,"MyThread set surface holder");  
-         }  
- 
- @Override
- publicvoid run(){  
-             Canvas canvas = null;  
- int count = 0;  
- while (isRun) {  
- try {  
- synchronized (holder) {  
-                         canvas = holder.lockCanvas();// 锁定画布，一般在锁定后就可以通过其返回的画布对象Canvas，在其上面画图等操作了。
-                         canvas.drawColor(Color.BLACK);// 设置画布背景颜色
-                         Paint p = new Paint(); // 创建画笔
-                         p.setColor(Color.RED);  
-                         Rect r = new Rect(500, 200, 300, 250);  
-                         canvas.drawRect(r, p);  
-                         canvas.drawText("这是第" + (count++) + "秒", 300, 310, p);  
-                         Thread.sleep(1000);// 睡眠时间为1秒
-                     }  
-                 } catch (Exception e) {  
-                     e.printStackTrace();  
-                 } finally {  
- if (canvas != null) {  
-                         holder.unlockCanvasAndPost(canvas);// 结束锁定画图，并提交改变。
-                     }  
-                 }  
-             }  
-         }  
-     }  
- }</span>  
在Media 播放过程中会需要用到两个SurfaceView,一个用于绘制显示界面，另外一个用于播放视频的显示
首先在main.xml中定义两个SurfaceView:
<SurfaceView
[Android](http://lib.csdn.net/base/android):id="@+id/playSurface"
        android:layout_width="fill_parent"
        android:layout_height="fill_parent" />
<SurfaceView
        android:id="@+id/mainSurface"
        android:layout_width="fill_parent"
        android:layout_height="fill_parent" />
然后使用的片断代码如下：
**[java]**[view
 plain](http://blog.csdn.net/andyhuabing/article/details/7657069#)[copy](http://blog.csdn.net/andyhuabing/article/details/7657069#)
[print](http://blog.csdn.net/andyhuabing/article/details/7657069#)[?](http://blog.csdn.net/andyhuabing/article/details/7657069#)
- private SurfaceView   mUIView;  
- private SurfaceView   mPlayView;  
- 
- private MyMediaplayerManager mPlayManager;  
- 
- mUIView = (SurfaceView)findViewById(R.id.mainSurface);  
- mSurfaceHolder.addCallback(new SurfaceHolder.Callback() {         
- @Override
- publicvoid surfaceDestroyed(SurfaceHolder holder) {  
- // TODO Auto-generated method stub              
-     }  
- 
- @Override
- publicvoid surfaceCreated(final SurfaceHolder holder) {  
- // TODO Auto-generated method stub              
-     }  
- @Override
- publicvoid surfaceChanged(SurfaceHolder holder, int format,  
- int width, int height) {  
- // TODO Auto-generated method stub              
-     }  
- });   
- 
- mPlayView = (SurfaceView)findViewById(R.id.playSurface);  
- holder.setType(SurfaceHolder.SURFACE_TYPE_PUSH_BUFFERS);  
- holder.addCallback(new SurfaceHolder.Callback() {  
- @Override
- publicvoid surfaceDestroyed(SurfaceHolder holder) {  
- // TODO Auto-generated method stub
-     }  
- @Override
- publicvoid surfaceCreated(SurfaceHolder holder) {  
-         mPlayManager.setDisplay(holder); // 这里设置video视频的显示Holder
-     }  
- 
- @Override
- publicvoid surfaceChanged(SurfaceHolder holder, int format, int width,  
- int height) {  
- // TODO Auto-generated method stub              
-     }  
- });  
- 
- // 设定SurfaceView的显示zorder序
- mUIView.setZOrderMediaOverlay(true);  
- mPlayView.setZOrderMediaOverlay(false);  
view 可看作就是一个图层，以上使用两个图层，一个是图形层，另一个是视频层，需要播放视频时只需要将图形层透明掉即可。
