# （LaTex)CTex的初次使用心得及入门教程 - tostq的专栏 - CSDN博客





2015年11月30日 11:52:07[tostq](https://me.csdn.net/tostq)阅读数：118047标签：[CTex																[latex																[文档编辑](https://so.csdn.net/so/search/s.do?q=文档编辑&t=blog)
个人分类：[论文编写](https://blog.csdn.net/tostq/article/category/5991535)





**摘要**

最近要发论文了，被知乎里人推荐使用论文编译软件（CTex、LaTex和Overleaf之类），瞬间感觉自己用Word简直Out了（书读少）。

学校里也听说过LaTex，不过因为当时没怎么写过论文，所以对这么麻烦的软件（什么文字还可以编译！写文章就像编程一样！）要一直谨谢不敏，但是最近使用Word进行格式编辑时，实在是烦晕了，特别是表格设置，还有什么分栏跨栏之类，整得头都大了，于是便决定尝试下LaTex，LaTex贴吧里有下载，不过实在太大了，安装也挺麻烦的，而刚好同学那里有个CTex（功能一样，国内高校好像都是用的这个），就直接拷了过来安装，安装很简单。（CTex和LaTex这类离线版本都挺大的，安装包差不多2个多G，还有一个离线版本Overleaf，也很不错，可是我的网速太慢，就放弃了）

安装之后，可以看到CTex下面有这么多东东：

![](https://img-blog.csdn.net/20151130115614895?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


我们直接点开WinEdt，就可以愉快的编译论文了。

不过之前，我们需要做如下准备：

1、  工作目录新建文件夹，用于存放写论文的图表之类

2、  将要投期刊会议的论文LaTex模板放在新建文件夹中。这里模板都可以到期刊所在网站上下到，如IEEETran.cls（IEEE通过期刊LaTex模板）

3、  如果你是新手，你最好还需要一个例子模板*.tex（上面LaTex模板只是相当于一个库，所以你不需要在上面编辑，而这个例子模板就是可以让你在上面根据你的内容进行修改）。你的论文就是在*.tex下编辑的，有点类似于html语言。因为CTex新建后是空白文件，而Overleaf可以直接根据你选择的模板生成一全套，所以我就从上面下了一个。

4、  将例子模板和论文里的图片都手拷到新建文件夹了

5、  打开WinEdt，在工作文件夹里新建项目

6、  最后我们就可以打开例子模板*.tex进行编译了

**一、编写*.tex文件**

也许打开*.tex文件后，你也许就会年花眼了，不过把其想像为Html语言，就非常好编写了。

![](https://img-blog.csdn.net/20151130115833360?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


*.tex的开头大概是这个样子的。

\documentclass 主要指的是文件类型 并指示样式模板为ieeeconf

\usepackage 包库，如graphics库是导入图片需要的，而cite库是引用需要的

\title 里面包含题名，直接把里面文字修改下就可以了

\author 里面包含作者名字

\begin{document} 文档开始

\begin{abstract}

         里面编写摘要

\end{abstract} 每个begin后面都接了end

\begin{keywords}

            里面写关键词

\end{keywords}

\section{INTRODUCTION} 第一部分 {}里面包含的都是这部分的题名

           内容 \section没有结束标志，里面的段落要有两个回车，那两段中心隔了一行，不隔行会把两段合并在一起

\subsection{小标题名}

子段，  可以有多个段

。。。

。。。

。。。

\section{CONCLUSIONS}总结

\end{document}

通过上面可以看出原来的文字编辑工作只需要在固定位置写入文字就可以了，完全不需要考虑布局了。

**二、导入图片**

导入图片首先要确保已经导入相关库：

\usepackage{graphics} % for pdf, bitmapped graphics files

然后引用时应用如下格式：

\begin{figure}[thpb]  图片导入开始

  \centering 图片居中

  \includegraphics[scale=0.3]{导入图片文件名} 里面的scale是指图像尺度大小

  \caption{例图名}

  \label{例图标号} 可以在正文位置用\ref{例图标示}来引用例图标号（1，2，，）

\end{figure} 图片结束

**三、导入公式**

公式主要可以用两种方式：

一种是文章内部的如下，其在段落中间，需要用$把公式内容包括在一起：$公式$

![](https://img-blog.csdn.net/20151130115814835?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


另一种是在段落中间，一般会有公式标号

![](https://img-blog.csdn.net/20151130115849799?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


其格式如下：

\begin{equation}

公式

\end{equation}

注意：在书写公式时，除了=、-、+之类的符号可以直接输入，其它都有专门的字符表示，如下

![](https://img-blog.csdn.net/20151130115924976?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


不过在CTex里面有一个非常好的公式编辑器，还可以通过手写来获得公式哦

![](https://img-blog.csdn.net/20151130115944681?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


**四、导入表格**

表格模板如下所示

\begin{table}[h] 表格开始，如果在双栏情况下需要跨栏表格，可以将table改为table*

\label{表格标示} 

\caption{表格名}

\begin{center}居中

\begin{tabular}{|c|c|c|c|c|c|}正式开始：这里的|就表格分成多列，共六列，|cc|也指两列，不过两列中间没有竖线，这里的c指列间是居中放置的

        \hline 指一条长横线，贯穿整个表格，这里是指表格的上框

1 &2&3 & 4 &5\\ \cline{2-5} （这里1 2 3 4是表格第一行的内容，而\cline是指此行下面的横线，并不贯穿整个表格，而是从第二个到第五个。而\\指两行的分割）

 &2 &3 & 4 &5\\ \hline （这里1 2 3 4 5是表格第二行的内容，\hline指一条长横线，贯穿整个表格，这里是指表格的下框）

\end{tabular}此表结束

\end{tabular}

\end{center}

\end{table}

以上画的表格是这个形状（上下边框由\hline画出，中间线由\cline，竖线由|确定
|1|2|3|4|5|
|----|----|----|----|----|
|2|3|4|5| |

**五、导入引用**

导入引用两种方式

一种需要一条一条引用

另一种则是通过*.bib引用文件直接导入就可以了，这个*.bib文件可以通过Endnote或Mendeley获得，而可以直接在下载文件时导入，如在IEEE的网站上搜索时，选择bibTex下载。

![](https://img-blog.csdn.net/20151130120056172?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


![](https://img-blog.csdn.net/20151130120113027?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

下载之后可以得到如上文档，将其保存为*.bib文件就可以了

然后将*.bib引用文档放到工作文件夹中，就可以使用了

只需要文档结束位置添加如下格式就可以了

\bibliographystyle{IEEEtran}引用样式

\bibliography{My_Collection}My_Collection是My_Collection.bib的文件名

\end{document} 整个文档结束的标志，不用管

如果不引用时，他不会显示*.bib里的引用。

只有在正文里引用里，才会按引用顺序例出引用文档，这非常方便，我们可以在*.bib文档内放置所有可以用到的文档信息，而在正文引用只需要调用\cite{引用别名}，那么其就会给我们自动例出。

这里的引用别名，可以自己在*.bib文件里设置如：

![](https://img-blog.csdn.net/20151130120204720?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

**六、文档编译**

![](https://img-blog.csdn.net/20151130120240109?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


如上图，主要有三个需要注意。首先是那个L的标示，选择Xelatex，然后直接点击，如果导入了引用文件*.bib而还需要点击B标志，然后就可以点击查看PDF。

这里还需要注意的是，如果是引用编译的时候有问题，还有可能是引用文件没有导入的原因，可以尝试下面的操作：

1、  重启软件

2、  反复来回点XeLaTex和引用编译

3、  引用文件里*.bib里的&、%等字符，需要变成\&之类。

**七、总结**

CTex虽说看起来复杂，但实际上上手并不难，本人不过刚接触CTex才三天，就已经差不多完成了自己的论文。当然CTex远不至这些，上面只是本人根据自己的需要，有针对性的学习了下，如果想系统地入门，可以看下《一份不太简短的
**LATEX 2*ε ***介绍》。当然最好的学习方式不是看，而是要自己去实际操作。最后因为本人只不过是个新手，如果上面有什么问题，希望大家能指导指导>o<](https://so.csdn.net/so/search/s.do?q=latex&t=blog)](https://so.csdn.net/so/search/s.do?q=CTex&t=blog)




