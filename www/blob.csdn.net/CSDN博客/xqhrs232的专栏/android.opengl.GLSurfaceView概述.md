# android.opengl.GLSurfaceView概述 - xqhrs232的专栏 - CSDN博客
2011年02月20日 09:26:00[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：11795
原文地址::[http://120.132.134.205/cmdn/supesite/?uid-5358-action-viewspace-itemid-6527](http://120.132.134.205/cmdn/supesite/?uid-5358-action-viewspace-itemid-6527)
 GLSurfaceView是一个视图，继承至SurfaceView，它内嵌的surface专门负责OpenGL渲染。
        GLSurfaceView提供了下列特性：
                1> 管理一个surface，这个surface就是一块特殊的内存，能直接排版到android的视图view上。
                2> 管理一个EGL display，它能让opengl把内容渲染到上述的surface上。
                3> 用户自定义渲染器(render)。
                4> 让渲染器在独立的线程里运作，和UI线程分离。
                5> 支持按需渲染(on-demand)和连续渲染(continuous)。
                6> 一些可选工具，如调试。
**使用GLSurfaceView**
        通常会继承GLSurfaceView，并重载一些和用户输入事件有关的方法。如果你不需要重载事件方法，GLSurfaceView也可以直接使用，你可以使用set方法来为该类提供自定义的行为。例如，GLSurfaceView的渲染被委托给渲染器在独立的渲染线程里进行，这一点和普通视图不一样，setRenderer(Renderer)设置渲染器。
**初始化GLSurfaceView**
        初始化过程其实仅需要你使用setRenderer(Renderer)设置一个渲染器(render)。当然，你也可以修改GLSurfaceView一些默认配置。
            * setDebugFlags(int)
            * setEGLConfigChooser(boolean)
            * setEGLConfigChooser(EGLConfigChooser)
            * setEGLConfigChooser(int, int, int, int, int, int)
            * setGLWrapper(GLWrapper) 
**定制android.view.Surface**
        GLSurfaceView默认会创建像素格式为PixelFormat.RGB_565的surface。如果需要透明效果，调用getHolder().setFormat(PixelFormat.TRANSLUCENT)。透明(TRANSLUCENT)的surface的像素格式都是32位，每个色彩单元都是8位深度，像素格式是设备相关的，这意味着它可能是ARGB、RGBA或其它。
**选择EGL配置**
        Android设备往往支持多种EGL配置，可以使用不同数目的通道(channel)，也可以指定每个通道具有不同数目的位(bits)深度。因此，在渲染器工作之前就应该指定EGL的配置。GLSurfaceView默认EGL配置的像素格式为RGB_656，16位的深度缓存(depth buffer)，默认不开启遮罩缓存(stencil buffer)。
        如果你要选择不同的EGL配置，请使用setEGLConfigChooser方法中的一种。
**调试行为**
        你可以调用调试方法setDebugFlags(int)或setGLWrapper(GLSurfaceView.GLWrapper)来自定义GLSurfaceView一些行为。在setRenderer方法之前或之后都可以调用调试方法，不过最好是在之前调用，这样它们能立即生效。
**设置渲染器**
        总之，你必须调用setRenderer(GLSurfaceView.Renderer)来注册一个GLSurfaceView.Renderer渲染器。渲染器负责真正的GL渲染工作。
**渲染模式**
        渲染器设定之后，你可以使用setRenderMode(int)指定渲染模式是按需(on demand)还是连续(continuous)。默认是连续渲染。
**Activity生命周期**
        Activity窗口暂停(pause)或恢复(resume)时，GLSurfaceView都会收到通知，此时它的onPause方法和onResume方法应该被调用。这样做是为了让GLSurfaceView暂停或恢复它的渲染线程，以便它及时释放或重建OpenGL的资源。
**事件处理**
        为了处理事件，一般都是继承GLSurfaceView类并重载它的事件方法。但是由于GLSurfaceView是多线程操作，所以需要一些特殊的处理。由于渲染器在独立的渲染线程里，你应该使用Java的跨线程机制跟渲染器通讯。queueEvent(Runnable)方法就是一种相对简单的操作，例如下面的例子。
         class MyGLSurfaceView extends GLSurfaceView {
             private MyRenderer mMyRenderer;
             public void start() {
                 mMyRenderer = ...;
                 setRenderer(mMyRenderer);
             }
             public boolean onKeyDown(int keyCode, KeyEvent event) {
                 if (keyCode == KeyEvent.KEYCODE_DPAD_CENTER) {
                     queueEvent(new Runnable() {
                         // 这个方法会在渲染线程里被调用
                         public void run() {
                             mMyRenderer.handleDpadCenter();
                         }});
                     return true;
                 }
                 return super.onKeyDown(keyCode, event);
             }
         }
        (注：如果在UI线程里调用渲染器的方法，很容易收到“call to OpenGL ES API with no current context”的警告，典型的误区就是在键盘或鼠标事件方法里直接调用opengl es的API，因为UI事件和渲染绘制在不同的线程里。更甚者，这种情况下调用glDeleteBuffers这种释放资源的方法，可能引起程序的崩溃，因为UI线程想释放它，渲染线程却要使用它。)
