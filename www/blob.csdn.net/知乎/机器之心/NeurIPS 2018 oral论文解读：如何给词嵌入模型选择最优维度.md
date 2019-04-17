# NeurIPS 2018 oral论文解读：如何给词嵌入模型选择最优维度 - 知乎
# 



机器之心专栏，作者：殷子&沈圆圆（斯坦福大学）。

> 最近结束的 NeurIPS 2018 会议上，斯坦福大学的研究者们在论文《On the Dimensionality of Word Embedding》中提出了一个理解词嵌入维度的理论框架。通过推广词嵌入空间的几何不变性，这篇论文解决了该领域一个悬而未决的问题，即如何给模型选择合适的维度。该论文是大会 30 篇 oral 论文之一（top 0.6%），并受到自然语言处理领域的广泛关注。在这里，论文的两位作者将亲自为我们解读其中的核心思想。
![](https://pic2.zhimg.com/v2-7630acd5a13a502b7b6bd9cf668ba755_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='816' height='253'></svg>)
论文地址：[https://papers.nips.cc/paper/7368-on-the-dimensionality-of-word-embedding.pdf](https://link.zhihu.com/?target=https%3A//papers.nips.cc/paper/7368-on-the-dimensionality-of-word-embedding.pdf)

**引言**

在 2013 年一篇划时代的论文中，Milokov 等人提出了 Word2Vec 词嵌入（word embedding）算法。随后于 2014 年，斯坦福自然语言处理小组的 Pennington 等人提出了 GloVe 方法。自此之后，词嵌入成为了自然语言处理中最具影响力的工具。在当前的深度学习时代，词嵌入在诸如情感判别、阅读理解、机器翻译等各种任务上几乎随处可见。词嵌入是深度学习模型应用到自然语言处理中不可或缺的一环：神经网络的输入是连续空间中的向量，而自然语言运用的则是离散的字符，词嵌入将自然语言转换成空间中的向量，是连接两者不可或缺的桥梁。

维度（dimensionality）是词嵌入模型里最重要的一个超参数。大家或许都训练过词嵌入模型，但是当我们训练的时候，有没有想过该如何选择维度这个超参数呢？纵观自然语言处理的论文，研究者对词嵌入维度的选择似乎都颇为随意。在 Word2Vec、GloVe 和 fastText 的原论文中，研究者们都选取 300 作为词向量的维度。这些颇有影响力的文章导致随后的研究者纷纷选择 300 维的词向量。除此之外，100、200 或者 50 也是常见的选择。然而，这些随意做出的选择并不能保证模型的最优性；事实上，大量的实证研究表明，词嵌入的学习过程也有欠拟合/过拟合的风险。如果维度选择得过小或过大，模型的表现将受其影响（见图 1）。普林斯顿大学的 Sanjeev Arora 教授在一篇博文中就探讨了这一现象，并提到这个现象并没有完善的理论解释。欠拟合/过拟合的存在导致研究者选择维度时需要格外谨慎。
![](https://pic3.zhimg.com/v2-94187c645e11533ee760ec9fb21dc1a6_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='507' height='462'></svg>)Figure 1 过小或过大的维度对词向量的表现产生负面影响
**一个词向量维度的理论框架**

在《On the dimensionality of word embedding》这篇文章中，作者探讨了如何通过理论研究理解词嵌入的维度对模型本身的影响。这篇文章的贡献主要有：
- 提出了 Pairwise Inner Product（PIP）损失函数。
- 利用矩阵微扰理论对词嵌入进行研究，并揭示了维度选择中的偏差-方差权衡（bias-variance tradeoff）。
- 从理论上证明了 Word2Vec 和 GloVe 对过拟合具有鲁棒性（robustness to overfitting）。
- 通过最小化 PIP 损失函数，提出了选择最优维度的理论指导。

**灵感来源**

维度选择是模型选择（model selection）的一种情形。在统计学习的框架中，模型选择有一套基于参数估计（parameter estimation）的完整理论。该理论通常可以分解为三个基本步骤：
- 定义一类估计量（class of estimators）
![](https://pic1.zhimg.com/v2-f26866e48937ebfaa9914ad5b3267440_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='23' height='24'></svg>)- 定义一个损失函数 
![](https://pic1.zhimg.com/v2-db0556662475889d0c2034ad68ddea38_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='118' height='33'></svg>)- 选择损失函数期望最小的估计量 
![](https://pic4.zhimg.com/v2-791411691de505da8350e6c8ddcd686f_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='301' height='38'></svg>)
该理论框架包括了大多数机器学习中的算法及模型，例如我们所熟悉的线性回归利用的就是线性估计量+平方损失函数。再如，多层感知机（multi-layer perceptron）利用的是全连接网络估计量+交叉熵损失函数。

不幸的是，这个理论框架并不能直接运用到词嵌入模型的选择中。事实上，目前的研究中并无词嵌入的损失函数。而且就算此类损失函数存在，我们也不知道有没有有效的方法去计算它。本文解决了这两方面的问题，并建立起一套基于矩阵微扰理论的维度选择框架。

**问题 1：词嵌入的损失函数**

在介绍文中提出的 Pairwise Inner Product（PIP）损失函数前，我们先简单复习一下词嵌入本身。假设有 n 个需要学习的词汇集
![](https://pic3.zhimg.com/v2-c6c0ee20e9668539a9c3e905d593014a_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='249' height='36'></svg>)
，词嵌入算法寻找的是从 V 到 
![](https://pic1.zhimg.com/v2-2793fdae122f3bebf70f82f3f96a4450_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='37' height='32'></svg>)
（d 维欧几里得空间）的一个映射：对每个词
![](https://pic2.zhimg.com/v2-91d8bf11419bf228c2f975995a03b539_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='39' height='28'></svg>)
，分配一个 d 维向量
![](https://pic4.zhimg.com/v2-937b434b0ae109f224445494910a42b3_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='31' height='24'></svg>)
。这个过程可以等效为寻找一个词嵌入矩阵（embedding matrix）E：E 的第 i 行是
![](https://pic1.zhimg.com/v2-8e8eacfe59e8e62be8be0f02c95652c0_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='31' height='26'></svg>)
。大家用 TensorFlow 训练深度模型时，一定都用过 tf.nn.embedding_lookup 这个算符。而我们研究的词嵌入矩阵就是这个算符中的第一个参数。
![](https://pic1.zhimg.com/v2-cc23e6dcdbe429597339c18efb6fd9d8_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='129'></svg>)
有许多算法可以帮助我们寻找这个映射，例如 Word2Vec 或 GloVe。然而，我们如何定义词嵌入间的损失函数？

词嵌入的蓬勃发展带动了一大批的深入研究。研究者们探讨如何提高词嵌入在不同任务上的表现，例如相似性（word similarity）和类比性（word analogy）。然而我们发现，这类的研究往往是「只见树木，不见森林」。本文提出的 PIP 损失函数所研究的是嵌入的几何性质。

**词嵌入的几何性质与几何不变性**

词向量是一个典型的几何模型。在这个模型中，向量的几何性质与词语的语义性质存在着对应关系，这是多么美妙的巧合！最常见的几个例子有：
- 相似性。词语的相似性可以通过向量夹角的余弦值来表示。对于两个词向量
![](https://pic1.zhimg.com/v2-2a1a71b619d789f4bed42d9dc360dfd8_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='31' height='27'></svg>)
和
![](https://pic2.zhimg.com/v2-13b2832da65798e95914bc3a90361849_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='29' height='32'></svg>)
，它们夹角的余弦值为
![](https://pic2.zhimg.com/v2-59bf34e6140e2f1e53eedd3ba9dcf649_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='308' height='77'></svg>)![](https://pic4.zhimg.com/v2-0e7cd5cdb567ec557d872761a0b6aaf7_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='53' height='20'></svg>)
的范围在-1 到 1 之间。这个值越大，代表第 i 个和第 j 个词之间的相似度越高。在下图的例子中，love 和 adore 对应的向量之间夹角很小，而 love 和baseball 之间的夹角则较大。
![](https://pic4.zhimg.com/v2-777f82d9b17214351d430788a9f41fc7_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='474' height='420'></svg>)Figure 2 词向量的几何相似性- 类比性。类比性考察的是两对词语之间的性质是否相同。Mikolov 的论文中提出的例子 king-queen=man-woman 即体现了这一性质。类似的还有 walk-walking=run-running，Paris-France=Berlin-Germany 等。词类比这一语义性质对应的几何性质为平行度：两对词向量的差是否互相平行。
![](https://pic4.zhimg.com/v2-b47bd87df01c86340e08d26292216a57_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='527' height='434'></svg>)Figure 3 词向量的类比性- 聚类性。通过观察训练出的词向量，研究者们发现词向量可以被聚类：义类似的向量之间的距离总是较小，因而这些词会倾向于聚成一类。例如，距离狗（dog）最近的词向量是猫（cat）、鼠（mouse）等等，这些词都代表了动物。聚类性的几何对应为向量间的距离。
![](https://pic2.zhimg.com/v2-1d2526788659ca278c362cca57650e19_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='880' height='335'></svg>)Figure 4 词向量的聚类性
已知的超参数选择方法往往仅针对某一类语义学任务。比如，研究者们通过词向量在人工标注测试集（例如 WordSim353）上的表现来选择维度。既然我们不知道最优维度是多少，那么我们就通过暴力测试来解决！我们把 100-1000 维的词向量统统训练一遍，然后观察它们在测试集上的表现。通过这个方法，我们可以选择和人工标注数据集上表现最好的维度。然而这个解决方案却不甚优雅：训练许多词嵌入模型浪费了大量计算资源，并且有过拟合的风险：在相似性任务测试集上表现最佳的词嵌入可能在类比性任务中表现不佳。

究其原因，这种研究方法掉入了「只见树木，不见森林」的思维陷阱。这些不同语义学任务体现的只是词向量的部分几何学性质：
- 相似性：向量间夹角
- 类比性：向量的平行度
- 聚类性：向量间距离

既然这些任务都是碎片化的几何性质，那我们为什么不上升一个视角，去研究词嵌入整体的几何性质呢？这就是 PIP 损失函数的中心思想。

**酉不变（unitary invariant）性质**

我们注意到，无论是向量夹角，还是平行度，亦或是距离—它们的共同点都具有酉不变性。假如把词嵌入整体做一个旋转或者镜面对称，这些几何性质都不会发生变化。原因在于，酉变换并不改变向量之间的相对位置！这并不是一个巧合。本文作者从逻辑上论证了以下结论：语义学任务所对应的几何性质一定具有酉不变性。语义学的基础是分布式假设（distributional hypothesis），而该假设要求一个词的意义只能通过其他词来相对定义。正因如此，如果我们对词嵌入实施了一个保持向量间相对位置的变换（即酉变换），向量所对应的语义学任务不应有任何改变。相似性、类比性、聚类性等具体的语义学任务只是词嵌入酉不变性的部分体现。因而当我们寻找一个损失函数时，我们应该着重测量词嵌入酉不变性质之间的距离。PIP 损失就是这样的一个损失函数。假如我们有两个词嵌入矩阵
![](https://pic3.zhimg.com/v2-30b2e481e96c8b8b70a692764e7868ee_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='37' height='30'></svg>)
 和 
![](https://pic4.zhimg.com/v2-04294a8781e9a0c4973e557b8379b267_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='38' height='29'></svg>)
，它们之间的 PIP 损失函数定义如下：



![](https://pic1.zhimg.com/v2-a09cbf456705ef3ca469b2847a8e213c_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='377' height='37'></svg>)
注意到 PIP 损失函数的酉不变性：假如 
![](https://pic3.zhimg.com/v2-2b5c47d8eefdcb628c5c62d194e51a1e_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='25' height='21'></svg>)
 是一个酉矩阵（
![](https://pic3.zhimg.com/v2-5a53dbcc97a662892c7c5c5ebc305baa_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='203' height='28'></svg>)
），
![](https://pic1.zhimg.com/v2-da5bf7859d44c06df3e589376b934424_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='38' height='32'></svg>)
是
![](https://pic3.zhimg.com/v2-79051c876b9d5e766395647f3e6871fa_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='37' height='30'></svg>)
 的一个酉变换（
![](https://pic4.zhimg.com/v2-704e61babeeaff203e6a58ac712c1473_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='121' height='32'></svg>)
），那么易见 
![](https://pic1.zhimg.com/v2-7306687bc7e159f0816789788a383664_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='252' height='39'></svg>)
。除此之外，作者证明了 PIP 损失函数可以用来测量所有酉不变性质：假如
![](https://pic2.zhimg.com/v2-6e35dd4afe5a510454c397fbf6dae8f5_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='37' height='28'></svg>)
和
![](https://pic1.zhimg.com/v2-da5bf7859d44c06df3e589376b934424_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='38' height='32'></svg>)
之间的 PIP 损失较小，那么它们之间所有的酉不变性质都较为接近（包括夹角、平行度、距离等等），并且反之亦然。换句话说，PIP 损失函数测量的是「森林」，而无论是相似、类别还是聚类性，都只是其中的某棵树木。




PIP 损失函数有一个漂亮的几何解释。
![](https://pic4.zhimg.com/v2-d871f270ab841029f3e46a4dfb877e13_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='70' height='33'></svg>)
的第 i 行是一个 n 维向量
![](https://pic1.zhimg.com/v2-820d3226c26fa1d3eaaf395fae2fa870_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='334' height='28'></svg>)
。该向量可以看成是
![](https://pic4.zhimg.com/v2-068e834d77ed8a52c4c3e243883154c3_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='29' height='27'></svg>)
的相对坐标；其参照物是所有其他的词向量
![](https://pic3.zhimg.com/v2-08842865f3d8f38a2ed577d348e57aa2_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='32' height='25'></svg>)
至
![](https://pic1.zhimg.com/v2-25f864edcbf508c1bf487e234912c4e4_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='35' height='24'></svg>)
（见下图）。在 PIP 损失函数中，当我们比较
![](https://pic4.zhimg.com/v2-5f64de80270cfa803c9416e65a46937b_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='70' height='36'></svg>)
和
![](https://pic3.zhimg.com/v2-0c48c4e5e3e5c46abf6e1376d6f7b5f2_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='72' height='33'></svg>)
的差时，我们测量的其实是词向量们的相对位移。通过建立这样的相对坐标系，我们摆脱了对绝对坐标系的依赖，从而得到了一个酉不变的损失函数。



![](https://pic2.zhimg.com/v2-30bfb30f05739140fc5e0b67cb99d8e5_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='327'></svg>)Figure 5 PIP 损失函数的几何解释
**问题 2：对 PIP 损失函数的矩阵微扰分析**

至此，我们已经解决了第一个难题：如何定义词嵌入的损失函数。而我们如何分析一个通过训练得到的词嵌入
![](https://pic4.zhimg.com/v2-e2e057c5d097c22e9043d11336428787_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='28' height='30'></svg>)
 和未知的最优词嵌入
![](https://pic4.zhimg.com/v2-53197e4ae63f0c2f1f9217ed14d63953_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='33' height='27'></svg>)
之间的 PIP 损失，从而找到最优维度呢？

我们发现多数的词嵌入算法本质上都是矩阵分解。例如潜在语义分析（Latent Semantic Analysis, LSA）通过奇异值分解（SVD）得到词向量，而最近的论文证明了 Skip-gram Word2Vec 和 GloVe 都是通过随机梯度的优化方法实现某类矩阵（统称为信号矩阵）的分解。既然词嵌入是通过分解信号矩阵得到的，我们自然想到利用矩阵分析的方法来了解 PIP 损失函数。本文的作者通过将词嵌入转化为带噪音的矩阵分解问题，开发了一套基于微扰理论（perturbation theory）的分析技术。通过这个技术，作者发现 PIP 损失函数有偏差-方差分解（bias-variance decomposition）。文章的主定理证明了如下结论：
![](https://pic3.zhimg.com/v2-5db27c18b3d3db0139401ca15d8f8f4e_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='478'></svg>)
该定理将训练得到的词嵌入矩阵
![](https://pic4.zhimg.com/v2-e2e057c5d097c22e9043d11336428787_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='28' height='30'></svg>)
与未知的最优词嵌入矩阵 
![](https://pic4.zhimg.com/v2-00d0491857a52d7be99f148844cfaecb_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='33' height='24'></svg>)
之间的 PIP 损失分解为三项：其中第一项为偏差（bias），第二和第三项均为方差（variance）。这三项也对应了一个基于信噪比的解释，其中信号为信号矩阵的频谱（spectrum），而噪音为信号矩阵的估计误差：
- 第一项：当我们选择一个 k 维的词嵌入时，我们丢失了 k+1 维及之后的信号，这导致了模型产生偏差。该项随着 k 的增大而减小。
- 第二项：噪音的存在导致我们对信号矩阵谱的大小产生估计误差。随着 k 的增大，更多的谱信号被引入，导致该项随着 k 的增大而增加。
- 第三项：同样由于噪音的存在，我们对信号矩阵谱的方向产生估计误差。同理于第二项，该项随着 k 增大而增加。

这个偏差-方差分解直接证明了最佳维度的存在。并且通过寻找偏差和方差的平衡点，我们可以直接求解理论上最优的维度！下图是 GloVe 算法在 Text8 数据集上的 PIP 损失函数。图中明显可见偏差和方差的存在，并且注意在 700 维左右，偏差与方差的和最小。这个就是最优维度。
![](https://pic1.zhimg.com/v2-3e063e1755996900cce3b6644d5f8c18_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='680' height='531'></svg>)Figure 6 PIP 损失函数的偏差-方差分解及最优维度
这套理论完整解释了维度对词嵌入的影响：当选取的维度过低时，词嵌入将有较大的偏差（high bias），因为模型丢失了较多信号。而维度过高时，词嵌入将有较大的方差（high-variance），因为模型中夹杂了大量的噪音。

文中比较了通过最小化 PIP 损失函数得到的最优维度在语义学任务中的表现。这些维度几乎达到最优表现。
![](https://pic3.zhimg.com/v2-5a43aa6699142aa9bb728767c21b1a82_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='314'></svg>)
通过对 PIP 损失函数的深入分析，作者证明了常用的词嵌入算法（例如 Word2Vec 和 GloVe）对过拟合具有鲁棒性。这个发现被实验所进一步证明（图 7 和图 8）。当 Word2Vec 和 GloVe 的维度选取过大时，由于过拟合的存在，它们在词义相似性任务上的表现有所下降；但是下降的幅度随维度的增加并不明显。甚至当维度达到 10000 时，它们的表现仍然处在可以接受的范围内。
![](https://pic2.zhimg.com/v2-a77160e6990fb7d0311f8502a8eedcdd_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='352'></svg>)Figure 7 过拟合对 Word2Vec 的影响


![](https://pic1.zhimg.com/v2-7b66c672869ed46ca4a1e10b94345ce4_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='349'></svg>)Figure 8 过拟合对 GloVe 的影响
本文用于词向量维度选择的代码已经在 GitHub 上放出：

[https://github.com/ziyin-dl/word-embedding-dimensionality-selection](https://link.zhihu.com/?target=https%3A//github.com/ziyin-dl/word-embedding-dimensionality-selection)[ziyin-dl/word-embedding-dimensionality-selection​github.com![图标](https://pic3.zhimg.com/v2-b2fb98cc8625d413b58aee4f81b8469e_ipico.jpg)](https://link.zhihu.com/?target=https%3A//github.com/ziyin-dl/word-embedding-dimensionality-selection)[https://github.com/ziyin-dl/word-embedding-dimensionality-selection](https://link.zhihu.com/?target=https%3A//github.com/ziyin-dl/word-embedding-dimensionality-selection)

目前源代码支持 Word2Vec、GloVe 和 LSA 三种算法。使用者只需要提供一个用来训练词嵌入的文本（txt 或 zip 格式），算法即可以自动选择该文本上词嵌入的最佳维度。
*![](https://pic1.zhimg.com/v2-18cc987d5f379a82f1208b6d90722318_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='73' height='24'></svg>)*
参考资料：
- Sanjeev Arora. Word embeddings: Explaining their properties, 2016. URL [http://www](https://link.zhihu.com/?target=http%3A//www). [http://offconvex.org/2016/02/14/word-embeddings-2/](https://link.zhihu.com/?target=http%3A//offconvex.org/2016/02/14/word-embeddings-2/). [Online; accessed 16-May-2018].
- Omer Levy and Yoav Goldberg. Neural word embedding as implicit matrix factorization. In Advances in neural information processing systems, pages 2177–2185, 2014.
- Tomas Mikolov, Kai Chen, Greg Corrado, and Jeffrey Dean. Efficient estimation of word representations in vector space. arXiv preprint arXiv:1301.3781, 2013a.
- Zi Yin and Yuanyuan Shen. On the dimensionality of word embedding. In Advances in Neural Information Processing Systems, 2018.


