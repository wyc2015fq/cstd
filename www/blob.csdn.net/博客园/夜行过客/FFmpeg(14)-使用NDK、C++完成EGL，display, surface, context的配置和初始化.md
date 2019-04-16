# FFmpeg(14)-使用NDK、C++完成EGL，display, surface, context的配置和初始化 - 夜行过客 - 博客园







# [FFmpeg(14)-使用NDK、C++完成EGL，display, surface, context的配置和初始化](https://www.cnblogs.com/yongdaimi/p/9860322.html)





** EGL**

![](https://img2018.cnblogs.com/blog/653161/201810/653161-20181027100940881-1309883902.png)

用它开发需要做哪些事情。



DIsplay 与原生窗口建立链接。EGL在Android中可以用java调，也可以用C++调。

       EGLDisplay eglGetDisplay                                // 获取原生窗口的链接

       EGLBoolean eglInitlize                                     // 再进行窗口的初始化。



Surface配置和创建Surface。创建一个渲染区域，它是最终我们用来显示的对象。

EGLBoolean eglChooseConfig                                // 对渲染区域进行参数的配置。比方说颜色的深度、红色、绿色对应的深度、颜色的存储格式等

EGLSurface eglCreateWindowSurface                      // 创建surface

经过上面两步操作后，会创建一个surface，我们将来的画面就会渲染到这块surface当中去。



Context 创建渲染环境。该渲染环境主要用于描述OpenGL ES的所有项目运行需要的数据结构(如用到的顶点数据怎么绘制)。包括顶点、片断着色器、顶点数据矩阵。

eglCreateConrtext

eglMakeCurrent



**GLSL**

具体的显示YUV数据。又称之为shade,着色器语言。它主要包含下列两个模块：**顶点着色器**和**片元着色器。**

**顶点着色器**是针对每个顶点执行一次。比方说要画一个三维的模型，这个时候就需要把每个顶点给计算出来，然后跟着光照的改变而改变（因为虽然三个三点的坐标是固定的，但是视角不同，这三个值就会不一样，这个时候就需要确定一个顶点着色器，把顶点的值给算出来，绘制视频则无此问题，只要绘制一个矩形就可以了）。

**片元着色器**是针对每个显示的片元(像素点)执行一次，执行的时候会调用相应的代码，这个时候就可以把每个像素相应的颜色给改变掉(比方说传递的是YUV的数据，但是YUV的数据是不能直接用来显示的，这个时候就可以在片元着色器里面把它的值进行转换，转换成RGB的值用于显示)。

GLSL的基本语法与C相同。

它支持向量和矩阵的操作。

GLSL提供了大量的内置函数来提供丰富的扩展功能。

同时GLSL还通过一些限定符操作来管理输入输出的。



**显示YUV代码演示**
- 准备YUV数据。通过FFmpeg工具生成一个YUV文件

ffmpeg –i 720.mp4 –pix_fmt yuv420p –s 424*240 out.yuv

把生成的YUV数据上传到模拟器

PS：需要下载FFmpeg.exe工具。

yuv420p,

p是平面格式(平面格式就是针对一幅图像,，Y存在第一位，再存U，再存V，它们是非交错的格式，就是Y全部存完了，再存U，再存V)。

如果是交错格式的话可能就是一个Y，一个U，一个V，这种存放方式。

示例代码：

需要包含库文件

```
target_link_libraries( # Specifies the target library.
                       native-lib
                       GLESv2
                       EGL
                       android

                       # Links the target library to the log library
                       # included in the NDK.
                       ${log-lib} )
```

C++代码

```
extern "C"
JNIEXPORT void JNICALL
Java_com_yuneec_testopengles_Xplay_open(JNIEnv *env, jobject instance, jstring url_,
                                        jobject surface) {
    const char *url = env->GetStringUTFChars(url_, 0);

    // 获取原始窗口
    ANativeWindow *nwin = ANativeWindow_fromSurface(env, surface);
    // EGL
    // 1. EGL display 的创建和初始化
    EGLDisplay display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    if (display == EGL_NO_DISPLAY) {
        LOGE("eglGetDisplay() failed");
        return;
    }
    if (EGL_TRUE != eglInitialize(display, 0, 0)) {
        LOGE("eglInitialize() failed");
        return;
    }
    // 2. Surface
    // surface 窗口配置
    // 输出配置
    EGLConfig config;
    EGLint configNum;
    EGLint configSpec[] = {
            EGL_RED_SIZE, 8,
            EGL_GREEN_SIZE, 8,
            EGL_BLUE_SIZE, 8,
            EGL_SURFACE_TYPE, EGL_WINDOW_BIT, EGL_NONE
    };
    if (EGL_TRUE != eglChooseConfig(display, configSpec, &config, 1, &configNum)) {
        LOGE("eglChooseConfig() failed");
        return;
    }
    // 创建Surface
    EGLSurface winSurface = eglCreateWindowSurface(display, config, nwin, 0);
    if (winSurface == EGL_NO_SURFACE) {
        LOGE("eglCreateWindowSurface() failed");
        return;
    }

    //3. context 创建关联的上下文
    const EGLint ctxAttr[] = {
            EGL_CONTEXT_CLIENT_VERSION,2,EGL_NONE
    };
    EGLContext context = eglCreateContext(display, config, EGL_NO_CONTEXT, ctxAttr);
    if (context == EGL_NO_CONTEXT) {
        LOGE("eglCreateContext failed");
        return;
    }
    if (EGL_TRUE != eglMakeCurrent(display, winSurface, winSurface, context)) {
        LOGE("eglMakeCurrent failed");
        return;
    }

    LOGI("EGL init success");

    env->ReleaseStringUTFChars(url_, url);
}
```














