# Android下基于OpenGL的程序会发生纹理丢失的原因 - xqhrs232的专栏 - CSDN博客
2011年06月15日 17:11:00[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：3622
 相关网帖
  1。[Workaround to losing the OpenGL context when Android pauses?](http://writeblog.csdn.net/questions/12629/workaround-to-losing-the-opengl-context-when-android-pauses)
[http://gamedev.stackexchange.com/questions/12629/workaround-to-losing-the-opengl-context-when-android-pauses](http://gamedev.stackexchange.com/questions/12629/workaround-to-losing-the-opengl-context-when-android-pauses)
 2。**Problems with EGL_CONTEXT_LOST**
[http://code.google.com/p/android/issues/detail?id=5064](http://code.google.com/p/android/issues/detail?id=5064)
|||
3。[EGL Context Preservation on Android](http://cjix.info/blog/g3d/egl-context-preservation-on-android/)
[http://cjix.info/blog/g3d/egl-context-preservation-on-android/](http://cjix.info/blog/g3d/egl-context-preservation-on-android/)
  一般基于GDI的Android应用程序在进入OnPause( )后，如果应用程序不被系统回收机制回收的话，这个APK用到的图片资源之类的还是存在于内存中的，所以这个APK返回前台进入OnReStart( )后图片资源不需要重新去加载并可以马上切回前台显示。
    最近为了加快自己基于OpenGL的应用程序与导航软件前后台来回切换的速度，也考虑用基于GDI的Android应用程序对资源的处理方式来处理，也就是图片资源（纹理）不是每次都去加载，想当然地认为基于OpenGL的应用程序跟基于GDI的应用程序一样，从后台切到前台，图片资源（纹理）还存在内存（应用程序还没有被系统的回收机制所杀掉），就把过去的那些图片资源（纹理）直接拿过来用，结果显示出来的应用程序却是白茫茫的一片，基于OpenGL的应用程序并没有像基于GDI的应用程序一样在内存里面保留原来的那些图片资源（纹理），也就有了Android下基于OpenGL的程序会发生纹理丢失的现象。
    后来看到Android下的SDK对所提供OpenGL接口的一些描述，才有点明白，觉得可能就是SDK中提到的原因，但不是十分肯定！
public static interface
# GLSurfaceView.Renderer
#### public abstract void onSurfaceCreated([GL10](reference/javax/microedition/khronos/opengles/GL10.html) gl,[EGLConfig](reference/javax/microedition/khronos/egl/EGLConfig.html) config)
Since: [API Level 3](guide/appendix/api-levels.html#level3)
Called when the surface is created or recreated. 
Called when the rendering thread starts and whenever the EGL context is lost. The EGL context will typically be lost when the Android device awakes after going to sleep.
Since this method is called at the beginning of rendering, as well as every time the EGL context is lost, this method is a convenient place to put code to create resources that need to be created when the rendering starts, and that need to be recreated when
 the EGL context is lost. Textures are an example of a resource that you might want to create here.
Note that when the EGL context is lost, all OpenGL resources associated with that context will be automatically deleted. You do not need to call the corresponding "glDelete" methods such as glDeleteTextures to manually delete these lost resources.
##### Parameters
configthe EGLConfig of the created surface. Can be used to create matching pbuffers.
