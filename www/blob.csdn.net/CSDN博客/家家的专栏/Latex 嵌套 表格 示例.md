# Latex 嵌套 表格 示例 - 家家的专栏 - CSDN博客





2011年11月29日 20:02:36[依海之燕](https://me.csdn.net/yihaizhiyan)阅读数：4111标签：[table																[c																[algorithm																[编译器](https://so.csdn.net/so/search/s.do?q=编译器&t=blog)
个人分类：[乐在科研中](https://blog.csdn.net/yihaizhiyan/article/category/742190)





                
 转自：[http://blog.renren.com/share/221226875/8030417521](http://blog.renren.com/share/221226875/8030417521)

介绍一下不规则表格的一种画法，使用multirow 这个包， 在latex文件最前面用这个包\usepackage{multirow}

在正文中用以下代码：这个是模版，根据需要调整。

\begin{table}[htbp]

\centering

\caption{\label{comparison}Result comparison on LN data}

\begin{tabular}{c|c|c|c|c|c|c|c}

\hline

\multirow{2}{*}{Instance} & \multirow{2}{*}{Original Instance} & \multirow{2}{*}{High Priority} & \multirow{2}{*}{Low Priority} & \multicolumn{2}{|c|}{Benchmark} & \multicolumn{2}{|c}{Our Algorithm} \\

\cline{5-8}

& & & & Utilization & Time(s) & Utilization & Time(s)\\

\hline

LN01\&02  &  LN01 \& LN02    &     LN01      &    LN02     &  99.3\%   & 624 &    &   \\

\hline

\end{tabular}

\end{table}

效果：

![](http://fmn.xnpic.com/fmn050/20110809/1610/b_large_O3Fp_525f000230805c73.jpg)

解释：

1 \begin{table}表示表格开始啦，中括号中的htbp表示的是表格的一些位置信息，居中啊，紧随文章啊，下一页的最开始啦之类的，这个是最基本的表格参数，不赘述。

2 \caption{}： {}中可以填上正文里这个表格的名字。在括号中同时可以放上"\label{}"这个选项， \label{} 是你在文章中提到这个表格时的标记，是方便作者自己记忆的，不会出现在正文中，有点类似于\cite{}引用文章。\label{mark}，mark是你给这个表上的标记。引用这个表的时候就\ref{mark}

3 \begin{tabular}{c|c|c|}: 告诉latex编译器开始划正式的表格了。c|c 告诉latex这个表有几列，每一个c表示一列，有|表示这个列和下一列之间有一条竖线，不加|表示没有竖线，看自己需求，也可以都不加|，在下面的真实内容编辑时加竖线。

4 \hline表示画一条横线，横线的长度是表的宽度。相比较，\cline{m-n}表示画一条从m列到n列的横线。

5 如果是标准的一列，那么就是content1 & content2 &content3, contenti是内容，之间用&隔开即可。

6 重点介绍多行（多列）合并：在不规则表中，可以将多行合并，就是\multirow{2}{*}{Name}, {2}表示2行合并成一行，{*}表示文字居中写，{Name}就是文字的内容了。\multicolumn{2}{|c|}{Name} 表示2列合并成一列，{|c|}表示合并后的一列左右是否加竖线。{Name}也是该列里面的内容，没有内容就空着。

\multirow 和\multcolumn是可以嵌套使用的，里层写在外层的名字处。相当于内容是一个合并，而不是文字。例如\multicolume{2}{|c|}{\multirow{2}{*}{Name}}。

写于桃源楼2803 ^_^](https://so.csdn.net/so/search/s.do?q=algorithm&t=blog)](https://so.csdn.net/so/search/s.do?q=c&t=blog)](https://so.csdn.net/so/search/s.do?q=table&t=blog)




