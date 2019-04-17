# latex 表格行间距设置 - 家家的专栏 - CSDN博客





2014年05月19日 11:25:58[依海之燕](https://me.csdn.net/yihaizhiyan)阅读数：9336








用到的package是：\usepackage{setspace}

命令：

**\begin{spacing}{1.19}**

**\end{spaceing}**




示例：

\makeatletter\def\@captype{table}\makeatother

\begin{minipage}[t]{0.45\linewidth}

\scriptsize

\centering

\caption{\small{Popular nonconvex surrogate functions of $||\theta||_0$ and their supergradients.}}\vspace{0.3cm}

\label{tab_nonpenlty}

\centering
**\begin{spacing}{1.19}**\begin{tabular}{l|l}

\hline 

。。。。。。。。。。。。



