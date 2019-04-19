# LaTex 基本使用（一） - 刘炫320的博客 - CSDN博客
2019年04月07日 16:48:15[刘炫320](https://me.csdn.net/qq_35082030)阅读数：17
### 文章目录
- [1. LaTex 项目组成](#1_LaTex__1)
- [2. XX.bib文件与XX.bst文件](#2_XXbibXXbst_11)
- [3. XX.sty文件与XX.cls文件](#3_XXstyXXcls_19)
- [4. XXX.tex文件](#4_XXXtex_30)
- [4.1 关键字语法](#41__34)
- [4.2文章结构](#42_42)
- [4.3 常用命令](#43__53)
- [4.4 列表](#44__61)
- [4.5 表格](#45__80)
- [4.6 引用与脚注](#46__165)
- [5. 补充](#5__182)
- [5.1 公式](#51__184)
- [5.2 插入图表](#52__192)
# 1. LaTex 项目组成
在学术论文撰写过程中，LaTex作为标准化论文的工具，一直被广为流传，更是传言，不会LaTex的，就写不出好文章（这话有点夸大了）。为此，我们还是需要学习如何使用LaTex。
本章，我们以简单的LaTex使用方式[Overleaf](https://www.overleaf.com)，以ACL 会议论文为例，进行LaTex使用方法介绍和说明。
首先，找到Overleaf的网站后，可以看到有很多会议和期刊的LaTex文件夹已经为我们准备好了，因此我们不再需要费尽心思去寻找模板了，这使得我们可以只关注与论文的内容，而不用去管论文的格式了。
当我们打开ACL 2019的论文模板后，我们的界面如图所示，左边是项目的目录，中间是我们的源文件，右边是PDF预览版。这里我们一一介绍，首先了解LaTex的组成结构，即左边的部分，左边部分包含整个LaTex项目的文件组成。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190407151917318.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzM1MDgyMDMw,size_16,color_FFFFFF,t_70)
# 2. XX.bib文件与XX.bst文件
第一个以bib结尾的文件为包含参考文献的文件，这里列举了我们所有论文中使用的参考文献，其格式均为bib的格式，这种格式可以使用谷歌学术中引用里使用BibTex导出即可。
在正文中，只需要在适当的位置写入以下语句即可，这里需要提一点的是，不同的论文对其参考文献的格式要求不同，一般会给出一个bst后缀的文件，这个文件是规定了参考文献的格式与排版，一般不需要我们手动修改，[如果需要修改的话](https://blog.csdn.net/cremesino/article/details/6122485)，我们后期会出进阶教程。
```
\bibliography{acl2019}  %bib文件名
\bibliographystyle{acl_natbib} %参考文献的格式的文件名
```
# 3. XX.sty文件与XX.cls文件
在本例中，第二个是PDF文件，这个不用说了，就是我们的输出文件，第三个文件为sty文件，这个文件表明的是LaTex文件中所使用的各种样式，这就像是在网页中的css文件一样。一般的，我们多半使用sty文件，也有使用cls文件，这个我们再以后的教程中详细介绍，本文主要简单介绍一下其内容，使得更容易上手。一般来说，[.cls 和 .sty 文件](https://www.jianshu.com/p/12b4a4b3afce)都是增加 LaTeX 功能的补足文件。它们在我们排版文章是时对应的使用 \documentclass{} 和 \usepackage{} 加载。
在主文件中，只需要在开头引入相关的样式文件即可，如下代码所示：
```
\documentclass[11pt,a4paper]{article} %这是引入一个cls文件
\usepackage[hyperref]{acl2019} %这是引入一个sty文件
\usepackage{times}
\usepackage{latexsym}
```
# 4. XXX.tex文件
第四个文件就是主文件，也就是编写我们内容的文件。我们可以看到，它其实是一个增强版的标记语言文件，既包含了格式化样式，也包含部分逻辑语句，就像是带了脚本的网页一样。
的确如此，在该文件中，是我们所写的主要内容，除了正常的内容表达外，他有一些语法如下所示：
## 4.1 关键字语法
- 注释的符号为`%`
- 关键字以`\`开头
- 关键字分为单标签和双标签两种
- 单标签格式为`\关键字{目标内容}`，如`\title{Test for ACL 2019}`，其标题名为"Test for ACL 2019"，特别的如使得字体缩小，则为`{\small目标文字}`。
- 双标签格式开始为`\begin{对象名}`，结束为`\end{对象名}`，如`\begin{abstract}`和`\end{abstract}`，该标签对中间的内容即为摘要。
- 还有一些特殊的关键字，如`\newcommand`表示[自定义命令](https://blog.csdn.net/sinat_38816924/article/details/84349748)，有点像宏定义一样；`\maketitle`则是[划开标题和正文](https://www.jianshu.com/p/84fc15282cbf)等，这些我们都在以后的教程中详细介绍。
- 还有一些[其他语法](https://blog.csdn.net/liyuming0000/article/details/51787570)。
## 4.2文章结构
好了，现在进入正文阶段，在正文中，最重要的是篇章的结构，首先看到的是文章层次，其定义方式如下，重启一段也可以直接空一行即可：
```
\part{part标题}
\section{section标题}
\subsection{subsection标题}
\subsubsection{subsubsection标题}
\paragraph{paragraph标题}
\subparagraph{subparagraph标题}
```
## 4.3 常用命令
接下来是几个常用的命令.
- 斜体`\emph{International Joint Conference on Artificial Intelligence}`
- 加粗`\textbf{The proceedings are designed for printing on A4paper.}`为加粗
- 交叉引用`~\ref{ssec:first}`为引用当前文件的位置，其对应位置上的代码为`\label{ssec:first}`，需紧跟section或者subsection。
- 不需转义`\verb|\aclfinalcopy|`表示使用原文\aclfinalcopy，不需要转义
- 顶格开启新段落`\noindent`
## 4.4 列表
- 有序列表
```
\begin{enumerate}[(1)] 
\item 有编号的列表 
\item ... 
\end{enumerate}
```
- 无序列表
```
\begin{itemize}
\item Left and right margins: 2.5 cm
\item Top margin: 2.5 cm
\item Bottom margin: 2.5 cm
\item Column width: 7.7 cm
\item Column height: 24.7 cm
\item Gap between columns: 0.6 cm
\end{itemize}
```
## 4.5 表格
- 普通表格
```
\begin{table}[t!]
\begin{center} %表格整体居中
\begin{tabular}{|l|rl|} %各列对齐方式
\hline %直线
\textbf{Type of Text} & \textbf{Font Size} & \textbf{Style} \\  %设置字体
\hline
paper title & 15 pt & bold \\ %各列以&分割
author names & 12 pt & bold \\
author affiliation & 12 pt & \\
the word ``Abstract'' & 12 pt & bold \\
section titles & 12 pt & bold \\
subsection titles & 11 pt & bold \\
document text & 11 pt  &\\
captions & 10 pt & \\
abstract text & 10 pt & \\
bibliography & 10 pt & \\
footnotes & 9 pt & \\
\hline
\end{tabular}
\end{center}
\caption{\label{font-table} Font guide. } %文章标题
\end{table}
```
- 双列表格
```
\begin{table}
\centering
\small
\begin{tabular}{cc} %一个c表示一列
\begin{tabular}{|l|l|} %|表示是否需要竖线
\hline
\textbf{Command} & \textbf{Output}\\\hline
\verb|{\"a}| & {\"a} \\
\verb|{\^e}| & {\^e} \\
\verb|{\`i}| & {\`i} \\ 
\verb|{\.I}| & {\.I} \\ 
\verb|{\o}| & {\o} \\
\verb|{\'u}| & {\'u}  \\ 
\verb|{\aa}| & {\aa}  \\\hline
\end{tabular} & 
\begin{tabular}{|l|l|} %第二个表格
\hline
\textbf{Command} & \textbf{Output}\\\hline
\verb|{\c c}| & {\c c} \\ 
\verb|{\u g}| & {\u g} \\ 
\verb|{\l}| & {\l} \\ 
\verb|{\~n}| & {\~n} \\ 
\verb|{\H o}| & {\H o} \\ 
\verb|{\v r}| & {\v r} \\ 
\verb|{\ss}| & {\ss} \\\hline
\end{tabular}
\end{tabular}
\caption{Example commands for accented characters, to be used in, \emph{e.g.}, \BibTeX\ names.}\label{tab:accents}
\end{table}
```
- 跨列表格
```
\begin{table*}[t!] %这里的星表示当为2列的文章时，此表格为1列表格。
\centering
\begin{tabular}{lll}
  output & natbib & previous ACL style files\\
  \hline
  \citep{Gusfield:97} & \verb|\citep| & \verb|\cite| \\
  \citet{Gusfield:97} & \verb|\citet| & \verb|\newcite| \\
  \citeyearpar{Gusfield:97} & \verb|\citeyearpar| & \verb|\shortcite| \\
\end{tabular}
\caption{Citation commands supported by the style file.
  The citation style is based on the natbib package and
  supports all natbib citation commands.
  It also supports commands defined in previous ACL style files
  for compatibility.
  }
\end{table*}
```
这里t表示表格的位置，具体的有以下四种选项：
- **h** Here - at the position in the text where the table environment appears.
- **t** Top - at the top of a text page.
- **b** Bottom - at the bottom of a text page.
- **p** Page of floats - on a separate float page, which is a page containing no text, only floats.
更多[表格注意事项](https://blog.csdn.net/juechenyi/article/details/77116011)可以参见连接。
## 4.6 引用与脚注
从上表中，可以看到5种引用格式，分别是：
```
\citep{Gusfield:97}  %(人名，时间)
\citet{Gusfield:97}  %人名（时间）
\citeyearpar{Gusfield:97} %（时间）
~\cite{Gusfield:97} %(人名，时间)
~\cite{Gusfield:97,Aho:72} %(人名，时间；人名，时间)
```
这里引用参考文献时，其别名只需要在参考文献的第一个部分加上即可。
- 
引用表格或图的形式`(see Table~\ref{font-table})`
- 
脚注`\footnote{This is how a footnote should appear.}`
至此，我们把整个ACL2019的模板自上而下梳理了一番，从而可以更好的使用这个模板了。至于更加深层的教程，我们会后续解答。
# 5. 补充
有两点在acl 模板中不存在的，第一个为有序的公式，第二个为外部图表。
## 5.1 公式
公式就是LaTex公式，与Makedown的语法保持一致.有序公式如下所示，无序公式使用`$$$$`符号或者`\begin{equation*}`即可。
```
\begin{equation}
1+2+3+\dots+(n-1)+n = \frac{n(n+1)}{2}
\end{equation}
```
## 5.2 插入图表
```
\begin{figure}[h]
\begin{center}
\includegraphics[scale=0.8]{图片名字}
\end{center}
\caption{外部图片标题}
\label{fig:test_demo}
\end{figure}
```
在此之前需要引入包`\usepackage{graphics}`
只需要按照上述步骤，我们就可以构建完整的论文了。
