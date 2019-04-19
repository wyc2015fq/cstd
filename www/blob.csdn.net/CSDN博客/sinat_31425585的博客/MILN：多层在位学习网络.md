# MILN：多层在位学习网络 - sinat_31425585的博客 - CSDN博客
2018年09月29日 11:42:46[Mirror_Yu_Chen](https://me.csdn.net/sinat_31425585)阅读数：241
    MILN(multilayer In-place Learning Network)应该是继LCA算法提出后，翁巨扬教授提出的第一个发育网络（developmental network），旨在提出一种通用的在位学习（ In-place Learning）网络来模拟大脑皮层抽取特征。在位学习是一种来自于基因等效性原则的生物学概念，每一个神经元都采用共同的基因程序（LCA），独立负责自身在环境中的学习。一般来说，在位学习具备简单、计算复杂性低等优点。
**    1、模型结构**
    MILN网络结构如图1所示。跟一般神经网络对比一下，MILN输入层对应Layer0，即数据从Layer0流入网络，Layer3对应于输出层，对应网络输出结果，Layer1和Layer2对应于中间层，用于发育出通用的不变特征。
![](https://img-blog.csdn.net/20180925095954980?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMxNDI1NTg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
                                                                 图1 多层在位学习网络结构示意图(摘自文献[1])
    再进一步解释一下这个模型，每一个圆圈代表一个cell(neuron)，从每一个cell引出的粗线段表示轴突，轴突与cell之间连接关系存在两种：兴奋(excitatory)和抑制(inhibitory)，实线表示兴奋连接，虚线表示抑制连接。其他区域的指示（图1中箭头）表示兴奋或抑制的监督信号。
    第![l](https://private.codecogs.com/gif.latex?l)层的第i个神经元存在着三种类型的权重：
    (1) 自下而上的兴奋权重![W_{b}](https://private.codecogs.com/gif.latex?W_%7Bb%7D)：连接自前一层 ![l-1](https://private.codecogs.com/gif.latex?l-1) 输入到当前神经元；
    (2) 侧抑制权重![W_{h}](https://private.codecogs.com/gif.latex?W_%7Bh%7D)：连接同一层中其他神经元到当前神经元；
    (3) 自上而下的（兴奋或抑制）权重![W_{t}](https://private.codecogs.com/gif.latex?W_%7Bt%7D)：这个权重由两个部分组成，第一个部分连接下一层 ![l+1](https://private.codecogs.com/gif.latex?l&plus;1) 层的神经元到当前神经元，第二部分连接其他处理区域（例如，其他感知模态——声音）或层（例如，动作层）到当前神经元。为了简化标记，我们只考虑自上而下的兴奋权重。
**    2、模型解析**
    我们再来看一下红色虚线包裹的区域，这个区域相当于搭建MILN的一个block，只需要弄懂这个，后面MILN的搭建过程相当于就是对block的堆叠。我们从左往右看，输入层Layer0中信息传到中间层Layer1时，这是一个自下而上的信息，Layer1中有一个自下而上的权重![W_{b}](https://private.codecogs.com/gif.latex?W_%7Bb%7D)与之对应，然后Layer1中神经元之间相互竞争，优胜的神经元会对其余神经元产生一种抑制效应，这样Layer1中存在一个抑制权重![W_{h}](https://private.codecogs.com/gif.latex?W_%7Bh%7D)与之对应，最后就是一个来自外部的监督信号，这是一个自上而下的信息，Layer1中有一个自上而下的权重![W_{t}](https://private.codecogs.com/gif.latex?W_%7Bt%7D)与之对应，最后Layer1轴突对应响应为：
![Z=g( W_{b}\cdot x-W_{h}\cdot h+W_{t}\cdot a)](https://private.codecogs.com/gif.latex?Z%3Dg%28%20W_%7Bb%7D%5Ccdot%20x-W_%7Bh%7D%5Ccdot%20h&plus;W_%7Bt%7D%5Ccdot%20a%29)
   然后就是一个top-k竞争机制，通过对轴突响应大小进行排序，找出最大的top-k个响应，这top-k个响应其实就对应于竞争后优胜的cell，我们需要更新这top-k个神经元的权重，对于竞争失败的剩余cell，我们不对其权重进行更新，这里表现出来的便是一种抑制关系，这竞争优胜的top-k个神经元抑制了其余神经元的更新，这也即是网络中兴奋和抑制连接的实现过程。
**    3、Topographic map 机制**
    这个机制主要启发自大脑皮层中，一个特定皮层区域负责检测一个目标类别，负责检测相似特征的神经元在地形图（Topographic map）上彼此靠近，聚集成块，这就自然形成了所谓的"modules"。为了形成这种地形图状皮层结构，我们可以通过改变更新过程来进行实现，其实就是在更新时，不仅更新优胜神经元，而且还更新优胜神经元周围的3x3区域邻居，如图2所示。
![](https://img-blog.csdn.net/20181001104351373?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMxNDI1NTg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
                                                                              图2 Topographic map 示意图
    说明一下，![A_{0,0}](https://private.codecogs.com/gif.latex?A_%7B0%2C0%7D)对应优胜神经元位置，其余的神经元位于当前优胜神经元在拓扑结构上的八领域上。更新![A_{i,j}](https://private.codecogs.com/gif.latex?A_%7Bi%2Cj%7D)位置神经元权重的计算公式如下：
![W_{i,j}=e^{-(i^{2}+j^{2})}](https://private.codecogs.com/gif.latex?W_%7Bi%2Cj%7D%3De%5E%7B-%28i%5E%7B2%7D&plus;j%5E%7B2%7D%29%7D)
**    4、在位学习算法**
    在位学习算法，指的就是LCA（叶成分分析）算法，这个在前面博客中已经进行了推导和描述，这里仅仅做一个归纳。
    假定，用 ![V_{1}^{t},V_{2}^{t},...,V_{c}^{t}](https://private.codecogs.com/gif.latex?V_%7B1%7D%5E%7Bt%7D%2CV_%7B2%7D%5E%7Bt%7D%2C...%2CV_%7Bc%7D%5E%7Bt%7D)， 这![c](https://private.codecogs.com/gif.latex?c)个列向量表示c个神经元，算法利用 ![k](https://private.codecogs.com/gif.latex?k) 维输入样本![y(1),y(2),...](https://private.codecogs.com/gif.latex?y%281%29%2Cy%282%29%2C...)增量式的对这![c](https://private.codecogs.com/gif.latex?c)个神经元进行更新，![V_{i}](https://private.codecogs.com/gif.latex?V_%7Bi%7D) 的模长等于将样本向量 ![y(t)](https://private.codecogs.com/gif.latex?y%28t%29) 投影到叶成分分量![V_{i}](https://private.codecogs.com/gif.latex?V_%7Bi%7D) 上时对应方差矩阵的特征值。当前层的输出为响应向量：![Z=(z_{1},z_{2},...,z_{c})](https://private.codecogs.com/gif.latex?Z%3D%28z_%7B1%7D%2Cz_%7B2%7D%2C...%2Cz_%7Bc%7D%29)。
   单层CCILCA 算法 ![Z=LCA(y)](https://private.codecogs.com/gif.latex?Z%3DLCA%28y%29) 如下：
    （1） 使用前 ![c](https://private.codecogs.com/gif.latex?c) 个观测量初始化这 ![c](https://private.codecogs.com/gif.latex?c) 个神经元：![V_{t}^{(c)}=y(t)](https://private.codecogs.com/gif.latex?V_%7Bt%7D%5E%7B%28c%29%7D%3Dy%28t%29) ， 并将每个神经元的年龄设置为1，![n(t)=1,t=1,2,...c](https://private.codecogs.com/gif.latex?n%28t%29%3D1%2Ct%3D1%2C2%2C...c)；
    （2） 更新过程，采用LCA算法更新这c个神经元
![](https://private.codecogs.com/gif.latex?)![t=c+1,c+2,...,do:](https://private.codecogs.com/gif.latex?t%3Dc&plus;1%2Cc&plus;2%2C...%2Cdo%3A)
        (a)  如果输出未给定，计算所有 ![c](https://private.codecogs.com/gif.latex?c) 个神经元的响应：
![z_{i}=g_{i}(\frac{y(t)V_{i}^{(t)}}{||V_{i}^{(t)}||}),\ \ g(x)=\frac{1}{1+e^{-(x/\sigma )}}](https://private.codecogs.com/gif.latex?z_%7Bi%7D%3Dg_%7Bi%7D%28%5Cfrac%7By%28t%29V_%7Bi%7D%5E%7B%28t%29%7D%7D%7B%7C%7CV_%7Bi%7D%5E%7B%28t%29%7D%7C%7C%7D%29%2C%5C%20%5C%20g%28x%29%3D%5Cfrac%7B1%7D%7B1&plus;e%5E%7B-%28x/%5Csigma%20%29%7D%7D)
        (b)  使用top-k竞争机制，模拟生物神经元侧抑制效应，并将这top-k个神经元的响应归一化：
![{z}'_{i}=(z_{i}-z_{k+1})/(z_{1}-z_{k+1})](https://private.codecogs.com/gif.latex?%7Bz%7D%27_%7Bi%7D%3D%28z_%7Bi%7D-z_%7Bk&plus;1%7D%29/%28z_%7B1%7D-z_%7Bk&plus;1%7D%29)
        (c)  将非零响应归一化到模长为1：
![s=\sum_{i=1}^{k}{z_{i}}',\ \ {z_{i}}'={z_{i}}'/s](https://private.codecogs.com/gif.latex?s%3D%5Csum_%7Bi%3D1%7D%5E%7Bk%7D%7Bz_%7Bi%7D%7D%27%2C%5C%20%5C%20%7Bz_%7Bi%7D%7D%27%3D%7Bz_%7Bi%7D%7D%27/s)
        (d)  只更新top-k个优胜的神经元：
![v_{j}(t)=\omega _{1}v(t-1)+\omega _{2}y_{j}x(t)](https://private.codecogs.com/gif.latex?v_%7Bj%7D%28t%29%3D%5Comega%20_%7B1%7Dv%28t-1%29&plus;%5Comega%20_%7B2%7Dy_%7Bj%7Dx%28t%29)
![\omega _{1}](https://private.codecogs.com/gif.latex?%5Comega%20_%7B1%7D)对应残差率，![\omega _{2}](https://private.codecogs.com/gif.latex?%5Comega%20_%7B2%7D)对应于学习率：
![\omega _{1}=\frac{n_{j}-1-\mu (n_{j})}{n_{j}},\omega _{2}=\frac{1+\mu (n_{j})}{n_{j}}](https://private.codecogs.com/gif.latex?%5Comega%20_%7B1%7D%3D%5Cfrac%7Bn_%7Bj%7D-1-%5Cmu%20%28n_%7Bj%7D%29%7D%7Bn_%7Bj%7D%7D%2C%5Comega%20_%7B2%7D%3D%5Cfrac%7B1&plus;%5Cmu%20%28n_%7Bj%7D%29%7D%7Bn_%7Bj%7D%7D)
![\mu (n_{j})](https://private.codecogs.com/gif.latex?%5Cmu%20%28n_%7Bj%7D%29) 对应于遗忘函数。
**    5、多层学习**
 多层情况下，除了存在自下而上的数据流入，还存在层间自上而下的信号监督，层与层之间存在双向的信号流动，通过神经元之间的竞争机制，判断当前神经元的状态为激活或抑制，每个神经元都通过同一套基因程序LCA来完成自身学习，学习的过程是一种在线增量式的学习方式，不需要去计算协方差矩阵这种二阶的统计量，同时不需要对样本进行保存，能够保证在有限的资源条件下，达到统计学角度最优的时空双最优学习，具体流程如下：
![](https://img-blog.csdn.net/20180929010927102?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMxNDI1NTg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
    在不经过任何预处理的前提下，三层的MILN网络，中间层神经元个数为20x20=400个，使用Mnist数据集，经过一次迭代，可得达到  83.99%的识别率，如图3所示。
![](https://img-blog.csdn.net/20180930211307900?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMxNDI1NTg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
                                                           图3 迭代一次，未经过任何预处理，三层MILN识别结果图
    中间层神经元个数为30x30=900个时，识别率为88.95%，如图4所示。
![](https://img-blog.csdn.net/20180930222351785?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMxNDI1NTg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
                                                                        图4  中间层30x30的三层MILN识别结果图
    中间层神经元个数为40x40=1600个时，识别率为 91.35%，如图5所示。
![](https://img-blog.csdn.net/20180930215232226?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMxNDI1NTg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
                                                                          图5  中间层40x40的三层MILN识别结果图
 ~~~~~未完待续~~~~~
    参考资料：
  [1] Weng J, Luwang T, Lu H, et al. A multilayer in-place learning network for development of general  invariances[J]. International Journal of Humanoid Robotics, 2007, 4(02):281-320.
   [2] Weng J, Luwang T, Lu H, et al. The Multilayer In-Place Learning Network for the Development of General Invariances and Multi-Task Learning[C]. International Joint Conference on Neural Networks. IEEE, 2007:2621-2628.
