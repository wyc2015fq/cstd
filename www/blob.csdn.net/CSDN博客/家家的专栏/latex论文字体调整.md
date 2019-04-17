# latex论文字体调整 - 家家的专栏 - CSDN博客





2011年12月01日 09:19:12[依海之燕](https://me.csdn.net/yihaizhiyan)阅读数：9417







**latex**论文**字体**调整




**latex**默认**字体**为10pt,而国内论文正文**字体**一般为小四，这个简单，指定**字体**为12pt就搞定了，可还有问


题，国内论文常用字号为三号、小三，四号、小四，而对应的point为16pt、15pt、14pt、12pt，当指定了正


文文本为12pt后，所能使用的相对大小为：\tiny-6pt、\scritpsize-8pt、\footnotesize-10pt、**\small**-11pt、


\normalsize-12pt、\large-14pt、\Large-17pt、\LARGE-20、\huge-25pt、\Huge-25pt。


而我们论文中能用到的只有\normalsize和\large了，这个时候要想使用三号和小三就得自己定义了，有两种方


法，可按各人爱好使用：


1.就地取材：


重定义相对**字体**大小，使他成为我们常用的字号，而我们缺的只有三号和小三，也就是16pt和15pt：


\renewcommand{\Large}{\fontsize{15pt}{\baselineskip}\selectfont}
\renewcommand{\LARGE}{\fontsize}{16pt}{\baselineskip}\selectfont}


这样我们就可以直接使用\Large和\LARGE作为小三和三号使用了。


2.闭门造车


\newcommand{\sihao}{\fontsize{14pt}{\baselineskip}{\selectfont}

\newcommand{\xiaosan}{\fontsize{15pt}{\baselineskip}{\selectfont}

\newcommand{\sanhao}{\fontsize{16pt}{\baselineskip}{\selectfont}


这样就可以用\sihao,\xiaosan,\sanhao来指定各字号。





