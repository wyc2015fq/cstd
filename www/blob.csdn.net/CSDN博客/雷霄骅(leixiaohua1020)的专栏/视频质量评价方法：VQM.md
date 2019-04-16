# 视频质量评价方法：VQM - 雷霄骅(leixiaohua1020)的专栏 - CSDN博客





2013年10月13日 21:24:00[雷霄骅](https://me.csdn.net/leixiaohua1020)阅读数：33
个人分类：[视频质量评价](https://blog.csdn.net/leixiaohua1020/article/category/1446693)










如何确定一个视频质量的好坏一直以来都是个棘手的问题。目前常用的方法就是通过人眼来直接观看，但是由于人眼的主观性及观看人员的单体差异性，对于同样的视频质量，不同的人的感受是不一样的。为此多个研究机构提出了视频质量客观测试方法，即利用机器或程序来评价视频质量的方法。本文正是要介绍一下其中的一种方法：VQM，并且总结了一些有关VQM视频质量评价方法的资料。VQM方法给出的客观分数比较符合主观感受，属于比较好的一种质量评价算法。

****

**各种视频质量评价方法对比：**

目前最常见的客观测试标准是PSNR（峰值信噪比）测试算法，但是其测试结果与主观评价结果的一致性较差，不能完全反应人眼的主观感觉。因此有多个机构提出了与主观评价结果一致性较好的客观评价算法，如美国国家电信和信息管理局（NTIA）提出的视频质量度量（VQM）算法及其模型、英国电信BT提出的全参考视频质量模型及其算法（BTFR）、巴西电信发展研究中心（CPqD）提出的基于分段的图像评价（IES）算法，以及泰克公司和Sarnoff公司提供的PQR算法等，这些客观算法都是在模拟人眼视觉模型的基础上建立的。

为验证这些客观测试标准与主观评价结果的一致性，视频质量专家组（VQEG）在2003年进行的第二次视频质量专题测试中，对上述客观算法与主观算法的关联性进行了验证测试，测试结果如表1和表2所示，可以发现，NTIA提供的VQM客观视频质量评价方法对于PAL制及N视频格式都有较好的表现，与主观测试结果的一致性较好。

![](https://img-blog.csdn.net/20131013210101828?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

表1 PAL制式下客观评价与主观评价结果关联性



![](https://img-blog.csdn.net/20131013210107406?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)



表2 NTSC制式下客观评价与主观评价结果关联性

VQM 客观测试标准是利用统计学原理模拟实际的人眼视觉系统，在提取参考及其对应的测试图像中人眼能够感知的图像特征值（亮度、色彩、时空变化等信息）的基础上，计算得出视频质量客观评价值，该评价值为人眼可感知的模糊、块失真、不均匀/不自然的运动、噪音和错误块等损伤的综合反映，其值位于0~1之间，值越小越好。


**VQM的具体处理过程**

如图1所示：



![](https://img-blog.csdn.net/20131013210220421?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)



1. 参考及测试视频流的采集保存及模拟/数字信号转换；
 2. 测试视频流的校准，依据参考视频，去除测试视频中的时间偏移、空间偏移、增益等非编解码引入的变化；
 3. 提取参考及测试视频图像中人眼可感知的特征：模糊、块失真、不均匀/不自然的运动、噪音和错误块等信息；
 4. 对于步骤3提取的每一种特征值进行一定的差分比较计算，依据视频测试序列的长短进行统计平均；
 5. 使用VQM合并计算公式合并步骤4计算的每种特征值的统计平均值，给出最终的视频质量客观评价值（VQM），最终计算获得的VQM值位于0到1之间，数值越小代表视频质量越好。

以上文字出处：[http://www.h3c.com.cn/About_H3C/Company_Publication/IP_Lh/2010/10/Home/Catalog/201006/679125_30008_0.htm](http://www.h3c.com.cn/About_H3C/Company_Publication/IP_Lh/2010/10/Home/Catalog/201006/679125_30008_0.htm)



**VQM客观和主观之间的对应关系**

Yubing Wang在《Survey of Objective Video Quality Measurements》文章中给出了VQM客观分数与主观质量（DMOS）之间的对应关系：

![](https://img-blog.csdn.net/20131013210822937?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

> 
注：该图是Wolf和Pinson在1992-1999年期间11次实验数据的合集。

主观感受单位是DMOS，取值在0-1之间，数值越小，代表主观感受越好。


由图可见，VQM客观分数和人眼主观感受（DMOS）之间是线性关系的。




