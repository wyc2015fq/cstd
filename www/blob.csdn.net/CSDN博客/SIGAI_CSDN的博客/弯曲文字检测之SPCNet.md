
# 弯曲文字检测之SPCNet - SIGAI_CSDN的博客 - CSDN博客
# [SIGAI_CSDN的博客](https://blog.csdn.net/sigai_csdn)


[博客首页](https://blog.csdn.net/SIGAI_CSDN)
[关于我们](https://me.csdn.net/SIGAI_CSDN)

2019年01月21日 15:18:58[SIGAI_csdn](https://me.csdn.net/SIGAI_CSDN)阅读数：434


获取全文PDF请查看：[弯曲文字检测之SPCNet](http://www.sigai.cn/paper_81.html)
![](https://img-blog.csdnimg.cn/20190121151535771.png)
一、简介
文字检测在深度学习的推动下，最近几年取得了长足的进步。由于多媒体检索，工业自动化，视力障碍人士辅助设备等应用的需求日益增长，场景文本检测是的计算机视觉的热门研究话题之一。给定一张自然场景图像，定位出图中的所有文字的位置，即场景文本检测的目标。过去的大多数文章主要集中于检测水平文字和倾斜文字，主要数据集也是基于水平文字和倾斜文字的，然而，自然场景中大量存在弯曲文本。因此本文提出一种方法，不仅可以检测水平和多方向文字，同时可以检测弯曲文字，对各种形状的文字检测都十分鲁棒。同时，本文还提出两个模块，可以有效抑制错误样本的检测。该文章被AAAI2019收录。
本文的主要贡献：
1 提出Text Context 模块(TCM) 和 Re-Score 模块(RS)，可以抑制错误样本检测。
2 所提出的方法可以有效检测任意形状的文字，包括水平，倾斜和弯曲的。3 该方法在几个基准数据集上均超越目前最好的算法，包括水平文字数据集，多方向，多语言和弯曲文字数据集。
二、related work
下面简单介绍几个文字检测的重要工作，这些算法在水平和倾斜文字上取得了不错的效果，也有一些尝试在弯曲文字上做实验，也是本文主要对比的一些算法。
1 PixelLink
该方法基于语义分割，通过分割文字区域得到文本位置，同时通过预测每个文字像素的上下左右的连接关系，从而区分挨得很近的文本实例。
2 EAST
该方法主要思路是传承DenseBox和UnitBox,是基于FCN直接做分割，同时对每个像素点去回归到边界框的上下左右距离和旋转角度。方法简单效果又好，是一个在速度和精度方面表现都很出色的算法。
3 RRPN
该方法基于Faster RNN做改进，通过引入一系列旋转的候选框，从而可以更好的匹配旋转文字，缺点在于引入成倍的anchor导致速度会大幅度下降。
4 TextSnake
该方法是第一个做弯曲文字检测的，基于语义分割，通过引入一系列圆盘和文字中心线对弯曲文字进行建模，可以灵活的检测弯曲文字。但是该方法后处理较为复杂。
不难看出，上述方法大多是基于语义分割和目标检测算法，并做了一些改进，基于语义分割的算法打多需要较为复杂的后处理，但是相对可以灵活检测弯曲文字。
三、算法框架
为了解决弯曲文字检测问题，作者通过引入Mask RCNN作为基准模型应用于文字检测，为什么要用Mask RCNN呢？ Mask RCNN是Facebook提出的实例分割框架，在COCO比赛中取得了冠军，可以准确检测和分割物体。同时，为了缓解文字检测中常见的FP问题，即错误样本检测的问题，作者还提出了Text Context 模块 和 Re-Score 模块。
网络流程图如下：
![](https://img-blog.csdnimg.cn/20190121151601470.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1NJR0FJX0NTRE4=,size_16,color_FFFFFF,t_70)
可以看到，(a)是标准的FPN结构，(b)是Text Context 模块，通过(b)得到融合后的特征。(c)是标准的Mask RCNN的三个分支(d)是作者引入的语义分割分支，去分割整张图的文字。
为什么要引入语义分割分支？
语义分割和实例分割是两个相辅相成的任务，多任务学习可以使得所有任务都得到提高，不仅如此，语义分割的中间结果可以和实例分割的特征做融合，从而降低文字错误检测的概率。
TCM模块
下图可以看到，FPN得到的每一个stage的特征用蓝色表示，经过上面一支的三个卷积可以直接预测文字分割结果，而1x1 卷积之后的分割图可以作为attention乘回原图，去增强检测特征中的文字区域，并且中间产生的特征可以相加得到绿色特征，从而将检测的特征和分割的特征融合起来，得到更为丰富的特征表达。
![](https://img-blog.csdnimg.cn/20190121151618147.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1NJR0FJX0NTRE4=,size_16,color_FFFFFF,t_70)
RS模块
首先为什么要引入RS模块？ 可以思考一下，Mask RCNN将文字检测的框的分类分数作为最终的分数，通过设置一个阈值去过滤掉分数较低的框。这样会引入一个问题，即对于倾斜文字或者弯曲文字，水平框内的文字区域占的比例较低，而大部分都是背景，从而会导致分类分数较低，但是在语义分割图上它们具有较高的响应。而一些错误样本具有较高的检测得分，但是在语义分割图上的响应较低。因此作者考虑将语义分割和实例分割的结果相融合，通过重新计算合理的分数，从而降低错误样本检测。
下图是一个例子，可以看到第一行的文字的分类分数是0.7，由于在语义分割分支上的响应较强，因此通过RS之后分数变为0.75。第二行是一个错误样本，分类分数很高，是0.78，但是语义分割分支上响应较弱，经过RS分数降低为0.39，因此后处理很容易被过滤掉。
![](https://img-blog.csdnimg.cn/20190121151637914.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1NJR0FJX0NTRE4=,size_16,color_FFFFFF,t_70)
这块是重打分的一个可视化说明，绿色的是水平box，红色的是instance seg的结果，下面的是全局语义分割的结果，作者把instance seg的结果投影到语义分割图上，并在该区域内算一个响应值，得到instance score,并和原来的classification score融合得到最终的score。
![](https://img-blog.csdnimg.cn/20190121151657158.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1NJR0FJX0NTRE4=,size_16,color_FFFFFF,t_70)
具体计算公式如下：
![s_{i}=\frac{e(s_{i1}^{cs}+s_{i1}^{is})}{e(s_{i1}^{cs}+s_{i1}^{is})+e(s_{i0}^{cs}+s_{i0}^{is})}](https://private.codecogs.com/gif.latex?s_%7Bi%7D%3D%5Cfrac%7Be%28s_%7Bi1%7D%5E%7Bcs%7D&plus;s_%7Bi1%7D%5E%7Bis%7D%29%7D%7Be%28s_%7Bi1%7D%5E%7Bcs%7D&plus;s_%7Bi1%7D%5E%7Bis%7D%29&plus;e%28s_%7Bi0%7D%5E%7Bcs%7D&plus;s_%7Bi0%7D%5E%7Bis%7D%29%7D)
Si是最终的分数，cs对应的是分类分数，is对应的是实例分数，最终分数是这两个分数通过softmax函数求得。
其中实例分数是实例分割结果投影到语义分割图上并计算投影区域内的像素均值得到。
具体计算公式如下。
![s_{i1}^{cs}=\frac{\sum _{j}p_{i}^{j}}{N}](https://private.codecogs.com/gif.latex?s_%7Bi1%7D%5E%7Bcs%7D%3D%5Cfrac%7B%5Csum%20_%7Bj%7Dp_%7Bi%7D%5E%7Bj%7D%7D%7BN%7D)
实验部分
1 标签如何生成？
![](https://img-blog.csdnimg.cn/20190121152208929.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1NJR0FJX0NTRE4=,size_16,color_FFFFFF,t_70)
可以看到对于不同的数据集，作者简单的用黄色代表实例分割的标注，绿色代表水平框的标注，同时右侧的图代表语义分割的标注。
2 Ablation Study
下图是Ablation Study的结果，在ICDAR2017的验证集上做了实验，可以看到基于Mask RCNN的baseline，发现TCM和RS分别能提高模型的最终结果。在recall保持一致的情况下precision涨点，说明能有效抑制FP。
![](https://img-blog.csdnimg.cn/20190121152145671.png)
下图是在多个基准数据集上和主流方法做对比，可以看到SCPNet都领先于目前主流的方法。
![](https://img-blog.csdnimg.cn/20190121152117504.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1NJR0FJX0NTRE4=,size_16,color_FFFFFF,t_70)
![](https://img-blog.csdnimg.cn/20190121152059224.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1NJR0FJX0NTRE4=,size_16,color_FFFFFF,t_70)
![](https://img-blog.csdnimg.cn/20190121152040350.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1NJR0FJX0NTRE4=,size_16,color_FFFFFF,t_70)
下图做了一个可视化结果分析，在ICDAR2015上，作者分别对比了EAST，RRPN和TextBoxes++，发现作者的方法相比它们的确能有效减少FP的检测。所有数据都可以在ICDAR官网看到。
![](https://img-blog.csdnimg.cn/2019012115202167.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1NJR0FJX0NTRE4=,size_16,color_FFFFFF,t_70)
最后放上作者在四个数据集上的检测可视化效果图，可以看出SPCNet的确可以灵活的检测任意形状的文字检测。
![](https://img-blog.csdnimg.cn/20190121152000338.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1NJR0FJX0NTRE4=,size_16,color_FFFFFF,t_70)
总结与展望
本文首先将Mask RCNN用于文字检测，并实现弯曲文字检测，通过RS和TCM有效抑制FP的检测，并取得了目前最好的结果。
未来是否可以考虑实现实时性文字检测？因为目前工业界对手机端等的算力受限场景需求很大，单纯用Mask RCNN不能满足实时性的检测，可以考虑从backbone, head等方面入手。
reference：
[1]Pixellink:Detecting scene text via instance segmentation. In AAAI2018
[2]Arbitrary-oriented scene text detection via rotation proposals. IEEE Transactions on Multimedia 2018.
[3]East: an efficient and accurate scene text detector. In CVPR 2017.
[4]Textsnake: A flexible representation for detecting text of arbitrary shapes. ECCV2018.


