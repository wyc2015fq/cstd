# FFmpeg解码流程+SDL播放流程及分屏处理 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2018年03月19日 10:28:17[boonya](https://me.csdn.net/boonya)阅读数：434








原文地址：[http://blog.csdn.net/leixiaohua1020/article/details/38868499](http://blog.csdn.net/leixiaohua1020/article/details/38868499)

##### FFmpeg解码视频流程



FFmpeg解码一个视频流程如下图所示：

![](https://img-blog.csdn.net/20140827132120380?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


##### SDL播放视频流程

SDL2.0显示YUV的流程图：

![](https://img-blog.csdn.net/20140827132301500?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


对比SDL1.2的流程图，发现变化还是很大的。几乎所有的API都发生了变化。但是函数和变量有一定的对应关系：

SDL_SetVideoMode()————SDL_CreateWindow()

SDL_Surface————SDL_Window

SDL_CreateYUVOverlay()————SDL_CreateTexture()

SDL_Overlay————SDL_Texture

不再一一例举。

下图为SDL1.x显示YUV的流程图。

![](https://img-blog.csdn.net/20140827134942937?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


简单解释各个变量的作用：

SDL_Window就是使用SDL的时候弹出的那个窗口。在SDL1.x版本中，只可以创建一个一个窗口。在SDL2.0版本中，可以创建多个窗口。
SDL_Texture用于显示YUV数据。一个SDL_Texture对应一帧YUV数据。
SDL_Renderer用于渲染SDL_Texture至SDL_Window。
SDL_Rect用于确定SDL_Texture显示的位置。注意：一个SDL_Texture可以指定多个不同的SDL_Rect，这样就可以在SDL_Window不同位置显示相同的内容（使用SDL_RenderCopy()函数）。
它们的关系如下图所示：


![](https://img-blog.csdn.net/20140827142100009?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


##### SDL实现分屏

下图举了个例子，指定了4个SDL_Rect，可以实现4分屏的显示。


![](https://img-blog.csdn.net/20140827132213562?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)




SDL_Rect用于确定SDL_Texture显示的位置。一个SDL_Texture可以指定多个不同的SDL_Rect.


```
SDL_Rect sdlRect;

sdlRect.x=0;
sdlRect.y=0;
sdlRect.w=screen_w;
sdlRect.h=screen_h;
SDL_Rect sdlRect2;
sdlRect2.x = screen_w;  //此处确定第二分屏显示位置
sdlRect2.y = 0;
sdlRect2.w = screen_w;
sdlRect2.h = screen_h;

// 复制渲染不同的SDLRect


SDL_RenderCopy(sdlRenderer, sdlTexture, NULL, &sdlRect);
SDL_RenderCopy( sdlRenderer, sdlTexture, NULL, &sdlRect2 );
```
注意：分屏和自定义插件多窗口播放是不一样的方式。


