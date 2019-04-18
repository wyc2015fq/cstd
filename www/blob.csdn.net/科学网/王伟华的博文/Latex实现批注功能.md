# 科学网—Latex实现批注功能 - 王伟华的博文




# Latex实现批注功能                           

已有 603 次阅读2019-2-21 11:05|个人分类:[科技论文写作、搜索经验集锦](http://blog.sciencenet.cn/home.php?mod=space&uid=81613&do=blog&classid=104604&view=me)|系统分类:[科研笔记](http://blog.sciencenet.cn/home.php?mod=space&do=blog&view=all&uid=81613&catid=1)



对LaTeX源文件进行修订有几种选择，包括changes，changebar，vertbars，framed等宏包和latexdiff工具（见http://www.tex.ac.uk/cgi-bin/texfaq2html?label=changebars的讨论）。


Latex批注功能的实现分为两步：




1.添加宏包，即\usepackage[margins,adjustmargins]{trackchanges}, 完事后，进行编译，便会发现，原来的pdf文档已变成两栏，其中右侧栏为空，该空栏为批注提供空间；




2.批注命令：(1)\change{A}{B}：将内容A用内容B替换，并在右侧空栏显示被替换掉的内容A;




                      (2)\add{A}：在命令处添加内容A;




                      (3)\remove{A}: 删除内容A;




                    (4) \annote{A}{B}: 为内容A添加侧边批注内容B,同时，内容A标注下划线

I have used the changes package in the past and I find it very useful. It has a key=value system so most of the things are customizable. You can define different authors and the changes are tracked depending on the id; here is a simple example (mostly from the manual).







\documentclass{article}




\usepackage{changes}

\usepackage{lipsum}% <- For dummy text

\definechangesauthor[name={Per cusse}, color=orange]{per}

\setremarkmarkup{(#2)}







\begin{document}

\lipsum[1-7]




This is \added[id=per,remark={we need this}]{new} text.

This is \added[id=per,remark={has to be in it}]{new} text.

This is \deleted[id=per,remark=obsolete]{unnecessary}text.

This is \replaced[id=per]{nice}{bad} text.




This is \added[remark={we need this}]{new} text.

This is \added[remark={has to be in it}]{new} text.

This is \deleted[remark=obsolete]{unnecessary}text.

This is \replaced{nice}{bad} text.







\listofchanges

\end{document}

then, the results can be shown  as:

[https://blog.csdn.net/xiaokun19870825/article/details/51062906](https://blog.csdn.net/xiaokun19870825/article/details/51062906)







And the nice thing is that if you supply the final option to the package declaration that is \usepackage[final]{changes} it clears the traces of changes made by the authors and respecting the last changes. 










方法2 用latexdiff 可以对Latex文档中所作的修改进行高亮显示。[https://blog.csdn.net/potatostyles/article/details/79086419](https://blog.csdn.net/potatostyles/article/details/79086419)[](https://blog.csdn.net/potatostyles/article/details/79086419)

#### [latexdiff的使用和配置](https://blog.csdn.net/potatostyles/article/details/79086419)







用 LaTeX 怎么进行批注和修改？

使用changes宏包

一个便捷的方法是使用 changes 宏包来实现，样例如下：

\documentclass{article}
\usepackage{changes}
	\definechangesauthor[name={Per cusse}, color=orange]{per}
	\setremarkmarkup{(#2)}
\begin{document}
This is \added[id=per,remark={we need this}]{new} text.
This is \added[remark={has to be in it}]{new} text.
This is \deleted[id=per,remark=obsolete]{unnecessary}text.
This is \replaced{nice}{bad} text.
\listofchanges
\end{document}


效果图如下：




changes 宏包加上[final]选项时，编译出来是修订后的文档。功能简单，使用比较简单。 选自：https://www.zhihu.com/question/22316670/answer/131498823







latex中的脚注、边注及注释






一、脚注

命令:\footnote{脚注内容}​

若想要改变编号形式，可以使用如下命令:

​\renewcommand{\thefootnote}{\alph{footnote}}  默认是阿拉伯数字

例子代码:

\renewcommand{\thefootnote}{\alph{footnote}}

As you begin your day and you are feeling happy,while you keep feeling happy,your day will be great!But if you begin your day in a bad mood\footnote{心情不好},and you do nothing to change it,your day will not be great at all.

效果:

![](http://s13.sinaimg.cn/mw690/006pUyk1zy74nlOOHeccc)

​二、边注

边注一般用的比较少，它的命令:\marginpar{边注内容}​，还可利用marginnote宏包的\marginnote命令，由于不使用浮动体，用得更广.​

切换边注的方向，使用\reversemarginpar和normalmarginpar命令

​例子代码:

​导言区加上:\usepackage{marginnote}

​As you begin your day and you are feeling happy,while you keep feeling happy,your day will be great!But if you begin your day in a bad mood\marginnote{第二行},and you do nothing to change it,your day will not be great at all.

效果:

![](http://s7.sinaimg.cn/mw690/006pUyk1zy74nmVvSMS46)

三、注释

较短的注释可以直接用%即可，若想对一大段文字进行注释可以使用verbatim宏包的comment环境.即:

\begin{comment}

......

\end{comment}​​




# LaTeX如何实现pdf中带有注释或批注

[http://www.latexstudio.net/archives/8279.html](http://www.latexstudio.net/archives/8279.html)


[http://www.latexstudio.net/archives/9771.html](http://www.latexstudio.net/archives/9771.html)


转载本文请联系原作者获取授权，同时请注明本文来自王伟华科学网博客。
链接地址：[http://blog.sciencenet.cn/blog-81613-1163385.html](http://blog.sciencenet.cn/blog-81613-1163385.html)

上一篇：[告别2018，迎接2019](blog-81613-1154643.html)
下一篇：[[转载]一些调和分析的reference](blog-81613-1168589.html)


