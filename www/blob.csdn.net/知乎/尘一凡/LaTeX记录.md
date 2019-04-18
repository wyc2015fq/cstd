# LaTeX记录 - 知乎
# 

**我该下载哪些LaTeX的套装和软件？**

[下载texlive吧，原因请点击](https://link.zhihu.com/?target=https%3A//liam0205.me/texlive/)

**如何入门LaTeX？**

[一份其实很短的LaTeX入门介绍](https://link.zhihu.com/?target=https%3A//liam0205.me/2014/09/08/latex-introduction/)

**如何实现中英文混排版？**

以下是用的是texlive2018套件：

> \documentclass[UTF8]{ctexart}
\begin{document}
你好，world!
\end{document}

**如何使用定理环境？**

第一步，在**导言区**写上 

> \newtheorem { LaTeX调用的名字 }{你取的名字}[计数器] 

比如：
\newtheorem{Lm}{引理}[section] 

第二步，在正文写上

> \begin{ 你所定义可以给LaTeX调用的名字}
*****这里写上你需要的文字********
\end{你所定义可以给LaTeX调用的名字}

比如：
\begin{Lm}
  (我的引理) $a^2+b^2=c^2$
\end{Lm}

效果如下：
![](https://pic2.zhimg.com/v2-773181fedbefa8d1620ccc715c489ca5_b.jpg)

