# Psychtoolbox 3 精确时间控制（1） - fighting！！！ - CSDN博客
2018年05月12日 19:28:52[dujiahei](https://me.csdn.net/dujiahei)阅读数：227

转自：http://blog.rainy.im/2015/05/01/psychtoolbox-3-accurate-timing-1/
https://www.jianshu.com/p/4ad939063f9f
Psychtoolbox 3 中一些基本概念与基本原理的介绍，重点总结精确到帧的呈现时间的控制与精确的键盘（鼠标）输入时间的控制。
### 1. Textures, Windows, Screens 等基本概念
PTB 中 screen 指代显示器，Screen 0 代表默认显示器，Screen 1 代表双屏系统中第二个显示器，以此类推。`Screen('Screens')`返回可用显示器的列表。
PTB 中 (onscreen) window 是用来展示实验刺激的，一个显示器（screen）最多只能有一个全屏显示的 window。所有的绘制操作都在 window 中执行。window 都是[双缓冲](http://en.wikipedia.org/wiki/Multiple_buffering#Double_buffering_in_computer_graphics)的，也就是包含了后缓冲区（backbuffer）和前缓冲区（frontbuffer）两部分。如果把显示器看作是一个舞台，后缓冲区相当于舞台的后台，将要绘制的图形在这里完成准备工作；而前缓冲区相当于看得见的舞台部分，也就是显示器呈现刺激的部分，如一般实验室采用的 CRT 显示器，在前缓冲区中阴极射线管通过扫描完成一次成像。当我们通过一些绘制指令（如`Screen('DrawTexture',...)`）完成绘制的准备工作后，需要通过调用`Screen('Flip',...)`指令，完成前后缓冲区的切换，同时保证这一切换过程（一般1ms之内即可完成）发生在两次（帧）扫描的间隔（也就是电子枪回溯到屏幕左上角的时间段），避免造成成像断裂等现象。`Flip` 指令最重要的功能是将图像绘制过程与实际呈现过程分离。
(offscreen) window 就是之前版本中的 Texture，唯一的不同是指令名称和执行效率。细微的差别就不再赘述。
### 2. Flip 指令
Screen('Flip?')% 可以通过 Screen('Func?')进行查询该指令的说明文档Usage:[VBLTimestampStimulusOnsetTimeFlipTimestampMissedBeampos]=Screen('Flip',windowPtr[,when][,dontclear][,dontsync][,multiflip]);
Flip 指令返回的第一个值 `VBLTimestamp` 是指上一帧扫描结束的时间点，接下来阴极射线管的电子枪需要回到屏幕左上角进行下一帧的扫描，这一间隔称为垂直空白间隙（vertical blank interval, VBL）。同时前后缓冲区的切换也从这一时间点开始。此时 PTB 会进入休眠状态，一直等到切换过程结束才会重新请求 CPU 资源，在这期间 CPU 可以保证系统其它部分的正常运行。等到切换完成之后，PTB 立即执行下面两件事：
- 查询电子枪（beam）当前的位置，并返回给变量 `Beampos`；
- 通过`GetSecs()`查询当前精确的时间戳。
一般的多任务操作系统（OS X & Windows），在具体事件的时间点上都可能存在非常细微的随机误差，例如 VBL 开始的时间（`VBLTimestamp`）、PTB 重新占用 CPU 的时间，一般这一时间差不会超过 1ms，但偶尔也有可能出现几毫秒的误差。为了补偿这一误差，PTB 利用电子枪回溯的位置作为另外一个高分辨率的时钟进行精确时间的控制，其计算的原理如下图所示：
![flip](http://7xiijd.com1.z0.glb.clouddn.com/flip.png)
Flip 返回的第二个值 `StimulusOnsetTime` 代表第二帧开始进行扫描的时间，即：`StimulusOnsetTime = VBLTimestamp + VBL`。
Beamposition 的查询必须依赖于显卡硬件的支持，当无法通过硬件查询准确的 Beamposition 时，也就无法获取 VBL 时间。这时 PTB 一般会报错：
----- ! PTB-ERROR:SYNCHRONIZATIONFAILURE ! ----Oneormoreinternalchecks(seeWarningsabove)indicatethatsynchronizationofPsychtoolboxtotheverticalretrace(VBL)isnotworkingonyoursetup.
如果对时间精确度的要求不是那么严格，或者程序还在调试阶段，可以通过`Screen('Preference', 'SkipSyncTests', 1)`跳过这一错误。此时，Flip 指令返回 `StimulusOnsetTime = VBLTimestamp; beampos = -1;`。
### 3. Accurate Timing Demo
了解了以上原理，再来看精确时间控制的[Demo](http://peterscarfe.com/accurateTimingDemo.html)：
首先是最简单随意的呈现方式，这样无法保证精确控制时间，只是为了与后面的方法进行比较：
forframe=:numFrames% Color the screen redScreen('FillRect',window,[0.50.50.5]);% Flip to the screenScreen('Flip',window);end
上一例子中没有明确给出 Flip 执行的时间，这导致我们无法精确控制刺激呈现的时间。下面的例子中，首先在后缓冲区准备绘制之前读取前一帧结束的时间`vbl`，然后在连续呈现`numFrames`帧刺激的过程中，明确指定每一次 Flip 的时间为上一帧扫描结束后的`(waitframes - 0.5)*ifi`。当`waitframes = 1;`时，前后两次 Flip 之间的间隔一定为IFI，也就是连续呈现两帧，从而确保刺激呈现的时间为两帧。
waitframes=;vbl=Screen('Flip',window);forframe=:numFrames% Color the screen redScreen('FillRect',window,[0.5]);% Flip to the screenvbl=Screen('Flip',window,vbl+(waitframes-0.5)*ifi);end
上一例子中我们希望 PTB 可以在连续两帧中呈现刺激，以保证刺激呈现时间。但现实并不一定如此，因为计算机操作系统需要同时维持所有软硬件组成的正常运转，如键盘、鼠标、网络连接等等，这就可能导致 CPU 无法及时响应 PTB 的请求。为了让 CPU 优先处理 PTB 的请求，我们需要提高 PTB 的优先等级；除此之外，还可以在绘制指令与 Flip 之间插入`Screen('DrawingFinished', window);`来阻止其它绘制，保证更好的时间精确度：
topPriorityLevel=MaxPriority(window);Priority(topPriorityLevel);vbl=Screen('Flip',window);forframe=:numFrames% Color the screen redScreen('FillRect',window,[0.50.5]);% Tell PTB no more drawing commands will be issued until the next flipScreen('DrawingFinished',window);% Flip to the screenvbl=Screen('Flip',window,vbl+(waitframes-0.5)*ifi);endPriority();
### 4. 其它精确时间控制的设定及方法
对于现代计算机硬件来说，基本上可以通过代码的控制做到精确的时间控制。如果实验仪器是一些相对老旧的硬件设备（例如……），可能需要一些额外的系统配置来辅助精确时间的控制。
**4.1 系统设置：**
- 退出系统中无用的应用程序、后台服务等，不要占用过多 CPU；
- 禁用杀毒软件；
- 确保有足够的内存，必要时利用虚拟内存；
- 确保节能选项不会减慢 CPU；
- 关闭蓝牙、WIFI、网络连接以及 CD/DVD 等外接设备；
- 不要盲目采用过高分辨率，这样可能加重显卡负担，增加处理时间误差。
**4.2 Matlab设置：**
- `matlab -nojvm` 模式运行（如果没有必要用到 Java 辅助工具，如 `GetChar`等）；
- 尽量减少 Matlab 图形界面窗口；
- 阅读 Matlab 文档，优化代码，充分利用内存加快执行速度；
- `help mlint` 查看执行效率的瓶颈及代码可能存在的问题。
**4.3 程序编写中需要注意的问题：**
- PTB 输出的 log 信息：
通过 `Screen('Preference', 'Verbosity', level);` 控制 PTB 执行过程中的输出信息，在程序调试时可以选择较高的 `level` 便于找到问题；正式实验可以采用 `Screen('Preference', 'Verbosity', 0);` 禁用所有输出信息，节省执行时间和资源。
- 函数的预加载：
Matlab 中的 `.M & .MEX` 源文件的加载需要一定的时间，如文件的定位、加载、执行等，这一初始化时间可能需要几百毫秒，所以在实验试次开始之前应该先执行一次需要的函数。
- 同一源文件内的`function`比通过文件加载的`function`快：
Matlab 中的`function`可以直接在当前脚本中定义，也可以通过单独的脚本文件定义（函数名与文件名一致），调试过程中为方便可以将不同的`function`切分成独立的脚本文件，但在正式实验时应该将必要的`function`整合到同一个执行脚本。
- 优化代码结构：
实验程序的主体，trial loop 内的代码应该以如下顺序执行：
% 1. 绘制命令Screen('DrawDots',...), Screen('DrawText',...),Screen('DrawTexture',...)...% 2. 阻止多余绘制Screen('DrawingFinished',win);% 3. 其它与显卡操作无关的代码，如声音、键盘、鼠标以及其它Matlab逻辑等KbWait;% 4. Flip 指令让预备绘制刺激登台vbl=Screen('Flip',win,vbl+0.5*ifi);
### 5. 总结
上述内容主要针对 CRT 显示器，CRT 与 液晶平板（LCD）的原理及优劣 Elze （2010）的文章中有详细介绍，文章还总结了2003-2009三本心理学杂志中75篇实验材料呈现时间低于50ms的研究，从各个研究中作者给出或推断的实验方法来看，许多研究者对显示器上刺激呈现时间的基本概念与操作还存在一些误解，导致实验实际呈现的时间并非研究者所认为的时间。一直以来都有争议而且最近比较热闹的心理学研究重复验证，其实很大程度上与实验条件控制有关，比如有些研究者文章中声称研究中刺激呈现时间为50ms，但是我们并不知道是如何做到的，实验程序中是否存在不合适的操作等。所以说应该建议研究者投稿录用后把实验程序开源，放到 Github 上，供所有人查看，也大大节约了重复实验的时间成本。哈哈，但是又有几个人愿意这样做呢~
到此为止基本介绍了 PTB 3 中对实验刺激呈现时间进行精确控制的方法以及相关注意事项，在实验过程中除了准确地呈现实验刺激之外，还需要准确地记录被试做出反应的时间，这主要涉及到键盘、鼠标或者特定的反应输入设备的精确控制。PTB 3 中如何准确地处理响应输入呢，下一节再说吧。
参考：
- [FAQ: Textures, Windows, Screens](https://github.com/Psychtoolbox-3/Psychtoolbox-3/wiki/FAQ%3A-Textures%2C-Windows%2C-Screens)
- [FAQ: Performance Tuning](https://github.com/Psychtoolbox-3/Psychtoolbox-3/wiki/FAQ%3A-Performance-Tuning)
- [Accurate Timing Demo](http://peterscarfe.com/accurateTimingDemo.html)
- Elze, T. (2010). Misspecifications of stimulus presentation durations in experimental psychology: A systematic review of the psychophysics literature. PloS one, 5(9), e12792.
