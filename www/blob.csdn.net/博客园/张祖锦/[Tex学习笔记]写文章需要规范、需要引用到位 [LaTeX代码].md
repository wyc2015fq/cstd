# [Tex学习笔记]写文章需要规范、需要引用到位. [LaTeX代码] - 张祖锦 - 博客园







# [[Tex学习笔记]写文章需要规范、需要引用到位. [LaTeX代码]](https://www.cnblogs.com/zhangzujin/p/3621667.html)





```
\documentclass{ctexart}

\usepackage{CJK,amsmath,amssymb,amsthm}

 

\begin{document}

写文章需要规范、需要引用到位.

 

方程:

\begin{equation}\label{NS}

u_t+(u\cdot\nabla)u-\triangle u+\nabla \pi=0

\end{equation}

为 Navier-Stokes 方程组. 对有限能量初值 $u_0\in L^2(\mathbb{R}^3)$, \eqref{NS} 有一个整体弱解, 这是 Leray 于 1934 年获得的 \cite{J_Leray_34_AM}.

 

\begin{thebibliography}{00}

\bibitem{J_Leray_34_AM}

    J. Leray,

    Sur le mouvement d'un liquide visqueux emplissant l'espace,

    {\it Acta Math.},

    {\bf 63} (1934), 193--248.

\end{thebibliography} 

\end{document} 

​
```












