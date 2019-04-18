# 科学网—MetaPost 笔记 - 王伟华的博文




# MetaPost 笔记                           

已有 8316 次阅读2012-12-22 15:03|个人分类:[科技论文写作、搜索经验集锦](http://blog.sciencenet.cn/home.php?mod=space&uid=81613&do=blog&classid=104604&view=me)|系统分类:[科研笔记](http://blog.sciencenet.cn/home.php?mod=space&do=blog&view=all&uid=81613&catid=1)|关键词:MetaPost|[MetaPost](misc.php?mod=tag&id=151505)



     也许你会问：

     在数学课本上的各个精确美妙、含有复杂关系的几何图形都是怎么绘制出来的？ 

     网上各大牛论文里那些恰如其分的示例图形是怎么产生的？ 

     网上某些精美的 PDF 演示文档是怎么制作的？ 

     ... 

当然，这些 MetaPost 都可以做到的，有些会做的更好，下面就让我们一起学习 MetaPost 吧！

    MetaPost 是由贝尔实验室的 John Hobby 基于 Donald Knuth 的 Metafont 而发明的一个矢量图形作图工具，现由 Taco Hoekwater 进行维护开发。MetaPost 是一种描述性语言，其文件后缀名是 mp，经过 mpost filename 命令处理后，生成 eps 文件，可以方便的插入到 TeX 文件中。

    MetaPost 特别适合于绘制精确的几何图形，可以很容易的用几句指令就可以绘制一个简单的几何图形，语言描述特别直观易懂，也可以用方程的形式来表达各个变量之间的关系，MetaPost 会自动解方程以求得各个变量的值，再来用这些变量来绘制图形。书画同源，MetaPost 可以精确的绘制各种复杂的几何图形，同样，计算机中运用的各种矢量字体（TrueType,OpenType类型）也可以用 MetaPost 制作.

Hello MetaPost 


MetaPost 是一门关于绘制图形的编程语言，本文与其他编程语言手册一样，以经典的 "hello world" 开篇，来对 MetaPost 语言进行大致的了解。



以下为 MetaPost 的一个程序文件：hello.mp




```
% hello.mp 


% 这里是注释 


  


beginfig(1); 


  draw (0cm,0cm)--(1cm,0cm)--(1cm,1cm)--(0cm,1cm)--cycle; 


endfig; 


  


end;
```






0、MetaPost 输出文件

在我们的编辑器里，敲入以上文件代码，保存为 hello.mp，然后运行命令：




```
mpost hello
```




mpost 是 MetaPost 的编译器，它会自动识别文件后缀名 "mp",所以我们编译此文件时可以省略文件后缀名，mpost 会把它自动补上。

运行以上命令后，在同一文件夹下会产生 hello.1 和 hello.log 这两个文件，否则就会在终端显示错误提示信息，如果这样，建议你再检查一下你的 hello.mp 文件是否正确输入。

hello.1 就是我们所想得到的图形文件，它是 eps 格式的,这个文件名是 mpost 的自动设置的，我们可以通过 MetaPost 中的 filenametemplate 命令来改变输出文件名的格式，其格式参数类似于 C 语言中的 printf 函数，如下：




```
% hello.mp 


% 这里是注释 


  


filenametemplate "%j-%c.eps"; 


  


beginfig(1); 


  draw (0cm,0cm)--(1cm,0cm)--(1cm,1cm)--(0cm,1cm)--cycle; 


endfig; 


  


end;
```




运行 mpost hello 后，就得到了一个名为 hello-1.eps 的图形文件，而不是 hello.1，我们可以用 ghostscript 把它打开，它是一个用黑色线条绘制的边长为 1cm 的正方形。



1、MetaPost 语句及注释

MetaPost 文件里，所有语句均已分号';'结束，

'%' 代表注释的开始，直至行尾。

在 hello.mp 文件的头两行就是注释内容。



2、MetaPost 文件结构

一个 MetaPost 程序文件可以输出数个图形文件，每一个图形文件都是在 beginfig 和其相对应的 endfig 之间的命令来生成的，其大致结构如下：






```
<seq> 


  


beginfig(1); 


    <seq> 


endfig; 


  


beginfig(2); 


    <seq> 


endfig; 


  


... 


  


  


beginfig(n); 


    <seq> 


endfig; 


  


end;
```




在 hello.mp 文件中只有一对 beginfig--endfig，所以我们只得到一个图形文件 hello.1，如果像上面一样，我们就会得到 hello.1，hello.2，...，hello.n 等 n 个图形文件。



3、MetaPost 中绘制命令 draw 和路径<path>

hello.mp 中的 draw 是 MetaPost 中最常用的绘图命令，其后跟了一个路径<path>，在这里是 (0cm,0cm)--(1cm,0cm)--(1cm,1cm)--(0cm,1cm)--cycle，

其中二元运算符 '--' 生成一个以其两参数为端点的线段，比如 (0cm,0cm)--(1cm,0cm) 生成一个路径<path>，这个路径就是以点(0cm,0cm)和点(1cm,0cm)为端点的线段，

'--' 也可以将一些点顺序相连生成连续线段，比如 (0cm,0cm)--(1cm,0cm)--(1cm,1cm)--(0cm,1cm) 就生成三条线段，它们首尾顺序相连，

如果想产生一个闭合路径<path>，将路径<path>首尾两点相连，就要用到 cycle 命令，

hello.mp 中，(0cm,0cm)--(1cm,0cm)--(1cm,1cm)--(0cm,1cm)--cycle 中的 cycle 就表示该路径起点(0cm,0cm)和终点(0cm,1cm)相连。



4、MetaPost 点<point>

MetaPost 中的点<point>是由一个二元组<pair>来表示的，在 hello.mp 中，(0cm, 1cm) 就代表 x 坐标为 0cm 而 y 坐标为 1cm 的点。



5、MetaPost 单位

MetaPost 的默认单位为 Postscript points，因为 Postscript points 比打印机的 points 略大些，也被称为 "big points"，

Postscript points 和 打印机的 points 的简写形式分别为 bp 和 pt,在 TeX 中其关系如下：

1 bp = 1/72 inches

1 pt = 1/72.27 inches

当然，MetaPost 也理解其他单位，cm(厘米)、mm(毫米)、in(英寸)和 pc(picas:1pc = 1/6in) 等，

这里给出的单位其实都是 MetaPost 内部定义的数值常量，比如 cm 常量就是数值 28.34645， 1cm 其实是 1*cm 的简写形式，也就是数值 28.34645，

注意，这里的 1cm 其实是一个数值，数值的单位就是 MetaPost 的默认单位 bp。

我们也可以任意自定义一个“单位”，比如 u:=2cm，以后我们就可以用这个单位来表示一个点，

(1u, 2u) = (1*2cm, 2*2cm) = (2cm, 4cm) = (56.6929, 113.3858) = (56.6929bp, 113.3858bp)。



6、退出 MetaPost

在 MetaPost 程序文件中，如果最后没有 end 命令，当运行 mpost <filename> 命令后，MetaPost 运行 <filename> 程序文件后，不会自动退出，而是进入一个命令行解释器中，在其中我们可以运行 MetaPost 所允许的命令语句,在其中键入 end; 语句后退出解释器。



附： Metapost资源汇总（见[http://blog.sina.com.cn/s/blog_5e16f1770100l2r1.html](http://blog.sina.com.cn/s/blog_5e16f1770100l2r1.html)） 

1. 用 MetaPost作图初步含源文件（黄正华） ![](http://bbs.chinatex.org/static/image/filetype/rar.gif)[**MetaPost1.rar**](http://bbs.chinatex.org/forum.php?mod=attachment&aid=ODQ1fGE0NTkyODAwfDEyODE5Mzg1NjF8NDcyOQ==)



半小时前 上传
[**下载**(409.55 KB)](http://bbs.chinatex.org/forum.php?mod=attachment&aid=ODQ1fGE0NTkyODAwfDEyODE5Mzg1NjF8NDcyOQ==)



2. MetaPost 的 m3D 宏包手册中译本（含源文件） ![](http://bbs.chinatex.org/static/image/filetype/zip.gif)[**m3D_cn.zip**](http://bbs.chinatex.org/forum.php?mod=attachment&aid=ODQ0fDIyZDY1N2Q0fDEyODE5Mzg1NjF8NDcyOQ==)


半小时前 上传
[**下载**(488.86 KB)](http://bbs.chinatex.org/forum.php?mod=attachment&aid=ODQ0fDIyZDY1N2Q0fDEyODE5Mzg1NjF8NDcyOQ==)



3. METAPOST使用说明 ![](http://bbs.chinatex.org/static/image/filetype/pdf.gif)[**METAPOSTcn.pdf**](http://bbs.chinatex.org/forum.php?mod=attachment&aid=ODQ2fGE5MTcxYTNmfDEyODE5Mzg1NjF8NDcyOQ==)


半小时前 上传
[**下载**(235.71 KB)](http://bbs.chinatex.org/forum.php?mod=attachment&aid=ODQ2fGE5MTcxYTNmfDEyODE5Mzg1NjF8NDcyOQ==)



4.METAPOST的艺术-中文 ![](http://bbs.chinatex.org/static/image/filetype/zip.gif)[**AOM.tar.gz**](http://bbs.chinatex.org/forum.php?mod=attachment&aid=ODQ3fDA1NDAwYTY3fDEyODE5Mzg1NjF8NDcyOQ==)


3 分钟前 上传
[**下载**(1.08 MB)](http://bbs.chinatex.org/forum.php?mod=attachment&aid=ODQ3fDA1NDAwYTY3fDEyODE5Mzg1NjF8NDcyOQ==)





[http://ect.bell-labs.com/who/hobby/MetaPost.html](http://ect.bell-labs.com/who/hobby/MetaPost.html)
这是语言的创立者的页面，自不必多说。

中文，王垠
[http://www.math.zju.edu.cn/ligangliu/LaTeXForum/MetaPost/MetaPost.htm](http://www.math.zju.edu.cn/ligangliu/LaTeXForum/MetaPost/MetaPost.htm)

实例网页，例子大大地有
[http://tex.loria.fr/prod-graph/zoonekynd/metapost/metapost.html](http://tex.loria.fr/prod-graph/zoonekynd/metapost/metapost.html)

其他

[http://csweb.ucc.ie/~dongen/mpost/mpost.](http://csweb.ucc.ie/~dongen/mpost/mpost.html)[html](http://csweb.ucc.ie/~dongen/mpost/mpost.html)

王垠对MetaPost的介绍：[http://learn.tsinghua.edu.cn:8080/2001315450/metapost.html](http://learn.tsinghua.edu.cn:8080/2001315450/metapost.html)
（见[http://blog.chinaunix.net/uid-20676695-id-1899668.html](http://blog.chinaunix.net/uid-20676695-id-1899668.html)）
1.生成流程
1)使用编辑器编辑mp文件和tex文件
2)使用mpost filename.mp生成filename.1文件(eps格式)
3)在tex中使用包graphicx，即
usepackage{graphicx}
使用includegraphics{filename.1}来插入图形。
4)生成pdf文件
这里我有个为解决问题，就是使用pdflatex时会提示includegraphics不能被识别，但是改用如下步骤做却没错： 

latex test.tex
dvips test.dvi -o [test.ps](http://test.ps/)
ps2pdf [test.ps](http://test.ps/)

为了方便，不妨写个make脚本或bash脚本来完成从mpost到最后生成pdf的步骤。


2.中文支持（我是UTF8的忠实拥护者）
为了要支持中文，必须给mp文件加个壳，如下： 
verbatimtex
%&latex
documentclass{article}
usepackage{CJKutf8}
begin{CJK}{UTF8}{song}
begin{document}
etex

beginfig(1);
beginfig(1)
&<60;&<60; ....... // 这里写图形代码
endfig;


verbatimtex 
end{CJK}
end{document}
etex

end
------------------
当然,在tex文件里还是要做相应的utf8处理。 
另外科学网还有一个博文：MetaPost介绍 （[http://blog.sciencenet.cn/blog-267716-575504.html](http://blog.sciencenet.cn/blog-267716-575504.html)）

MetaPost是一个矢量制图语言。

MetaPost介绍: 

[http://www.berlinix.com/MetaPost.html](http://www.berlinix.com/MetaPost.html)

在线制图

[http://www.tlhiv.org/mppreview/](http://www.tlhiv.org/mppreview/)

MetaPost示例大全

[http://tex.loria.fr/prod-graph/zoonekynd/metapost/metapost.html](http://tex.loria.fr/prod-graph/zoonekynd/metapost/metapost.html)

王垠的MetaPost介绍

[http://docs.huihoo.com/homepage/shredderyin/metapost.html](http://docs.huihoo.com/homepage/shredderyin/metapost.html)










转载本文请联系原作者获取授权，同时请注明本文来自王伟华科学网博客。
链接地址：[http://blog.sciencenet.cn/blog-81613-645284.html](http://blog.sciencenet.cn/blog-81613-645284.html)

上一篇：[[转载]LaTeX and Metapost Notes](blog-81613-644799.html)
下一篇：[Abel](blog-81613-647563.html)








