# 【Python学习系列二十三】Scikit_Learn库降维方法(矩阵分解)-PCA&FA - 专注于数据挖掘算法研究和应用 - CSDN博客





2017年07月11日 14:34:59[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：3963
所属专栏：[python专栏](https://blog.csdn.net/column/details/16333.html)









## 1主成分分析PCA

### 1.1 精确PCA和似然估计

PCA基于最大方差的正交变量分解多维数据集。在scikit-learn库中，PCA的实现是先通过fit方法计算n维的特征值和特征向量，然后通过transformer对象做数据转换，转换后的新数据可以在n维上得到映射。

可选参数whiten =True使得可以将数据投射到奇异矩阵上，同时将每个属性缩放到单位方差。这对下步要学习的模型来说是非常有用的，如果下步要学习的模型具有均质性，如RBF内核的SVM支持向量机和K-Means聚类算法。

Iris数据集有4个特征，PCA后可以用最大方差的2个维度来映射，效果如图：

![](https://img-blog.csdn.net/20170711142718868?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZmpzc2hhcnBzd29yZA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

PCA对象还给出概率估算，可以根据其方差量给出数据的可能性。因此，PCA也实现了一个评分方法可以用来做交叉验证。

![](https://img-blog.csdn.net/20170711142834757?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZmpzc2hhcnBzd29yZA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




实现案例：

1)iris数据集应用LDA和PCA后降维后的2D投射效果比较

http://scikit-learn.org/stable/auto_examples/decomposition/plot_pca_vs_lda.html#sphx-glr-auto-examples-decomposition-plot-pca-vs-lda-py

2)基于概率PCA和因子分析FA的模型选择

http://scikit-learn.org/stable/auto_examples/decomposition/plot_pca_vs_fa_model_selection.html#sphx-glr-auto-examples-decomposition-plot-pca-vs-fa-model-selection-py

### 1.2 增量PCA

PCA对象是非常有用，不过对大容量数据集来说也有不足。最大的局限是PCA只支持批量处理，这就意味着所有要处理的数据要在主存中完成fit处理。增量PCA对象使用了不同的处理方法，它只处理小型数据进行部分计算，就能最大程度接近PCA结果。

增量PCA使实现非核心主成分分析(out-of-corePrincipal Component Analysis)成为可能，步骤：

1)对从网络数据库或硬盘中一次取出来的数据块使用partial_fit方法；

2)使用numpy.memmap在内存映射文件上调用fit方法。
增量PCA仅存储特征和噪声方差的估计，目的是更新explained_variance_ratio_incrementally。这就是为什么内存使用取决于每个批次的样本数量，而不是在数据集中要处理的样本数。

