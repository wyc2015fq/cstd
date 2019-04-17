# OpenGL ES 3.0（三）EGL 概述 - 我的学习笔记 - CSDN博客





2018年08月11日 21:53:22[zouzhiheng](https://me.csdn.net/u011330638)阅读数：196








OpenGL ES 定义了一个渲染图形的规范，但没有定义窗口系统。为了让 GLES 能够适合各种平台，GLES 将与知道如何通过操作系统创建和访问窗口的库，即 EGL 结合使用。在使用 GLES 进行任何操作之前，必须先创建一个 OpenGL 上下文，这是通过 EGL 实现的。

EGL 提供了以下机制： 

    1) 和设备的本地窗口系统通信 

    2) 查询绘图表面的可用类型及配置 

    3) 在 OpenGL ES 3.0 及其它图形渲染 API 之间同步渲染 

    4) 管理渲染资源，如纹理贴图
# 与窗口系统通信

EGL 在 OpenGL ES 3.0（和其他 Khronos 图形API）和运行在你的计算机上的本地窗口系统之间提供了一个“粘合”层。在 EGL 确定哪些类型的绘图表面或底层系统的任何其他特性可用之前，它需要与窗口系统建立一个通信通道。

因为每个窗口系统都有不同的语义，EGL 提供了一个基本的不透明类型 EGLDisplay，它封装了所有系统的依赖关系，以便与本地窗口系统进行交互。任何使用 EGL 的应用程序需要执行的第一个操作是创建并初始化一个与本地 EGL dispaly 的连接：

```
EGLint majorVersion;
EGLint minorVersion;

EGLDisplay display = eglGetDisplay ( EGL_DEFAULT_DISPLAY );
if ( display == EGL_NO_DISPLAY ) {
    // 无法打开与本地窗口系统的连接
}

if ( !eglInitialize ( display, &majorVersion, &minorVersion ) ) {
    // 无法初始化 EGL
}
```

# 检查错误

EGL 的大多数函数都会返回 EGL_TRUE 或 EGL_FALSE，但如果调用失败，它会记录一个表示错误原因的错误码，然而，这个错误码不会直接返回给开发者，开发者需要调用以下函数查询：

```
// 返回在特定线程中最近一次 EGL 函数调用产生的错误码
EGLint eglGetError()
```

# 确定可用的 surface 配置

EGL 初始化成功之后，我们就可以确定哪些类型和配置的渲染表面可用，通常有两种方法： 

    1) 查询每一个 surface 的配置，并找到一个最好的选择 

    2) 指定一组要求，让 EGL 推荐一个最佳匹配
两种方式都会返回一个 EGLConfig，它是一个 EGL 内部数据结构的标识符，包含特定 surface 及其特征的信息（比如每一个颜色分量的位数）。

如果是第一种方式，则使用：

```
EGLBoolean eglGetConfigs(EGLDisplay display,
                         EGLConfig *configs,
                         EGLint maxReturnConfigs,
                         EGLint *numConfigs)
```

调用这个函数的方式有两种： 

    1) configs 设为 NULL，numConfigs 会返回可用的 EGLConfig 的数量 

    2) 分配一组未初始化的 EGLConfig，并设置 maxReturnConfigs 为数组的长度，调用完成后，numConfigs 会更新为被更改的 configs 的数量
如果想让 EGL 选择匹配的 EGLConfig，则使用 eglChooseConfig：

```
EGLBoolean eglChooseConfig(EGLDisplay display,
                           const EGLint *attribList,
                           EGLConfig *configs,
                           EGLint maxReturnConfigs,
                           EGLint *numConfigs)
```

将 attribList 设置为你认为重要的首选属性（如果 attribList 不合法，则错误码为 EGL_BAD_ATTRIBUTE），比如：

```
EGLint attribList[] = {
    EGL_RENDERABLE_TYPE, EGL_OPENGL_ES3_BIT_KHR,
    EGL_RED_SIZE, 5,
    EGL_GREEN_SIZE, 6,
    EGL_BLUE_SIZE, 5,
    EGL_DEPTH_SIZE, 1,
    EGL_NONE
};
```

代码示例：

```
const EGLint MaxConfigs = 10;
EGLConfig configs[MaxConfigs]; // We'll accept only 10 configs
EGLint numConfigs;

if ( !eglChooseConfig( display, attribList, configs, MaxConfigs, &numConfigs ) ) {
    // Something did not work ... handle error situation
} else {
    // Everything is okay; continue to create a rendering surface
}
```

