# [Tex学习笔记]积分平均 - 张祖锦 - 博客园







# [[Tex学习笔记]积分平均](https://www.cnblogs.com/zhangzujin/p/3757177.html)





$\def\avint{\mathop{\mathchoice{\,\rlap{-}\!\!\int}
                              {\rlap{\raise.15em{\scriptstyle -}}\kern-.2em\int}
                              {\rlap{\raise.09em{\scriptscriptstyle -}}\!\int}
                              {\rlap{-}\!\int}}\nolimits}$
$\avint$

$$\avint_\Omega f(x)\rd x=\frac{1}{|\Omega|}\int_\Omega f(x)\rd x.$$



以下能在tex下如输入“积分平均”. 非常感谢 [御坂01034](http://home.cnblogs.com/u/misaka01034/) 的帮助.



\documentclass[12pt]{elsarticle}

%\hoffset         -0.6in
%\voffset          -0.5in
%\textwidth       6.0in
%\textheight      9.0in

\usepackage{amsmath,amsthm,amssymb,dsfont,a0size,bm,indentfirst,wasysym}
\usepackage{stmaryrd,CJK,color,graphicx}
\def\Xint#1{\mathchoice
{\XXint\displaystyle\textstyle{#1}}%
{\XXint\textstyle\scriptstyle{#1}}%
{\XXint\scriptstyle\scriptscriptstyle{#1}}%
{\XXint\scriptscriptstyle\scriptscriptstyle{#1}}%
\!\int}
\def\XXint#1#2#3{{\setbox0=\hbox{$#1{#2#3}{\int}$ }
\vcenter{\hbox{$#2#3$ }}\kern-.6\wd0}}
\def\ddashint{\Xint=}
\def\dashint{\Xint-}
\begin{document}

$\ddashint$

$$\dashint$$
\end{document} 



















