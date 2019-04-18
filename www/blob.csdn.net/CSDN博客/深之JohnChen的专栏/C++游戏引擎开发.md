# C++游戏引擎开发 - 深之JohnChen的专栏 - CSDN博客

2009年02月06日 10:18:00[byxdaz](https://me.csdn.net/byxdaz)阅读数：36066


**游戏引擎开发**

##### [用C++实现跨平台游戏引擎开发](http://blog.donews.com/xzwenlan/archive/2006/02/22/737453.aspx)

　　你是否梦想写一部格斗游戏但却无从着手呢？是否你只因游戏开发好玩而对之感兴趣？本文我们将分析一个通用的跨平台游戏引擎，每个游戏开发新手都可以自由地使用它。

　　1. 3D游戏引擎的简短历史

　　在游戏开发中，从一开始就确定正确的开发平台是很重要的。是否你的游戏支持Windows，Linux和OS X?是否你的游戏开发只使用OpenGL就足够了？OpenGL是十九世纪九十年代初期设计的，起初只运行于价值约$25，000的Unix CAD工作站上，后来移植到Windows和其它一些低端平台上。与此同时，随着游戏工业的发展，图形加速器价格从$2,000剧跌到你今天看到的价值约$150的大众市场价格。 

　　确实，许多人都会援引在1996年用OpenGL开发成功的革命性的游戏Quake，作为以上急速发展现象的直接的原因。然而，成功的Quake级的游戏开发标准要求更多：世界级音频支持，网络连接，用户输入设备支持，以及实时的管理能力等。既需要实现跨平台支持又能使游戏效果激动人心，要实现这样的解决方案最好建立一个体面的游戏开发站台。

　　2. 用于C++，Java和其它开发语言的简单DirectMedia层

　　对，历史就是这样有趣，但并不是每一部游戏都要做成Quake的克隆品。一直被业界许多人吹捧有着许多优点的选择是简单DirectMedia层(SDML)。这是一套跨平台的多[媒体库](http://www.yesky.com/key/148/165148.html)，它提供对于音频，键盘，鼠标，游戏杆，OpenGL和2D视频帧缓冲的低级存取。SDML支持几乎我能想像出的每一个平台，包括Linux，Windows，所有的MacOS变异物，WinCE，Dreamcast还有另外一些操作系统。它被广泛应用于开发MPEG播放器，硬件仿真器，和许多流行的游戏，包括获奖的运行于Linux平台的Civilization:Call to Power。

　　SDML用C写成，但生来就与C++一起工作，已经绑定到了另外许多语言，包括Ada，Eiffel，Java，Lua，ML，Perl，PHP，Pike，Python和Ruby。SDML的应用环境简直就没有什么限制，而且它碰巧是我最喜爱的开源飞行模拟器GL-[117](http://www.yesky.com/key/3638/168638.html)(见图1)的开发引擎。事实上，513游戏的当前开发已经基于SDML引擎而且被注册到了SDML的主页。

![](http://dev.yesky.com/imagelist/05/08/le2k5in80255.jpg)
图1.GL-117中的一个视图

　　3. 通道视觉效果演示程序

　　研究游戏引擎的最好方法是看一些示例程序代码。简单地看一下图2中用SDML实现的2D通道类型演示图，你就能发现你仅用几行代码所能完成的工作。你可以使用该实例作为一个保护屏程序，音乐可视化动画效果，等等。篇幅所限，我已经整理了实际的绘制代码。请跟随我的注释分析下面对SDML的工作原理的描述:

#include "Tunnel.h"
// SDL 相关变量定义
SDL_Surface *screen,*bBuffer,*Image;
SDL_Rect rScreen,rBuffer;
int main (int argc, char **argv)
{
　int flag = SDL_SWSURFACE;// 请求一个软件表面.
　//软件表面处于系统内存中，
　// 一般不如硬件表面速度快
　#ifdef WIN32
　int fullscreen = MessageBox(NULL, "Show [Full](http://www.yesky.com/key/3384/168384.html) Screen (y/n):","Screen Setting", MB_YESNO);
　if (fullscreen==IDYES) {
　　flag |= SDL_FULLSCREEN; // 如果用户需要，接管整个屏幕
　}
　#endif
　Tunnel_Timer(); // 读取起始的系统时钟值
　SDL_Init( SDL_INIT_VIDEO ); // 初始化视频子系统
　//把屏幕设置到 320x240，32位颜色
　screen = SDL_SetVideoMode( 320, 240, 32, flag);
　// 如果可用的话，为屏幕表面请求硬件缓冲
　bBuffer = SDL_CreateRGBSurface( SDL_HWSURFACE, screen->w,screen->h,screen->format->BitsPerPixel,
　　　　screen->format->Rmask,
　　　　screen->format->Gmask,
　　　　screen->format->Bmask,
　　　　screen->format->Amask);
　// 这是种子图像，一旦开始它就会盘旋起来
　Image = SDL_LoadBMP( "tunnel_map.bmp" );
　Image = SDL_ConvertSurface(Image, screen->format, SDL_HWSURFACE);
　rBuffer.x = 0;
　rBuffer.y = 0;
　rBuffer.w = bBuffer->w;
　rBuffer.h = bBuffer->h;
　// 忽视大多数事件, 包括 鼠标动作， 并取消光标
　SDL_EventState(SDL_ACTIVEEVENT, SDL_IGNORE);
　SDL_EventState(SDL_MOUSEMOTION, SDL_IGNORE);
　SDL_ShowCursor( SDL_DISABLE );
　Tunnel.Set( 320, 240 ); // 通道将填充整个的缓冲区
　Tunnel.Precalc( 16 ); //内部的圆圈直径
　while (SDL_PollEvent(NULL)==0) {
　　float fTime = Tunnel_GetTime();
　　//在修改前，必须锁定表面，特别当缓冲区处于图形硬件内存中时
　　SDL_LockSurface(bBuffer);
　　SDL_LockSurface(Image);
　　Tunnel.Draw(bBuffer, Image, 180*sin(fTime), fTime*100);
　　SDL_UnlockSurface(bBuffer); // 在更新以后你可以开锁
　　SDL_UnlockSurface(Image);
　　// 把缓冲区中的数据输出到屏幕绘图区域并强迫进行重画
　　SDL_BlitSurface( bBuffer, NULL, screen, &rBuffer );
　　SDL_UpdateRect( screen, 0, 0, 0, 0 );
　}
　Tunnel.Free();
}

![](http://dev.yesky.com/imagelist/05/08/e6gw64n5b9z0.gif)
图 2. 演示旋转和扭曲的2D通道

　　4. 对另外一些游戏引擎的探索

　　让我们看一下另外一些开源的游戏引擎。

　　a) ALLEGRO(Allegro低级游戏开发例程)

　　Allegro是一个开源的可移植的库，主要针对视频游戏和多媒体编程。Allegro由Shawn Hargreaves(近来称为Climax)创建，现在成长为一个能够跨越许多操作系统如Linux，Windows，MacOS，MS-DOS和许多另外的流行平台等的游戏系统。

　　除了具有一个高级的2D图形库，它能容易地存取鼠标，键盘，游戏杆和高精度定时器中断。Allegro并没有包装或替换OpenGL，但是通过参观他们广阔的开发站点(http://www.allegro.cc/)，你能学习怎样把OpenGL集成到Allegro游戏程序中。

　　大约有700种不同的游戏工程，与Allegro一起发行，其中最为杰出的两类是街机游戏和谜题游戏。我特别地喜欢经典的街机游戏Zaxxon(见图3)的重制品。

![](http://dev.yesky.com/imagelist/05/08/92l2i0v64gtu.gif)
图3.酷毙的Zaxxon的重制品

　　b) Irrlicht:点燃快速实时的3D引擎

　　这个Irrlicht 引擎是一个跨平台，高性能实时引擎，用C++写成。你可以选择[Direct3D](http://www.yesky.com/key/2279/167279.html)，OpenGL或基于软件的着色技术。高端特点包括动态阴影，粒子系统，人物动画，进门和出门技术和碰撞检测(见图4)。Irrlicht支持Windows和Linux并提供到语言Java，Perl，Ruby等的绑定。业界先驱Nikolaus Gebhardt在他的朋友的少部分帮助下完成的这个引擎工。

![](http://dev.yesky.com/imagelist/05/08/w0mv597xl089.gif)
图4.在Irrlicht中的一个十分逼真的场景

　　c) ClanLib:为多玩家游戏设计的引擎

　　ClanLib提供了一个平台独立的接口来书写游戏-它们有一个共同的到低级库如DirectX和OpenGL的接口。借助于ClanLib，你只需编写少量代码即可在Windows，Linux和OSX系统上开发游戏程序。ClanLib包括一个广泛的声音库，2D碰撞检测，动画，[GUI](http://www.yesky.com/key/4326/169326.html)框架和网络库。图5显示了游戏XenoHammer中的一个场景。

![](http://dev.yesky.com/imagelist/05/08/k290a2y1b1wy.gif)
图5.XenoHammer屏幕快照

##### [用C++实现跨平台游戏开发之Allegro引擎](http://blog.donews.com/xzwenlan/archive/2006/02/22/737449.aspx)

要：本文重点讨论开源游戏开发库Allegro(Allegro低级游戏例程)，同时涉及到一些深度技术并提供了一个简单的示例程序，帮你进一步确定它是否是适合你的开发平台。

**一、 一个适于多环境的引擎**

　　Allegro最开始被研发于八十年代后期古老的Atari ST平台上，随后被快速地移植到流行的DJGPP环境（一个在九十年代早期流行的32位的[MS-DOS](http://www.yesky.com/key/2029/112029.html)扩展程序）。此后，Allegro被移植到最为流行的Windows C++开发环境中，包括VS，MinGW，Cygwin和Borland C++。另外的支持它的平台包括Linux，BeOS，QNX，Mac OSX以及几乎任何其它带有X11库的Unix平台上。

　　Allegro能着色到各种类型的位图和硬件加速的环境中，例如DirectX，XWindows，SVGAlib，FreeBE/AF，CGDirectDisplay，QuickDraw，等等。Allegro并不想提供它自己的3D环境或模拟器，但是OpenGL可以被容易地集成，这是通过使用AllegroGL库-它提供了一个类似于GLUT的接口（包括扩展管理）-实现的。

**二、 性能概要**

　　在进一步使用API开发前，让我们看一下Allegro提供的总体功能：

　　·具体到像素级的绘图函数，包括平坦阴影，gouraud阴影，纹理贴图，z缓冲的多边形和圆绘制，填充，[贝塞尔](http://www.yesky.com/key/2525/92525.html)样条曲线，图案填充，精灵，blitting（位图复制），位图计算缩放和旋转，半透明/光效果以及比例字体支持的文本输出 

　　·FLI/FLC（在计算机生成的动画方面，这种格式比MPEG有更高的压缩性能）动画播放器

　　·播放后台MIDI音乐，可达64种同时的声音效果，并能录制样本[波形](http://www.yesky.com/key/1357/91357.html)和MIDI输入(声音平台支持，包括WaveOut，DirectSound，OSS，ESD，CoreAudio和QuickTime，等等)

　　·容易地存取鼠标，键盘，游戏杆等设备，还支持高分辨率定时器中断，包括一个DOS版本的垂直折回中断模拟器

　　·读/写LZSS压缩文件的例程

　　·数学函数，包括定点算术，表查找和3D矢量/矩阵/四元数操作

　　·GUI对话框管理器和文件选择器

　　·内建地支持16位和[UTF-8](http://www.yesky.com/key/430/100430.html)格式的[Unicode](http://www.yesky.com/key/822/100822.html)字符 

**三、 使用引擎**

　　使用Allegro进行开发，就象在许多其它游戏场合下一样，游戏的总体结构都包括游戏开始前的初始化，游戏循环以及游戏完成后的清理。初始化意味着既包含Allegro启动代码也包含在开始的位置实现基本地装载或生成你的游戏级别。在创建你的初始化代码时，启动Allegro基本上没有什么代价付出(见图1).

　　如果你需要很多屏幕相关的真实性能，建议你首先礼貌地用get_gfx_mode_list()函数查询一下最大可用方式：

#include <allegro.h> //必须放于系统头文件的引用之后
set_color_depth(32); // 缺省情况下使用8位颜色
if (set_gfx_mode(GFX_AUTODETECT, 640, 480, 0, 0) != 0) {
　abort_on_error("Couldn’t set a 32 bit color resolution");
}

　　set_gfx_mode()的最后两个参数用于指定虚拟缓冲区的大小-我们的图形屏幕存储于其中。这可以使创建一个卷边游戏-其中地形是连续移动的-变得容易。例如，你可能要使虚拟缓冲区，比方说，宽出20%以留出足够的空间来平滑卷动新的精灵和地形。

**四、 一个完整的Allegro实例**

　　本教程将使用Kee-Yip Chan的SnookerClone演示程序，它是基于James Lohr的另一个具有相同名字的演示程序。图1显示了演示程序的基本屏幕快照。

![](http://soft.yesky.com/imagelist/05/09/0ah37p241fjz.jpg)
图1.Kee-Yip Chan的"SnookerClone"演示程序

　　这个工程向你展示了许多不同的Allegro技术，包括动画，键盘输入和鼠标输入，碰撞和游戏物理知识(例如重力)。它利用了三个主要的元素:一个有8个扶手的旋转的车轮，一个用箭头键来控制的大红球，还有一些从顶部往下坠落的蓝球。车轮以接触方式推动红球，而当红球碰上蓝球时，它们之间相互影响。

　　下列是完整的Allegro演示程序的代码:

1 #include <allegro.h>
2 vector<Point> g_points; //aka球上点的列表
3 vector<Joint> g_joints; //物理对象列表，如车轮和缓冲器
4 kVec g_accControl;
6 int main(void)
7 {
8 　allegro_init(); // 初始化allegro.
9 　install_keyboard(); // 启动键盘.
10　install_mouse(); // 启动鼠标.
11　install_timer(); //过程show_mouse()所需要;
13　// 创建一个800x600的非全屏窗口.
14　set_gfx_mode(GFX_AUTODETECT_WINDOWED, 800, 600, 0, 0);
16　set_window_title("Kee-Yip Chan’s Snooker Clone");
17　text_mode(-1); // 文本将被画在透明的背景之上
19　BITMAP* buffer = create_bitmap(SCREEN_W, SCREEN_H);
　　//创建一张位图用于双缓冲.
21　 // 初始化数据.
22 create_joints(g_joints); //注册车轮、地板和缓冲器的硬编码的屏幕位置
25 // 创建顶点以组成aka球： 玩家所用球和三个蓝球
26 // 的位置, 速度, 大小和质量.
27 g_points.push_back(Point(kVec(100, 300),kVec(0, 0),16, 10));
// 玩家.
28 g_points.push_back(Point(kVec(50, 40), kVec(0, 0),12, 5));
// 中等的球.
29 g_points.push_back(Point(kVec(80, 40), kVec(0, 0) 12, 5));
//中等的球.
30 g_points.push_back(Point(kVec(110, 40),kVec(0, 0),6, 1));
// 小球.
32 //主循环,在按ESC键后退出
33 while(!key[KEY_ESC]) { //检查输入.
34 　if(key[KEY_UP])
35 　　g_accControl.y = -0.07; //Jet pack.向上加速
36 　if(key[KEY_LEFT])
37 　　g_accControl.x = -0.07; //左走.向左加速
38 　if(key[KEY_RIGHT])
39 　　g_accControl.x = 0.07; //右走.向右加速
41 　　static bool leftMousePressed = false,
　　　　rightMousePressed = false;
42 　if(mouse_b & 1) { //鼠标左键按下
43 　　if(!leftMousePressed){
44 　　　leftMousePressed = true; // 创建一个新球.
45 　　　g_points.push_back(Point(kVec(mouse_x, mouse_y),kVec(0, 0), 12, 5));
46 　　}
47 　}
48 　if(!(mouse_b & 1))
49 　//保证不重复鼠标按键
50 　//否则,就会出现许多的新球
51 　leftMousePressed = false;
52 　if(mouse_b & 2) { //鼠标右键按下
53 　　if(!rightMousePressed){
54 　　　rightMousePressed = true; // 创建一个新球
55 　　　g_points.push_back(Point(kVec(mouse_x, mouse_y),kVec(0, 0), 6, 1));
56 　　}
57 　}
58 　if(!(mouse_b & 2))
59 　　//保证不重复鼠标按键
60 　　//否则,就会出现许多的新球.
61 　　rightMousePressed = false;
63 　　doPhysics();
65 　　// 着色:如果我们能再次使用缓冲区，则清除它;
　　　　//否则,旧图像将滞留显示
66 　　//用白色进行清除.
67 　　clear_to_color(buffer, makecol(255, 255, 255));
68 　　for(unsigned i = 0; i < g_points.size(); i++) {
　　　　　//画点.
69 　　　//画一个实心球
70　　　 circlefill(buffer, //画向缓冲区
71　　　 g_points[i].position.x,g_points[i].position.y,// aka 球的中心点的位置
72　　　 g_points[i].size, // 半径.
73　　　 (i == 0) ? makecol(255, 0, 0) : makecol(0, 0, 255)); //红色如果是玩家;否则为蓝色
75 　　　// 画一个轮廓球.
76　　　 circle(buffer, //画向缓冲区
77　　　　 g_points[i].position.x,g_points[i].position.y, // aka 球的中心点的位置.
78　　　　 g_points[i].size, // 半径.
79　　　　 makecol(0, 0, 0)); //红色如果是玩家;否则为蓝色.
81 　　}
83　　 // 画接合点
84　　 for (unsigned i = 0; i < g_joints.size(); i++)
85　　　 line(buffer, //画向缓冲区
86　　　　 g_joints[i].p1.x, g_joints[i].p1.y, // 点 1.
87 　　　　g_joints[i].p2.x, g_joints[i].p2.y, // 点 2.
88 　　　　makecol(0, 0, 0)); // 黑颜色.
89 　　　);
91 　　// 打印指令.
92　　 textout(buffer, font, "Left Mouse Button - new big ball Right Mouse Button - new small ball",
93 　　　　125, 1, makecol(0, 0, 0));
95 　　textout(buffer, font, "Arrow Keys - move red ball",
96　　　　 300, 592, makecol(0, 0, 0));
98　　 show_mouse(buffer); // 画鼠标光标.
100　　 draw_sprite(screen, buffer, 0, 0);// 把缓冲区中的数据画向屏幕.
101 　} // while循环结束
103 　return 0;
105 }END_OF_MAIN();
　　33-101行包括了典型的游戏编程循环模式。游戏继续进行直到玩家按下ESC键退出为止。34-39行支持同时进行的键盘输入，因为你可以按下向上和向左箭头键来获取粗略的斜向运动。

　　在41-61行,鼠标动作被捕获到全局变量mouse_b(用于按钮)，mouse_x和mouse_y。如果你一直在使用一滚轮鼠标，你还可以使用变量mouse_z。我们对反向弹跳逻辑进行了一点硬编码以确保每次鼠标按下事件只有一个球下落。

　　第63行调用doPhysics()，其目的是旋转车轮的线段，更新球位置，检测球碰撞和适当地改变它们的方向矢量。这个模块(350行的数学代码)有点深入了些，但它确实是一个一流的实现，值得你深入研究。

　　余下的代码，65-101行，开始着色，在典型的示例程序中这属于常规实现部分。这里的着色用典型的双缓冲区技术，下一次屏幕变化被计算出来并进行脱屏绘制并在最的一毫秒进行缓冲交换(第100行)。这确保了视觉的连续性又减少了烦人的闪烁-对象看上去是随机地绘制的。在着色代码部分，对line()和circlefill()的调用是相当直接的：circlefill()以x，y，半径和填充颜色作为参数。

　　textout_ex()函数的功能稍强于textout()(示于92-96行)，允许你指定前景和背景颜色。Allegro提供例程以直接从GRX格式.fnt文件，8x8或8x16 BIOS格式.fnt文件，位图图象以及数据文件格式中装入字体。作为选择，你能导入一种大范围的Unicode字体，这可以通过写一个.txt脚本-它为每一范围的字符指定相应的不同的源文件-来实现。如果你想要支持TrueType，那么你需要AllegroTTF或相同功能的插件。
最后，在第100行的draw_sprite()实现一个覆盖性复制新生成的位图到第14行创建的屏幕对象上。覆盖性复制意指只有非透明的颜色像素被复制。在本例中，我确信它已被退化成一个"blit"(块复制)转储。

**五、 Allegro的音频**

　　这个snooker演示程序只涉及到了一些最基本的图形和I/O函数，但是并没有用到Allegro的音频开发包。该包中的MIDI混频，音响效果和录音API，其效果达到或超过几乎每一个我所见过的专业的声音库。Allegro音频应用软件大量存在，包括WinCab-一个MP3和OGG Vorbis音乐唱片机，还有LouTronic Rhythm Box-一个鼓声生成合成器，它具有可全面融合到一起的snare鼓，低音鼓和hi hat的效果。下面我们简单地回顾Allegro音频API的一小部分。

　　每一个使用音频的程序都应该调用reserve_voices()来指定数字和MIDI声音驱动程序分别使用的声音的数目。接下去，你能控制这些音频轨道的混合.

　　你可以非常容易地象下面这样插入一个音轨：

MIDI *midFile = load_midi("myfile.mid’);
play_midi(midFile, TRUE);//连续循环
　　对于更复杂的需要，你可以安装三个钩子函数之一，它们可以使你拦截MIDI玩家事件。如果被设置为非NULL，这些例程将在每次MIDI消息，元事件和系统独占的数据块中被分别调用。

　　Allegro的数字音频系统被设计为从最基本的配置到可高度扩展的。你能容易安装读取器和写入器来处理新的或者不同的音频文件类型， 例如:

register_sample_file_type("mp3",load_mp3,NULL);//安装MP3读取器
　　当正播放数字音频时，你可以随时编辑它。下列代码改变将在播放一个样本参数时改变该样本(用于操作循环播放的声音):

void adjust_sample(const SAMPLE *spl, int vol, int pan, int freq, int loop);
　　你能改变音量，平移音频数据并清除循环标志，在下次执行到循环末尾时，这将停止该样本。如果在播放相同样本的好几个副本，这会调整它遇到的第一个副本。如果该样本没有播放，对它没有任何影响。 

##### [用C++实现跨平台游戏开发之Irrlicht引擎](http://blog.donews.com/xzwenlan/archive/2006/02/22/737442.aspx)

　　Irrlicht引擎是一个用C++书写的高性能实时的3D引擎，可以应用于C++程序或者.NET语言中。通过使用[Direct3D](http://www.yesky.com/key/3962/38962.html)（Windows平台），OpenGL 1.2或它自己的软件着色程序，可以实现该引擎的完全跨平台。尽管是开源的，该Irrlicht库提供了可以在商业级的3D引擎上具有的艺术特性，例如动态的阴影，粒子系统，角色动画，室内和室外技术以及碰撞检测等(见图1)。

![](http://dev.yesky.com/imagelist/05/09/s583kng9ok1v.jpg)
图1.Irrlicht 3D引擎

　　Irrlicht是一个德国神话故事中的一种动物的名字，它能够发光和飞翔，可以在大部分的沼泽地附近发现它。单词"Irrlicht"是两个德国单词（"irr"意思是疯狂的；而"Licht"意思是光）的组合。在英语中，它被译为"鬼火"。

　　Irrlicht十分幸运地为一个巨大的活跃的开发团队以大量的工程所支持。然而，因为Irrlicht主要由游戏名家Nikolaus Gebhardt所设计，所以该游戏在设计上十分连贯。你可以在网上到处发现有Irrlicht的增强程序，如可选用的地形生成器，入口生成器，输出器，world层生成器，相关教程和编辑器等。而且，它独立地创建了到Java，Perl，Ruby，BASIC，Python，LUA甚至更多种语言的绑定。而最为重要的是，它是完全自由的。

**二、 Irrlicht特性**

　　在深入分析API之前，请让我更具体地介绍一下Irrlicht提供给了3D游戏开发者哪些功能： 

　　·一个可以运行于Linux以及Windows 98，ME，NT，2000和XP（MacOS在计划之中）等操作系统之上的引擎

　　·针对Direct3D 8生成器或Direct3D 9生成器(可选)提供了Anti-aliasing支持

　　·可换肤的GUI环境(包括一个很酷的具有金属质地的带阴影的皮肤)，给一些老式的对话框加上漂亮的外观

　　·场景管理系统，它允许无缝的室内/室外过渡

　　·角色动画系统，带有骨骼和变形目标动画功能

　　·一个特殊的效果系统，包括粒子效果(雨，烟，火，雪，等等)，告示板，灯光贴图，环境，地图，模板缓冲区阴影，雾，纹理动画，视差贴图，[凹凸](http://www.yesky.com/key/2435/32435.html)贴图，还有更多

　　·内建的材质支持，包括支持Pixel and Vertex Shaders版本1.1到3.0，ARB Fragment and Vertex程序以及HLSL（GLSL正在计划中）

　　·.NET语言绑定，这使得引擎可用于所有的.NET语言例如C#，Visual Basic.NET以及Delphi.NET

　　·一内建的平台独立的软件生成器，特性有：z-缓冲，Gouraud阴影，alpha混合和透明性，还有快速的2D绘图(见图2)

　　·你久已期待的2D绘图功能，例如alpha混合，基于关键色的位图复制，字体绘制，以及混合3D与2D图形

　　·能直接导入常见的建模文件格式:Maya，3DStudio [Max](http://www.yesky.com/key/1908/36908.html)，COLLADA，DeleD，Milkshape，[Quake](http://www.yesky.com/key/116/35116.html) 3 levels，Quake2 models，DirectX，Pulsar，My3DTools，FSRad以及Cartography Shop

　　·能直接从BMP，PNG，Photoshop，JPEG，[Targa](http://www.yesky.com/key/4268/34268.html)和PCX导入纹理

　　·快速而易用的碰撞检测与响应

　　·为快速的3D运算和容器模板库进行了优化处理

　　·直接读取档案(可能是压缩的，如.zip文件)

　　·集成了快速的XML分析器

　　·为实现容易的本地化开发提供Unicode支持

![](http://dev.yesky.com/imagelist/05/09/t8ex7l6e12x8.jpg)
图2:基于Irrlicht的游戏Yet Another Space Shooter(YASS)，这里显示的是一个静态游戏帧中的令人吃惊的着色效果
**三、 在Irrlicht中的特殊效果**

　　在本文的例子中，我将向你展示怎样使用模板缓冲区影子技术，还有粒子系统，告示板，动态光以及水表面场景结点等技术。参见图3。

![](http://dev.yesky.com/imagelist/05/09/6jz90ent5gpt.gif)
图3.结合动态的光和水进行的场景着色

　　Irrlicht引擎自动地检查是否你的硬件支持模板缓冲；而如果不支持，则不启动阴影。在这个演示程序中，在方法createDevice()中的’shadows’标志被置位，以产生从一个动画角色投下的动态影子。如果这个实例程序在你的PC上运行太慢，可以把这个标志设置为false或者干脆再买一块更好些的图形加速卡。

　　为能够使用Irrlicht.DLL文件，你需要链接到Irrlicht.lib库文件。你可以在工程设置对话框中设置这个选项；但是为了容易实现，你可以使用一个pragma预编译注释命令。方法createDevice()负责实例化根对象-它使用引擎完成一切事情。参数如下:

　　·deviceType:设备类型。当前你可选取Null设备以及软设备，如DirectX8，DirectX9或OpenGL。

　　·windowSize:要创建的窗口的大小或全屏幕模式。这个例子中使用512x384。

　　·bits:每像素位数(当在全屏幕情况时)。仅允许值为16或者32。

　　·fullscreen:指定是否你想使设备运行于全屏幕方式。

　　·stencilbuffer:指定是否你想使用模板缓冲区以用于绘制阴影。

　　·vsync:指定是否你想启动vsync(仅在全屏幕情况)，可选。

　　·eventReceiver:一个接收事件的对象，可选。

　　为适合于本实例环境，你将装载一个3D Studio Max文件（一幢房子）。该房子看起来并没有什么特别的，但是Irrlicht引擎能为你创建一个相当酷的纹理贴图。只需使用造型操纵器并为之创建一个planar纹理贴图即可：

#include <irrlicht.h>
#include <iostream>
using namespace irr;
#pragma comment(lib, "Irrlicht.lib")
int main()
{
　//让我们假定用户在本例中使用OpenGL
　//当然，也可以指定DirectX 8, 9, 等等.
　video::E_DRIVER_TYPE driverType = video::EDT_OPENGL;
　//创建设备，如果创建失败立即退出。
　IrrlichtDevice *device = createDevice(driverType,
　core::dimension2d(640, 480), 16, false, true);
　if (device == 0)
　　return 1;
　video::IVideoDriver* driver = device->getVideoDriver();
　scene::ISceneManager* smgr = device->getSceneManager();

　　我对从这个导入文件产生的发射光线颜色的效果并不满意。下列代码显示怎样实现这些步骤：

scene::IAnimatedMesh* mesh = smgr->getMesh("room.3ds");
smgr->getMeshManipulator()->makePlanarTextureMapping(
mesh->getMesh(0), 0.008f);
scene::ISceneNode* node = 0;
node = smgr->addAnimatedMeshSceneNode(mesh);
node->setMaterialTexture(0, driver->getTexture("wall.jpg"));
node->getMaterial(0).EmissiveColor.set(0,0,0,0);
**　四、 水动画**

　　你将添加的第一个特殊的效果是水动画。为此，WaterSurfaceSceneNode导入一个造型文件并使之象水表面一样地波动。如果你让这个场景结点使用一种相当好的材质如MT_REFLECTION_2_LAYER，那么它看起来相当酷：

mesh = smgr->addHillPlaneMesh("myHill",
core::dimension2d(20,20),
core::dimension2d(40,40), 0, 0,
core::dimension2d(0,0),
core::dimension2d(10,10));
node = smgr->addWaterSurfaceSceneNode(mesh->getMesh(0),3,300,30);
node->setPosition(core::vector3df(0,7,0));
node->setMaterialTexture(0,driver->getTexture("water.jpg"));
node->setMaterialTexture(1,driver->getTexture("stones.jpg"));
node->setMaterialType(video::EMT_REFLECTION_2_LAYER);
　　作为输入造型，你可以创建一个陡峭的平面造型，但是你也可以为此使用任何其它的造型。你甚至能重用room.3ds输入文件(它看上去确实很奇怪)。该实例还用一个普通的石头纹理模型来绘制所有另外的表面。

**五、透明的告示板和灯光**

　　第二个特殊的效果是很基本的但是非常有用:一个透明的告示板，伴之有一个动态的灯光。为产生这种效果，你只需要产生一个灯光场景结点，并让它四处飞行；而且，为了让它看起来更酷一些，可以把一个告示板场景结点依附到它上面：

//创建灯光
node = smgr->addLightSceneNode(0, core::vector3df(0,0,0),
video::SColorf(1.0f, 0.6f, 0.7f, 1.0f), 600.0f);
scene::ISceneNodeAnimator* anim = 0;
anim = smgr->createFlyCircleAnimator(core::vector3df(0,150,0),250.0f);
node->addAnimator(anim);
anim->drop();
// 把告示板依附到灯光
node = smgr->addBillboardSceneNode(node, core::dimension2d(50, 50));
node->setMaterialFlag(video::EMF_LIGHTING, false);
node->setMaterialType(video::EMT_TRANSPARENT_ADD_COLOR);
node->setMaterialTexture(0,driver->getTexture("particlewhite.bmp"));

**六、 粒子系统**

　　下面介绍的这个特别效果更有趣：一个粒子系统。在Irrlicht引擎中，粒子系统既是组件化的，也是可扩展的，但是仍然易于使用。你只需要简单地把粒子发射器放到一个粒子系统场景结点，这样以来粒子看上去没有产生源。这些发射器可以据需要进行灵活配置，并经常带有许多参数，如粒子方向，粒子数量，以及粒子颜色等。 

　　当然，发射器类型有区别(例如，一个点发射器能够使粒子从一个固定的点上发出粒子)。如果该引擎提供的粒子发射器还不能满足你的要求，你可以容易地创建你自己的发射器。这只需简单地从IParticleEmitter接口派生一个新类并使用setEmitter()方法把它依附到粒子系统上去即可。

　　下一个实例将创建一个盒子粒子发射器。你可能已经猜出，它从一个跳跃的盒中随机生成粒子。由参数来定义盒子，粒子的方向，每秒产生粒子的最小和最大数目，颜色以及粒子的最小和最大生命周期。

　　一个完全由发射器组成的粒子系统将是令人生厌的，因为缺乏真实感。因此，Irrlicht支持粒子影响器-它负责在粒子到处飞扬时予以修整。一旦添加到粒子系统上，它们就能模仿另外的更真实的效果，象重力或风。在本例中的粒子影响器只是简单地修改粒子的颜色来产生一种淡出效果。

　　可能你已经猜出，粒子影响器是通过派生IParticleAffector接口实现的，然后通过使用addAffector()方法把它添加到粒子系统上去。在你为该粒子系统设置了一种好看的材质后，你就有了一个看上去相当酷的野外宿营火的效果。通过调整材质，纹理，粒子发射器，还有影响器参数，你能容易地创建烟雾，下雨，爆炸，下雪等效果：

scene::IParticleSystemSceneNode* ps = 0;
ps = smgr->addParticleSystemSceneNode(false);
ps->setPosition(core::vector3df(-70,60,40));
ps->setScale(core::vector3df(2,2,2));
ps->setParticleSize(core::dimension2d(20.0f, 10.0f));
scene::IParticleEmitter* em = ps->createBoxEmitter(
core::aabbox3d(-7,0,-7,7,1,7),
core::vector3df(0.0f,0.03f,0.0f),
80,100,
video::SColor(0,255,255,255), video::SColor(0,255,255,255),
800,2000);
ps->setEmitter(em);
em->drop();
scene::IParticleAffector* paf =ps->createFadeOutParticleAffector();
ps->addAffector(paf);
paf->drop();
ps->setMaterialFlag(video::EMF_LIGHTING, false);
ps->setMaterialTexture(0, driver->getTexture,"particle.bmp"));
ps->setMaterialType(video::EMT_TRANSPARENT_VERTEX_ALPHA);
**七、 影子投射**

　　最后但也不容忽视一个问题是，你需要为一个动画角色产生一个动态的影子。为此，你装载一个Quake2.md2模型文件并把它放到你的world上去。为了创建影子，你只需要调用方法addShadowVolumeSceneNode()。你可能通过调用ISceneManager::setShadowColor()来控制影子的颜色；注意，这仅是全局可调整的，并影响所有的影子。好，下面就是你的产生动态影子效果的代码：

mesh = smgr->getMesh("../../media/faerie.md2");
scene::IAnimatedMeshSceneNode* anode = 0;
anode = smgr->addAnimatedMeshSceneNode(mesh);
anode->setPosition(core::vector3df(-50,45,-60));
anode->setMD2Animation(scene::EMAT_STAND);
anode->setMaterialTexture(0, driver->getTexture("../../media/Faerie5.BMP"));
anode->addShadowVolumeSceneNode();
smgr->setShadowColor(video::SColor(220,0,0,0));
**八、 游戏循环**

　　最后，你能进入由device->run()方法控制的游戏循环。该循环将不断运行，直到通过获取一个关闭窗口事件(例如在Windows操作系统下的ALT-F4击键)来退出设备。你必须在一个beginScene()和endScene()命令对之间绘制每样东西。beginScene()用指定的一种颜色清屏，如果需要的话，可以同时清除深度缓冲区。然后你就可以让场景管理器和GUI环境来绘制它们的内容。随着调用endScene()，每一样东西都被绘制到屏幕上去。在本例中，你还可以动态地在标题栏上显示帧每秒（FPS）数，这对于严肃的游戏开发者是十分重要的事情：

scene::ICameraSceneNode* camera = smgr->addCameraSceneNodeFPS();
camera->setPosition(core::vector3df(-50,50,-150));
int lastFPS = -1;
while(device->run())
{
　driver->beginScene(true, true, 0);
　smgr->drawAll();
　driver->endScene();
　int fps = driver->getFPS();
　if (lastFPS != fps)
　{
　　core::stringw str = L"Campfire FX example [";
　　str += driver->getName();
　　str += "] FPS:";
　　str += fps;
　　device->setWindowCaption(str.c_str());
　　lastFPS = fps;
　}
}
device->drop();
　　结束循环后，你必须删除先前用createDevice()方法创建的Irrlicht设备。通过使用Irrlicht引擎，你应该删除所有你用以’create’开头的方法或函数创建的所有对象。你可以通过简单地调用device->drop()来删除该设备对象。

**九、你可能喜欢的Irrlicht插件**

　　正如在前面所介绍的，Irrlicht有一群勤奋的独立开发人员并为之产生了大量的插件，也用之开发了相当多的游戏。这些开发者中提出的许多的改进被再次集成到Irrlicht的随后的发行版本中。下面我列举其中的几个例子，我想这会吸引许多颇有前程的开发者感兴趣：

　　·[OCTTools](http://www.constantthought.com/project/OCTTools)，是一套用于Irrlicht的工具，由Murphy McCauley所创建，用于操作OCT文件相关的:输出器，加载器，甚至更多。

　　·[ICE](http://www.technomagicum.net/projects/IrrLicht/ICE/)(Irrlicht通用引擎)是一个开发框架，它提供了一个工程的轮廓实现，从而加快了新工程的开发。

　　·[MIM](http://www.constantthought.com/project/MIM)，由Murphy McCauley所创建，是一个非常有用的基于XML的文件格式，可用于Irrlicht的加载器，转换器及其各种工具。

　　·[My3D](http://my3dproject.nm.ru/)是一个开发工具包，它能够使你把来自于各种3D包(3DStudio MAX，Giles，等等)中的灯光贴图场景直接输出到Irrlicht中。

　　·[Dusty](http://www.daveandrews.org/dustyengine.php)引擎允许程序员创建"任务"-这些"任务"可以完成程序员想做的任何事情。之后，这些任务被添加到一棵普通的任务树上去，而每个任务可以有它们希望数目的孩子任务。任务"组"允许游戏设计者在一棵完整的树上执行普通的操作，例如暂停，继续或破坏等。 

　　·[Irrlicht RPG](http://www.technomagicum.net/projects/IrrLicht/RPG/)(Erring Light)是一个3D 绕行走游戏引擎，最初是针对RPG类游戏开发的。

　　·[2](http://www.dev.your3dgames.com/)[D 图像和精灵](http://www.dev.your3dgames.com/)类组成了一个很有用的库，它扩展了Irrlicht的2D能力。

　　·[Zenprogramming](http://zenprogramming.tripod.com/)站点，提供第一个针对Irrlicht的非正式的外部地形生成器，此处也提供很多相关的教程。 


