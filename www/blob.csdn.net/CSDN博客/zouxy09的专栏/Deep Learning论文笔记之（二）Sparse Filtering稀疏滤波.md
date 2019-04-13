
# Deep Learning论文笔记之（二）Sparse Filtering稀疏滤波 - zouxy09的专栏 - CSDN博客


2013年08月15日 14:36:39[zouxy09](https://me.csdn.net/zouxy09)阅读数：33877


**Deep Learning论文笔记之（二）Sparse Filtering稀疏滤波**
zouxy09@qq.com
http://blog.csdn.net/zouxy09
自己平时看了一些论文，但老感觉看完过后就会慢慢的淡忘，某一天重新拾起来的时候又好像没有看过一样。所以想习惯地把一些感觉有用的论文中的知识点总结整理一下，一方面在整理过程中，自己的理解也会更深，另一方面也方便未来自己的勘察。更好的还可以放到博客上面与大家交流。因为基础有限，所以对论文的一些理解可能不太正确，还望大家不吝指正交流，谢谢。
本文的论文来自：
Sparse filtering, J. Ngiam, P. Koh, Z. Chen, S. Bhaskar, A.Y. Ng. NIPS2011。在其论文的支撑材料中有相应的Matlab代码，代码很简介。不过我还没读。
下面是自己对其中的一些知识点的理解：
**《****Sparse Filtering****》**
本文还是聚焦在非监督学习Unsupervised feature learning算法。因为一般的非监督算法需要调整很多额外的参数hyperparameter。本文提出一个简单的算法：sparse filtering。它只有一个hyperparameter（需要学习的特征数目）需要调整。但它很有效。与其他的特征学习方法不同，sparse filtering并没有明确的构建输入数据的分布的模型。它只优化一个简单的代价函数（L2范数稀疏约束的特征），优化过程可以通过几行简单的Matlab代码就可以实现。而且，sparse filtering可以轻松有效的处理高维的输入，并能拓展为多层堆叠。
sparse filtering方法的核心思想就是避免对数据分布的显式建模，而是优化特征分布的稀疏性从而得到好的特征表达。
**一、非监督特征学习**
一般来说，大部分的特征学习方法都是试图去建模给定训练数据的真实分布。换句话说，特征学习就是学习一个模型，这个模型描述的就是数据真实分布的一种近似。这些方法包括denoising autoencoders，restricted Boltzmann machines (RBMs)，independent component analysis (ICA)和sparse coding等等。
这些方法效果都不错，但烦人的一点就是，他们都需要调节很多参数。比如说学习速率learning rates、动量momentum（好像rbm中需要用到）、稀疏度惩罚系数sparsity penalties和权值衰减系数weight decay等。而这些参数最终的确定需要通过交叉验证获得，本身这样的结构训练起来所用时间就长，这么多参数要用交叉验证来获取时间就更多了。我们花了大力气去调节得到一组好的参数，但是换一个任务，我们又得调节换另一组好的参数，这样就会花了俺们太多的时间了。虽然ICA只需要调节一个参数，但它对于高维输入或者很大的特征集来说，拓展能力较弱。
![](https://img-blog.csdn.net/20130815142433640?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvem91eHkwOQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
本文中，我们的目标是研究一种简单并且有效的特征学习算法，它只需要最少的参数调节。虽然学习数据分布的模型是可取的，而且效果也不错，但是它往往会使学习的算法复杂化，例如：RBMs需要近似对数划分log-partition函数的梯度，这样才可能优化数据的似然函数。Sparse coding需要在每次的迭代过程中寻找活跃的基的系数，这是比较耗时的。而且，稀疏因子也是一个需要调整的参数。本文方法主要是绕过对数据分布的估计，直接分析优化特征的分布。那怎样的特征的分布才是优的呢？到这里，我们需要先关注特征的一些主要属性：population sparsity，lifetime sparsity和high dispersal。怎样的特征才是好的特征，才是对分类或者其他任务好的特征。我们的学习算法就应该学会去提取这种特征。
**二、特征分布**
上面讨论的特征学习算法都可以认为是生成特定的特征分布Feature distributions。例如，sparse coding只用少许的非零系数（特征）来描述每个样本。而一个面向特征分布的方法可以认为是直接优化特征分布的一些属性，使其可以更好的描述样本。
我们引入一个特征分布矩阵，矩阵的每一行是一个特征，每一列是一个样本。每个元素![](https://img-blog.csdn.net/20130815142520859?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvem91eHkwOQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)表示第i个样本的第j个特征的激活值。由上面的分析可以知道，这是一个由输入到特征的映射函数（特征提取函数）来得到。
下面我们就来讨论下什么样的特征分布才是好的：
**1****）每个样本的特征应该是稀疏的（****Population Sparsity****）**
每个样本都只用很少的激活（非零）特征来描述。具体来说，对于特征矩阵的每一列（一个样本）f(i)，只有很少的非零元素。其他的都是0。例如，一幅图像可以由里面包含的一些目标来描述，如果里面存在很多可能的目标，那么在某一时刻，也只可能出现一些。我们称之为population sparsity（种群稀疏）。
**2****）样本间的特征应该是稀疏的（****Lifetime Sparsity****）**
好的特征应该是具有区分性的，这样才可以区分样本。例如需要区分人脸和人手，那么很明显，肤色不是区分性的特征，因为人脸和人手都有肤色。但如果看有没有眼睛，那么就很容易区分是人脸还是人手了，所以眼睛就是一个区分性的特征。所以要区分样本，就要选择样本独有的，而不是大家都有的特征。稍微学术点的表达就是，每个特征只允许在少量的样本内被激活。也就是说，在特征矩阵中，每一行（一种特征）应该只有少量的非零元素。这个特征的属性被称为lifetime sparsity（存在稀疏）。
**3****）特征的分布应该是均匀的（****High Dispersal****）**
对每一行（一种特征在不同样本的时候的不同取值）的特征的分布，应该和其他行的特征的分布相似，或者说每种特征都应该具有相似的统计特性。具体来说，对矩阵的每一行，我们取该行所有元素（一种特征在不同样本的时候的不同取值）的平方后的均值作为其统计特性的描述。每一行都存在一个均值，那么每行的均值都应该是一样的，这样就可以认为所有的特征都具有相似的分布。这种属性我们称之为high dispersal（高分散性）。但对于一个好的特征描述来说，这个属性并不是必要的。但它可以防止特征的退化，也就是可以防止提取到相同的特征（如果提取到相同的特征，那么特征既冗余，又没有增加信息量，所以一般都要求提取到的特征是正交的）。对于过完备的特征表达。high dispersal可以理解为只有很少的inactive不活跃的特征。例如，PCA编码一般不会满足high dispersal，因为大的特征值对应的特征向量（也就是特征code）大部分总是活跃active的。
很多特征学习方法其实有包含上面的这些约束的。例如sparse RBM会约束一个特征的激活值靠近一个目标的值（lifetime sparsity）。ICA会归一化每个特征，还会优化特征的lifetime sparsity。Sparse autoencoder也会显式的优化lifetime sparsity。
另外，基于聚类的算法，例如K-means，是population sparsity约束的一种极端形式，它的一个聚类中心只对应一个特征，对每个样本来说，只有一个特征是激活的（只有一个值是1，其他全是0）。Triangle三角激活函数，本质上也会保证population sparsity。Sparse coding实际上也可以被视为存在population sparsity。
本文中，我们从特征分布的角度去推导得到一种简单的特征学习算法。它仅仅需要优化high dispersal和population sparsity。因为在我们的实验中，我们发现，实现这两种特征的属性对学习一个过完备的特征表达来说已经足够了。在后面，我们会说明，这两种属性的结合实际上已经蕴含着保证特征的lifetime sparsity。
**三、****Sparse filtering**
下面我们就说明下，sparse filtering是如何捕捉到上面说的那些特性的。我们先考虑下从每个样本中计算线性特征。具体来说，我们用![](https://img-blog.csdn.net/20130815142701328?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvem91eHkwOQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)来表示第i个样本（特征矩阵中第i列）的第j个特征值（特征矩阵中第j行）。因为是线性特征，所以![](https://img-blog.csdn.net/20130815142837968?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvem91eHkwOQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)。第一步，我们先简单的对特征矩阵的行进行归一化，然后再对列进行归一化，然后再将矩阵中所有元素的绝对值求和。
具体来说，我们先归一化每个特征为相等的激活值。具体做法是将每一个特征除以其在所有样本的二范数：![](https://img-blog.csdn.net/20130815142857781?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvem91eHkwOQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)。然后我们再归一化每一个样本的特征。这样，他们就会落在二范数的单位球体unit L2-ball上面了。具体做法是：![](https://img-blog.csdn.net/20130815142910718?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvem91eHkwOQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)。这时候，我们就可以对这些归一化过的特征进行优化了。我们使用L1范数惩罚来约束稀疏性。对于一个有M个样本的数据集，sparse filtering的目标函数表示为：
![](https://img-blog.csdn.net/20130815142923781?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvem91eHkwOQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
**3.1****、****Optimizing for population sparsity**
其中![](https://img-blog.csdn.net/20130815142942843?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvem91eHkwOQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)这一项度量的就是第i个样本的特征的population sparsity，也就是限制每个样本只有很少的非零值。因为归一化的特征![](https://img-blog.csdn.net/20130815143000609?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvem91eHkwOQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)被约束只能落在二范数的单位球体上面，所以当这些特征是稀疏的时候，也就是样本接近特征坐标轴的时候，上面的目标函数才会最小化。反之，如果一个样本的每个特征值都差不多，那么就会导致一个很高的惩罚。可能有点难理解，我们看下图：
![](https://img-blog.csdn.net/20130815143043875?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvem91eHkwOQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
左图：假设我们的特征维数是两维(f1, f2)，我们有两个样本，绿色和褐色的。每个样本都会先投影到二范数的球体上面（二维的话就是单位圆），再进行稀疏性的优化。可以看到，当样本落在坐标轴的时候，特征具有最大的稀疏性（例如，一个样本落在f2轴上，那么这个样本的表示就是(0, 1)，一个特征值为1，其他的为0，那么很明显它具有最大的稀疏性）。右图：因为归一化，特征之间会存在竞争。上面有一个只在f1特征上增加的样本。可以看到，尽管它只在f1方向上增加（绿色三角型转移到蓝色三角型），经过列归一化后（投影到单位圆上），可以看到第二个特征f2会减少（绿色圆圈转移到蓝色圆圈）。也就是说特征之间存在竞争，我变大，你就得变小。
对特征进行归一化的一个属性就是它会隐含的进行特征间的竞争。归一化会使得如果只有一个特征分量f(i)增大，那么其他所有的特征分量的值将会减小。相似的，如果只有一个特征分量f(i)减小，那么其他所有的特征分量的值将会增大。因此，我们最小化![](https://img-blog.csdn.net/20130815143226437?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvem91eHkwOQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)，将会驱使归一化的特征趋于稀疏和大部分接近于0。也就是，一些特征会比较大，其他的特征值都很小（接近于0）。因此，这个目标函数会优化特征的population sparsity。
上面的公式与Treves-Rolls的population/life-time sparsity的度量非常相似：
![](https://img-blog.csdn.net/20130815143314921?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvem91eHkwOQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
这里F是特征的个数。这个度量常用来衡量大脑神经元激活的稀疏度。我们提出的公式可以看成是这个度量的平方根，再乘以一个尺度因子。
**3.2****、****Optimizing for high dispersal**
上面说到，特征的high dispersal属性要求每个特征被恒等激活。在这里，我们粗鲁地强制每个特征的激活值平方后的均值相等。在上面sparse filtering的公式中，我们首先通过将每个特征除以它在所有样本上面的二范数来归一化每个特征，使他们具有相同的激活值：![](https://img-blog.csdn.net/20130815143336343?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvem91eHkwOQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)。实际上，它和约束每个特征具有相同的平方期望值有一样的效果。![](https://img-blog.csdn.net/20130815143353093?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvem91eHkwOQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)，因此，它已经隐含的优化了high dispersal属性。
**3.3****、****Optimizing for lifetime sparsity**
我们发现，对population sparsity和high dispersal的优化就已经隐含的优化了特征的lifetime sparsity。这其中的缘由是什么呢。首先，一个具有population sparsity的特征分布在特征矩阵里会存在很多非激活的元素（为0的元素）。而且，因为满足high dispersal，这些零元素会近似均匀的分布在所有的特征里。因此，每一个特征必然会有一定数量的零元素，从而保证了lifetime sparsity。所以，对于population sparsity和high dispersal的优化就已经足够得到一个好的特征描述了。
**四、****Deep sparse filtering**
因为sparse filtering的目标函数是不可知的，我们可以自由地选择一个前向网络来计算这些特征。一般来说，我们都使用比较复杂的非线性函数，例如：
![](https://img-blog.csdn.net/20130815143451578?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvem91eHkwOQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
或者多层网络来计算这些特征。这样，sparse filtering也算是训练深度网络的一种自然的框架了。
有sparse filtering的深度网络可以使用权威的逐层贪婪算法来训练。我们可以先用sparse filtering来训练得到一个单层的归一化的特征，然后用它当成第二层的输入去训练第二层，其他层一样。实验中，我们发现，它能学习到很有意义的特征表达。
**五、实验与分析**
在我们的实验中，我们采用软绝对值函数作为我们的激活函数：
![](https://img-blog.csdn.net/20130815143459906?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvem91eHkwOQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
其中ɛ=10^-8，然后用现有的L-BFGS算法来优化sparse filtering的目标函数直至收敛。
![](https://img-blog.csdn.net/20130815143517390?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvem91eHkwOQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
其他的实验结果看原文。
**六、总结与讨论**
**6.1****、与****divisive normalization****的联系**
sparse filtering的population sparsity和divisive normalization有着紧密的联系。divisive normalization是一种视觉处理过程，一个神经元的响应会除以其邻域所有神经元的响应的和（或加权和）。divisive normalization在多级目标识别中是很有效的。然而，它被当成一个预处理阶段引入，而不是属于非监督学习（预训练）的一部分。实际上，sparse filtering把divisive normalization结合到了特征学习过程中，通过让特征间竞争，学习到满足population sparse的特征表达。
**6.2****、与****ICA****和****sparse coding****的联系**
sparse filtering的目标函数可以看出ICA目标函数的归一化版本。在ICA中，目标函数是最小化线性滤波器组的响应，例如||**Wx**||1，但需要满足滤波器间相互正交的约束。正交约束保证我们学习到的特征是不同的。在sparse filtering中，我们用一个归一化的稀疏惩罚来替代这个约束，每个滤波器的响应都会除以所有滤波器响应的二范数||**Wx**||1/||**Wx**||2，在这里，就给滤波器引入了竞争，而不需要添加正交化。
同样，我们也可以将归一化的思想运用到sparse coding框架。在sparse coding中，类似于L1/L2的稀疏惩罚已经被使用了。我们用一个归一化的惩罚，例如||**s**||1/||**s**||2来替换一般的L1范数的惩罚，例如||**s**||1。归一化惩罚具有尺度不变性，对于数据的变化可以更加鲁棒。


