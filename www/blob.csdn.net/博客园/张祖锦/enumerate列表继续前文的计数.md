# enumerate列表继续前文的计数 - 张祖锦 - 博客园







# [enumerate列表继续前文的计数](https://www.cnblogs.com/zhangzujin/p/10299172.html)




\documentclass[a4paper]{article}
\usepackage{enumitem} % load the package
\begin{document}
  \section{My items}
    \begin{enumerate}
      \item First item
      \item Second item
    \end{enumerate}
  \section {Additional items}
    \begin{enumerate}[resume] % tell the enumerate to resume numbering
      \item Third item
      \item Fourth item
    \end{enumerate}
\end{document}











