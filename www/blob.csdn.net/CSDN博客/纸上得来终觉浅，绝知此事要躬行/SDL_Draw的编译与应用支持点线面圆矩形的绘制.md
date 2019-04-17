# SDL_Draw的编译与应用支持点线面圆矩形的绘制 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2018年08月10日 11:11:07[boonya](https://me.csdn.net/boonya)阅读数：191








SDL_Draw的编译本文是在Linux系统下操作。

### 1.安装

从 [http://sdl-draw.sourceforge.net](http://sdl-draw.sourceforge.net)下载,解压到本地。

嵌入式编译：
`./configure --prefix=${PWD}/tmp --host=arm-linux --build=i386 --target=arm-linux --disable-static --enable-shared --disable-sdltest --with-sdl-prefix=/home/mt2601/GUI_source/mydirectfb-yizhi/SDL-1.2.15/tmp`


**修改./Makefile ./src/Makefile ./libtool**

所有有关编译的东西都加上arm-linux-, 比如ranlib加上 arm-linux-ranlib



make && make install只会生成静态库，不知道为什么，但是可以用

**编译：（README.txt里有说明）**
`arm-linux-gcc -o sdldraw sdldraw.c -Wall `/home/mt2601/GUI_source/mydirectfb-yizhi/SDL-1.2.15/tmp/bin/sdl-config --cflags` -I./include `/home/mt2601/GUI_source/mydirectfb-yizhi/SDL-1.2.15/tmp/bin/sdl-config --libs` ./tmp/lib/libSDL_draw.a`
注：安装完成后，它的头文件SDL_draw还需要手动复制从include的文件夹中复制到/usr/include/SDL下,否则编译时还是找不到这个头文件。安装如果感觉不是很清晰请参考：[https://blog.csdn.net/action086/article/details/7328210](https://blog.csdn.net/action086/article/details/7328210)

### 2.编译

安装完成后，就可以编译了。

编译命令：
` g++ demo.cpp -o demo -lSDL_draw`
示例程序如下：

```
#include <stdlib.h>
#include "SDL/SDL.h"
#include "SDL/SDL_draw.h"
lib
  2 
int main(int argc, char *argv[])
{
  SDL_Surface *screen;// 屏幕
  int width, height;
  Uint8 video_bpp;
  Uint32 videoflags;
   if ( SDL_Init(SDL_INIT_VIDEO) < 0 ) {
    fprintf(stderr, "SDL初始化失败。。。。 %s", SDL_GetError());
    exit(1);
  }
  atexit(SDL_Quit); //退出
  videoflags = SDL_SWSURFACE | SDL_ANYFORMAT;
  width = 640;
  height = 480;
  video_bpp = 0;
  /*Video mode activation*/
  screen = SDL_SetVideoMode(width, height, video_bpp, videoflags);
  if (!screen) {
    fprintf(stderr, "不能激活视频模式: %dx%d: %s\n",
            width, height, SDL_GetError());
    exit(2);
  }
{
  Uint32 c_red = SDL_MapRGB(screen->format, 255,0,0);
  Draw_Circle(screen, 50,50, 50, c_red); // 画圆，圆心（50，50） 半径50
  Draw_Rect(screen, 500,400, 50,50, c_red);//画矩形 
  SDL_UpdateRect(screen, 0, 0, 0, 0);//刷新屏幕
//Wait 10 seconds
    SDL_Delay( 10000 );
  fprintf(stderr, "显示结束。。。。");
  return 0;

}
}
```





### **3.更多了解**


**Projects using SDL_draw**

- The game [Asteroids](http://www.2dgame-tutorial.com/sdl/asteroids/asteroids.htm) (maybe not accesible).
- The ToolKit [SDL_VKPToolkit](http://groups.yahoo.com/group/SDL_VKPToolkit/).
- The game [Board Warfare](http://personalpages.tds.net/~mcozart/games.html).



**Library API**


```
#include "SDL_draw.h"



void Draw_Pixel(SDL_Surface *super,

                Sint16 x, Sint16 y, 

                Uint32 color);


Draw a colored pixel on coordinates x,y.



void Draw_Line(SDL_Surface *super,

               Sint16 x1, Sint16 y1, Sint16 x2, Sint16 y2,

               Uint32 color);


Draw a line from x1,y1 to x2,y2.



void Draw_Circle(SDL_Surface *super,

                 Sint16 x0, Sint16 y0, Uint16 r,

                 Uint32 color);


Draw a circle with center x0,y0 and radius r.



void Draw_FillCircle(SDL_Surface *super,

                     Sint16 x0, Sint16 y0, Uint16 r,

                     Uint32 color);


Draw a filled circle with center x0,y0 and radius r.



void Draw_HLine(SDL_Surface *super,

                Sint16 x0,Sint16 y0, Sint16 x1,

                Uint32 color);


Draw a horizontal line from x0,y0 to x1,y0.



void Draw_VLine(SDL_Surface *super,

                Sint16 x0,Sint16 y0, Sint16 y1,

                Uint32 color);


Draw a vertical line from x0,y0 to x0,y1.



void Draw_Rect(SDL_Surface *super,

               Sint16 x,Sint16 y, Uint16 w,Uint16 h,

               Uint32 color);


Draw a rectangle with upper left corner in x,y being w the width and h the height.



void Draw_FillRect(SDL_Surface *super,

                   Sint16 x,Sint16 y, Uint16 w,Uint16 h,

                   Uint32 color);


The same as above but the rectangle is filled. This function is equivalent to SDL_FillRect (is a MACRO).



void Draw_Ellipse(SDL_Surface *super,

                  Sint16 x0, Sint16 y0,

                  Uint16 Xradius, Uint16 Yradius,

                  Uint32 color);


Draw a ellipse with center in x0,y0. Xradius is the major axis and Yradius is the minor axis.



void Draw_FillEllipse(SDL_Surface *super,

                      Sint16 x0, Sint16 y0,

                      Uint16 Xradius, Uint16 Yradius,

                      Uint32 color);


Draw a filled ellipse (same parameters as the above function).



void Draw_Round(SDL_Surface *super,

                Sint16 x0,Sint16 y0, Uint16 w,Uint16 h,

                Uint16 corner, Uint32 color);


Draw a rectangle with rounded corners. x0,y0 is the upper left corner of the rectangle, w is the width and h is the height. corner is the radius of the corner.



void Draw_FillRound(SDL_Surface *super,

                Sint16 x0,Sint16 y0, Uint16 w,Uint16 h,

                Uint16 corner, Uint32 color);


The same as above but the rounded rectangle is filled.
```




The file sdldrawtest.c is a example application for the library. You can compile it using (for the GNU Compiler):


```
$ export CFLAGS="`sdl-config --cflags` -I./include"

$ export LIBS="`sdl-config --libs` ./src/.libs/libSDL_draw.a"

$ gcc -o sdldrawtest sdldrawtest.c -Wall $CFLAGS $LIBS
```




### 4.图像绘制示例

点线面及图像加载示例：[https://blog.csdn.net/thearcticocean/article/details/55823288](https://blog.csdn.net/thearcticocean/article/details/55823288)

几何绘图示例：[https://www.cnblogs.com/1024Planet/p/5750481.html](https://www.cnblogs.com/1024Planet/p/5750481.html)

### 5.C#扩展考虑

如果能编译生成dll文件，那么C#也可以引用了。

```
using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using static SDL2.SDL;

namespace SDL2.src
{
    /// <summary>
    /// SDL_draw version is 1.2.13
    /// </summary>
    public static class SDL2_draw
    {
        /// <summary>
        /// DLL文件需要自己编译生成
        /// </summary>
        private const string nativeLibName = "SDL2_draw.dll";

        /// <summary>
        ///  Draw a colored pixel on coordinates x,y.
        /// </summary>
        /// <param name="SDL_Surface"></param>
        /// <param name="x"></param>
        /// <param name="y"></param>
        /// <param name="color"></param>
        [DllImport(nativeLibName, EntryPoint = "Draw_Pixel", CallingConvention = CallingConvention.Cdecl)]
        public static extern void Draw_Pixel(IntPtr SDL_Surface,
                int x, int y,
                Int32 color);



        /// <summary>
        ///  Draw a line from x1,y1 to x2,y2.
        /// </summary>
        /// <param name="SDL_Surface"></param>
        /// <param name="x1"></param>
        /// <param name="y1"></param>
        /// <param name="x2"></param>
        /// <param name="y2"></param>
        /// <param name="color"></param>
        [DllImport(nativeLibName, EntryPoint = "Draw_Line", CallingConvention = CallingConvention.Cdecl)]
        public static extern void Draw_Line(IntPtr SDL_Surface,
               int x1, int y1, int x2, int y2,
               int color);



        /// <summary>
        ///  Draw a circle with center x0, y0 and radius r.
        /// </summary>
        /// <param name="SDL_Surface"></param>
        /// <param name="x0"></param>
        /// <param name="y0"></param>
        /// <param name="r"></param>
        /// <param name="color"></param>
        [DllImport(nativeLibName, EntryPoint = "Draw_Circle", CallingConvention = CallingConvention.Cdecl)]
        public static extern void Draw_Circle(IntPtr SDL_Surface,
                 int x0, int y0, int r,
                 int color);



        /// <summary>
        /// Draw a filled circle with center x0,y0 and radius r.
        /// </summary>
        /// <param name="SDL_Surface"></param>
        /// <param name="x0"></param>
        /// <param name="y0"></param>
        /// <param name="r"></param>
        /// <param name="color"></param>
        [DllImport(nativeLibName, EntryPoint = "Draw_FillCircle", CallingConvention = CallingConvention.Cdecl)]
        public static extern void Draw_FillCircle(IntPtr SDL_Surface,
                     int x0, int y0, int r,
                     int color);




        /// <summary>
        /// Draw a horizontal line from x0, y0 to x1, y0.
        /// </summary>
        /// <param name="SDL_Surface"></param>
        /// <param name="x0"></param>
        /// <param name="y0"></param>
        /// <param name="x1"></param>
        /// <param name="color"></param>
        [DllImport(nativeLibName, EntryPoint = "Draw_HLine", CallingConvention = CallingConvention.Cdecl)]
        public static extern void Draw_HLine(IntPtr SDL_Surface,
                        int x0, int y0, int x1,
                        int color);



        /// <summary>
        /// Draw a vertical line from x0, y0 to x0, y1.
        /// </summary>
        /// <param name="SDL_Surface"></param>
        /// <param name="x0"></param>
        /// <param name="y0"></param>
        /// <param name="y1"></param>
        /// <param name="color"></param>
        [DllImport(nativeLibName, EntryPoint = "Draw_VLine", CallingConvention = CallingConvention.Cdecl)]
        public static extern void Draw_VLine(IntPtr SDL_Surface,
                int x0, int y0, int y1,
                int color);



        /// <summary>
        /// Draw a rectangle with upper left corner in x,y being w the width and h the height.
        /// </summary>
        /// <param name="SDL_Surface"></param>
        /// <param name="x"></param>
        /// <param name="y"></param>
        /// <param name="w"></param>
        /// <param name="h"></param>
        /// <param name="color"></param>
        [DllImport(nativeLibName, EntryPoint = "Draw_Rect", CallingConvention = CallingConvention.Cdecl)]
        public static extern void Draw_Rect(IntPtr SDL_Surface,
               int x, int y, int w, int h,
               int color);



        /// <summary>
        /// The same as above but the rectangle is filled.This function is equivalent to SDL_FillRect(is a MACRO).
        /// </summary>
        /// <param name="SDL_Surface"></param>
        /// <param name="x"></param>
        /// <param name="y"></param>
        /// <param name="w"></param>
        /// <param name="h"></param>
        /// <param name="color"></param>
        [DllImport(nativeLibName, EntryPoint = "Draw_FillRect", CallingConvention = CallingConvention.Cdecl)]
        public static extern void Draw_FillRect(IntPtr SDL_Surface,
                   int x, int y, int w, int h,
                   int color);



        /// <summary>
        ///  Draw a ellipse with center in x0,y0.Xradius is the major axis and Yradius is the minor axis.
        /// </summary>
        /// <param name="SDL_Surface"></param>
        /// <param name="x0"></param>
        /// <param name="y0"></param>
        /// <param name="Xradius"></param>
        /// <param name="Yradius"></param>
        /// <param name="color"></param>
        [DllImport(nativeLibName, EntryPoint = "Draw_Ellipse", CallingConvention = CallingConvention.Cdecl)]
        public static extern void Draw_Ellipse(IntPtr SDL_Surface,
                  int x0, int y0,
                  int Xradius, int Yradius,
                  int color);



        /// <summary>
        ///  Draw a filled ellipse(same parameters as the above function).
        /// </summary>
        /// <param name="SDL_Surface"></param>
        /// <param name="x0"></param>
        /// <param name="y0"></param>
        /// <param name="Xradius"></param>
        /// <param name="Yradius"></param>
        /// <param name="color"></param>
        [DllImport(nativeLibName, EntryPoint = "Draw_FillEllipse", CallingConvention = CallingConvention.Cdecl)]
        public static extern void Draw_FillEllipse(IntPtr SDL_Surface,
                      int x0, int y0,
                      int Xradius, int Yradius,
                      int color);



        /// <summary>
        /// Draw a rectangle with rounded corners.x0, y0 is the upper left corner of the rectangle, w is the width and h is the height. corner is the radius of the corner.
        /// </summary>
        /// <param name="SDL_Surface"></param>
        /// <param name="x0"></param>
        /// <param name="y0"></param>
        /// <param name="w"></param>
        /// <param name="h"></param>
        /// <param name="corner"></param>
        /// <param name="color"></param>
        [DllImport(nativeLibName, EntryPoint = "Draw_Round", CallingConvention = CallingConvention.Cdecl)]
        public static extern void Draw_Round(IntPtr SDL_Surface,
                int x0, int y0, int w, int h,
                int corner, int color);




        /// <summary>
        /// The same as above but the rounded rectangle is filled.
        /// </summary>
        /// <param name="SDL_Surface"></param>
        /// <param name="x0"></param>
        /// <param name="y0"></param>
        /// <param name="w"></param>
        /// <param name="h"></param>
        /// <param name="corner"></param>
        /// <param name="color"></param>
        [DllImport(nativeLibName, EntryPoint = "Draw_FillRound", CallingConvention = CallingConvention.Cdecl)]
        public static extern void Draw_FillRound(IntPtr SDL_Surface,
                        int x0, int y0, int w, int h,
                        int corner, int color);

        

    }
}
```





