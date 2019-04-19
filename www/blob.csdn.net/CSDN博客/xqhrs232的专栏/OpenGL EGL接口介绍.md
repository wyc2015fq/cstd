# OpenGL EGL接口介绍 - xqhrs232的专栏 - CSDN博客
2011年12月16日 16:45:53[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：2033
原文地址::[http://www.cnblogs.com/wanqieddy/archive/2011/11/24.html](http://www.cnblogs.com/wanqieddy/archive/2011/11/24.html)
[http://www.cnitblog.com/zouzheng/archive/2011/05/30/74326.html](http://www.cnitblog.com/zouzheng/archive/2011/05/30/74326.html)
EGL 是 OpenGL ES 和底层 Native 平台视窗系统之间的接口。本章主要讲述 OpenGL ES 的 EGL API ，以及如何用它创建 Context 和绘制Surface 等，并对用于 OpenGL 的其他视窗 API 做了比较分析，比如 WGL 和 GLX 。本章中将涵盖如下几个方面：
EGL 综述
EGL 主要构成（ Display ， Context ， Configuration ）
在 Brew 和 Windows CE 上使用 EGL
EGL 和其他 OpenGL 视窗系统的比较
EGL 介绍
EGL 是为 OpenGL ES 提供平台独立性而设计。在本章中，你将详细地学习每个 EGL API ，并了解使用 EGL 时候需要注意的平台特性和限制。 OpenGL ES 为附加功能和可能的平台特性开发提供了扩展机制，但仍然需要一个可以让 OpenGL ES 和本地视窗系统交互且平台无关的层。 OpenGL ES 本质上是一个图形渲染管线的状态机，而 EGL 则是用于监控这些状态以及维护 Frame
 buffer 和其他渲染 Surface 的外部层。图 2-1 是一个典型的 EGL 系统布局图。
![](http://pic002.cnblogs.com/img/afilover/201001/2010010922403158.png)
EGL 视窗设计是基于人们熟悉的用于 Microsoft Windows （ WGL ）和 UNIX （ GLX ）上的 OpenGL 的 Native 接口，与后者比较接近。OpenGL ES 图形管线的状态被存储于 EGL 管理的一个 Context 中。 Frame
 Buffers 和其他绘制 Surfaces 通过 EGL API 创建、管理和销毁。EGL 同时也控制和提供了对设备显示和可能的设备渲染配置的访问。
### EGL 数据类型
EGL包含了自己的一组数据类型，同时也提供了对一组平台相关的本地数据类型的支持。这些Native数据类型定义在EGL系统的头文件中。一旦你了解这些数据类型之间的不同，使用它们将变得很简单。多数情况下，为保证可移植性，开发人员将尽可能使用抽象数据类型而避免直接使用系统数据类型。通过使用定义在EGL中Native类型，可以让你写的EGL代码运行在任意的EGL的实现上。Native
 EGL类型说明如下：
l**NativeDisplayType **平台显示数据类型，标识你所开发设备的物理屏幕
l**NativeWindowType **平台窗口数据类型，标识系统窗口
l**NativePixmapType **可以作为 Framebuffer 的系统图像（内存）数据类型，该类型只用于离屏渲染
下面的代码是一个NativeWindowType定义的例子。这只是一个例子，不同平台之间的实现千差万别。使用native类型的关键作用在于为开发者抽象化这些细节。QUALCOMM使用IDIB结构定义native类型，如下：
struct IDIB {
     AEEVTBL(IBitmap) *pvt; // virtual table pointer
     IQueryInterface * pPaletteMap; // cache for computed palette mapping info
     byte * pBmp; // pointer to top row
     uint32 * pRGB; // palette
     NativeColor ncTransparent; // 32-bit native color value
     uint16 cx; // number of pixels in width
     uint16 cy; // number of pixels in height
     int16 nPitch; // offset from one row to the next
     uint16 cntRGB; // number of palette entries
     uint8 nDepth; // size of pixel in bits
     uint8 nColorScheme; // IDIB_COLORSCHEME_...(ie. 5-6-5)
     uint8 reserved[6];
};
接下来的小节中，我们将深入更多EGL数据类型细节。标准EGL数据类型如表2.1所示。
表2.1 EGL 数据类型
|数据类型|值|
|----|----|
|EGLBoolean|EGL_TRUE =1, EGL_FALSE=0|
|EGLint|int 数据类型|
|EGLDisplay|系统显示 ID 或句柄|
|EGLConfig|Surface 的 EGL 配置|
|EGLSurface|系统窗口或 frame buffer 句柄|
|EGLContext|OpenGL ES 图形上下文|
|NativeDisplayType|Native 系统显示类型|
|NativeWindowType|Native 系统窗口缓存类型|
|NativePixmapType|Native 系统 frame buffer|
### EGL Displays
EGLDisplay 是一个关联系统物理屏幕的通用数据类型。对于 PC 来说， Display 就是显示器的句柄。不管是嵌入式系统或 PC ，都可能有多个物理显示设备。为了使用系统的显示设备， EGL 提供了 EGLDisplay 数据类型，以及一组操作设备显示的 API 。
下面的函数原型用于获取 Native Display ：
EGLDisplay eglGetDisplay (NativeDisplayType display);
其中 display 参数是 native 系统的窗口显示 ID 值。如果你只是想得到一个系统默认的 Display ，你可以使用 **EGL_DEFAULT_DISPLAY**参数。如果系统中没有一个可用的 native display ID 与给定的 display 参数匹配，函数将返回 EGL_NO_DISPLAY ，而没有任何 Error 状态被设置。
由于设置无效的 display 值不会有任何错误状态，在你继续操作前请检测返回值。
下面是一个使用 EGL API 获取系统 Display 的例子：
m_eglDisplay = eglGetDisplay( system.display);
if (m_eglDisplay == EGL_NO_DISPLAY || eglGetError() != EGL_SUCCESS))
throw error_egl_display;
Initialization 初始化
    和很多视窗 API 类似， EGL 在使用前需要初始化，因此每个 EGLDisplay 在使用前都需要初始化。初始化 EGLDisplay 的同时，你可以得到系统中 EGL 的实现版本号。了解当前的版本号在向后兼容性方面是非常有价值的。嵌入式和移动设备通常是持续的投放到市场上，所以你需要考虑到你的代码将被运行在形形色色的实现上。通过动态查询 EGL 版本号，你可以为新旧版本的 EGL 附加额外的特性或运行环境。基于平台配置，软件开发可用清楚知道哪些 API 可用访问，这将会为你的代码提供最大限度的可移植性。
下面是初始化 EGL 的函数原型：
EGLBoolean eglInitialize (EGLDisplay dpy, EGLint *major, EGLint *minor);
其中 dpy 应该是一个有效的 EGLDisplay 。函数返回时， major 和 minor 将被赋予当前 EGL 版本号。比如 EGL1.0 ， major 返回 1 ，minor 则返回 0 。给 major 和 minor 传 NULL 是有效的，如果你不关心版本号。
       eglQueryString() 函数是另外一个获取版本信息和其他信息的途径。通过 eglQueryString() 获取版本信息需要解析版本字符串，所以通过传递一个指针给 eglInitializ() 函数比较容易获得这个信息。注意在调用 eglQueryString() 必须先使用 eglInitialize() 初始化 EGLDisplay ，否则将得到 EGL_NOT_INITIALIZED 错误信息。
下面是获取 EGL 版本字符串信息的函数原型：
const char * eglQueryString (EGLDisplay dpy, EGLint name);
参数name可以是EGL_VENDOR, EGL_VERSION, 或者EGL_EXTENSIONS。这个函数最常用来查询有哪些EGL扩展被实现。所有EGL扩展都是可选的，如果你想使用某个扩展特性，请检查该扩展是否被实现了，而不要想当然假定已经实现了。如果没有扩展被实现，将返回一个Null字符串，如果给定的name参数无效，则会得到EGL_BAD_PARAMETER.错误信息。
# 初始化EGL
OpenGL ES是一个平台中立的图形库，在它能够工作之前，需要与一个实际的窗口系统关联起来，这与OpenGL是一样的。但不一样的是，这部份工作有标准，这个标 准就是EGL。而OpenGL时代在不同平台上有不同的机制以关联窗口系统，在Windows上是wgl，在X-Window上是xgl，在Apple OS上是agl等。EGL的工作方式和部份术语都接近于xgl。
OpenGL ES的初始化过程如下图所示意：
```
Display → Config → Surface
```
```
↑
```
```
Context
```
```
↑
```
```
Application → OpenGL Command
```
1. 获取Display。
Display代表显示器，在有些系统上可以有多个显示器，也就会有多个Display。获得Display要调用EGLboolean eglGetDisplay(NativeDisplay dpy)，参数一般为 EGL_DEFAULT_DISPLAY 。该参数实际的意义是平台实现相关的，在X-Window下是XDisplay ID，在MS Windows下是Window DC。
2. 初始化egl。
调用 EGLboolean eglInitialize(EGLDisplay dpy, EGLint *major, EGLint *minor)，该函数会进行一些内部初始化工作，并传回EGL版本号(major.minor)。
3. 选择Config。
所为Config实际指的是FrameBuffer的参数，在MS Windows下对应于PixelFormat，在X-Window下对应Visual。一般用EGLboolean eglChooseConfig(EGLDisplay dpy, const EGLint * attr_list, EGLConfig * config, EGLint config_size, EGLint *num_config)，其中attr_list是以EGL_NONE结束的参数数组，通常以id,value依次存放，对于个别标识性的属性可以只有
 id，没有value。另一个办法是用EGLboolean eglGetConfigs(EGLDisplay dpy, EGLConfig * config, EGLint config_size, EGLint *num_config) 来获得所有config。这两个函数都会返回不多于config_size个Config，结果保存在config[]中，系统的总Config个数保存 在num_config中。可以利用eglGetConfig()中间两个参数为0来查询系统支持的Config总个数。
Config有众多的Attribute，这些Attribute决定FrameBuffer的格式和能力，通过eglGetConfigAttrib ()来读取，但不能修改。
4. 构造Surface。
Surface实际上就是一个FrameBuffer，通过 EGLSurface eglCreateWindowSurface(EGLDisplay dpy, EGLConfig confg, NativeWindow win, EGLint *cfg_attr) 来创建一个可实际显示的Surface。系统通常还支持另外两种Surface：PixmapSurface和PBufferSurface，这两种都不 是可显示的Surface，PixmapSurface是保存在系统内存中的位图，PBuffer则是保存在显存中的帧。
Surface也有一些attribute，基本上都可以故名思意， EGL_HEIGHT EGL_WIDTH EGL_LARGEST_PBUFFER EGL_TEXTURE_FORMAT EGL_TEXTURE_TARGET EGL_MIPMAP_TEXTURE EGL_MIPMAP_LEVEL，通过eglSurfaceAttrib()设置、eglQuerySurface()读取。
5. 创建Context。
OpenGL的pipeline从程序的角度看就是一个状态机，有当前的颜色、纹理坐标、变换矩阵、绚染模式等一大堆状态，这些状态作用于程序提交的顶点 坐标等图元从而形成帧缓冲内的像素。在OpenGL的编程接口中，Context就代表这个状态机，程序的主要工作就是向Context提供图元、设置状 态，偶尔也从Context里获取一些信息。
用EGLContext eglCreateContext(EGLDisplay dpy, EGLSurface write, EGLSurface read, EGLContext * share_list)来创建一个Context。
6. 绘制。
应用程序通过OpenGL API进行绘制，一帧完成之后，调用eglSwapBuffers(EGLDisplay dpy, EGLContext ctx)来显示。
### EGL Configurations
EGLConfigs 是一个用来描述 EGL surface 配置信息的数据类型。要获取正确的渲染结果， Surface 的格式是非常重要的。根据平台的不同，surface 配置可能会有限制，比如某个设备只支持 16 位色深显示，或是不支持 stencil buffer ，还有其他的功能限制或精度的差异。
下面是获取系统可用的 EGL 配置信息的函数原型：
EGLBoolean eglGetConfigs (EGLDisplay dpy, EGLConfig *configs,EGLint config_size, EGLint *num_config);
参数 configs 将包含在你的平台上有效的所有 EGL framebuffer 配置列表。支持的配置总数将通过 num_config 返回。实际返回的 configs 的配置个数依赖于程序传入的 config_size 。如果 config_size < num_config ，则不是所有的配置信息都将被返回。如果想获取系统支持的所有配置信息，最好的办法就是先给 eglGetConfig 传一个 NULL 的 configs 参数， num_config 将得到系统所支持的配置总数，然后用它来给configs 分配合适的内存大小，再用得到的 configs 来调用 eglGetConfig 。
下面是如果使用 eglGetConfig() 函数的例子：
EGLConfig *configs_list;
EGLint num_configs;
// Main Display
m_eglDisplay = eglGetDisplay(EGL_DEFAULT_DISPLAY);
if( m_eglDisplay == EGL_NO_DISPLAY || eglGetError() != EGL_SUCCESS )
return FALSE;
if( eglInitialize( m_eglDisplay, NULL, NULL ) == EGL_FALSE || eglGetError() != EGL_SUCCESS )
return FALSE;
// find out how many configurations are supported
if ( eglGetConfigs( m_eglDisplay, NULL, 0, &num_configs)
== EGL_FALSE || eglGetError() != EGL_SUCCESS )
return FALSE;
configs_list = malloc(num_configs * sizeof(EGLConfig));
if (configs_list == (EGLConfig *)0)
return FALSE;
// Get Configurations
if( eglGetConfigs( m_eglDisplay, configs_list, num_configs, &num_configs)
== EGL_FALSE || eglGetError() != EGL_SUCCESS )
return FALSE;
由于当前平台的限制，通常只有很少的配置可用。系统支持的配置通常是利用系统硬件提供最好的性能。当你移植游戏到多个平台，它们的EGL 配置可能会有细微的差别，我们希望作为通用的移植问题来直接处理这些问题。
选择一个 EGL Configuration
基于 EGL 的属性，你可以定义一个希望从系统获得的配置，它将返回一个最接近你的需求的配置。选择一个你特有的配置是有点不合适的，因为只是在你的平台上使用有效。 eglChooseConfig() 函数将适配一个你所期望的配置，并且尽可能接近一个有效的系统配置。
下面是选择一个 EGL 配置的函数原型：
EGLBoolean eglChooseConfig(EGLDisplay dpy, const EGLint *attrib_list,
EGLConfig *configs, EGLint config_size, EGLint * num_config);
参数 attrib_list 指定了选择配置时需要参照的属性。参数 configs 将返回一个按照 attrib_list 排序的平台有效的所有 EGL framebuffer 配置列表。参数 config_size 指定了可以返回到 configs 的总配置个数。参数 num_config 返回了实际匹配的配置总数。
下面是如果使用 eglChoosetConfig() 函数的例子：
EGLint attrs[3] = { EGL_DEPTH_SIZE, 16, EGL_NONE };
EGLint num_configs;
EGLConfigs *configs_list;
// Get the display device
if ((eglDisplay = eglGetDisplay(EGL_NO_DISPLAY)) == EGL_NO_DISPLAY)
{
return eglGetError();
}
// Initialize the display
if (eglInitialize(eglDisplay, NULL, NULL) == EGL_FALSE)
{
return eglGetError();
}
// Obtain the total number of configurations that match
if (eglChooseConfig(eglDisplay, attrs, NULL, 0, &num_configs) == EGL_FALSE)
{
return eglGetError();
}
configs_list = malloc(num_configs * sizeof(EGLConfig));
if (configs_list == (EGLConfig *)0)
return eglGetError();
// Obtain the first configuration with a depth buffer of 16 bits
if (!eglChooseConfig(eglDisplay, attrs, &configs_list, num_configs, &num_configs))
{
return eglGetError();
}
如果找到多个合适的配置，有一个简单的排序算法用来匹配最接近你所查询的配置。表 2-2 显示了基于属性值的用来选择和排序的顺序，也包括了 EGL 规范中所有 EGL 配置属性及其默认值。
表 2.1 EGL 配置属性默认值和匹配法则
|属性|数据类型|默认值|排序优先级|选择顺序|
|----|----|----|----|----|
|EGL_BUFFER_SIZE|int|0|3|Smaller value|
|EGL_RED_SIZE|int|0|2|Larger value|
|EGL_GREEN_SIZE|int|0|2|Larger value|
|EGL_BLUE_SIZE|int|0|2|Larger value|
|EGL_ALPHA_SIZE|int|0|2|Larger value|
|EGL_CONFIG_CAVET|enum|EGL_DONT_CARE|1(first)|Exact value|
|EGL_CONFIG_ID|int|EGL_DONT_CARE|9|Exact value|
|EGL_DEPTH_SIZE|int|0|6|Smaller value|
|EGL_LEVEL|int|0|-|Equal value|
|EGL_NATIVE_RENDERABLE|Boolean|EGL_DONT_CARE|-|Exact value|
|EGL_NATIVE_VISUAL_TYPE|int|EGL_DONT_CARE|8|Exact value|
|EGL_SAMPLE_BUFFERS|int|0|4|Smaller value|
|EGL_SAMPLES|int|0|5|Smaller value|
|EGL_STENCIL_SIZE|int|0|7|Smaller value|
|EGL_SURFACE_TYPE|bitmask|EGL_WINDOW_BIT|-|Mask value|
|EGL_TRANSPARENT_TYPE|enum|EGL_NONE|-|Exact value|
|EGL_TRANSPARENT_RED_VALUE|int|EGL_DONT_CARE|-|Exact value|
|EGL_TRANSPARENT_GREEN_VALUE|int|EGL_DONT_CARE|-|Exact value|
|EGL_TRANSPARENT_BLUE_VALUE|int|EGL_DONT_CARE|-|Exact value|
这里不是讲OpenGL的教程，事实上，OpenGL 是与硬件和平台无关的。这里主要讲在Android下，OpenGL 应用的基本架构、OpenGL 与Android 窗口系统通过EGL的绑定
根据官方网站（[http://www.khronos.org/egl/](http://www.khronos.org/egl/)）的描述，EGL是图形资源管理层，工作在图形渲染API（如OpenGL）与运行平台（Android）的窗口系统之间
从1.5（API 3）开始Android 支持 OpenGL ES 1.0，到 2.2（API 8）时支持 OpenGL ES 2.0。版本对应关系如下（待完善）
|Android|SDK API|EGL|OpenGL ES|OpenGL|
|----|----|----|----|----|
|1.5|3|1.0|1.0|1.3|
|1.6|4||||
|2.1|7||||
|2.2|8||2.0||
Android、EGL、OpenGL 三者关系如下：
   Android Windowing    ( SurfaceView)          ^          | +--------+------------------------+ | EGL    |           +------------+ | |        |              | Display | | |        |           +------------+ | |  +-----v-------+   +------------+ | |    | Surface |     | Config | | |  +-----^-------+   +------------+ | |        |           +------------+ | |        |           | Context | | |        |           +------------+ | +--------+------------------------+          |          v        OpenGL

EGL 官网有一个1.0版本的 Specification，详细讲述了Surface、Display、Context 概念。简单地说
（1）Display 是图形显示设备（显示屏）的抽象表示。大部分EGL函数都要带一个 Display 作为参数
（2）Context 是 OpenGL 状态机。Context 与 Surface 可以是一对一、多对一、一对多的关系
（3）Surface 是绘图缓冲，可以是 window、pbuffer、pixmap 三种类型之一
EGL 工作流程为：
（1）初始化
（2）配置
（3）创建Surface（绑定到平台Windowing系统）
（4）绑定Surface与Context
（5）Main Loop：渲染（OpenGL），交换离线缓冲（offline buffer）与显示缓冲
（6）释放资源
在Android SDK中，EGL类在javax.microedition.khronos.egl包中，OpenGL 类在 javax.microedition.khronos.opengles包中
下面是一个完整的 EGL/OpenGL 应用，当点击屏幕时，根据点击坐标更新屏幕背景颜色
package roxit.opengldemo; import javax.microedition.khronos.egl.EGL10; import javax.microedition.khronos.egl.EGLConfig; import javax.microedition.khronos.egl.EGLContext; import javax.microedition.khronos.egl.EGLDisplay; import javax.microedition.khronos.egl.EGLSurface; import javax.microedition.khronos.opengles.GL10; import android.app.Activity; import android.os.Bundle; import android.util.Log; import android.view.MotionEvent; import android.view.SurfaceHolder; import android.view.SurfaceView; import android.view.View; import android.view.Window; import android.view.WindowManager; import android.view.SurfaceHolder.Callback; import android.view.View.OnTouchListener; public class OpenGlDemo extends Activity implements [Callback](http://java.sun.com/j2se/1.5.0/docs/api/javax/security/auth/callback/Callback.html), [Runnable](http://java.sun.com/j2se/1.5.0/docs/api/java/lang/Runnable.html), OnTouchListener { private SurfaceView view; private boolean rendering = false; private final [Object](http://www.google.com/search?sitesearch=java.sun.com&q=allinurl%3Aj2se%2F1+5+0%2Fdocs%2Fapi+Object) renderLock = new [Object](http://www.google.com/search?sitesearch=java.sun.com&q=allinurl%3Aj2se%2F1+5+0%2Fdocs%2Fapi+Object)(); private GL10 gl; private float red = 0.2f, green = 0.3f, blue = 0.8f; @[Override](http://java.sun.com/j2se/1.5.0/docs/api/java/lang/Override.html) public void onCreate(Bundle savedInstanceState) { requestWindowFeature(Window.FEATURE_NO_TITLE); getWindow().addFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN); super.onCreate(savedInstanceState); view = new SurfaceView(this); view.getHolder().addCallback(this); view.setOnTouchListener(this); setContentView(view); } public void surfaceCreated(SurfaceHolder holder) { synchronized (renderLock) { Log.d("OpenGlDemo >>>", "Start rendering..."); rendering = true; new [Thread](http://java.sun.com/j2se/1.5.0/docs/api/java/lang/Thread.html)(this).start(); } } public void surfaceChanged(SurfaceHolder holder, int format, int width, int height) {   } public void surfaceDestroyed(SurfaceHolder holder) { synchronized (renderLock) { rendering = false; } } public void run() { Init EGL10 egl = (EGL10) EGLContext.getEGL(); EGLDisplay disp = egl.eglGetDisplay(EGL10.EGL_DEFAULT_DISPLAY); egl.eglInitialize(disp, new int[2]); Config EGLConfig[] configs = new EGLConfig[1]; egl.eglChooseConfig(disp, new int[] { EGL10.EGL_DEPTH_SIZE, 16, EGL10.EGL_NONE }, configs, 1, new int[1]); EGLConfig config = configs[0]; Create surface and bind with native windowing EGLSurface surf = egl.eglCreateWindowSurface(disp, config, view .getHolder(), null);   Bind with OpenGL context EGLContext contx = egl.eglCreateContext(disp, config, EGL10.EGL_NO_CONTEXT, null); egl.eglMakeCurrent(disp, surf, surf, contx); gl = (GL10) contx.getGL(); while (true) { synchronized (renderLock) { if (!rendering) { break; } } render(gl); egl.eglSwapBuffers(disp, surf); } Log.d("OpenGlDemo >>>", "Stop rendering"); // Finalize egl.eglMakeCurrent(disp, EGL10.EGL_NO_SURFACE, EGL10.EGL_NO_SURFACE, EGL10.EGL_NO_CONTEXT); egl.eglDestroyContext(disp, contx); egl.eglDestroySurface(disp, surf); gl = null; } private void render(GL10 gl) { gl.glClearColor(red, green, blue, 1.0f); gl.glClear(GL10.GL_COLOR_BUFFER_BIT); } public boolean onTouch([View](http://java.sun.com/j2se/1.5.0/docs/api/javax/swing/text/View.html) view, MotionEvent e) { red = e.getX() / view.getWidth(); green = e.getY() / view.getHeight(); blue = 1.0f; return true; } }
上面的应用，就是在Android 下的OpenGL 应用的最基本结构，涉及了不少EGL细节的操作
如果你google 一下“Android OpenGL”，得到结果十之八九是使用了android.opengl.GLSurfaceView。使用GLSurfaceView实现上面的简单应用，代码要简单得多
是这样的：GLSurfaceView隐藏了EGL操作及渲染线程的细节，并提供了生命周期回调方法
但，基本上，使用 GLSurfaceView 没法控制渲染循环，例如：你没法控制帧速（fps）
EGL是由Khronos Group提供的一组平台无关的API。它的功能：
1> 和本地窗口系统（native windowing system）通讯；
2> 查询可用的配置；
3> 创建OpenGL ES可用的“绘图表面”（drawing surface）；
4> 同步不同类别的API之间的渲染，比如在OpenGL ES和OpenVG之间同步，或者在OpenGL和本地窗口的绘图命令之间；
5> 管理“渲染资源”，比如纹理映射（rendering map）。
● EGLDisplay
EGL可运行于GNU/Linux的X Window System，Microsoft Windows和MacOS X的Quartz。
EGL把这些平台的显示系统抽象为一个独立的类型：EGLDisplay。
使用EGL的第一步就是初始化一个可用的EGLDisplay：
- EGLint majorVersion;  
- EGLint minorVersion;  
- EGLBoolean success = EGL_FALSE;  
- EGLDisplay display = eglGetDisplay(EGL_DEFAULT_DISPLAY);  
- if (display != EGL_NO_DISPLAY)  
- {  
-     success = eglInitialize(display, &majorVersion, &minorVersion);  
- }  
- if (success != EGL_TRUE)  
- {  
-     EGLint errno = eglGetError();  
- if (errno != EGL_SUCCESS)  
-     {  
-         _TCHAR errmsg[32];  
-         _stprintf(errmsg, _T("[EGL] Initialization failed. Error code: 0x%04x"), errno);  
- // EGL_BAD_DISPLAY      EGLDisplay参数错误  
- // EGL_NOT_INITIALIZED  EGL不能初始化  
-     }  
- }  
这里用到了三个EGL函数：
- EGLDisplay eglGetDisplay(EGLNativeDisplayType id);  
- EGLBoolean eglInitialize(EGLDisplay display, EGLint* majorVersion, EGLint* minorVersion);  
- EGLint eglGetError();  
● EGLConfig
初始化过后，要选择一个合适的“绘图表面”。
- EGLBoolean eglGetConfigs(EGLDisplay display,    // 已初始化好  
-                          EGLConfig* configs,    // 如果为NULL，则返回EGL_TRUE和numConfigs，即图形系统所有可用的配置  
-                          EGLint maxConfigs,     // 上面那个configs数组的容量  
-                          EGLint* numConfigs);   // 图形系统返回的实际的可用的配置个数，存储在configs数组里  
用例：
- static const EGLint CONFIG_ATTRIBS[] =  
- {  
-     EGL_RED_SIZE,       5,  
-     EGL_GREEN_SIZE,     6,  
-     EGL_BLUE_SIZE,      5,  
-     EGL_DEPTH_SIZE,     16,  
-     EGL_ALPHA_SIZE,     EGL_DONT_CARE,  
-     EGL_STENCIL_SIZE,   EGL_DONT_CARE,  
-     EGL_SURFACE_TYPE,   EGL_WINDOW_BIT,  
-     EGL_NONE            // 属性表以该常量为结束符  
- };  
- GLint numConfigs;  
- EGLConfig config;  
- if (success != EGL_FALSE)  
-     success = eglGetConfigs(display, NULL, 0, &numConfigs);  
- if (success != EGL_FALSE && numConfigs > 0)  
-     success = eglChooseConfig(display, CONFIG_ATTRIBS, &config, 1, &numConfigs);  
可以查询某个配置的某个属性：
- EGLBoolean eglGetConfigAttrib(EGLDisplay display,    // 已初始化  
-                               EGLConfig config,      // 某个配置  
-                               EGLint attribute,      // 某个属性  
-                               EGLint * value);  
让EGL为你选择一个配置：
- EGLBoolean eglChooseConfig(EGLDisplay display,  
- const EGLint* attribs,    // 你想要的属性事先定义到这个数组里  
-                            EGLConfig* configs,       // 图形系统将返回若干满足条件的配置到该数组  
-                            EGLint maxConfigs,        // 上面数组的容量  
-                            EGLint* numConfigs);      // 图形系统返回的可用的配置个数  
EGL如果选择了多个配置给你，则按一定规则放到数组里：
1> EGL_CONFIG_CAVEAT
2> EGL_COLOR_BUFFER_TYPE
3> 按color buffer所占位宽
4> EGL_BUFFER_SIZE
5> EGL_SAMPLE_BUFFERS
6> EGL_SAMPLES
7> EGL_DEPTH_SIZE
8> EGL_STENCIL_SIZE
9> EGL_ALPHA_MASK_SIZE
10> EGL_NATIVE_VISUAL_TYPE
11> EGL_CONFIG_ID
● EGLSurface
- EGLSurface eglCreateWindowSurface(EGLDisplay display,  
-                                   EGLConfig config,  
-                                   EGLNativeWindowType window, // 在Windows上就是HWND类型  
- const EGLint* attribs);     // 此属性表非彼属性表  
这里的属性表并非用于OpenGL ES 2.0，而是其它的API，比如OpenVG。我们只需要记住一个：EGL_RENDER_BUFFER [EGL_BACK_BUFFER, EGL_FRONT_BUFFER]。
OpenGL ES 2.0是必须工作于双缓冲窗口系统的。
该属性表当然也可以为NULL，也可以只有一个EGL_NONE。那表示所有属性使用默认值。
如果函数返回EGL_NO_SURFACE，则失败。错误码：
EGL_BAD_MATCH:          属性设置错误。比如EGL_SURFACE_TYPE没有设置EGL_WINDOW_BIT
EGL_BAD_CONFIG:         因为配置错误，图形系统不支持
EGL_BAD_NATIVE_WINDOW:  窗口句柄错误
EGL_BAD_ALLOC:          无法创建绘图表面。比如先前已经创建一个了。
● pixel buffer
OpenGL ES 2.0可以向pixel buffer渲染，同样使用硬件加速。pbuffer经常用来生成纹理映射。如果想渲染到纹理，常用更高效的framebuffer对象。
在EGL_SURFACE_TYPE里使用使用EGL_PBUFFER_BIT可创建pbuffer：
- EGLSurface eglCreatePbufferSurface(EGLDisplay display,  
-                                    EGLConfig config,  
- const EGLint* attribs);  
使用到的属性：
EGL_WIDTH, EGL_HEIGHT
EGL_LARGEST_PBUFFER:        如果参数不合适，可使用最大的pbuffer
EGL_TEXTURE_FORMAT:         [EGL_NO_TEXTURE] 如果pbuffer要绑定到纹理映射，要指定纹理的格式
EGL_TEXTURE_TARGET:            [EGL_NO_TEXTURE, EGL_TEXTURE_2D]
EGL_MIPMAP_TEXTRUE:         [EGL_TRUE, EGL_FALSE]
创建失败时返回EGL_NO_SURFACE，错误码：
EGL_BAD_ALLOC:      缺少资源
EGL_BAD_CONFIG:     配置错误
EGL_BAD_PARAMETER:  EGL_WIDTH和EGL_HEIGHT为负数
EGL_BAD_MATCH:      配置错误；如果用于纹理映射，则高宽参数错误；EGL_TEXTURE_FORMAT和EGL_TEXTURE_TARGET只有一个不是EGL_NO_TEXTURE
EGL_BAD_ATTRIBUTE:  指定了EGL_TEXTURE_FORMAT、EGL_TEXTURE_TARGET或者EGL_MIPMAP_TEXTRUE，却不指定使用OpenGLES在配置里
使用pbuffer的例子：
- EGLint cfgAttribs[] =  
- {  
-     EGL_SURFACE_TYPE,    EGL_PBUFFER_BIT,  
-     EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,  
-     EGL_RED_SIZE,        5,  
-     EGL_GREEN_SIZE,      6,  
-     EGL_BLUE_SIZE,       5,  
-     EGL_DEPTH_SIZE,      1,  
-     EGL_NONE  
- };  
- const EGLint MAX_CONFIG = 10;  // 我们要从10个配置里挑选一个  
- EGLConfig configs[MAX_CONFIG];  
- EGLint numConfigs;  
- if (!eglChooseConfig(display, cfgAttribs, configs, MAX_CONFIG, &numConfigs))  
- {  
- // 报错  
- }  
- else  
- {  
- // 挑选一个配置  
- }  
- EGLint PBufAttribs[] =  
- {  
-     EGL_WIDTH,  512,  
-     EGL_HEIGHT, 512,  
-     EGL_LARGEST_PBUFFER, EGL_TRUE,  
-     EGL_NONE  
- };  
- EGLRenderSurface pbuffer = eglCreatePbufferSurface(display, config, PBufAttribs);  
- if (pbuffer == EGL_NO_SURFACE)  
- {  
- // 创建失败，报各种错  
- }  
- EGLint width, height;  
- if (!eglQuerySurface(display, pbuffer, EGL_HEIGHT, &height)  
-     || !eglQuerySurface(display, pbuffer, EGL_WIDTH, &width)  
- {  
- // 查询不到信息，报错  
- }  
pbuffer和普通的窗口渲染最大的不同是不能swap，要么拷贝其值，要么修改其绑定成为纹理。
● EGLContext
- EGLContext eglCreateContext(EGLDisplay display,  
-                             EGLConfig config,  
-                             EGLContext context,    // EGL_NO_CONTEXT表示不向其它的context共享资源  
- const EGLint * attribs)// 我们暂时只用EGL_CONTEXT_CLIENT_VERSION  
- const EGLint attribs[] =  
- {  
-     EGL_CONTEXT_CLIENT_VERSION, 2,  
-     EGL_NONE  
- };  
- EGLContext context = eglCreateContext(display, cfg, EGL_NO_CONTEXT, attribs);  
- if (context == EGL_NO_CONTEXT)  
- {  
- if (EGL_BAD_CONFIG == eglGetError())  
-     {  
-         ...  
-     }  
- }  
- if (!eglMakeCurrent(display, window, window, context)) // 两个window表示读写都在一个窗口  
- {  
- // 报错  
- }  
● 渲染同步
只使用OpenGL ES 2.0，那么，glFinish即可保证所有的渲染工作进行下去。
但使用OpenVG或本地图形API渲染字体，要比使用OpenGL ES 2.0要容易。所以，你可能要在同一个窗口使用多个库来渲染。
可以用EGL的同步函数：EGLBoolean eglWaitClient() 延迟客户端的执行，等待服务器端完成OpenGL ES 2.0或者OpenVG的渲染。
如果失败，返回错误码：EGL_BAD_CURRENT_SURFACE。
如果要等待本地图形API的渲染完成，使用：EGLBoolean eglWaitNative(EGLint engine)。
engine参数必须是EGL_CORE_NATIVE_ENGINE。其它值都是通过EGL扩展来指定。
如果失败，返回错误码：EGL_BAD_PARAMETER。

