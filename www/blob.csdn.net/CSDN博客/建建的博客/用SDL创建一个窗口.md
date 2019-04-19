# 用SDL创建一个窗口 - 建建的博客 - CSDN博客
2017年11月23日 09:54:56[纪建](https://me.csdn.net/u013898698)阅读数：142
个人分类：[视频编解码学习](https://blog.csdn.net/u013898698/article/category/6703370)
原文来自：[http://www.aaroncox.net/tutorials/2dtutorials/sdlwindow.html](http://www.aaroncox.net/tutorials/2dtutorials/sdlwindow.html)
注意：这里我们想当然你已经知道怎么在你的IDE集成开发环境里配置好SDL
创建一个窗口
要用SDL创建一个窗口，设计到以下步骤：
◆初始化SDL视频库
◆创建窗口
◆设置窗口标题
◆关闭SDL
1.初始化SDL视频库
我们用SDL_Init()来初始化SDL视频库，并传入一个参数 SDL_INIT_VIDEO.
2.创建一个窗口
我们用SDL_SetVideoMode()来创建一个窗口，这个函数包含四个参数，依次是窗口的宽度、高度、单个像素占用位数和一个
标记变量。其中第三个参数最为简单的方法是设置为0，这样它就为当前默认的显示设置）。最后一个参数这里我们用
SDL_HWSURFACE和SDL_DOUBLEBUF.联起来使用要用或操作符: SDL_HWSURFACE | SDL_DOUBLEBUF。
SDL_SetVideoMode()函数不仅仅是创建了一个窗口，它还创建了一块内存区域叫"screen buffer"，用来显示图像。这块区
域负责显示画面到屏幕，标记变量SDL_HWSURFACE表示在显存里头创建缓存；SDL_DOUBLEBUF表示我要创建两个缓存区域，一
个用作前端缓存，这里存放的就是我们正在显示到屏幕的内容；另一个用作后端缓存，这里存放的是我们将要显示到屏幕上
的内容。当我们显示将要显示的内容时候，只需要交换前端缓存和后端缓存，这样后端缓存的内容就被显示出来。（译者注
：也就是说前端缓存又变成了后端缓存，可以用来存放下一幅要显示的画面)这项技术叫做双缓存，用来加速图像的渲染过
程。
SDL_SetVideoMode()函数会返回一个指向SDL_Surface类型的screen buffer。SDL_Surface结构代表了一块内存区域，在那
里存放了图像的信息。当我们画图时，只需要将图像数据信息放到这个结构里面。
3.设置窗口标题
我们可以用SDL_WM_SetCaption()来设置窗口的标题，它有两个参数，第一个参数就是标题的内容，第二个参数用来指定一
个自定义的图标，设置为0表示使用默认的图标。
4.关闭SDL
我们呼叫SDL_Quit()函数来关闭SDL。它会释放前面通过SDL_SetVideoMode()返回的SDL_Surface类型的screen
 buffer。
下面的代码创建了一个窗口，当你运行的时候，会发现显示片刻之后，它会关闭。因为我们在创建该窗口之后，紧接着调用
了SDL_Quit()。下一阶段，我们会用到一个循环，让窗口停在那里，知道你点击关闭。
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)#include "SDL.h"
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)constint WINDOW_WIDTH =640;
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)constint WINDOW_HEIGHT =480;
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)constchar* WINDOW_TITLE ="SDL Start";
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)int main(int argc, char**argv)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)
{
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)   SDL_Init( SDL_INIT_VIDEO );
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)   SDL_Surface* screen = SDL_SetVideoMode( WINDOW_WIDTH, WINDOW_HEIGHT, 0,
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)      SDL_HWSURFACE | SDL_DOUBLEBUF );
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)   SDL_WM_SetCaption( WINDOW_TITLE, 0 );
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)   SDL_Quit();
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)return0;
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedBlockEnd.gif)}
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)
保持窗口一直打开。
要保持窗口一直打开，我们创建一个循环，就像下面这样：
bool gameRunning = true;
while (gameRunning)
{
}
当我们要退出循环时候，只需要将gameRunning赋值为false。这里我选择用户关闭的时候给他赋值。
要想侦测到用户关闭窗口，我们呼叫 SDL_PollEvent()函数。这个函数有一个SDL_Event结构体的参数，里面包含了当前发
生事件的信息。函数返回0表示没有事件发生，所以我们需要检查它的返回值来确定所要处理的事件。
虽然SDL_Event结构体里面有各种变量，但是这里我们所要考虑的只有type变量，如果这个变量值为SDL_QUIT，那么我们就
知道了用户想要关闭窗口。(也就是用户按下了了窗口上方的X按钮)
下面的代码用来创建一个窗口，它一直打开知道点击关闭为止。
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)#include "SDL.h"
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)constint WINDOW_WIDTH =640;
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)constint WINDOW_HEIGHT =480;
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)constchar* WINDOW_TITLE ="SDL Start";
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)int main(int argc, char**argv)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)
{
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)   SDL_Init( SDL_INIT_VIDEO );
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)   SDL_Surface* screen = SDL_SetVideoMode( WINDOW_WIDTH, WINDOW_HEIGHT, 0,
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)      SDL_HWSURFACE | SDL_DOUBLEBUF );
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)   SDL_WM_SetCaption( WINDOW_TITLE, 0 );
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)   SDL_Event event;
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)bool gameRunning =true;
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)while (gameRunning)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedSubBlockStart.gif){
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)if (SDL_PollEvent(&event))
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedSubBlockStart.gif){
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)if (event.type == SDL_QUIT)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedSubBlockStart.gif){
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)            gameRunning =false;
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedSubBlockEnd.gif)         }
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedSubBlockEnd.gif)      }
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedSubBlockEnd.gif)   }
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)   SDL_Quit();
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)return0;
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedBlockEnd.gif)}
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)
上面就是我们用SDL创建窗口的过程。
