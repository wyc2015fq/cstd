# 基于HEVC 的UHD（超高清 4K）视频的主观质量评价 - 雷霄骅(leixiaohua1020)的专栏 - CSDN博客





2013年10月20日 17:50:00[雷霄骅](https://me.csdn.net/leixiaohua1020)阅读数：38









Philippe Hanhart等人在论文《Subjective quality evaluation of the upcoming HEVC videocompression standard》中对新一代视频编码标准HEVC（H.265）做了主观质量评价以及客观质量评价试验。在此记录一下他们的实验过程以及结果。



该论文使用的4个测试序列如下表所示，可以看出分辨率都是UHD（超高清 4K）的。

![](https://img-blog.csdn.net/20131019193305718?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

测试序列的内容如下图所示。

![](https://img-blog.csdn.net/20131019193310078?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

分别计算了4个序列的TI（时间复杂度）和SI（空间复杂度），并以散点图的形式统计了一下，如下图所示。

有关TI和SI可以参考：[衡量视频序列特性的TI（时间信息）和SI（空间信息）](http://blog.csdn.net/leixiaohua1020/article/details/9056443)

![](https://img-blog.csdn.net/20131019193317703?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

实验使用的编码器参数设置如下表所示。该实验使用H.264作为进行对比的编码标准。

![](https://img-blog.csdn.net/20131019193321812?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

3个测试序列的码率如下表所示。表3是期望达到的码率。表4是实际的码率。表5是为了达到期望的码率而设置的QP。

对于一种编码标准（HEVC或H.264），每个测试序列设定了R1——R5五个码率。

![](https://img-blog.csdn.net/20131019193326265?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

下图是使用客观质量评价算法PSNR得到的结果。可以看出，同等视频质量的前提下（即同等PSNR的情况下），HEVC的码率要明显低于H.264的码率。但是具体低多少码率则跟序列的内容有关。

![](https://img-blog.csdn.net/20131019193330500?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

下图是主观评价实验会用到的DSIS（双刺激损伤评价法(Double Stimulus Impairment Scale)：看原始图像,再看编码后图像,比较之打分,循环。）的示意图。

![](https://img-blog.csdn.net/20131019193356375?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

下表所示是测试的硬件配置。

![](https://img-blog.csdn.net/20131019193401671?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

下图所示是主观评价实验室的环境，符合ITU的建议。

![](https://img-blog.csdn.net/20131019194826375?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

下图是主观评价实验的结果。感觉主观实验趋势不像客观实验那么“整齐”啊，连出来的“线”比较乱。

![](https://img-blog.csdn.net/20131019194834187?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

这张图乍一看感觉很高深。我也琢磨了半天。现在解释一下。A代表AVC（H.264），H代表HEVC，R1-R5代表五种码率。例如PeopleOnStreet序列的A-R5代表AVC码率为R5的视频序列（查一下发现码率为20Mbps）。纵坐标代表条件A（Condition A），横坐标代表条件B（Condition B）。当处于条件A的序列视频质量比条件B的序列的质量好的时候，图中显示白色。当A比B差的时候，图中为黑色。如果A和B差不多的话，就是灰色了。每个视频序列都会和其他9个视频序列作比较。

![](https://img-blog.csdn.net/20131019194909156?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

下图反映了主观评价的视频质量和客观评价的视频质量之间的关系。横坐标为客观的PSNR，纵坐标为主观的MOS。PeopleOnStreet和Sintel2两个序列总体上还是比较符合规律的。Traffic序列看上去不太符合规律啊。。。

注：PSNR和MOS的对应关系可以参考：[全参考视频质量评价方法（PSNR，SSIM）以及相关数据库](http://blog.csdn.net/leixiaohua1020/article/details/11694369)

![](https://img-blog.csdn.net/20131019194904156?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)



下表列出了同等视频质量的前提下，HEVC相对于H.264节约的码率。

问题的关键是什么算做“同等视频质量”。左边一列使用客观指标PSNR作为衡量视频质量的标准。右边一列使用主观评价的结果MOS作为衡量视频质量的标准。

![](https://img-blog.csdn.net/20131019194958656?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)






