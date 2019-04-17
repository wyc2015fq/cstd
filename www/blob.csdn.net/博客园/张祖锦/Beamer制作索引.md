# Beamer制作索引 - 张祖锦 - 博客园







# [Beamer制作索引](https://www.cnblogs.com/zhangzujin/p/9249314.html)





\documentclass{beamer}
\usepackage{multicol}
\usepackage{makeidx}
 
\newenvironment{theindex}{%
\let\item\par % definitions for subitem etc
\begin{multicols}{2}
}{\end{multicols}}
\newcommand\indexspace{}
 
\makeindex
 
\begin{document}
\begin{frame}{test 1}
\index{Laplace}Laplace transform is... \LaTeXe\ can make you happy!
\index{Test}
\index{A}\index{B}\index{C}\index{D}\index{E}\index{F}
\end{frame}
 
\begin{frame}{test 2}
\printindex
\end{frame}
\end{document}















