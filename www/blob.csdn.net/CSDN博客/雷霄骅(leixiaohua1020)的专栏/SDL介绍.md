# SDL介绍 - 雷霄骅(leixiaohua1020)的专栏 - CSDN博客





2013年09月23日 23:37:00[雷霄骅](https://me.csdn.net/leixiaohua1020)阅读数：39
个人分类：[纯编程](https://blog.csdn.net/leixiaohua1020/article/category/1647545)










SDL库的作用说白了就是封装了复杂的视音频底层操作，简化了视音频处理的难度。

以下转自WiKi：



SDL（Simple DirectMedia Layer）是一套开放源代码的跨平台多媒体开发库，使用C语言写成。SDL提供了数种控制图像、声音、输出入的函数，让开发者只要用相同或是相似的代码就可以开发出跨多个平台（Linux、Windows、Mac OS X等）的应用软件。目前SDL多用于开发游戏、模拟器、媒体播放器等多媒体应用领域。
 SDL使用GNU宽通用公共许可证为授权方式，意指动态链接（dynamic link）其库并不需要开放本身的源代码。因此诸如《雷神之锤4》等商业游戏也使用SDL来开发。


 结构
 虽然SDL时常被比较为‘跨平台的DirectX’，然而事实上SDL是定位成以精简的方式来完成基础的功能，它大幅度简化了控制图像、声音、输出入等工作所需撰写的代码。但更高级的绘图功能或是音效功能则需搭配OpenGL和OpenAL等API来达成。另外它本身也没有方便创建图形用户界面的函数。
 SDL在结构上是将不同操作系统的库再包装成相同的函数，例如SDL在Windows平台上其实是DirectX的再包装，旧版本包装的是DirectX 5，现时的版本（SDL 1.2）则是DirectX 7。而在使用X11的平台上（包括Linux），SDL则是与Xlib库沟通来输出图像。
 虽然SDL本身是使用C语言写成，但是它几乎可以被所有的编程语言所使用，例如：C++、Perl、Python（借由pygame库）、Pascal等等，甚至是Euphoria、Pliant这类较不流行的编程语言也都可行。
 SDL库分为 Video、Audio、CD-ROM、Joystick 和 Timer 等若干子系统，除此之外，还有一些单独的官方扩充函数库。这些库由官方网站提供，并包含在官方文档中，共同组成了SDL的“标准库”：
 SDL_image—支持时下流行的图像格式：BMP、PPM、XPM、 PCX、GIF、JPEG、PNG、TGA。
 SDL_mixer—更多的声音输出函数以及更多的声音格式支持。
 SDL_net—网络支持。
 SDL_ttf—TrueType字体渲染支持。
 SDL_rtf—简单的RTF渲染支持。

![](https://img-blog.csdn.net/20130923233206828?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


 子系统
 SDL将功能分成下列数个子系统（subsystem）：
 Video（图像）—图像控制以及线程（thread）和事件管理（event）。
 Audio（声音）—声音控制
 Joystick（摇杆）—游戏摇杆控制
 CD-ROM（光盘驱动器）—光盘媒体控制
 Window Management（视窗管理）－与视窗程序设计集成
 Event（事件驱动）－处理事件驱动
 以下是一支用C语言写成、非常简单的SDL示例：





```cpp
// Headers
#include "SDL.h"
 
// Main function
int main(int argc, char* argv[])
{
    // Initialize SDL
    if(SDL_Init(SDL_INIT_EVERYTHING) == -1)
        return(1);
 
    // Delay 2 seconds
    SDL_Delay(2000);
 
    // Quit SDL
    SDL_Quit();
 
    // Return
    return 0;
}
```





SDL与DirectX比较对应关系：


|**SDL**|**DirectX**|**说明**|
|----|----|----|
|SDL_Video、SDL_Image、OpenGL|DirectDraw、Direct3D||
|SDL_Audio、SDL_Mixer|DirectSound||
|SDL_Joystick、SDL_Base|DirectInput||
|SDL_Net|DirectPlay||
|SMPEG、SDL_Video、SDL_Audio、SDL_Sound、SDL_Filter|DirectShow||


 使用SDL开发的游戏:

《Spore》
 《Tux Paint》
 《Simutrans》
 《Stepmania》
 《Frozen Bubble》
 《无冬之夜》（Neverwinter Nights）
 《魔法门之英雄无敌III》（Heroes of Might and Magic III）（Linux版）
 《模拟城市3000》（Sim City 3000）（Linux版）
 《虚幻竞技场》（Unreal Tournament）（Linux版）
 《虚幻竞技场2003》（Unreal Tournament 2003）（Linux版）
 《虚幻竞技场2004》（Unreal Tournament 2004）（Linux版）
 《第二人生》（Second Life）（Linux版）
 《佣兵战场》（Soldier of Fortune）（Linux版）
 《韦诺之战》（Battle for Wesnoth）
 Bos Wars




