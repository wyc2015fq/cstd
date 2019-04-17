# FFmpeg和SDL教程（二）：输出到屏幕 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2018年01月12日 18:34:39[boonya](https://me.csdn.net/boonya)阅读数：428








英文原文地址：[http://dranger.com/ffmpeg/tutorial02.html](http://dranger.com/ffmpeg/tutorial02.html)



## SDL和视频
要画到屏幕上，我们要使用SDL。 SDL代表简单直接层，是一个优秀的多媒体库，是跨平台的，并在多个项目中使用。您可以在官方网站上获得该库，或者可以下载适用于您的操作系统的开发包（如果有的话）。您将需要库来编译本教程的代码（以及其他代码）。


SDL有很多方法可以将图像绘制到屏幕上，特别是用于在屏幕上显示电影的方法 - 它称为YUV覆盖。 Y[UV（技术上不是YUV，而是YCbCr）](http://en.wikipedia.org/wiki/YCbCr)*注意：在“YCbCr”“YUV”的会议上有些人很烦恼。一般来说，YUV是模拟格式，YCbCr是数字格式。 ffmpeg和SDL都在其代码和宏中将YCbCr称为YUV。是一种存储像RGB原始图像数据的方法。粗略地说，Y是亮度（或“亮度”）分量，U和V是颜色分量。 （它比RGB更复杂，因为有些颜色信息被丢弃，每2个Y样本可能只有1个U和V样本。）SDL的YUV叠加接收YUV数据的原始数组并显示它。它接受4种不同的YUV格式，但是YV12是最快的。除了U和V阵列切换之外，还有另一种称为YUV420P的YUV格式，与YV12相同。 420表示以4：2：0的比例进行二次采样，基本上每4个亮度采样就有一个颜色采样，所以颜色信息被分解。这是一个节省带宽的好方法，因为人眼不会察觉到这种变化。名称中的“P”表示格式为“平面” - 仅表示Y，U和V组件在单独的数组中。 ffmpeg可以将图像转换为YUV420P，还有很多视频流已经以这种格式存在，或者很容易转换成这种格式。


所以我们目前的计划是从教程1中取代SaveFrame（）函数，而不是输出我们的帧到屏幕上。但首先我们要看看如何使用SDL库。首先，我们必须包括图书馆和初级SDL：



```cpp
#include <SDL.h>
#include <SDL_thread.h>

if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER)) {
  fprintf(stderr, "Could not initialize SDL - %s\n", SDL_GetError());
  exit(1);
}
```
SDL_Init（）从本质上告诉图书馆我们将要使用的功能。 SDL_GetError（）当然是一个方便的调试功能。


## 创建一个显示
现在我们需要在屏幕上放置一些东西。 使用SDL显示图像的基本区域称为曲面：

```cpp
SDL_Surface *screen;

screen = SDL_SetVideoMode(pCodecCtx->width, pCodecCtx->height, 0, 0);
if(!screen) {
  fprintf(stderr, "SDL: could not set video mode - exiting\n");
  exit(1);
}
```
这设置了具有给定宽度和高度的屏幕。 下一个选项是屏幕的位深度 - 0是一个特殊值，意思是“与当前显示相同”。 （这不适用于OS X;请参阅源代码。）
现在我们在该屏幕上创建一个YUV叠加层，以便我们可以输入视频，并设置我们的SWSContext将图像数据转换为YUV420：


```cpp
SDL_Overlay     *bmp = NULL;
struct SWSContext *sws_ctx = NULL;

bmp = SDL_CreateYUVOverlay(pCodecCtx->width, pCodecCtx->height,
                           SDL_YV12_OVERLAY, screen);

// initialize SWS context for software scaling
sws_ctx = sws_getContext(pCodecCtx->width,
                         pCodecCtx->height,
			 pCodecCtx->pix_fmt,
			 pCodecCtx->width,
			 pCodecCtx->height,
			 PIX_FMT_YUV420P,
			 SWS_BILINEAR,
			 NULL,
			 NULL,
			 NULL
			 );
```
如前所述，我们使用YV12来显示图像，并从ffmpeg获取YUV420数据。
## 显示图像

那很简单！ 现在我们只需要显示图像。 让我们一直走到我们完成框架的地方。 我们可以摆脱我们为RGB框架所做的所有工作，并且将用我们的显示代码替换SaveFrame（）。 为了显示图像，我们将制作一个[AVPicture](http://dranger.com/ffmpeg/data.html#AVPicture)结构，并设置它的数据指针和lineize到我们的YUV覆盖：


```cpp
if(frameFinished) {
    SDL_LockYUVOverlay(bmp);

    AVPicture pict;
    pict.data[0] = bmp->pixels[0];
    pict.data[1] = bmp->pixels[2];
    pict.data[2] = bmp->pixels[1];

    pict.linesize[0] = bmp->pitches[0];
    pict.linesize[1] = bmp->pitches[2];
    pict.linesize[2] = bmp->pitches[1];

    // Convert the image into YUV format that SDL uses
    sws_scale(sws_ctx, (uint8_t const * const *)pFrame->data,
	      pFrame->linesize, 0, pCodecCtx->height,
	      pict.data, pict.linesize);
    
    SDL_UnlockYUVOverlay(bmp);
  }
```
首先，我们锁定叠加层，因为我们将要写入它。这是一个很好的习惯，以便你以后没有问题。如前所示，AVPicture结构有一个数据指针，它是一个由4个指针组成的数组。由于我们在这里处理YUV420P，所以我们只有3个通道，因此只有3组数据。其他格式可能有一个alpha通道或其他东西的第四个指针。 lineize是它听起来像。我们的YUV覆盖中的类似结构是像素和间距变量。 （“pitch”是SDL用于指代给定数据行的宽度的术语）。因此，我们所做的是将pict.data的三个数组指向我们的叠加层，所以当我们写入pict的时候，我们实际上写入我们的覆盖层，这当然已经分配了必要的空间。同样，我们直接从我们的叠加层获取线路信息。我们将转换格式更改为PIX_FMT_YUV420P，并像以前一样使用sws_scale。


## 绘制图像
但是我们仍然需要告诉SDL实际显示我们提供的数据。我们还通过这个函数一个矩形，说明电影应该去哪里，什么样的宽度和高度应该缩放。这样，SDL就可以为我们进行缩放，并且可以通过图形处理器来加速缩放：

```cpp
SDL_Rect rect;

  if(frameFinished) {
    /* ... code ... */
    // Convert the image into YUV format that SDL uses
    sws_scale(sws_ctx, (uint8_t const * const *)pFrame->data,
              pFrame->linesize, 0, pCodecCtx->height,
	      pict.data, pict.linesize);
    
    SDL_UnlockYUVOverlay(bmp);
	rect.x = 0;
	rect.y = 0;
	rect.w = pCodecCtx->width;
	rect.h = pCodecCtx->height;
	SDL_DisplayYUVOverlay(bmp, &rect);
  }
```
现在我们的视频显示！
让我们花点时间向您展示SDL的另一个特性：事件系统。 SDL的设置是为了在SDL应用程序中键入或移动鼠标或发送信号时生成事件。 如果您的程序想要处理用户输入，则会检查这些事件。 您的程序也可以组成事件来发送SDL事件系统。 这在使用SDL进行多线程编程时特别有用，我们将在教程4中看到。在我们的程序中，我们将在完成处理数据包后立即轮询事件。 现在，我们只是要处理SDL_QUIT事件，所以我们可以退出：

```cpp
SDL_Event       event;

    av_free_packet(&packet);
    SDL_PollEvent(&event);
    switch(event.type) {
    case SDL_QUIT:
      SDL_Quit();
      exit(0);
      break;
    default:
      break;
    }
```
我们之前就是这样！ 摆脱所有的旧版本，你准备好编译。 如果您正在使用Linux或变体，使用SDL库进行编译的最佳方法是：

```cpp
gcc -o tutorial02 tutorial02.c -lavformat -lavcodec -lswscale -lz -lm \
`sdl-config --cflags --libs`
```
sdl-config只是打印出适当的gcc标志来正确包含SDL库。 你可能需要做一些不同的事情来让它在你的系统上编译。 请检查您的系统的SDL文档。 一旦编译完成，继续运行。
当你运行这个程序时会发生什么？ 视频疯了！ 实际上，我们只是尽可能快地显示所有的视频帧，我们可以从电影文件中提取它们。 我们现在还没有任何代码来确定何时需要显示视频。 最后（在教程5中），我们将开始同步视频。 但首先我们错过了更重要的东西：声音！






