# 目标检测（十七）--PVANet - 工作笔记 - CSDN博客





2017年09月12日 16:48:53[Eason.wxd](https://me.csdn.net/App_12062011)阅读数：6596
所属专栏：[机器视觉](https://blog.csdn.net/column/details/33959.html)









**PVANET: Deep but Lightweight Neural Networks for Real-time Object Detection**
- intro: “less channels with more layers”, concatenated ReLU, Inception, and HyperNet, batch normalization, residual connections
- arxiv: [http://arxiv.org/abs/1608.08021](http://arxiv.org/abs/1608.08021)
- github: [https://github.com/sanghoon/pva-faster-rcnn](https://github.com/sanghoon/pva-faster-rcnn)
- leaderboard(PVANet 9.0): [http://host.robots.ox.ac.uk:8080/leaderboard/displaylb.php?challengeid=11&compid=4](http://host.robots.ox.ac.uk:8080/leaderboard/displaylb.php?challengeid=11&compid=4)

**PVANet: Lightweight Deep Neural Networks for Real-time Object Detection**
- intro: Presented at NIPS 2016 Workshop on Efficient Methods for Deep Neural Networks (EMDNN). Continuation of[arXiv:1608.08021](https://arxiv.org/abs/1608.08021)
- arxiv: [https://arxiv.org/abs/1611.08588](https://arxiv.org/abs/1611.08588)

转自：[http://blog.csdn.net/zimenglan_sysu/article/details/52471410](http://blog.csdn.net/zimenglan_sysu/article/details/52471410)﻿﻿

根据作者开源的model，pt文件，和代码等，

笔者跑了`example_train_384`这个实验（具体看上面的开源代码下的` models/pvanet/example_train_384`），其结果如下：

trainset： pascal voc 07 trainval-set

testset：  pascal voc 07 testset

mAP：71.81%

stepsize：5w

iterations：10w

lr policy：step

另外把stepsize改为8w，itrations改为11w，其mAP为72.6%，这个比vgg16的69.6%要好。

还跑了trainset为pascal voc 0712的trainval，其中（5w/10w，分别为stepsize和iterations），mAP为73.6%，这个比vgg16的75.8%要低。

=====

**更新**

跑trainset为pascal voc 0712的trainval，其中（32w iterations，iter_size为3，采用plateau lr policy：2w，3w，4w，5w），mAP为**77.15%**，这个比vgg16的75.8%要高。

跑trainset为pascal voc 0712的trainval，其中（32w iterations，iter_size为3，采用plateau lr policy：2w，3w，4w，5w，global context branch），mAP为**78.38%**，这个比vgg16的75.8%要高。



=====

**最新的图**

![](https://img-blog.csdn.net/20161128225505075?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

![](https://img-blog.csdn.net/20161128225540232?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

![](https://img-blog.csdn.net/20161128225629670?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

=====

具体可以参考[The Mean AP is 0.7190 when I test the model trained by example_train_384, is normal? #10](https://github.com/sanghoon/pva-faster-rcnn/issues/10)。

=====

先看leaderboard

![](https://img-blog.csdn.net/20160908150800545?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

再看下speed

![](https://img-blog.csdn.net/20160908150821811?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

厉害的不要不要的。



=====

转正题，下面介绍下这篇论文，

老规矩，上图。

![](https://img-blog.csdn.net/20160908150832842?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

**一句话：**

利用目前各种设计network的方法，如batch normalization，inception，C.ReLU，residual connection，multi-scale representation等，设计一个deep，but thin的feature extractor network，然后在这个feature extractor network上把faster-rcnn的rpn和rcnn接上，得到一个完整的detection deep network，并用了SVD分解来降低fc的纬度，以及用较少的proposals（200个），在pascal voc上取得傲人的performance（mAP&speed）。当然其中也用到了一些训练技巧，如learning rate scheduling[1]。



而该论文的最重要的一个贡献就是第一个用Inception来做detection，不论是accuracy还是speed，都充分证明了其适合用于detection，而不仅仅是classification。



=====

**先看framework**

![](https://img-blog.csdn.net/20160908145935806?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

从tab 1大致可以看到论文中的framework（称为PAVNet），其实和VGG16很是相似的，

区别在于，PAVNet把VGG16的每个层（conv，relu等）用C.ReLU和inception代替，并引入了residual connection和multi-scale representation。

下面就一一说下其中key component。



=====

**C.ReLU**

![](https://img-blog.csdn.net/20160908150405417?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

![](https://img-blog.csdn.net/20160908150347858?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



=====

**inception （相对来比较简单）**

![](https://img-blog.csdn.net/20160908150509168?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



正是这样的inception设计，使得模型可以看到不同大小的感受野，从而可以检测到不同大小的物体，

![](https://img-blog.csdn.net/20160908150649581?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



=====

**residual connection & multi-scale representation & BN**

可以从tab 1中知道，residual connection主要是以prejected的方式：

1x1 conv is applied for projecting pool1_1 into conv2_1, conv2_3 into conv3_1, conv3_4 into conv4_1, and conv4_4 into conv5_1.




multi-scale representation obtained by four steps:

1 conv3_4 is down-scaled into “downscale” by 3x3 max-pool with stride 2;

2 conv5_4 is up-scaled into “upscale” by 4x4 channel-wise deconvolution whose weights are fixed as  bilinear interpolation;   

3 “downscale”, conv4_4 and “upscale” are combine into “concat” by channel-wise concatenation; 

4 after 1x1 conv, the final output is obtained (**convf**).



至于BN这个就没有什么好说的了，feature extractor network的每个conv后接BN（+Scale）



=====

**rpn & rcnn**

这个跟faster-rcnn的相差不大，其中rpn用了25 anchors of 5 scales (3, 6, 9, 16, 25) and 5 aspect ratios (0.5, 0.667, 1.0, 1.5,2.0)

而rcnn在训练是用了12k个proposals（NMS之前，其threshold为0.4），而测试时仅用200个proposals（这个减少了1/3的测试时间）

![](https://img-blog.csdn.net/20160908151511970?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



=====

**training & testing**

看图不说话。

![](https://img-blog.csdn.net/20160908151614767?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



=====

**嗯，最后看performance**

![](https://img-blog.csdn.net/20160908152446574?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

![](https://img-blog.csdn.net/20160908152504083?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



=====

**最后说下是怎么提速的**

1 设计了一个thin的feature extractor network（本文的重点）

2 将少了proposals的个数（由faster-rcnn的300个变成200个）

3 对rcnn的fc进行SVD降维（降纬后还finetune了一下），令人惊讶的是没有多大的精度损失



=====

总言之，该论文成功论证了这样的thin network不仅适合做classfication，而且还可以做detection，其精度和速度都是傲娇的

想想其的online application，就觉得很吓人了

估计它的团队很快就将其布置到实际应用上了

﻿﻿



