# 用 cairo 实现跨平台图形 - Koma Hub - CSDN博客
2019年03月02日 17:58:22[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：147
**Table of Contents**
[cairo 的优点和用途](#N1004F)
[为什么要学习一种新的绘图模型？](#N1008B)
[cairo 呈现目标](#N10094)
[cairo 的实际应用情况](#N100AC)
[用 cairo 进行概念性绘图](#N100BE)
[正确的 cairo 术语](#N100DC)
[cairo 示例应用程序](#N10107)
[cairo 的发展过程](#N10121)
[下载资源](#%E4%B8%8B%E8%BD%BD%E8%B5%84%E6%BA%90)
[相关主题](#%E7%9B%B8%E5%85%B3%E4%B8%BB%E9%A2%98)
[https://www.ibm.com/developerworks/cn/linux/l-cairo/index.html](https://www.ibm.com/developerworks/cn/linux/l-cairo/index.html)
用于产生一致输出的矢量绘图库
## cairo 的优点和用途
cairo 是一个免费的矢量绘图软件库，它可以绘制多种输出格式。cairo 支持许多平台，包括 Linux、BSD、Microsoft® Windows® 和 OSX（BeOS 和 OS2 后端也正在开发）。Linux 绘图可以通过 X Window 系统、Quartz、图像缓冲格式或 OpenGL 上下文来实现。另外，cairo 还支持生成 PostScript 或 PDF 输出，从而产生高质量的打印结果。在理想情况下，cairo 的用户可以在打印机和屏幕上获得非常接近的输出效果。
本文将向您介绍 cairo，以及它对应用程序的意义。本文的示例将产生一个 pdf、ps、png、svg 和 gtk 窗口，显示的图形是 IBM 徽标。
cairo 的一项主要设计目标是提供尽可能接近的输出。这种一致的输出使 cairo 非常适合 GUI 工具集编程和跨平台应用程序开发。使用同一个绘图库打印高分辨率的屏幕和绘制屏幕内容，这种功能具有显著的优点。
另外，在支持的每种目标平台上，cairo 尝试智能化地使用底层硬件和软件支持。高质量矢量图形和高性能的结合使 cairo 成为优秀的 UNIX® 绘图系统。
cairo 是用 C 编写的，但是为大多数常用的语言提供了绑定。选用 C 语言有助于创建新的绑定，同时在进行 C 语言调用时可以提供高性能。应该特别注意 Python 绑定，它支持快速原型开发，而且降低了学习 cairo 绘图 API 的门槛。
计算机绘图系统和体系结构很早就出现了，cairo 的设计借鉴了 PostScript 和 PDF 模型的许多经验。cairo 之所以借鉴 PostScript 和 Portable Document Format（PDF）方法是因为，它们都使用数学语句定义图像。由于用几何方法表示图像，所以可以在任何时候在一定范围内计算几何描述，从而重新创建整个图像（或一部分图像）。图形的几何性质被表示为点、曲线和直线（这些元素构成了矢量）。
因为可以通过重新计算数学描述来重新绘制图像或其组成部分，所以在对图像进行放大、缩小或变换时不会出现分辨率损失。但是，矢量绘图也有一些限制。例如，如果对矢量图像进行极高比例的缩放，超出了大多数人实际需要的范围，就可能导致失真。在放大时，由于计算中要对误差进行舍入，所以某些直线可能看起来不正确。在缩小时，某些直线可能会无法看到或难以分辨。
cairo 采用矢量绘图还有一个优点：矢量图像往往比较小。这是因为用相当简单的数学表达式就可以表达大量信息。矢量绘图的优点是绘图过程相当简单。由绘图库负责将点、线以及相关表达式转换为用户可以看到的东西。
描述曲线的表达式称为 **Bezier 曲线**或路径，这个名称源自数学家 Pierre Bezier。Bezier 曲线由至少两个锚点（anchor point）组成，在锚点之间还有一个或多个点，这些点称为**控制点（handle）**。移动控制点就可以改变曲线的形状。如果您用过 Photoshop 或 GIMP 这样的工具，那么可能熟悉这种曲线。但是，在使用那些工具时，最终保存的图形格式可能是位图！文件的格式决定是否保留 Bezier 路径信息，还是在给定的范围内计算它并以位图格式保存计算结果。
> 
矢量绘图与位图绘图
cairo 是一个**矢量绘图（vector drawing）**库，因此绘图需要对图形进行几何描述，而不是描述位图中填充的像素。在采用**位图绘图（bitmap drawing）**时，按照预先决定的布局用预先决定的颜色填充一系列像素，而且图形的质量与位图的大小成正比。
在放大或修改位图图像时，位图绘图方法的效果就会变差。图像常常会变得模糊，就像是近距离观看背投电视或其他大屏幕电视时的效果。在某一距离上，图像可能看起来很清楚，但是靠近之后就会看到许多离散的点。因为数据无法定义预先定义的像素之间应该是什么，所以放大时会很明显地损失清晰度。
到编写本文时，可用的 cairo 绑定包括 C++、Ruby、Perl、Java™ 语言和 .Net/mono 等等。这些绑定处于不同的开发状态，成熟度各不相同；可以通过 cairo 项目主页了解它们的最新情况（见 [参考资料](https://www.ibm.com/developerworks/cn/linux/l-cairo/index.html#artrelatedtopics) 中的链接）。目前，Python 和 C++ 绑定似乎在开放源码社区中得到了广泛采用。
正如前面提到的，一些图形工具集也为 cairo 提供了绑定，这使 cairo 开发更加简便了。2.8 之后的 Gtk+ 版本包含对 cairo 的完整支持，而且以后的 GTK 版本一直选用 cairo 作为基本绘图系统。另外，GNUstep 和 FLTK 等工具集已经开始支持用 cairo 来满足图形呈现需求。
如果您打算进行任何跨平台开发，而且需要对绘图操作和组合进行低层控制，那么选用 cairo 作为绘图 API 是非常合适的。如果需要跨平台功能，但是不需要低层绘图控制，那么可以使用在 cairo 之上构建的一些绘图库。
## 为什么要学习一种新的绘图模型？
坦率地说，我认为目前的开放源码解决方案在几个方面存在欠缺。xprint 的优点是提供了统一的显示和打印 API，但是它通常作为单独的服务器进程运行，而且它的 API 很糟糕。libgnomeprint 提供单独的打印和显示模型，但是打印和绘图 API 的分离使屏幕和打印机的呈现效果产生差异。
cairo 从以前的绘图库借鉴了许多经验，而且从一开始就以实现统一 API 为设计目标。
## cairo 呈现目标
cairo 可以呈现以下输出格式：
- X Window System（如果可能的话，会利用 Render 扩展）
- OpenGL（使用 glitz 后端）
- In-Memory Images（pixbuffs 等等）
- PostScript（适用于打印）
- PDF（Portable Document Format）文件
- SVG（Scalable Vector Graphics）格式
但是，并非所有呈现目标都产生相同的效果。尽管 cairo 力求在各种后端上产生相同的输出，但是每种后端各有优势。例如，PDF 后端会尽可能使用矢量计算（只在必要时生成图像），而 PostScript 后端实际上会为每个页面生成一个大图像。
cairo 中的呈现模型受到许多原有技术的影响。cairo 采用了 PostScript 中的路径、笔画（stroke）和填充（fill）概念，还实现了 PDF 和现代 X 服务器实现的呈现扩展中的 Porter-Duff 图像组合技术。另外，cairo 还实现了剪切、蒙板和渐变等补充特性。
## cairo 的实际应用情况
许多有影响力的开放源码项目已经采用了 cairo，cairo 已经成为 Linux 图形领域的重要软件。已经采用 cairo 的重要项目包括：
- Gtk+，一个广受喜爱的跨平台图形工具集
- Pango，一个用于布置和呈现文本的免费软件库，它主要用于实现国际化
- Gnome，一个免费的桌面环境
- Mozilla，一个跨平台的 Web 浏览器基础结构，Firefox 就是在这个基础结构上构建的
- OpenOffice.org，一个可以与 Microsoft Office 匹敌的免费办公套件
## 用 cairo 进行概念性绘图
在用 cairo 进行绘图时，最简单的操作就像手工绘画时一样：选择绘图介质，选择画笔，选择颜色，考虑线条的布局，然后进行实际绘制。cairo 文档通过与画家的作画过程进行类比来介绍 cairo 操作，下面的说明也采用这种方式。
对于画家来说，最容易的事儿通常是选择空白的绘图介质。在现实世界中，艺术家可以选择在纸上、画布上甚至墙上作画。在 cairo 中，也必须选择空白的绘图介质。在用 cairo 进行绘图时，需要设置一个 cairo 上下文，这是主对象。在这个上下文中，可以选择目标外表，比如 PostScript 文件、PDF 文档或屏幕图像。这样就可以选择我们要在什么东西上进行绘图。
现在考虑画家的下一项任务：选择画笔。画家往往花费相当长的时间来选择形状和大小合适的画笔。在 cairo 中，也有画笔形状的概念，这由笔画宽度来表达。不同的笔画宽度会产生不同粗细的线条。
接下来，cairo 用户需要以精确的坐标来表达绘图操作，这一点与现实世界中的绘画过程不同。艺术家只需将画笔放在纸上的大概位置，但是计算机必须知道进行绘图的 x 和 y 坐标。
设置了画笔和开始绘图的位置之后，就需要想像笔画的形状。简单的图形可能由直线组成，但是与现实的画家一样，也可以绘制曲线和弧线。
最后，必须定义笔画终止的位置。同样，这个位置也要用一对 (x,y) 坐标来指定。
另外，还可以给绘制的对象涂上颜色。按照 cairo 术语，这称为**填充（fill）**。对于上面描述的每种操作，cairo 中都有对应的简便的 API 实现。后面会介绍其中一些 API。
这些基本操作可以帮助您构建一些非常复杂的图形。甚至可以使用 cairo 执行画家无法轻松完成的工作，比如对现有的图形进行放大等转换，或者将图形移动到虚拟纸张上的其他地方。
GIMP 或 Photoshop 也允许执行其中的许多操作，但是 cairo 是不同的：cairo 是一种编程式绘图方式。GIMP 和 Photoshop “在幕后” 使用 cairo 这样的工具来实现绘图。在用这些工具进行绘图时，通过鼠标操作自动设置坐标点和工具类型（比如方框），并通过 GUI 环境选择画笔和笔画宽度。如示例代码所示（见 [下载](https://www.ibm.com/developerworks/cn/linux/l-cairo/index.html#artdownload)），cairo 需要显式的交互，比如 “使用笔画宽度 1 绘制一个弧线，半径为 10，中心在位置 z”。
## 正确的 cairo 术语
在讨论任何技术时，都要使用正确的术语。cairo API 的术语分为三类：核心绘图术语、外表术语和与字体相关的术语（更多细节见 [参考资料](https://www.ibm.com/developerworks/cn/linux/l-cairo/index.html#artrelatedtopics)）。
首先，cairo 有一个绘图上下文（drawing context），这相当于画家的画布。上下文是 cairo_t 类型的，要呈现图形，就必须有一个上下文。在绘图上下文上的常见操作是绘制 Bezier 图形、直线和曲线。表示一系列曲线和相关数据的 cairo 术语是**路径（path）**。可以绘制这些路径，并设置笔画宽度或填充。
使用一个非常简单的 API 将坐标转换为路径。这个 API 非常有意义，因为它使我们不必考虑线性代数或图形学课本中讨论的复杂的转换矩阵。可以对 cairo 绘图操作执行任何转换，包括对图形或部分图形进行剪切、缩放或旋转。通过指定点来绘制每条路径。cairo 按照将点连接起来的方法执行操作。在后面将提供一个示例。
接下来，讨论各种 cairo 外表类型。有几种 cairo 外表类型，它们分别对应一种输出目标。cairo **外表（surface）**是执行绘图的位置。具体地说，有用于图像（内存缓冲区）的外表、用于 Open GL 的 glitz 外表、用于呈现文档的 PDF 和 PostScript 外表以及用于直接执行绘图的 XLib 和 Win32 外表。这些外表类型都派生自外表基类型 cairo_surface_t。
在 cairo 中，**模式（pattern）**是一种可以读取的内容，它用作绘图操作的源或蒙板。cairo 中的模式可以是实体模式、基于外表的模式甚至逐变模式。
到目前为止，我们只谈到了笔画路径。但是，笔画路径一般只能产生不怎么有趣的线条图。实际上，简单的线条笔画只是 cairo 中的 5 种基本绘图操作之一。这 5 种操作是：
- cairo_stroke
- cairo_fill
- cairo_show_text/cairo_show_glyphs
- cairo_paint
- cairo_mask
尽管简单的线条绘图很方便，但是它们的表达能力不够强，不足以表示字体这样的复杂图形。cairo 为字体提供了一个基类 cairo_font_face_t。cairo 支持可缩放字体，其中包含给定字体大小的缓存标准。另外，可以用各种字体选项控制如何显示给定的字体。在使用 cairo 时，在 UNIX 上常用的字体是 Freetype 字体，在 Windows 平台上使用 Win32 字体。
## cairo 示例应用程序
我编写了一段绘制 IBM 徽标的 cairo 代码。可以从下面的 [下载](https://www.ibm.com/developerworks/cn/linux/l-cairo/index.html#artdownload) 一节下载这段代码。运行它应该会产生以下输出：
图 1. 用 cairo 生成的 IBM 徽标
![](https://img-blog.csdnimg.cn/20190302175632974.gif)
请注意代码中的 `cairo_stroke (cr)` 行，这一行出现在绘制字母之后，但在添加注册标志之前。如果不设置笔画，cairo 就不绘制任何东西。忘记设置笔画是初学者经常犯的错误。
## cairo 的发展过程
cairo 的版本号采用与 Linux 内核相似的规则 —— 也就是，奇数版本是实验性的开发版本，不适合在生产环境中使用。偶数版本是稳定版本：最初的 1.0 版本主要关注顺利地向用户提供 API 和产生高质量输出。1.2 API 完善了几个开发不太完整的后端，当前的 1.4 系列主要关注优化和添加新功能。
cairo 的开发人员提供了一些出色的示例代码片段，演示了 cairo API 的各种功能（见 [参考资料](https://www.ibm.com/developerworks/cn/linux/l-cairo/index.html#artrelatedtopics) 一节中的链接）。所以不必等待 cairo 的下一个版本了，现在就下载并试用当前版本！
## 下载资源
- [绘制 IBM 徽标的 cairo 代码](http://www.ibm.com/developerworks/apps/download/index.jsp?contentid=257733&filename=cairo-example.tar&method=http&locale=zh_CN) (cairo-example.tar | 20KB)
## 相关主题
- 您可以参阅本文在 developerWorks 全球站点上的 [英文原文](http://www.ibm.com/developerworks/linux/library/l-cairo/?S_TACT=105AGX52&S_CMP=cn-a-l)。
- [cairo 图形项目](http://cairographics.org/) 主页包含 [cairo 用户手册](http://www.cairographics.org/manual/) 和演示 cairo API 各个部分的 [代码片段](http://cairographics.org/samples/) 等内容。
- 查阅 [cairo 最新的语言和工具集绑定列表](http://www.cairographics.org/bindings)，包括用于 [Objective Caml](http://www.cairographics.org/cairo-ocaml/) 和 [GNUstep](http://savannah.gnu.org/cgi-bin/viewcvs/gnustep/gnustep/core/back/Source/cairo/) 的绑定。
- 阅读 *GNOME Journal* 文章 “[Writing a Widget Using Cairo and GTK+2.8](http://gnomejournal.org/article/34/writing-a-widget-using-cairo-and-gtk28)”。
- Technical Advisory Service for Images（TASI）提供了 [常用矢量图形术语及解释](http://www.tasi.ac.uk/advice/vector/vector-glossary.html)。
- 在 [developerWorks Linux 专区](http://www.ibm.com/developerworks/cn/linux/) 中可以找到为 Linux 开发人员准备的更多参考资料，还可以查阅 [最受欢迎的文章和教程](http://www.ibm.com/developerworks/cn/linux/top10/index.html)。
- 使用 [IBM 试用软件](http://www.ibm.com/developerworks/downloads/?S_TACT=105AGX52&S_CMP=cn-a-l) 构建您的下一个 Linux 开发项目，这些软件可以从 developerWorks 直接下载。
```cpp
#include <gtk/gtk.h>
#include <cairo.h>
#include <cairo-pdf.h>
#include <cairo-ps.h>
#include <cairo-svg.h>
#include <math.h>
#include <gdk/gdkkeysyms.h>
#include <string.h>
#define WIDTH  800
#define HEIGHT 600
#define STRIDE WIDTH*4
#define MAX_COORDS 1024
/* This path desribes what will be drawn later 
   The bulk of it is doing the IBM letters by connect the dots
   at the very end, we will get fancy and add a (R) Registered logo.
*/
static void
travel_path (cairo_t *cr)
{
   gint     pen_radius         = 10;
   cairo_set_source_rgb (cr, 1,1,1);
   cairo_paint (cr);
   cairo_set_line_width (cr, pen_radius*2);
   /* Use IBM Blue Pen Color with no ALPHA */
   cairo_set_source_rgba (cr, .3, .42, .69, 1);
   /* This Draws the IBM 8 Lines "I" Logo */
   cairo_move_to (cr, 10, 10);
   cairo_line_to (cr, 160, 10); 
   cairo_move_to (cr, 10, 40);
   cairo_line_to (cr, 160, 40);
   /* NOTE: Narrower Middle: 1/3 width */
   cairo_move_to (cr, 60, 70);
   cairo_line_to (cr, 110, 70);
   cairo_move_to (cr, 60, 100);
   cairo_line_to (cr, 110, 100);
   cairo_move_to (cr, 60, 130);
   cairo_line_to (cr, 110, 130);
   cairo_move_to (cr, 60, 160);
   cairo_line_to (cr, 110, 160);
   /* END Narrower Middle */
   cairo_move_to (cr, 10, 190);
   cairo_line_to (cr, 160, 190);
   cairo_move_to (cr, 10, 220);
   cairo_line_to (cr, 160, 220);
   /* END "I" DRAWING */
   /* This Draws the IBM 8 Lines "B" Logo */
   cairo_move_to (cr, 170, 10);
   cairo_line_to (cr, 340, 10); 
   cairo_move_to (cr, 170, 40);
   cairo_line_to (cr, 360, 40); 
   cairo_move_to (cr, 200, 70);
   cairo_line_to (cr, 250, 70);
   /*B's have holes in them! */
   cairo_move_to (cr, 300, 70); 
   cairo_line_to (cr, 360, 70); 
   cairo_move_to (cr, 210, 100);
   cairo_line_to (cr, 350, 100); 
   cairo_move_to (cr, 210, 130);
   cairo_line_to (cr, 350, 130); 
   cairo_move_to (cr, 200, 160);
   cairo_line_to (cr, 250, 160);
   /*B's have holes in them! */
   cairo_move_to (cr, 300, 160); 
   cairo_line_to (cr, 360, 160); 
   cairo_move_to (cr, 170, 190);
   cairo_line_to (cr, 360, 190); 
   cairo_move_to (cr, 170, 220);
   cairo_line_to (cr, 340, 220); 
   /* END "B" DRAWING */
	
   /* THE EVER POINTY "M"  */
   cairo_move_to (cr, 370, 10);
   cairo_line_to (cr, 470, 10); 
   cairo_move_to (cr, 560, 10);
   cairo_line_to (cr, 660, 10); 
   cairo_move_to (cr, 370, 40);
   cairo_line_to (cr, 490, 40); 
   cairo_move_to (cr, 540, 40);
   cairo_line_to (cr, 660, 40); 
   cairo_move_to (cr, 400, 70);
   cairo_line_to (cr, 510, 70); 
   cairo_move_to (cr, 520, 70);
   cairo_line_to (cr, 630, 70); 
   cairo_move_to (cr, 400, 100);
   cairo_line_to (cr, 630, 100); 
   cairo_move_to (cr, 400, 130);
   cairo_line_to (cr, 470, 130);
   cairo_move_to (cr, 480, 130);
   cairo_line_to (cr, 550, 130);
   cairo_move_to (cr, 560, 130);
   cairo_line_to (cr, 630, 130); 
   cairo_move_to (cr, 400, 160);
   cairo_line_to (cr, 470, 160);
   cairo_move_to (cr, 490, 160);
   cairo_line_to (cr, 540, 160);
   cairo_move_to (cr, 560, 160);
   cairo_line_to (cr, 630, 160); 
   cairo_move_to (cr, 370, 190);
   cairo_line_to (cr, 470, 190);
   cairo_move_to (cr, 500, 190);
   cairo_line_to (cr, 530, 190);
   cairo_move_to (cr, 560, 190);
   cairo_line_to (cr, 660, 190); 
   cairo_move_to (cr, 370, 220);
   cairo_line_to (cr, 470, 220);
   cairo_move_to (cr, 510, 220);
   cairo_line_to (cr, 520, 220);
   cairo_move_to (cr, 560, 220);
   cairo_line_to (cr, 660, 220); 
 
   /* END POINTY LETTERS */
   
   /* We stroke the path so we see everything we just specified
      by connecting the dots
   */
   cairo_stroke(cr);
 
   /* Let us add a disclaimer and show some fancy cairo: */
   /* We are going to want a nice fine lined circle around the R 
   you need to make sure you have stroked existing things
   that you wanted drawn with the larger pen before continuing. 
   */
   cairo_set_line_width (cr, pen_radius*.5);
   /* Now we will draw the fancy circle around the "R" */
   /* NOTE: The angles are in radians */
   cairo_move_to (cr, 710, 200);
   double angle1 = 0 * (M_PI/180.0);  
   double angle2 = 360 * (M_PI/180.0);
   /* We draw a large black circle */
   cairo_set_source_rgba (cr, 0, 0, 0, 1);
   cairo_arc (cr, 710, 200, 20, angle1, angle2);
   cairo_stroke (cr);
   /* We draw a smaller white circle centered on it */
   cairo_set_source_rgba (cr, 1, 1, 1, 1);
   cairo_arc (cr, 710, 200, 20, angle1, angle2);
   /* We use the fill operator to fill in the circle! */
   cairo_fill (cr);
   /* We are going to draw the letter "R" with black pen*/
   cairo_move_to (cr, 695,212); /* Bottom left of text at point */
   cairo_set_source_rgba (cr, 0, 0, 0, 1);
   cairo_select_font_face (cr, "Sans", CAIRO_FONT_SLANT_NORMAL,
                                       CAIRO_FONT_WEIGHT_BOLD);
   cairo_set_font_size (cr, 40);
   cairo_show_text (cr, "R");
    /* We stroke everything we have just done 
       to actually draw it...
    */ 
    cairo_stroke (cr);	 
}
/* Apply our path to the surface specified */
static void
draw (cairo_surface_t *surface)
{
    cairo_t *cr;
    cr = cairo_create (surface);
    /* Try applying the scale and rotate factors here to examine their effects on the output!*/
    /* cairo_rotate (cr, -M_PI / 4); */
    /* cairo_scale (cr, 2, 1.0);  */
    travel_path (cr);
    cairo_destroy (cr);
}
/* Function needed to draw to gtk window */
static void
draw_gtk (GtkWidget      *widget,
          GdkEventExpose *eev,
          gpointer        data)
{
  cairo_t *cr;
  cr = gdk_cairo_create (widget->window);
  travel_path (cr);
  cairo_destroy (cr);
}
/* We will draw our path on multiple surfaces to demonstrate 
   some of the various cairo backend
*/
int
main (gint    argc,
      gchar **argv)
{
    cairo_surface_t *surface;
    /* PDF Backend */
    surface = cairo_pdf_surface_create ("IBM.pdf",
					WIDTH, HEIGHT);
    draw (surface);
    cairo_surface_destroy (surface);
    /* Postscript Backend */
    surface = cairo_ps_surface_create ("IBM.ps",
					WIDTH, HEIGHT);
    draw (surface);
    cairo_surface_destroy (surface);
    /* Image backend */
    surface = cairo_image_surface_create (CAIRO_FORMAT_ARGB32,
					  WIDTH, HEIGHT);
    draw (surface);
    cairo_surface_write_to_png (surface, "IBM.png");
    cairo_surface_destroy (surface);
    /* Scalable Vector Graphics Backend */
    surface = cairo_svg_surface_create ("IBM.svg",
					WIDTH, HEIGHT);
    draw (surface);
    cairo_surface_destroy (surface);
    /* GTK Window using Cairo */
    gtk_init (NULL, NULL);        /* Fire up GTK!       */
    GtkWidget *mainwin;           /* Make a new windows */
    GtkWidget *canvas = NULL;     /* Make a new canvas  */
  
    mainwin = gtk_window_new (GTK_WINDOW_TOPLEVEL);
    canvas = gtk_drawing_area_new ();
    gtk_widget_set_size_request (canvas, WIDTH, HEIGHT);     
    gtk_container_add (GTK_CONTAINER (mainwin), canvas); /* Place the canvas in the window */
 
    g_signal_connect (mainwin, "destroy", G_CALLBACK (gtk_main_quit), NULL); /* Quit graphically */
    /* Instead of drawing like usual, we connect the expose event to do the drawing! */
    g_signal_connect (G_OBJECT (canvas), "expose-event",
                      G_CALLBACK (draw_gtk), NULL);
    gtk_widget_show_all (mainwin); /* Show the window on the screen */
    gtk_main ();
    return 0;
}
```
