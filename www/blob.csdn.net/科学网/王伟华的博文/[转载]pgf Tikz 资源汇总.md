# 科学网—[转载]pgf/Tikz 资源汇总 - 王伟华的博文




# [转载]pgf/Tikz 资源汇总                           

已有 6098 次阅读2015-1-12 19:28|个人分类:[科技论文写作、搜索经验集锦](http://blog.sciencenet.cn/home.php?mod=space&uid=81613&do=blog&classid=104604&view=me)|系统分类:[科研笔记](http://blog.sciencenet.cn/home.php?mod=space&do=blog&view=all&uid=81613&catid=1)|文章来源:转载



LaTeX以及相关“红包”（宏包） 更多的用法和模板见[http://bbs.ctex.org/?fromuid=91689](http://bbs.ctex.org/?fromuid=91689) 和[http://bbs.chinatex.org/?fromuid=95511](http://bbs.chinatex.org/?fromuid=95511)

LaTeX编辑部[http://zzg34b.w3.c361.com/index.htm](http://zzg34b.w3.c361.com/index.htm)

[LaTeX的一些宏包及细节知识](http://blog.chinaunix.net/uid-20289887-id-1710422.html)


[fancyhdr页眉页脚设计史上最全代码](http://blog.sina.com.cn/s/blog_5e16f1770100g46l.html)

[常见图形宏包合集与示例](http://blog.sina.com.cn/s/blog_5e16f1770100gywd.html)



PGF/TikZ


1. 什么是pgf？
pgf是一个在tex系统中的画图宏包，tex尤其是beamer中使用pgf作图，“作精确图还比较方便, 色彩锐丽”（huangzh73）。除了可以精确的作图外，对于某些不要求精确控制的图形绘制，如：流程图，树图，等等，也提供了简便易用的支持。

PGF and TikZ examples gallery：[http://www.fauskes.net/pgftikzexamples/](http://www.fauskes.net/pgftikzexamples/)
A comprehensive list of PGF and TikZ examples: [http://www.texample.net/tikz/examples/all/list/](http://www.texample.net/tikz/examples/all/list/)
Edward Tufte’s book Beautiful evidence:

[http://www.edwardtufte.com/bboard/q-and-a-fetch-msg?msg_id=0001TV&topic_id=1](http://www.edwardtufte.com/bboard/q-and-a-fetch-msg?msg_id=0001TV&topic_id=1)

有个法国教师做了2d几何图的扩展包tkz-2d：
主页：[http://www.altermundus.fr/](http://www.altermundus.fr/)
下载：[http://www.altermundus.fr/pages/downloads/](http://www.altermundus.fr/pages/downloads/)

下面是一些自己用过的例子
数据流图：[http://www.fauskes.net/pgftikzexamples/simple-flow-chart/](http://www.fauskes.net/pgftikzexamples/simple-flow-chart/)
基于数据流的系统架构图：[http://www.fauskes.net/pgftikzexamples/inertial-navigation-system/](http://www.fauskes.net/pgftikzexamples/inertial-navigation-system/)
时序图: [http://www.fauskes.net/pgftikzexamples/pgf-umlsd/](http://www.fauskes.net/pgftikzexamples/pgf-umlsd/)
二维的数据图plot2d：
[http://www.fauskes.net/pgftikzexamples/tkz-plot2d/](http://www.fauskes.net/pgftikzexamples/tkz-plot2d/)
[http://www.fauskes.net/pgftikzexamples/pgfplots/](http://www.fauskes.net/pgftikzexamples/pgfplots/)
有限状态机图：[http://www.fauskes.net/pgftikzexamples/state-machine/](http://www.fauskes.net/pgftikzexamples/state-machine/)
二维几何图：[http://www.fauskes.net/pgftikzexamples/tkz-2d/](http://www.fauskes.net/pgftikzexamples/tkz-2d/)
三维几何图：[http://www.fauskes.net/pgftikzexamples/3d-cone/](http://www.fauskes.net/pgftikzexamples/3d-cone/)
正则表达式图：[http://www.fauskes.net/pgftikzexamples/diagram-chains/](http://www.fauskes.net/pgftikzexamples/diagram-chains/)
图论相关图样：[http://www.fauskes.net/pgftikzexamples/diagram-chains/](http://www.fauskes.net/pgftikzexamples/diagram-chains/)
分类图：[http://www.fauskes.net/pgftikzexamples/computer-science-mindmap/](http://www.fauskes.net/pgftikzexamples/computer-science-mindmap/)
公式说明图：[http://www.fauskes.net/pgftikzexamples/beamer-arrows/](http://www.fauskes.net/pgftikzexamples/beamer-arrows/)
负反馈系统控制图：[http://www.fauskes.net/pgftikzexamples/control-system-principles/](http://www.fauskes.net/pgftikzexamples/control-system-principles/)
便签条图：[http://www.fauskes.net/pgftikzexamples/boxes-with-text-and-math/](http://www.fauskes.net/pgftikzexamples/boxes-with-text-and-math/)
绘制二维迷宫：[http://www.fauskes.net/pgftikzexamples/maze/](http://www.fauskes.net/pgftikzexamples/maze/)
神经网络图：[http://www.fauskes.net/pgftikzexamples/neural-network/](http://www.fauskes.net/pgftikzexamples/neural-network/)
三维曲面：[http://cs.nju.edu.cn/yangxc/dcc2003.files/matlab/matlab/2_3_2.htm](http://cs.nju.edu.cn/yangxc/dcc2003.files/matlab/matlab/2_3_2.htm)
pgf画Venn图的例子[http://bbs.ctex.org/viewthread.php?tid=36595](http://bbs.ctex.org/viewthread.php?tid=36595)
下面是用MetaPost画Venn图的另一个例子[http://bbs.ctex.org/viewthread.php?tid=35348](http://bbs.ctex.org/viewthread.php?tid=35348)
0. PGF/TikZ 1.10 版手册前7章译文，李文军先生翻译，见本帖附件
1.  PGF/TikZ 学习笔记(1) —— 机械手臂绘制与标注
[http://bbs.ctex.org/viewthread.php?tid=40420](http://bbs.ctex.org/viewthread.php?tid=40420)
2.  PGF/TikZ 学习笔记(2) —— 带有半透明阴影效果的文本盒
[http://bbs.ctex.org/viewthread.php?tid=40454&extra=page=1](http://bbs.ctex.org/viewthread.php?tid=40454&extra=page=1)
3.  PGF/TikZ 学习笔记(3) —— 使用 TikZ 对 LaTeX 文档添加批注
[http://bbs.ctex.org/viewthread.php?tid=40633&extra=page=1](http://bbs.ctex.org/viewthread.php?tid=40633&extra=page=1)
---- Frogge 对此作了一些改进，见[http://bbs.ctex.org/viewthread.php?tid=42079&extra=page=1](http://bbs.ctex.org/viewthread.php?tid=42079&extra=page=1)第 11 帖。
4.  PGF/TikZ 与Gnuplot 配合使用
[http://bbs.ctex.org/viewthread.php?tid=40364&extra=page=1](http://bbs.ctex.org/viewthread.php?tid=40364&extra=page=1)
5. 如何画椭圆中心到椭圆上任一点的连线
[http://bbs.ctex.org/viewthread.php?tid=40732&extra=page=2](http://bbs.ctex.org/viewthread.php?tid=40732&extra=page=2)
6. enhanced support for drawing ER diagrams
[http://bbs.ctex.org/viewthread.php?tid=40461&extra=page=2](http://bbs.ctex.org/viewthread.php?tid=40461&extra=page=2)
7. pgf与Mathematica结合画图
[http://bbs.ctex.org/viewthread.php?tid=40700&highlight=](http://bbs.ctex.org/viewthread.php?tid=40700&highlight=)
8. 例学pgf_pgfmanual封面代码分析
[http://bbs.ctex.org/viewthread.php?tid=58922](http://bbs.ctex.org/viewthread.php?tid=58922)

以上所有文档在这里下载：

[http://cid-3cfe530319707374.office.live.com/browse.aspx/ChinaTeX/TIKZ-PGF](http://cid-3cfe530319707374.office.live.com/browse.aspx/ChinaTeX/TIKZ-PGF)

**利用命令绘图**

利用命令可以精准控制图形的形状和位置，对于结构性较强的图形，利用命令画图比手工绘图更值得推荐。LaTeX 本身有一些命令可以绘制简单的图形，但绘制复杂图形则需要使用一些宏包，其中常用的宏包有：
- 
tikz
非常强大的作图宏包，几乎可以画任何图形。甚至可以绘制简单的函数图像。其官方使用手册的最新版厚达726页。网上也有非常多的实例展示如何用 tikz 命令绘制各种图形，例如这个网页。

- 
pstricks
老牌的作图宏包，也非常强大。但不支持 pdflatex 编译。

- 
metapost
这是在 LaTeX 诞生之初就有的绘图工具，但因为不是 LaTeX 的宏包，而只是一个外部命令行工具，使用起来不够方便。不能直接在 LaTeX 中用代码画图，而必须用 metapost 命令画好图生成 eps 或 pdf 格式的文件供 LaTeX 调用。不过， metapost 的绘图能力独步天下，大概只有 tikz 可以与之匹敌。

- 
gnuplot
外部命令行工具，绘制函数图像的不二选择。提供和 LaTeX 的接口。

- 
xy-pic（其实宏包名为xy）
如果是画交换图，特别是范畴论中的图形，使用 xy 宏包会极为方便。但画结构性不那么好的图形则比较麻烦。

- 
bussproof
写 Gentzen 式树状逻辑推演极为方便。

- 
qtree
画 tableau 证明树或语法分析树极为方便，但树枝没有箭头。


其中前 3 种熟练掌握一种就完全够用了，后 4 种则是面向特殊用途的。

**利用**** GUI ****绘图软件绘图**

毕竟有些复杂的图用命令绘制仍然不方便（特别是结构性不那么好的图），这时需要使用外部绘图软件先手工绘制出图形，然后在 LaTeX 文档中调用由这些软件生成的图片或 tex 代码。理论上，任何绘图软件都可以生成可供 LaTeX 调用的图片，但考虑到有些图形上需要添加公式，这时普通的绘图软件就不够用了。我所了解的支持添加 LaTeX 公式的绘图软件有如下这些：
- 
Inkscape
非常强大的矢量绘图软件，可实现很多复杂的效果，跨平台，且支持多种文件格式保存。Ubuntu 可通过源安装。没有特别声明支持 LaTeX，但实际上所绘图片可以直接存成 tex 格式（其代码利用了 pstricks 宏包），也可以存成 pdf 文件，然后在保存选项中选择包含 LaTeX 代码（用于处理图片中的公式），Inkscape 会生成一个名为![[转]LaTeX作图工具介绍](/static/ueditor/themes/default/images/spacer.gif).pdf*tex**的文件，最后在** LaTeX **主文档中使用** input **命令包含这个文件即可。如果不需要绘制函数图形，**Inkscape **是这里所列的绘图软件中绘图能力最强的。*

- 
*Ipe**比** Inkscape **小巧，因而绘图功能也较弱，但如果只需要绘制简单图形，也够用了。不能导出为** tex **代码，直接生成** eps **或** pdf **格式图片供** LaTeX **文档调用，能自动剪裁图片大小，去掉白边。跨平台。**Ubuntu **可通过源安装。**Linux **下必须通过命令行启动。*

- 
*LaTeXDraw**与** Ipe **类似。好处是在手工绘图的同时自动生成** tex **代码（利用了** pstricks **宏包）。跨平台。**Ubuntu **可通过源安装。*

- 
*XFig**比较老牌的支持** LaTeX **的** GUI **绘图软件。手工绘图后生成** .pstex**（存储图片信息）和** .pstex*t（存储图片中的公式信息）文件供 tex 主文档调用。跨平台。Ubuntu 可通过源安装。虽然不是专业的图片编辑软件，但与 Inkscapte 相比，XFig 处理简单的数学图形可能更方便。缺点是：界面丑陋，而且不支持 pdflatex 编译，要先用 latex 编译，然后转成 pdf。

- 
TpX
是我接触最早的支持 LaTeX 的 GUI 绘图软件，据说是一个经济学家因为要出书，图片太多，不方便处理，所以自己动手写了这个软件。与 Ipe 类似。小巧，方便。缺点是只支持 Windows。GeoGebra，专门绘制函数图像，支持导出为 tikz 或 pstricks 代码，跨平台。Ubuntu 可通过源安装。

- 
Dia
专门绘制流程图，支持导出为 tikz 或 pstricks 代码，跨平台。Ubuntu 可通过源安装。



转载本文请联系原作者获取授权，同时请注明本文来自王伟华科学网博客。
链接地址：[http://blog.sciencenet.cn/blog-81613-858832.html](http://blog.sciencenet.cn/blog-81613-858832.html)

上一篇：[欢度春节](blog-81613-855442.html)
下一篇：[科研实用计算机技术四部曲](blog-81613-859441.html)


