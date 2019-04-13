
# 年龄估计——DEX - 小花生的博客 - CSDN博客


2018年08月06日 19:51:34[Peanut_范](https://me.csdn.net/u013841196)阅读数：990


《DEX: Deep EXpectation of apparent age from a single image》
ICCV2015，Rasmus Rothe et al，DEX
2015年ChaLearn LAP（Look At People）的冠军之作
**真实年龄**(Actual age：real age of an individual )：实际上就是我们正常人从出生到现在按照年份计算的年龄
**表面年龄**(Apparent age：suggested age by human subjects from the visual appearance )：这个是其他人对你的外貌进行评估，认为的你的年龄，也就是别人对你的年龄的估计。
**估计年龄**(Estimated age: recognized age by an algorithm from the visual appearance )：这个就是通过你的外貌，利用算法计算出来的年龄。
**本文的三点贡献：**
**1.分享了IMDB-WIKI数据集**，包含524230张从IMDB 和Wikipedia爬取的名人数据图片。
![这里写图片描述](https://img-blog.csdn.net/20180806194038614?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
**2.提出了新颖的化年龄回归变成年龄分类的算法**。本质就是在0-100之间的101类分类后，对于得到的分数（softmax）和对应的label（0-100）相乘，并将最终结果求和，得到最终识别的年龄。
整体流程：
![这里写图片描述](https://img-blog.csdn.net/20180806194107794?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)[ ](https://img-blog.csdn.net/20180806194107794?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
**3.DEX取得了LAP 2015的最好成绩**
通过将年龄回归问题看做是分类+softmax期望结合的问题，使用的网络是VGG-16，先在ImageNet上进行分类预训练，然后在作者搜集的IMDB-WIKI数据集上进行微调，最终获得了第一名。
**网络结构：**
网络为VGG16，预先在ImageNet上进行分类训练，然后在IMDB-WIKI数据集上进行微调。
单独回归器训练的时候，将VGG16最后一层神经元改为1个，从而获得回归年龄。
单独分类器训练的时候，最后一层神经元数目改为101进行分类，从而获得每个类别的概率。
最终通过将分类器各个类别概率乘以对应的年龄，得到网络预测的年龄：
![](https://img-blog.csdn.net/20180806194138180?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)[  ](https://img-blog.csdn.net/20180806194138180?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
其中，O表示输出层的101个维度，o表示属于某个类别的可能性，y表示某一个年龄。
[
](https://img-blog.csdn.net/20180806194138180?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)**评价标准：**
本文对结果进行评价时采用的方法分别是MAE和$\epsilon-error$。
MAE：顾名思义，就是mean absolute error，计算结果与标签之间的绝对值误差。
$\epsilon-error$：该比赛要求使用的标准评价方法。因为LAP数据集标注的年龄是不同人对于同一个人进行年龄估计的均值，因此其包含均值和方差两个属性。因此，该误差的计算方式如下：
![这里写图片描述](https://img-blog.csdn.net/20180806194629632?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
**最终识别结果：**
![这里写图片描述](https://img-blog.csdn.net/20180806194704796?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
**失败的例子：**
![这里写图片描述](https://img-blog.csdn.net/2018080619473420?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
失败案例中，影响的因素有多种：人脸检测时检测到背景模糊的人脸，肤色和光照问题，面部遮挡问题，黑白照片问题等。
**文章创新点：**
1. 提出了将年龄回归问题看做是分类问题，使用分类概率乘以对应标签，得到一个预测结果。该结果比单独分类、回归精度高很多。
2. 作者开放了数据集IMDB-WIKI。
参考：
1.[https://blog.csdn.net/hanjiangxue_wei/article/details/79718833](https://blog.csdn.net/hanjiangxue_wei/article/details/79718833)
2.[https://blog.csdn.net/qq_14845119/article/details/79634682](https://blog.csdn.net/qq_14845119/article/details/79634682)
3.[https://data.vision.ee.ethz.ch/cvl/rrothe/imdb-wiki/](https://data.vision.ee.ethz.ch/cvl/rrothe/imdb-wiki/)
IMBD-WIKI数据集
目前开源的最大的人脸年龄数据集
大小：IMDB上20,284个名人，共460,723张图片；wiki上62,328张图片。
标签：年龄和性别。
问题：性别不平衡，女:男 = 4:1，性别标签感觉很多都是错的。
---

###### 注：博众家之所长，集群英之荟萃。