如果执行成功，则返回一组匹配的 EGLConfig，内部按相关优先级排序（具体可查看 [eglChooseConfig](https://www.khronos.org/registry/EGL/sdk/docs/man/html/eglChooseConfig.xhtml)）。

# 创建 EGL Window

获取到适合的 EGLConfig 之后，就可以创建窗口了：

```
EGLSurface eglCreateWindowSurface(EGLDisplay display,
                                  EGLConfig config,
                                  EGLNativeWindowType window,
                                  const EGLint *attribList)
```

参数 window 是一个本地窗口，创建本地窗口根据平台不同而不同，可参考平台自身的 API。

attribList 可以为空，传 NULL 或第一个元素为 EGL_NONE 的 list，所有相关属性都会使用默认值

# 创建渲染上下文（rendering context）

EGLContext 是 OpenGL 3.0 中内部的数据结构，包含了操作所需的所有状态信息：

```
EGLContext eglCreateContext(EGLDisplay display,
                            EGLConfig config,
                            EGLContext shareContext,
                            const EGLint *attribList)
```

函数中的的 attribList 只接受一个属性：EGL_CONTEXT_CLIENT_VERSION。

之后需要把 EGLContext 和 EGLSurface 关联起来：

```bash
EGLBoolean eglMakeCurrent(EGLDisplay display,
                          EGLSurface draw,
                          EGLSurface read,
                          EGLContext context)
```

这一步完成之后，就能在当前线程中执行 OpenGL 操作了。

# 代码示例

以 Android 平台为例，使用 EGL 搭建 OpenGL 上下文需要传一个 Surface 用于获取 Window：
`ANativeWindow *window = ANativeWindow_fromSurface(env, surface);`
之后 EGL 就可以使用这个 window 了：

```
GLboolean EGLCore::buildContext(ANativeWindow *window) {
    // 与本地窗口系统通信
    mDisplay = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    if (mDisplay == EGL_NO_DISPLAY) {
        LOGE("eglGetDisplay failed: %d", eglGetError());
        return GL_FALSE;
    }

    GLint majorVersion;
    GLint minorVersion;
    if (!eglInitialize(mDisplay, &majorVersion, &minorVersion)) {
        LOGE("eglInitialize failed: %d", eglGetError());
        return GL_FALSE;
    }

    EGLConfig config;
    // 查找可用的 surface 配置
    EGLint numConfigs = 0;
    EGLint attribList[] = {
            EGL_RED_SIZE, 5,
            EGL_GREEN_SIZE, 6,
            EGL_BLUE_SIZE, 5,
            EGL_RENDERABLE_TYPE, EGL_OPENGL_ES3_BIT_KHR,
            EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
            EGL_NONE
    };

    // 让 EGL 推荐匹配的 EGLConfig
    if (!eglChooseConfig(mDisplay, attribList, &config, 1, &numConfigs)) {
        LOGE("eglChooseConfig failed: %d", eglGetError());
        return GL_FALSE;
    }

    if (numConfigs < 1) {
        LOGE("eglChooseConfig get configs number less than one");
        return GL_FALSE;
    }

    // 创建渲染上下文(rendering context)
    GLint contextAttribs[] = {EGL_CONTEXT_CLIENT_VERSION, 3, EGL_NONE};
    mContext = eglCreateContext(mDisplay, config, EGL_NO_CONTEXT, contextAttribs);
    if (mContext == EGL_NO_CONTEXT) {
        LOGE("eglCreateContext failed: %d", eglGetError());
        return GL_FALSE;
    }

    EGLint format = 0;
    if (!eglGetConfigAttrib(mDisplay, config, EGL_NATIVE_VISUAL_ID, &format)) {
        LOGE("eglGetConfigAttrib failed: %d", eglGetError());
        return GL_FALSE;
    }
    ANativeWindow_setBuffersGeometry(window, 0, 0, format);

    // 创建 On-Screen 渲染区域
    mSurface = eglCreateWindowSurface(mDisplay, config, window, 0);
    if (mSurface == EGL_NO_SURFACE) {
        LOGE("eglCreateWindowSurface failed: %d", eglGetError());
        return GL_FALSE;
    }

    // 把 EGLContext 和 EGLSurface 关联起来
    if (!eglMakeCurrent(mDisplay, mSurface, mSurface, mContext)) {
        LOGE("eglMakeCurrent failed: %d", eglGetError());
        return GL_FALSE;
    }

    LOGD("buildContext succeed");
    return GL_TRUE;
}
```

不再需要时，记得释放资源：

```
void EGLCore::release() {
    eglDestroySurface(mDisplay, mSurface);
    eglMakeCurrent(mDisplay, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
    eglDestroyContext(mDisplay, mContext);
}
```






