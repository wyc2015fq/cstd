# SDL2源代码分析7：显示（SDL_RenderPresent()） - 雷霄骅(leixiaohua1020)的专栏 - CSDN博客





2014年11月09日 00:59:05[雷霄骅](https://me.csdn.net/leixiaohua1020)阅读数：10929
所属专栏：[开源多媒体项目源代码分析](https://blog.csdn.net/column/details/osmedia.html)











=====================================================


SDL源代码分析系列文章列表：

[SDL2源代码分析1：初始化（SDL_Init()）](http://blog.csdn.net/leixiaohua1020/article/details/40680907)

[SDL2源代码分析2：窗口（SDL_Window）](http://blog.csdn.net/leixiaohua1020/article/details/40701203)


[SDL2源代码分析3：渲染器（SDL_Renderer）](http://blog.csdn.net/leixiaohua1020/article/details/40723085)

[SDL2源代码分析4：纹理（SDL_Texture）](http://blog.csdn.net/leixiaohua1020/article/details/40743061)

[SDL2源代码分析5：更新纹理（SDL_UpdateTexture()）](http://blog.csdn.net/leixiaohua1020/article/details/40876089)

[SDL2源代码分析6：复制到渲染器（SDL_RenderCopy()）](http://blog.csdn.net/leixiaohua1020/article/details/40895593)

[SDL2源代码分析7：显示（SDL_RenderPresent()）](http://blog.csdn.net/leixiaohua1020/article/details/40895797)


[SDL2源代码分析8：视频显示总结](http://blog.csdn.net/leixiaohua1020/article/details/41207787)


=====================================================


上一篇文章分析了SDL纹理赋值给渲染目标的函数SDL_RenderCopy()。这篇文章分析SDL显示视频最后的一个函数：SDL_RenderPresent()。

![](https://img-blog.csdn.net/20141107171722706)


SDL播放视频的代码流程如下所示。
**初始化: **

> SDL_Init(): 初始化SDL。 
SDL_CreateWindow(): 创建窗口（Window）。 
SDL_CreateRenderer(): 基于窗口创建渲染器（Render）。 
SDL_CreateTexture(): 创建纹理（Texture）。 
**循环渲染数据: **

> SDL_UpdateTexture(): 设置纹理的数据。 
SDL_RenderCopy(): 纹理复制给渲染器。 
SDL_RenderPresent(): 显示。

上篇文章分析了该流程中的第6个函数SDL_RenderCopy()。本文继续分析该流程中的最后一个函数SDL_RenderPresent()。




## SDL_RenderPresent()

### 函数简介

SDL使用SDL_RenderPresent()显示画面。SDL_RenderPresent()的原型如下。

```cpp
void SDLCALL SDL_RenderPresent(SDL_Renderer * renderer);
```
参数renderer用于指定渲染器。


### 函数调用关系图

SDL_RenderPresent()关键函数的调用关系可以用下图表示。

[![](https://img-blog.csdn.net/20141107172233131?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)](http://my.csdn.net/leixiaohua1020/album/detail/1794103)

上面的图片不太清晰，更清晰的图片上传到了相册里面：

[http://my.csdn.net/leixiaohua1020/album/detail/1794103](http://my.csdn.net/leixiaohua1020/album/detail/1794103)


把相册里面的图片保存下来就可以得到清晰的图片了。




### 源代码分析

SDL_RenderPresent()的源代码位于render\SDL_render.c中。如下所示。

```cpp
void SDL_RenderPresent(SDL_Renderer * renderer)
{
    CHECK_RENDERER_MAGIC(renderer, );


    /* Don't draw while we're hidden */
    if (renderer->hidden) {
        return;
    }
    renderer->RenderPresent(renderer);
}
```


从源代码中可以看出，SDL_RenderPresent()调用了SDL_Render的RenderPresent()方法显示图像。

下面我们详细看一下几种不同的渲染器的RenderPresent()的方法。



#### 1.Direct3D

Direct3D 渲染器中对应RenderPresent()的函数是D3D_RenderPresent()，它的源代码如下所示（位于render\direct3d\SDL_render_d3d.c）。

```cpp
static void D3D_RenderPresent(SDL_Renderer * renderer)
{
    D3D_RenderData *data = (D3D_RenderData *) renderer->driverdata;
    HRESULT result;


    if (!data->beginScene) {
        IDirect3DDevice9_EndScene(data->device);
        data->beginScene = SDL_TRUE;
    }


    result = IDirect3DDevice9_TestCooperativeLevel(data->device);
    if (result == D3DERR_DEVICELOST) {
        /* We'll reset later */
        return;
    }
    if (result == D3DERR_DEVICENOTRESET) {
        D3D_Reset(renderer);
    }
    result = IDirect3DDevice9_Present(data->device, NULL, NULL, NULL, NULL);
    if (FAILED(result)) {
        D3D_SetError("Present()", result);
    }
}
```

从代码中可以看出，该函数调用了2个最关键Direct3D的API：
IDirect3DDevice9_EndScene()：结束一个场景。
IDirect3DDevice9_Present()：显示。



#### 2.OpenGL

OpenGL渲染器中对应RenderPresent()的函数是GL_RenderPresent()，它的源代码如下所示（位于render\opengl\SDL_render_gl.c）。

```cpp
static void GL_RenderPresent(SDL_Renderer * renderer)
{
GL_ActivateRenderer(renderer);


    SDL_GL_SwapWindow(renderer->window);
}
```

代码比较简单，只有两行。关键的显示函数位于SDL_GL_SwapWindow()函数中。下面看一下SDL_GL_SwapWindow()的代码（位于video\SDL_video.c。感觉这里调用关系稍微有点乱…）。

```cpp
void SDL_GL_SwapWindow(SDL_Window * window)
{
    CHECK_WINDOW_MAGIC(window, );


    if (!(window->flags & SDL_WINDOW_OPENGL)) {
        SDL_SetError("The specified window isn't an OpenGL window");
        return;
    }


    if (SDL_GL_GetCurrentWindow() != window) {
        SDL_SetError("The specified window has not been made current");
        return;
    }


    _this->GL_SwapWindow(_this, window);
}
```

从上述代码中可以看出，SDL_GL_SwapWindow()调用了SDL_VideoDevice的GL_SwapWindow()函数。我们看一下在“Windows视频驱动”的情况下，该函数的代码。在“Windows视频驱动”的情况下，调用GL_SwapWindow()实际上是调用了WIN_GL_SwapWindow()函数。看一下WIN_GL_SwapWindow()函数的代码（位于video\windows\SDL_windowsopengl.c）。

```cpp
void WIN_GL_SwapWindow(_THIS, SDL_Window * window)
{
    HDC hdc = ((SDL_WindowData *) window->driverdata)->hdc;


    SwapBuffers(hdc);
}
```


代码中调用了简单的一个函数SwapBuffers()，完成了显示功能。




#### 3.Software

Software渲染器中对应RenderPresent()的函数是SW_RenderPresent()，它的源代码如下所示（位于render\software\SDL_render_sw.c）。

```cpp
static void SW_RenderPresent(SDL_Renderer * renderer)
{
    SDL_Window *window = renderer->window;


    if (window) {
        SDL_UpdateWindowSurface(window);
    }
}
```


从代码中可以看出，SW_RenderPresent()调用了一个函数SDL_UpdateWindowSurface()。我们看一下SDL_UpdateWindowSurface()的代码（位于video\SDL_video.c）。



```cpp
int SDL_UpdateWindowSurface(SDL_Window * window)
{
    SDL_Rect full_rect;


    CHECK_WINDOW_MAGIC(window, -1);


    full_rect.x = 0;
    full_rect.y = 0;
    full_rect.w = window->w;
    full_rect.h = window->h;
    return SDL_UpdateWindowSurfaceRects(window, &full_rect, 1);
}
```




SDL_UpdateWindowSurface()又调用了另一个函数SDL_UpdateWindowSurfaceRects()。继续看SDL_UpdateWindowSurfaceRects()的代码。



```cpp
int SDL_UpdateWindowSurfaceRects(SDL_Window * window, const SDL_Rect * rects,
                             int numrects)
{
    CHECK_WINDOW_MAGIC(window, -1);


    if (!window->surface_valid) {
        return SDL_SetError("Window surface is invalid, please call SDL_GetWindowSurface() to get a new surface");
    }


    return _this->UpdateWindowFramebuffer(_this, window, rects, numrects);
}
```




SDL_UpdateWindowSurfaceRects()调用了SDL_VideoDevice的UpdateWindowFramebuffer()函数。在“Windows视频驱动”的情况下，相当于调用了WIN_UpdateWindowFramebuffer()。我们看一下该函数的代码（位于video\windows\SDL_windowsframebuffer.c）



```cpp
int WIN_UpdateWindowFramebuffer(_THIS, SDL_Window * window, const SDL_Rect * rects, int numrects)
{
    SDL_WindowData *data = (SDL_WindowData *) window->driverdata;


    BitBlt(data->hdc, 0, 0, window->w, window->h, data->mdc, 0, 0, SRCCOPY);
    return 0;
}
```



经过一系列的寻找之后，终于找到了Software渲染器显示视频的“源头”：BitBlt()函数。





