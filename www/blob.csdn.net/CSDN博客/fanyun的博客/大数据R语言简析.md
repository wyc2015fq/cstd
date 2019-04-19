# 大数据R语言简析 - fanyun的博客 - CSDN博客
2016年03月17日 11:13:43[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：3439
所属专栏：[大数据](https://blog.csdn.net/column/details/bigdata01.html)
R语言是用于统计分析、绘图的语言和操作环境。R是属于GNU系统的一个自由、免费、源代码开放的软件，它是一个用于统计计算和统计制图的优秀工具。
R是统计领域广泛使用的诞生于1980年左右的S语言的一个分支。可以认为R是S语言的一种实现。而S语言是由AT&T贝尔实验室开发的一种用来进行数据探索、统计分析和作图的解释型语言。最初S语言的实现版本主要是S-PLUS。S-PLUS是一个商业软件，它基于S语言，并由MathSoft公司的统计科学部进一步完善。后来Auckland大学的Robert Gentleman和Ross Ihaka及其他志愿人员开发了一个R系统。由“R开发核心团队”负责开发。
 R的使用与S-PLUS有很多类似之处，这两种语言有一定的兼容性。S-PLUS的使用手册，只要稍加修改就可作为R的使用手册。所以有人说：R，是S-PLUS的一个“克隆”。
R是基于S语言的一个GNU项目，通常用S语言编写的代码都可以不作修改的在R环境下运行。 R的语法是来自Scheme。R语言是开源的，对所有人是完全免费，自由使用，R语言源代码托管在github上；可以运行在多操作系统中，如Windows、Linux和UNIX等。
R语言资源：
- 主页：http://www.r-project.org
- CRAN: http://cran.r-project.org
- 国内镜像：
- http://mirror.bjtu.edu.cn/cran/
- http://mirrors.ustc.edu.cn/CRAN/
- http://mirror.lzu.edu.cn/CRAN/
- http://mirrors.xmu.edu.cn/CRAN/
R优势及特点
R是一套完整的数据处理、计算和制图软件系统，提供了广泛的统计分析和绘图技术环境：包括线性和非线性模型、统计检验、时间序列、分类、聚类等方法。，包括若干统计程序和强大的各种数学计算、统计计算函数库，用户可以简单地指定数据库和若干参数进行进行一个统计分析，也可以灵活机动的进行数据分析，创造出符合需要的新的统计计算方法。
从 R 语言的发展历史上看，R 主要是统计学家为解决数据分析领域问题而开发的语言，因此 R 具有一些独特的优势：
- 有效数据存储和处理系统；
- 拥有一整套数组和矩阵的操作运算符（其向量、矩阵运算方面功能尤其强大），统计学家和几乎覆盖整个统计领域的前沿算法（3700+ 扩展包）；
- 完整连贯的统计分析工具；
- 高质量、广泛的统计分析、数据挖掘平台
- 重复性的分析工作（Sweave = R + LATEX），借助 R 语言的强大的分析能力 + LaTeX 完美的排版能力，可以自动生成分析报告；
- 优秀的统计制图、绘图功能，制图具有印刷的素质，也可加入数学符号；
- 一种相当完善、简洁和高效的程序设计语言：可操纵数据的输入和输出，可实现分支、循环，用户可自定义功能；
- R语言是彻底面向对象的统计编程语言；
- R语言和其它编程语言、数据库之间有很好的接口；
- 开放的源代码（free, in both senses），可以部署在任何操作系统，比如 Windows, Linux, Mac OS X, BSD, Unix强大的社区支持
- 方便的扩展性
- 可通过相应接口连接数据库，如 Oracle、DB2、MySQL
- 同 Python、Java、C、C++ 等语言进行互调
- 提供 API 接口均可以调用，比如 Google、Twitter、Weibo
- 其他统计软件大部分均可调用 R，比如 SAS、SPSS、Statistica等
- 甚至一些比较直接的商业应用，比如 Oracle R Enterprise, IBM Netezza, R add-on for Teradata, SAP HANA, Sybase RAP
R的功能能够通过由用户撰写的套件增强。增加的功能有特殊的统计技术、绘图功能，以及编程界面和数据输出/输入功能。这些软件包是由R语言、LaTeX、Java及最常用C语言和Fortran撰写。其中有几款较为常用，例如用于经济计量、财经分析、人文科学研究以及人工智能。
![082000_wy04_12.jpg](http://dataunion.org/wp-content/uploads/2014/12/1412181003584411.jpg)
与Matlab相比，R更具备开放性
- R是自由软件，Matlab是商业软件；
- R可以方便的通过“包”进行扩展，R的核心只有25个包，但是有几千个外部包可以调用，当然你也可以开发自己的；
- R语言比Matlab的要强大；
- R和其他编程语言/数据库之间有很好的接口；其他语言也可以很方便的调用R的API和结果对象。
- R常用于金融和统计领域。大多数人使用R就是因为它的统计功能，R的内部实现了很多经典的or时髦的统计技术。
效果演示
通过一个简单的例子，让R看起来更直观。
在R的控制台输入如下命令：
> install.packages(‘quantmod’) # 安装quantmod包
> require(quantmod) #引用quantmod包
> getSymbols(“GOOG”,src=”yahoo”,from=”2013-01-01″, to=’2013-04-24′) #从雅虎财经获取google的股票数据
> chartSeries(GOOG,up.col=’red’,dn.col=’green’) #显示K线图 > addMACD() #增加MACD图
就能够看到下图的效果了：
![wKioOVGMlfnAJTHXAABfL3nwKDE722.jpg](http://dataunion.org/wp-content/uploads/2014/12/141218100358442.jpg)
