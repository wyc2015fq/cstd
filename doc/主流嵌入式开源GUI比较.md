# 主流嵌入式开源GUI比较

  

# 前言

　　本文主要说明了MicroWindows、MiniGui、QT、GTK+、OpenGUI和FLTK，第七小节是对比的详细表格，内容是根据网上的一些资料还有各大开源GUI的官网的资料整理而来，目的是为自己以后学习把握方向。

# 1  MicroWindows

　　首先看一下效果图片吧，都在[MicroWindows](http://www.microwindows.org/)官网 
 ![这里写图片描述](http://www.microwindows.org/images/pixiloe_loadmon.gif) ![这里写图片描述](http://www.microwindows.org/images/pixiloe_calc.gif) 
 ![这里写图片描述](http://www.microwindows.org/images/pixil_scrtop.gif) ![这里写图片描述](http://www.microwindows.org/images/pixil_sysconf.gif) 
 　　Microwindows 是一个著名的开放式源码嵌入式GUI 软件，遵循MPL协议，目的是把图形视窗环境引入到运行Linux 的小型设备和平台上。作为X Window 的替代品，Microwindows可以使用更少的RAM 和文件存储空间（100K-600K）提供与X Window 相似的功能。Microwindows 允许设计者轻松加入各种显示设备、鼠标、触摸屏和键盘等。Microwindows 的可移植性非常好，基本上用 C 语言实现，只有某些关键代码使用了汇编以提高速度。Microwindows 支持ARM 芯片。尽管Microwindows 完全支持linux，但是它内部的可移植结构是基于一个相对简单的屏幕设备接口，可在许多不同的RTOS和裸机上运行。 
 　　Microwindows 的图形引擎能够运行在任何支持readpixel， writepixel，drawhorzline,  drawvertline 和setpalette  的系统之上。在底层函数的支持之下，上层实现了位图，字体，光标以及颜色的支持。系统使用了优化的绘制函数，这样当用户在移动窗口时可以提供更好的响应。内存图形绘制和移动的实现使得屏幕画图显得很平滑，这点特别在显示动画、多边形绘制、任意区域填充、剪切时有用。Microwindows  支持新的Linux内核帧缓存(FrameBuffer)结构，目前提供每像素1、2、4、8、16、24  和32 位的支持，另外还支持彩色显示和灰度显示，其中彩色显示包括真彩色(每像素15、16 和32 位)和调色板(每像素1， 2， 4 和 8  位)两种模式。在彩色显示模式下，所有的颜色用RGB  格式给出，系统再将它转换成与之最相似的可显示颜色，而在单色模式下中则是转换成不同的灰度级。Microwindows支持窗口覆盖和子窗口概念、完全的窗口和客户区剪切、比例和固定字体，还提供了字体和位图文件处理工具。Microwindows支持的中文字符集是GB2312，并不支持Unicode。 
 　　Microwindows 采用分层设计方法。在最底层，屏幕，鼠标/触摸屏以及键盘驱动程序提供了对物理设备访问的能力。在中间层，实现了一个可移植的图形引擎，支持行绘制，区域填充，剪切以及颜色模型等。在上层，实现多种API 以适应不同的应用环境。 
 　　两种API：Win32和Nano-X  
 　　Microwindows中使用两种流行的图形编程接口：Microsoft Windows  Win32/WinCE图形显示接口（GDI）和Xlib-like接口。前者应用于所有的Windows  CE和Win32应用程序，后者就象Nano-X,应用于所有Linux  X插件集的最底层。这样做可以让大量的Windows程序员开发图形应用程序，类似地也可以让Linux图形程序员用X接口开发图形应用程序。  
 　　Nano-X服务器占用的资源大约在100K到150K。原始Nano-X应用程序的平均大小在30K到60K。由于Nano-X是为有内存限制的低端设备设计的，所以它不象X那样支持很多函数，因此它实际上不能作为微型X（Xfree86）的替代品。 
 　　Microwindows系统以图形方式支持目标平台的主机平台仿真。也就是说，用于Linux的Microwindows应用程序可以在桌面上进行开发和原型设计，运行和测试，而无需在目标平台上进行交叉编译和运行。这是使用Microwindows’  X  screen驱动程序完成的，而不是framebuffer驱动程序，其中目标应用程序在桌面主机上运行并显示在X窗口中。可以告诉驱动程序以每像素位数和颜色深度精确地模拟目标平台的显示。因此，即使桌面系统是24位颜色，它可以显示一个2bpp灰度级来预览目标应用程序。由于主机和目标机都运行Linux，因此大多数所有操作系统服务都可在桌面主机上使用。

# 2  MiniGui

　　[MiniGui](http://www.minigui.org/en/)官网，更多效果图参见我前面写的文章[《MiniGui Samples运行效果》](http://blog.csdn.net/anyuliuxing/article/details/77936639) 
 ![这里写图片描述](https://img-blog.csdn.net/20170912090141749?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYW55dWxpdXhpbmc=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast) ![这里写图片描述](https://img-blog.csdn.net/20170912091258294?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYW55dWxpdXhpbmc=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

　　MiniGUI 是针对嵌入式系统设计和优化的完整图形支持系统，遵循GPL协议。作为操作系统和应用程序之间的一个中间件，MiniGUI 将底层操作系统和硬件平台的细节隐藏起来，并为上层的应用程序提供了一致接口。MiniGUI 的主要技术特性描述如下： 
 　　跨操作系统支持，具体包括普通嵌入式 Linux/uClinux、VxWorks、eCos、uC/OS-II、pSOS、ThreadX、Nucleus、OSE 等，同时还提供 Win32 平台上的 SDK 开发包，方便嵌入式应用程序的开发和调试。 
 　　多运行模式支持，为了适应不同的操作系统运行环境，MiniGUI可配置成三种运行模式：MiniGUI-Threads、MiniGUI-Processes及 MiniGUI-Standalone。 
 　　内建资源支持。我们可以将 MiniGUI 所使用的资源，诸如位图、图标和字体等编译到函数库中，该特性可提高 MiniGUI 的初始化速度，并且非常适合 eCos/uCOS-II/ThreadX 等无文件系统支持的实时嵌入式操作系统。 
 　　完备的多窗口机制和消息传递机制。 
 　　提供常用的控件类，包括静态文本框、按钮、单行和多行编辑框、列表框、组合框、菜单按钮、进度条、滑块、属性页、工具栏、树型控件、月历控件、旋钮控件、酷工具栏、网格控件、动画控件等。 
 对话框和消息框支持。 
 　　其它 GUI 元素，包括菜单、加速键、插入符、定时器等。 
 　　界面皮肤支持，用户可通过皮肤支持获得华丽的图形界面。 
 　　支持低端显示设备（比如单色 LCD）和高端显示设备（8 位色及以上显示设备）。通过 MiniGUI 的图形抽象层及图形引擎技术，还可以支持特殊的显示设备，比如 YUV 显示设备。 
 　　提供有增强 GDI 函数，包括光栅操作、复杂区域处理、椭圆、圆弧、多边形以及区域填充等函数。在提供有兼容于 C99  规范的数学库平台上，还提供有高级二维绘图函数，可设置线宽、线型以及填充模式等。通过 MiniGUI  的图形抽象层及图形引擎技术，我们也可以让上述高级 GDI 接口在低端显示屏上实现。 
 　　Windows 的资源文件支持，如位图、图标、光标等。 
 各种流行图像文件的支持，包括 Windows BMP、GIF、JPEG、PNG等（JPEG 及 PNG的支持通过 libjpeg 及 libpng 函数库提供）。 
 　　多字符集和多字体支持，目前支持  ISO8859-1～ISO8859-15、GB2312、GBK、GB18030、BIG5、EUC-JP、Shift-JIS、EUC-KR、UNICODE（UTF-8、UTF-16  编码）等字符集，支持等宽点阵字体、变宽点阵字体、Qt/Embedded 使用的嵌入式字体 QPF、TrueType 以及 Adobe  Type1 等矢量字体（TrueType 及 Type1 的支持分别通过 freetype 1.3 及 libt1  函数库提供）。同时，对于点阵字体 MiniGUI 也可以进行自动放大处理，并可针对电视等特殊显示设备实现防锯齿功能。 
 多种键盘布局的支持。MiniGUI 除支持常见的美式 PC 键盘布局之外，还支持法语、德语等西欧语种的键盘布局。 
 　　输入法支持，用于提供各种可能的输入形式；内建有适合 PC 平台的汉字（GB2312）输入法支持，包括内码、全拼、智能拼音、五笔及自然码等。 
 针对嵌入式系统的特殊支持，包括一般性的 I/O 流操作，字节序相关函数等。 
 　　副屏支持，当系统中有多个视频设备时，可将一个作为MiniGUI的主屏，实现完整的多窗口系统；而其它设备作为副屏，在其上通过MiniGUI 的图形接口来实现文字渲染、图形显示等功能。 
 和其它针对嵌入式产品的图形系统相比，MiniGUI 在对系统的需求上具有如下几大优势： 
 　　可伸缩性强 
 　　MiniGUI 丰富的功能和可配置特性，使得它既可运行于 CPU 主频只有 60MHz 的低端产品中，亦可运行于高端嵌入式设备中，并使用 MiniGUI 的高级控件风格及皮肤界面等技术，创建华丽的用户界面。 
 MiniGUI 的跨操作系统特性，使得 MiniGUI 可运行在最简单的嵌入式操作系统之上，如  uC/OS-II，也可以运行在具有现代操作系统特性的嵌入式操作系统之上，如 Linux，而且 MiniGUI 为嵌入式 Linux  系统提供了完整的多窗口图形环境。这些特性，使得MiniGUI 具有非常强的可伸缩性。可伸缩性是 MiniGUI  从设计之初就考虑且不断完善而来的。这个特性使得 MiniGUI 可应用于简单的行业终端，也可应用于复杂的消费类电子产品。 
 　　轻型、占用资源少 
 　　MiniGUI 是一个定位于轻量级的嵌入式图形库，对系统资源的需求完全考虑到了嵌入式设备的硬件情况，如 MiniGUI 库所占的空间最小可以裁剪到500K  左右，对目前的嵌入式设备来说，满足这一条件是绰绰有余的。此外，测试结果表明，MiniGUI 能够在 CPU 主频为30 MHz，仅有4M RAM的系统上正常运行（使用 uClinux 操作系统），这是其它针对嵌入式产品的图形系统所无法达到的。 
 　　高性能、高可靠性 
 　　MiniGUI  良好的体系结构及优化的图形接口，可确保最快的图形绘制速度。在设计之初，我们就充分考虑到了实时嵌入式系统的特点，针对多窗口环境下的图形绘制开展了大量的研究及开发，优化了  MiniGUI 的图形绘制性能及资源占用。MiniGUI 在大量实际系统中的应用，尤其在工业控制系统的应用，证明 MiniGUI  具有非常好的性能。 
 从 1999 年 MiniGUI 的第一个版本发布以来，就有许多产品和项目使用 MiniGUI，MiniGUI  也不断从这些产品或者项目当中获得发展动力和新的技术需求，逐渐提高了自身的可靠性和健壮性。有关 MiniGUI  的最新成功案例，您可以访问飞漫公司网站的典型案例部分： <http://www.minigui.org/> 
 　　可配置性 
 　　为满足嵌入式系统各种各样的需求，必须要求 GUI 系统是可配置的。和 Linux 内核类似，MiniGUI  也实现了大量的编译配置选项，通过这些选项可定 MiniGUI 库中包括哪些功能而同时不包括哪些功能。大体说来，我们可以在如下几个方面对  MiniGUI 进行定制配置： 
 　　指定 MiniGUI 要运行的硬件平台。 指定 MiniGUI 要运行的操作系统。 指定生成基于线程的 MiniGUI-Threads  运行模式还是基于进程的 MiniGUI-Processes 运行模式，或者只是最简单的 MiniGUI-Standalone 运行模式。  指定需要支持的 GAL 引擎和 IAL 引擎，以及引擎相关选项。 指定需要支持的字体类型。 指定需要支持的字符集。 指定需要支持的图像文件格式。  指定需要支持的控件类。 指定控件和窗口的整体风格，可以通过指定不同的渲染器完成。 这些配置选项大大增强了 MiniGUI  的灵活性，对用户来讲，可针对具体的应用需求量体裁衣，开发最适合产品需求的应用软件。

# 3  QT

　　[QT](https://www.qt.io/) 官网，效果图如下 
 ![这里写图片描述](http://doc.qt.io/qt-5/images/piechart_customization.png) 
 ![这里写图片描述](http://doc.qt.io/qt-5/images/volumetric-example.png) 
 　　Qt 是一个跨平台的C++应用程序框架，支持Windows、Linux、Mac OS  X、Android、iOS、Windows Phone、嵌入式系统等。也就是说，Qt  可以同时支持桌面应用程序开发、嵌入式开发和移动开发，覆盖了现有的所有主流平台。你只需要编写一次代码，发布到不同平台前重新编译即可。 
 　　Qt 不仅仅是一个GUI库，它除了可以创建漂亮的界面，还有很多其他组件，例如，你不再需要研究STL，不再需要C++的头文件，不再需要去找解析XML、连接数据库、Socket 的各种第三方库，这些 Qt 都已经内置了。Qt 是应用程序开发的一站式解决方案！Qt 虽然庞大，封装较深，但它的速度并不慢，虽不及MFC，但比Java、C#要快。Qt 程序最终会编译成本地代码，而不是依托虚拟机。 
 　　使用Qt开发的软件，相同的代码可以在任何支持的平台上编译与运行，而不需要修改源代码。会自动依平台的不同，表现平台特有的图形界面风格。 
 　　Linux/X11：用于X Window System（如Solaris、AIX、HP-UX、Linux、BSD）。支持KDevelop和Eclipse IDE集成 
 　　Mac：用于Apple Mac OS X。基于Cocoa框架。支持Universal Binary。支持以Xcode编辑、编译和测试。 
 　　Windows：用于Microsoft Windows。支持Visual Studio集成，也可以使用MinGW编译 
 　　Embedded Linux：用于嵌入式Linux。可以通过编译移除不常使用的组件与功能。通过自己的视窗系统QWS，不需依赖X Window System，直接写入Linux帧缓冲。可以减少内存消耗。并提供虚拟帧缓冲QVFb，方便在桌面系统上进行嵌入式测试。 
 　　Windows CE / Mobile：用于Windows CE 
 　　Symbian：用于Symbian platform 
 　　Maemo/MeeGo：用于Maemo 
 　　Wayland：用于Wayland显示服务器，Qt应用程序可以在运行时切换图形后端，如X与Wayland。

# 4  Gtk+

　　[Gtk+](https://www.gtk.org/) 官网，效果图如下 
 ![这里写图片描述](https://www.gtk.org/images/screenshots/screenshot-maemo.png) 
 ![这里写图片描述](https://www.gtk.org/images/screenshots/screenshot-gnome-desktop.png) 
 　　Gtk+(GIMP Tool Kit，GIMP工具包)是一个用于创造图形用户接口的图形库。Gtk+是基于LGPL授权的，因此可以用Gtk+开发开放源码软件、自由软件，甚至商业的、非自由的软件，并且不需要为授权费或版权费花费一分钱。Gtk已在大量软件项目，包括Gnome中得到了广泛应用。  
 　　Gtk+图形库使用一系列称为“构件”的对象来创建应用程序的图形用户接口。它提供了窗口、标签、命令按钮、开关按钮、检查按钮、无线按钮、框架、列表框、组合框、树、列表视图、笔记本、状态条等构件。可以用它们来构造非常丰富的用户界面。  
 　　GTK本质上是面向对象的应用程序编程接口(API)。GObject 也就是这种面向对象的机制可以为 GTK+绑定很多种开发语言。虽然完全是用C写成的，但它仍然是用类和回调函数(指向函数的指针)的方法实现的。因为Gtk+是用C语言编写的，所以在开发Linux下的GUI程序时使用C语言是非常方便的。 在GTK下还可以使用图形化编程工具Glade，一种GUI生成器，可以快速生成创建界面的C源程序。  
 　　自从Qt推出了嵌入式版本之后，虽然GTK＋并非商业公司所发展，但也加紧脚步推出了GtkFB方案，其宗旨就是要为嵌入式系统推出一套基于GTK＋的GUI解决方案。与Qt/Embedded类似，GtkFB也跳过X层直接与FrameBuffer沟通，因此也具有Qt/Embedded的几项优点，不过由于不是商业软件，在发展的速度上较为缓慢。

# 5  OpenGUI

　　[OpenGUI](http://www.tutok.sk/fastgl/)官网，效果图如下 
 ![这里写图片描述](http://www.tutok.sk/fastgl/img/3ab1458e.png) 
 ![这里写图片描述](http://www.tutok.sk/fastgl/img/dmr.png) 
 　　OpenGUI在Linux系统上存在已经很长时间了。这个库是用C++编写的，只提供C++接口。OpenGUI基于一个用汇编实现的x86图形内核，提供了一个高层的C/C++图形/窗口接口。OpenGUI提供了二维绘图原语、消息驱动的API及BMP文件格式支持。OpenGUI功能强大，使用方便。OpenGUI  支持鼠标和键盘的事件，在Linux上基于FrameBuffer或者SVGALib实现绘图。由于其基于汇编实现的内核并利用MMX指令进行了优化，OpenGUI运行速度非常快。正由于其内核用汇编实现，可移植性受到了影响。通常在驱动程序一级，性能和可移植性是矛盾的，必须找到一个折衷。OpenGUI采用LGPL条款发布。OpenGUI比较适合于基于x86平台的实时系统，跨平台的可移植性较差，目前发展较慢。

# 6  FLTK

　　[FLTK](http://www.fltk.org/index.php)官网，效果图如下 
 ![这里写图片描述](http://www.fltk.org/images/fltk-widgets-std.png)  
 ![这里写图片描述](http://www.fltk.org/images/fluid-flphoto.png) 
 　　FLTK，如同其名字所表达的：The Fast Light Toolkit，一个轻量级的GUI开发库。但这轻量级并不代表功能的羸弱，相反，FLTK在具有基本的GUI功能之外，还拥有一些特殊的功能，比如跨平台、内置 OpenGL功能、速度更快、尺寸更小、协议（LGPL）宽松等。当然，缺点也是有的，比如对于复杂的界面构件支持不够，资源支持的不足等。 
 　　FLTK的底层只提供一套完整的画点、画线功能，另外附带了字体的显示功能，但FLTK对字体的支持还很粗糙。 
 　　最大的缺点是：就是对非英文字体特别是中文的支持比较差，甚至是非常差，它不支持unicode编码。需要到2.0版本才可能支持，还在开发当中。

# 7  比较

## 7.1 MiniGUI和MicroWindows

　　MiniGUI和MicroWindows均为自由软件，但这两个系统的技术路线却有所不同。MiniGUI的策略是建立在比较成熟的图形引擎之上，比如Svgalib和LibGGI，开发的重点在于窗口系统、图形接口之上。而MicroWindows的开发重点则在底层的图形引擎之上，所以可以对裸显示器直接操作，而窗口系统和图形接口方面的功能还稍有欠缺。比如说，MiniGUI有一套用来支持多字符集和多编码的函数接口，可以支持各种的字符集，包括GB、BIG5、UNI-CODE等。

　　Nano-X 的优点 
 　　与Xlib实现不同，Nano-X仍在每个客户机上同步运行，这意味着一旦发送了客户机请求包，服务器在为另一个客户机提供服务之前一直等待，直到整个包都到达为止。这使服务器代码非常简单，而运行的速度仍非常快；占用很小的资源。

　　Nano-X 的缺点 
 　　1、联网功能部件至今没有经过适当地调整（特别是网络透明性）； 
 　　2、还没有太多现成的应用程序可用； 
 　　3、没有很多文档说明而且没有很好的支持 
 　　4、图形引擎中存在许多低效算法，值得一提的是，该项目的许多控件是从 MiniGUI 中移植过去的，扫雷游戏也是从 MiniGUI  中移植过去的。由于该项目缺乏一个强有力的核心代码维护人员，2010年Microwindows推出版本0.92后，该项目的发展开始陷于停滞状态。

　　MiniGui优点 
 　　将现代窗口和图形技术带入到嵌入式设备的MiniGUI，是一个非常适合于实时嵌入式设备的高效、可靠、可定制、小巧灵活的图形用户界面支持系统，而且还有miniStudio集成开发环境 
 　　1、支持多种嵌入式操作系统，具备优秀的可移植性； 
 　　2、可伸缩的系统架构，易于扩展； 
 　　3、功能丰富，可灵活剪裁； 
 　　4、小体积高性能间的最佳平衡； 
 　　5、广泛的应用领域。

　　MiniGui缺点 
 　　1、对图形设备的抽象层次太高。

| 名称     | MiniGui           | MicroWindows     | 备注             |
| -------- | ----------------- | ---------------- | ---------------- |
| 厂商     | 飞漫（中国）      | 世纪软件（美国） |                  |
| 开发语言 | C                 | C                |                  |
| 开源协议 | GPL               | MPL              |                  |
| 体系结构 | 类WIN32           | WIN32/Nano-X     |                  |
| 最新版本 | 3.0.12（2012.01） | 0.92（2010.12）  | 都很久没有更新   |
| 资源占用 | 最小500K          | 100K~600K        |                  |
| 网上资源 | 一般              | 少               |                  |
| 技术资料 | 一般              | 较少，官网有     |                  |
| 开发环境 | vim、eclipse      | vim、eclipse     |                  |
| 移植难度 | 相同              | 相同             | 移植难度都不太大 |
| 复杂度   | 中                | 中               |                  |
| 学习难度 | 中                | 中               | 应用程序实现类似 |
| 应用范围 | 中                | 中               | 根据网上资料     |

　　MiniGui支持系统：Linux/uClinux、VxWorks、eCos、uC/OS-II、pSOS、ThreadX、Nucleus、OSE 等，同时还提供 Win32 平台上的 SDK 开发包 
 　　MicroWindows支持系统：Linux、RTOS、裸机、X Window、MSDOS、能够在支持readpixel，writepixel，drawhorzline和drawvertline和setpalette的任何系统上运行

## 7.2 QT和GTK+

　　QT优点 
 　　相对来说易学：Qt封装的很好，几行代码就可以开发出一个简单的客户端，Qt是面向对象的，Qt对于用户开发而言是十分方便的，因为Qt良好的封装机制令Qt的模块化程度非常高并且可重用性好。 
 　　资料丰富：资料丰富能够成倍降低学习成本，否则只能去看源码， 
 　　漂亮的界面：Qt很容易做出漂亮的界面和炫酷的动画 
 　　独立安装：Qt程序最终会编译为本地代码，不需要其他库的支撑， 
 　　跨平台：如果程序需要运行在多个平台下，同时又希望降低开发成本，Qt几乎是必备的。 
 　　集成环境：Qt Creator为广大开发者提供了免费而又友好的集成开发环境。

　　QT缺点 
 　　库很大，Qt库如果不进行裁剪的话，实在是太大了，Qt5中的icudt52.dll就有20多M 
 　　qt Embedded memeory requirememnts:

| Architecture            | 编译器    | QtCore | QtGui | QtNetwork | QtWebKit |      |       |      |      |
| ----------------------- | --------- | ------ | ----- | --------- | -------- | ---- | ----- | ---- | ---- |
| 最小                    | 正常      | 最小   | 正常  | 最小      | 正常     | 最小 | 正常  |      |      |
| linux x86 g++           | GCC 4.2.4 | 1.7M   | 2.7M  | 3.3M      | 9.9M     | 653K | 1.1M  | N/A  | 17M  |
| linux arm g++           | GCC 4.1.1 | 1.9M   | 3.2M  | 4.1M      | 11M      | 507K | 1.0M  | N/A  | 17M  |
| linux mips g++ (MIPS32) | GCC 4.2.4 | 2.0M   | 3.2M  | 4.5M      | 12M      | 505K | 1003K | N/A  |      |


 
 　　GTK+优点 
 　　完全Cairo绘制：Cairo是什么？它是一个矢量的绘图库，GTK+2.8，Cairo才开始作为GTK的重要绘图引擎，从来带来矢量、抗锯齿等特性 
 　　支持CSS的主题API，还支持简单的动画属性 
 　　多点设备的支持，设计灵活、可扩展 
 　　GObject Introspection：它让GTK+的语言扩展能力尽乎无限。以前，除了GTK+、GTKMM（C++）以外，还有一些动态语言的绑定，如Pygtk、perl-gtk。但这些实现是各种独立的，维护成本非常高。但是GObject  Introspection实现了GObject对象的统一调用，这意味着各种语言只要实现一个GObject  Introspection的包装，什么GTK+的API，都可以通过它来调用了。就像在Python里用ctype一样，但是通过GI技术，能像使用原生API一样来使用这些C库。目前为止，对GI支持比较好的已经有Python、JavaScript了。GNOME  3.0的重要组件——GNOME Shell即是使用了JavaScript来实现其UI逻辑部分。

| 名称     | QT                                  | GTK+                  | 备注         |
| -------- | ----------------------------------- | --------------------- | ------------ |
| 厂商     | Digia（芬兰）                       | GTK+ Team             |              |
| 开发语言 | C++、QML                            | C、绑定其他语言       |              |
| 开源协议 | GPL、LGPL                           | LGPL                  |              |
| 体系结构 | X Window、Framebuffer               | X Window、Framebuffer |              |
| 最新版本 | 5.9.1（2017.6.30）                  | 3.22.20（2017.9.4）   | 都一直在更新 |
| 资源占用 | qt4.8资源占用见上表                 | 未知                  |              |
| 网上资源 | 很多，视频教程也有                  | 中                    |              |
| 技术资料 | 很多                                | 中                    |              |
| 开发环境 | Fake Vim、Qt Creator、Visual Studio | vim、eclipse          |              |
| 移植难度 | 比MicroWindows大                    | 中                    |              |
| 复杂度   | 复杂                                | 中                    |              |
| 学习难度 | 中                                  | 中                    |              |
| 应用范围 | 广                                  | 中                    |              |

　　QT支持系统：Linux/X11、Mac、Windows、Android、IOS、Embedded Linux、Windows  CE/Mobile、Maemo/MeeGo、Wayland，Qt 可以同时支持桌面应用程序开发、嵌入式开发和移动开发，覆盖了现有的所有主流平台 
 　　GTK+支持系统：GNU/Linux and Unix、Windows、Mac OS X、X11、Win32、HTML 5、Wayland