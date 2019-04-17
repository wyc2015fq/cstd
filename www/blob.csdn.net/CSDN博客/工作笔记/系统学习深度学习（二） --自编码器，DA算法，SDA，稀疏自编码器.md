# 系统学习深度学习（二） --自编码器，DA算法，SDA，稀疏自编码器 - 工作笔记 - CSDN博客





2017年01月10日 13:21:56[Eason.wxd](https://me.csdn.net/App_12062011)阅读数：10899








转自：http://www.cnblogs.com/neopenx/p/4370350.html，作者写的很好，轻松易懂。



起源：PCA、特征提取....



随着一些奇怪的高维数据出现，比如图像、语音，传统的统计学-机器学习方法遇到了前所未有的挑战。

数据维度过高，数据单调,噪声分布广，传统方法的“数值游戏”很难奏效。数据挖掘？已然挖不出有用的东西。

为了解决高维度的问题，出现的线性学习的PCA降维方法，PCA的数学理论确实无懈可击，但是却只对线性数据效果比较好。

于是，寻求简单的、自动的、智能的特征提取方法仍然是机器学习的研究重点。比如LeCun在1998年[CNN总结性论文](http://ieeexplore.ieee.org/xpl/articleDetails.jsp?reload=true&arnumber=726791)中就概括了今后机器学习模型的基本架构。

![](https://img-blog.csdn.net/20170110152459546?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvQXBwXzEyMDYyMDEx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

当然CNN另辟蹊径，利用卷积、降采样两大手段从信号数据的特点上很好的提取出了特征。对于一般非信号数据，该怎么办呢？？

**Part I  自动编码器（AutoEncoder）**

自动编码器基于这样一个事实：原始input（设为x）经过加权（W、b)、映射（Sigmoid）之后得到y，再对y反向加权映射回来成为z。

通过反复迭代训练两组（W、b），使得误差函数最小，即尽可能保证z近似于x，即完美重构了x。

那么可以说正向第一组权（W、b）是成功的，很好的学习了input中的关键特征，不然也不会重构得如此完美。结构图如下：![](https://img-blog.csdn.net/20170110152529188?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvQXBwXzEyMDYyMDEx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

从生物的大脑角度考虑，可以这么理解，学习和重构就好像编码和解码一样。

![](https://img-blog.csdn.net/20170110152540205?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvQXBwXzEyMDYyMDEx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

这个过程很有趣，首先，它没有使用数据标签来计算误差update参数，所以是无监督学习。

其次，利用类似神经网络的双隐层的方式，简单粗暴地提取了样本的特征。 

这个双隐层是有争议的，最初的编码器确实使用了两组（W，b），但是Vincent在2010年的[论文](http://jmlr.org/papers/volume11/vincent10a/vincent10a.pdf)中做了研究，发现只要单组W就可以了。

即W'=WT, W和W’称为Tied Weights。实验证明，W'真的只是在打酱油，完全没有必要去做训练。

逆向重构矩阵让人想起了逆矩阵，若W-1=WT的话，W就是个正交矩阵了，即W是可以训成近似正交阵的。

由于W'就是个酱油，训练完之后就没它事了。正向传播用W即可，相当于为input预先编个码，再导入到下一layer去。所以叫自动编码器，而不叫自动编码解码器。

我当时看到这个过程和概念，也纠结明明是编解码器，为什么又叫编码器。

**Part II 降噪自动编码器（Denoising Autoencoder）**

Vincent在2008年的[论文](http://www.iro.umontreal.ca/~lisa/publications2/index.php/publications/show/217)中提出了AutoEncoder的改良版——dA。推荐首先去看这篇paper。

论文的标题叫 "Extracting and Composing Robust Features"，译成中文就是"提取、编码出具有鲁棒性的特征"

怎么才能使特征很鲁棒呢？就是以一定概率分布（通常使用二项分布）去擦除原始input矩阵，即每个值都随机置0,  这样看起来部分数据的部分特征是丢失了。

以这丢失特征的数据x'去计算y，计算z，并将z与原始x做误差迭代，这样，网络就学习了这个破损（原文叫Corruputed）的数据。

这个破损的数据是很有用的，原因有二：

**其之一，通过与非破损数据训练的对比，破损数据训练出来的Weight噪声比较小。**降噪因此得名。

原因不难理解，因为擦除的时候不小心把输入噪声给×掉了。

![](https://img-blog.csdn.net/20170110152551237?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvQXBwXzEyMDYyMDEx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

**其之二，破损数据一定程度上减轻了训练数据与测试数据的代沟。**由于数据的部分被×掉了，因而这破损数据

一定程度上比较接近测试数据。（训练、测试肯定有同有异，当然我们要求同舍异）。

这样训练出来的Weight的鲁棒性就提高了。图示如下：

![](https://img-blog.csdn.net/20170110152602048?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvQXBwXzEyMDYyMDEx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

关键是，这样胡乱擦除原始input真的很科学？真的没问题？  Vincent又从大脑认知角度给了解释：

![](https://img-blog.csdn.net/20170110152611768?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvQXBwXzEyMDYyMDEx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

paper中这么说到：人类具有认知被阻挡的破损图像能力，此源于我们高等的联想记忆感受机能。

我们能以多种形式去记忆（比如图像、声音，甚至如上图的词根记忆法），所以即便是数据破损丢失，我们也能回想起来。

另外，就是从特征提取的流形学习**(Manifold Learning)**角度看：


![](https://img-blog.csdn.net/20170110162715327?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvQXBwXzEyMDYyMDEx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

破损的数据相当于一个简化的PCA，把特征做一个简单的降维预提取。

**Part III  自动编码器的奇怪用法**

自动编码器相当于创建了一个隐层，一个简单想法就是加在深度网络的开头，作为原始信号的初级filter，起到降维、提取特征的效果。

关于自动编码器取代PCA的基本用法，参考 [http://www.360doc.com/content/15/0324/08/20625606_457576675.shtml](http://www.360doc.com/content/15/0324/08/20625606_457576675.shtml)

当然Bengio在2007年[论文](http://www.iro.umontreal.ca/~lisa/publications2/index.php/publications/show/190)中仿照DBN较之于RBM做法：作为深度网络中各个layer的参数初始化值，而不是用随机小值。

即变成了[Stacked AutoEncoder](http://www.cnblogs.com/neopenx/p/4378635.html)。

当然，这种做法就有一个问题，AutoEncoder可以看作是PCA的非线性补丁加强版，PCA的取得的效果是建立在降维基础上的。

仔细想想CNN这种结构，随着layer的推进，每层的神经元个数在递增，如果用了AutoEncoder去预训练，岂不是增维了？真的没问题？

paper中给出的实验结果认为AutoEncoder的增维效果还不赖，原因可能是非线性网络能力很强，尽管神经元个数增多，但是每个神经元的效果在衰减。

同时，随机梯度算法给了后续监督学习一个良好的开端。整体上，增维是利大于弊的。

Part IV  代码与实现

具体参考  [http://deeplearning.net/tutorial/dA.html](http://deeplearning.net/tutorial/dA.html)

有几个注意点说下：

①cost函数可以使用交错熵(Cross Entroy)设计，对于定义域在[0,1]这类的数据，交错熵可用来设计cost函数。

其中Logistic回归的似然函数求导结果可看作是交错熵的特例。参考 [http://en.wikipedia.org/wiki/Cross_entropy](http://en.wikipedia.org/wiki/Cross_entropy)

也可以使用最小二乘法设计。

②RandomStreams函数存在多个，因为要与非Tensor量相乘，必须用shared版本。

所以是  fromtheano.tensor.shared_randomstreamsimportRandomStreams

而不是 from theano.tensor import RandomStreams

转自：http://blog.csdn.net/zouxy09/article/details/8775524

下面是另一篇博客，我做了一点删减。

**AutoEncoder自动编码器**

        Deep Learning最简单的一种方法是利用人工神经网络的特点，人工神经网络（ANN）本身就是具有层次结构的系统，如果给定一个神经网络，我们假设其输出与输入是相同的，然后训练调整其参数，得到每一层中的权重。自然地，我们就得到了输入I的几种不同表示（每一层代表一种表示），这些表示就是特征。自动编码器就是一种尽可能复现输入信号的神经网络。为了实现这种复现，自动编码器就必须捕捉可以代表输入数据的最重要的因素，就像PCA那样，找到可以代表原信息的主要成分。

       具体过程简单的说明如下：

**给定无标签数据，用非监督学习学习特征：**

![](https://img-my.csdn.net/uploads/201304/09/1365439723_4504.jpg)

       在我们之前的神经网络中，如第一个图，我们输入的样本是有标签的，即（input, target），这样我们根据当前输出和target（label）之间的差去改变前面各层的参数，直到收敛。但现在我们只有无标签数据，也就是右边的图。那么这个误差怎么得到呢？

![](https://img-my.csdn.net/uploads/201304/09/1365439745_1862.jpg)

        如上图，我们将input输入一个encoder编码器，就会得到一个code，这个code也就是输入的一个表示，那么我们怎么知道这个code表示的就是input呢？我们加一个decoder解码器，这时候decoder就会输出一个信息，那么如果输出的这个信息和一开始的输入信号input是很像的（理想情况下就是一样的），那很明显，我们就有理由相信这个code是靠谱的。所以，我们就通过调整encoder和decoder的参数，使得重构误差最小，这时候我们就得到了输入input信号的第一个表示了，也就是编码code了。因为是无标签数据，所以误差的来源就是直接重构后与原输入相比得到。

![](https://img-my.csdn.net/uploads/201304/09/1365439780_9725.jpg)



**Denoising AutoEncoders降噪自动编码器：**

        降噪自动编码器DA是在自动编码器的基础上，训练数据加入噪声，所以自动编码器必须学习去去除这种噪声而获得真正的没有被噪声污染过的输入。因此，这就迫使编码器去学习输入信号的更加鲁棒的表达，这也是它的泛化能力比一般编码器强的原因。DA可以通过梯度下降算法去训练。

![](https://img-my.csdn.net/uploads/201304/09/1365439902_7892.jpg)

**通过编码器产生特征，然后训练下一层。这样逐层训练（这里说的是SDA）：**

       那上面我们就得到第一层的code，我们的重构误差最小让我们相信这个code就是原输入信号的良好表达了，或者牵强点说，它和原信号是一模一样的（表达不一样，反映的是一个东西）。那第二层和第一层的训练方式就没有差别了，我们将第一层输出的code当成第二层的输入信号，同样最小化重构误差，就会得到第二层的参数，并且得到第二层输入的code，也就是原输入信息的第二个表达了。其他层就同样的方法炮制就行了（训练这一层，前面层的参数都是固定的，并且他们的decoder已经没用了，都不需要了）。

![](https://img-my.csdn.net/uploads/201304/09/1365439805_4681.jpg)

**有监督微调：**

      经过上面的方法，我们就可以得到很多层了。至于需要多少层（或者深度需要多少，这个目前本身就没有一个科学的评价方法）需要自己试验调了。每一层都会得到原始输入的不同的表达。当然了，我们觉得它是越抽象越好了，就像人的视觉系统一样。

       到这里，这个AutoEncoder还不能用来分类数据，因为它还没有学习如何去连结一个输入和一个类。它只是学会了如何去重构或者复现它的输入而已。或者说，它只是学习获得了一个可以良好代表输入的特征，这个特征可以最大程度上代表原输入信号。那么，为了实现分类，我们就可以在AutoEncoder的最顶的编码层添加一个分类器（例如罗杰斯特回归、SVM等），然后通过标准的多层神经网络的监督训练方法（梯度下降法）去训练。

        也就是说，这时候，我们需要将最后层的特征code输入到最后的分类器，通过有标签样本，通过监督学习进行微调，这也分两种，一个是只调整分类器（黑色部分）：

![](https://img-my.csdn.net/uploads/201304/09/1365439828_1081.jpg)

       另一种：通过有标签样本，微调整个系统：（如果有足够多的数据，这个是最好的。end-to-end learning端对端学习）

![](https://img-my.csdn.net/uploads/201304/09/1365439852_7450.jpg)

       一旦监督训练完成，这个网络就可以用来分类了。神经网络的最顶层可以作为一个线性分类器，然后我们可以用一个更好性能的分类器去取代它。

       在研究中可以发现，如果在原有的特征中加入这些自动学习得到的特征可以大大提高精确度，甚至在分类问题中比目前最好的分类[算法](http://lib.csdn.net/base/datastructure)效果还要好！

        AutoEncoder存在一些变体，这里简要介绍下两个：

**Sparse AutoEncoder稀疏自动编码器：**

      当然，我们还可以继续加上一些约束条件得到新的Deep Learning方法，如：如果在AutoEncoder的基础上加上L1的正则限制（L1主要是约束每一层中的节点中大部分都要为0，只有少数不为0，这就是Sparse名字的来源），我们就可以得到Sparse AutoEncoder法。

![](https://img-my.csdn.net/uploads/201304/09/1365439878_3585.jpg)

       如上图，其实就是限制每次得到的表达code尽量稀疏。因为稀疏的表达往往比其他的表达要有效（人脑好像也是这样的，某个输入只是刺激某些神经元，其他的大部分的神经元是受到抑制的）。



**Sparse Coding稀疏编码**

       如果我们把输出必须和输入相等的限制放松，同时利用线性代数中基的概念，即O = a1*Φ1 + a2*Φ2+….+ an*Φn， Φi是基，ai是系数，我们可以得到这样一个优化问题：

Min |I – O|，其中I表示输入，O表示输出。

       通过求解这个最优化式子，我们可以求得系数ai和基Φi，这些系数和基就是输入的另外一种近似表达。

![](https://img-my.csdn.net/uploads/201304/09/1365483354_9315.jpg)

       因此，它们可以用来表达输入I，这个过程也是自动学习得到的。如果我们在上述式子上加上L1的Regularity限制，得到：

Min |I – O| + u*(|a1| + |a2| + … + |an
 |)

        这种方法被称为Sparse Coding。通俗的说，就是将一个信号表示为一组基的线性组合，而且要求只需要较少的几个基就可以将信号表示出来。“稀疏性”定义为：只有很少的几个非零元素或只有很少的几个远大于零的元素。要求系数 ai 是稀疏的意思就是说：对于一组输入向量，我们只想有尽可能少的几个系数远大于零。选择使用具有稀疏性的分量来表示我们的输入数据是有原因的，因为绝大多数的感官数据，比如自然图像，可以被表示成少量基本元素的叠加，在图像中这些基本元素可以是面或者线。同时，比如与初级视觉皮层的类比过程也因此得到了提升（人脑有大量的神经元，但对于某些图像或者边缘只有很少的神经元兴奋，其他都处于抑制状态）。

         稀疏编码[算法](http://lib.csdn.net/base/datastructure)是一种无监督学习方法，它用来寻找一组“超完备”基向量来更高效地表示样本数据。虽然形如主成分分析技术（PCA）能使我们方便地找到一组“完备”基向量，但是这里我们想要做的是找到一组“超完备”基向量来表示输入向量（也就是说，基向量的个数比输入向量的维数要大）。超完备基的好处是它们能更有效地找出隐含在输入数据内部的结构与模式。然而，对于超完备基来说，系数ai不再由输入向量唯一确定。因此，在稀疏编码算法中，我们另加了一个评判标准“稀疏性”来解决因超完备而导致的退化（degeneracy）问题。（[详细过程请参考：UFLDL
 Tutorial稀疏编码](http://deeplearning.stanford.edu/wiki/index.php/%E7%A8%80%E7%96%8F%E7%BC%96%E7%A0%81)）

![](https://img-my.csdn.net/uploads/201304/09/1365483386_5095.jpg)

       比如在图像的Feature Extraction的最底层要做Edge Detector的生成，那么这里的工作就是从Natural Images中randomly选取一些小patch，通过这些patch生成能够描述他们的“基”，也就是右边的8*8=64个basis组成的basis，然后给定一个test patch, 我们可以按照上面的式子通过basis的线性组合得到，而sparse
 matrix就是a，下图中的a中有64个维度，其中非零项只有3个，故称“sparse”。

       这里可能大家会有疑问，为什么把底层作为Edge Detector呢？上层又是什么呢？这里做个简单解释大家就会明白，之所以是Edge Detector是因为不同方向的Edge就能够描述出整幅图像，所以不同方向的Edge自然就是图像的basis了……而上一层的basis组合的结果，上上层又是上一层的组合basis……（就是上面第四部分的时候咱们说的那样）

       Sparse coding分为两个部分：

**1）Training阶段：**给定一系列的样本图片[x1, x 2, …]，我们需要学习得到一组基[Φ1, Φ2, …]，也就是字典。

       稀疏编码是k-means算法的变体，其训练过程也差不多（EM算法的思想：如果要优化的目标函数包含两个变量，如L(W, B)，那么我们可以先固定W，调整B使得L最小，然后再固定B，调整W使L最小，这样迭代交替，不断将L推向最小值。EM算法可以见我的博客：“[从最大似然到EM算法浅解](http://blog.csdn.net/zouxy09/article/details/8537620)”）。

       训练过程就是一个重复迭代的过程，按上面所说，我们交替的更改a和Φ使得下面这个目标函数最小。

![](https://img-my.csdn.net/uploads/201304/09/1365483429_5706.jpg)

      每次迭代分两步：

a）固定字典Φ[k]，然后调整a[k]，使得上式，即目标函数最小（即解LASSO问题）。

b）然后固定住a [k]，调整Φ [k]，使得上式，即目标函数最小（即解凸QP问题）。

      不断迭代，直至收敛。这样就可以得到一组可以良好表示这一系列x的基，也就是字典。

**2）Coding阶段：**给定一个新的图片x，由上面得到的字典，通过解一个LASSO问题得到稀疏向量**a**。这个稀疏向量就是这个输入向量x的一个稀疏表达了。

![](https://img-my.csdn.net/uploads/201304/09/1365483467_1398.jpg)

例如：

![](https://img-my.csdn.net/uploads/201304/09/1365483491_9524.jpg)







[](http://blog.csdn.net/zouxy09/article/details/8777094#)[](http://blog.csdn.net/zouxy09/article/details/8777094#)[](http://blog.csdn.net/zouxy09/article/details/8777094#)[](http://blog.csdn.net/zouxy09/article/details/8777094#)[](http://blog.csdn.net/zouxy09/article/details/8777094#)[](http://blog.csdn.net/zouxy09/article/details/8777094#)



