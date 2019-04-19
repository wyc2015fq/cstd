# linux之x86裁剪移植---字符界面sdl开发入门 - xqhrs232的专栏 - CSDN博客
2016年05月11日 11:42:02[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：353
原文地址::[http://blog.csdn.net/mao0514/article/details/51336718](http://blog.csdn.net/mao0514/article/details/51336718)
linux下有没有TurboC2.0那样的画点、线、圆的图形函数库，有没有grapihcs.h，或者与之相对应或相似的函数库是什么？有没有DirectX这样的游戏开发库？SDL就是其中之一。
    SDL（Simple DirectMedia Layer）是一个夸平台的多媒体游戏支持库，其中包含了对图形、声音、游戏杆、线程等的支持，目前可以运行在许多平台上，其中包括linux的 FrameBuffer控制台、svgalib、X Window环境，以及Windows DirectX、BeOS等。SDL是编写夸平台游戏和多媒体应用的优秀平台，与Windows的DirectX有的一比。主页：http:
 //www.libsdl.org。
    SDL库几乎已经成了目前流行的Linux的标配的多媒体库，系统安装时一般都已经默认安装了它们。利用SDL库开发应用程序，首先，要在程序中声明要使用的相应的头文件，比如：#include <SDL/SDL.h>，然后，在编译时指出要连接的SDL库即可，比如：gcc -lSDL test.c -o test。SDL库一般位于系统的标准头文件目录/usr/include里，编译器会在这个目录里找相应的头文件，如果要进一步省略“SDL/”，则必须在编译时指定头文件的具体位置，例如：gcc
 -I /usr/include/SDL -lSDL test.c -o test。也可以：gcc `sdl-config-libs-cflags` test.c -o test。“｀”不是单引号，而是位于键盘左上方的反引号。
    要在linux控制台字符界面的环境下进行图形开发，还要打开framebuffer功能，方法是修改/boot/grub/grub.conf配置文件，在kernel...一行后面添加vga=0x317。如下：
title Fedora Core (2.6.15-1.2054_FC5)
    root (hd0,5)
    kernel /vmlinuz-2.6.15-1.2054_FC5 ro root=LABEL=/ rhgb quiet vga=0x0317
    initrd /initrd-2.6.15-1.2054_FC5.img
关于VGA值与显示器分辨率的关系如表：
    640X480        800X600        1024X768    1280X1024
8位色    0x301        0x303        0x305        0x307
16位色    0x311        0x314        0x317        0x31A
24位色    0x312        0x315        0x318        0x31B
初始化图形模式
要加载和初始化SDL库需要调用SDL_Init()函数，该函数以一个参数来传递要激活的子系统的标记，返回-1表示初始化失败。
下表列出来SDL的各个子系统：
标记        表示
SDL_INIT_VIDEO        视频子系统
SDL_INIT_AUDIO        音频子系统
SDL_INIT_CDROM        光驱子系统
SDL_INIT_TIMER        计时器子系统
SDL_INIT_JOYSTICK    游戏杆子系统
SDL_INIT_EVERYTHING    全部子系统
要同时激活多个子系统，可以把相应的标记按位或，如：SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO);
初始化SDL库之后，还需要设置一下视频模式，通过调用SDL_SetVideoMode()来完成：
SDL_Surface *screen;
screen=SDL_etVideoMode(640,480,16,SDL_SWSURFACE);/*640 X 480 X 16位色*/
SDL_Surface 定义在SDL_video.h中，它是一个绘图平面，所有的绘图操作都是在其上完成的。在退出图形模式时由SDL自动处理。不需要用时需显示的释放：SDL_FreeSurface(surface);
先来看一下一个完整的例子：
//ex_sdl.c
#include <stdlib.h>
#include <SDL.h>
int main()
{
    SDL_Surface *screen;
    Uint32 color;
    if ( SDL_Init( SDL_INIT_VIDEO) < 0 ) {
        fprintf(stderr, "无法初始化SDL: %s\n", SDL_GetError());
        exit(1);
    }
SDL_ShowCursor(0);
    screen = SDL_SetVideoMode(640, 480, 16, SDL_SWSURFACE);  /*640 X 480 X 16位色*/
    if ( screen == NULL ) {
        fprintf(stderr, "无法设置640x480x16位色的视频模式：%s\n", SDL_GetError());
        exit(1);
    }
    atexit(SDL_Quit);
    color = SDL_MapRGB(screen->format,0,0,255);  /*蓝色*/
    SDL_FillRect(screen,&screen->clip_rect,color);  /*整个屏幕填充颜色*/
    SDL_UpdateRect(screen,0,0,0,0);              /*更新整个屏幕*/
    SDL_Delay(5000);                          /*延迟５秒钟*/
}
atexit(SDL_Quit);的作用是在程序退出时调用SDL_Quit()函数，这样就不必在每个要退出的地方都调用SDL_Quit()。
