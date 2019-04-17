# AAAI 2019 | 半监督学习也能自动化？南大和第四范式提出Auto-SSL - 知乎
# 



**机器之心专栏，作者：李宇峰、王海、魏通、涂威威。**

> 让机器自己学习如何搞定一个数据集或任务一直是 AutoML 领域所关注的，它可能不再需要手动数据预处理、不需要人工搭建模型架构或花式调参。但是这些任务基本上都是基于图像识别等监督学习，而实际场景中更多的是未标注数据。因此南京大学 lamda 研究组和第四范式的研究者表示自动化也可以蔓延到半监督学习，他们借此提出了一种 Auto-SSL 模型。

模型选择和超参数优化是机器学习技术应用过程中的关键步骤。给定⼀个机器学习任务，人工参与模型选择以及超参数优化通常是⼀个耗时、繁琐的过程。为了解决这样的问题，⼀些关于自动化机器学习的研究工作被提出，比如 Auto-WEKA 、Auto-Sklearn 和 Auto-Keras 等。

另外近几年 NIPS、PAKDD 等会议已经开始举办 AutoML 比赛，这一新兴领域已经获得学术界和工业界的广泛关注。

以往 AutoML 的工作通常侧重于监督学习问题，需要解决包括特征工程、模型选择、和超参数优化等问题。已有的 AutoML 系统已经可以得到不错的模型预测性能。例如，Auto-WEKA 结合了机器学习框架 WEKA，并利用贝叶斯优化方法对新数据集进行模型以及超参数的选择。

Auto-Sklearn 改进了 Auto-WEKA，它使用元学习来初始化学习算法以及超参数，并集成了评估过程中产生的模型，从而得到鲁棒的模型选择结果。另外，谷歌也开发了一套针对图像分类的自动机器学习产品 Cloud AutoML。以上研究表明 AutoML 可以自动地选择一个适合当前任务的监督学习模型以及超参数。

然而在⼀些实际的应⽤场景中，获取有标记数据会⾮常困难。例如，在网页分类、医学图像分类等领域中通常只能获得大量的未标记数据以及少量标记数据。因此能利用未标注信息的半监督学习（SSL）在现实应用中普遍存在，但自动化的 SSL 研究仍然有限。在本文工作中，我们主要研究了现有 AutoML 技术无法直接解决的 SSL 问题。
- 首先，已有的元学习主要从大量有标记数据提取元特征，从而初始化学习算法以及超参数。对于含有大量未标记数据以及少量有标记数据的数据集，提取能刻画数据分布的元特征对于半监督学习算法的选择至关重要。
- 其次，SSL 在实施过程中可能会出现性能的严重下降，也就是说 SSL 利用了未标记数据以后的模型预测性能还不如仅仅利用有标记数据的监督学习算法。最近一些关于安全半监督的方法已经提出，然而这些研究方法通常侧重学习过程的某一个方面，并没有关注一个自动化的 SSL 解决方案。

为了解决以上问题，本文针对 SSL 提出了自动半监督学习方法（AUTO-SSL）。首先，受到 Auto-Sklearn 的启发，本文使用元学习以快速初始化 SSL 算法。考虑到未标记的数据分布对于 SSL 算法的选择至关重要，本文使用多种无监督聚类算法提取簇内和簇间的统计信息以增强元学习的表现。

其次，本文使用一种大间隔方法，用于微调超参数以缓解 SSL 可能出现的性能降低问题。这种方法的基本思想是，如果某个超参数较好，其在未标记数据上的模型预测结果分类间隔较大。在不同领域的 40 个数据集上，实验结果表明本文提出的方法相比以往的系统有较大的性能提升，包括 AutoML 系统 AUTO-SKLEARN 以及经典 SSL 方法。此外不同于传统的 SSL 方法会出现性能下降的情况，本文提出的方法几乎不会出现这样的情况。

