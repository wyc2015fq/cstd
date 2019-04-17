# LaTex表格内单元格内容强制换行 - 家家的专栏 - CSDN博客





2015年09月08日 16:38:09[依海之燕](https://me.csdn.net/yihaizhiyan)阅读数：4849








﻿﻿

/newcommand{/tabincell}[2]{/begin{tabular}{@{}#1@{}}#2/end{tabular}}%放在导言区

%然后使用&/tabincell{c}{}&就可以在表格中自动换行

%比如这么用

/begin{tabular}{|c|c|}

/hline

1 & the first line //

/hline

2 & /tabincell{c}{haha// heihei//zeze} //

/hline

/end{tabular}

以下为一例子，可直接存为.tex文件编译运行:

\documentclass[a4paper,12pt]{article}

\begin{document}

\begin{table}

\newcommand{\tabincell}[2]{\begin{tabular}{@{}#1@{}}#2\end{tabular}}

  \centering

  \begin{tabular}{|c|c|c|}\hline

1 & \tabincell{c}{the first line \\ the next\\the next\\ last} & \tabincell{c}{one \\ one}\\\hline

2 & \tabincell{c}{hello\\ aha\\ ok \\yes\\en} & \tabincell{c}{two \\ two \\ two}
\\\hline

\end{tabular}

  \caption{longtitle}

\end{table}

\end{document}




转自：[http://blog.sina.com.cn/s/blog_531bb7630101841e.html](http://blog.sina.com.cn/s/blog_531bb7630101841e.html)