![](https://img-blog.csdn.net/20170711142945683?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZmpzc2hhcnBzd29yZA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


![](https://img-blog.csdn.net/20170711142955745?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZmpzc2hhcnBzd29yZA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




案例：

1）增量PCA。

http://scikit-learn.org/stable/auto_examples/decomposition/plot_incremental_pca.html#sphx-glr-auto-examples-decomposition-plot-incremental-pca-py

### 1.3 基于随机SVD的PCA

通过抛弃与较低奇异值相关联的分量的奇异向量，能将数据投影较低维度空间，同时又保留大部分差异。

举个例子，如果我们对64x64像素灰度图片进行脸部识别，数据的维度是4096，在如此广泛的数据上训练RBF支持向量机是很慢的。而且，我们知道实际有用的维度远远少于4096，因为所有人脸的照片看起来都相像。

样本集取决于较低维度上的维度数，例如说约200。PCA算法可用于线性变换数据，同时降低维数并同时保留最大的差异性。

在这样的情况下，PCA类使用可选参数svd_solver='randomized'是非常有用的。在执行实际变换时，由于我们将要抛弃大部分奇异向量，所以将计算限制为奇异向量的近似估计是更有效的。
举个例子，以下显示的16个样本肖像(以0.0为中心)来自Olivetti数据集。下图是依据前16个奇异矢量重画的肖像。由于只需计算前16个奇异向量的数据集，该数据集有400个样本和4096个特征，计算时间少于1秒。

![](https://img-blog.csdn.net/20170711143045902?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZmpzc2hhcnBzd29yZA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


![](https://img-blog.csdn.net/20170711143055185?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZmpzc2hhcnBzd29yZA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


![](https://img-blog.csdn.net/20170711143145948?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZmpzc2hhcnBzd29yZA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




案例：

1）使用特征脸和SVM进行人脸识别

http://scikit-learn.org/stable/auto_examples/applications/face_recognition.html#sphx-glr-auto-examples-applications-face-recognition-py

2）人脸数据集降维

http://scikit-learn.org/stable/auto_examples/decomposition/plot_faces_decomposition.html#sphx-glr-auto-examples-decomposition-plot-faces-decomposition-py

参考：

1）随机性结构查找：构造近似矩阵分解的随机算法

https://arxiv.org/abs/0909.4061

### 1.4 内核PCA
内核PCA是PCA的扩展，通过使用内核函数实现非线性降维，包括去噪、压缩和结构预测（内核依赖估计）功能。内核PCA支持transform和inverse_transform。

![](https://img-blog.csdn.net/20170711143220662?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZmpzc2hhcnBzd29yZA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)







案例：

1)内核PCA

http://scikit-learn.org/stable/auto_examples/decomposition/plot_kernel_pca.html#sphx-glr-auto-examples-decomposition-plot-kernel-pca-py

### 1.5 稀疏PCA和微稀疏PCA

稀疏PCA是PCA的变异，目的是抽取数据最优重建的稀疏向量集合。微稀疏PCA是稀疏PCA的变异，速度更快但精度低；对于给定的迭代次数，通过迭代该组特征的小量数据来达到增加的速度。

PCA的缺点是通过该方法抽取的特征是完全稠密的，即当它们表示为原始变量的线性组合时，它们具有非零系数，这给高低维映射带来困难。在很多情况下，真正的潜在向量可以自然地假设为稀疏向量，例如在面部识别中，向量可能自然地映射到人脸的部分。

稀疏主成分的产生更简洁、可解释的表示、明确强调哪些原始特征有助于样本之间的差异。
下面例子阐述了对来自Olivetti人脸数据集通过稀疏PCA抽取16个向量，可以看到正则化方法是如何生成很多零特征。此外，数据的自然结构导致非零系数垂直相邻。



模型不会在数学上强制执行：每个特征是一个向量，![](https://img-blog.csdn.net/20170711143312749?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZmpzc2hhcnBzd29yZA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)，并没有垂直相邻性的概念，除非在人脸友好的可视化中作为64x64像素的图像。
可视局部的内在特征受数据固有结构所影响，使得局部模式的重构误差最小化；也存在由稀疏性带来的规则，这些规则考虑到了结构间的相似性和相异性。回顾稀疏PCA方法，在使用方面的更多细节，可以看下文的示例部分。

![](https://img-blog.csdn.net/20170711143347815?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZmpzc2hhcnBzd29yZA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


![](https://img-blog.csdn.net/20170711143356614?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZmpzc2hhcnBzd29yZA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




请注意，对稀疏PCA问题来说有很多不同的提法，其中之一就是本文所提及到的。最优问题的解决是基于L1范数正则惩罚项的PCA问题（字典学习）。

![](https://img-blog.csdn.net/20170711143436352?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZmpzc2hhcnBzd29yZA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


在几乎没有训练样本可用时， L1正则导致的稀疏性能防止从噪音中学习特征。惩罚程度（因而稀疏度）可以通过超参数alpha进行调整。小值导致了温和的正则化因式分解，而较大的值将许多系数缩小到零。

注意：本着在线算法的原则，由于MiniBatchSparsePCA类并没有实现partial_fit方法，因为算法沿着特征方向处理，而不是样本方向。

案例：

1）人脸数据集降维

http://scikit-learn.org/stable/auto_examples/decomposition/plot_faces_decomposition.html#sphx-glr-auto-examples-decomposition-plot-faces-decomposition-py

参考：

1）在线词典学习稀疏编码

http://www.di.ens.fr/sierra/pdfs/icml09.pdf

2）结构化稀疏主成分分析

链接失效，可以直接到百度学术以文章名搜索该文。



## 2 截断奇异值分解(TSVD)和潜在语义分析

TSVD是SVD的变异，它只计算k个最大奇异值，其中k是用户定义的参数。当TSVD应用于主题-文档矩阵时（由CountVectorizer或TfidfVectorizer返回），这种转换被称为潜在语义分析（LSA），因为它将主题-文档矩阵转换成低维度的“语义”空间。特别的是，LSA可以消除同义词和多义词的影响（这两个都带来词的多重含义），这导致主题-文档矩阵过度稀疏，并且在诸如余弦相似性的度量下表现出差的相似性。

注意：LSA也被称为潜在语义索引LSI，尽管严格地说它是指在持久索引中用于信息检索的目的。

数学定义上，TSVD应用于训练向量空间X以生成低秩的近似X的向量空间：

![](https://img-blog.csdn.net/20170711143637957?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZmpzc2hhcnBzd29yZA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



注意：自然语言处理（NLP）和信息检索（IR）文献中的LSA所采用的大多数处理方式是交换矩阵的轴，使其具有n_features×n_samples的形状。这里以与Scikit-learn
 API相匹配的不同方式提供LSA，但是找到的奇异值是相同的。

TSVD和SVD类似，不同的是，前者是直接计算样本矩阵而非协方差矩阵。当从特征值中减去x的列（每特征）均值时，得到的矩阵上截断SVD相当于PCA。实际上，这意味着TSVD转换方法接受了scipy.sparse
矩阵，而不需要对它们进行致密，因为即使对于中型文档集合，致密化也可能填满内存。

当TSVD转换方法对任何（稀疏）特征矩阵计算时，建议在LSA /文档处理设置中使用tf-idf矩阵进行原始频率计数。特别地，应该启用子线性缩放和逆文档频率（sublinear_t_t
 =True，use_idf = True）使特征值更接近高斯分布，补偿LSA对文本数据的错误假设。

案例：

1）应用k-means聚类文本文档

http://scikit-learn.org/stable/auto_examples/text/document_clustering.html#sphx-glr-auto-examples-text-document-clustering-py

参考：

1）矩阵分解和潜在语义索引

https://nlp.stanford.edu/IR-book/pdf/18lsi.pdf

## 3 字典学习

### 3.1 预计算字典的稀疏编码

SparseCoder对象是一种估计器，可用于将信息转换为来自固定的预计算字典（例如离散小波基）的原子的稀疏线性组合。因此，该对象不实现fit方法。其转换相当于一个稀疏的编码问题：用尽可能少的字典原子的线性组合表示数据。字典学习的所有变体实现以下变换方法，可以通过transform_method初始化参数控制：

1）正交匹配追踪OMP算法；

2）最小角回归算法；

3）最小角回归计算Lasso算法；

4）坐标下降法求解Lasso算法；

5）阈值算法；

阈值算法非常快，但不能精准重构，在分类任务的文献中已被证明是有用的。对于图像重构任务来说，正交匹配追踪算法最准确、无偏见的。

字典学习对象通过split_code参数提供在稀疏编码的结果中分离正值和负值的可能性。当使用字典学习来提取用于监督学习的特征时，这是有用的，因为它允许学习算法将不同的权重分配给特定样本的负类和正类。

单个样本的分割代码的长度为2 * n_components，并使用以下规则构建：首先，计算长度为n_components的正则代码；然后split_code的第一个n_components条目用正则代码向量的正部分填充，分割代码的后半部分用正则代码向量的负部分填充，只有一个正号；因此，split_code是非负数。

案例：

1）使用预计算的字典进行稀疏编码

http://scikit-learn.org/stable/auto_examples/decomposition/plot_sparse_coding.html#sphx-glr-auto-examples-decomposition-plot-sparse-coding-py

### 3.2通用字典学习

词典学习（Dictionary Learning）是一个矩阵因式分解问题，相当于找到一个（通常是过完备的）在已fit方法处理的数据的稀疏编码中表现良好的字典。

将数据表示为来自过完备字典的稀疏组合的样本被认为是哺乳动物初级视觉皮层的工作方式。因此，应用于图像补丁的字典学习已被证明在诸如图像完成、修复和去噪以及监督识别任务的图像处理任务中给出良好的结果。
字典学习是通过交替更新稀疏代码解决的优化问题，作为解决多个Lasso问题的一个解决方案，考虑到字典固定，然后更新字典以最适合稀疏代码。

![](https://img-blog.csdn.net/20170711143748614?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZmpzc2hhcnBzd29yZA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


![](https://img-blog.csdn.net/20170711143838576?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZmpzc2hhcnBzd29yZA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


![](https://img-blog.csdn.net/20170711143847058?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZmpzc2hhcnBzd29yZA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




在使用这样一个过程来fit字典之后，transform只是一个稀疏编码步骤，与所有字典学习对象共享相同的实现（参见使用预计算字典的稀疏编码）。

以下图像显示了从浣熊脸部图像中提取4x4像素图像补丁是如何学习的。
![](https://img-blog.csdn.net/20170711143918461?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZmpzc2hhcnBzd29yZA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



案例：

1）应用字典学习进行图像去噪

http://scikit-learn.org/stable/auto_examples/decomposition/plot_image_denoising.html#sphx-glr-auto-examples-decomposition-plot-image-denoising-py

参考：

1）在线词典学习稀疏编码

http://www.di.ens.fr/sierra/pdfs/icml09.pdf

### 3.3 微量字典学习

[MiniBatchDictionaryLearning](http://scikit-learn.org/stable/modules/generated/sklearn.decomposition.MiniBatchDictionaryLearning.html#sklearn.decomposition.MiniBatchDictionaryLearning)类实现了更适合大型数据集的字典学习，算法更快准确性低。

默认情况下，[MiniBatchDictionaryLearning](http://scikit-learn.org/stable/modules/generated/sklearn.decomposition.MiniBatchDictionaryLearning.html#sklearn.decomposition.MiniBatchDictionaryLearning)将数据分成小批量，并通过在指定次数的迭代中循环使用小批量，以在线方式进行优化。但是，目前它没有退出迭代的停止条件。

训练器还实现了partial_fit方法，在微批处理中仅迭代一次来更新字典。当数据开始未准备好可用，或数据未进入内存时，该方法适用于在线学习。
![](https://img-blog.csdn.net/20170711143950929?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZmpzc2hhcnBzd29yZA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



字典学习聚类

注意，当使用字典学习来提取特征（例如用于稀疏编码）时，聚类可以是学习字典的良好代理。例如，MiniBatchKMeans训练器在计算上是有效的，并使用partial_fit方法实现在线学习。

案例：在线学习人脸部分字典

## 4 因子分析
![](https://img-blog.csdn.net/20170711144116756?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZmpzc2hhcnBzd29yZA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

![](https://img-blog.csdn.net/20170711144124665?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZmpzc2hhcnBzd29yZA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




这两个模型都基本上估计出具有低秩协方差矩阵的高斯。因为这两个模型都是概率性的，所以它们可以集成到更复杂的模型中，例如：因子分析集成。如果假设隐变量上的非高斯先验，则得到非常不同的模型（例如Fast ICA）。
因子分析可以产生与PCA相似的特征（载荷矩阵的列）。不过，不能对这些特征做出任何一般性的说明（例如他们是否正交）。

![](https://img-blog.csdn.net/20170711144203912?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZmpzc2hhcnBzd29yZA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


![](https://img-blog.csdn.net/20170711144216688?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZmpzc2hhcnBzd29yZA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




因子分析相比于PCA的主要优点是可以独立地对输入空间的每个方向（异方差噪声）建模方差：
![](https://img-blog.csdn.net/20170711144300887?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZmpzc2hhcnBzd29yZA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



在异方差噪声存在的情况下，比概率PCA可做更好的模型选择：
![](https://img-blog.csdn.net/20170711144314591?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZmpzc2hhcnBzd29yZA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



案例：

1）应用概率PCA和因子分析（FA）进行模型选择

http://scikit-learn.org/stable/auto_examples/decomposition/plot_pca_vs_fa_model_selection.html#sphx-glr-auto-examples-decomposition-plot-pca-vs-fa-model-selection-py

## 5 独立成分分析ICA

ICA将多变量信息分解为最大独立的加性子成分。Scikit-Learn库中实现了Fast ICA算法。通常，ICA不用于降低维度，而是用于分重复信息。由于ICA模型不包括噪声项，因此，为了使模型正确，必须清洗。这可以在内部使用whiten参数或手动使用其中一种PCA变体进行。
ICA通常用于分离混合信号（称为盲源分离的问题），如下例所示：

![](https://img-blog.csdn.net/20170711144400623?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZmpzc2hhcnBzd29yZA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)







ICA也可以作为一种非线性降维方法，可以找到具有一些稀疏性的特征：
![](https://img-blog.csdn.net/20170711144414741?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZmpzc2hhcnBzd29yZA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




![](https://img-blog.csdn.net/20170711144447404?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZmpzc2hhcnBzd29yZA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




案例：

1）应用Fast ICA进行盲源分离

http://scikit-learn.org/stable/auto_examples/decomposition/plot_ica_blind_source_separation.html#sphx-glr-auto-examples-decomposition-plot-ica-blind-source-separation-py

2）2D点图上的Fast ICA

http://scikit-learn.org/stable/auto_examples/decomposition/plot_ica_vs_pca.html#sphx-glr-auto-examples-decomposition-plot-ica-vs-pca-py

3）人脸数据集降维

http://scikit-learn.org/stable/auto_examples/decomposition/plot_faces_decomposition.html#sphx-glr-auto-examples-decomposition-plot-faces-decomposition-py

## 6 非负矩阵分解（NMF或NNMF）

NMF是一种替代的分解方法，假设数据和组件是非负数的。在数据矩阵不包含负值的情况下，应用NMF而不是PCA或其变体。NMF通过对平方Frobenius范数进行优化，将样本X分解成两个非负元素的矩阵W和H：
![](https://img-blog.csdn.net/20170711144524947?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZmpzc2hhcnBzd29yZA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



这个规范是欧几里得规范到矩阵的一个明显的扩展。（NMF文献中已经提出了其他优化目标，尤其是Kullback-Leibler分歧，但目前尚未实施。）

与PCA不同，通过叠加成分而不减去，以加法方式获得向量的表示。这种添加模型对于表示图像和文本是有效的。

在[Hoyer，04]中已经观察到，当加以约束时，NMF可以产生基于分量表示的数据集，从而可训练模型。
与PCA特征脸相比，以下示例显示了NMF从Olivetti人脸数据集中的图像中发现的16个稀疏组件：

![](https://img-blog.csdn.net/20170711144604625?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZmpzc2hhcnBzd29yZA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


![](https://img-blog.csdn.net/20170711144616282?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZmpzc2hhcnBzd29yZA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




init属性确定应用的初始化方法，这对方法的性能有很大的影响。NMF实现了非负双奇异值分解方法。NNDSVD基于两个SVD过程，一个近似数据矩阵，另一个是使用单位秩矩阵的代数性质，得到的部分SVD因子的近似正部分矩阵。基本的NNDSVD算法更适合稀疏分解。其变体NNDSVDa（其中所有零被设置为等于数据的所有元素的平均值）和NNDSVDar（其中将零设置为小于数据平均值的随机扰动除以100）在稠密案例中适用。

也可以通过设置init =“random”正确缩放随机非负矩阵来初始化NMF。整数种子或RandomState也可以传递给random_state以控制重复性。
在NMF中，L1和L2先验可以被添加到损失函数中以使模型正则化。L2先验使用Frobenius规范，而L1先验使用elementwiseL1范数。与ElasticNet一样，我们使用l1_ratio（）参数和alpha（）参数的正则化强度来控制L1和L2的组合。先验项：

![](https://img-blog.csdn.net/20170711144703441?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZmpzc2hhcnBzd29yZA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


和正则化目标函数：


![](https://img-blog.csdn.net/20170711144721004?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZmpzc2hhcnBzd29yZA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




NMF同时对W和H正则。公共函数non_negative_factorization允许通过声明属性进行更精细的控制，并且可以仅将W，仅H或两者正规化。

案例：

1）人脸数据集降维

http://scikit-learn.org/stable/auto_examples/decomposition/plot_faces_decomposition.html#sphx-glr-auto-examples-decomposition-plot-faces-decomposition-py

2）主题提取与非负矩阵分解和潜在Dirichlet分配

http://scikit-learn.org/stable/auto_examples/applications/topics_extraction_with_nmf_lda.html#sphx-glr-auto-examples-applications-topics-extraction-with-nmf-lda-py

参考：

1）通过非负矩阵分解学习对象的部分

链接失效；

2）非负矩阵因式分解与稀疏约束

http://www.jmlr.org/papers/volume5/hoyer04a/hoyer04a.pdf

3）非负矩阵分解的投影梯度法

http://www.csie.ntu.edu.tw/~cjlin/nmf/

4）基于SVD的初始化：非负矩阵分解的开始

http://scgroup.hpclab.ceid.upatras.gr/faculty/stratis/Papers/HPCLAB020107.pdf

5）用于大规模非负矩阵和张量因子分解的快速局部算法

链接失效。

## 7潜在狄利克雷分配（LDA）

潜在Dirichlet分配是离散数据集（如文本语料库）的集合的生成概率模型。它也是一个主题模型，用于从文档集合中发现抽象主题。
LDA图示模型是一个三级贝叶斯模型：

![](https://img-blog.csdn.net/20170711144840004?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZmpzc2hhcnBzd29yZA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


![](https://img-blog.csdn.net/20170711144852194?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZmpzc2hhcnBzd29yZA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




LDA实现在线变分贝叶斯算法，并支持在线和批量更新方法。批处理方法在每次完全传递数据后更新变分变量，在线方法从微型批量数据点更新变分变量。

注意：虽然在线方法保证收敛到局部最优点，但最优点的质量和收敛速度可能取决于小批量大小和学习率设置相关的属性。

当LDA应用于“文档主题”矩阵时，矩阵将被分解为“主题项”矩阵和“文档主题”矩阵。虽然“主题”矩阵作为**components_**存储在模型中，但是可以通过transform方法计算“文档主题”矩阵。

LDA也实现了partial_fit方法，当数据可以顺序提取时使用。

案例：

1）主题提取与非负矩阵分解和潜在Dirichlet分配

http://scikit-learn.org/stable/auto_examples/applications/topics_extraction_with_nmf_lda.html#sphx-glr-auto-examples-applications-topics-extraction-with-nmf-lda-py

参考：

1）LDA

链接失效。

2）LDA在线学习

链接失效。

3）随机变量推论

http://www.columbia.edu/~jwp2128/Papers/HoffmanBleiWangPaisley2013.pdf



说明：

翻译自：[http://scikit-learn.org/stable/modules/decomposition.html#decompositions](http://scikit-learn.org/stable/modules/decomposition.html#decompositions)

经过两天的努力成稿，里面有很多属于还是没有完全翻译透彻，这和基础理论还没掌握有关系，这部分有些是直译，有些就参考机器学习专栏[http://blog.csdn.net/column/details/16315.html](http://blog.csdn.net/column/details/16315.html)来翻译。

要加强算法基础数学理论的掌握，多阅读相关英文文献，才能提高翻译水平。







