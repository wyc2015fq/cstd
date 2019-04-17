# CodeViz产生函数调用图 - DoubleLi - 博客园






一.安装过程  
	1.CodeViz的安装


   	0).确认你已经安装Graphviz[
[http://www.graphviz.org/]](http://www.graphviz.org/%5D)

   	1).下载CodeViz.[
[http://www.csn.ul.ie/~mel/projects/codeviz/]](http://www.csn.ul.ie/~mel/projects/codeviz/%5D)

      	具体过程如下:


      	比如:我下载的是最新的一个版本:codeviz-1.0.11.tar.gz


      	下面的${CODEVIZ}指的是该文件解压后的目录.


      	a).tar xzvf codeviz-1.0.11.tar.gz


      	b).cd ${CODEVIZ}


            查看README,因为这关系到后面下载的gcc的版本


      	c).由于codeviz-1.0.11对应的gcc版本是3.4.6,因此下载的是


         gcc-3.4.6.tar.gz


         [
[ftp://ftp.gnu.org/pub/gnu/gcc/gcc-3.4.6/gcc-3.4.6.tar.gz]](ftp://ftp.gnu.org/pub/gnu/gcc/gcc-3.4.6/gcc-3.4.6.tar.gz%5D)


         把下载的gcc-3.4.6.tar.gz放入${CODEVIZ}/compilers/


      	这样,所需的工具基本下载完成了. 


    	2.编译


      	0).cd ${CODEVIZ}


         ./configure


      	1).查看生成的Makefile文件.


         由于CodeViz实际上是给gcc打了补丁,然后重新编译gcc,这就涉及到你自己编译


的gcc的安装路径.其路径是Makefile变量中的${GCCGRAPH}定义的,你修改该路径则可以指定你自己编译的


gcc的安装路径,默认的是/usr/local/gccgraph.[推荐不要改变该默认值]


      	2).make


         这就开始编译gcc-3.4.6了,过程有些漫长,可以休息一下去了:)


      	3).make install



      	整个过程是不是很简单.




二.CodeViz的使用 
     如果你对这个工具的原理颇感兴趣,哪请参考<分析函数调用关系图(call graph)的几

种方法>:

     [[http://blog.csdn.net/Solstice/archive/2005/09/24/488865.aspx]](http://blog.csdn.net/Solstice/archive/2005/09/24/488865.aspx%5D)


     其原理可以简单说明一下:

     其基本原理是给 GCC 打个补丁，让它在编译时每个源文件时 dump 出其中函数的 c

all graph，然后用 Perl 脚本收集并整理调用关系，转交给Graphviz绘制图形。

     来自<用CodeViz绘制函数调用关系图(call graph)>

     [[http://blog.csdn.net/Solstice/archive/2005/09/22/486788.aspx]](http://blog.csdn.net/Solstice/archive/2005/09/22/486788.aspx%5D)


     在这篇文档有简要的使用步骤:

     	1. 想办法让 gcc 能编译你关心的源码，每个c源文件生成对于的.c.cdepn文件。只

要编译(参数 -c)就行，无需连接。[实际上就是指定编译器为你编译出来的哪个gcc-3.4.

6,关于怎么指定编译编译的问题,就不属于这篇文档的范畴了:)]


	2. 调用genfull生成full.graph，这个文件记录了所有函数在源码中的位置和它们之间的

调用关系。[查看帮助的方式genfull --help]


	3. 使用gengraph生成我关心的函数的调用关系。[]


     	关于如何使用gengraph生成你感兴趣的调用图,也请参考上面这篇文档的几个例子,实

在不行,那就看help吧:)


     	实在不行,哪看[[http://www.csn.ul.ie/~mel/projects/codeviz/]](http://www.csn.ul.ie/~mel/projects/codeviz/%5D)

	啃E文吧,应该也不算难哦!:)


     	就是这些了,如何让GNU下的这些工具如何更有创意,就看你的了:)


	PS:由于上面全是源码安装,应该和发行版无关,不过RH9及其以下偶就不保证了:(



三.CodeViz的使用示例

    
恩,终于来点正式得了,好了,就从Linux内核得net目录分析起来吧!

    编译时记得指定CC=/usr/local/gccgraph/gcc哦,实在不懂,哪请麻烦你去看看Makefile.


    好了,先产生full.graph

    1.在内核目录下面,指定genfull -s "inlcude/linux net/ipv4"就可以了,不过根据

产生的信息,netfilter这块貌似有很多函数重名哦,由于只是演示一下,先暂且不管了,并且我们这

不是分析netfilter:)[解决重名冲突的方法是指定目录以及文件的时候范围更小一些]

    2.根据产生出来的full.graph来生成我们感兴趣的函数调用图

    恩,我们就从进入IP层的第一个函数分析起吧--ip_rcv(),暂且分析三层.[层数多了想来也麻烦啊:)]

    好了,gengrap用将起来,还是直接给出结果吧,呵呵

![](http://img44.pp.sohu.com/images/blog/2007/1/28/17/3/110fdf6427b.jpg)


   是不是还不错:)

   也许你不想先看路由部分代码吧,哪使用-s参数啊,先不看:

![](http://img64.pp.sohu.com/images/blog/2007/1/28/17/28/110fdf0d4eb.jpg)
    这样是不是明朗很多,呵呵,好了,就说到这了.


参考文档:

1.<用CodeViz绘制函数调用关系图(call graph)>
[http://blog.csdn.net/Solstice/archive/2005/09/22/486788.aspx](http://blog.csdn.net/Solstice/archive/2005/09/22/486788.aspx)


2.<分析函数调用关系图(call graph)的几种方法>
[http://blog.csdn.net/Solstice/archive/2005/09/24/488865.aspx](http://blog.csdn.net/Solstice/archive/2005/09/24/488865.aspx)


3.[http://www.csn.ul.ie/~mel/projects/codeviz/](http://www.csn.ul.ie/~mel/projects/codeviz/)








