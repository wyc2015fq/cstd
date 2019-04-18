# SNIPPER 算法笔记 - AI之路 - CSDN博客





2018年09月01日 09:12:55[AI之路](https://me.csdn.net/u014380165)阅读数：1580








论文：SNIPER: Efficient Multi-Scale Training 

论文链接：[https://arxiv.org/abs/1805.09300](https://arxiv.org/abs/1805.09300)

代码链接：[https://github.com/MahyarNajibi/SNIPER](https://github.com/MahyarNajibi/SNIPER)
**这篇文章是之前[SNIP](https://blog.csdn.net/u014380165/article/details/80793334)论文的升级，升级点在于减少SNIP的计算量**。SNIP借鉴了multi-scale training的思想进行训练，multi-scale training是用图像金字塔作为模型的输入，这种做法虽然能够提高模型效果，但是计算量的增加也非常明显，因为模型需要处理每个scale图像的每个像素，而SNIPER（Scale Normalization for Image Pyramids with Efficient Resampling）算法通过引入context-regions（文中用chips表示，chip翻译过来就是碎片的意思，因为是从一张完整的图像中裁剪出多个chip，所以这个用词也比较形象），使得模型不再处理每个尺寸图像的每个像素点，而是选择那些对检测最有帮助的区域参与训练，这样就大大减少了计算量。这些chips主要分为两大类，一种是postivice chips，这些chips包含ground truth；另一种是从RPN网络输出的ROI抽样得到的negative chips，这些chips相当于是难分类的背景，而那些容易分类的背景就没必要进行多尺度训练了。因此模型最终只处理这些chips，而不是处理整张图像，这样就达到提升效果的同时提升速度（在单卡V100上，每秒能处理5张图像，这速度在two-stage类型的目标检测算法中其实并不算快，毕竟是基于多尺度图像训练，但是效果是真的好）。

因此这篇文章的核心内容就是关于如何选择positive chip和negative chip。

**positive chip选择的思想是希望一个chip中尽可能包含合适尺度的ground truth box**。假设有n个scale，这n个scale用{s1, s2,…, si,…sn}表示，Ci表示每张图像在尺度i时得到的chip集合。另外用Cipos表示positive chip集合，Cineg表示negative chip集合。假设一个区域范围Ri = [rimin, rimax], 其中i的范围是[1, n]，表示scale，Ri表示对于尺度i而言，哪些尺寸范围的ground truth box才能被选进chip，在Ri范围内的ground truth boxe集合用gi表示，每个chip都希望能尽可能多的包含ground truth boxe，而且只有当某个ground truth box完全在一个chip中才说明该chip包含了该ground truth box，这样得到的尺度i的positive chip集合就是Cipos。最终每个ground truth box就能以一个合适的尺度存在于chip中，这样就大大减少了模型对背景区域的处理。

**Figure1是关于SNIPER的positive chip选择过程**。左边图中的绿色实线框表示ground truth，各种颜色的虚线框（一共4个）表示SNIPER算法基于原图生成的chips，这些chips包含了所有ground truth。右边图是关于这4个chips的具体内容，其中绿色实线框表示对于该chip而言有效的ground truth，红色实线框表示对该chip而言无效的ground truth。因为不同scale图像的Ri范围有重叠，所以一个ground truth box可能属于不同scale的多个chip中，比如Figure1中电视机既出现在finest scale也出现在coarsest scale中。而且同一个ground truth box在同一个scale中也可能属于多个chip。 
![这里写图片描述](https://img-blog.csdn.net/20180901091153495?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTQzODAxNjU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
**如果只基于前面的positive chip，那么因为大量的背景区域没有参与训练，所以容易误检（比较高的false positive rate），传统的multi scale训练方式因为有大量的背景区域参与计算，所以误检率没那么高，但因为大部分背景区域都是非常容易分类的，所以这部分计算量是可以避免的，于是就有了negative chip seleciton**。接下来问题就转变成怎么判断哪些背景容易分类，哪些背景不容易分类呢？作者采取了一种相对简单的方法，那就是基于RPN网络的输出，也就是region proposal（也叫ROI）来构建negative chip。我们知道在Faster RCNN系列算法中的RPN网络是用来生成region proposal的，这些region proposal表示最有可能存在object的区域，因为这些region proposal是通过RPN网络粗筛选得到的。假如某个背景区域连region proposal都没有，那说明这个背景区域是非常容易被成功分类的，这些区域就没有必要参与训练了，真正参与训练的negative proposal都是容易误判的proposal，这样就能减少false positive rate。这就是这篇文章关于如何选择negative chip的思想。

**Figure2是关于SNIPER的negative chip选择过程，第一行是输入图像和ground truth信息**。第二行图像中的红色小圆点表示没有被positive chips（Cipos）包含的negative proposals，因为proposals较多，用框画出来的话比较繁杂，所以用红色小圆点表示。橘色框表示基于这些negative proposals生成的negative chips，也就是Cineg。每个negative chip是这么得到的：对于尺度i而言，首先移除包含在Cipos的region proposal，然后在Ri范围内，每个chip都至少选择M个proposal。在训练模型时，每一张图像的每个epoch都处理固定数量的negative chip，这些固定数量的negative chip就是从所有scale的negative chip中抽样得到的。 
![这里写图片描述](https://img-blog.csdn.net/20180901091217860?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTQzODAxNjU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
**实验结果：**

Table2是关于是否有negative chip以及scale数量对实验结果的影响。因为AP值的计算和误检相关，而没有negative chip参与训练时容易产生误检，所以AP值相对较低（表格第1行和第3行的对比）。因为文中的实验默认scale数量是3，所以为了测试scale数量对结果的影响，作者去掉最大尺寸的scale，保留其他2个scale进行训练，结果显示AP下降明显（表格第1行和第2行的对比）。 
![这里写图片描述](https://img-blog.csdn.net/20180901091234253?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTQzODAxNjU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
Table3是关于SNIPER算法和其他算法的对比，除了最后两行是实例分割的结果外，剩下都是检测的结果对比。 
![这里写图片描述](https://img-blog.csdn.net/20180901091245675?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTQzODAxNjU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)







