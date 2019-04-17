# 科学网—[转载]Latex相关资源汇总 - 王伟华的博文




# [转载]Latex相关资源汇总                           

已有 20051 次阅读2010-5-10 22:00|个人分类:[科技论文写作、搜索经验集锦](http://blog.sciencenet.cn/home.php?mod=space&uid=81613&do=blog&classid=104604&view=me)|系统分类:[科研笔记](http://blog.sciencenet.cn/home.php?mod=space&do=blog&view=all&uid=81613&catid=1)|关键词:Latex,,,,LaTeX|[LaTeX](misc.php?mod=tag&id=4885)|文章来源:转载



# 前言

      latex并不难，latex也不是艺术，而是给Knowledge Engineers的一个撰文工具，仅此而已。一篇文章真正的价值在于里面的发现和思维逻辑，文本、图片、排版都只是形式罢了。如果没有好的内容，就是做的再漂亮，也不会得到别人的认可。可是，大家都是俗人，都还是会喜欢美轮美奂的东西。形式很多时候也不可缺，可以帮助推销自己的paper。认同了这两点，可以开始下面的阅读。 



# Latex简介

     TeX是计算机科学家图灵奖得主Knuth教授设计的一款权威的科技论文排版软件！更重要的它是开源 (Open Source)的，Knuth教授无偿公开了它的所有源代码。正因为这个原因，无数的爱好者们大胆尝试TeX。并在其基础上开发了一个宏集 ――Latex。Latex方便好用，被广泛传播，成了当今世界科技界最权威的论文排版软件。

     TeX和LaTeX排版软件和MS的Word软件不同，不个是''所见即所得''(WYSIWYG，what you is see what you get)，一个是''所想即所得''(WYWWYG，what you want what you get)。风格上迥然不同，因此需要稍微改变一下自己的习惯。 

TeX/LaTeX是一种计算机文档制作语言，所以它有
a. 源代码。 包括故障区域和begin{document}之前的部分；
b. 编译环境。 包括TeX/LaTeX发行版本和版本号，比如ctex-2.4.5-7。
c. 编译方法。 比如生成pdf文件，就有pdflatex、latex+dvipdfm(x)、latex+dvips+ps2pdf等多种方法。有时ps2pdf无法完成格式转换，可以用acrobat distiller。 
Tex的workflow如下图所示： 
![tex-workflow.jpg](http://fmn010.xnimg.cn/fmn010/blog/20080819/22/37/A663672558934CHA.jpg)
从tex文件到pdf文件的编译序列很多，有pdflatex、latex + dvipdfm(x)、latex + dvips + ps2pdf。个人推荐latex + dvipdfmx。这样的编译遍数较少，生成速度较快。生成的pdf文件字体更清晰。 

关于TeX(LaTeX)的学习，毫无疑问，Knuth(高德纳)：Donald~E.~Knuth, ''The TeX book'', Addison-Wesley, 1984，%7Eknuth/，是权威之选，本书排版堪称完美，从中可以看出大师的魅力。 
此外，最好的一个简短详细的文献是''lshort''，这个中文都有(一份不太简短的LaTeX介绍).  
最后，IEEEtran_HOWTO (in IEEEtrans template)也有很不错的latex用法介绍。[http://www.ieee.org/web/publications/authors/transjnl/index.html](http://www.ieee.org/web/publications/authors/transjnl/index.html)
CTex下载： [http://www.ctex.org/HomePage](http://www.ctex.org/HomePage)
常用论坛，答疑解惑： [http://bbs.ctex.org](http://bbs.ctex.org)
搜索和下载package的站点：[http://www.ctan.org/](http://www.ctan.org/)
一个有趣的blog: [http://latex.yo2.cn/articles/latex_blog.html](http://latex.yo2.cn/articles/latex_blog.html)

# Latex下的绘图工具 

写文章最重要的就是形象思维，精致的图能给Reviewer良好的第一印象，也体现了Author的专业与否。 各IEEE journal都只接受矢量图。使用点阵图的paper很可能在上传的时候被拒绝。画矢量图的常用工具如下： 
Adobe Illustrator: 鼠标绘图中最好的工具，做出来的图可以很艺术化。Adobe的东西强于CorelDraw。自己整理了一个网上的教程，下次上传上来。 
MetaPost: 一种功能强大但比较底层的绘图语言。差不多就在DC的级别画像素点。适合画几何图形，尤其是因为它递归函数调用的能力。 
PGF/TikZ： 语言描述层面的绘图工具。在node和connection那个逻辑层面上的，有节点自动布局和自动布线的功能。用它画Venn图的语法也很简单。 
visio: 适合画流程图.
latexcad: 在图上面附加Label。Label代码全部嵌入在tex的源文件里面。
xymatrix: 适合pi演算那样的公式推导。 
数据图
gnuplot:
matlab: 
origin: 
关于TeX和LaTeX的插图方面可以关参考王磊的《latex中文插图指南》，配合它的源文件，可以非常有效学习和掌握插图方面的技巧。 

## MetaPost相关资料 



MetaPost适合几何图形绘制，有强大的递归执行能力，比如下面的分形结构。 
![tree.jpg](http://fmn006.xnimg.cn/fmn006/blog/20080820/12/29/A654853945791CHA.jpg)

MetaPost Examples:  [http://www.math.zju.edu.cn/ligangliu/LaTeXForum/MetaPost/Metapost_Examples.htm](http://www.math.zju.edu.cn/ligangliu/LaTeXForum/MetaPost/Metapost_Examples.htm)
MetaPost Intro: [http://people.ku.edu/~syliu/shredderyin/metapost.html](http://people.ku.edu/~syliu/shredderyin/metapost.html)
MetaPost Configuration [http://huagw.blog.hexun.com/15332414_d.html](http://huagw.blog.hexun.com/15332414_d.html)
王垠的个人主页 [http://people.ku.edu/~syliu/shredderyin/](http://people.ku.edu/~syliu/shredderyin/)
John Hobby为MetaPost写的用户手册''A User's Manual for MetaPost'' 
MPS的图片文件转EPS - 不用再纠缠于prologues的设置 [http://www.ida.liu.se/~joned/download/mps2eps/](http://www.ida.liu.se/~joned/download/mps2eps/)
Learning METAPOST by Doing: [http://www.tlhiv.org/MetaPost/documentation/MetaPost_Learning.pdf](http://www.tlhiv.org/MetaPost/documentation/MetaPost_Learning.pdf)
MetaPost illustration examples (for a mathematics textbook): [http://www.topology.org/tex/conc/mp/](http://www.topology.org/tex/conc/mp/)

## PGF/TikZ 



1. 什么是pgf？
pgf是一个在tex系统中的画图宏包，tex尤其是beamer中使用pgf作图，“作精确图还比较方便, 色彩锐丽”（huangzh73）。除了可以精确的作图外，对于某些不要求精确控制的图形绘制，如：流程图，树图，等等，也提供了简便易用的支持。 

PGF and TikZ examples gallery： [http://www.fauskes.net/pgftikzexamples/](http://www.fauskes.net/pgftikzexamples/)
A comprehensive list of PGF and TikZ examples: [http://www.texample.net/tikz/examples/all/list/](http://www.texample.net/tikz/examples/all/list/)
Edward Tufte’s book Beautiful evidence:  [http://www.edwardtufte.com/bboard/q-and-a-fetch-msg?msg_id=0001TV&topic_id=1](http://www.edwardtufte.com/bboard/q-and-a-fetch-msg?msg_id=0001TV&topic_id=1)

有个法国教师做了2d几何图的扩展包tkz-2d： 
主页： [http://www.altermundus.fr/](http://www.altermundus.fr/)
下载： [http://www.altermundus.fr/pages/downloads/](http://www.altermundus.fr/pages/downloads/)

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
三维曲面： [http://cs.nju.edu.cn/yangxc/dcc2003.files/matlab/matlab/2_3_2.htm](http://cs.nju.edu.cn/yangxc/dcc2003.files/matlab/matlab/2_3_2.htm)
pgf画Venn图的例子 [http://bbs.ctex.org/viewthread.php?tid=36595](http://bbs.ctex.org/viewthread.php?tid=36595)
下面是用MetaPost画Venn图的另一个例子 [http://bbs.ctex.org/viewthread.php?tid=35348](http://bbs.ctex.org/viewthread.php?tid=35348)

2. pgf的作者？
pgf也是beamer 的作者 Till Tantau 教授开发的. 起初只有 pgf, 后来有了 tikz and pgf 宏包的说法, 一般还是称为 pgf 宏包. 在使用中 usepackage{tikz} 就自动加入了 pgf. 也许 tikz 可以认为是 pgf 进一步发展的产物.正因为两者出自同一个牛人之手，所以beamer和pgf结合使用确实非常的完美。 

3.安装和更新的方法:   
pgf在ctex下的安装可参考beamer的安装。它们的关系差不多是beamer requires pgf，pgf requires xcolor。 

## Matlab画数据图的一个例子 



hand = plot(xx, yy, 'k^-.', xx, yy, 'ko-.', xx, yy, 'b^--', xx, yy, 'bo--', xx, yy, 'r^-', xx, yy, 'ro-'); 
set(hand, 'LineWidth', 2); 
hand = legend('label1', 'label2', 'label3', 'label4', 'label5', 'label6', 1); 
set(hand, 'FontSize', 14); 
hand = xlabel('xlabel'); 
set(hand, 'FontSize', 14); 
hand = ylabel('ylabel'); 
set(hand, 'FontSize', 14); 
xlim([xmin, xmax]); 
ylim([xmin, ymax]); 
还没找到怎么设置XAxis和YAxis的字体大小的办法。 

Origin画的数据图比Matlab好看些，但有时也感觉Origin太艳丽了，不那么正统。也还推荐gnuPlot。 

# GnuPlot的安装和使用 

个人感觉GnuPlot做出来的图没有Pgf和Origin的效果好。 
推荐资料： Plotting with GnuPlot:   [http://f3wm.free.fr/linux/gnuplot.html](http://f3wm.free.fr/linux/gnuplot.html)

1. Windows下的安装 
安装很简单。在 [http://www.gnuplot.info/](http://www.gnuplot.info/)下载安装包gp424win32.zip. 解压后找到bin/wgnuplot.exe，双击运行即可。 
测试一下安装。运行命令： 
gnuplot> plot [-3.14:3.14] sin(x) 

2. 和Latex的整合 
gnuplot提供了直接输出tex文件的功能，只需要把output设置为latex就可以了。 
在命令行下输入：
set terminal latex 
set output "sin.tex" 
plot [-3.14:3.14] sin(x)      
把这个文件直接插入你的文章中，例如
begin{figure}
  begin{center}
    input{sin.tex} 
  end{center}
end{figure} 

3. 生成xy axis和label 
set terminal latex    
set output "sinagain.tex" 
set size 5/5.,4/3.  #设置图片的大小 
set format xy "$%g$" #设置x、y轴文字的格式 
set title "This is a plot of $y=sin(x)$" #设置图片标题 
set xlabel "This is the $x$ axis" #设置x轴的文字 
set ylabel "This is the $y$ axis" #设置y轴的文字 
plot [0:6.28] [0:1] sin(x)  

4. 多条曲线的对比 
set terminal latex 
set output "combine.tex" 
set size 5/5.,4/3.  #设置图片的大小 
set format xy "$%g$" 
set title "Combination" 
set xlabel "$x$ axis" 
set ylabel "$y$ axis" 
plot [-3.14:3.14] 0.2*x with points, sin(x) with lines 

5. 一个3d作图和并列子图显示的例子  
set terminal latex 
set output "tic.tex" 
set format y "$%g$" 
set format x "$%.2f$" 
set title "This is $\sin(x)$" 
set xlabel "This is the $x$ axis" 
set ylabel "$\sin(x)$" 
set xtics -pi, pi/4 #设置x的间距 
plot [-pi:pi] sin(x) 
set terminal latex 
set output 'plot3d.tex' 
set samples 20, 20 
set isosamples 21, 21 
set contour base 
set cntrparam levels auto 10 
set title "3D gnuplot demo - some more interesting contours" 0.000000,0.000000  font "" 
set xlabel "X axis" -5.000000,-2.000000  font "" 
set ylabel "Y axis" 4.000000,-1.000000  font "" 
set zlabel "Z axis" 0.000000,0.000000  font "" 
splot [x=-3:3] [y=-3:3] sin(x) * cos(y) 

begin{figure*}[!t] 
centerline{% 
subfigure[Case I]{input{tic.tex}% 
label{fig_first_case}} 
hfil 
subfigure[Case II]{input{plot3d.tex}% 
label{fig_second_case}}% 
} 
caption{Simulation results} 
label{fig_sim} 
end{figure*} 

# LaTeX2html安装及在winEdt界面中的配置 



任何Academic person都有需要构建个人主页，加强与外界的交流；撰写课件，教书育人。Html因为其易访问性成为最适合的格式。Latex2html和TtH能将tex文档编译成html，并较好的支持公式的输出，当然它们look and feel也比较academic，可以用来装学者。 



**系统 **
Windows XP SP2  
CTeX-2.4.6-Full  
Perl编译器：ActivePerl 5.10.0.1003 for Windows (x86). [http://www.activestate.com/Products/Downlo...x?id=ActivePerl](http://www.activestate.com/Products/Downlo...x?id=ActivePerl)
NetPBM 图形软件(CTeX-2.4.6-Ful l没有安装) ：现在网上很难找到合适的版本(据说1.2是work的)．用TtH还是比Latex2html简单一点．  
Postscript 解释器： CTeX-2.4.6-Full 已安装好了Ghostscript，在目录C:CTeXGhostgum下。 
注册序列码如下。Name: Registered  s/n: 195938022 5598 
其余的安装参考：[http://bbs.ctex.org/redirect.php?fid=7&tid=43334&goto=nextnewset](http://bbs.ctex.org/redirect.php?fid=7&tid=43334&goto=nextnewset)

Tex4ht在CTex中的配置，参考 [http://xaero.mmiyy.cn/notes/latex/tex4ht.htm](http://xaero.mmiyy.cn/notes/latex/tex4ht.htm)
1.　把C:CTeXWinEdtBinTeXTtH.bat改名为TtH.bat.bak
2.　把C:CTeX...htlatex.bat（具体路径在C:CTex下面search一下，不同CTex版本不同）复制到C:CTeXWinEdtBinTeX下，改名为TtH.bat，则Accessories/HTML/TtH(Tex—>HTML)菜单变得可用。 

**Tex4ht中数学公式的转换 **

Tex4ht 先将数学公式转为dvi, 再利用外部软件, 如：ImageMagick, 转换为.png, .jpg or .gif文件。
依据我的经验，尚需要以下步骤才能使Tex4Ht正确转换数学公式为png文件：
4. 安装ImageMagick 最新版本，Google搜索可以找到很多下载地址。 注意，为了避免在转换Tex时出现莫明其妙错误， Ctex以及ImageMagick安装目录中不要带有空格，中文等特殊字符。
5. 测试ImageMagick是否安装成功，运行终端程序cmd，输入convert, 如果能正确找到文件就安装成功。
6. 依然要修改 ％Ctex%texmftex4htbasewin32tex4ht.env 文件，找到：alternative instructions for old versions of convert 将该节中 Gconvert 前的 % 全部去掉。
至此 大功告成。 应该可能正常转换带数学符号的， 可以试试以下Tex文档： 

documentclass{article}
usepackage{CJK}
usepackage{tex4ht}
begin{document}
begin{CJK*}{GBK}{song}
测试文档\ test document\
begin{math}frac{pi}{gamma}
\frac{a+b-c}{d+e-k }
end{math}
end{CJK*}
end{document} 

# Beamer相关资料

Slides和Presentation的重要性就不必说了。 
A Beamer Quickstart (推荐) ：[http://heather.cs.ucdavis.edu/~matloff/beamer.html](http://heather.cs.ucdavis.edu/~matloff/beamer.html )
A practical guide to beamer: [http://faq.ktug.or.kr/wiki/uploads/beamer_guide.pdf](http://faq.ktug.or.kr/wiki/uploads/beamer_guide.pdf)
黄正华老师的总结: [http://bbs.ctex.org/viewthread.php?tid=27695&extra=&page=1](http://bbs.ctex.org/viewthread.php?tid=27695&extra=&page=1)
Beamer的在线教程: [http://www.math.umbc.edu/~rouben/beamer/quickstart.html](http://www.math.umbc.edu/~rouben/beamer/quickstart.html)
Beamer的安装：[http://dsec.math.pku.edu.cn/~yuhj/wiki/TeXSlides.html#sec-1](http://dsec.math.pku.edu.cn/~yuhj/wiki/TeXSlides.html#sec-1)
一个即刻上手的模板：[http://www.bossh.net/forums/index.php?showtopic=1638](http://www.bossh.net/forums/index.php?showtopic=1638)
推荐一个好的Beamer模板库-非常简洁: [http://www.math.sinica.edu.tw/www/tex/beamer_template.jsp](http://www.math.sinica.edu.tw/www/tex/beamer_template.jsp )
Beamer Tips: [http://xueruini.myipcn.org/publish/TeX/Beamer.html](http://xueruini.myipcn.org/publish/TeX/Beamer.html )
用 Beamer 做相册:  [http://linux.cs.nthu.edu.tw/~clark/nb/archives/2008-02-03T23_32_45.php](http://linux.cs.nthu.edu.tw/~clark/nb/archives/2008-02-03T23_32_45.php)





**beamer的安装配置 **1、下载beamer：[http://sourceforge.net/projects/latex-beamer/](http://sourceforge.net/projects/latex-beamer/)
　 下载pgf：[http://sourceforge.net/projects/pgf/](http://sourceforge.net/projects/pgf/)
　 下载xcolor：[http://sourceforge.net/projects/xcolor/](http://sourceforge.net/projects/xcolor/)
2、解压到各自文件夹beamer、pgf、xcolor
3、将beamer、pgf、xcolor复制到TeXHOMEctexlocaltextmftexlatex，并删除 TeXHOME/texmf/tex/latex/ 下的beamer、pgf、xcolor 
4、进入TeXHOMEtexlocaltextmftexlatexxcolor，用“记事本”打开 xcolor.ins，启动cmd，定位到这个文件夹 
5、运行命令： `latex xcolor.ins'，`latex xcolor.dtx'，`latex xcolor.dtx'，`makeindex -s gind.ist xcolor.idx'，`latex xcolor.dtx'，`latex xcolor.dtx'
(这些命令在xcolor.ins文件里都写出来了)
6、测试安装是否成功，运行命令：`latex xcolor1.tex'， `latex xcolor1.tex'
7、然后运行 WinEdt 的 Accessories 菜单下 MikTex options 的 refresh now 即可。 

注意事项： 当beamer做幻灯片时，需要使用 usepackage[square, authoryear, comma]{natbib} 和 citep{xxx} 做文章引用。因为用 natbib 和 citep 才能在beamer下很好的显示 square bracket。另外，做幻灯时，推荐用 bibliographystyle{authordate1}。这样方便通过引用标记（作者+年份）看到该引文的分量。 

# Bibtex的使用　

Reviewer在读你的Paper时，第一件事是读标题，了解你的Niche,你的Research要解决的问题；第二件事就是查阅你的Bibliography，目的是： (1) 看看你有没有很合适的引用他的文章(Academic guys对自己的work的impact ratio是很在意的)。 (2) 也同时看看你引用的文章都来自于什么级别的刊物和会议，大概的年份分布(有没有反映最新的研究进展,从这里就能看出survey about related work做得够不够,治学严谨与否).Experts in this field 也会注意本领域的经典文章有没有引用. (3) 查看一下Paper quality如何.bibliography算是paper的小角落.处理不好就会给reviewers这篇paper文字工作不够细致的印象. 比如说, 作者名字缩写, 会议名的缩写, etc. 这方面别全依赖于工具,如reference manager或endnote. 它们的item也都是从IEEE, ACM 这样的站点下载的。 

Bibtex的作用是从.bib生成latex可直接识别的bibitem的.bbl格式。这个格式转换的风格由BibTex Style （BST）file决定。常用的BST如下： 
IEEEtranBST：[http://www.ctan.org/tex-archive/macros/latex/contrib/IEEEtran/bibtex/](http://www.ctan.org/tex-archive/macros/latex/contrib/IEEEtran/bibtex/)
中文的文章的bst：[http://bbs.ctex.org/viewthread.php?tid=33591](http://bbs.ctex.org/viewthread.php?tid=33591)

为了正确的生成Reference List, makefile需要调用latex（或pdflatex）三遍。 
latex %texfilename%
bibtex %texfilename%
latex %texfilename%
latex %texfilename% 
参考链接 [http://bbs.ctex.org/viewthread.php?action=printable&tid=950](http://bbs.ctex.org/viewthread.php?action=printable&tid=950)

在正文里面引用时，IEEE模板需要引用cite package。具体参考IEEEtranBST_howto 
ifCLASSOPTIONcompsoc
% requires cite.sty v4.0 or later (November 2003)
usepackage[nocompress]{cite}
else
usepackage{cite}
fi 
正文里使用cite{%label1%,%label2%,%label3%}命令。 
新手注意：Reference list里面只会显示那些正文里面引用了的那些文章。如果正文没有cite，别怪bibtex报错。另外，对图片、表格、公式的引用是ref命令，别搞混了。 参考：[http://latex.yo2.cn/articles/latex-bibtex-introduction.html](http://latex.yo2.cn/articles/latex-bibtex-introduction.html)

# Other Tips 细节决定成败 


**怎么输入左单引号、左双引号、右单引号、有双引号？ **
左单引号：`(键盘上1旁边的那个)；左双引号：``；
右单引号：'(键盘分号的右边那个)；右双引号：''或"。
在普通模式下，''和"是一样的；但在数学模式下，''是两个导数符号。 

**怎么输入“度数”之类的单位？ **试试SIunits包。 

**我用了bib文件来保存citation，但所有title里面的单词都成了小写，怎么保持一些abbrevation的大写状态？ **
用括号把title中的abbreviation框起来，如下： 
{TOSSIM}: accurate and scalable simulation of entire TinyOS applications。 

**怎么把"Chapter x"换成“第x章”，"Figure x"换成“图x”，"Theorem"换成“定理”， "Proof"换成“证明”……？ **
前两个建议用CCT或CJK的CJKcaption{GB}命令来解决。
第三个用newtheorem{theorem}{定理}。以后这么用：
       begin{theorem}...end{theorem}
第四个用renewcommand{proofname}{证明}可以解决(需要amsmath包)。
类似的问题可以参考CJK的GB.cap和amsmath的文档。 

**我要写算法伪代码/C/C++/Java...代码，怎么办？**
listings包不错，不妨一试。算法伪代码么，个人觉得algorithm2e不错。

**latex提供哪些定理环境？**
很遗憾，没有。但是我们可以自己定义，而且amsthm包已经预定义了三种style。
下面是我自己常用的定义，大家作个参考吧。:)
  theoremstyle{plain}
  newtheorem{thm}{Theorem}
  newtheorem{lem}[thm]{Lemma}
  newtheorem{cor}[thm]{Corollary}
  newtheorem{clm}[thm]{Claim}
  newtheorem{prop}{Proposition}
  newtheorem{fact}{Fact}
  theoremstyle{definition}
  newtheorem{defi}{Definition}
  newtheorem{example}{Example}
  theoremstyle{remark}
  newtheorem{remark}{Remark}
  newtheorem{proviso}{Proviso}
amsthm还定义了proof环境，最后会自动加一个QED符号。很方便吧？
需要提醒大家：它和一些会议、杂志提供的文档类冲突。这些会议、杂志提供的文档类一般会提供类似的环境，大家要看看它们的文档。 

**怎么插入一张图片到latex中？**
usepackage{graphicx}
includegraphics{xxxx.yyy}
但是这样还没有搞定。LaTeX在编译的时候必须知道图片的大小尺寸，称作bounding   box。以下方法可以告诉LaTeX bounding box： 
注意：Bounding Box的问题，编者的经验是用GSView的File->PS to EPS和AcrobatRd的Document->Crop Pages完成比较好。这样不会损害矢量图特性，能被各IEEE conference & journal接受。 

**以下几种情况要善于使用"~"的小空格 **
1. 中文与英文之间一般加入小空隙排出来才好看，要加上"~"。 
2. 中文与行内公式之间也要加。
3. 如果"."不表示句子的结束，应加入"空格"，如"Mr. Wang"应输入为"Mr.空格Wang"。 因为当"."表示句号时，TeX会加入一段额外的空隙。
4. TeX遇到一个单词以大写字母结束时会自动认为这并不是句子的结束，故这种情况不必加入空格。但如果确实是句子的结束，就需要在"."前加上"@”，如“I study in SJTU@.”。
5. 对Figure、Equation、Table的引用时,要加"~"，比如
figurename~ref{%label%}
Tab.~ref{%label%}
Eq.~ref{%label%} 

**我的系统crash，论文稿件都丢了，怎么办？ **
丢了的话就没什么特别的好办法了，去找IT guys做disk recovery吧。这很麻烦，所以还是建议预先使用CVS或SVN来做冗余备份和version control。这样boss也方便查看你的进度，一举多得。 
WinCVS的使用参考  [http://203.68.102.46/online_book/content.php?chapter_sn=223](http://203.68.102.46/online_book/content.php?chapter_sn=223)

**如何配置WinEdit的界面Layout? 比如说，达到下面的简洁形式。 **
![](http://fmn022.xnimg.cn/fmn022/blog/20081126/14/51/A226549257482TON.jpg)
Menu的Options->Apperances->Docking可以改变Layout的配置。基本属于BorderLayout。 
要控制各类Viewer窗口的显示与否，对Menu点鼠标右键，就可以看到相关选项。 

**如何压缩论文篇幅？ **主要靠文字的精炼。或者把证明推导放到Appendix (4journal)。
适当缩小图片或者缩小图片与文字的间隔。用命令 
begin{figure}[htb] 
...... 
vspace{-0.3cm} 
end{figure} 

**如何在pdf文档中生成书签？ **
先 
usepackage{lineno} 
usepackage{indentfirst} 
usepackage{hyperref} 
usepackage{natbib} 
再 
section{} subsectioin{}
即可。  

**如何生成hyper reference？ **
usepackage{hyperref} 
hypersetup{ 
    unicode=false,          % non-Latin characters in Acrobat’s bookmarks 
    pdftoolbar=true,        % show Acrobat toolbar? 
    pdfmenubar=true,        % show Acrobat menu? 
    pdffitwindow=true,      % page fit to window when opened 
    pdftitle={My title},    % title 
    pdfauthor={Author},     % author
    pdfsubject={Subject},   % subject of the document 
    pdfcreator={Creator},   % creator of the document 
    pdfproducer={Producer}, % producer of the document
    pdfkeywords={keywords}, % list of keywords 
    pdfnewwindow=true,      % links in new window 
    colorlinks=true,        % false: boxed links; true: colored links 
    linkcolor=red,          % color of internal links 
    citecolor=green,        % color of links to bibliography 
    filecolor=magenta,      % color of file links 
    urlcolor=cyan           % color of external links 
} 
href{...url...}{...caption...} 
具体如何在pdf文档里面加入hyperref, bookmark, thumbnail，参考[http://www.mpch-mainz.mpg.de/~joeckel/pdflatex/](http://www.mpch-mainz.mpg.de/~joeckel/pdflatex/)

**Latex里面有没有比较好看的手写字体？ **
我自己喜欢lucida casual。 
下面是一个gallery： [http://tug.org/store/lucida/complete.html](http://tug.org/store/lucida/complete.html)
这是sample文档： [http://tug.org/store/lucida/lucida-sample.pdf](http://tug.org/store/lucida/lucida-sample.pdf)
这个下载地址： [http://www.rzuser.uni-heidelberg.de/~t91/analysis-cd/software/miktex_2.0/](http://www.rzuser.uni-heidelberg.de/~t91/analysis-cd/software/miktex_2.0/)和 [http://www.ctan.org/tex-archive/fonts/psfonts/bh/lucida/](http://www.ctan.org/tex-archive/fonts/psfonts/bh/lucida/)
个人认为比较好看的适合正文的罗马字体有： Lucida Bright( *The LaTeX Companion *)、Bookman  ( *Effiective STL  *和  *The Pragmatic Programmer *)、 Palatino( *Computer Organization & Design *)等等。 

**我需要某个符号，我需要写某个样子的数学公式，怎么办？ **Latex 里面有一份文档，Higher Mathematics，很适合你。ctex的用户可以在CTeXCTEXdoc下找到ch8.pdf，就是它。如果你想要的符号这里面没有，可以去查一查同一目录下的 symbol.pdf 

**Latex中并列子图的实现 **
下面是个例子，详细说明请参考IEEEtran_HowTo。 
usepackage{graphicx}
usepackage{subfigure} 
%跨越多列的subfigure
begin{figure*}
centering mbox{ 
renewcommand{thesubfigure}{(aarabic{subfigure})} 
subfigure[big]{includegraphics[width=.30textwidth]{yours.eps}}quad
subfigure[small]{includegraphics[width=.20textwidth]{yours.eps}} }
end{figure*} 
%setcounter{subfigure}{0} 
%单列的并列子图  
begin{figure}
centering mbox{ 
renewcommand{thesubfigure}{(barabic{subfigure})} 
subfigure[big]{includegraphics[width=.30textwidth]{yours.eps}}quad
subfigure[small]{includegraphics[width=.20textwidth]{yours.eps}}}
caption{Several figures}
end{figure} 











**Latex 里面如何输入长等号？**defEqlfill@{arrowfill@RelbarRelbarRelbar}
newcommand{extendEql}[1][]{ext@arrow 0099Eqlfill@{#1}}
makeatother
$$a \extendEql{\mbox{\textrm{def}}} b$$

Latex 里面如何调整列表环境的间隔？
设置方法一：在列表环境里进行设置，这样每次使用列表时自己随时设置，比较随意些：
documentclass{article}
usepackage{times}
pagestyle{empty}
setcounter{page}{6}
setlengthtextwidth{159.0pt}
usepackage{pifont}
renewcommandlabelitemi{ding{43}}
begin{document}
  begin{itemize}setlength{itemsep}{-itemsep}
  item Text of the first item in the list.
  item Text of the first sentence in the second item of the list. And the second sentence.
  end{itemize}
  begin{enumerate} setlength{itemsep}{0pt}
        item item of the list.
        item item of the list.
        item item of the list.
end{enumerate}end{document}

设置方法二：方法一有一个缺陷就是你要随时设置比较麻烦，下面的方法较为简单，可以进行统一的设置。简单方便：documentclass{article}
usepackage{times}
pagestyle{empty}
setcounter{page}{6}
setlengthtextwidth{159.0pt} 
usepackage{pifont} 
renewcommandlabelitemi{ding{43}}
usepackage{atbeginend} % 可选宏包, 能解决许多问题,
%比如itemize, enumerate环境item之间的控制
%用法
AfterBegin{itemize}{addtolength{itemsep}{-0.5baselineskip}}
AfterBegin{enumerate}{addtolength{itemsep}{-0.5baselineskip}}
begin{document}
  begin{itemize}
        item item of the list.
        item item of the list.
        item item of the list.
  end{itemize}
  begin{enumerate}
        item item of the list.
        item item of the list.
        item item of the list.
end{enumerate}
end{document}

Latex 里面如何打印如下常用符号？


使用marvosym 宏包
演示：
![marvosym-demo.jpg](https://rnibyq.blu.livefilestore.com/y1pdRV6v6focrCvU8kmecqUzXXGt1do4rCqgEFq4y3zAGujHCI6s04vPpT-5_hrzlofVLLr9vqDN4ZOhPd8E2704Vn0nxD7cfwP/201022716440152.png)
代码：
documentclass[a4paper,twoside]{book}
usepackage{CJK}
usepackage[body={398pt,550pt},footskip=30pt,%
            marginparwidth=60pt,marginparsep=10pt]{geometry}
%setlengthtextwidth{180.0pt}
usepackage{marvosym}
begin{CJK*}{GBK}{song}
begin{document}
            noindentEmailct~XXXXXXX有限公司~~xxxx~xxxx~xxxxxx~xxx-xxx~室\%
            Telefon~underline{0755-12345678}~~\Letter~underline{xxx@xxxxxxxxx.com}~~\%
            Mobilefone~underline{13812345678}~~\Pickup~underline  {http://www.xxxxxxxxx.com/}\%
打勾: ${surd}$\
打叉: ${texttimes}$
paragraph{}
clearpage
end{CJK*}
end{document}

转自[http://babe-hut.spaces.live.com/blog/cns!8C5812EAE39B56CC!200.entry](http://babe-hut.spaces.live.com/blog/cns!8C5812EAE39B56CC!200.entry)



附：


|Latex常见问题集|
|----|
|**如何设置标题和副标题**title{Introduction to LaTeX{ } Symbols and Commands \ [2ex] begin{large} Common Expressions in emph{AURORA} end{large} }通过换行符号\，分开主标题和副标题，然后设置缩进位置2个X的位置，最后用一个局部环境设置副标题的字体大小。There are two basic ways to change font sizes in Latex:- To change the basic font size used all the way through your paper, put either  "11pt" or "12pt" in your documentclass line.  For example, if you had: documentclass{report}  but you wanted to use 12pt type (10pt is the default), you would change it  to: documentclass[12pt]{report}NOTE:  12pt is an option to the "report" class, not a separatepackage, so doing documentclass{report} usepackage{12pt}will *not* work.   - To change just a part of your paper into a different font size, you can use  some of the sizing environments.  In increasing size, they are: tiny scriptsize footnotesize small normalsize large Large LARGE huge Huge  The case is important in these commands.  Also, in some document styles,  some of these commands may produce the same size font.  For example, if you  wanted to just make a small part of your text in a different font, you would  use something like: This is in normal text, while these words are in {large large text}.  Or, if you wanted to put a larger region in a different size, you'd use  something like: begin{small} this will all be in small text this too. etc.. end{small}**Latex中数学常用符号的输入**1、数学符号的重叠显示，用于变量上面斜杠$rlap{$backslash$} a  $$rlap{$setminus$} a  $$diagdown llap{a}   $2、在箭头上方/下方写字$ u(x) overset{text{UMP}}{Longrightarrow} x(p,w) $ \$ u(x) underset{text{UMP}}{Longrightarrow} x(p,w) $ \$Y  xrightarrow[text{ Cost Function }]{text{Cost Minimization}}  c(w,q) $在括号上下方写字，用overbrace or underbrace。    f_{X_1,cdots, X_k}(x_1, cdots, x_k)=       overbrace{int_{-infty}^{infty} cdots int_{-infty}^{infty}}^{n-k}        f(x_1, cdots, x_k, xi_{k+1}, cdots, xi_{n})       d xi_{k+1} cdots dxi_{n}3、公式排列一个短公式写一行， 用equation,(有编号，加*无编号) 一个长公式分几行写，没有对齐功能，用multiline ，(有编号，加*无编号) 一个长公式分几行写，有对齐功能，用split。自身无编号。要编号，外套equation。 一组公式，无对齐功能，用gather。(有编号，加*无编号)  一组公式，有对齐功能，用align。(有编号，加*无编号)       begin{align*}           a+b & = c+d \           x+y & = c+d      end{align*}上述两个公式按照&的位置对齐。如果不加*，则对每个公式进行编号，加了*就不编号。 将一组公式用类似矩阵形式进行排版对齐，用align或 flalign。(有编号，加*无编号) 4、矩阵的输入利用bmatrix环境，带方括号begin{equation} X=begin{bmatrix}  1 & cdots & 2 \  2 & cdots & 3end{bmatrix} end{equation}利用matrix环境，啥括号都不带begin{equation} X=begin{matrix}  1 & cdots & 2 \  2 & cdots & 3end{matrix} end{equation}利用array环境，自己在括号的地方写，可以用任意括号形式begin{align}    E(X)=left[           begin{array}{ccc}             E(x_{11}) & cdots & E(x_{1n}) \             cdots & cdots & cdots \             E(x_{n1}) & cdots & E(x_{nn}) \           end{array}         right]   %如果用“right.”，那么后面的括号就隐藏了，可以用来表示分段函数。  end{align}**常见技巧**1.多个blankspace等于一个blankspace，多个空行(line)等于一个空行，一个空行效果等于另起一段，并不会真正出现一个空行。2.九大保留字符的输入# $ % ^ & _ { }      前面加就可以，如$                    要用命令$\backslash$3.latex命令的书写要求$a\pm b___FCKpd___1nbsp; $apm3b___FCKpd___1nbsp; 命令pm后面的空格、数字或者任何非字母的字符都标志着该命令的结束。如何在命令后产生一个空格：{}可以保护后面的空格。Get LaTeX{} Started.一些命令(footnote 或phantom)内嵌于caption 或section时会失效，这时加上protect可以保证不失效。section{Higer Order Difference Equations footnote{See James(2000)}}  %footnote失效section{Higer Order Difference Equations protectfootnote{See James(2000)}}注意protect只保护紧跟其后的命令本身，命令的参数并不受到保护，如果要保护参数，需要在参数前加protect.其他一些脆弱命令还有：All commands that have an optional argument are fragile. Environments delimited by begin ... end are fragile. Display math environment delimited by [ ... ] Math environment ( ... ) ，However, $ ... $ is robust Line breaks, \ .item commands .footnote commands .4.如何打印命令(将程序命令打印出来而不让命令执行)begin{verbatim} Get LaTeX{}       Started. end{verbatim}verb*|like this :-) |5.添加程序注释的两种方法%Get LaTeX{}              Started.begin{comment} Get LaTeX{}       Started. end{comment}第二个方法要在导言区添加usepackage{verbatim}6.document class有哪些常见类型article, proc, minimal, report, book, slides7.页面式样pagestyle{plain}pagestyle{headings}pagestyle{empty}thispagestyle{empty}  %将当前页面的式样改为empty.8.大型文档中插入一些子文档的方法include{filename}   %想让插入的子文档在新的一页开始显示;input{filename} %插入的子文档直接显示，不用新开一页。includeonly{file1,file2,file3}  %在导言区使用。只有在此列出的子文档才会在正文中被插入。9.快速检查语法usepackage{syntonly}syntaxonly10.分行分页的方法\  or  newline  :另起一行，但不另起一段。\* :另起一行，但禁止分页。newpage:另起一页。11.确定单词断点位置hyphenation{FORTRAN Hy-phen-a-tion}   %只有在-出现的地方才能断，没有-的单词不能断，必须整体出现。un-derstood %在正文中输入understood的时候，插入-告诉latex这里可以分开。12.几个单词整体出现mbox{0116 291 2319}  %让这几位数字作为一个整体出现在一行，不在不同的行显示。fbox{0116 291 2319}  %功能和mbox一样，同时还在这个整体周围加框。13.特殊符号引号``well understood `physical' formula''   双引号要用两个重音号``和两单引号''前后表示。破折号daughter-in-law, X-rated\pages 13--67\yes---or no? \$0$, $1$ and $-1$波浪号~\~understood \~{}physical $\sim$formula   效果各不同温度度数$-30\,^{\circ}\mathrm{C}$$-30$textcelsius  %这一种要加载usepackage{textcomp}欧元符号texteuro 100euro 100 $需加载\usepackage[official]{eurosym}或者\usepackage[gen]{eurosym}\EURtm  $需加载\usepackage{marvosym}省略号直接用...不行，要用 \ldots, 文本中位于下部的省略号$cdots___FCKpd___1nbsp;%math mode, 数学符号中的位于中部的省略号$cdot___FCKpd___1nbsp; %math mode,数学符号中位于中部的一个点号。连字不管你信不信，在latex中，ff和f{}f显示的结果是不同的。ff是两个连在一起的整体符号。14.调整单词间隔frenchspacing15.论文的标题、章节目等安排title{My Dissertation}  %在后面正文中用maketitle生成标题author{George Bush, Allen Greenspan} date{today}tableofcontents %在此位置插入章节目录listoffigures  %在此位置插入图形目录listoftables  %在此位置插入表格目录part{XXX} %产生单独的一页，例如Part I XXXchapter{Introduction to SAS} %章 1 只有report和book才有章的划分。section{My God}  %节 1.1 subsection{...}  %目 1.1.1subsubsection{...}  %以下不再编号paragraph{...}subparagraph{...}section*{My God}  %加了*后，该节就不出现在目录中，也不编号。appendix  %该命令说明后面开始附录，下面的chapter命令采用字母编号chapter{Mathematical analysis}  %Appendix A Mathematical analysissection{Set Theory}   %A.1 Set Theory16.book风格的文档frontmatter %紧跟着begin{document}命令出现，该命令启用罗马数字编页码。mainmatter %出现在第一章前面，该命令启用阿拉伯数字编页码。appendix  %标志书中附录材料的开始。该命令后的各章序号改用字母标记。backmatter  %应该插入书中最后一部分内容的前面，如参考文献和索引17.交叉引用法label{sec:my1} %放在在节、子节、图、表或定理后面，作为后面需要引用的标记ref{sec:my1} %引用标志词sec:my1那里的节、子节、图、表或定理的序号pageref{sec:my1} %引用标志词sec:my1所在的页码18.脚注、页边注和尾注footnote{type the text here.}marginpar{type the text here.}尾注的方法usepackage{endnotes}  %加载尾注宏包endnote{type the text here.} %在需要尾注的地方添加theendnotes   %指明尾注内容在文中该地方出现。19.强调underline{text} %下划线强调emph{text}  %斜体强调20. 三种枚举方法Itemize、Enumerate 和Descriptionflushleftbegin{enumerate}  item The strategy:    begin{itemize} item First,look silly.item[-] Second,be smart.end{itemize}  item The result:    begin{description} item[Stupid] You'll be smart.item[Smart] You'll be stupid.end{description}end{enumerate}21.文本的对齐和居中begin{flushleft}。。。。。。end{flushleft}begin{flushright}。。。。。。end{flushright}begin{center}。。。。。。end{center}22.引文、诗歌、摘要begin{quote}。。。。。。end{quote} %一次少量引用begin{quotation}。。。。。。end{quotation} %一次大量引用begin{verse}。。。。。。end{verse} %引用诗歌，排成韵律风格，每一行后用\断行。begin{abstract}。。。。。。end{abstract} %学术论文的摘要。23.表格begin{tabular}{|r||l @{ + } c@{ . }rrr@{ * }c|c|} hline hlinePosition & Club & Games & W & T & L & Goals & Points\[1.5ex] hline1 & Amesville Rockets & 33 & 19 & 13 & 1 & 66:31 & 51:15 \2 & Borden Comets & 33 & 18 & 9 & 6 & 65:37 & 45:21 \... & ..... & .. & .. & .. & .. & ... & ... \17 & Quincy Giants & 33 & 7 & 5 & 21 & 40:89 & 19:47 \18 & Ralston Regulars & 33 & 3 & 11 & 19 & 37:74 & 17:49\ hline hlineend{tabular}另一种更加复杂的表，可以随意画横线，几列共用一个标签begin{tabular}{|r|l||rrr|r@{ : }l|r@{ : }l||c|} hlinemulticolumn{10}{|c|}{bfseries {itshape Table 1}. 1st Regional Soccer League --- Final Results 2002/03}\ hlineitshape Position & itshape Club & itshape W & itshape T & itshape L & multicolumn{2}{|c|}{itshape Goals} & multicolumn{2}{|c||}{itshape Points} & itshape Remarks\[1ex] hline hline1 & Amesville Rockets & 19 & 13 & 1 & 66&31 & 51&15 & League Champs\ hline2 & Borden Comets & 18 & 9 & 6 & 65&37 & 45&21 & Trophy Winners\ hline... & ..... & .. & .. & .. & .. & ... & ... & ...& ...\ hline17 & Quincy Giants & 7 & 5 & 21 & 40&89 & 19&47 & Candidates \ cline{1-9}18 & Ralston Regulars & 3 & 11 & 19 & 37&74 & 17&49 & for Losers\ hlineend{tabular}24. 浮动体浮动体就是一个整体在正文中上下浮动，寻找合适的放置位置，通常为了避免一个“整体对象”遭遇分页等尴尬问题。例如不想让表格一半在上一页，一半在下一页。将上面表格作为浮动体处理，浮动体放置许可规则[!hbtp]表示忽略内部参数[!]，首先优先放在这儿here[h]，其次是bottom[b]，再次放在top[t]，然后考虑放在一个floating page[p]。下面是表和图的浮动体例子。其中caption命令添加标题，[..]内的是简单标题，在listoffigures 和 listoftables 中出现，{...}内的是长标题，在正文中显示。begin{table}[!hbtp]caption[Finals]{1st Regional Soccer League --- Final Results.}begin{tabular}{|r|l||rrr|r@{ : }l|r@{ : }l||c|} hlinemulticolumn{10}{|c|}{bfseries {itshape Table 1}. 1st Regional Soccer League --- Final Results 2002/03}\ hlineitshape Position & itshape Club & itshape W & itshape T & itshape L & multicolumn{2}{|c|}{itshape Goals} & multicolumn{2}{|c||}{itshape Points} & itshape Remarks\[1ex] hline hline1 & Amesville Rockets & 19 & 13 & 1 & 66&31 & 51&15 & League Champs\ hline2 & Borden Comets & 18 & 9 & 6 & 65&37 & 45&21 & Trophy Winners\ hline... & ..... & .. & .. & .. & .. & ... & ... & ...& ...\ hline17 & Quincy Giants & 7 & 5 & 21 & 40&89 & 19&47 & Candidates \ cline{1-9}18 & Ralston Regulars & 3 & 11 & 19 & 37&74 & 17&49 & for Losers\ hlineend{tabular}end{table}Figure ref{white} is an example of Pop-Art.begin{figure}[!hbp]makebox[textwidth]{framebox[5cm]{rule{0pt}{5cm}}}caption[FiveFig]{Five by Five in Centimetres.label{white}}end{figure} 浮动体的放置顺序遵循“先来先放”的原则。有时等待序列中浮动体太多，命令clearpage可以让等待结束，立刻新开一页放置所有等待中的浮动体。在LaTeX文档中插入图片[http://hepg.sdu.edu.cn/Service/tips/latex/latexfig.html](http://hepg.sdu.edu.cn/Service/tips/latex/latexfig.html)25.如何使用中文字体先在导言区加载usepackage{CJK}然后在正文使用中文的地方用begin{CJK*}{GBK}{song}   中文或者英文  end{CJK*}加*和不加*的区别在于对~的处理不同。加了*后，汉字之间的空格用~代替。不加*，汉字之间空格用英文正常空格。常用的其他字体还有：fs（仿宋）、kai（楷体）、hei（黑体）、li（隶书）、you（幼圆）。命令中的 GBK 是指中文内码，还可以用 GB 和 BIG5（繁体字），前提是相应的字库设置是正确的。如果在中间想要将宋体改变仿宋，就可以使用：CJKfamily{GBK}{fs}26.段首缩进latex默认每一段段首缩进，但是正文第一段不缩进。如果要缩进，采用下面两种方法：(1).加载usepackage{indentfirst}的方法。(2).在该段首用parindent来专门控制缩进，例如：setlength{parindent}{4em} %放置在段首，它设置为当前字号的4个大写字母M的宽度，大约正好是4个汉字的宽度27.行距和段距的控制setlength{baselineskip}{1.8em} %行距控制为1.8个M，直到下次出现修改为止。1.8em是中文常见行距。setlength{parskip}{1ex}  %段距控制为1个x，直到下次出现修改为止。28.页眉页脚usepackage{fancyhdr}pagestyle{fancy}29.页面设置usepackage[top=1in, bottom=1in, left=1in, right=1in]{geometry}30.如何设置字体颜色documentclass[letterpaper,11pt]{report}usepackage{color}definecolor{Blue}{rgb}{0.3,0.3,0.9}begin{document}Hello! {color{Blue}{World}}!end{document}|

**左对齐** begin{flushleft}
 end{flushleft}


**右对齐**
 begin{flushright}
 end{flushright}


**列举环境**
 begin{itemize}
 end{itemize}

 begin{enumerate}
 end{enumerate}
 保持环境
 verb|  |

 begin{verbatim}
 end{verbatim}


 小页环境
 begin{minipage}{size}
 end{minipage}


 诗歌环境
 begin{verse}
 end{verse}

**浮动表格环境**
 begin{table}
 end{table}

 固定表格环境
 begin{tabular}
 end{tabular}



**浮动图片环境**
 begin{figure}
 end{figure}

10种字号命令
 {tiny  }   {scriptsize  }  {footnotesize  }  {small  }  {normalsize  }  {large  }  {Large  }
 {LARGE  }  {huge  }  {Huge  }



英文字体命令
 {rm  } 罗马字体    {bf  } 黑体   {it  } 意大利字体   {sc  } 小号大写字体  {sl  } 斜体
 {tt  } 打字机字体  {cal } 花体  {em  } 强调型字体 



几种数字的格式
 arabic{}   roman{}  Roman{}  alph{}  Alph{} 



章节命令
 部分命令
 part{}



 章命令
 chapter{}


节命令
 section{}
 subsection{}
 subsubsection{}
 subsubsubsection{}


 附录命令
 appendix{}



使用上面所说的命令，处理一般的文字文档应该没多大问题了。但是一篇文章中还有三个很重要的部分：公式，图片，表格现在我们还没法处理。





下面就来讲一讲。

  要在Latex文档中加载图片，必须要包括graphicx宏包。
 下面即是典型代码：

 begin{figure}
 begin{center}
 includegraphics[width=10cm,height=6cm]{1.eps}%就在前面括号中写图片名
 renewcommand{figurename}{图}
 caption{}
 label{}
 end{center}
 end{figure}

 这段代码功能是把名为1.eps的图片调进来，放在浮动图片环境中，并居中放置。
 includegraphics[width=10cm,height=6cm]{1.eps}中的includegraphics 即为调用图片的命令。[]中的参数用于指定加载图片后图片显示的宽度和高度。
 renewcommand{figurename}{图}用于将其默认的标号（为figure）改为“图”
 caption{}给这幅图片取一个名字或作一段说明用于显示出来。
 label{}中也写一个名字用来标明这幅图片，方便前面或后面的交叉引用。







插入表格

 插入表格没有图片步骤多，但也要注意一些细节问题。

 begin{table}
 centering
 renewcommand{tablename}{表}
 caption{}
 begin{tabular}{|l|l|l|}
 hline
  1 & 2 & 3\
 hline
  8.2 & 7.8 & 12.7\
  75.7 & 152.8 & 249.9\
 hline
 end{tabular}
 end{table}

 这段代码用来生成一个3行3列的浮动表格，并左右置中放置。
 begin{tabular}{|l|l|l|}中的|l|l|l|表示有三列，列与列之间用竖线隔开（共四根竖线）。
 hline表示画一根横线。






转载本文请联系原作者获取授权，同时请注明本文来自王伟华科学网博客。
链接地址：[http://blog.sciencenet.cn/blog-81613-322468.html](http://blog.sciencenet.cn/blog-81613-322468.html)

上一篇：[[转载]火车带你游西藏（犹如身临其境）](blog-81613-321101.html)
下一篇：[数码相机摄影技巧基础](blog-81613-324953.html)


