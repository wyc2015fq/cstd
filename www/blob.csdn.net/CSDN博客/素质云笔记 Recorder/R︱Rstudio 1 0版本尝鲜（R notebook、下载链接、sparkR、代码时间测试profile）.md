
# R︱Rstudio 1.0版本尝鲜（R notebook、下载链接、sparkR、代码时间测试profile） - 素质云笔记-Recorder... - CSDN博客

2016年11月02日 15:43:17[悟乙己](https://me.csdn.net/sinat_26917383)阅读数：4385



**每每以为攀得众山小，可、每每又切实来到起点，大牛们，缓缓脚步来俺笔记葩分享一下吧，please~**![](https://img-blog.csdn.net/20161213101203247)
———————————————————————————

2016年11月1日，RStudio 1.0版本正式发布！这是自2011年2月RStudio发布以来的第10个主要版本，也是更新最大的一次。主要亮点如下：
1.R Notebooks 的辅助工具**（实时反馈结果，这个RMD做不来）**
2.对sparklyr包的集成支持（R与Spark连接）
3.profvis包进行性能分析**（代码运行步骤、代码运行时间）**
4.基于readr/readxl/haven，优化数据读取**（任意修改载入行名、列名以及一些字符串处理）**
5.R Markdown的辅助工具（websites和bookdown包）
6.其他优化和bug修复
**——部分内容参考自：R语言中文社区**

下载链接：[https://www.rstudio.com/products/rstudio/download3/](https://www.rstudio.com/products/rstudio/download3/)闲慢的可以看看我下的百度云盘链接：链接：[http://pan.baidu.com/s/1b5A5Y2](http://pan.baidu.com/s/1b5A5Y2)密码：xlmg
（版本：RStudio-1.0.44）

————————————————————————————————————

# 一、R notebook

1、安装问题
如果R的版本够的话，需要预装knitr，rmarkdown，同时你点击R notebook就会直接帮你加载。
但是另外一台电脑在低版本的R中，3.1好像需要加装tibble 1.0，并且尝试了还是用不来，报错：
`tibble1.1 is required but 1.0 is available`
会让你check that getOption("repos")看看版本对不对，不够我加载得对的tibble包，还是没用，所以不知道是不是版本问题。

2、界面
![](https://img-blog.csdn.net/20161102142657380)
界面跟Rmarkdown差不多，查看[R语言自动化报告格式——knitr](http://blog.csdn.net/sinat_26917383/article/details/47449367)
有以下几个新功能点：
为R Markdown增添一个强大的创作引擎。在数据分析领域，R Notebooks 具有极为引人注目的优势！


（1）实时反馈结果

可以看到代码运行到哪一步以及每步的运行结果；可以实时反馈结果，如果是传统的R Markdown模式，每次修改都需要重新knit(你懂的)才能看到效果，如果遇到大规模的计算，需要等上半天。
而在R Notebooks中，你可以立即看到你执行的代码的效果。此处“效果”包括各种内容：控制台打印结果、绘制图表、数据框，甚至交互的HTML控件。
![](https://img-blog.csdn.net/20161102144938732)

（2）兼容其他语言

R Notebooks不仅可以运行R代码，还可以运行Python, Bash, SQLor C++ (Rcpp).直接用SQL语句，牛不牛！
![](https://img-blog.csdn.net/20161102145238522)
（3）其他：丰富的输出格式。即时查看功能以及分享功能。

————————————————————————————————————

# 二、sparkR
Sparklyr 包是一个新的接口在R与Apache Spark. RStudio现在集成支持Spark和sparklyr包，主要工具如下：
1.创建和管理Spark连接
2.浏览表和Spark数据框的列
3.预览Spark数据框的前1000行
一旦安装好sparklyr包，你就可以发现Spark面板。这个面板包括一个新的连接，可以用于本地或者远程spark实例连接。
![](https://img-blog.csdn.net/20161102153203402)
连接成功后，你可以看淡Spark集群中的数据表。
![](https://img-blog.csdn.net/20161102153208464)
使用RStudio浏览Spark数据框中的数据。
![](https://img-blog.csdn.net/20161102153213015)


————————————————————————————————————————————————————

# 三、
# Profiling with profvis——代码测试神器

每一步代码的耗时，有了这个无疑在写函数的时候，就能够很好地对自己的代码进行测试，提高编译效率。笔者觉得这个乃是本次更新最大亮点。
![](https://img-blog.csdn.net/20161102153440531)
在栏目中就有这个，你可以指定行进行测试，用"profile selected Line(s)"
也可以用"start profilling " 一步一步测试，直到你的代码截止，然后点击“stop profilling”
![](https://img-blog.csdn.net/20161102153627471)

1、分析结果解读一：代码运行步骤
在分析结果中主要有两块内容：上部，是代码本身，以及执行每一行所消耗的内存及时间；下部是一个火焰图(什么鬼)，显示R在执行过程中具体干了啥，横向从左到右代表时间轴，纵向代表了调用栈也就是当前调用的函数。
![](https://img-blog.csdn.net/20161109102254143)

分析结果还有个Data页，点开来是个调用树，展示了各个函数调用的花费情况(仍然是内存及时间)。

2、分析结果解读二：代码运行时间
分析结果还有个Data页，点开来是个调用树，展示了各个函数调用的花费情况(仍然是内存及时间)。
![](https://img-blog.csdn.net/20161109102328036)
`a <- 1`代码可以调戏profvis
（本节内容来自公众号子豹）

————————————————————————————————————

# 四、数据输入——新手数据导入福音

RStudio 现在集成了readr/readxl/haven包，以提供高效的工具实现不同格式文件的导入。可以直接支持：
![](https://img-blog.csdn.net/20161102153816160)
导入的界面很亲民，同时把调用的代码也显示出来，新手可以来这看看，免去了很多麻烦！！
![](https://img-blog.csdn.net/20161102153820426)

1、功能点一：指定输入数据是否需要行名
![](https://img-blog.csdn.net/20161109102622493)
于是我们指定跳过6行记录，并且不要将第一行作为列名。
但是有些字段明明是数字却被显示成了字符串，可以直接在列名的下拉框里进行修正。（来源公众号：子豹）


