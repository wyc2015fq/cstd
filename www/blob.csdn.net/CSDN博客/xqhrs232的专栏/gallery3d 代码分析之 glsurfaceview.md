# gallery3d 代码分析之 glsurfaceview - xqhrs232的专栏 - CSDN博客
2011年02月20日 09:42:00[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1705标签：[代码分析																[buffer																[float																[class																[debugging																[import](https://so.csdn.net/so/search/s.do?q=import&t=blog)](https://so.csdn.net/so/search/s.do?q=debugging&t=blog)](https://so.csdn.net/so/search/s.do?q=class&t=blog)](https://so.csdn.net/so/search/s.do?q=float&t=blog)](https://so.csdn.net/so/search/s.do?q=buffer&t=blog)](https://so.csdn.net/so/search/s.do?q=代码分析&t=blog)
个人分类：[Android/Java](https://blog.csdn.net/xqhrs232/article/category/906925)
原文地址::[http://www.linuxgraphics.cn/android/gallery3d_glsurfaceview.html](http://www.linuxgraphics.cn/android/gallery3d_glsurfaceview.html)
**作者:** 刘鹏 
**日期: **2010-04-01 
本文简要介绍了如何使用　android SDK 提供的 GlSurfaceView 进行　OpenGL ES 开发。 
#### 简介
SDK 中的　android.opengl.GLSurfaceView　类提供如下功能：
- 在 OpenGL ES 和 View 系统之间建立联系； 
- 使得 OpenGL ES 可以工作在 Activity 生命周期中； 
- 可选择合适的 frame buffer 像素格式； 
- 创建并管理一个单独的渲染线程，可以实现平滑的动画； 
- 提供　debugging 工具和 API。 
#### 一个简单的 GLSurfaceView 应用
package com.example.android.apis.graphics;
import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;
import android.app.Activity;
import android.opengl.GLSurfaceView;
import android.os.Bundle;
public class ClearActivity extends Activity {
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        mGLView = new GLSurfaceView(this);
        mGLView.setRenderer(new ClearRenderer());
        setContentView(mGLView);
    }
    @Override
    protected void onPause() {
        super.onPause();
        mGLView.onPause();
    }
    @Override
    protected void onResume() {
        super.onResume();
        mGLView.onResume();
    }
    private GLSurfaceView mGLView;
}
class ClearRenderer implements GLSurfaceView.Renderer {
    public void onSurfaceCreated(GL10 gl, EGLConfig config) {
        // Do nothing special.
    }
    public void onSurfaceChanged(GL10 gl, int w, int h) {
        gl.glViewport(0, 0, w, h);
    }
    public void onDrawFrame(GL10 gl) {
        gl.glClear(GL10.GL_COLOR_BUFFER_BIT | GL10.GL_DEPTH_BUFFER_BIT);
    }
}
这个程序功能很简单，每帧绘制时将屏幕设置成黑色。但它是一个完整的工作在 Activity 生命周期中的　OpenGL 程序。当 activity 暂停时，它暂停渲染；当 activity 继续时，它继续渲染。可以将这个程序用作非交互式的 demo 程序。可以在 ClearRenderer.onDrawFrame() 接口中增加 OpenGL 调用做很多的绘制。
GLSurfaceView.Render 接口有三个方法：
- onSurfaceCreated():该方法在渲染开始前调用，OpenGL ES 的绘制上下文被重建时也会被调用。当 activity 暂停时绘制上下文会丢失，当 activity 继续时，绘制上下文会被重建。另外，创建长期存在的 OpenGL 资源(如 texture)往往也在这里进行。
- onSurfaceChanged():当 surface 的尺寸发生改变时该方法被调用。往往在这里设置 viewport。若你的 camera 是固定的，也可以在这里设置 camera。
- onDrawFrame():每帧都通过该方法进行绘制。绘制时通常先调用 glClear　函数来清空 framebuffer，然后在调用 OpenGL ES 的起它的接口进行绘制。
#### 输入如何处理
若是开发一个交互型的应用(如游戏)，通常需要子类化 GLSurfaceView，由此可以获取输入事件。下面有个例子：
package com.google.android.ClearTest;
import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;
import android.app.Activity;
import android.content.Context;
import android.opengl.GLSurfaceView;
import android.os.Bundle;
import android.view.MotionEvent;
public class ClearActivity extends Activity {
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        mGLView = new ClearGLSurfaceView(this);
        setContentView(mGLView);
    }
    @Override
    protected void onPause() {
        super.onPause();
        mGLView.onPause();
    }
    @Override
    protected void onResume() {
        super.onResume();
        mGLView.onResume();
    }
    private GLSurfaceView mGLView;
}
class ClearGLSurfaceView extends GLSurfaceView {
    public ClearGLSurfaceView(Context context) {
        super(context);
        mRenderer = new ClearRenderer();
        setRenderer(mRenderer);
    }
    public boolean onTouchEvent(final MotionEvent event) {
        queueEvent(new Runnable(){
            public void run() {
                mRenderer.setColor(event.getX() / getWidth(),
                        event.getY() / getHeight(), 1.0f);
            }});
            return true;
        }
        ClearRenderer mRenderer;
}
class ClearRenderer implements GLSurfaceView.Renderer {
    public void onSurfaceCreated(GL10 gl, EGLConfig config) {
        // Do nothing special.
    }
    public void onSurfaceChanged(GL10 gl, int w, int h) {
        gl.glViewport(0, 0, w, h);
    }
    public void onDrawFrame(GL10 gl) {
        gl.glClearColor(mRed, mGreen, mBlue, 1.0f);
        gl.glClear(GL10.GL_COLOR_BUFFER_BIT | GL10.GL_DEPTH_BUFFER_BIT);
    }
    public void setColor(float r, float g, float b) {
        mRed = r;
        mGreen = g;
        mBlue = b;
    }
    private float mRed;
    private float mGreen;
    private float mBlue;
}
这个应用在每一帧中清空屏幕。当 tap 屏幕时，改变屏幕的颜色。这里在 ClearGLSurfaceView.onTouchEvent() 中使用了 queueEvent()。queueEvent() 方法用于在 UI 线程和渲染线程间通信。也可以用起它的 Java 线程通信技术，如 synchronized 方法，但 queueEvent 最简单的线程通信方法。
#### 其他的 GLSurfaceView 例子
在 [Android](http://writeblog.csdn.net/index.html) SDK 的 API Demo例子程序中还有很多例子：
- GLSurfaceView 
- Kube 
- Translucent GLSurfaceView：透明背景 
- Textured Triangle：纹理贴图 
- Sprite Text：在 texture 上写文本并显示在 3D 场景中 
- Touch Rotate:旋转 3D 对象 
#### 选择一个 Surface
GLSurfaceView 提供了接口可选择 surface 的类型。默认情况下， GLSurfaceView 会使用一个 16 位 RGB　frame buffer，带 16 位深度。你也可以根据自己的需要进行选择，比如在　Translucent GLSurfaceView　例子里，需要一个 Alpha 通道来实现透明。GLSurfaceView 提供了 setEGLSurfaceChooser()方法来选择 surface。
选择一个 RGB (565)的16位 framebuffer,接口如下：
setEGLConfigChooser(boolean needDepth)
若要定制 red, green, blue, alpha 和 depth，则用如下接口：
setEGLConfigChooser(int redSize, int greenSize,int blueSize, int alphaSize,int depthSize, int stencilSize)
使用自己实现的 EGLConfigChooser，用如下的接口：
setEGLConfigChooser(EGLConfigChooser configChooser)
#### 持续型渲染模式 & 通知型渲染模式
大多数 3D 应用，如游戏、模拟等都是持续型渲染的动画，还有些 3D 应用是反应式的(reactive)，它们往往先被动等待，当用户有了动作再做出反应。对于这种应用，持续渲染屏幕是浪费时间。若开发反应式的应用，可以调用下面的方法
GLSurfaceView.setRenderMode(RENDERMODE_WHEN_DIRTY);
停止持续渲染。当调用
GLSurfaceView.requestRender()
时，程序再渲染屏幕。
#### 调试
GLSurfaceView.setDebugFlags() 方法可以激活 log　或者错误检测，它们可以帮助调试 OpenGL ES 调用。具体使用时，在　GLSurfaceView 的构造函数中，调用 setRender() 之前调用　GLSurfaceView.setDebugFlags()就可以了。下面是个例子：
public ClearGLSurfaceView(Context context) {
    super(context);
    // Turn on error-checking and logging
    setDebugFlags(DEBUG_CHECK_GL_ERROR | DEBUG_LOG_GL_CALLS);
    mRenderer = new ClearRenderer();
    setRenderer(mRenderer);
}
#### Reference
- [Introducing GLSurfaceView](http://developer.android.com/resources/articles/glsurfaceview.html)