**论文：Towards Automated Semi-Supervised Learning**
![](https://pic2.zhimg.com/v2-01707ba333c0764204738a3bb2026c4d_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='286'></svg>)
论文地址：[http://lamda.nju.edu.cn/liyf/paper/aaai19-autossl.pdf](https://link.zhihu.com/?target=http%3A//lamda.nju.edu.cn/liyf/paper/aaai19-autossl.pdf)

**自动半监督学习定义**

 符号表示：给定包含大量未标记数据和少量有标记数据的半监督数据集
![](https://pic3.zhimg.com/v2-2921bcc01fa41bb0daccd4311323458a_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='288' height='37'></svg>)
，其中 
![](https://pic2.zhimg.com/v2-b7e2e3559793e4ef99a316b6564898d9_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='163' height='30'></svg>)
 表示有标记数据，
![](https://pic2.zhimg.com/v2-a45b4cf54d37c75ec1d1de72ef94e729_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='138' height='30'></svg>)
 表示未标记数据，
![](https://pic3.zhimg.com/v2-b506194085d63f1107b2d34584f6ec76_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='158' height='30'></svg>)
表示样本
![](https://pic3.zhimg.com/v2-d81359bcbcff5c578cc68c78ca0942ce_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='32' height='24'></svg>)
的标记。自动半监督学习的目标如下：

定义：给定半监督学习算法 
![](https://pic2.zhimg.com/v2-eb29bbef82a7cb927d8354a6a29a8271_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='160' height='30'></svg>)
 以及每个学习算法 
![](https://pic2.zhimg.com/v2-f64fa774baf05fbf3976a3f410ee4351_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='34' height='22'></svg>)
 对应的超参数集合
![](https://pic1.zhimg.com/v2-9fe103dac938aeb2caffd031923e4540_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='27' height='22'></svg>)
，基准监督学习算法 
![](https://pic2.zhimg.com/v2-377219b63742fc3245e718db3692582d_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='20' height='20'></svg>)
 以及对应的超参数集合 
![](https://pic1.zhimg.com/v2-7eca6bdbbe9459548c4510c7dd423810_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='19' height='20'></svg>)
。假定模型 
![](https://pic4.zhimg.com/v2-de54c42dcbb3c90ab90258a8203ea98b_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='70' height='20'></svg>)
 是自动半监督学习系统（AUTO-SSL）在数据集
![](https://pic4.zhimg.com/v2-031ac0cc7418a9cdfee6ca67f7c45cfb_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='27' height='20'></svg>)
上得到的训练模型，
![](https://pic2.zhimg.com/v2-8599bedbfff9e6b94b0bc71210fed6b5_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='32' height='20'></svg>)
 是基准监督学习算法在有标记数据 
![](https://pic4.zhimg.com/v2-50450b87472edb56dc1a22240e0d99b7_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='21' height='22'></svg>)
 上得到的训练模型。那么 AUTO-SSL 的目标可以表示为：
![](https://pic1.zhimg.com/v2-7da09efc5ea0e8b114e33ea7b556ef70_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='93' height='21'></svg>)
 通常要显著好于 
![](https://pic2.zhimg.com/v2-c4dfec0925fc110008c6a31286d01d9d_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='79' height='23'></svg>)
，最坏情况下，也不会比 
![](https://pic2.zhimg.com/v2-c4dfec0925fc110008c6a31286d01d9d_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='79' height='23'></svg>)
 差。其中，
![](https://pic2.zhimg.com/v2-714fa3a12ac1812028c0f8986c2bc2d1_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='72' height='20'></svg>)
 表示模型
![](https://pic4.zhimg.com/v2-1bf15fc481edeceb8366b76801f23aa7_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='30' height='20'></svg>)
 在未标记数据上的预测性能。

**传统元学习和增强元特征**

元学习 [Brazdil et al.2008] 旨在利用以往的经验知识来指导新任务的学习，具有学会学习的能力。具体来说，在 AutoML 中，我们收集大量经验数据集的性能数据和其对应的元特征，其中元特征是用来刻画数据集的特征，有助于有效地初始化目标任务上的学习算法 [Feurer et al.2015]。元特征是元学习的核心，传统的元特征主要包括描述数据集基本的结构信息的元特征 (Simple metafeatures)、表示数据集主要成分统计量的 PCA 元特征 (PCA metafeatures )、统计数据的分布情况的统计元特征 (Statistical metafeatures )。详细的传统元特征如下表:
![](https://pic2.zhimg.com/v2-c30ab2a942d045d2217b560f0bf59879_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='604' height='382'></svg>)
对于 SSL，数据分布信息对于 SSL 算法选择至关重要，例如，基于图的半监督学习算法要求数据具有平滑性假设，即相似的样本具有相同的标记；而半监督支持向量机要求数据具有低密度假设，即半监督支持向量机试图找到能将两类数据分开，且穿过数据低密度区域的划分超平面。本文通过无监督学习算法提取数据的分布信息，详细的聚类算法以及提取的特征如下表所示：
![](https://pic1.zhimg.com/v2-da8ac460a1630c1ef92bd02fb386ae10_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='578' height='638'></svg>)
**大间隔方法用于超参数优化**

 元学习能够有效地给目标任务初始化 SSL 算法，但无法对超参数进行优化。然而，实际的应用场景中，超参数优化虽然可以得到不错的性能调整，但效率低下，通常需要消耗大量的时间。传统的 AutoML 系统通过贝叶斯优化调整超参数，需要大量的标记数据切分验证集并多次进行模型评估，对于 SSL，标记数据通常不足以用来切分验证集进行模型评估。本文提出了一种大间隔的方法来优化超参数，基本思想是，如果某个超参数较好，其在未标记数据上的模型预测结果分类间隔较大，反之亦然。下图给出了大间隔方法用于优化超参数的示例。
![](https://pic3.zhimg.com/v2-e0a18827f6a3adb2dab58068ae621a36_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='692' height='314'></svg>)
**实验结果**

为了充分考察 AUTO-SSL 在实际场景中的效果，本文在 40 个数据集上同 AUTO-SKLEARN、传统 SSL 方法以及监督学习方法进行了对比实验。其中数据集涵盖了商业、生命科学、物理、社交、金融、计算机等各种不同的应用领域。因为有标记数据数量是影响 SSL 性能的重要因素之一，我们展示了 AUTO-SSL 在多种场景下可以获得同传统 SSL 方法相比更加鲁棒的结果。

**5.1AUTO-SSL 与 AUTO-SKLEARN 的比较结果**

图 3 给出了在 20 个有标记数据情况下，AUTO-SSL 与 AUTO-SKLEARN 模型预测性能的比较结果，其中绿色部分表示性能的提升量，红色部分表示性能的下降量。可以看出，AUTO-SSL 利用了未标记数据辅助提升模型预测性能，相比仅仅利用标记数据的 AUTO-SKLEARN 在多数情况下会有比较大的性能提升。
![](https://pic1.zhimg.com/v2-eb72335321551f0f5790cc6ef6240994_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='692' height='293'></svg>)
**5.2AUTO-SSL 与传统 SSL 方法的比较结果 **

表 2-3 给出了 40 数据集上 AUTO-SSL 与传统 SSL 方法的详细比较结果，ASSL 表示本文提出的方法。图 4 给出了 AUTO-SSL 和传统 SSL 方法相比基准监督学习方法 SVM 的比较结果，从图中可以看出，传统 SSL 方法存在一些情况下，利用未标记数据以后的模型预测性能不如仅仅利用有标记数据的监督学习方法，而 AUTO-SSL 能够避免这种情况的出现。
![](https://pic1.zhimg.com/v2-e3dc71de7d69798bb3d94414ca43cd80_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='692' height='705'></svg>)


![](https://pic2.zhimg.com/v2-ceaa6f7b3fc47fd96db2db8c931288c9_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='692' height='237'></svg>)
**5.3 有标记数据数量对结果的影响**

图 5 给出了不同有标记数据情况下，AUTO-SSL 与对比方法的平均预测性能。表 4 给出了不同有标记数据情况下，AUTO-SSL 与传统 SSL 方法相比于基准监督学习方法「胜/平/负」的统计情况；其中，「胜/平/负」表示该方法得到的模型预测性能相比基准监督学习方法 SVM 有」显著提升/无显著性/显著下降」。从实验结果可以看出，AUTO-SSL 相比各个比较方法更能够得到鲁棒的模型预测结果。
![](https://pic2.zhimg.com/v2-4949fbe8dcaea0ae59fdf170da92518d_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='692' height='292'></svg>)


![](https://pic3.zhimg.com/v2-3215fb790faf9a97e513fa8580d27d1e_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='692' height='488'></svg>)



