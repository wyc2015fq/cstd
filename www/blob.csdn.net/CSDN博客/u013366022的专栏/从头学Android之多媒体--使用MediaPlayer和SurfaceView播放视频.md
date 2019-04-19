# 从头学Android之多媒体--使用MediaPlayer和SurfaceView播放视频 - u013366022的专栏 - CSDN博客
2014年08月13日 14:27:56[slitaz](https://me.csdn.net/u013366022)阅读数：548
**应用场景：**
使用VideoView播放视频简单、方便，但除此之外，我们还可以利用MediaPlayer用来播放视频。但是我们知道MediaPlayer主要用于播放音频，它是没有提供输出图像的输出界面，这时我们就用到了SurfaceView控件，将它与MediaPlayer结合起来，就能达到了视频的输出了。首先来了SurfaceView这个控件类
**SurfaceView类**
**![](http://hi.csdn.net/attachment/201112/28/0_132504315473j7.gif)**
**构造方法**
|**方法名称**|**描述**|
|----|----|
|**public SurfaceView(Context context)**|通过Context创建SurfaceView对象|
|**public SurfaceView(Context context, AttributeSet attrs)**|通过Context对象和AttributeSet创建SurfaceView对象|
|**public SurfaceView(Context context, AttributeSet attrs, int defStyle)**|通过Context对象和AttributeSet创建并可以指定样式，SurfaceView对象|
**常用方法**
|**方法名称**|**描述**|
|----|----|
|**public SurfaceHolder getHolder ()**|得到SurfaceHolder对象用于管理SurfaceView|
|**public void setVisibility (int visibility)**|设置是否可见，其值可以是VISIBLE, INVISIBLE, GONE.|
**SurfaceHolder**
**![](http://hi.csdn.net/attachment/201112/28/0_1325043170eY0j.gif)**
它是一个接口，用于管理SurfaceView。里面有两个常用的内部接口SurfaceHolder.Callback,SurfaceHolder.Callback2而Callback2是实现于Callback的
**常用方法**
|**方法名称**|**描述**|
|----|----|
|**public abstract void addCallback (SurfaceHolder.Callback callback)**|添加一个Callback对象监听SurfaceView的变化|
|**public abstract void removeCallback (SurfaceHolder.Callback callback)**|移除Callback|
|**public abstract void setType (int type)**|设置SurfaceView的控制方式|
|**public abstract Canvas lockCanvas ()**|锁定整个SurfaceView对象，获取该Surface上的Canvas|
|**public abstract Canvas lockCanvas (Rect dirty)**|锁定SurfaceView上Rect划分的区域，获取该Surface上的Canvas|
|**public abstract void unlockCanvasAndPost (Canvas canvas)**|调用该方法，之前所绘制的图形还处于缓冲之中，下一次的lockCanvas()方法锁定的区域可能会“遮挡”它|
**SurfaceHolder.CallBack**
**![](http://hi.csdn.net/attachment/201112/28/0_1325043205x44x.gif)**
在Callback里有三个抽象方法
|**方法名称**|**描述**|
|----|----|
|**public abstract void surfaceChanged (SurfaceHolder holder, int format, int width, int height)**|SurfaceView改变时触发|
|**public abstract void surfaceCreated (SurfaceHolder holder)**|SurfaceView创建时触发|
|**public abstract void surfaceDestroyed (SurfaceHolder holder)**|SurfaceView销毁时触发|
**如何理解这几个类或者接口之间的关系？**
这样理解：
SurfaceView它用于显示，SurfaceHolder就是用于用来管理这个显示的SurfaceView对象的，但在SurfaceHolder是怎么样去管理这个对象的呢？这就用到了SurfceHolder.addCallback()方法添加一个SurfaceHolder接口的内部接口的三个抽象方法用于管理或者说是用于监听SurfaceView。这样就达到了管理SurfaceView的目的。
**实例：使用MediaPlayer与SurfaceView播放视频**
**实现效果：**
**![](http://hi.csdn.net/attachment/201112/28/0_1325043305u0RU.gif)**
**实现步骤：**
1)       创建MediaPlayer对象，并设置加载的视频文件(setDataSource())
2)       在界面布局文件中定义SurfaceView控件
3)       通过MediaPlayer.setDisplay(SurfaceHolder sh)来指定视频画面输出到SurfaceView之上
4)       通过MediaPlayer的其他一些方法用于播放视频
**代码实现：**
**布局文件：main.xml**
**[html]**[view
 plain](http://blog.csdn.net/worker90/article/details/7108358#)[copy](http://blog.csdn.net/worker90/article/details/7108358#)[print](http://blog.csdn.net/worker90/article/details/7108358#)[?](http://blog.csdn.net/worker90/article/details/7108358#)
- <?xmlversion="1.0"encoding="utf-8"?>
- <LinearLayoutxmlns:android="http://schemas.android.com/apk/res/android"
- android:layout_width="fill_parent"
- android:layout_height="fill_parent"
- android:orientation="vertical">
- 
- <SurfaceView
- android:id="@+id/surfaceView"
- android:layout_width="fill_parent"
- android:layout_height="360px"/>
- 
- <LinearLayout
- android:layout_width="fill_parent"
- android:layout_height="wrap_content"
- android:gravity="center_horizontal"
- android:orientation="horizontal">
- 
- <ImageButton
- android:id="@+id/btnplay"
- android:layout_width="wrap_content"
- android:layout_height="wrap_content"
- android:src="@drawable/play"/>
- 
- <ImageButton
- android:id="@+id/btnpause"
- android:layout_width="wrap_content"
- android:layout_height="wrap_content"
- android:src="@drawable/pause"/>
- 
- <ImageButton
- android:id="@+id/btnstop"
- android:layout_width="wrap_content"
- android:layout_height="wrap_content"
- android:src="@drawable/stop"/>
- </LinearLayout>
- 
- </LinearLayout>
**Activity代码**
**[java]**[view
 plain](http://blog.csdn.net/worker90/article/details/7108358#)[copy](http://blog.csdn.net/worker90/article/details/7108358#)[print](http://blog.csdn.net/worker90/article/details/7108358#)[?](http://blog.csdn.net/worker90/article/details/7108358#)
- package com.jiahui.surfaceview;  
- 
- import android.app.Activity;  
- import android.media.AudioManager;  
- import android.media.MediaPlayer;  
- import android.os.Bundle;  
- import android.view.SurfaceHolder;  
- import android.view.SurfaceHolder.Callback;  
- import android.view.SurfaceView;  
- import android.view.View;  
- import android.view.View.OnClickListener;  
- import android.widget.Button;  
- import android.widget.ImageButton;  
- 
- publicclass SurfaceViewVideoDemoActivity extends Activity  implements OnClickListener{  
- 
-     ImageButton btnplay, btnstop, btnpause;  
-     SurfaceView surfaceView;  
-     MediaPlayer mediaPlayer;  
- int position;  
- 
- publicvoid onCreate(Bundle savedInstanceState) {  
- super.onCreate(savedInstanceState);  
-         setContentView(R.layout.main);    
-         btnplay=(ImageButton)this.findViewById(R.id.btnplay);  
-         btnstop=(ImageButton)this.findViewById(R.id.btnplay);  
-         btnpause=(ImageButton)this.findViewById(R.id.btnplay);  
- 
-         btnstop.setOnClickListener(this);  
-         btnplay.setOnClickListener(this);  
-         btnpause.setOnClickListener(this);  
- 
-         mediaPlayer=new MediaPlayer();  
-         surfaceView=(SurfaceView) this.findViewById(R.id.surfaceView);  
- 
- //设置SurfaceView自己不管理的缓冲区
-         surfaceView.getHolder().setType(SurfaceHolder.SURFACE_TYPE_PUSH_BUFFERS);         
-         surfaceView.getHolder().addCallback(new Callback() {          
- @Override
- publicvoid surfaceDestroyed(SurfaceHolder holder) {  
- 
-             }  
- 
- @Override
- publicvoid surfaceCreated(SurfaceHolder holder) {  
- if (position>0) {  
- try {  
- //开始播放
-                         play();  
- //并直接从指定位置开始播放
-                         mediaPlayer.seekTo(position);  
-                         position=0;                       
-                     } catch (Exception e) {  
- // TODO: handle exception
-                     }  
-                 }  
-             }             
- @Override
- publicvoid surfaceChanged(SurfaceHolder holder, int format, int width,  
- int height) {  
- 
-             }  
-         });       
-     }  
- @Override
- publicvoid onClick(View v) {     
- switch (v.getId()) {  
- case R.id.btnplay:  
-             play();  
- break;  
- 
- case R.id.btnpause:  
- if (mediaPlayer.isPlaying()) {  
-                 mediaPlayer.pause();  
-             }else{  
-                 mediaPlayer.start();  
-             }  
- break;  
- 
- case R.id.btnstop:  
- if (mediaPlayer.isPlaying()) {  
-                 mediaPlayer.stop();  
-             }  
- 
- break;  
- default:  
- break;  
-         }  
- 
-     }  
- @Override
- protectedvoid onPause() {    
- //先判断是否正在播放
- if (mediaPlayer.isPlaying()) {  
- //如果正在播放我们就先保存这个播放位置
-             position=mediaPlayer.getCurrentPosition()  
-                     ;  
-             mediaPlayer.stop();  
-         }  
- super.onPause();  
-     }  
- 
- privatevoid play() {  
- try {  
-             mediaPlayer.reset();  
-             mediaPlayer  
-             .setAudioStreamType(AudioManager.STREAM_MUSIC);  
- //设置需要播放的视频
-             mediaPlayer.setDataSource("/mnt/sdcard/movie.3gp");  
- //把视频画面输出到SurfaceView
-             mediaPlayer.setDisplay(surfaceView.getHolder());  
-             mediaPlayer.prepare();  
- //播放
-             mediaPlayer.start();          
-         } catch (Exception e) {  
- // TODO: handle exception
-         }  
-     }  
- }  
 如需转载引用请注明出处：http://blog.csdn.net/jiahui524
