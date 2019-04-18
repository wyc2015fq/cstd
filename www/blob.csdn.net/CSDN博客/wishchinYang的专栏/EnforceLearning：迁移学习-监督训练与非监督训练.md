# EnforceLearning：迁移学习-监督训练与非监督训练 - wishchinYang的专栏 - CSDN博客
2018年11月27日 17:55:21[wishchin](https://me.csdn.net/wishchin)阅读数：210
**前言**
        CNN刷分ImageNet以来，迁移学习已经得到广泛的应用，不过使用ImageNet预训练模型迁移到特定数据集是一个全集到子集的迁移，不是标准定义的迁移学习（模型迁移），而是“模型移动”。若对网络结构不加修改地进行运用，则只是一个参数调优的过程。
        迁移学习（Transfer Learning,TL）对于人类来说，就是掌握举一反三的学习能力。比如我们学会骑自行车后，学骑摩托车就很简单了；在学会打羽毛球之后，再学打网球也就没那么难了。对于计算机而言，所谓迁移学习，就是能让现有的模型算法稍加调整即可应用于一个新的领域和功能的一项技术。
        从定义上看来，迁移学习为特定模型的定义域迁移。举一反三有一个基本的要求，即是自然数是递归系统，自然数对加法运算形成一个半群。这就引发迁移学习的必然要求，在某个层面具有特定的相似性。 参考：[一文读懂迁移学习](http://www.sohu.com/a/156932670_387563)。
迁移学习，对应了模型的**初始化问题**。
- 对于局部极值的问题，nature综述中，三个大牛作者的说法是：对于深度网络来说，局部极值从来都不是一个问题，从任意的初始参数值开始训练网络，最后都能达到相似的分类效果。这也是被**最近**的理论和实践所证明的。**但是从随机初始化到最优解，有可能需要不可负担的算力成本和时间成本**。
        迁移学习的一个重要结果为，使深度学习**从无监督预训练到仅使用有监督训练**，使深度监督学习变得有效且被广泛使用，参考：[DeepLearning系列15-有监督和无监督训](https://blog.csdn.net/u012526120/article/details/49591213)练。Andrew NG的教程是06年到12年之间的产物，当时无监督训练是主流，Li feifei的CNN教程和caffe官网的教程是产生于12年之后，这时数据库足够大（上千万级别），模型足够先进（ReLU激活函数，dropout等等），同时计算速度足够快（GPU加速），使得无监督预训练（自编码神经网络）在很多应用场景中失去了存在的价值，有监督训练已足够完成任务。更重要的一点是迁移学习的使用，通过不断的定义域迁移，从minist到iris、cifar10、ImageNet、再到各类专业领域数据集，逐步对模型的少量修改最终演化出各个领域较优的深度模型，推动了深度学习的巨大发展。
        迁移学习的极端情况为**一次学习**和**零次学习**，分别对应只有一个新样本的学习和没有任何新数据（仅提供几种简单的属性描述和属性值定义域的描述）的学习。
        CVPR2018论文：T[askonomy: Disentangling Task Transfer Learning](https://zhuanlan.zhihu.com/p/38425434)；
****迁移学习的基本方法/表象****
- 
**样本迁移（Instance based TL）**
在源域中找到与目标域**相似的数据**，把这个数据的权值进行调整，使得新的数据与目标域的数据进行匹配。下图的例子就是找到源域的例子3，然后加重该样本的权值，使得在预测目标域时的比重加大。优点是方法简单，实现容易。缺点在于权重的选择与相似度的度量依赖经验，且源域与目标域的数据分布往往不同。
样本迁移的假设为样本空间的近似或者包含特性。模型并没有迁移，而只是“移动”。
![](http://5b0988e595225.cdn.sohucs.com/images/20170713/e35847d33e0649439388cf46da9c90d6.png)
- 
**特征迁移（Feature based TL）**
假设源域和目标域含有一些共同的交叉特征，通过**特征变换**，将源域和目标域的特征变换到相同空间，使得该空间中源域数据与目标域数据具有相同分布的数据分布，然后进行传统的机器学习。优点是对大多数方法适用，效果较好。缺点在于难于求解，容易发生过适配。
![](http://5b0988e595225.cdn.sohucs.com/images/20170713/72c45f2ae3d746c8b32eff6a0965b864.png)
- 
**模型迁移（Parameter based TL）**
假设源域和目标域**共享模型参数**，是指将之前在源域中通过大量数据训练好的模型应用到目标域上进行预测，比如利用上千万的图象来训练好一个图象识别的系统，当我们遇到一个新的图象领域问题的时候，就不用再去找几千万个图象来训练了，只需把原来训练好的模型迁移到新的领域，在新的领域往往只需几万张图片就够，同样可以得到很高的精度。优点是可以充分利用模型之间存在的相似性。缺点在于模型参数不易收敛。
![](http://5b0988e595225.cdn.sohucs.com/images/20170713/496a8b16841e49d1a217ff5075fb6459.png)
- 
**关系迁移（Relation based TL）**
假设两个域是相似的，那么它们之间会共享某种相似关系，将源域中逻辑网络关系应用到目标域上来进行迁移，比方说生物病毒传播到计算机病毒传播的迁移。
![](http://5b0988e595225.cdn.sohucs.com/images/20170713/d81cbd5866e249578be6d03e148a53ed.png)
**数学定义要求**
**定义域迁移：**
         迁移学习的直接定义为特定模型的**定义域迁移**。这就要求新的定义域与模型的定义域结构相近，即模型大致可以描述新的数据集的结构，如图中的两类定义域有大面积的交叉。此种形式为模型的假设空间-形式空间不曾改变，即未改变模型的超参数，而模型的参数空间发生变化，即模型的泛函定义域产生了一些变化。
         大量的CNN模型应用使用了迁移方法。使用ImageNet的预训练模型应用于特定领域，并未改变CNN的结构，只使用新定义域样本进行了模型调优。或者在一个已理论或实验证明的较优结构上，进行少量的新样本混合，直接可以训练原始模型。
**高层特征提取：**
        特征迁移，在两类不同的定义域上，可能定义域空间完全不重合，而直觉上有相似的特定高层特征，因此可以使用特定的特征迁移模型。比如猫狗识别模型和豹构识别模型。
         方法通过特征变化，把一个特征空间变化到另一个特征空间，对应了**修改CNN的底层，修改了模型**。在数学上相当于重构泛函形式的过程，把新的泛函形式映射到模型的泛函形式空间。
       参考：[什么是**One-Shot Learning**](https://www.zhihu.com/question/50996014)？[Discriminative Learning of Latent Features for Zero-Shot Recognition](https://zhuanlan.zhihu.com/p/36030705). **Zero-short learning 就是用来解决识别我们的训练样本中从没就没有出现过的样本。**但是，虽然训练样本中从来没有出现过该样本，但是却有类似的样本出现过。
![](https://pic4.zhimg.com/80/v2-abe8fb0625178c6ccf79c26282926377_hd.jpg)
       感性认识的话，虽然我们不知道这东西叫鸭嘴兽，但是我们可以说出他和谁谁谁像。
**剥离高层特征：**
        模型迁移，在相近的两个定义域内，通常具有相似的底层特征，可以使用抽象高层特征的模型迁移。
         在CNN领域，标示为特定的CNN模型可以不加修改的直接使用前几层的特征提取层，而仅对后几层的网络进行超参数寻优和参数调优。对应**修改了CNN模型的高层，修改了模型**。
        比如在一个固定的证明检测猫的多层CNN模型中，可以对此CNN模型做少量修改和训练即使用。通过近修改后几层的结构，并对新模型进行相对少量新样本的训练，即可以完成从识别“波斯猫”到识别“新加坡猫”的迁移。
**关系迁移**
       关系迁移的要求是两类定义具有相似的或者同构的关系，即两类数据通过不同的特征抽象，可以通过相似的逻辑进行描述。
**总结：**
       基本的迁移模型为模型的完全迁移，基于定义域具有相似的特征，仅通过改变模型的参数，使用新的数据集训练原始模型即可。模型迁移基于不同的定义域具有相似的底层特征，近做模型顶层的修改，使用新的数据集训练顶层模型。特征迁移基于不同的定义域具有相似的高层特征，对原始模型添加新的底层映射，使用新的数据集训练模型底层；关系迁移基于不同的定义域具有相似的逻辑关系，可迁移的为模型的逻辑层，仅需要对模型的前期特征提取进行训练即可。
### 其他
****2. 迁移学习的一般化定义****
条件：给定一个源域Ds和源域上的学习任务Ts，目标域Dt和目标域上的学习任务Tt
目标：用Ds和Ts学习目标域上的预测函数f（·）
限制条件：Ds≠Dt，Ts≠Tt
****3 迁移学习的分类******3.1 按特征空间分**
同构迁移学习（Homogeneous TL）: 源域和目标域的特征空间相同，XS=XT
异构迁移学习（Heterogeneous TL）：源域和目标域的特征空间不同，XS≠XT
**3.2 按迁移情景分**
归纳式迁移学习（Inductive TL）：源域和目标域的学习任务不同
直推式迁移学习（Transductive TL):源域和目标域不同，学习任务相同
无监督迁移学习（Unsupervised TL):源域和目标域均没有标签
****迁移学习工具包****
[Boosting for Transfer Learning] (http://www.cse.ust.hk/TL/code/C_TraDaBoost.rar), Wenyuan Dai，Qiang Yang, et al., C语言
[Selective Transfer Between Learning Tasks Using Task-Based Boosting] (http://cs.brynmawr.edu/~eeaton/TransferBoost/TransferBoostExp.java), Eric Eaton and Marie desJardins, Java
[Domain Adaptation in Natural Language Processing] (http://www.mysmu.edu/faculty/jingjiang/software/DALR.html), Jiang Jiang,C++
[Triplex Transfer Learning: Exploiting both Shared and Distinct Concepts for Text Classification] (http://www.intsci.ac.cn/users/zhuangfuzhen/code&data_TriTL.rar), Fuzhen Zhuang , Ping Luo, et al.,Matlab
[Heterogeneous Transfer Learning for Image Classification] (http://www.cse.ust.hk/~yinz/htl4ic.zip), Yin Zhu, Yuqiang Chen, et al.,Matlab
[Domain Adaptation Toolbox] (https://github.com/viggin/domain-adaptation-toolbox), Ke Yan,Matlab
****9. 参考资料****
> 
2017香港科技大学杨强机器之心GMIS峰会演讲
2016香港科技大学杨强KDD China技术峰会演讲Sinno Jialin Pan and Qiang Yang, A survey on transfer learningKarl Weiss, Taghi M. Khoshgoftaar and DingDing Wang, A survey of transfer learning龙明盛，迁移学习问题与方法研究
> 
注：上文部分图片摘自以上参考资料
