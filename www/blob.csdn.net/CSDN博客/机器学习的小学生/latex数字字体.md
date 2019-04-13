
# latex数字字体 - 机器学习的小学生 - CSDN博客


2018年10月10日 09:31:57[机器学习的小学生](https://me.csdn.net/xuluhui123)阅读数：745


`\documentclass{article}
\usepackage[UTF8]{ctex}
\usepackage{threeparttable}
\usepackage[scale={0.9,0.9}]{geometry}
\DeclareMathAlphabet{\mathpzc}{OT1}{pzc}{m}{it}%数学符号字体的设置
%\usepackage{bm}      % 粗斜体 \bm
\usepackage{bbm}     % \mathbbm, \mathbbss, \mathbbmtt
\usepackage{dsfont}  % \mathds
\usepackage{yfonts}  % \textfrak, \textswab
\usepackage{amssymb} % \mathfrak, \mathcal
\usepackage{mathrsfs}% \mathscr, 不同于\mathcal or \mathfrak 之类的英文花体字体%产生拉普拉斯变换式的字母
\begin{document}
\newcommand{\testmath}[1]{ \begin{tabular}{l}
    $#1{ABCDEFGHIJKLMNOPQRSTUVWXYZ}$ \\[-5pt]
    $#1{abcdefghijklmnopqrstuvwxyz\ 0123456789}$ 
\end{tabular}}
\newcommand{\testtext}[1]{ \begin{tabular}{l}
    #1{ABCDEFGHIJKLMNOPQRSTUVWXYZ} \\[-5pt]
    #1{abcdefghijklmnopqrstuvwxyz\ 0123456789} 
\end{tabular}}
\begin{table}
\centering
\begin{threeparttable}
\begin{tabular}{c|l|c}
  \hline
  command          &       result            & package \\\hline
  default          & \testmath{}             & non        \\\hline
  \verb|\mathrm|   & \testmath{\mathrm}      & non        \\\hline
  \verb|\mathit|   & \testmath{\mathit}      & non        \\\hline
  \verb|\mathbf|   & \testmath{\mathbf}      & non        \\\hline
  \verb|\mathtt|   & \testmath{\mathtt}      & non        \\\hline
  \verb|\mathsf|   & \testtext{\textsf}      & non\tnote{a}        \\\hline
  \verb|\mathcal|  & \testmath{\mathcal}     & non\tnote{b}        \\\hline
  \verb|\mathbb|   & \testmath{\mathbb}      & \verb|\usepackage{amssymb}|\tnote{c}   \\\hline
  \verb|\mathfrak| & \testmath{\mathfrak}    & \verb|\usepackage{amssymb}|   \\\hline
%  \verb|\bm|       & \testmath{\bm}          & \verb|\usepackage{bm}|        \\\hline
  \verb|\mathbbm|  & \testmath{\mathbbm}     & \verb|\usepackage{bbm}|       \\\hline
  \verb|\mathbbmss|  & \testmath{\mathbbmss} & \verb|\usepackage{bbm}|       \\\hline
  \verb|\mathbbmtt|  & \testmath{\mathbbmtt} & \verb|\usepackage{bbm}|       \\\hline
  \verb|\mathscr|  & \testmath{\mathscr}     & \verb|\usepackage{mathrsfs}|  \\\hline
  \verb|\mathds|   & \testmath{\mathds}      & \verb|\usepackage{dsfont}|\tnote{d}  \\\hline
  \verb|\mathpzc|  & \testmath{\mathpzc}     & non\tnote{e}  \\\hline
  \verb|\textfrak| & \testtext{\textfrak}     & \verb|\usepackage{yfonts}|  \\\hline
  \verb|\textswab| & \testtext{\textswab}     & \verb|\usepackage{yfonts}|  \\\hline
\end{tabular}
\begin{tablenotes}
\item [a] \verb|\mathpzc| conflicts with \verb|\mathsf|, so here the result is in fact from \verb|\textsf|
       which gives the same result with \verb|\mathsf|.
\item [b] The useage of package \verb|eucal| can change the font appearence.
\item [c] The \verb|amssymb| package is a superset of the \verb|amsfonts| package.
\item [d] Using \verb|sans| option, \verb|\usepackage[sans]{dsfont}|, gives sans version font.
\item [e] Need the command: \verb|\DeclareMathAlphabet{\mathpzc}{OT1}{pzc}{m}{it}|
\end{tablenotes}
\end{threeparttable}
\end{table}
\end{document}`![在这里插入图片描述](https://img-blog.csdn.net/20181010093444471?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3h1bHVodWkxMjM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

