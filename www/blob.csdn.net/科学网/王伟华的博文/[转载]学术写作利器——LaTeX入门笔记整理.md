# 科学网—[转载]学术写作利器——LaTeX入门笔记整理 - 王伟华的博文




# [转载]学术写作利器——LaTeX入门笔记整理                           

已有 2824 次阅读2014-10-3 17:23|个人分类:[科技论文写作、搜索经验集锦](http://blog.sciencenet.cn/home.php?mod=space&uid=81613&do=blog&classid=104604&view=me)|系统分类:[科研笔记](http://blog.sciencenet.cn/home.php?mod=space&do=blog&view=all&uid=81613&catid=1)|文章来源:转载



LaTeX入门教程（上）

比Word更适合学术写作的编纂语言

Credit：讲座是敝系的几个技术宅办的，作者只负责记录和整理。后面的更新是个人心得。
写这篇日志的初衷是小圈子分享和个人心得记录，适合完全不了解tex的入门者阅读，我的水平有限所以肯定有所疏漏，欢迎提出建议。具体使用什么写作工具，还要看写作内容和个人习惯，毕竟优化工作效率才是最终目标 :) 有兴趣的可以翻翻推荐语，有不少高人给出了相关资源和意见。

LaTeX是专为学术写作开发的语言和编纂程序，拥有强大的package资源，这意味着用LaTeX写作可以避免Word带来的令人头疼的排版问题，而且世界上有很多人已经写好了针对各种写作格式的coding package，我们只需要套用这些package就行了。很多主流学术期刊都有自己的LaTeX模板，所以要把同一篇文章投给不同期刊的话，只要套用相应的package就可以了，比起在word里面一点点调整页边距行距引用格式之类的细节，LaTeX的效率显然要高得多。

第一眼看上去，LaTex的语法也许有些吓人，但是只要你熟悉了基本的编纂模式，就会发现它的强大之处。 


第一部分：新手资源
线上编纂
LaTeX可以进行线上编纂，好处是能够随时自动保存工作进度和数据，而且可以多人合作编写文档。

常用的LaTex online compiler基本就是sharelatex.com和writelatex.com这两个，需要注册，基本账户免费。


本地编纂
如果希望用LaTeX Compiler在本地电脑工作，则需要以下软件：

For Windows:

