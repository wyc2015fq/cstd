# DIY发现新行星操作指南 | 谷歌开源了行星发现代码 - 知乎
# 



> 原作 Chris Shallue
Root 编译自 Google Research Blog
量子位 出品 | 公众号 QbitAI

去年12月，谷歌大脑用机器学习发现了两个系外行星，分别是开普勒80 g和开普勒90 i。

开普勒90 i还是颗类地行星诶！
![](https://pic3.zhimg.com/v2-2875050bdae37a7ddc8447adcd048a06_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='750' height='422'></svg>)因为这次新系外行星的发现，开普勒90有了第8颗行星
这还只是在分析了670个恒星的数据之后的收获。而已有观察数据的恒星有20万个。

能不能从中刨出更多新行星，尤其是类地行星，得靠更多的人。

所以，谷歌大脑研究员**Chris Shallue今天开源了发现新行星的AstroNet算法**。

借这次开源的机会，谷歌大脑顺便给大家讲讲该算法模型的原理。

## 发现新行星的思路

开普勒望远镜，会收集恒星亮度变化的数据。

当一颗行星经过恒星前方时，它会暂时阻挡一些光线，导致开普勒光度计测的亮度下降。
![](https://pic1.zhimg.com/v2-a4d2061b3a3d8f007531187cf8bcfe4c_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='949' height='532'></svg>)
等行星绕远之后，亮度会再爬升回原来的水平。所以从亮度变化的数据图上，我们可以看到一个“U形”凹陷。

不过，恒星亮度降低也还有其他的可能。比如说双恒星，恒星斑，开普勒光度计被宇宙射线干扰了或仪器本身的噪声。
![](https://pic4.zhimg.com/v2-b249daf2630b862cbf5a728fe49a1fdb_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='448'></svg>)
光度曲线有“V形”的话，意味着一个非常大的物体（即另一颗恒星）在开普勒观测的恒星前面经过；如果光度曲线有两次亮度下降的话，那么说明这是一个较亮和一颗较淡的双恒星系统，较大的亮度下降，是由较暗的那颗恒星跑到较亮的那颗前面去了，反之则反；最下方那个光度曲线的变化原因不详。

**本来，从开普勒数据库里找行星的办法是这样的↓**

天文学家们用软件，自动批量处理开普勒数据，寻找光度曲线中的U形信号。

只有那些信噪比超过一定的阈值，是值得后续分析的信号，他们才会手动跟踪，再确认筛选出来的信号是不是行星。

而低于阈值的所有信号都扔掉。

不过即使设置了阈值，再次确认的工作量还是很庞大。到目前为止，手动判断的信号超过30000个，但只有大概2500个才是行星。

也就是说，每12个已被软件筛选过的信号里，只有一个是天文学家要找的行星。

还有另外一种情况，有一些真正行星的信号被软件的阈值给筛掉了。这相当于得到了假阴性的结果。

完全有可能存在着一些行星，是在软件筛选阈值以下的。比如地球，因为围绕着相对较暗的恒星，就低于阈值。

## 试试机器学习大法？

开普勒近四年来观察了约20万颗恒星，每30分钟拍摄一张照片，得到了140亿个数据。这就意味着有大约2万亿个可能的行星！即使用当前最强的算力去分析这些信息，也很费时费力。

为了加快这个过程，提高效率，谷歌大脑尝试用机器学习，分析那些还没处理过的开普勒数据。

与德州大学奥斯丁分校的Andrew Vanderburg合作，谷歌大脑研发了发现新行星的神经网络AstroNet。
![](https://pic1.zhimg.com/v2-f141294aa0b58003d06514b4feecbfac_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='426'></svg>)
通过训练CNN，预测给定的开普勒信号是不是行星。之所以选CNN，是因为它们在其他空间、时间结构类的问题上表现不错，比如说，音频生成，图像分类等。

目前，有3万个开普勒数据是已经被人类手动处理过的，并有最终的判断结果，这等于说已经有了标注好的训练集。

谷歌拿了其中的一半数据作训练集，其中有约3500个是经过验证的行星或强烈的行星候选者，来训练AstroNet神经网络，怎么区分行星。

AstroNet模型的输入数据是同一光度曲线的两个独立视图：一个宽视图，一个放大视图。

宽视图检查光度曲线上是否有除了U形以外的其他信号，像前面提到过的，双恒星系统就有第二次光度降低；放大视图，可仔细检测信号的形状，看是U形还是V形。

训练完成之后，谷歌开始研究哪些光度曲线的特征重度影响AstroNet算法最后的判断结果。

由此来看，这些特征与之前讨论的行星发现原理是否相符。

方法就是，遮挡住输入数据光度曲线的一小部分区域，看输出结果会不会变。

那些对模型决策特别重要的区域，如果它们被遮挡的话，则算法会认为，这个信号是行星的可能性大大减小。

而遮挡不重要的区域就不会产生显著的影响。

以下，是AstroNet准确预测出该光度曲线对应的是双恒星，而不是行星。

绿点，指的是这些点被遮挡时，预测结果会反转。

比如说，一个双恒星系统的第二个U形被遮挡时，AstroNet会改变原有预测结果：原先，认为这个是行星的概率是0％，被遮挡之后，其行星概率飙升到40％。

那么就可以说明，这部分的绿点，是AstroNet否认这条光度曲线对应的星是行星的充分不必要依据。

当然，如果光度下降部分，放大之后实际上是个“V形”，这也表明这是个双恒星，而不是行星。
![](https://pic1.zhimg.com/v2-4647c6bad511f73b35a7f4302bb804d8_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='320'></svg>)
## AstroNet立功，找到之前被漏掉的行星

在确认了AstroNet的行星判断依据和谷歌之前讨论的原理一致之后，谷歌就可以放心地用AstroNet了。

为了测试AstroNet的效果，谷歌用一组670颗恒星的数据跑一遍，看能不能发现被之前方法阈值错漏掉的行星。

选这些恒星，是因为已知这些恒星有多个行星了。也许，这些恒星还有些尚未被发现的行星。

谷歌大脑把搜索条件放宽，比之前天文学家设置的信噪比阈值要更低。然后才发现了开普勒90 i和开普勒80 g。

## 泥萌要不要自己动手试试？

来，我们走一遍开普勒-90 i发现的过程。

回头泥萌可以参考这个找新的。

第一步，是按照代码主页上的说明来训练模型。

从开普勒望远镜下载和处理数据会花一段时间，这之后模型的训练、新数据的预测速度会相对较快。

使用一种Box Least Squares（BLS）的算法，筛查出新输入数据中的“U形”行星信号。 BLS算法可用VARTOOLS、LcTools等软件实现。
![](https://pic2.zhimg.com/v2-3244ce4d086b0217bd67f1dc20024b89_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='256'></svg>)
这个图就是开普勒90 i的光度曲线。当时BLS算法就检测到了这个持续时间长达2.7小时的U形信号，每14.4天出现一次。该数据取自开普勒从2009年1月3号16：48开始记录的信号。

**为了用已训练好的模型运行这个检测信号，只需执行以下命令：**

> python predict.py —kepler_id=11442793 —period=14.44912 —t0=2.2 
—duration=0.11267 —kepler_data_dir=$HOME/astronet/kepler 
—output_image_file=$HOME/astronet/kepler-90i.png 
—model_dir=$HOME/astronet/model

输出的预测结果是0.94，这意味着该模型判断，该信号有94％的可能性是真的行星。

当然，这只是发现和验证系外行星的整个过程中的一小步：这个模型做的只是预测工作，并不是有很强说服力的论证。

还得把这个信号交给天文学家来复核，只有天文学家再次确认之后，才算是真的系外行星。

至于后续天文学家的工作，详细过程请参考谷歌1月30号发表的论文*Identifying Exoplanets with Deep Learning: A Five-planet Resonant Chain around Kepler-80 and an Eighth Planet around Kepler-90*中的第6.3和6.4节。

经过后续天文学家的分析，确认了这个信号是一个真正的系外行星，就是后面我们大家都知道的开普勒90 i了。
![](https://pic4.zhimg.com/v2-331dcf28e927d013216fa8131e895d6b_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='154'></svg>)
这次捡漏到了的两颗新行星，还是从670颗恒星数据里刨出来的。

要知道开普勒观测的数据有20万颗恒星！这个巨大的宝藏需要更多的人一起来挖。**想发现颗新行星吗？开始动手吧！**

BTW，前天3月7号，是开普勒望远镜上天的九周年。寻找太阳系外类地行星这么多年，布吉岛适合我们搬迁的那一颗在哪里。

最后，附AstroNet代码，

[https://github.com/tensorflow/models/tree/master/research/astronet](https://link.zhihu.com/?target=https%3A//github.com/tensorflow/models/tree/master/research/astronet)

& 1月30号发表的论文，

[http://iopscience.iop.org/article/10.3847/1538-3881/aa9e09/pdf](https://link.zhihu.com/?target=http%3A//iopscience.iop.org/article/10.3847/1538-3881/aa9e09/pdf)

& 原文链接，

[https://research.googleblog.com/2018/03/open-sourcing-hunt-for-exoplanets.html](https://link.zhihu.com/?target=https%3A//research.googleblog.com/2018/03/open-sourcing-hunt-for-exoplanets.html)

— **完** —

欢迎大家关注我们的专栏：[量子位 - 知乎专栏](https://zhuanlan.zhihu.com/qbitai)

诚挚招聘

量子位正在招募编辑/记者，工作地点在北京中关村。期待有才气、有热情的同学加入我们！相关细节，请在量子位公众号(QbitAI)对话界面，回复“招聘”两个字。

[量子位 QbitAI](https://zhuanlan.zhihu.com/qbitai) · 头条号签约作者

վ'ᴗ' ի 追踪AI技术和产品新动态


