# Face R-FCN算法笔记 - AI之路 - CSDN博客





2017年12月25日 22:49:29[AI之路](https://me.csdn.net/u014380165)阅读数：1599








论文：Detecting Faces Using Region-based Fully Convolutional Networks 

链接：[https://arxiv.org/abs/1709.05256](https://arxiv.org/abs/1709.05256)

腾讯AI Lab的文章，总结起来就是在R-FCN框架上做了一些修改，使其更好地应用在人脸检测上，修改主要包含三个方面，可以看原文的这段话：**We improve the R-FCN framework for targeting face detection in three aspects. First, we introduce additional smaller anchors and modify the position sensitive RoI pooling to a smaller size for suiting the detection of the tiny faces. Second, we propose to use position-sensitive average pooling instead of normal average pooling for the last feature voting in R-FCN, which leads to an improved embedding. Third, multi-scale training strategy and on-line Hard Example Mining (OHEM) strategy are adopted for training. In the testing phase, we also ensemble the multi-scale detection results to improve the performance.** 可以看出主要是一些trick，比如用更小尺寸的anchor、将原来R-FCN中最后的vote操作由average pooling改为带权重的average pooling、采用multi-scale的train和test以及OHEM等等，都可以借鉴。整体读下来，文章的思路还是比较清晰的。

**Figure2是Face R-FCN的网络结构**，主要就是最后的position-sensitive average pooling和R-FCN不一样，其他可以参考R-FCN结构。 
![这里写图片描述](https://img-blog.csdn.net/20171225224729995?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxNDM4MDE2NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
**关于前面提到的第一点改进，也就是引入小尺寸的anchor和减小原来Position-Sensitive ROI Pooling的输出尺寸**。文中虽然没有更详细的参数介绍，但是思想还是比较直接的，目的还是针对小尺寸的人脸。

**关于前面提到的第二点改进，那就是将R-FCN中的average pooling替换成Position-Sensitive Average Pooling**。Position-Sensitive Average Pooling虽然名字上看起来比较复杂，但其实就是带权重的average pooling，**为什么要将原来的average pooling换成带权重的average pooling，因为作者认为每个区域的贡献程度不是完全一样的**。原文是这么说的：Hence, we propose to perform weighted average for each area of the output of position sensitive RoI pooling in order to re-weight the region, which is called position-sensitive average pooling. 具体的公式如下，比较容易理解。 
![这里写图片描述](https://img-blog.csdn.net/20171225224809712?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxNDM4MDE2NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
**关于前面提到的第三点改进，就是在训练和测试时候的一些trick**，原文讲得很详细了，这里直接粘贴过来。In the training phase, we resize the shortest side of the input to 1024 or 1200 pixels. During training, we also apply OHEM on negative samples and set the positive and negative samples ratio to 1:3 in each mini-batch. **In the testing phase, we build an image pyramid for each test image. Each scale in the pyramid is independently tested. The results from various scales are eventually merged together as the final result of the image.**

**实验结果：**
**实验是在两个常用的人脸检测数据集FDDB和WIDER FACE上进行的，检测的主网络采用ResNet-101**。提一个细节，原文这句话：Specifically, we freeze the general kernels (weights of few layers at the beginning) of the pre-trained model throughout the entire training process in order to keep the essential feature extractor trained on ImageNet. 就是在一开始fine tune的时候，模型前面一些层的参数是固定的，只更新后面几层，等到训练进行到一定程度后，再全部更新，目的是怕一开始就更新所有层参数的话容易影响基础特征的提取。

**Figure3是在WIDER FACE数据集上的结果对比：**
![这里写图片描述](https://img-blog.csdn.net/20171225224845862?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxNDM4MDE2NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
FDDB数据集：There are two evaluation protocols for evaluating the FDDB dataset: one is 10-fold cross-validation and the other is unrestricted training (using the data outside FDDB for training). Our experiments strictly follow the protocol for unrestricted training. 

因此在FDDB数据集上测试的Model-A的训练数据是WIDER FACE的训练数据，Model-B的训练数据是WIDER FACE的训练数据加上其他个人收集的数据。 
**Figure4是在FDDB数据集上的结果对比：**
![这里写图片描述](https://img-blog.csdn.net/20171225224906656?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxNDM4MDE2NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)






