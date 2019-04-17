# 表情识别（五）--MBP+CNN - 工作笔记 - CSDN博客





2018年05月28日 14:41:48[Eason.wxd](https://me.csdn.net/App_12062011)阅读数：7294
所属专栏：[机器视觉](https://blog.csdn.net/column/details/33959.html)













EmotiW 2015 LBP特征输入到CNN模型中，并经过特殊映射，得到高于基线15%的性能提升。

作者采用了：
- 4个cnn模型VGG S,VGG M-2048,VGG M-4096和GoogleNet
- 5种不同特征作为CNN输入 （RGB, LBP，以及作者额外三种处理的LBP特征）
- 进行了20次实验。实验中10个最好的model中只有一个是RGB作为输入的。
- ![](https://img-blog.csdn.net/20180529145347304)





由于LBP的差值不能反映两点间的差异，作者提出了mapping方法让其差异能代表两点真实差距。 将图像转换为LBP代码，使模型对照明亮度变化具有鲁棒性。如下图：

![](https://img-blog.csdn.net/20180528144621893)

LBP特征不再多数，作者所谓的映射，是采用MDS，多维度压缩方法，将LBP特征值，映射到一个测地空间，MDS就是一种监督类维度归约方法。

另外，作者同样采用了中心 crop,上采样等数据增强方式，提高精度。

给个结果：





![](https://img-blog.csdn.net/20180528145616348)

![](https://img-blog.csdn.net/20180528145712886)

说实话，效果不是很好，毕竟，只是对单一特征的处理，改变提升不大。



