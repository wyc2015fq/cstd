# latex 符号，公式_用户5706034127_新浪博客
|||

很多符号是需要包含宏的，否则报错，：！undefined control sequence
比如，正比符号 \varpropto  需要包含
  \usepackage{amssymb}
\begin{aligned} 需要包含
 usepackage{amsmath}
\begin{equation}\label{eq11}
\begin{aligned}
p(d|q) &\varpropto  p(q|d)p(d) \\
&\overset{rank}{=}
 \log p(q|d) \\
&\overset{rank}{=}
 \sum_{w \in q} c(w,q) \log p(w|d)
\end{aligned}
\end{equation}
排序等价符号，
\overset{rank}{=} 
\overset{rank}{123141} 
符号大全：
http://blog.csdn.net/caiandyong/article/details/53351737

![latex <wbr>符号，公式](http://img.blog.csdn.net/20161126151131363?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![latex <wbr>符号，公式](http://img.blog.csdn.net/20161126151205707?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![latex <wbr>符号，公式](http://img.blog.csdn.net/20161126151235356?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![latex <wbr>符号，公式](http://img.blog.csdn.net/20161126151258435?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

