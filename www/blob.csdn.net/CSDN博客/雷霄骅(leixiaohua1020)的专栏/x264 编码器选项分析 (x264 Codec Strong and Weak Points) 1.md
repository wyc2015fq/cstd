# x264 编码器选项分析 (x264 Codec Strong and Weak Points) 1 - 雷霄骅(leixiaohua1020)的专栏 - CSDN博客





2014年09月02日 12:55:38[雷霄骅](https://me.csdn.net/leixiaohua1020)阅读数：6481










文章目录：

[x264 编码器选项分析 (x264 Codec Strong and Weak Points) 1](http://blog.csdn.net/leixiaohua1020/article/details/39004293)


[x264 编码器选项分析 (x264 Codec Strong and Weak Points) 2](http://blog.csdn.net/leixiaohua1020/article/details/39023071)

======================

本文简单翻译了MSU实验室做的X264的Option（即编码选项，后文称其英文名）分析报告《x264 Codec Strong and Weak Points》。看了之后感觉分析得十分透彻，而且其采用的方法也很有参考价值，因此记录一下其中的要点以作备忘。众所周知，X264的Option非常繁多，如何搭配这些繁多的参数以让编码后的视频体积小，质量好，同时编码速度快，确实是一个很让人头疼的问题。本报告中的实验就是为了解决上述问题而设计的。


## 一．简介
本文通过使用客观质量评价算法分析X264编码器的选项。下表列出了文中用到的几个词汇。

![](https://img-blog.csdn.net/20140902115126227?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
简单翻译一下：
|名称|定义|例子|
|----|----|----|
|Option（选项）|编码选项|B帧数，运动估计算法|
|Option value（值）|编码选项的值|--me（运动估计算法）包含以下值：”dia”,  “hex”, “umh”, “esa”, “tesa”|
|Preset（预设）|由一系列固定Value（值）的Option（选项）组成的集合||
|Pareto-optimal Point (Preset) （[帕累托最优](http://baike.baidu.com/view/98065.htm)的预设）|没有任何其他Preset（预设）可以比该Preset获得更好的视频质量和更快的编码速度。||
|Envelope line Points (Preset) （包络线上的预设）|位于Convex hull（[凸包](http://baike.baidu.com/view/707209.htm)）上的预设。代表了最好的预设。||
|Parameter λ（λ参数）|代表期望的编码时间和码率的比值。可以用下式表示：M=λT+Q其中T代表了相对编码时间，Q代表了相对的视频质量。||




## 二．选项（Option）和选项的值（Option value）
本报告分析的X264的Option如下所列。

![](https://img-blog.csdn.net/20140902114919671?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

![](https://img-blog.csdn.net/20140902124055676?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


简单翻译一下：

|选项|值|备注|
|----|----|----|
|Partitions（分块方式）--partitions x|"none""p8x8,b8x8,i8x8,i4x4""all"|宏块使用的分块方式。默认值：  "p8x8,b8x8,i8x8,i4x4"|
|B-Frames（B帧数）--bframes n|024|I帧和P帧之间连续B帧出现的数量。默认值：0|
|Reference Frames（参考帧数）--ref n|148|参考帧的数量。默认值：1|
|Motion Estimation Method（运动估计方法）--me x|“dia”“hex”“umh”“tesa”|运动估计的方法。详细见注释。默认值：“hex”|
|Subpixel Motion Estimation（子像素运动估计）--subme n|1456|子像素(subpixel)估测复杂度。默认值：5|
|Mixed References--mixed-refs|offon|默认值：off|
|WeightedPrediction--weightb|offon|每一帧对B帧的影响力与其和该B帧的距离相关。默认值：off|



|注：1.运动估计方法简介  dia(diamond菱形搜索) 是最简单的搜索方式，从最优预测值出发，往上、左、下、右一个像素处检测运动向量，挑选最好值，然后重复该步骤，直至找不到更优的运动向量。  hex(hexagon正六角形搜索) 的策略类似，但它对周围六个点进行range-2搜索，因此称为正六角形搜索。此方法效率大大高于dia，且速度相当，因此通常编码常用此项。  umh(uneven multi-hex不对称多六角形搜索)比hex慢很多，但能搜索复杂的多六角形，以避免错过很难找到的运动向量。与hex和dia相似，M.E.半径范围参数直接控制umh的搜索半径，使用者可自行增减搜索的空间尺寸。  esa(exhaustive全面搜索) 在最优预测值附近M.E.半径范围范围内的整个空间内，以高度优化的智能方式搜索运动向量。相当于数学上的穷举法，搜索区域内的每一个运动向量，但是更快些。然而，此方法远远慢于UMH，且好处不多，对于普通编码没有太大用处。  tesa (transformed exhaustive变换全面搜索)算法尝试对各个运动向量近似哈达玛变换比较法。与exhaustive类似，但效果略好，速度略慢。2. 子像素运动估计子像素(subpixel)估测复杂度，越大越好。数值1-5单纯控制子像素细化强度。数值6会开启模式决策RDO，数值8将开启运动向量和内部预测模式RDO。RDO模式大幅慢于低级模式。采用低于2的值，会使用一种较快、但较低质量的lookahead模式，同时会影响--scenecut的决策，因此不推荐。可选值：0. fullpel only1. QPel SAD 1 iteration2. QPel SATD 2 iterations3. HPel on MB then QPel4. Always QPel5. Multi QPel + bi-directional motion estimation6. RD on I/P frames7. RD on all frames8. RD refinement on I/P frames9. RD refinement on all frames10. QP-RD (requires --trellis=2, --aq-mode > 0)11. Full RD|
|----|


## 三．最佳Preset
下图显示了本次实验的全部结果。实验系统通过枚举上表所列参数的所有选项，而后进行压缩编码得到的实验结果。每一个绿色的点代表一种Preset压缩的结果，可见实验的数据量很大。

![](https://img-blog.csdn.net/20140902124207795?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


下面简单解释一下这张图的含义。纵坐标代表视频的码率，取值越低，代表同等视频质量的的情况下码率越小。横坐标代表视频的编码时间，取值越低，代表编码时间越低。坐标取值是一个相对值。每一个绿色的点代表一种Preset。因此可知，越是位于左下角的Preset，其编码速度越快，且码率越低。最优的Preset应该是位于凸包（Convex hull）上的点（即那条红颜色线上的点）。需要注意的是，横坐标和纵坐标都是一个相对值而不是绝对的码率和时间。横坐标和纵坐标的值都是相比于X264的Default Preset而言的。X264的Default Preset即X264全部使用默认值的Preset，位于这张图的（1,1）点处。

如果固定编码时间，则最优的Preset位于包络线上。如图所示，位于粉红色五角星位置的Preset在相同的编码时间下码率最低。

![](https://img-blog.csdn.net/20140902124014500?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


X264默认值如下表所示。


|Option|默认值|
|----|----|
|Partitions|“p8x8,b8x8,i8x8,i4x4”|
|B-Frames|0|
|Reference Frames|1|
|Motion Estimation Method|“hex”|
|Subpixel Motion Estimation|5|
|Mixed References|off|
|Weighted Prediction|off|





下图显示了Pareto-optimal Point（帕累托最优点）Preset（即没有任何其他Preset可以比该Preset获得更好的视频质量和更快的编码速度）以及凸包（Convex hull）上的Preset。

![](https://img-blog.csdn.net/20140902124140812?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

凸包（Convex hull）上的Preset数据统计如下。

![](https://img-blog.csdn.net/20140902124204234?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

凸包（Convex hull）上的Preset分析结果如下。表格中一方面列出了凸包（Convex hull）上的Preset使用较多的Option以及使用较少的Option。另一方面列出了消耗时间较长但是质量较高的Preset以及速度较快但是质量较差一些的Preset。


![](https://img-blog.csdn.net/20140902124302015?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


## 四．“彩云图”的Preset分析
本章中，每张图对应一种关注的Option，包含不同Option Value的 Preset被标记为不同的颜色。实验结果如下所示。

![](https://img-blog.csdn.net/20140902124344515?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

![](https://img-blog.csdn.net/20140902124401406?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


![](https://img-blog.csdn.net/20140902124637847?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


![](https://img-blog.csdn.net/20140902124654727?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


![](https://img-blog.csdn.net/20140902124711715?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


![](https://img-blog.csdn.net/20140902124724725?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


![](https://img-blog.csdn.net/20140902124739093?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)




从以上图表可以得出的各个Option的分析结果如下表所示。

![](https://img-blog.csdn.net/20140902124801526?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
简单翻译一下：
|选项|值|结论|
|----|----|----|
|Partitions（分块方式）--partitions x|"none""p8x8,b8x8,i8x8,i4x4""all"|要求速度快的时候用”none”，要求视频质量高的时候用”all”。要求速度质量均衡考虑的时候使用"p8x8,b8x8,i8x8,i4x4"|
|B-Frames（B帧数）--bframes n|024|要求速度极高的时候用”0”。其他情况下用”2”，”4”，它们二者之间区别不大。|
|Reference Frames（参考帧数）--ref n|148|要求速度快的时候用”1”，要求视频质量极高的时候用”8”。要求速度质量均衡考虑的时候使用"4"|
|Motion Estimation Method（运动估计方法）--me x|“dia”“hex”“umh”“tesa”|要求速度快的时候用”dia”和”hex”，要求视频质量极高的时候用”tesa”。要求速度质量均衡考虑的时候使用"umh"。|
|Subpixel Motion Estimation（子像素运动估计）--subme n|1456|要求速度快的时候用”1”，要求视频质量高的时候用”6”。要求速度质量均衡考虑的时候使用"4"|
|Mixed References--mixed-refs|offon||
|WeightedPrediction--weightb|offon||


注1：“Weighted prediction”作用不明显
注2:“bframes”中的2和 4 B-frames差别不大。

“彩云图”的分析结论如下表所示。该表列出了不同的使用环境下Option应该使用的值（Option value）。分成三种情况：速度最重要，速度和视频质量平均考虑，视频质量最重要。

![](https://img-blog.csdn.net/20140902124919464?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


原文地址：

[http://compression.ru/video/codec_comparison/pdf/x264_options_analysis_08.pdf](http://compression.ru/video/codec_comparison/pdf/x264_options_analysis_08.pdf)




