# 介绍一个开源的 C++ 开发框架 openFrameworks 。 - DoubleLi - 博客园






作为一个图形图像方向的研究生，我经常都在和 OpenGL 、OpenCV 等多种 C++ 库打交道。这些库遵循着不同的规则和用法；另外，为了让自己的程序具有更多的交互能力，编写界面也是一个家常便饭的工作。

然而，随着工程复杂性的增加，库的管理和界面的维护也变得越来越困难：一方面，库的增加和删除不仅会增加学习成本，也会对系统的逻辑层带来影响。而另一方面，如果要让自己的项目易于维护，就要尽可能地应用设计模式，让逻辑和界面分离。但对于科研，一味陷入设计模式的桎梏又会带来过早优化的问题，影响科研进度。

直到后来，我接触到了 [openFrameworks](http://openframeworks.cc/) ，简直有种相逢恨晚的感觉。openFrameworks 封装了常用的 C++ 库，在此基础上提供了一个直观统一的接口，也大幅简化了编写界面的流程，使得开发图形程序变得很轻松。

本文将为大家介绍这个让人着迷的开发框架 —— openFrameworks。

## 什么是 openFrameworks

openFrameworks（以下简称 oF） 是一个开源的、跨平台的 C++ 工具包，它的设计目的为开发创造过程提供一个更加简单和直观的框架。

![openFrameworks](http://www.hahack.com/images/openframeworks-intro/0.jpg)

oF 的强大之处在于，它不仅是一个通用的胶水（glue），同时它还封装了多种常用的库，包括：
- [OpenGL](http://www.opengl.org/)、[GLEW](http://glew.sourceforge.net/)、[GLUT](http://www.opengl.org/resources/libraries/glut/)、[libtess2](https://code.google.com/p/libtess2/)、[cairo](http://cairographics.org/) - 用于处理图形；
- [rtAudio](http://www.music.mcgill.ca/~gary/rtaudio/)、[PortAudio](http://www.portaudio.com/)、[OpenAL](http://http//connect.creativelabs.com/openal)、[Kiss FFT](http://kissfft.sourceforge.net/)、[FMOD](http://www.fmod.org/) - 用于音频的输入、输出和分析；
- [FreeType](http://freetype.sourceforge.net/index2.html) - 用于字体显示；
- [FreeImage](http://freeimage.sourceforge.net/) - 用于图像存储和载入；
- [Quicktime](http://developer.apple.com/quicktime/)、[GStreamer](http://gstreamer.freedesktop.org/)、[videoInput](https://github.com/ofTheo/videoInput) - 用于视频播放和截取；
- [Poco](http://pocoproject.org/) - 用于开发网络应用；
- [OpenCV](http://opencv.org/) - 用于计算机视觉；
- [Assimp](http://assimp.sourceforge.net/) - 用于读入 3D 模型。

这些库虽然遵循着不同的规则和用法，但 oF 在它们基础上提供了一个通用的接口，使得使用它们变得很容易。

除此之外，oF 的另一亮点在于它具有很好的跨平台特性。目前它支持 5 种操作系统（Windows、OSX、Linux、iOS、Android）以及 4 种 集成开发环境（XCode、Code::Blocks、Visual Studio、Eclipse）。

## 安装和配置 oF

下面介绍如何在 Linux 下安装和配置 oF 。

### 下载 oF

访问 oF 的[官方下载页面](http://openframeworks.cc/download/)，找到适用于你的操作系统和 IDE 的版本，点击下载。例如，我的计算机是 Linux Arch 64位的系统，所以选择的是 code::blocks (64 bit)。

![下载 openFrameworks](http://www.hahack.com/images/openframeworks-intro/download.png)

### 安装依赖

下载完成后，将其解压，开启终端，`cd` 到解压后目录，例如：



```
$ cd $HOME/Documents/programming/openFrameworks
```



之后，根据你的 Linux 发行版的不同，`cd` 进入 *scripts/linux/<操作系统发行版名称>* ，例如：



```
$ cd scripts/linux/archlinux
```



执行两个命令，安装 code::block 和其他依赖（需要 root 权限）：



```
$ sudo ./install_codeblocks.sh


$ sudo ./install_dependencies.sh


$ sudo ./install_codecs.sh
```



### 编译 oF

安装完依赖后，回到上一级目录：



```
$ cd ..
```



编译 oF：



```
$ ./compileOF.sh
```



编译过程中，如果你和我一样遇到找不到 freetype.h 的问题，可能是 FreeType 在 2.5.1 之后改变了头文件的结构导致的。需要将根目录里的 `/libs/openFrameworks/graphics/` 目录下的 ofTrueTypeFont.cpp 开头部分改为：



```
#include "ft2build.h"


/* Corrected setup of include files for freetype as of 2.5.1 dh


#include "freetype2/freetype/freetype.h"


#include "freetype2/freetype/ftglyph.h"


#include "freetype2/freetype/ftoutln.h"


#include "freetype2/freetype/fttrigon.h"


*/


#include FT_FREETYPE_H


#include FT_GLYPH_H


#include FT_OUTLINE_H


#include FT_TRIGONOMETRY_H
```



此时的 oF 已经可以工作了。我们可以测试它提供的示例。cd 到根目录里的 */examples/gui/guiExample/* 目录，编译该工程并执行：



```
$ make


$ make run
```



将会运行一个界面如下图的程序。与左侧面板里的控件交互将可以改变该形状的属性1 1多阅读 example 的示例代码是个好习惯。。

![guiExample 程序](http://www.hahack.com/images/openframeworks-intro/example.png)

### 编译项目生成器

为了方便日后创建工程，oF 还提供了一个项目生成器 projectGenerator 。使用它前同样需要先编译。回到 `compileOF.sh` 脚本所在的目录，敲入如下命令：



```
$ ./compilePG.sh
```



完成后，在 oF 的根目录下找到 projectGenerator 目录，进去里面可以找到 projectGenerator ，我们可以执行它：



```
$ cd ../../projectGenerator


$ ./projectGenerator
```



程序界面如下图所示。点击左侧的每个黑色按钮将可以修改项目名、生成路径，以及依赖的插件（Addon)。

![projectGenerator](http://www.hahack.com/images/openframeworks-intro/projectGenerator.png)

点击右下角的 GENERATE PROJECT 按钮后，将会在 Path 字段指定的路径中生成一个项目，如上图所示就是 `/home/ehome/Documents/programming/openframeworks/apps/myApps/mySketch` ：



```
$ cd /home/ehome/Documents/programming/openframeworks/apps/myApps


$ ls


addons.make bin config.make Makefile mySketch.cbp mySketch.workspace src
```



其中：
- addons.make 文件 - 用于维护这个工程所依赖的插件列表；
- config.make 文件 - 用于添加查找路径，修改优化标记以及其他的设置；
- Makefile 文件 - 工程的 Makefile ，一般不需要直接修改它。在 oF 中，make 的目标包括：
- Debug：生成带有调试标记的可执行程序；
- Release：生成经编译器优化的可执行程序；
- clean：清除目标文件和可执行程序；
- CleanDebug：只清除 debug 目标的生成结果；
- CleanRelease：只清除 release 目标的生成结果；
- help：打印帮助信息；
- run：执行生成的可执行程序。

- mySketch.cbp 和 mySketch.workspace 文件 - Code::Blocks 的工程文件。

### 注册环境变量

我们可以看看 Makefile 文件的内容：



```
$ cat Makefile


# Attempt to load a config.make file.


# If none is found, project defaults in config.project.make will be used.


ifneq ($(wildcard config.make),)


include config.make


endif


 


# make sure the the OF_ROOT location is defined


ifndef OF_ROOT


OF_ROOT=../../..


endif


 


# call the project makefile!


include $(OF_ROOT)/libs/openFrameworksCompiled/project/makefileCommon/compile.project.mk
```



如上所示，编译 openFrameWorks 的工程时，系统需要从 oF 的根路径中引入另一个名为 `compile.project.mk` 的 Makefile，这个根路径存储在 `OF_ROOT` 变量中，默认值是 `../../..` ，即当前目录往上三级的目录。之所以使用这个默认值，是因为使用 projectGenerator 生成的项目都默认存放在 `oF根目录/apps/myApps` 目录下。为了方便在其他地方创建和编译工程，可以人为地定义一个 `OF_ROOT` 变量。将下面这一行添加到用户主目录下的 `.bashrc` 文件中：



```
export OF_ROOT=<你的 oF 根目录>
```



## 入门实例

接下来将介绍如何开发基于 oF 的 C++ 程序2 2主要参考了 [Jeff Crouse](http://jeffcrouse.info/) 所编写的教程 [ofTutorials - Chapter 1 - Getting Started](http://openframeworks.cc/tutorials/introduction/001_chapter1.html)。。

### testApp.cpp

双击 mySketch.cbp 文件，打开 Code::Blocks 开发环境，在左边的项目管理器中双击打开 test.App 文件。如下图所示：

![使用 Code::Blocks 开发程序](http://www.hahack.com/images/openframeworks-intro/codeblocks.png)

testApp.cpp 将会是你的好朋友。在编辑窗口中，你可以看到如下的内容：



```
#include "testApp.h"


 


//--------------------------------------------------------------


void testApp::setup(){


 


}


 


//--------------------------------------------------------------


void testApp::update(){


 


}


 


//--------------------------------------------------------------


void testApp::draw(){


 


}


 


//--------------------------------------------------------------


void testApp::keyPressed(int key){


 


}


 


...
```



上面的代码包含了 4 类函数：
- `setup` - 这个函数将在应用程序生命期的最开始就被调用，甚至在你编写的程序窗口打开之前。利用这个函数，我们可以做一些准备工作，例如在窗口打开之前，先修改窗口的大小；
- `update` 和 `draw` - 当 `setup` 函数运行完成后，系统将进入一个 `update` 和 `draw` 不断交替运行的循环，这个循环将持续到程序结束。也就是说，`setup()` 运行完成后，`update()` 开始运行，然后是 `draw()` ，然后又是 `update()` ，然后又是 `draw()` …… 3 3这个交替频率就是帧率，它的默认值取决于你的电脑的处理速度。`update()` 通常用来更新你的程序的状态（例如改变变量的值），而 `draw()` 则常用来在你的窗口中绘制内容；
- `keyPressed`、`keyReleased`、`mouseMoved`、`mouseDragged`、`mousePressed`、`mouseReleased`、`windowResized`, `gotMessage`、`dragEvent` - 与前面三种函数不同，这类函数仅当用户触发某类事件才会被调用。

我们先试着直接编译这个项目，此时的程序窗口里还没有东西：

![](http://www.hahack.com/images/openframeworks-intro/957951e.png)

### 绘制图形

之后，我们可以试着在窗口中画一个圆。oF 提供了 [`ofCircle`](http://openframeworks.cc/documentation/graphics/ofGraphics.html#!show_ofCircle) 函数用于绘制圆。

往 `draw` 函数里头添加一句内容，：



```
void testApp::draw(){


ofCircle(200, 300, 60);


}
```



第二行告诉系统在坐标 (200, 300) 处画一个半径为 60 的圆。

![](http://www.hahack.com/images/openframeworks-intro/9579GAl.png)

### 添加颜色

现在这个圆看起来很单调，可以给这个圆添加颜色。oF 提供了 [`ofSetColor`](http://openframeworks.cc/documentation/graphics/ofGraphics.html#!show_ofSetColor) 函数用于设置颜色。将 `draw()` 函数改为：



```
void testApp::draw(){


ofSetColor(255, 0, 255);


ofCircle(200, 300, 60);


}
```



新加的这一行（第2行）告诉系统在绘制图形前选择一个颜色，这个颜色的 R、G、B 三原色的色值分别为 (255, 0, 255) 。

![](http://www.hahack.com/images/openframeworks-intro/9579TKr.png)

我们可以用同样的方法再画一个青色的圆：



```
void testApp::draw(){


ofSetColor(255, 0, 255);


ofCircle(200, 300, 60);


 


ofSetColor(0, 255, 255);


ofCircle(500, 500, 100);


}
```



![](http://www.hahack.com/images/openframeworks-intro/9579gUx.png)

### 其他的形状

除了画圆，oF 也可以画其他的图案：
- `ofRect` - 画一个矩形。参数是：(x, y, width, height) ；
- `ofTriangle` - 画一个三角形。参数是三个顶点的坐标：(x1, y1, x2, y2, x3, y3)
- `ofLine` - 画一条线段。参数是两个端点的坐标 (x1, y1, x2, y2)
- `ofEllipse` - 画椭圆。参数是：(x, y, width, height)
- `ofCurve` - 画一条从点 (x1, y1) 到 (x2, y2) 的贝塞尔曲线，曲线的形状由两个控制点 (x0, y0) 和 (x3, y3) 控制 4 4[贝塞尔曲线](http://zh.wikipedia.org/wiki/%E8%B2%9D%E5%A1%9E%E7%88%BE%E6%9B%B2%E7%B7%9A) 的控制点比较难以掌握。如果你用过 Photoshop 里的钢笔工具，你大概就会明白是怎么一回事。。

### 让形状动起来

接下来我们将编写代码让窗口里的图形动起来。主要的思路就是用两个变量控制圆的坐标，然后在程序的运行过程中改变这个变量。在 test.cpp 文件的开头声明两个变量，分别用于存放圆的 x 坐标和 y 坐标：



```
#include "testApp.h"


 


int myCircleX;


int myCircleY;
```



在 `setup()` 函数中为这两个变量添加初始值 5 5别忘了前面介绍的[每类函数的用途](http://www.hahack.com/codes/openframeworks-intro/#testapp.cpp)。：



```
void testApp::setup(){


myCircleX = 0;


myCircleY = 200;


}
```



用这两个变量绘制图形：



```
void testApp::draw(){


ofSetColor(255, 0, 255);


ofCircle(myCircleX, myCircleY, 60);


}
```



要在运行过程中修改这两个变量，可以在 `update()` 函数中编写相关代码。例如，让这个圆一直向右移动，当超出屏幕时，再回到原来开始的地方：



```
void testApp::update(){


myCircleX++;


if (myCircleX > ofGetWindowWidth())


myCircleX = 0;


}
```



其中，第 3 行的 `ofGetWindowWidth()` 函数用来获取窗口的宽度6 6如果不考虑拉伸窗口，也可以用 1024 这个值代替，因为 oF 的默认窗口大小是 1024x768 。。

### 改变帧率

你可能会发现上面的程序在运行的时候有一个问题：圆圈的运动存在时快时慢的情况。如前面所说，这是由于你的程序的帧率，或者说 `update()` 函数和 `draw()` 函数交替执行的频率不稳定造成的。在 `draw()` 函数中添加下面这一行代码可以在窗口的左上方显示帧率信息：



```
ofDrawBitmapString(ofToString(ofGetFrameRate())+"fps", 10, 15);
```



你可以发现这个数值会在程序运行的过程中存在较大波动，尤其是当你同时还在执行其他耗费计算资源的任务时，这个数值会下降得更加明显，相应的这个圆圈的运动速度也会跟着变慢。

让窗口中的动画变得更加平滑的方法是把帧率限制在一个合理的值，例如 60 fps ：



```
void testApp::setup(){


ofSetFrameRate(60);


 


myCircleX = 300;


myCircleY = 200;


}
```



如果你觉得经过这么一改动之后这个圆圈慢的让你无法忍受，你可以通过修改圆圈的移动速度来加速。例如：



```
void testApp::update(){


myCircleX+=4;


if (myCircleX > ofGetWindowWidth())


myCircleX = 0;


}
```



### 添加交互

接下来，我们将为这个程序添加键盘和鼠标的交互。要添加键盘交互，可以通过修改 `keyPressed()` 函数和 `keyReleased()` 函数来完成。其中，`keyPressed()` 捕获的是按下键盘按键的事件，而 `keyReleased()` 捕获的是松开键盘按键的事件 7 7额外提一下， oF 似乎并不能很好的识别 DVORAK 等其他键盘布局。解决方法见[这个帖子](http://forum.openframeworks.cc/t/dvorak-keyboard-input-is-not-recognised/13772)。。

例如：



```
void testApp::keyPressed(int key){


if('w' == key || 'W' == key)


{


myCircleY-=4;


}


if('s' == key || 'S' == key)


{


myCircleY+=4;


}


if('a' == key || 'A' == key)


{


myCircleX-=4;


}


if('d' == key || 'D' == key)


{


myCircleX+=4;


}


}
```



将通过 `w` 、`s`、`a`、`d` 四个按键控制圆圈的运动。出于鲁棒性考虑，小写和大写的字母都要考虑进去，因为按键是通过十进制的 ASCII 码来判断的，而大写字母和小写字母的 ASCII 码是不同的。上面的代码也可以等价的用 ASCII 码来代替8 8温馨小提示：Linux 下可以通过 `man ascii`查询每个字母对应的 ASCII 编码。一般人我不告诉他。：



```
void testApp::keyPressed(int key){


if(119 == key || 87 == key) // w key


{


myCircleY-=4;


}


if(115 == key || 83 == key) // s key


{


myCircleY+=4;


}


if(97 == key || 65 == key) // a key


{


myCircleX-=4;


}


if(100 == key || 68 == key) // d key


{


myCircleX+=4;


}


}
```



添加鼠标事件则通过修改 `mouseMoved()` 、`mouseDragged()`、`mousePressed()` 和 `mouseReleased()` 来完成，顾名思义，分别捕获的是鼠标的移动、拖拽、单击、松开操作。



```
void testApp::mouseMoved(int x, int y ){


 


}


 


void testApp::mouseDragged(int x, int y, int button){


 


}


 


void testApp::mousePressed(int x, int y, int button){


 


}


 


void testApp::mouseReleased(int x, int y, int button){


 


}
```



例如，我们可以编写代码实现鼠标拖动圆圈：



```
void testApp::mouseDragged(int x, int y, int button){


if (0 == button) { // left button


float distance = ofDist(myCircleX, myCircleY, x, y);


 


if (distance < 100){


myCircleX = x;


myCircleY = y;


}


}


}
```



第 2 行用于判断触发此事件的按键是否为左键；第 3 行的 `ofDist()` 函数用于计算鼠标当前位置和圆心的距离。如果这个距离小于半径 100 ，则可以判断当前鼠标落在这个圆圈的范围以内，可以用鼠标的位置代替圆心的位置。

### 其他优化

#### 调整圆圈精度

如果近一点观察圆圈，你可能会发现圆圈的周围有点粗糙。

![](http://www.hahack.com/images/openframeworks-intro/9579SeA.png)

可以修改圆圈的绘制精度来让圆圈更加平滑。在 `setup()` 函数中添加这一句：



```
ofSetCircleResolution(120);
```



![](http://www.hahack.com/images/openframeworks-intro/9579foG.png)

#### 抗锯齿和垂直同步

抗锯齿和垂直同步也是常常使用的优化画面的手段：



```
ofSetVerticalSync(true); 


ofEnableSmoothing();
```



## 实用的插件

oF 的另外一大杀手锏在于它的社区非常活跃，现在已经开发出了数量可观的第三方插件。这里只收集了极小一部分实用插件。更全面的插件列表可以访问 [ofxaddons.com](http://ofxaddons.com/)9 9什么？有墙？！其实几乎所有插件都是托管在 Github 上的。所以如果在 Github 上搜 “ofx” ，也可以找到这些 oF 插件哦。。
- [ofxUI](http://github.com/rezaali/ofxUI) - 华丽丽的 UI 库。提供了很多新颖而实用的界面控件。
- [ofxCv](http://github.com/kylemcdonald/ofxCv) - OpenCV 的另一套可选的 oF 插件（oF 自带一个 oFOpenCv 插件）；
- [ofxLibRocket](https://github.com/underdoeg/ofxLibRocket) - 对 [librocket](http://librocket.com/) 库的封装，这个库允许你使用 html 和 css 来布局 C++ 窗口；
- [ofxTrueTypeFontUC](https://github.com/hironishihara/ofxTrueTypeFontUC) - 对 ofTrueTypeFont 类的扩展，使其支持 Unicode 字符（例如汉字）；
- [ofxPCL](http://github.com/satoruhiga/ofxPCL) - 对 PCL（一个专门用于处理点云的库） 的封装；
- [ofxTimeline](https://github.com/YCAMInterlab/ofxTimeline) - 一个用来绘制可编辑的 timeline 控件的插件；
- [ofxMidi](http://github.com/danomatika/ofxMidi) - Midi 音乐的插件；
- [ofxSpeech](http://github.com/latrokles/ofxSpeech) - 语音识别插件；
- [ofxVideoRecorder](https://github.com/timscaffidi/ofxVideoRecorder) - 录制视频插件；
- [ofxImageSequence](http://github.com/Flightphase/ofxImageSequence) - 一个用于像播放视频一样播放图像序列的插件；
- [ofxGifEncoder](http://github.com/jesusgollonet/ofxGifEncoder) - 生成 Gif 动画的插件；
- [ofxVolumetrics](http://github.com/timscaffidi/ofxVolumetrics) - 简单的体绘制插件；
- [ofxDelaunay](http://github.com/obviousjim/ofxDelaunay) -
- [ofxFft](https://github.com/kylemcdonald/ofxFft) - 对两个用于进行傅里叶变换的库 FFTW 和 KissFFT 的封装；
- [ofxNodejs](http://github.com/satoruhiga/ofxNodejs) - 桥接 Node.js 的插件；
- [ofxLua](http://github.com/danomatika/ofxLua) - 桥接 Lua 的插件；
- [ofxBox2d](http://github.com/vanderlin/ofxBox2d) - 对流行的 2D 物理模拟库 [box2d](http://box2d.org/about/) 的封装；
- [ofxBullet](https://github.com/NickHardeman/ofxBullet) - 对另一个物理模拟库 [Bullet Physics](http://bulletphysics.org/wordpress/) 的封装；
- [ofxLearn](https://github.com/genekogan/ofxLearn) - 通用的机器学习插件，支持分类、回归、聚类等任务；
- [ofxJSON](http://github.com/jefftimesten/ofxJSON) - 对 Json 库 [JsonCpp](http://www.hahack.com/codes/openframeworks-intro/jsoncpp.sourceforge.net/%E2%80%8E) 的封装；
- [ofxHttpServer](http://github.com/arturoc/ofxHttpServer) - 一个基于 [libmicrohttpd](https://www.gnu.org/s/libmicrohttpd) 的 http 服务器插件；
- [ofxAddonTemplate](https://github.com/openframeworks/ofxAddonTemplate) - 一个空的目录框架，可以借鉴它自己编写插件（这都有……`--bb`）。

使用这些插件的方法很简单：
- 访问这个插件的 Github 项目主页；
- 复制它的代码仓库地址；
- 进入你的 oF 根目录下的 addons 目录，`git clone` 这个项目；
- 如果这个项目自带 example ，可以直接 `make && make run` 编译和执行它看看结果。

## 相关链接
- [About openFrameworks](http://www.openframeworks.cc/about/)：更多关于 openFrameworks 的资料，包括设计方法；
- [tutorials](http://openframeworks.cc/tutorials/)：更多 openFrameworks 的教程；
- [Official Documentation](http://www.openframeworks.cc/documentation/)：官方文档；
- [oF Forum](http://forum.openframeworks.cc/)：官方论坛；
- [ofxaddons.com](http://ofxaddons.com/)：这里搜集了非常丰富的 openFrameworks 插件；
- [Programming Interactivity](http://www.amazon.com/Programming-Interactivity-Designers-Processing-Openframeworks/dp/0596154143)：一本介绍 openFrameworks、Processing 以及 Arduino 的好书；









