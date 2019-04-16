# x264 编码器选项分析 (x264 Codec Strong and Weak Points) 2 - 雷霄骅(leixiaohua1020)的专栏 - CSDN博客





2014年09月03日 11:13:43[雷霄骅](https://me.csdn.net/leixiaohua1020)阅读数：5194










文章目录：

[x264 编码器选项分析 (x264 Codec Strong and Weak Points) 1](http://blog.csdn.net/leixiaohua1020/article/details/39004293)


[x264 编码器选项分析 (x264 Codec Strong and Weak Points) 2](http://blog.csdn.net/leixiaohua1020/article/details/39023071)

======================


上篇文章简单翻译了MSU实验室做的X264的Option（即编码选项，后文称其英文名）分析报告《x264 Codec Strong and Weak Points》的前面部分。前面部分部分比较基础，而其后的内容相对来说要复杂许多。本文继续上篇文章的内容，简单记录接下来的内容。



前文通过“彩云图”的方法分析了X264的Preset。“彩云图”这种方式的优点是一目了然，可以清晰地看出各种Option取不同的值对视频的质量和编码速度的影响。但“彩云图”这种分析是有局限性的。关键在于它对X264的Option只能定性的作分析，而不能定量地作分析。如果想要定量地画出图表，还需要下文所述的几种方法。

下文所述的几种方法，可以定量地分析X264的Option。根据不同的情况，画出相应的图表。



## Lambda的 Preset分析

编码速度和编码质量对于视频编码器来说都很重要。但是这两个因素差别太大，因而很难以比较它们。如果一个Preset的速度和质量都高于另一个Preset，我们可以说前面那个Preset更好一些。但是如果一个Preset的速度高于另一个Preset，质量却低于另一个Preset，该如何比较？比较极端的情况下，我们通过主观的方式可以进行比较。比如说Preset A比Preset B的速度高两倍，但是码率高0.1%，我们可以说Preset A好于Preset B。为了定量研究上述问题，我们引入参数λ，用于表示期望编码时间与期望质量之间的比值。

λ=期望编码时间较优/期望质量较优


较大的λ代表期望的速度比质量重要（如果λ为无穷大，则只追求最快的速度，而质量根本无足轻重）。较小的λ含义则相反。

我们将通过以下方式分析数据

1.计算所有Preset的λ值。

2.根据λ值选取“前10%的Preset”。

3.选择不同的λ值，分别得到它们“前10%的Preset”，最后得到实验结果。

注意下面这张图，非常形象了说明了实验的步骤。

![](https://img-blog.csdn.net/20140903094843812?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


### 选择λ

本实验的难点之一是λ的取值。取值过程不再细说，最后选定了0.01-7的多个λ值。较大的λ代表速度比质量重要（如果λ为无穷大，则希望速度最快而质量无所谓）。较小的λ含义则相反。



### 据λ值选取“前10%的Preset”

其中有较多的数学方法，讲述起来很繁琐，自己也没有深入进去看。大概明白其选择”前10%的Preset“的方法，简单叙述一下。

筛选的目的其实就是找到一部分速度和质量都比较好的Preset。前文已经分析过，实验结果图上越是位于左边的点，代表其Preset速度快；越是位于下面的点，代表其Preset质量好。因此位于左下方的Preset是最好的。关键的问题就是选择哪些位于左下方的点。实验中使用下述公式进行筛选：

M=λT+Q

这是一个普通的线性函数。斜率是λ，横坐标T是相对时间，纵坐标M是相对码率，截距Q是一个不定的数字。这样当我们选定一个λ的时候，就确定了一条斜率固定，截距可以变化的直线，如图所示（如图中的粉红和蓝色的线）。下面假设这条线的截距从0开始逐渐增加，当它碰上的第一个点，就是该λ值最佳的Preset（因为它是最靠近左下方的，如下图中红色五角星的点）。然后直线截距继续增加，会遇到越来越多的点。直到获取到占总点数10%的点（如图中浅蓝色的点）。至此，直线遇到的所有的点就可以列为该λ下“前10%的Preset”。

![](https://img-blog.csdn.net/20140903095552203?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


下面几张图分别列举了不同的λ情况下，“前10%的Preset”的点的分布情况。可以看出，λ越小，点越分布在图表的下方（例如λ=0.01，λ=0.02）。代表期望编码质量更优。λ越大，点越分布在图表左边（例如λ=7）。代表期望编码时间更优。

![](https://img-blog.csdn.net/20140903095628406?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


![](https://img-blog.csdn.net/20140903095900513?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


![](https://img-blog.csdn.net/20140903095911729?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


最后的分析就是针对上述的“前10%的Preset”的点进行分析的。其它没有列入的位于图表右上方的点都不在本次实验数据分析的范围内。由此可见本次实验数据主要分析图表中左下方的点（也就是性能比较好的Preset），取不同的λ，分析的点也不同。

###  实验结果

最终的分析结论如下列图所示。每种Option可以得到一张结果图，它的横坐标为λ（图上是Q/T），值越大，代表期望的时间更优（也就是希望更快一些，质量相对较差），值越小，代表期望质量更优（质量要好，时间相对较慢），纵坐标为百分比，代表每个Option Value占用的比例。

![](https://img-blog.csdn.net/20140903100327835?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


![](https://img-blog.csdn.net/20140903100343045?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


![](https://img-blog.csdn.net/20140903100356804?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


![](https://img-blog.csdn.net/20140903100405353?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


![](https://img-blog.csdn.net/20140903100422778?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


![](https://img-blog.csdn.net/20140903100208343?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


![](https://img-blog.csdn.net/20140903100215031?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)





最终得到的各Option的分析结果如下表所示。

![](https://img-blog.csdn.net/20140903101017538?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


最终得到的分析结果如下表所示。下表分别列出了λ在不同取值范围内（取值越小，代表对质量要求高；取值大，代表对速度要求高；当然表中也包含了对应的Time和Bitrate）各个Option最适合的value。

![](https://img-blog.csdn.net/20140903101025697?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)







## 距离凸包（Convex hull）的Preset分析



上一章介绍的“Lambda的Preset分析”方法，集中分析了位于实验数据图表左下方的Preset。这些Preset都属于比较“好”的Preset。要么质量高，要么速度快，要么不但质量高而且速度快。本章介绍的距离凸包的Preset分析和上一章有很大的不同。本章将要衡量所有的Preset。大体上的思想是将Preset分成不同“质量”的Preset：距离凸包近（位于左下方）的Preset属于“优秀生”；距离凸包远（位于右上方）的Preset则属于“差生”（例如位于右上方的Preset，不但质量低，而且速度慢）。最后得到不同的Preset的“质量”下各个Option的value的分布情况。

PS：凸包可以理解为包络线

![](https://img-blog.csdn.net/20140903101456921?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


算法不再细说。直接说一说实验中的图表。根据实验结果图表中各个Preset与凸包的距离，将不同的Preset分成了不同的类别。下图列出了1-10，11-20，21-30，31-40，41-50，51-60这些类别的Preset，并标记成了不同的颜色。绿色的点为不在这些类别中的Preset。

![](https://img-blog.csdn.net/20140903101818861?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)





下图继续列出了62-70，71-80，81-90，91-100，101-110，111-120这些类别的Preset，并标记成了不同的颜色。

![](https://img-blog.csdn.net/20140903101836053?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


下图列出了不同类别的点的个数。可以看出后几个类别点的个数要稍微小一点。

![](https://img-blog.csdn.net/20140903101947860?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


最终分析的结论图如下图所示。每个Option对应一张图表。横坐标为上文所述的根据与凸包的距离而划分的类别，纵坐标为不同的Option value所占的比例。

![](https://img-blog.csdn.net/20140903102026641?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

![](https://img-blog.csdn.net/20140903102037015?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


![](https://img-blog.csdn.net/20140903101821265?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


![](https://img-blog.csdn.net/20140903102050665?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


![](https://img-blog.csdn.net/20140903102100805?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


![](https://img-blog.csdn.net/20140903102106219?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


![](https://img-blog.csdn.net/20140903102110977?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


最终得到的各Option的分析结果如下表所示。

![](https://img-blog.csdn.net/20140903102134421?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)




##  不同视频序列的分析

本部分的主要目的是验证上文所述方法的合理性。上文所述的方法只使用了一个测试视频序列。而实际环境中视频的内容多种多样。因此必须确定本报告提出的方法也适用于其他的视频。否则本报告的研究就没有意义了。下面列出了3个标准测试视频序列的实验结果图：“bus”，“news”，“stefan”。简单描述一下图表的含义：每张图表对应一个测试视频序列的实验结果；除了包含该测试视频序列的实验结果之外，还包含了其他2个测试视频序列位于凸包（convex hull）上的点，并标记以不同的颜色。

从结果上来看，尽管视频的内容不同，它们位于凸包上的点的位置是趋近于一条包络线的。也就是说，“最佳Preset”不太受视频内容的影响（当然还是有一点点的，只是程度不大）。




![](https://img-blog.csdn.net/20140903103205741?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


![](https://img-blog.csdn.net/20140903102947671?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


![](https://img-blog.csdn.net/20140903103215397?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


本报告中使用的是“bus”测试视频序列的实验结果。




## 总结

本报告一共用了4种方法分析x264的选项：
1.使用凸包（convex hull）分析最佳Option。
2.使用“彩云图”分析Option。这种方法简单直观，但是无法精确的量化分析。

3.使用Lambda参数分析Option。这种方法可以分析Option的速度和质量的权衡。

4.使用凸包距离分析Option。这种方法可以分析“好”和“坏”的Option。




原文地址：[http://compression.ru/video/codec_comparison/pdf/x264_options_analysis_08.pdf](http://compression.ru/video/codec_comparison/pdf/x264_options_analysis_08.pdf)













