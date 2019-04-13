
# Microsoft+R：Microsoft R Open （MRO）安装和多核运作 - 素质云笔记-Recorder... - CSDN博客

2016年08月01日 15:13:56[悟乙己](https://me.csdn.net/sinat_26917383)阅读数：8212



**每每以为攀得众山小，可、每每又切实来到起点，大牛们，缓缓脚步来俺笔记葩分享一下吧，please~**![](https://img-blog.csdn.net/20161213101203247)
———————————————————————————
本文转载于公众号大猫的R语言课堂，公众号作者使用这个MRO+Visual Studio 2015来运行。
文中介绍了下载方法以及设置多线程。
**传统的RGui是单线程运行，此时microsoft收购之后实现了多线程，在高效上跨出了一大步。**

同时，笔者在使用的时候有一个非常大的困惑：

里面的函数包是不是跟原来的一样？

——从包的数量上来看，应该是一致的。但是不再是原来的R的社区。现在是MRAN社区。

链接是：https://mran.revolutionanalytics.com/packages/

——————————————————————————————————————————————————————————————

写在前面上次关于MRO的文章推送之后，许多小伙伴表示对于Microsoft R这个增强版本的R相当有兴趣，希望大猫快点更新，所以大猫加班加点（最近要投Paper，时间有点紧啊），经过若干小时的奋战，新一期的大猫课堂又和大家见面啦！
性能测试（一）：甩了社区版R几条街话说上回提到RevolutionAnalytics和Microsoft携手后推出了免费版的Microsoft R Open （MRO）以及企业版的Microsoft R Server （MRS）。依靠着BLAS/LAPACK库以及英特尔的多线程鸡血大补丸，微软宣称和社区版CRAN R相比，MRO在矩阵运算上的效率可谓有着恐怖的提高。然而口说无凭，为了服众，微软公布了基准测试成绩（Performance Benchmark）：
![](http://mmbiz.qpic.cn/mmbiz/oXQ3icGRzN2c3CiaXlfxsTY3SaoQz0wIRKjpVBxLR93S1IE6UmNibsWcx3FR3dP8qAbskIXmEKRgiaicXuHBoYpy2fw/640?wx_fmt=png&tp=webp&wxfrom=5&wx_lazy=1)


基准测试代码可以从github下载，有兴趣的小伙伴们请戳：https://github.com/andrie/version.compare
整个基准测试分成了6大部分，分别为矩阵相乘、Cholesky分解、QR分解、奇异值分解、主成分分析、线性判别分析。这六大任务覆盖了常见的所有矩阵密集型运算，具有相当的代表性。那MRO的成绩如何？以矩阵相乘为例，我们发现，CRAN R在单线程下需要运行140.63秒，而同样在单线程下，MRO只需要14.11秒，速度是原来的近10倍！然而这里的提高仅仅是由BLAS/LAPCAK库所带来的，在多线程下面MRO有着怎样的表现呢？上面的表格告诉我们，在4线程（大多数的超极本都有2核2线程或者2核4线程）以及8线程（大多数台式PC的情况）的条件下，MRO只花了3.27秒以及1.89秒就跑完测试，性能分别是CRAN R的43倍及74倍！Intel大补丸见效之快、药效之好实在是令人咋舌！
在其他项目的测试中，MRO相比CRAN R都有着2x~50x的提高。可能有小伙伴会问，那CRAN R在多线程形况下性能有多少提高呢？大猫提醒大家，CRAN R执行命令是单线程的，因此哪怕在多核CPU上跑CRAN R，性能也不会比单核有多大提高！
为了更加直观反映性能提高，微软在下图中把CRAN R的成绩标准化为1，这样“27.41”就表示性能是基准的27.41倍：

![](http://mmbiz.qpic.cn/mmbiz/oXQ3icGRzN2c3CiaXlfxsTY3SaoQz0wIRKzelkFZ5HiaZQnysEHiaxk2MwkIcuoh38bEUft33WibP9bN2LKgJytAMZQ/640?wx_fmt=png&tp=webp&wxfrom=5&wx_lazy=1)


注：MRO的性能提高主要体现在矩阵运算与多线程上。如果你的代码/包本身几乎不包含任何矩阵运算，那么使用MRO并不会带来太多性能上的提高。
性能测试（二）：SAS也不在话下！在被Microsoft收购之前，初生牛犊不怕虎的RevolutionAnalytics还做过一件相当嚣张的事：为了宣传他们的企业版R，RevolutionAnalytics发布了一本白皮书，书名相当直言不讳，叫做Revolution R Enterprise: Faster than SAS。在白皮书中，RA团队宣称他们“想方设法保证了测试的公平”。首先，他们特地设计了一个“最贴近商业应用环境”的基准测试。不仅如此，为了尽可能消除争议，他们特地邀请了一个企业SAS顾问来帮助他们优化作为比较的SAS代码，使之最大可能体现SAS的语法优势。最后，他们还大费周章部署了分布式SAS Grid，使之也能使用多线程（在SAS中部署分布式要比R中复杂得多，大猫曾经有尝试，最终放弃……）。一切准备妥当，RA公布了一个难以置信的结果：
ScaleR ran the analysis tasks 42 times FASTER than SAS！！
这个结果自然令R阵营欢呼雀跃，但也难免引起了广泛的争议。大猫也没有仔细读完白皮书，故暂时不好发表意见。不过作为一个7年的SAS使用者，大猫在这里中肯地说：即使速度的确不如R，但是SAS在稳定性上仍旧是R无法企及的。一个很常见的例子是：在SAS中，无论你的程序陷入了怎样的死循环，按一个terminate键立马就能退出，而且程序从来不崩溃；但是在R中，一旦陷入死循环或者假死状态，想要退出只能重启R进程，而且working space里的数据需要重新加载！这是相当令人痛苦的。打个比方，SAS就好像AK47，R就好比M4A1，后者先进，但是前者——耐操啊！

![](http://mmbiz.qpic.cn/mmbiz/oXQ3icGRzN2c3CiaXlfxsTY3SaoQz0wIRKeeJTa0U2U0k8L7bFpTDcuLxOp27MCdoYDnS7iaGREt2QYAmFNUJE7ibg/640?wx_fmt=jpeg&tp=webp&wxfrom=5&wx_lazy=1)


白皮书中对R vs. SAS大战的总结。想要更多了解这场R和SAS的大战，请访问以下链接：http://www.revolutionanalytics.com/sites/default/files/revolution-analytics-sas-benchmark-whitepaper-mar2014.pdf
如何安装说了那么多MRO的引人入胜之处，小伙伴们一定希望马上体验。其实MRO的安装非常简单：
1）首先，进入MRO下载页面：https://mran.revolutionanalytics.com/download/
2）在下载页面中，我们能够看到针对多个系统的MRO版本下载，从Windows、MacOS一直到Ubantu、Red Hat等各种Linux发行版。因为大猫用的是Win10，所以直接点击Windows-Microsoft R Open这个链接就可以了。

![](http://mmbiz.qpic.cn/mmbiz/oXQ3icGRzN2c3CiaXlfxsTY3SaoQz0wIRKGrfHodRGibCP8Zibh396laHByZ55LJRPfM1iaTiaq5BVPude3faFuibYKBw/640?wx_fmt=jpeg&tp=webp&wxfrom=5&wx_lazy=1)
注：只有64bit系统才能运行MRO！
3）最重要的一步来啦，接下来我们就要安装MRO的灵魂——来自英特尔的鸡血多线程补丁MKL。我们只要点击对应MRO版本的MKL进行下载即可。注意，需要先安装MRO再安装MKL库，否则会安装失败。
4）大功告成！这时启动MRO，会在启动界面中发现MRO已经自动侦测到你的多核CPU并且自动设置线程数：
![](http://mmbiz.qpic.cn/mmbiz/oXQ3icGRzN2c3CiaXlfxsTY3SaoQz0wIRK05ZxcibyHBy82p3OaHVjGUJ8cIhCa8zDfJFuRtHFqhM0h5WmtwZlR6w/640?wx_fmt=jpeg&tp=webp&wxfrom=5&wx_lazy=1)

由于大猫用的是Surface Pro 4，所以在这里MRO显示自动开启了双线程。
当然，你也可以手动查看并且设置所用的核心数。其中，查看使用核心数的的语句是getMKLthreads()，设置核心数的语句是setMKLthreads(<value>)。例如：
![](http://mmbiz.qpic.cn/mmbiz/oXQ3icGRzN2c3CiaXlfxsTY3SaoQz0wIRKwlGTiaEenUb2kFaq44elTfgjloib1C8reRcCxNAT4Yf2RVvXHnz1bwPA/640?wx_fmt=jpeg&tp=webp&wxfrom=5&wx_lazy=1)
本期总结本期大猫带领大家领略了MRO在矩阵运算上相对于CRAN R的巨大提高，同时还介绍了MRO的安装方法。但是，除了多线程运算，MRO还有其他杀手锏！比如TimeMachine——可以把你加载的包返回到任意时间以及任意版本。此外，俗话说好马配好鞍，在安装了MRO之后，你需要一个能最大程度上发挥MRO潜力的开发工具——那就是同样来自于Microsoft、这个星球上当之无愧的最强大的集成开发环境——Visual Studio。在这里大猫先放两张自己所用的开发环境截图，在下一期中，大猫带你继续领略Microsoft R的无限魅力！

![](http://mmbiz.qpic.cn/mmbiz/oXQ3icGRzN2c3CiaXlfxsTY3SaoQz0wIRKn7SoyRYZYUqtmnP5T1VgcOmSMAmDebftuylxRe3A1wvVuSlpGicbIog/640?wx_fmt=png&tp=webp&wxfrom=5&wx_lazy=1)![](http://mmbiz.qpic.cn/mmbiz/oXQ3icGRzN2c3CiaXlfxsTY3SaoQz0wIRK9VcSmH2iczKc3aTtm9zMmCg2BLDmxcia7GLICRSJUiccIeAicCw9TNToAw/640?wx_fmt=png&tp=webp&wxfrom=5&wx_lazy=1)

Microsoft R Open + R Tools for Visual Studio开发环境截图



**每每以为攀得众山小，可、每每又切实来到起点，大牛们，缓缓脚步来俺笔记葩分享一下吧，please~**![](https://img-blog.csdn.net/20161213101203247)
———————————————————————————


