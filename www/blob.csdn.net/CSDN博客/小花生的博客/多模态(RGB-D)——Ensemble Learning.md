
# 多模态(RGB-D)——Ensemble Learning - 小花生的博客 - CSDN博客


2018年10月04日 20:53:36[Peanut_范](https://me.csdn.net/u013841196)阅读数：302


《Improving a Deep Learning based RGB-D Object Recognition Model by Ensemble Learning》
2017，Andreas Aakerberg et al. Ensemble Learning集成学习
1.引言：
增加RGB图像同深度信息是一种众所周知的方法，来有效地提升物体识别模型的准确率。
另外一种提升视觉识别模型表现的方法是集成学习（ensemble learning）。
Depth数据中包含了关于物体形状的几何信息；RGB中包含了纹理、颜色和表观信息。
此外，Depth数据对光照和颜色不敏感，具有更强的鲁棒性。
作者发现一个已经存在的多模态RGB-D物体识别模型能有效的提升识别表现，通过集成两个普通（通用）模型和一个专家（特定）模型，在Washington RGB-D Object Dataset上进行评估。
2.Ensemble Methods：
1）Unweighted Averaging：
CNNs标准的集成方法是未加权的平均。每个模型的softmax概率进行平均，来得到最后的预测结果。
![在这里插入图片描述](https://img-blog.csdn.net/20181004204632344?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
2）Weighted Averaging：
加权平均的方法和未加权的方法类似，不同点在于每个独立的模型拥有自己的权重。
![在这里插入图片描述](https://img-blog.csdn.net/20181004204655460?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
![在这里插入图片描述](https://img-blog.csdn.net/20181004204711371?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
权值的确定：
网格搜索（grid search）寻找所有可能的值，所有独立的候选模型在验证集上表现来确定最后的权重大小，权值的和（sum）为1。（距离加权）
3）Majority Voting:
多数表决能够被使用当集成的模型>2时。每个独立的模型给出预测的label，得票数最多的为最后的预测结果，majority voting 更依赖于所有模型的top-1准确率。
3.实验结果：
本文采用了两种集成策略A和B，在这三个模型中，对每个模型的输出概率使用加权平均来得到最优的结果。
1）Ensemble A：
由两个模型组成，权重分别为0.57和0.43。
2）Ensemble B：
由三个模型组成，权重分别为0.17,0.13和0.7，效果优于A的方法。
3）集成测试结果：
![在这里插入图片描述](https://img-blog.csdn.net/20181004204755885?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
从结果来看，集成B的方法带来效果明显。
[
](https://img-blog.csdn.net/20181004204755885?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)小结：
采用集成学习（ensemble learning）的方法来提升RGB-D物体识别的准确率，加权平均（Weighted Averaging）的方法带来提升较为明显，权值确定使用网格搜索（grid search）。
---
注：博众家之所长，集群英之荟萃。

[
](https://img-blog.csdn.net/20181004204755885?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
