# sdl初学笔记 - Koma Hub - CSDN博客
2018年12月09日 17:56:06[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：184
**目录**
[1.介绍](#1.%E4%BB%8B%E7%BB%8D)
[例子1：初始化](#%E4%BE%8B%E5%AD%901%EF%BC%9A%E5%88%9D%E5%A7%8B%E5%8C%96)
[2.图形和视频](#2.%E5%9B%BE%E5%BD%A2%E5%92%8C%E8%A7%86%E9%A2%91)
[2.1SDL的视频介绍](#2.1SDL%E7%9A%84%E8%A7%86%E9%A2%91%E4%BB%8B%E7%BB%8D)
[2.1.1初始化视频显示](#2.1.1%E5%88%9D%E5%A7%8B%E5%8C%96%E8%A7%86%E9%A2%91%E6%98%BE%E7%A4%BA)
[例子2——1 初始化视频显示](#%E4%BE%8B%E5%AD%902%E2%80%94%E2%80%941%20%E5%88%9D%E5%A7%8B%E5%8C%96%E8%A7%86%E9%A2%91%E6%98%BE%E7%A4%BA)
[2.1.2初始化最好的视频模式](#2.1.2%E5%88%9D%E5%A7%8B%E5%8C%96%E6%9C%80%E5%A5%BD%E7%9A%84%E8%A7%86%E9%A2%91%E6%A8%A1%E5%BC%8F)
[例子2——2 初始化最好的视频模式](#%E4%BE%8B%E5%AD%902%E2%80%94%E2%80%942%20%E5%88%9D%E5%A7%8B%E5%8C%96%E6%9C%80%E5%A5%BD%E7%9A%84%E8%A7%86%E9%A2%91%E6%A8%A1%E5%BC%8F)
[2.1.3读取和显示一个bmp 文件](#2.1.3%E8%AF%BB%E5%8F%96%E5%92%8C%E6%98%BE%E7%A4%BA%E4%B8%80%E4%B8%AAbmp%20%E6%96%87%E4%BB%B6)
[例子2——3 读取和现实一个bmp 文件](#%E4%BE%8B%E5%AD%902%E2%80%94%E2%80%943%20%E8%AF%BB%E5%8F%96%E5%92%8C%E7%8E%B0%E5%AE%9E%E4%B8%80%E4%B8%AAbmp%20%E6%96%87%E4%BB%B6)
[2.1.4直接绘图来显示](#2.1.4%E7%9B%B4%E6%8E%A5%E7%BB%98%E5%9B%BE%E6%9D%A5%E6%98%BE%E7%A4%BA)
[例子2——4getpixel()](#%E4%BE%8B%E5%AD%902%E2%80%94%E2%80%944getpixel())
[例子2——6 使用putpixel()](#%E4%BE%8B%E5%AD%902%E2%80%94%E2%80%946%20%E4%BD%BF%E7%94%A8putpixel())
[2.2在sdl中使用opengl](#2.2%E5%9C%A8sdl%E4%B8%AD%E4%BD%BF%E7%94%A8opengl)
[2.2.1初始化](#2.2.1%E5%88%9D%E5%A7%8B%E5%8C%96)
[例子2——7 初始化SDL 的opengl](#%E4%BE%8B%E5%AD%902%E2%80%94%E2%80%947%20%E5%88%9D%E5%A7%8B%E5%8C%96SDL%20%E7%9A%84opengl)
[2.2.2绘制](#2.2.2%E7%BB%98%E5%88%B6)
[例子2——8SDL 和OpenGL](#%E4%BE%8B%E5%AD%902%E2%80%94%E2%80%948SDL%20%E5%92%8COpenGL)
[3 输入处理](#3%20%E8%BE%93%E5%85%A5%E5%A4%84%E7%90%86)
[3.1控制杆处理](#3.1%E6%8E%A7%E5%88%B6%E6%9D%86%E5%A4%84%E7%90%86)
[3.1.1初始化](#3.1.1%E5%88%9D%E5%A7%8B%E5%8C%96)
[3.1.2询问](#3.1.2%E8%AF%A2%E9%97%AE)
[3.1.3打开一个控制杆接受控制杆事件](#3.1.3%E6%89%93%E5%BC%80%E4%B8%80%E4%B8%AA%E6%8E%A7%E5%88%B6%E6%9D%86%E6%8E%A5%E5%8F%97%E6%8E%A7%E5%88%B6%E6%9D%86%E4%BA%8B%E4%BB%B6)
[3.1.4优化的控制杆功能](#3.1.4%E4%BC%98%E5%8C%96%E7%9A%84%E6%8E%A7%E5%88%B6%E6%9D%86%E5%8A%9F%E8%83%BD)
[3.2处理键盘](#3.2%E5%A4%84%E7%90%86%E9%94%AE%E7%9B%98)
[3.2.1键盘相关结构体](#3.2.1%E9%94%AE%E7%9B%98%E7%9B%B8%E5%85%B3%E7%BB%93%E6%9E%84%E4%BD%93)
[3.2.2SDLKey](#3.2.2SDLKey)
[3.2.4SDL_keysym](#3.2.4SDL_keysym)
[3.2.5SDL_KeyboardEvent](#3.2.5SDL_KeyboardEvent)
[3.2.6读取键盘事件](#3.2.6%E8%AF%BB%E5%8F%96%E9%94%AE%E7%9B%98%E4%BA%8B%E4%BB%B6)
[3.2.7一个更加详细的查看](#3.2.7%E4%B8%80%E4%B8%AA%E6%9B%B4%E5%8A%A0%E8%AF%A6%E7%BB%86%E7%9A%84%E6%9F%A5%E7%9C%8B)
[3.2.8游戏模式输入](#3.2.8%E6%B8%B8%E6%88%8F%E6%A8%A1%E5%BC%8F%E8%BE%93%E5%85%A5)
# 1.介绍
SDL 是一个由8 个子系统组成——音频、CDROM、事件驱动、文件I/O、操作杆驱动、线程、时钟和视频。在你使用这些子系统之前，你必须要初始化他们，使用SDL_Init（或者SDL_InitSubSystem。SDL_Init 必须在其他SDL 功能被调用之前调用。所以初始化默认的子系统和视频子系统你需要调用SDL_Init(SDL_INIT_VIDEO)，而初始化默认的子系统、视频系统和时间系统你需要调用SDL_Init ( SDL_INIT_VIDEO | SDL_INIT_TIMER )）SDL_Init 的退出使用SDL_Quit（和SDL_QuitSubSystem）SDL_Quit 关掉所有子系统，包括默认的，它应该被调用在SDL 应用退出之前。
SDL_Init 和SDL_Quit 坚固的嵌入你的程序工具箱，你能写你的第一个和最基础的SDL应用。但是，我们必须准备处理错误。许多SDL 功能返回一个值指出这个功能是成功或者失败，例如，SDL_Init 返回-1 如果不能初始化一个子系统。SDL 提供一个有用的设备允许你精确的定位错误的类型， 每次一个SDL 内错误发生一个错误信息被存储， 使用SDL_GetError 可以找到。经常使用这个，你能知道错误的很多。
### 例子1：初始化
```cpp
#include "SDL.h" /* All SDL App's need this */
#include <stdio.h>
int main() 
{
    printf("Initializing SDL.\n");
    /* Initialize defaults, Video and Audio */
    if((SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO)==-1)) 
	{
        printf("Could not initialize SDL: %s.\n", SDL_GetError());
        exit(-1);
    }
    printf("SDL initialized.\n");
    printf("Quiting SDL.\n");
    /* Shutdown all subsystems */
    SDL_Quit();
    printf("Quiting....\n");
    exit(0);
}
```
编译：
```
$ gcc demo-init.c `pkg-config --libs --cflags sdl`
$ ./a.out 
Initializing SDL.
SDL initialized.
Quiting SDL.
Quiting....
```
# 2.图形和视频
## 2.1SDL的视频介绍
视频也许是最常用的SDL 的应用地方，所以它有最全套的子系统。这里是一些例子来显示这个基础。
## 2.1.1初始化视频显示
### 例子2——1 初始化视频显示
```cpp
#include "SDL.h" /* All SDL App's need this */
#include <stdio.h>
int main() 
{
    printf("Initializing SDL.\n");
    /* Initialize defaults, Video and Audio */
    if((SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO)==-1)) 
	{
        printf("Could not initialize SDL: %s.\n", SDL_GetError());
        exit(-1);
    }
    printf("SDL initialized.\n");
    
    SDL_Surface *screen;
        
    /* Clean up on exit */
    atexit(SDL_Quit);
    /*
     * Initialize the display in a 640x480 8-bit palettized mode,
     * requesting a software surface
     */
    screen = SDL_SetVideoMode(640, 480, 8, SDL_SWSURFACE);
    if ( screen == NULL ) 
	{
        fprintf(stderr, "Couldn't set 640x480x8 video mode: %s\n",
        SDL_GetError());
        exit(1);
    }
    printf("Quiting SDL.\n");
    /* Shutdown all subsystems */
    SDL_Quit();
    printf("Quiting....\n");
    exit(0);
}
```
显示一个：
![](https://img-blog.csdnimg.cn/20181209164038132.PNG?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1JvbmdfVG9h,size_16,color_FFFFFF,t_70)
## 2.1.2初始化最好的视频模式
如果你有一个偏好一个可靠的像素深度但是将要接受其他的， 那么如下使用SDL_SetVideoMode 和SDL_ANYFORMAT。你也能使用SDL_VideoModeOK() 来找到当地视频模式来尽量匹配你要求的模式。
### 例子2——2 初始化最好的视频模式
```cpp
#include "SDL.h" /* All SDL App's need this */
#include <stdio.h>
int main() 
{
    printf("Initializing SDL.\n");
    /* Initialize defaults, Video and Audio */
    if((SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO)==-1)) 
	{
        printf("Could not initialize SDL: %s.\n", SDL_GetError());
        exit(-1);
    }
    printf("SDL initialized.\n");
    
    SDL_Surface *screen;
        
    /* Clean up on exit */
    atexit(SDL_Quit);
    
    /* Have a preference for 8-bit, but accept any depth */
    screen = SDL_SetVideoMode(640, 480, 8, SDL_SWSURFACE|SDL_ANYFORMAT);
    if ( screen == NULL ) {
        fprintf(stderr, "Couldn't set 640x480x8 video mode: %s\n",
        SDL_GetError());
        exit(1);
    }
    printf("Set 640x480 at %d bits-per-pixel mode\n",
    screen->format->BitsPerPixel);
    printf("Quiting SDL.\n");
    /* Shutdown all subsystems */
    //SDL_Quit();
    printf("Quiting....\n");
    exit(0);
}
```
## 2.1.3读取和显示一个bmp 文件
下面的功能读取和显示一个作为参数给出的bmp 文件，前提SDL 被初始化视频模式被设置。
### 例子2——3 读取和现实一个bmp 文件
```cpp
#include "SDL.h" /* All SDL App's need this */
#include <stdio.h>
void display_bmp(char *file_name)
{
    SDL_Surface *image;
    SDL_Surface *screen;
    
    /* Load the BMP file into a surface */
    image = SDL_LoadBMP(file_name);
    if (image == NULL) {
        fprintf(stderr, "Couldn't load %s: %s\n", file_name, SDL_GetError());
        return;
    }
    /* Have a preference for 8-bit, but accept any depth */
    screen = SDL_SetVideoMode(640, 480, 8, SDL_SWSURFACE|SDL_ANYFORMAT);
    if ( screen == NULL ) {
        fprintf(stderr, "Couldn't set 640x480x8 video mode: %s\n",
        SDL_GetError());
        exit(1);
    }
    printf("Set 640x480 at %d bits-per-pixel mode\n",
        screen->format->BitsPerPixel);
        
    /*
    * Palettized screen modes will have a default palette (a standard
    * 8*8*4 colour cube), but if the image is palettized as well we can
    * use that palette for a nicer colour matching
    */
    if (image->format->palette && screen->format->palette) {
        SDL_SetColors(screen, image->format->palette->colors, 0,
        image->format->palette->ncolors);
    }
    /* Blit onto the screen surface */
    if(SDL_BlitSurface(image, NULL, screen, NULL) < 0)
        fprintf(stderr, "BlitSurface error: %s\n", SDL_GetError());
    
    SDL_UpdateRect(screen, 0, 0, image->w, image->h);
    
    /* Free the allocated BMP surface */
    SDL_FreeSurface(image);
    
    sleep(2);
}
int main() 
{
    printf("Initializing SDL.\n");
    /* Initialize defaults, Video and Audio */
    if((SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO)==-1)) 
	{
        printf("Could not initialize SDL: %s.\n", SDL_GetError());
        exit(-1);
    }
    printf("SDL initialized.\n");
    
    SDL_Surface *screen;
    display_bmp("a.bmp");
    
    printf("Quiting SDL.\n");
    /* Shutdown all subsystems */
    //SDL_Quit();
    printf("Quiting....\n");
    exit(0);
}
```
结果运行时会显示一个图片到屏幕上，显示两秒钟。
## 2.1.4直接绘图来显示
以下的两个功能能够用于得到和设置单独的一个平面的像素。他们仔细的被写和任何当前SDL 的深度工作。基础要锁定这个表面再调用他们，在调用其他SDL 功能之前需要解锁。
在像素值和他们的红、绿、蓝部件之间转换，使用SDL_GetRGB() 和SDL_MapRGB()。
### 例子2——4getpixel()
```cpp
#include "SDL.h" /* All SDL App's need this */
#include <stdio.h>
/*
* Return the pixel value at (x, y)
* NOTE: The surface must be locked before calling this!
*/
Uint32 getpixel(SDL_Surface *surface, int x, int y)
{
    int bpp = surface->format->BytesPerPixel;
    /* Here p is the address to the pixel we want to retrieve */
    Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;
    switch(bpp) {
        case 1:
            return *p;
        case 2:
            return *(Uint16 *)p;
        case 3:
            if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
                return p[0] << 16 | p[1] << 8 | p[2];
            else
                return p[0] | p[1] << 8 | p[2] << 16;
        case 4:
            return *(Uint32 *)p;
        default:
            return 0; /* shouldn't happen, but avoids warnings */
    }
}
int main() 
{
    printf("Initializing SDL.\n");
    /* Initialize defaults, Video and Audio */
    if((SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO)==-1)) 
	{
        printf("Could not initialize SDL: %s.\n", SDL_GetError());
        exit(-1);
    }
    printf("SDL initialized.\n");
    
    SDL_Surface *screen;
    /* Have a preference for 8-bit, but accept any depth */
    screen = SDL_SetVideoMode(640, 480, 8, SDL_SWSURFACE|SDL_ANYFORMAT);
    if ( screen == NULL ) {
        fprintf(stderr, "Couldn't set 640x480x8 video mode: %s\n",
        SDL_GetError());
        exit(1);
    }
    printf("Set 640x480 at %d bits-per-pixel mode\n",
        screen->format->BitsPerPixel);
        
    getpixel(screen, 12, 12);
    
    printf("Quiting SDL.\n");
    /* Shutdown all subsystems */
    SDL_Quit();
    printf("Quiting....\n");
    exit(0);
}
```
下面的代码使用putpixel()功能来设置一个黄色像素在屏幕中间
### 例子2——6 使用putpixel()
```cpp
#include "SDL.h" /* All SDL App's need this */
#include <stdio.h>
/*
* Return the pixel value at (x, y)
* NOTE: The surface must be locked before calling this!
*/
void putpixel(SDL_Surface *surface, int x, int y, Uint32 color)
{
    int bpp = surface->format->BytesPerPixel;
    /* Here p is the address to the pixel we want to retrieve */
    Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;
    *p = color;
}
int main() 
{
    printf("Initializing SDL.\n");
    /* Initialize defaults, Video and Audio */
    if((SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO)==-1)) 
	{
        printf("Could not initialize SDL: %s.\n", SDL_GetError());
        exit(-1);
    }
    printf("SDL initialized.\n");
    
    SDL_Surface *screen;
    /* Have a preference for 8-bit, but accept any depth */
    screen = SDL_SetVideoMode(640, 480, 8, SDL_SWSURFACE|SDL_ANYFORMAT);
    if ( screen == NULL ) {
        fprintf(stderr, "Couldn't set 640x480x8 video mode: %s\n",
        SDL_GetError());
        exit(1);
    }
    printf("Set 640x480 at %d bits-per-pixel mode\n",
        screen->format->BitsPerPixel);
        
    /* Code to set a yellow pixel at the center of the screen */
    int x, y;
    Uint32 yellow;
    /* Map the color yellow to this display (R=0xff, G=0xFF, B=0x00)
    Note: If the display is palettized, you must set the palette first.
    */
    yellow = SDL_MapRGB(screen->format, 0xff, 0xff, 0x00);
    x = screen->w / 2;
    y = screen->h / 2;
    /* Lock the screen for direct access to the pixels */
    if ( SDL_MUSTLOCK(screen) ) {
        if ( SDL_LockSurface(screen) < 0 ) {
            fprintf(stderr, "Can't lock screen: %s\n", SDL_GetError());
            return;
        }
    }
    putpixel(screen, x, y, yellow);
    if ( SDL_MUSTLOCK(screen) ) {
        SDL_UnlockSurface(screen);
    }
    
    /* Update just the part of the display that we've changed */
    SDL_UpdateRect(screen, x, y, 1, 1);
    
    sleep(4);
    printf("Quiting SDL.\n");
    /* Shutdown all subsystems */
    SDL_Quit();
    printf("Quiting....\n");
    exit(0);
}
```
# 2.2在sdl中使用opengl
SDL 有能力创建和使用opengl 环境在一系列的平台上（Linux/X11，Win32，BeOs，MacOs，Classic/Toolbox，MacOs X，FreeBSD/X11 和Solaris/X11）。允许你使用SDL 的音频，事件处理，线程和定时器在你的opengl 应用（一个功能经常被GLUT 执行）。
## 2.2.1初始化
初始化SDL 来使用opengl 是非常不同于正常的初始化SDL。有三个不同;你必须传递SDL_OPENGL 到SDL_SetVideoMode，你必须详说明一系列GL 属性（深度缓冲尺寸，帧缓冲尺寸）通过使用SDL_GL_SetAttribute 和最终的，如果你希望来使用“双缓冲”你必须详说明它作为一个GL 属性，不通过传送SDL_DOUBLEBUF 标记到SDL_SetVideoMode
### 例子2——7 初始化SDL 的opengl
```cpp
/*这个我没有测试*/
/* Information about the current video settings. */
const SDL_VideoInfo* info = NULL;
/* Dimensions of our window. */
int width = 0;
int height = 0;
/* Color depth in bits of our window. */
int bpp = 0;
/* Flags we will pass into SDL_SetVideoMode. */
int flags = 0;
/* First, initialize SDL's video subsystem. */
if( SDL_Init( SDL_INIT_VIDEO ) < 0 ) {
    /* Failed, exit. */
    fprintf( stderr, "Video initialization failed: %s\n",
    SDL_GetError( ) );
    quit_tutorial( 1 );
}
/* Let's get some video information. */
info = SDL_GetVideoInfo( );
if( !info ) {
    /* This should probably never happen. */
    fprintf( stderr, "Video query failed: %s\n",
    SDL_GetError( ) );
    quit_tutorial( 1 );
}
/*
* Set our width/height to 640/480 (you would
* of course let the user decide this in a normal
* app). We get the bpp we will request from
* the display. On X11, VidMode can't change
* resolution, so this is probably being overly
* safe. Under Win32, ChangeDisplaySettings
* can change the bpp.
*/
width = 640;
height = 480;
bpp = info->vfmt->BitsPerPixel;
/*
* Now, we want to setup our requested
* window attributes for our OpenGL window.
* We want *at least* 5 bits of red, green
* and blue. We also want at least a 16-bit
* depth buffer.
*
* The last thing we do is request a double
* buffered window. '1' turns on double
* buffering, '0' turns it off.
*
* Note that we do not use SDL_DOUBLEBUF in
* the flags to SDL_SetVideoMode. That does
* not affect the GL attribute state, only
* the standard 2D blitting setup.
*/
SDL_GL_SetAttribute( SDL_GL_RED_SIZE, 5 );
SDL_GL_SetAttribute( SDL_GL_GREEN_SIZE, 5 );
SDL_GL_SetAttribute( SDL_GL_BLUE_SIZE, 5 );
SDL_GL_SetAttribute( SDL_GL_DEPTH_SIZE, 16 );
SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );
/*
* We want to request that SDL provide us
* with an OpenGL window, in a fullscreen
* video mode.
*
* EXERCISE:
* Make starting windowed an option, and
* handle the resize events properly with
* glViewport.
*/
flags = SDL_OPENGL | SDL_FULLSCREEN;
/*
* Set the video mode
*/
if( SDL_SetVideoMode( width, height, bpp, flags ) == 0 ) {
    /*
    * This could happen for a variety of reasons,
    * including DISPLAY not being set, the specified
    * resolution not being available, etc.
    */
    fprintf( stderr, "Video mode set failed: %s\n",
    SDL_GetError( ) );
    quit_tutorial( 1 );
}
```
## 2.2.2绘制
相比初始化，在SDL 中使用opengl 与在其他api 中使用opengl 是相同的，e.g.GLUT。你还是使用所有同样功能的调用和数据结构。但是如果你在使用一个双缓冲显示，你必须使用SDL_GL_GetAttribute 来看你是否真正的获得了它。一个全面的例子显示如下。
### 例子2——8SDL 和OpenGL
```cpp
/*
* SDL OpenGL Tutorial.
* (c) Michael Vance, 2000
* briareos@lokigames.com
*
* Distributed under terms of the LGPL.
*/
#include <SDL/SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <stdio.h>
#include <stdlib.h>
static GLboolean should_rotate = GL_TRUE;
static void quit_tutorial( int code )
{
    /*
    * Quit SDL so we can release the fullscreen
    * mode and restore the previous video settings,
    * etc.
    */
    SDL_Quit( );
    /* Exit program. */
    exit( code );
}
static void handle_key_down( SDL_keysym* keysym )
{
    /*
    * We're only interested if 'Esc' has
    * been presssed.
    *
    * EXERCISE:
    * Handle the arrow keys and have that change the
    * viewing position/angle.
    */
    switch( keysym->sym ) {
        case SDLK_ESCAPE:
            quit_tutorial( 0 );
            break;
        case SDLK_SPACE:
            should_rotate = !should_rotate;
            break;
        default:
            break;
    }
}
static void process_events( void )
{
    /* Our SDL event placeholder. */
    SDL_Event event;
    /* Grab all the events off the queue. */
    while( SDL_PollEvent( &event ) ) {
        switch( event.type ) {
            case SDL_KEYDOWN:
                /* Handle key presses. */
                handle_key_down( &event.key.keysym );
                break;
            case SDL_QUIT:
                /* Handle quit requests (like Ctrl-c). */
                quit_tutorial( 0 );
                break;
        }
    }
}
static void draw_screen( void )
{
    /* Our angle of rotation. */
    static float angle = 0.0f;
    /*
    * EXERCISE:
    * Replace this awful mess with vertex
    * arrays and a call to glDrawElements.
    *
    * EXERCISE:
    * After completing the above, change
    * it to use compiled vertex arrays.
    *
    * EXERCISE:
    * Verify my windings are correct here ;).
    */
    static GLfloat v0[] = { -1.0f, -1.0f, 1.0f };
    static GLfloat v1[] = { 1.0f, -1.0f, 1.0f };
    static GLfloat v2[] = { 1.0f, 1.0f, 1.0f };
    static GLfloat v3[] = { -1.0f, 1.0f, 1.0f };
    static GLfloat v4[] = { -1.0f, -1.0f, -1.0f };
    static GLfloat v5[] = { 1.0f, -1.0f, -1.0f };
    static GLfloat v6[] = { 1.0f, 1.0f, -1.0f };
    static GLfloat v7[] = { -1.0f, 1.0f, -1.0f };
    static GLubyte red[] = { 255, 0, 0, 255 };
    static GLubyte green[] = { 0, 255, 0, 255 };
    static GLubyte blue[] = { 0, 0, 255, 255 };
    static GLubyte white[] = { 255, 255, 255, 255 };
    static GLubyte yellow[] = { 0, 255, 255, 255 };
    static GLubyte black[] = { 0, 0, 0, 255 };
    static GLubyte orange[] = { 255, 255, 0, 255 };
    static GLubyte purple[] = { 255, 0, 255, 0 };
    /* Clear the color and depth buffers. */
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    /* We don't want to modify the projection matrix. */
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity( );
    /* Move down the z-axis. */
    glTranslatef( 0.0, 0.0, -5.0 );
    /* Rotate. */
    glRotatef( angle, 0.0, 1.0, 0.0 );
    if( should_rotate ) {
        if( ++angle > 360.0f ) {
            angle = 0.0f;
        }
    }
    /* Send our triangle data to the pipeline. */
    glBegin( GL_TRIANGLES );
    glColor4ubv( red );
    glVertex3fv( v0 );
    glColor4ubv( green );
    glVertex3fv( v1 );
    glColor4ubv( blue );
    glVertex3fv( v2 );
    glColor4ubv( red );
    glVertex3fv( v0 );
    glColor4ubv( blue );
    glVertex3fv( v2 );
    glColor4ubv( white );
    glVertex3fv( v3 );
    glColor4ubv( green );
    glVertex3fv( v1 );
    glColor4ubv( black );
    glVertex3fv( v5 );
    glColor4ubv( orange );
    glVertex3fv( v6 );
    glColor4ubv( green );
    glVertex3fv( v1 );
    glColor4ubv( orange );
    glVertex3fv( v6 );
    glColor4ubv( blue );
    glVertex3fv( v2 );
    glColor4ubv( black );
    glVertex3fv( v5 );
    glColor4ubv( yellow );
    glVertex3fv( v4 );
    glColor4ubv( purple );
    glVertex3fv( v7 );
    glColor4ubv( black );
    glVertex3fv( v5 );
    glColor4ubv( purple );
    glVertex3fv( v7 );
    glColor4ubv( orange );
    glVertex3fv( v6 );
    glColor4ubv( yellow );
    glVertex3fv( v4 );
    glColor4ubv( red );
    glVertex3fv( v0 );
    glColor4ubv( white );
    glVertex3fv( v3 );
    glColor4ubv( yellow );
    glVertex3fv( v4 );
    glColor4ubv( white );
    glVertex3fv( v3 );
    glColor4ubv( purple );
    glVertex3fv( v7 );
    glColor4ubv( white );
    glVertex3fv( v3 );
    glColor4ubv( blue );
    glVertex3fv( v2 );
    glColor4ubv( orange );
    glVertex3fv( v6 );
    glColor4ubv( white );
    glVertex3fv( v3 );
    glColor4ubv( orange );
    glVertex3fv( v6 );
    glColor4ubv( purple );
    glVertex3fv( v7 );
    glColor4ubv( green );
    glVertex3fv( v1 );
    glColor4ubv( red );
    glVertex3fv( v0 );
    glColor4ubv( yellow );
    glVertex3fv( v4 );
    glColor4ubv( green );
    glVertex3fv( v1 );
    glColor4ubv( yellow );
    glVertex3fv( v4 );
    glColor4ubv( black );
    glVertex3fv( v5 );
    glEnd( );
    /*
    * EXERCISE:
    * Draw text telling the user that 'Spc'
    * pauses the rotation and 'Esc' quits.
    * Do it using vetors and textured quads.
    */
    /*
    * Swap the buffers. This this tells the driver to
    * render the next frame from the contents of the
    * back-buffer, and to set all rendering operations
    * to occur on what was the front-buffer.
    *
    * Double buffering prevents nasty visual tearing
    * from the application drawing on areas of the
    * screen that are being updated at the same time.
    */
    SDL_GL_SwapBuffers();
}
static void setup_opengl( int width, int height )
{
    float ratio = (float) width / (float) height;
    /* Our shading model--Gouraud (smooth). */
    glShadeModel( GL_SMOOTH );
    /* Culling. */
    glCullFace( GL_BACK );
    glFrontFace( GL_CCW );
    glEnable( GL_CULL_FACE );
    /* Set the clear color. */
    glClearColor( 0, 0, 0, 0 );
    /* Setup our viewport. */
    glViewport( 0, 0, width, height );
    /*
    * Change to the projection matrix and set
    * our viewing volume.
    */
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity( );
    /*
    * EXERCISE:
    * Replace this with a call to glFrustum.
    */
    gluPerspective( 60.0, ratio, 1.0, 1024.0 );
}
int main( int argc, char* argv[] )
{
    /* Information about the current video settings. */
    const SDL_VideoInfo* info = NULL;
    /* Dimensions of our window. */
    int width = 0;
    int height = 0;
    /* Color depth in bits of our window. */
    int bpp = 0;
    /* Flags we will pass into SDL_SetVideoMode. */
    int flags = 0;
    /* First, initialize SDL's video subsystem. */
    if( SDL_Init( SDL_INIT_VIDEO ) < 0 ) {
        /* Failed, exit. */
        fprintf( stderr, "Video initialization failed: %s\n",
        SDL_GetError( ) );
        quit_tutorial( 1 );
    }
    /* Let's get some video information. */
    info = SDL_GetVideoInfo( );
    if( !info ) {
        /* This should probably never happen. */
        fprintf( stderr, "Video query failed: %s\n",
        SDL_GetError( ) );
        quit_tutorial( 1 );
    }
    /*
    * Set our width/height to 640/480 (you would
    * of course let the user decide this in a normal
    * app). We get the bpp we will request from
    * the display. On X11, VidMode can't change
    * resolution, so this is probably being overly
    * safe. Under Win32, ChangeDisplaySettings
    * can change the bpp.
    */
    width = 640;
    height = 480;
    bpp = info->vfmt->BitsPerPixel;
    /*
    * Now, we want to setup our requested
    * window attributes for our OpenGL window.
    * We want *at least* 5 bits of red, green
    * and blue. We also want at least a 16-bit
    * depth buffer.
    *
    * The last thing we do is request a double
    * buffered window. '1' turns on double
    * buffering, '0' turns it off.
    *
    * Note that we do not use SDL_DOUBLEBUF in
    * the flags to SDL_SetVideoMode. That does
    * not affect the GL attribute state, only
    * the standard 2D blitting setup.
    */
    SDL_GL_SetAttribute( SDL_GL_RED_SIZE, 5 );
    SDL_GL_SetAttribute( SDL_GL_GREEN_SIZE, 5 );
    SDL_GL_SetAttribute( SDL_GL_BLUE_SIZE, 5 );
    SDL_GL_SetAttribute( SDL_GL_DEPTH_SIZE, 16 );
    SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );
    /*
    * We want to request that SDL provide us
    * with an OpenGL window, in a fullscreen
    * video mode.
    *
    * EXERCISE:
    * Make starting windowed an option, and
    * handle the resize events properly with
    * glViewport.
    */
    flags = SDL_OPENGL | SDL_FULLSCREEN;
    /*
    * Set the video mode
    */
    if( SDL_SetVideoMode( width, height, bpp, flags ) == 0 ) {
        /*
        * This could happen for a variety of reasons,
        * including DISPLAY not being set, the specified
        * resolution not being available, etc.
        */
        fprintf( stderr, "Video mode set failed: %s\n",
        SDL_GetError( ) );
        quit_tutorial( 1 );
    }
    /*
    * At this point, we should have a properly setup
    * double-buffered window for use with OpenGL.
    */
    setup_opengl( width, height );
    /*
    * Now we want to begin our normal app process--
    * an event loop with a lot of redrawing.
    */
    while( 1 ) {
        /* Process incoming events. */
        process_events( );
        /* Draw the screen. */
        draw_screen( );
    }
    /*
    * EXERCISE:
    * Record timings using SDL_GetTicks() and
    * and print out frames per second at program
    * end.
    */
    /* Never reached. */
    return 0;
}
```
编译方法见：[https://blog.csdn.net/Rong_Toa/article/details/80188671](https://blog.csdn.net/Rong_Toa/article/details/80188671)
`$ gcc demo_opengl-2.c `pkg-config --libs --cflags sdl` -lGL -lglut -lGLU -lXmu -Bstatic -Bdyanmic`
结果：
![](https://img-blog.csdnimg.cn/20181209171826673.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1JvbmdfVG9h,size_16,color_FFFFFF,t_70)
# 3 输入处理
## 3.1控制杆处理
### 3.1.1初始化
在一个SDL 程序中使用一个控制杆的第一步是初始化SDL 的控制杆子系统。传入SDL_INIT_JOYSTICK 标志到SDL_Init 函数可以完成这一步。这个控制杆标记通常与其他标志关联（像video 标志）因为这个控制杆经常用于控制某些东西。
例子3——1 初始化SDL 支持控制杆
```cpp
#include "SDL.h" /* All SDL App's need this */
#include <stdio.h>
int main() 
{
    printf("Initializing SDL.\n");
    /* Initialize defaults, Video and SDL_INIT_JOYSTICK */
    if((SDL_Init(SDL_INIT_VIDEO|SDL_INIT_JOYSTICK)==-1)) 
	{
        printf("Could not initialize SDL: %s.\n", SDL_GetError());
        exit(-1);
    }
    printf("SDL initialized.\n");
    printf("Quiting SDL.\n");
    /* Shutdown all subsystems */
    SDL_Quit();
    printf("Quiting....\n");
    exit(0);
}
```
这个将尝试启动SDL 的视频和控制杆子系统。
### 3.1.2询问
如果我们通过询问测试，我们能安全的假设SDL 库已经被初始化且控制杆子系统开始工作。我们现在能调用一些视频和（或）音频功能在我们需要控制杆之前。最后我们必须确认有一个真实的控制杆在工作。即使你知道一个控制杆将在在系统上，它也希望检查因为它能帮助识别控制杆被拔去的时候。这个用于检查控制杆的功能是SDL_NumJoysticks。
这个功能简单的返回控制杆启动数目。如果它至少一个。接下来一步是决定哪个控制杆用户想使用。如果这个控制杆活动个数只有一个然后它会安全的假设这个控制杆是用户想使用的。SDL 有一个功能来获取控制杆由系统分配的名字的功能SDL_JoystickName。这个控制杆被划分为从0 开始作为第一个控制杆和最后一个控制杆，这个数目被SDL_NumJoysticks - 1 返回。示范一个所有活动的控制杆清单被打印到stdout。
例子3——2 询问活动控制杆个数
```cpp
#include "SDL.h" /* All SDL App's need this */
#include <stdio.h>
int main() 
{
    printf("Initializing SDL.\n");
    /* Initialize defaults, Video and SDL_INIT_JOYSTICK */
    if((SDL_Init(SDL_INIT_VIDEO|SDL_INIT_JOYSTICK)==-1)) 
	{
        printf("Could not initialize SDL: %s.\n", SDL_GetError());
        exit(-1);
    }
    printf("SDL initialized.\n");
    
    printf("%i joysticks were found.\n\n", SDL_NumJoysticks() );
    printf("The names of the joysticks are:\n");
    
    int i;
    for( i=0; i < SDL_NumJoysticks(); i++ )
    {
        printf(" %s\n", SDL_JoystickName(i));
    }
    
    printf("Quiting SDL.\n");
    /* Shutdown all subsystems */
    SDL_Quit();
    printf("Quiting....\n");
    exit(0);
}
```
```
$ gcc demo-init-stick.c `pkg-config --libs --cflags sdl`
$ ./a.out 
Initializing SDL.
SDL initialized.
1 joysticks were found.
The names of the joysticks are:
 VirtualPS/2 VMware VMMouse
Quiting SDL.
Quiting....
```
### 3.1.3打开一个控制杆接受控制杆事件
SDL 的事件驱动可以打开一个控制杆工作。控制杆能引发4 种类型的事件
SDL_JoyAxisEvent 事件出发当一个坐标改变
SDL_JoyBallEvent 事件出现当一个控制杆跟踪球位置改变
SDL_JoyHatEvent 事件出现当一个帽子的位置改变
SDL_JoyButtonEvent 事件出现当一个按钮按下或者释放
事件从所有打开的控制杆接受。为了接受控制杆事件首先做的事情是调用SDL_JoystickEventState 加上SDL_ENABLE 功能。例如我们我们只对系统中第一个控制杆事件感兴趣，不管它是啥。从它接受事件我们要做这个：
例3——3 打开一个控制杆
```cpp
SDL_Joystick *joystick;
SDL_JoystickEventState(SDL_ENABLE);
joystick = SDL_JoystickOpen(0);
```
如果我们想接受其他控制杆事件我们需要打开他们而调用SDL_JoystickOpen 就像我们打开控制杆0 一样，除非我们要存储SDL_Joystick结构体（他们返回一个不同类型的指针）我们只需要控制杆指针当我们询问这个控制杆或者当我们关闭这个控制杆。
这之上所有代码我们只是用于初始化控制杆为了实时读取值。所有我们现在需要的是一个事件循环，所有SDL 程序应该有任何方法来接受系统退出事件。我们现在必须添加代码来检查事件循环中至少一些以下注意到的事件。让我们假设我们事件循环像这样：
```cpp
SDL_Event event;
/* Other initializtion code goes here */
/* Start main game loop here */
while(SDL_PollEvent(&event))
{
    switch(event.type)
    {
        case SDL_KEYDOWN:
            /* handle keyboard stuff here */
            break;
        case SDL_QUIT:
            /* Set whatever flags are necessary to */
            /* end the main game loop here */
            break;
    }
}
/* End loop here */
```
处理控制杆事件我们只不过为他们添加case，首先我们将添加坐标处理代码，坐标检查能获取棘手的kinda 因为许多接受的控制杆事件是垃圾。控制杆坐标有一个微小改变趋势只是在它们设计的轨迹路径之间。你必须设置一个门槛来补偿改变和忽略事件如果它们没有超出门槛。10%是一个通常的很好的门槛值。这个听起来比它本身更复杂。这里是一个坐标事件处理
例子3——4 控制杆坐标事件
```cpp
case SDL_JOYAXISMOTION: /* Handle Joystick Motion */
    if ( ( event.jaxis.value < -3200 ) || (event.jaxis.value > 3200 ) )
    {
    /* code goes here */
    }
    break;
```
其他坐标事件特技例如上——下、左——右动作是两个不同不同的轴设置。最重要的坐标是坐标0（左——右）和坐标1（上——下）。在代码中处理它们我们要做以下：
例子3——5 更多控制杆坐标事件
```cpp
case SDL_JOYAXISMOTION: /* Handle Joystick Motion */
    if ( ( event.jaxis.value < -3200 ) || (event.jaxis.value > 3200 ) )
    {
        if( event.jaxis.axis == 0)
        {
            /* Left-right movement code goes here */
        }
        if( event.jaxis.axis == 1)
        {
            /* Up-Down movement code goes here */
        }
    }
    break;
```
理想的代码应该使用event.jaxis.value 来缩放某些东西。例如让我们假设你正使用控制杆来控制太空船的动作。如果用户正使用一个类似的控制杆且他们推动这个杆移动一点，飞船会移动一点。设计你的代码更好的用于这个情况因为它是为使用户类似的控制更好和同样的用户数字控制经验。
如果你的控制杆有任何另外的坐标然后人啊们也许被用于其他杆或者阀门控制且这些坐标返回值为event.jaxis.axis 值。
按钮处理是简单对比坐标检查。
例子3——6 控制杆按钮事件
```cpp
case SDL_JOYBUTTONDOWN: /* Handle Joystick Button Presses */
    if ( event.jbutton.button == 0 )
    {
        /* code goes here */
    }
    break;
```
按钮检查比坐标检查简单因为一个按钮只有按下或者没有按下。这个SDL_JOYBUTTONDOWN 事件被触发当一个按钮被按下且这个SDL_JOYBUTTONUP 事件被触发当一个按钮被释放。我们必须知道啥子按钮被按下， 读取event.jbutton.button 可以完成这个事件。
最后当我们完成使用我们的控制杆我们应该关闭他们调用SDL_JoystickClose。关闭我们开启控制杆0 我们应该在程序的末尾做这件事：
```cpp
SDL_JoystickClose(joystick);
```
整体上：
```cpp
#include "SDL.h" /* All SDL App's need this */
#include <stdio.h>
int main() 
{
    printf("Initializing SDL.\n");
    /* Initialize defaults, Video and SDL_INIT_JOYSTICK */
    if((SDL_Init(SDL_INIT_VIDEO|SDL_INIT_JOYSTICK)==-1)) 
	{
        printf("Could not initialize SDL: %s.\n", SDL_GetError());
        exit(-1);
    }
    printf("SDL initialized.\n");
    
    printf("%i joysticks were found.\n\n", SDL_NumJoysticks() );
    printf("The names of the joysticks are:\n");
    
    int i;
    for( i=0; i < SDL_NumJoysticks(); i++ )
    {
        printf(" %s\n", SDL_JoystickName(i));
    }
    
    SDL_Joystick *joystick;
    SDL_JoystickEventState(SDL_ENABLE);
    joystick = SDL_JoystickOpen(0);
    
    SDL_Event event;
    /* Other initializtion code goes here */
    /* Start main game loop here */
    while(SDL_PollEvent(&event))
    {
        switch(event.type)
        {
            case SDL_KEYDOWN:
                /* handle keyboard stuff here */
                break;
            case SDL_QUIT:
                /* Set whatever flags are necessary to */
                /* end the main game loop here */
                break;
            case SDL_JOYAXISMOTION: /* Handle Joystick Motion */
                if ( ( event.jaxis.value < -3200 ) || (event.jaxis.value > 3200 ) )
                {
                    if( event.jaxis.axis == 0)
                    {
                        /* Left-right movement code goes here */
                    }
                    if( event.jaxis.axis == 1)
                    {
                        /* Up-Down movement code goes here */
                    }
                }
                break;
            case SDL_JOYBUTTONDOWN: /* Handle Joystick Button Presses */
                if ( event.jbutton.button == 0 )
                {
                    /* code goes here */
                }
                break;
        }
    }
    /* End loop here */
    SDL_JoystickClose(joystick);
    
    
    printf("Quiting SDL.\n");
    /* Shutdown all subsystems */
    SDL_Quit();
    printf("Quiting....\n");
    exit(0);
}
```
### 3.1.4优化的控制杆功能
关心好控制那么你能使用在世界上的每种控制杆，但是有一小部分设备SDL不能支持。Joyball 是我们下一个清单目标，它们与我们用的轴有很多的不同。Joyball 存储相对改变不像轴存储绝对改变。一个轨迹球事件即包含了x 轴的改变又包含了y 的改变。下面是例子：
例子3——7 控制杆球事件
```cpp
case SDL_JOYBALLMOTION: /* Handle Joyball Motion */
    if( event.jball.ball == 0 )
    {
        /* ball handling */
    }
    break;
```
下面的检查第一个joyball 在控制杆上。这个位置的改变将被存储到event.jball.xrel 和event.jball.yrel。最后我们有这个hat 事件。Hats 报告只有它们直接压入的。我们检查hat's位置使用bitmasks：
```cpp
/*
SDL_HAT_CENTERED
SDL_HAT_UP
SDL_HAT_RIGHT
SDL_HAT_DOWN
SDL_HAT_LEFT
*/
```
也有一些预定义联合体如下：
```cpp
/*
SDL_HAT_RIGHTUP
SDL_HAT_RIGHTDOWN
SDL_HAT_LEFTUP
SDL_HAT_LEFTDOWN
*/
```
Hat 的例子如下：
例子3——8 控制杆hat 事件：
```cpp
case SDL_JOYHATMOTION: /* Handle Hat Motion */
    if ( event.jhat.hat | SDL_HAT_UP )
    {
        /* Do up stuff here */
    }
    if ( event.jhat.hat | SDL_HAT_LEFT )
    {
        /* Do left stuff here */
    }
    if ( event.jhat.hat | SDL_HAT_RIGHTDOWN )
    {
        /* Do right and down together stuff here */
    }
    break;
```
另外询问控制杆数量和他们的名字有另外的功能来询问连接的控制杆能力：
SDL_JoystickNumAxes 返回控制杆的轴数
SDL_JoystickNumButtons 返回控制杆的按钮数量
SDL_JoystickNumBalls 返回控制杆球的个数
SDL_JoystickNumHats 返回控制杆hats 的个数
使用这些功能我们必须传入控制杆结构体在我们打开控制杆获得的。例如：
例子3——9 询问控制杆特性
```cpp
/*这个代码将会得到第一个控制杆的按钮个数。*/
int number_of_buttons;
SDL_Joystick *joystick;
joystick = SDL_JoystickOpen(0);
number_of_buttons = SDL_JoystickNumButtons(joystick);
```
整体：
```cpp
#include "SDL.h" /* All SDL App's need this */
#include <stdio.h>
int main() 
{
    printf("Initializing SDL.\n");
    /* Initialize defaults, Video and SDL_INIT_JOYSTICK */
    if((SDL_Init(SDL_INIT_VIDEO|SDL_INIT_JOYSTICK)==-1)) 
	{
        printf("Could not initialize SDL: %s.\n", SDL_GetError());
        exit(-1);
    }
    printf("SDL initialized.\n");
    
    printf("%i joysticks were found.\n\n", SDL_NumJoysticks() );
    printf("The names of the joysticks are:\n");
    
    int i;
    for( i=0; i < SDL_NumJoysticks(); i++ )
    {
        printf(" %s\n", SDL_JoystickName(i));
    }
    
    SDL_Joystick *joystick;
    SDL_JoystickEventState(SDL_ENABLE);
    joystick = SDL_JoystickOpen(0);
    
    SDL_Event event;
    /* Other initializtion code goes here */
    /* Start main game loop here */
    while(SDL_PollEvent(&event))
    {
        switch(event.type)
        {
            case SDL_KEYDOWN:
                /* handle keyboard stuff here */
                break;
            case SDL_QUIT:
                /* Set whatever flags are necessary to */
                /* end the main game loop here */
                break;
            case SDL_JOYAXISMOTION: /* Handle Joystick Motion */
                if ( ( event.jaxis.value < -3200 ) || (event.jaxis.value > 3200 ) )
                {
                    if( event.jaxis.axis == 0)
                    {
                        /* Left-right movement code goes here */
                    }
                    if( event.jaxis.axis == 1)
                    {
                        /* Up-Down movement code goes here */
                    }
                }
                break;
            case SDL_JOYBUTTONDOWN: /* Handle Joystick Button Presses */
                if ( event.jbutton.button == 0 )
                {
                    /* code goes here */
                }
                break;
            case SDL_JOYBALLMOTION: /* Handle Joyball Motion */
                if( event.jball.ball == 0 )
                {
                    /* ball handling */
                }
                break;
            /*
            SDL_HAT_CENTERED
            SDL_HAT_UP
            SDL_HAT_RIGHT
            SDL_HAT_DOWN
            SDL_HAT_LEFT
            */
            /*
            SDL_HAT_RIGHTUP
            SDL_HAT_RIGHTDOWN
            SDL_HAT_LEFTUP
            SDL_HAT_LEFTDOWN
            */
            case SDL_JOYHATMOTION: /* Handle Hat Motion */
                if ( event.jhat.hat | SDL_HAT_UP )
                {
                    /* Do up stuff here */
                }
                if ( event.jhat.hat | SDL_HAT_LEFT )
                {
                    /* Do left stuff here */
                }
                if ( event.jhat.hat | SDL_HAT_RIGHTDOWN )
                {
                    /* Do right and down together stuff here */
                }
                break;
        }
    }
    
    /*这个代码将会得到第一个控制杆的按钮个数。*/
    int number_of_buttons = SDL_JoystickNumButtons(joystick);
    
    /* End loop here */
    SDL_JoystickClose(joystick);
    
    
    printf("Quiting SDL.\n");
    /* Shutdown all subsystems */
    SDL_Quit();
    printf("Quiting....\n");
    exit(0);
}
```
## 3.2处理键盘
### 3.2.1键盘相关结构体
你如果熟悉数据类型在键盘接受时将会更加容易明白这个指导，所以我将首先解释他们。
### 3.2.2SDLKey
SDLKey 是一个枚举类型定义在SDL/include/SDL_keysym.h，具体见表（这个表自己看看英文文档去吧，实在太多）。每一个SDLKey 记号描述一个键，SDLK_a 与键盘上的a 按键是一致的，SDLK_SPACE 与键盘上的空格键是一致的，等等。
3.2.3SDLMod
SDLMod 是一个枚举类型， 与SDLKey 类似， 但是它枚举键盘的控制键(Control,Alt,Shift)。这个完整的控制器记号见表（还是看英文文档） SDLMod 值能联合起来描述一些控制。
### 3.2.4SDL_keysym
```cpp
typedef struct{
    Uint8 scancode;
    SDLKey sym;
    SDLMod mod;
    Uint16 unicode;
} SDL_keysym;
```
这个SDL_keysym 结构体描述一个按键按下或者一个按键释放。其中scancode 参数是硬件描述，它应该被忽略除非你知道你在做什么。这个sym 参数是被按下或者释放的SDLKey 的键盘值。这个mod 参数描述键盘控制器在按下或者释放发生状态。所以一个KMOD_NUM|KMOD_CAPS|KMOD_LSHIFT 的意思是Numlock,Capslock 和左边shift 按键全部按下（或者在锁住按键时启动）。最后，这个unicode 参数存储键盘的16 位的unicode 码。
注意：应该注意和明白这个参数只有当SDL_keysym 正在描述一个按键按下有效时候，不是按键释放的时候。Unicode 值只有在按键按下才有意义因为这个unicode 值描述一个国际符号且只有按键按下才产生符号。要查询unicode 的信息要去它的网站去找。
注意：unicode 翻译必须使用SDL_EnableUNICODE 起作用。
### 3.2.5SDL_KeyboardEvent
```cpp
typedef struct{
    Uint8 type;
    Uint8 state;
    SDL_keysym keysym;
} SDL_KeyboardEvent;
```
这个SDL_KeyboardEvent 描述一个键盘事件（显然的）。这个SDL_Event 联合体的key 参数成员是一个SDL_KeyboardEvent 结构体。这个type 参数描述一个事件是键盘释放（SDL_KEYUP）或者是一个键盘按下（SDL_KEYDOWN）事件。这个state 参数是非常的多余，它报告与type 同样的信息但是用不同的值（SDL_RLEASED 和SDL_PRESSED）。这个keysym 包含键盘按下或者释放信息。
### 3.2.6读取键盘事件
从事件队列中读取键盘事件是非常简单的（这个事件队列和它的使用在表中有描述）。我们读取事件使用SDL_PollEvent 在一个while() 循环且检查SDL_KEYUP 和SDL_KEYDOWN 事件使用一个switch 结构，像这样例子3——10 读取键盘事件
```cpp
/* Poll for events. SDL_PollEvent() returns 0 when there are no */
/* more events on the event queue, our while loop will exit when */
/* that occurs. */
while( SDL_PollEvent( &event ) ){
    /* We are only worried about SDL_KEYDOWN and SDL_KEYUP events */
    switch( event.type ){
        case SDL_KEYDOWN:
            printf( "Key press detected\n" );
            break;
        case SDL_KEYUP:
            printf( "Key release detected\n" );
            break;
        default:
            break;
    }
}
```
### 3.2.7一个更加详细的查看
在我们能读取SDL 事件之前， SDL_Init 必须被开启且视频模式必须被设置使用SDL_SetVideoMode 。但是有两个其他功能我们必须使用来获得所有响应信息， 使用SDL_GetKeyName。
注意：unicode 编码值< 0x80 会直接翻译为一个ASCII 符号编码值。这个在下面的例子中使用。
例子3——11 说明键盘事件信息
```cpp
#include "SDL.h"
/* Function Prototypes */
void PrintKeyInfo( SDL_KeyboardEvent *key );
void PrintModifiers( SDLMod mod );
/* main */
int main( int argc, char *argv[] ){
    SDL_Event event;
    int quit = 0;
    /* Initialise SDL */
    if( SDL_Init( SDL_INIT_VIDEO ) ){
    fprintf( stderr, "Could not initialise SDL: %s\n", SDL_GetError() );
    exit( -1 );
    }
    /* Set a video mode */
    if( !SDL_SetVideoMode( 320, 200, 0, 0 ) ){
    fprintf( stderr, "Could not set video mode: %s\n", SDL_GetError() );
    SDL_Quit();
    exit( -1 );
    }
    /* Enable Unicode translation */
    SDL_EnableUNICODE( 1 );
    /* Loop until an SDL_QUIT event is found */
    while( !quit ){
        /* Poll for events */
        while( SDL_PollEvent( &event ) ){
            switch( event.type ){
                /* Keyboard event */
                /* Pass the event data onto PrintKeyInfo() */
                case SDL_KEYDOWN:
                case SDL_KEYUP:
                    PrintKeyInfo( &event.key );
                    break;
                /* SDL_QUIT event (window close) */
                case SDL_QUIT:
                    quit = 1;
                    break;
                default:
                    break;
            }
        }
    }
    /* Clean up */
    SDL_Quit();
    exit( 0 );
}
/* Print all information about a key event */
void PrintKeyInfo( SDL_KeyboardEvent *key ){
    /* Is it a release or a press? */
    if( key->type == SDL_KEYUP )
        printf( "Release:- " );
    else
        printf( "Press:- " );
    /* Print the hardware scancode first */
    printf( "Scancode: 0x%02X", key->keysym.scancode );
    /* Print the name of the key */
    printf( ", Name: %s", SDL_GetKeyName( key->keysym.sym ) );
    /* We want to print the unicode info, but we need to make */
    /* sure its a press event first (remember, release events */
    /* don't have unicode info */
    if( key->type == SDL_KEYDOWN ){
        /* If the Unicode value is less than 0x80 then the */
        /* unicode value can be used to get a printable */
        /* representation of the key, using (char)unicode. */
        printf(", Unicode: " );
        if( key->keysym.unicode < 0x80 && key->keysym.unicode > 0 ){
            printf( "%c (0x%04X)", (char)key->keysym.unicode,
            key->keysym.unicode );
        }
        else{
            printf( "? (0x%04X)", key->keysym.unicode );
        }
    }
    printf( "\n" );
    /* Print modifier info */
    PrintModifiers( key->keysym.mod );
}
/* Print modifier info */
void PrintModifiers( SDLMod mod )
{
    printf( "Modifers: " );
    /* If there are none then say so and return */
    if( mod == KMOD_NONE ){
        printf( "None\n" );
        return;
    }
    /* Check for the presence of each SDLMod value */
    /* This looks messy, but there really isn't */
    /* a clearer way. */
    if( mod & KMOD_NUM ) printf( "NUMLOCK " );
    if( mod & KMOD_CAPS ) printf( "CAPSLOCK " );
    if( mod & KMOD_LCTRL ) printf( "LCTRL " );
    if( mod & KMOD_RCTRL ) printf( "RCTRL " );
    if( mod & KMOD_RSHIFT ) printf( "RSHIFT " );
    if( mod & KMOD_LSHIFT ) printf( "LSHIFT " );
    if( mod & KMOD_RALT ) printf( "RALT " );
    if( mod & KMOD_LALT ) printf( "LALT " );
    if( mod & KMOD_CTRL ) printf( "CTRL " );
    if( mod & KMOD_SHIFT ) printf( "SHIFT " );
    if( mod & KMOD_ALT ) printf( "ALT " );
    printf( "\n" );
}
```
### 3.2.8游戏模式输入
我发现人们在游戏中和其他使用键盘事件和其他交互应用总是一直不明白一个fundemental 点。
键盘事件只有在一个按键从没有按下到按下的状态发生时候，且取代多功能。
想象你有一个图像想从外面移动过来使用光标按键——当你按下左箭头按键你想让它滑动到左边，当你按下这个向下按键你想向下滑动。测试下面的代码，它是许多人会犯的错误。
```cpp
/* Alien screen coordinates */
int alien_x=0, alien_y=0;
/* Initialise SDL and video modes and all that */
/* Main game loop */
/* Check for events */
while( SDL_PollEvent( &event ) ){
    switch( event.type ){
        /* Look for a keypress */
        case SDL_KEYDOWN:
            /* Check the SDLKey values and move change the coords */
            switch( event.key.keysym.sym ){
        case SDLK_LEFT:
            alien_x -= 1;
            break;
        case SDLK_RIGHT:
            alien_x += 1;
            break;
        case SDLK_UP:
            alien_y -= 1;
            break;
        case SDLK_DOWN:
            alien_y += 1;
            break;
        default:
            break;
        }
    }
}
```
首先你也许认为这是一个完美的响应代码，但是这不是。像我说的键盘事件只有当一个键状态改变时发生，所以用户将必须按下且释放这个左箭头按键100 次来移动像右边100像素。
避免这个问题我们必须不使用这个事件来改变外部的位置，我们使用事件来设置用于一个区分选择代码移动到外部标记。就像这样：
例子3——12 适当的游戏移动
```cpp
/* Alien screen coordinates */
int alien_x=0, alien_y=0;
int alien_xvel=0, alien_yvel=0;
/* Initialise SDL and video modes and all that */
/* Main game loop */
/* Check for events */
while( SDL_PollEvent( &event ) ){
    switch( event.type ){
        /* Look for a keypress */
        case SDL_KEYDOWN:
            /* Check the SDLKey values and move change the coords */
            switch( event.key.keysym.sym ){
                case SDLK_LEFT:
                    alien_xvel = -1;
                    break;
                case SDLK_RIGHT:
                    alien_xvel = 1;
                    break;
                case SDLK_UP:
                    alien_yvel = -1;
                    break;
                case SDLK_DOWN:
                    alien_yvel = 1;
                    break;
                default:
                    break;
            }
            break;
        /* We must also use the SDL_KEYUP events to zero the x */
        /* and y velocity variables. But we must also be */
        /* careful not to zero the velocities when we shouldn't*/
        case SDL_KEYUP:
            switch( event.key.keysym.sym ){
                case SDLK_LEFT:
                    /* We check to make sure the alien is moving */
                    /* to the left. If it is then we zero the */
                    /* velocity. If the alien is moving to the */
                    /* right then the right key is still press */
                    /* so we don't tocuh the velocity */
                    if( alien_xvel < 0 )
                    alien_xvel = 0;
                    break;
                case SDLK_RIGHT:
                    if( alien_xvel > 0 )
                    alien_xvel = 0;
                    break;
                case SDLK_UP:
                    if( alien_yvel < 0 )
                    alien_yvel = 0;
                    break;
                case SDLK_DOWN:
                    if( alien_yvel > 0 )
                    alien_yvel = 0;
                    break;
                default:
                    break;
            }
            break;
        default:
        break;
    }
}
/* Update the alien position */
alien_x += alien_xvel;
alien_y += alien_yvel;
```
可以被看到的，我们使用两个额外的差异，alien_xvel 和alien_vvel，描述了船的移动，这些差异我们下载当我们发现键盘按下和释放。
参考/复制粘贴自：《SDL中文资料.pdf》
