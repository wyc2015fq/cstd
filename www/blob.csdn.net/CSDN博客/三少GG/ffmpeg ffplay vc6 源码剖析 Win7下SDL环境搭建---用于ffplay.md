# ffmpeg/ffplay vc6 源码剖析||Win7下SDL环境搭建---用于ffplay - 三少GG - CSDN博客
2011年11月21日 19:41:39[三少GG](https://me.csdn.net/scut1135)阅读数：2740标签：[include																[windows																[objective c																[smalltalk																[haskell																[freebsd](https://so.csdn.net/so/search/s.do?q=freebsd&t=blog)](https://so.csdn.net/so/search/s.do?q=haskell&t=blog)](https://so.csdn.net/so/search/s.do?q=smalltalk&t=blog)](https://so.csdn.net/so/search/s.do?q=objective c&t=blog)](https://so.csdn.net/so/search/s.do?q=windows&t=blog)](https://so.csdn.net/so/search/s.do?q=include&t=blog)
个人分类：[流媒体基础](https://blog.csdn.net/scut1135/article/category/934830)
## [ffmpeg/ffplay vc6 源码剖析](http://www.cnblogs.com/mcodec/articles/1933754.html)
Posted on 2011-01-12 12:41 [mcodec](http://www.cnblogs.com/mcodec/) 阅读(1859) [评论(17)](http://www.cnblogs.com/mcodec/articles/1933754.html#commentform)[编辑](http://www.cnblogs.com/mcodec/admin/EditArticles.aspx?postid=1933754)[收藏](http://www.cnblogs.com/mcodec/articles/1933754.html#)![](http://www.cnblogs.com/mcodec/aggbug/1933754.html?type=2&webview=1)
ffmpeg/ffplay是当今多媒体领域的王者，很多很多的人想研究学习ffmpeg/ffplay，但苦于ffmpeg/ffplay庞大的代码量，令人望而生畏。为帮助更多的人研习ffmpeg/ffplay，在保持ffmpeg/ffplay体系架构的完整性的前提下，把ffmpeg/ffplay大规模的瘦身后，研习门槛一下子降低了n多个数量级。附件一个是对瘦身后的ffmpeg/ffplay的代码完整的剖析pdf文档，另一个是瘦身后的ffmpeg/ffplay的完整源代码，最大化帮助各位网友研究学习ffmpeg/ffplay。
特别注意：
1：如果VC6 debug模式跑出错误了，就用VS2005跑。
2：重新上传了ffsrc.7z，修正了一个内存泄露，改正了黑屏的问题(不好意思，以前上传的时候不小心，传的是debug的版本)。  
pdf档下载地址：[http://files.cnblogs.com/mcodec/ffdoc.7z](http://files.cnblogs.com/mcodec/ffdoc.7z)
源代码下载地址：[http://files.cnblogs.com/mcodec/ffsrc.7z](http://files.cnblogs.com/mcodec/ffsrc.7z)
分类: [ffmpeg/ffplay](http://www.cnblogs.com/mcodec/category/213431.html)
标签: [ffmpeg ffplay vc6 源码剖析](http://www.cnblogs.com/mcodec/tag/ffmpeg%20ffplay%20vc6%20%E6%BA%90%E7%A0%81%E5%89%96%E6%9E%90/)
++++++++++++++++++++++++++++
1.
**[WINDOWS下SDL环境的搭建](http://blog.csdn.net/yanghangjun/article/details/5985329)**
**软件获取：**
**[http://www.libsdl.org/download-1.2.php](http://www.libsdl.org/download-1.2.php) 。**
**目前没有VC9.0版本，我们可以下载VC8.0版本。**
**设置SDL库：**
 解压缩SDL-devel-1.2.13-VC8.zip，docs里面包含了官方文档，这将是你学习SDL的主要参考资料。找到你在硬盘上安装VC的位置，类似：
C:/Program Files/Microsoft Visual Studio 9.0/VC
打开include文件夹，在里面建立一个新文件夹，取名为SDL，打开这个新的文件夹：
C:/Program Files/Microsoft Visual Studio 9.0/VC/include/SDL
然后，将SDL Development Libraries中include文件夹里面的文件全部拷贝到刚才建立起来的那个新文件夹中。
然后，回到VC的./VC文件夹下，打开lib文件夹：
C:/Program Files/Microsoft Visual Studio 9.0/VC/lib
将SDL Development Libraries中lib文件夹下的SDL.lib和SDLmain.lib两个文件拷贝到刚才的那个VC的lib文件夹下。 (或者如2中在工程中设置include和lib目录
最后，**把SDL Development Libraries中lib文件夹下的SDL.dll文件拷贝到C:/WINDOWS/system32下。当然，另外一个选择是将SDL.dll随时绑定到使用了SDL库编译的*.exe文件所在的文件夹中。这通常在发布你的程序的时候使用。**
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
2.
[Windows下的SDL安装](http://blog.chinaunix.net/space.php?uid=9563036&do=blog&id=352008) (2006-05-26 11:00)
分类： [SDL代码片段](http://blog.chinaunix.net/space.php?uid=9563036&do=blog&frmd=0&classid=23781&view=me)
首先你要下载并安装SDL开发包。
如果装在C盘下，路径为C:\SDL1.2.5
如果在WINDOWS下。你可以按以下步骤：
**1.打开VC++，点击"Tools",Options2,点击directories 选项3.选择"Include files"增加一个新的路径。"C:\SDL1.2.5\include"4，现在选择"Libary files“增加"C:\SDL1.2.5\lib"现在你可以开始编写你的第一个SDL程序了.**
这里有一个简单的程序。目的是在窗口里显示一幅图片。
步骤：
1.我们先用SDL_Init()初始化SDL_video系统。
2,我们设置程序的窗口标题
3，我们创建窗口并用SDL_SetVideoMode()取得平面(surface)的
指针（呵呵，很像DDRAW吧里的缓冲面）但比DDRAW初始化简单太
多了。
4.将背景图片载入临时平面(surface）
5，用SDL_DisplayFormat()创建一个和主平面相同的拷贝做为临
时平面。（这就是类似DDRAW双缓冲吧）可以加快渲染速度。
6.调用SDL_FreeSurface释放临时平面占用内存
7.现在在"main loop"中不断的检测事件和更新屏幕。
8.当用户点击ESC或"q",停止循环并退出
9，释放内存。
用SDL_FreeSuface释放平面
用SDL_Quit()清空SDL
下面是代码：
#include "SDL.h"
int main ( int argc, char *argv[] )
{
  /* initialize SDL */
  SDL_Init(SDL_INIT_VIDEO);
  /* set the title bar */
  SDL_WM_SetCaption("SDL Test", "SDL Test");
  /* create window */
  SDL_Surface* screen = SDL_SetVideoMode(640, 480, 0, 
0);
  /* load bitmap to temp surface */
  SDL_Surface* temp = SDL_LoadBMP("hello.bmp");
  /* convert bitmap to display format */
  SDL_Surface* bg = SDL_DisplayFormat(temp);
  /* free the temp surface */
  SDL_FreeSurface(temp);
  SDL_Event event;
  int gameover = 0;
  /* message pump */
  while (!gameover)
  {
    /* look for an event */
    if (SDL_PollEvent(&event)) {
      /* an event was found */
      switch (event.type) {
        /* close button clicked */
        case SDL_QUIT:
          gameover = 1;
          break;
        /* handle the keyboard */
        case SDL_KEYDOWN:
          switch (event.key.keysym.sym) {
            case SDLK_ESCAPE:
            case SDLK_q:
              gameover = 1;
              break;
          }
          break;
      }
    }
    /* draw the background */
    SDL_BlitSurface(bg, NULL, screen, NULL);
    /* update the screen */
    SDL_UpdateRect(screen, 0, 0, 0, 0);
  }
  /* free the background surface */
  SDL_FreeSurface(bg);
  /* cleanup SDL */
  SDL_Quit();
  return 0;
}
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
### [SDL是什么？](http://blog.csdn.net/yanghangjun/article/details/5989263)
分类： [SDL](http://blog.csdn.net/yanghangjun/article/category/749216)2010-11-05
 11:0624人阅读[评论](http://blog.csdn.net/yanghangjun/article/details/5989263#comments)(0)[收藏](http://blog.csdn.net/yanghangjun/article/details/5989263)[举报](http://blog.csdn.net/yanghangjun/article/details/5989263#report)
SDL是Simple DirectMedia Layer（简易直控媒体层）的缩写。它是一个跨平台的多媒体库，以用于直接控制底层的多媒体硬件的接口。这些多媒体功能包括了音频、键盘和鼠标（事件）、游戏摇杆等。当然，最为重要的是提供了2D图形帧缓冲（framebuffer）的接口，以及为OpenGL与各种操作系统之间提供了统一的标准接口以实现3D图形。从这些属性我们可以看出，SDL基本上可以认为是为以电脑游戏为核心开发的多媒体库。
        SDL支持主流的操作系统，包括Windows和Linux。在官方的介绍中，我们可以找到它所支持的其他平台。（SDL supports Linux, Windows, Windows CE, BeOS, MacOS, Mac OS X, FreeBSD, NetBSD, OpenBSD, BSD/OS, Solaris, IRIX, and QNX. ）。SDL本身从C语言开发，并且能很好的在C++等高级语言中使用。在官方可以看到SDL所支持的语言很多。（Including Ada, C#, Eiffel,
 Erlang, Euphoria, Guile, Haskell, Java, Lisp, Lua, ML, Objective C, Pascal, Perl, PHP, Pike, Pliant, Python, Ruby, Smalltalk, and Tcl. ）
        SDL在GNU LGPL version 2下发布，这意味着你可以免费的使用。并且可以免费的用于商业软件的制作（只要你直接使用SDL的动态链接库，Windows下的SDL.dll）。如果你将SDL库编译进了自己的二进制代码中，你需要指明你所使用的SDL库的版本以及包括你自己修改的源代码，并说明这些代码的原始出处。这是很宽松的法律，你可以用如此强大的多媒体库完全合法的免费开发商业游戏。
