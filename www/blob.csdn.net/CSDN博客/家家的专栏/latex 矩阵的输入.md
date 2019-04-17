# latex 矩阵的输入 - 家家的专栏 - CSDN博客





2012年02月27日 21:00:01[依海之燕](https://me.csdn.net/yihaizhiyan)阅读数：6383








![LaTeX技巧198：一个省略号较多的矩阵如何输入？](http://s15.sinaimg.cn/middle/5e16f17747c91cd1e6ace&690)


如上矩阵在平时输入时较为常见，输入方法如下：

演示代码：
\documentclass{article}
\setcounter{page}{6}
\setlength\textwidth{207.0pt}

\usepackage{amsmath}
\setcounter{MaxMatrixCols}{20}

\begin{document}
\[
 \begin{Vmatrix}
  \,a&b&c&d&e&f&g&h&i&j &\cdots\,{} \\
     &a&b&c&d&e&f&g&h&i &\cdots\,{} \\
     & &a&b&c&d&e&f&g&h &\cdots\,{} \\
     & & &a&b&c&d&e&f&g &\cdots\,{} \\
     & & & &\ddots&\ddots&\hdotsfor[2]{5}\,{}
 \end{Vmatrix} \]
\end{document}

**转自：[http://blog.sina.com.cn/s/blog_5e16f1770100grna.html](http://blog.sina.com.cn/s/blog_5e16f1770100grna.html)**