MikTex: [http://miktex.org/download](http://miktex.org/download)

和TexMaker: [http://www.xm1math.net/texmaker/download.html](http://www.xm1math.net/texmaker/download.html)

For Mac:

MacTeX: [http://www.tug.org/mactex/index.html](http://www.tug.org/mactex/index.html)

和TexMaker，地址同Windows.

之所以都需要两个软件是因为它们各自的作用不同：MikTex是package管理软件，而TexMaker是用这些package和你自己的文字来生成最终PDF文件的软件。package的作用会在下面专门解释。

下面以sharelatex.com的界面为例子演示LaTeX的基本编纂规则和package的作用。


第二部分：文档编写基础
基本界面
打开sharelatex.com注册账号之后，点击右上角的New Project，在下拉菜单中选择example project，随便起个名字，出现这个Metro风格的界面。 


|![ShareLaTeX基本界面](http://img5.douban.com/view/note/large/public/p8089977.jpg)|
|----|
|ShareLaTeX基本界面|


如果图太小看不清请戳[http://www.douban.com/photos/photo/1884275523/large](http://www.douban.com/photos/photo/1884275523/large)。从左到右的三竖栏分别是文件相关信息、编码栏和文件生成栏，暂时叫它们ABC栏，其中B栏是进行文档编写的主要工作区。B栏中的改动在点击C栏的Recompile之后才能看到生成文件的相应改动。

B栏中蓝色的字符是function，function后面{}的里面是argument。LaTeX的function使用“”符号表示开头，“%”表示comment out即不执行此行代码。有编程知识的同学都知道这些语句的意义是把特定的argument插入到function表达式中，LaTeX也不例外。

基本function解释

documentclass{article}的意思是这篇文档的类型是学术文章，除此之外也有book，booklet等其他出版物类型，这一句限定了排版能够使用的格式，比如说如果类型是article，那么排版就可以用APA/Chicago/MLA等版式，而不是专用于booklet的一些版式；

userpackage这一行不用改动，只是定义文档的编码方式；

title是整个文档的标题，会自动居中，可以试着把例子中的42改成其他标题，再点击Recompile，可以看到生成了带新标题的文件。

再往下，author和date是文档的作者和日期信息，如果想让date自动显示今天的日期，就把这一行改为date{today}。

下面两行usepackage规定了文档使用的文字排版是natbib，图表排版是graphicx。natbib在理科写作中比较常用，如果要换成文科生常用格式比如APA的话，把上面一行改成：usepackage{apacite}。

这里就要讲到package的作用了。package类似于一个插件，能够把各种事先设定好的标准排版方式、图表排列方式、公式生成方式等等直接插入文档中，免去了手动调整这些参数的麻烦。很多常用的package都有高人写好的代码，我们只要拿过来用就行了，比用word调整页边距、页码等等要方便得多。apacite就算是一个广泛使用的package，我们学校也有专门的机构负责编写论文排版的package，我们只要在usepackage{}中把这些package作为argument插入{}就可以了。

如果需要的话，我们可以在下一行加一个function:tableofcontents，然后点击recompile，可以看到latex会自动在标题后面插入目录，而且目录内容会根据后面的正文小标题改变。还可以在tableofcontents后面加上另一个function：pagebreak，这样文档的正文就会另起一页。pagebreak类似于word里面的分页符，在任何需要分页的地方都可以使用。

begin{document}是正文正式的开始标志，相应的结束标志end{document}则在文档最后。end{document}后面的任何内容都不会在compile里面显示出来，所以如果我们需要只把编码的一部分生成文档，只要在这部分的结束处输入end{document}就行了。begin和end的命令也适用于标记文中插入的图表的起始和结束。

很多时候我们还需要在文章前面加上abstract，方法也很简单，只要像下面这样进行编码：
begin{document}
maketitle
begin{abstract}
（这里放置abstract的文字）
end{abstract}
就可以了。
注意maketitle这一行一定要在begin{document}的下面，否则LaTeX会判定为语法错误。maketitle命令的作用是把标题信息（也就是begin{document}之前的title，author，date等）显示出来，如果comment out或者删去maketitle这一行，这些标题信息将不会显示在C栏的PDF中。当然，如果你正好不需要显示标题，就可以删去这个命令。

section{introduction}是对文档部分的划分，括号里是这部分的名称，latex会生成具体的部分数字编号。子分层的命令是subsection。一般来说latex不允许过多子分层，比如说在usepackage{apacite}规则中，只允许两个子分层也就是subsubsection{}。

了解了以上基础知识之后，就可以进行最基本的LaTeX文档编写了。下面几部分要讲的是如何插入引用和生成参考书目。 

第三部分：引用
Google Scholar和LaTeX的协作
打开Google Scholar首页，点击右上角设置，将“参考书目管理软件”设置为“显示导入BibTex的链接”。然后随便搜索一篇文章，点击搜索结果下面的“导入BibTex”，可以看到几行代码，这就是LaTeX能够识别的bibliography。



|![设置导入BibTeX之后的Google Scholar条目](http://img5.douban.com/view/note/large/public/p8089979.jpg)|
|----|
|设置导入BibTeX之后的Google Scholar条目|




|![点击“导入BibTex”之后的代码](http://img3.douban.com/view/note/large/public/p8089981.jpg)|
|----|
|点击“导入BibTex”之后的代码|


把这几行代码复制粘贴到sharelatex界面A栏的reference.bib中:



|![讲代码复制到reference.bib里面](http://img3.douban.com/view/note/large/public/p8089983.jpg)|
|----|
|讲代码复制到reference.bib里面|



但是如果这时候点recompile，会看到生成的reference里并没有新加入的这篇文章，因为我们在正文中还没有引用它。

于是接下来讲一下如何在正文中插入引用。


在正文中插入引用
复制刚才粘贴的代码的第一行中划红线的内容（也就是BibTeX自动生成的引用文章代码），然后在正文中需要插入引用的部分插入citeA{文章代码}，这样就能在正文中引用了。


|![正文中引用，注意只复制文章代码](http://img3.douban.com/view/note/large/public/p8089984.jpg)|
|----|
|正文中引用，注意只复制文章代码|


这时候再点recompile，才能看到reference中出现了新加的这篇文章。注意：citeA是APA格式的引用function，示例文档代码里面的citep是另一种排版格式的引用function。

在reference中，latex会自动调整文章标题的大小写为只大写首单词的开头字母。如果需要保留文章原标题的大小写格式，就在reference.bib里面相应文章标题title的两边再加上一个括号{}，比如title={{A bird’s-eye view of learner CORPUS research}}，就可以保留原标题的大小写了。


第四部分：LaTeX维基百科全书
上面也提到了LaTeX的function中有很多不同的package供选择，那么到哪里去找不同类型的package呢？比如说我这篇文章用了apacite，那如果我下一篇文章要用MLA格式，该用什么package？

LaTeX的维基页面[http://en.wikibooks.org/wiki/LaTeX](http://en.wikibooks.org/wiki/LaTeX)就提供了这样一个百科全书式的资源，也有PDF版可以下载。

页面中详细列出了各部分function所能够使用的package，只要点开目录链接去找到package名称就行了。对于本地编纂软件来说，则需要下载package之后用MikTex来进行管理。

（今天的讲座只讲到了这里，下周继续写）

========2013. 04.05 期末赶论文期间更新的分割线=========

自从写了LaTeX教程笔记之后，我试着用ShareLatex代替word完成了三个written assignment和两个presentation，写作体验和效果都还不错。所以像我一样的文科僧大可不必对写代码这件事感到头疼，比起HTML/CSS，LaTeX还算是对新手比较友好的，毕竟很多常用的格式和排版都有写好的LaTeX package。

下面就以我自己已经完成的一个assignment和一个presentation作为例子来讲LaTeX的具体应用和注意事项。主要涉及的document type有article和beamer两种，主要的功能有1）用qtree画树形结构，2）用tabular做表格，3）调整行间距/字体字号/页边距等，4）beamer（幻灯片）的一些简单排版。

********实例一：用Beamer做presentation*******

LaTeX也可以做幻灯片，而且直接保存为pdf格式，不用担心pptx在不同的电脑上渲染效果不同了。网上有一个流传较广的Beamer Quick Start：[http://www.math.umbc.edu/~rouben/beamer/](http://www.math.umbc.edu/~rouben/beamer/)，我的slide基本上也是照着这个教程做出来的，下文简称为BQS。另一个pdf格式的教程[http://www.uncg.edu/cmp/reu/presentations/Charles%20Batts%20-%20Beamer%20Tutorial.pdf](http://www.uncg.edu/cmp/reu/presentations/Charles%20Batts%20-%20Beamer%20Tutorial.pdf) 也比较实用。

【PREAMBLE】
首先在preamble（就是begin{document}前面的那些部分）中定义：documentclass[xcolor=dvipsnames]{beamer}
其中方括号[]里的指令是要求beamer使用xcolor来生成dvipsnames指定的色彩效果。LaTeX的很多指令都是这个[]{}的格式，大多数遵循【[ ]定义具体参数 - { }定义上层argument】的规则，所以这一行的意思是“建立一个颜色可由dvip指定的beamer文件”。（beamer自带的default）

然后下面这几行：
usecolortheme[named=Maroon]{structure} 
usetheme{Boadilla} 
usepackage{helvet}
usecolortheme就是来指定beamer的颜色是Maroon，{structure}的意义我也不清楚，大概是只在heading和title这些构建structure的文字上菜使用自定义颜色的意思。所以可以看到，生成的slide中只有标题和页脚等部分才是红色的。
usetheme是指定beamer的主题，类似ppt中的设计模板。BQS中给出了好几个常用的theme。需要注意的是有一些不常用的theme是不会直接被sharelatex识别的，必须和开发者设计的其他的指令配合使用。不过类似Boadilla，Warsaw，Singapore和BQS里面给的这些theme都能被sharelatex识别，而且足够应付一般的presentation了。
usepackage{helvet}用来指定beamer使用的字体。beamer的默认字体是略带圆角的某个字体，如果不喜欢的话就在preamble中另外设置，几个比较常用的helvet (Helvetica)，times (Times New Roman)，bookman (Bookman Old Style)，serif等等都是不错的选择。字体对应的名称见：[http://www.uncg.edu/cmp/reu/presentations/Charles%20Batts%20-%20Beamer%20Tutorial.pdf](http://www.uncg.edu/cmp/reu/presentations/Charles%20Batts%20-%20Beamer%20Tutorial.pdf) 中的第48页。

【标题和页眉设置】
title[Short title of the presentation]{Full title of the presentation}
标题：方括号是缩写，会显示在每一张slide的页脚；大括号里是完整的标题。
subtitle{subtitle here}
副标题。
author[John Smith]{John Smith}
演讲者姓名。方括号里的名字会显示在页脚中。
institute[XX University]{M.A. in XXX\
XXX University\[1ex]
  texttt{example@XXX.edu}}
演讲者的affiliation。方括号还是页脚内容。大括号中的\用来分行，[1ex]用来减小email的字号。
date[March 2013]{today}
演讲日期。估计都已经摸着规律了，beamer中title这几行，方括号里都是页脚的内容。

【插入新幻灯片】
begin{document}之后就是幻灯片正文了。每一张幻灯片的起始和结束用begin{frame}和end{frame}来标示。这两个tag之间的代码就是这张幻灯片上的内容。begin{frame}{slide title}则可以把title直接插入页眉，更符合实际需要；但是相应的end{frame}后面不需要加上{slide title}。

【插入Bulletin point】
begin{itemize}
item （空一格后输入每个point的内容） 
item （同上）
item
...
end{itemize}

【插入数字列表】
begin{enumerate}
item
item
item
...
end{enumerate}
（学过HTML的同学可以把itemize和enumerate理解为ordered list和unordered list）

【分行】
在两行文字中间插入par。也可以用一行空白来代替。
需要较大行间距，则可在两行之间插入：
par
bigskip

【插入图片】
先把要插入的图片用A栏的upload file导入。如果compiler是pdfLaTeX的话，常用图片格式比如JPG/PNG/BMP都兼容，如果compiler是LaTeX则只有EPS才能插入。compiler可在setting中设置。
导入之后，输入插入图片的指令includegraphics[scale=X]{X.PNG}，如下图所示，左边为代码，右边为对应的slide：


|![用.includegraphics插入图片（红色划线部分）](http://img5.douban.com/view/note/large/public/p8342548.jpg)|
|----|
|用includegraphics插入图片（红色划线部分）|


scale就是图片的缩放，X一般是个大于0小于1的数字，比如0.6就是原图片缩放百分之六十。{ }里面是图片文件名。【注意：图片文件名中不能有空格，否则latex无法识别】

如果需要让图片居中，则需要centering这个命令：
...
par
centering
includegraphics[] {}
par
...
经个人实验得出结论：par这个空行命令在这里是必不可少的，因为如果不用par把居中的图片和上下文分隔开的话，上下文靠近图片的部分也会居中。

插入图片的方法是把图片作为Figure插入，比如：（建立一个example file也能看到类似的例子）
begin{figure}[h!]
centering
includegraphics[scale=1.7]{universe.jpg}
caption{The Universe}
label{threadsVsSync}
end{figure}
这样还能给图片加上caption，功能更完善一些。

【控制内容显示先后顺序】
LaTeX也可以实现powerpoint那种逐一显示每一项的效果。还是刚才那张图：


|![用.pause控制内容显示顺序（蓝色划线）](http://img3.douban.com/view/note/large/public/p8342550.jpg)|
|----|
|用pause控制内容显示顺序（蓝色划线）|


加入pause这个命令后，下面新加入的内容会在一张新的slide上显示，播放的时候，配合讲解，可以营造一种“揭晓谜底”“步步推进”的效果。

【插入简单表格】


|![插入简单表格](http://img3.douban.com/view/note/large/public/p8342551.jpg)|
|----|
|插入简单表格|

图片中代码太小看不清楚所以直接复制在这里：
begin{frame}{Quantifier and Scope}
The scope of quantifier is directly reflected by the surface structure;
par
bigskip
begin{center}
begin{tabular}{|c|c|c|c|} hline
textsc{Obj} position & Scrambling & Trace & Ambiguity \ hline
Spec,TP & A-scrambling   & non-recoverable & unambiguous\ hline
Spec,HP & A'-scrambling  & recoverable & ambiguous \ hline
end{tabular}
end{center}
(cf.23 and 24)
par
bigskip
Disambiguation can be reached by inserting a higher adverb such as 'fortunately'.
end{frame}
关键就是从begin{center}到end{center}之间的这一段代码。
{center}是表格居中的命令；
begin{tabular}{|c|c|c|c|} hline这一行建立表格的结构。其中第二个{ }里面有多少个用“|” 分开的格子，表格就有多少个column。可以把“|”理解为vertical line，很形象。每个格子里面能出现的字幕只有r/c/l三种，分别代表左对齐、居中、右对齐，所以这个表格所有的单元格都是居中对齐的。hline是加入一个horizontal line也就是分行的命令。
再往下是表格的具体内容：
Spec,TP & A-scrambling   & non-recoverable & unambiguous\ hline
因为设置了表格有四个column，所以对应的每一个row里面就要用三个“&”把四个column分隔开。“&”相当于“|”的作用。每一个row的结束由“空格 + \”标示。最后的hline用来给这一横行下面加上边栏。如果没有hline，就没有这一行下面的边栏。
以此类推，最后以
end{tabular}
end{center}
结束这个表格。




本文转自[http://www.douban.com/note/264288981/](http://www.douban.com/note/264288981/)
[IPE: LaTeX 文档的画图工具](http://www.latexstudio.net/archives/3797)
[http://www.latexstudio.net/archives/3797](http://www.latexstudio.net/archives/3797)


IPE(Integrated Picture Environment)就是，而且支持 Windows, Linux, MAC OS X，用下来感觉上手特别快，操作起来像 windows 画图板一样的傻瓜。



转载本文请联系原作者获取授权，同时请注明本文来自王伟华科学网博客。
链接地址：[http://blog.sciencenet.cn/blog-81613-832825.html](http://blog.sciencenet.cn/blog-81613-832825.html)

上一篇：[数据挖掘中所需的概率论与数理统计知识](blog-81613-832402.html)
下一篇：[如何选择数学方向](blog-81613-833616.html)


