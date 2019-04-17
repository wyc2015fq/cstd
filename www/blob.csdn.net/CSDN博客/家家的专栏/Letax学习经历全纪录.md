# Letax学习经历全纪录~ - 家家的专栏 - CSDN博客





2011年11月18日 16:17:22[依海之燕](https://me.csdn.net/yihaizhiyan)阅读数：2201








最近感觉脑子生锈了，总是转不起来~  这可不行啊~ 

 估计学点新东西就好些了~ 润滑润滑这颗生锈的脑袋~

[CTEX](http://www.ctex.org/HomePage) 的下载地址：  [http://www.ctex.org/CTeXDownload](http://www.ctex.org/CTeXDownload)

 A LaTeX Editor下载地址： [http://texstudio.sourceforge.net/#downloads](http://texstudio.sourceforge.net/#downloads)



参考资料：[http://wenku.baidu.com/view/0e139848c850ad02de8041b1.html](http://wenku.baidu.com/view/0e139848c850ad02de8041b1.html)



常出现的编译错误~

LATEX： 

1、missing inserted 误用数学符号，比如_ 使用滴命令为：\underline{  }

2、LaTeX可以通过这符号$^$ 和$_$
 来设置上标和下标。用^ 来表示上标，用_ 来表示下标，如果上标的内容多于一个字符，注意用{
 } 把上标括起来，上下标是可以嵌套的，下面是一些简单例子：

$\sum_{i=1}^n a_i=0$
$f(x)=x^{x^x}$

3、段首空两格最为简单的方法： 不用\\空一行~

4、插入分割线：\hrule{}

5、,通过修改\itemsep的值,可以很好的改变条目间距.

|**代码**|
|----|
|\begin{thebibliography}{99}\addtolength{\itemsep}{-1em} % 缩小参考文献间的垂直间距\bibitem{Knu01} Knuth1,D.E.,The \TeX Book, Addison-Wesley   \bibitem{Knu02} Knuth2,D.E.,The \TeX Book, Addison-Wesley   \bibitem{Knu03} Knuth3,D.E.,The \TeX Book, Addison-Wesley   \bibitem{Knu04} Knuth4,D.E.,The \TeX Book, Addison-Wesley\end{thebibliography}|



 参考文件类型


|条目|说明|不可少域|可省略域|
|----|----|----|----|
|@article|期刊或杂志上的一篇文章|author, title, journal, year|volume, number, pages, month, note|
|@book|有确定出版社的书籍|author或editor, title, publisher, year|volume, number, series, address, edition, month, note|
|@booklet|印制的有封皮的作品，但没有出版社或赞助机构的名称|title|author, howpublished, address, month, year, note.|
|@conference|同@inproceedings|| |
|@inbook|一本书的一部分(章，节或某些页)|author或editor, title, chapter和/或pages, publisher,year|volume或number, series, type, address, edition, month, note|
|@incollection|一本书中有自己题目的一部分|author, title, booktitle, publisher, year|editor, volume或number, series, type, chapter, pages, address, edition, month, note|
|@inproceedings|会议论文集中的一篇文章|author, title, booktitle, year|editor, volume或number, series, pages, address, month, organization, publisher, note|
|@manual|科技文档|title|author, organization, address, edition, month, year, note|
|@mastersthesis|硕士论文|author, title, school, year|type, address, month, note|
|@phdthesis|博士论文|author, title, school, year|type, address, month, note|
|@proceedings|会议论文集|title, year|editor,volume或number, series, address, month, organization, publisher, note|
|@techreport|学校或其它研究机构印制的报告|author, title, institution, year|type, number, address, month, note|
|@unpublished|有作者和标题的还未出版的作品|author, title, note|month, year|
|@misc|不属于其它任何类型的作品|无|author, title, howpublished, month, year, note|

转自：[http://blog.renren.com/share/91803902/7889949694](http://blog.renren.com/share/91803902/7889949694)


