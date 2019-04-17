# NLP之最大熵模型 - miner_zhu的博客 - CSDN博客





2018年09月19日 21:46:41[miner_zhu](https://me.csdn.net/miner_zhu)阅读数：172








最大熵模型的基本原理是：在只掌握关于未知分布的部分信息的情况下，符合已知知识的概率分布可能有多个，但使熵值最大的概率分布最真实地反映了事件的分布情况，因为熵定义了随机变量的不确定性，当熵最大时，随机变量最不确定，最难准确地预测其行为。也就是说，在已知部分信息的前提下，关于未知分布最合理的推断应该是符合已知信息最不确定或最大随机的推断。

最大熵模型参数训练的任务就是选取有效的特征fi及其权重λi。由于可以利用歧义点所在的上下文信息（如词形、词性、窗口大小等）作为特征条件，而歧义候选往往有多个，因此，各种特征条件和歧义候选可以组合出很多特征函数，必须对其进行筛选。常用的筛选方法有：①从候选特征集中选择那些在训练数据中出现频次超过一定阈值的特征；② 利用互信息作为评价尺度从候选特征集中选择满足一定互信息要求的特征；③利用增量式特征选择方法从候选特征集中选择特征。第三种方法比较复杂，一般不用。

对于参数λ，常用的获取方法是通用迭代算法（generalized iterative scaling, GIS）。GIS算法要求对训练样本集中每个实例的任意（a, b） ∈A×B，特征函数之和为常数，即对每个实例的k个特征函数均满足![](https://img-blog.csdn.net/20180919214220269?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L21pbmVyX3podQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)＝C（C为一常数）。如果该条件不能满足，则在训练集中取：

![](https://img-blog.csdn.net/20180919214251799?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L21pbmVyX3podQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

并增加一个特征fl：fl（a, b）＝C－![](https://img-blog.csdn.net/20180919214339920?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L21pbmVyX3podQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)。其中，l＝k＋1。与其他特征函数不一样，fl（a, b）的取值范围为：0～C。

GIS算法的描述如下：

（1）    初始化：λ［1..l］＝0；

（2）    根据公式（6-34）计算每个特征函数fi的训练样本期望值：E （fi）；

（3）    执行如下循环，迭代计算特征函数的模型期望值Ep（fi）：

①    利用公式（6-40）和公式（6-39）计算概率 （a|b）；

②    若满足终止条件，则结束迭代；否则，修正λ：λ（n＋1）＝λ（n）＋![](https://img-blog.csdn.net/20180919214503291?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L21pbmVyX3podQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)继续下轮迭代。

（4）算法结束，确定λ，算出每个 （a|b）。

迭代终止的条件可以为限定的迭代次数，也可以是对数似然（L（p））的变化值小于某个阈值ε：

![](https://img-blog.csdn.net/20180919214600968?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L21pbmVyX3podQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

a, b）为（a, b）在训练样本中出现的概率。

由于λ的收敛速度受C取值的影响，因此，人们改进了GIS算法，限于篇幅这里不再详细介绍.



