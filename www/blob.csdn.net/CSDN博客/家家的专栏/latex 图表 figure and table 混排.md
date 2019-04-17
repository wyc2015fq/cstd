# latex 图表 figure and table 混排 - 家家的专栏 - CSDN博客





2014年05月14日 20:17:15[依海之燕](https://me.csdn.net/yihaizhiyan)阅读数：3233








图像和表格并排排列：

\makeatletter\def\@captype{figure}\makeatother
\begin{minipage}{0.3\textwidth}
\centering
\section{ Standard }
\includegraphics[scale=0.40]{/home/u2/webapp/public/data/images/articles/4803_thumb_160.jpg}

\end{minipage}
\makeatletter\def\@captype{table}\makeatother
\begin{minipage}{.7\textwidth}
\centering
\subsection{ Color Per Size Quantities }
\begin{tabular}{|c|c|c|c|c|c|c|}
  \hline
  % after \\: \hline or \cline{col1-col2} \cline{col3-col4} ...
   Measurements Nam &  Measurements Nam & Measurem & a & a & a & a \\
  a & a & a & a & a & a & a \\
  a & a & a & a & a & a & a \\
  a & a & a & a & a & a & a \\
  a & a & a & a & a & a & a \\
  \hline
\end{tabular}

\end{minipage}




转自：http://www.educity.cn/wenda/118274.html



