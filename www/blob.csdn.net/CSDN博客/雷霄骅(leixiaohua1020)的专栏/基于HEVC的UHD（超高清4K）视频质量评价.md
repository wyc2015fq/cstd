# 基于HEVC的UHD（超高清4K）视频质量评价 - 雷霄骅(leixiaohua1020)的专栏 - CSDN博客





2013年10月19日 19:10:00[雷霄骅](https://me.csdn.net/leixiaohua1020)阅读数：131









Sung-Ho Bae等人在论文《Assessments of Subjective Video Quality onHEVC-Encoded 4K-UHD Video forBeyond-HDTV Broadcasting Services》中，对基于HEVC编码的4K超高清视频序列进行了主观质量评价和客观质量评价。这在目前来说还是比较超前的。在此记录一下其实验过程以及实验结论。



**选择序列的过程**

选择序列的过程如下图所示。一共对36个测试序列进行筛选。计算Ct（时间复杂度）以及Cs（空间复杂度）。以Cs为横坐标，Ct为纵坐标，散点图如下图所示。

空心的点代表没有被选中的序列。被选中的序列标记成了红点。注意，该图被划分为9个区域，分别表示Ct和Cs的高，中，低（H，M，L），该9个区域每个区域选择一个序列。

![](https://img-blog.csdn.net/20131018231039125?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

选择的序列的Cs（空间复杂度）以及Ct（时间复杂度）如图所示。

![](https://img-blog.csdn.net/20131018230250203?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

选择的序列的内容如图所示。

![](https://img-blog.csdn.net/20131018230305828?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)



CU和TU的四叉树结构如下图所示。

![](https://img-blog.csdn.net/20131018230401625?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

编码器配置如下表所示。

![](https://img-blog.csdn.net/20131018230748859?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

预期设定的比特率和实际比特率之间的比较如下表所示。每个序列期望达到18，23，36Mbps三个比特率。通过设定不同的QP，以达到相应的目标比特率。需要注意的是，根据序列内容的不同，设定的QP是不一样的。

![](https://img-blog.csdn.net/20131018230834546?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

下图（a）反映了不同码率的情况下，视频平均PSNR的大小。

（b）反映了不同码率下，同时又是不同Cs（空间复杂度），视频平均PSNR的大小。

（c）反映了不同码率下，同时又是不同Ct（时间复杂度），视频平均PSNR的大小。

![](https://img-blog.csdn.net/20131018230551984?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

估算编码23Mbps的视频序列需要的QP值是通过下图所示的方法。如图所示，横坐标为QP，纵坐标为码率。已知QP为21的时候，码率大概为38Mbps，QP为24的时候，码率大概为17Mbps。则大致可以估计出，如果想要码率达到23Mbps，QP值应该取23（这两个数竟然一样，实在是一个巧合）

![](https://img-blog.csdn.net/20131018230718703?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

DSIS（双刺激损伤评价法(Double Stimulus Impairment Scale):看原始图像,再看编码后图像,比较之打分,循环。）如下图所示。

![](https://img-blog.csdn.net/20131018230731281?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

4组主观评价实验的测试序列参数如下表所示。

![](https://img-blog.csdn.net/20131019003613453?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

4组实验的数据量太大了，这里不一一说明，只列出几张结果图。

第一组实验实验结果如图所示。注意第一组实验观察距离是0.75H。（a）图是各种序列的平均值。（b）图分为三种：低Cs，中Cs，高Cs。（c）图分为三种：低Ct，中Ct，高Ct。

![](https://img-blog.csdn.net/20131019005340750?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

第二组实验实验结果如图所示。注意第二组实验观察距离是1.5H。依然是三个图。（a）图是各种序列的平均值。（b）图分为三种：低Cs，中Cs，高Cs。（c）图分为三种：低Ct，中Ct，高Ct。

![](https://img-blog.csdn.net/20131019005344828?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

第一组实验数据和第二组实验数据之间的比较。测试序列数量还是很多的。注意第一组实验观察距离是0.75H，第二组实验观察距离是1.5H。

整体来说差别不是很大，但是离近些的话，会更容易感受到视频质量的下降。画红色圈的序列表现比较明显。

![](https://img-blog.csdn.net/20131019005349468?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

第三组实验数据和第四组实验数据之间的比较。受损YUV420序列与受损YUV444序列之间的比较。第三组实验和第四组实验都是使用YUV444序列进行编码的。第三组实验解码后的受损序列是YUV420格式的。第四组实验解码后的受损序列是YUV444格式的。

使用YUV444格式编码的话，编码UV数据需要相对比较大的数据量。因而在码率相同的情况下，使用YUV420格式编码的时候Y的数据量会比YUV444大一些。从实验结果来看，YUV420序列的质量要好于YUV444。尤其在序列“Chuno−LM 18，23Mbps”，以及“MapleTree−HL 18,23,36Mbps”序列上表现明显。

![](https://img-blog.csdn.net/20131019005355468?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

第二组实验数据和第四组实验数据之间的比较。第二组实验数据是YUV420编码后解码得到YUV420数据。第四组实验数据是YUV444编码后解码得到YUV444数据。

![](https://img-blog.csdn.net/20131019005402015?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)



论文地址：[http://ieeexplore.ieee.org/xpl/articleDetails.jsp?reload=true&arnumber=6480797](http://ieeexplore.ieee.org/xpl/articleDetails.jsp?reload=true&arnumber=6480797)






