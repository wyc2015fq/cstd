# LCA（Lobe Component Analysis）：叶成分分析 - sinat_31425585的博客 - CSDN博客
2018年09月10日 11:17:52[Mirror_Yu_Chen](https://me.csdn.net/sinat_31425585)阅读数：259
   前面提到的CCIPCA可以算作翁巨扬教授提出的第一个心智发育框架IHDR的核心算法，而LCA则是他现阶段提出的where-what-network框架的核心算法，也即是他提到的发育网络的“基因”组成。
    CCIPCA对应于PCA（主成分分析）算法，是PCA算法的增强版，使得计算算法在计算判别空间的特征向量时，covariance-free，且学习过程为在线增量式的学习。CCILCA则对应于ICA（独立成分分析），是ICA算法的增强版。这里说明一下，主成分分析与独立成分分析的最大不同之处在于：**PCA得到的特征向量是彼此之间相互正交的，也即是正交分解，ICA得到的特征向量则不必两两间正交，实际上，特征向量也不必满足严格正交关系。**
    1、叶成分
    假设有C个神经元，LCA将样本空间![\chi](https://private.codecogs.com/gif.latex?%5Cchi)划分为C个互相不重叠的区域，也即是叶区域：
![\chi =R_{1}\bigcup R_{2}\bigcup ...\bigcup R_{C}...........................(1)](https://private.codecogs.com/gif.latex?%5Cchi%20%3DR_%7B1%7D%5Cbigcup%20R_%7B2%7D%5Cbigcup%20...%5Cbigcup%20R_%7BC%7D...........................%281%29)
   其中，叶区域之间两两不相交，每一个叶区域采用一个单位特征向量![v_{i}](https://private.codecogs.com/gif.latex?v_%7Bi%7D)表示，这些特征向量不需要彼此之间正交或线性独立，他们就形成了一个叶特征子空间![S](https://private.codecogs.com/gif.latex?S)：
![S = span\left \{ v_{1} , v_{2}, ..., v_{C}\right \}............................(2)](https://private.codecogs.com/gif.latex?S%20%3D%20span%5Cleft%20%5C%7B%20v_%7B1%7D%20%2C%20v_%7B2%7D%2C%20...%2C%20v_%7BC%7D%5Cright%20%5C%7D............................%282%29)
   如果样本X的分布为一个方差为1的高斯分布，样本将在所有方向都存在相同密度。通常，样本的分布不是高斯分布，概率密度集中在某一个方向，每一个主要的聚集方向，称之为一个lobe，如图1所示。
![](https://img-blog.csdn.net/20180908205008310?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMxNDI1NTg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
                             图1 叶成分（摘自文献Dually Optimal Neuronal Layers: Lobe Component Analysis）
    2、叶成分对应最优的空间表示
    首先，假定所有的C个叶区域![\left \{ R_{1},R_{2}, ... ,R_{C} \right \}](https://private.codecogs.com/gif.latex?%5Cleft%20%5C%7B%20R_%7B1%7D%2CR_%7B2%7D%2C%20...%20%2CR_%7BC%7D%20%5Cright%20%5C%7D)都已经给定，来推导最优表征；然后，我们对叶区域进行更新，对叶区域进行调整。
    假设一个样本输入X属于叶区域![R_{i}](https://private.codecogs.com/gif.latex?R_%7Bi%7D)，我们希望使用![R_{i}](https://private.codecogs.com/gif.latex?R_%7Bi%7D)对应的特征向量![v_{i}](https://private.codecogs.com/gif.latex?v_%7Bi%7D)来表示，即![X\approx\alpha v_{i}](https://private.codecogs.com/gif.latex?X%5Capprox%5Calpha%20v_%7Bi%7D)，使得近似误差![E\left \| X-\alpha v_{i} \right \|^{2}](https://private.codecogs.com/gif.latex?E%5Cleft%20%5C%7C%20X-%5Calpha%20v_%7Bi%7D%20%5Cright%20%5C%7C%5E%7B2%7D)最小，这个时候，最优解为：![\alpha =X\cdot v_{i}](https://private.codecogs.com/gif.latex?%5Calpha%20%3DX%5Ccdot%20v_%7Bi%7D) ，这个时候![X\approx \left ( X\cdot v_{i} \right )v_{i}](https://private.codecogs.com/gif.latex?X%5Capprox%20%5Cleft%20%28%20X%5Ccdot%20v_%7Bi%7D%20%5Cright%20%29v_%7Bi%7D) 。
    那么，对于C个叶区域而言，全局的最小近似误差为：
![R_{j}=\left \{ X| j = argmin(X-\left ( X\cdot v_{i} \right )v_{i}) \right \},i=1,2,...,C..........................(3)](https://private.codecogs.com/gif.latex?R_%7Bj%7D%3D%5Cleft%20%5C%7B%20X%7C%20j%20%3D%20argmin%28X-%5Cleft%20%28%20X%5Ccdot%20v_%7Bi%7D%20%5Cright%20%29v_%7Bi%7D%29%20%5Cright%20%5C%7D%2Ci%3D1%2C2%2C...%2CC..........................%283%29)
   反过来，如果我们已知样本输入X属于叶区域![R_{i}](https://private.codecogs.com/gif.latex?R_%7Bi%7D)，那么采用怎样的特征向量![v_{i}](https://private.codecogs.com/gif.latex?v_%7Bi%7D)来最优地表征该叶区域，表征X的平方近似误差为：
![\left \|X- \left ( X\cdot v_{i} \right )v_{i} \right \|^{2}=\left (X- \left ( X\cdot v_{i} \right )v_{i} \right )^{T}\left (X- \left ( X\cdot v_{i} \right )v_{i} \right )](https://private.codecogs.com/gif.latex?%5Cleft%20%5C%7CX-%20%5Cleft%20%28%20X%5Ccdot%20v_%7Bi%7D%20%5Cright%20%29v_%7Bi%7D%20%5Cright%20%5C%7C%5E%7B2%7D%3D%5Cleft%20%28X-%20%5Cleft%20%28%20X%5Ccdot%20v_%7Bi%7D%20%5Cright%20%29v_%7Bi%7D%20%5Cright%20%29%5E%7BT%7D%5Cleft%20%28X-%20%5Cleft%20%28%20X%5Ccdot%20v_%7Bi%7D%20%5Cright%20%29v_%7Bi%7D%20%5Cright%20%29)
![=X^{T}X-v_{i}^{T}Xv_{i}^{T}X-X^{T}(X\cdot v_{i})v_{i} + (X\cdot v_{i})v_{i}^{T} (X\cdot v_{i})v_{i}](https://private.codecogs.com/gif.latex?%3DX%5E%7BT%7DX-v_%7Bi%7D%5E%7BT%7DXv_%7Bi%7D%5E%7BT%7DX-X%5E%7BT%7D%28X%5Ccdot%20v_%7Bi%7D%29v_%7Bi%7D%20&plus;%20%28X%5Ccdot%20v_%7Bi%7D%29v_%7Bi%7D%5E%7BT%7D%20%28X%5Ccdot%20v_%7Bi%7D%29v_%7Bi%7D)
![= X^{T}X-X^{T} v_{i}v_{i}^{T}X-X^{T}(X^{T} v_{i})v_{i} + (X^{T} v_{i})v_{i}^{T} (X^{T}v_{i})v_{i}](https://private.codecogs.com/gif.latex?%3D%20X%5E%7BT%7DX-X%5E%7BT%7D%20v_%7Bi%7Dv_%7Bi%7D%5E%7BT%7DX-X%5E%7BT%7D%28X%5E%7BT%7D%20v_%7Bi%7D%29v_%7Bi%7D%20&plus;%20%28X%5E%7BT%7D%20v_%7Bi%7D%29v_%7Bi%7D%5E%7BT%7D%20%28X%5E%7BT%7Dv_%7Bi%7D%29v_%7Bi%7D)
![= X^{T}X-X^{T} v_{i}v_{i}^{T}X-X^{T}X^{T} v_{i}v_{i} + X^{T} v_{i}v_{i}^{T} X^{T}v_{i}v_{i}](https://private.codecogs.com/gif.latex?%3D%20X%5E%7BT%7DX-X%5E%7BT%7D%20v_%7Bi%7Dv_%7Bi%7D%5E%7BT%7DX-X%5E%7BT%7DX%5E%7BT%7D%20v_%7Bi%7Dv_%7Bi%7D%20&plus;%20X%5E%7BT%7D%20v_%7Bi%7Dv_%7Bi%7D%5E%7BT%7D%20X%5E%7BT%7Dv_%7Bi%7Dv_%7Bi%7D)
![= X^{T}X-X^{T} v_{i}v_{i}^{T}X-X^{T}X^{T} v_{i}v_{i} + X^{T} X^{T}v_{i}v_{i}](https://private.codecogs.com/gif.latex?%3D%20X%5E%7BT%7DX-X%5E%7BT%7D%20v_%7Bi%7Dv_%7Bi%7D%5E%7BT%7DX-X%5E%7BT%7DX%5E%7BT%7D%20v_%7Bi%7Dv_%7Bi%7D%20&plus;%20X%5E%7BT%7D%20X%5E%7BT%7Dv_%7Bi%7Dv_%7Bi%7D)
![= X^{T}X-(v_{i}^{T}X)^{T}(v_{i}^{T}X) =X^{T}X-(X^{T}v_{i})^{T}(X^{T}v_{i})](https://private.codecogs.com/gif.latex?%3D%20X%5E%7BT%7DX-%28v_%7Bi%7D%5E%7BT%7DX%29%5E%7BT%7D%28v_%7Bi%7D%5E%7BT%7DX%29%20%3DX%5E%7BT%7DX-%28X%5E%7BT%7Dv_%7Bi%7D%29%5E%7BT%7D%28X%5E%7BT%7Dv_%7Bi%7D%29)
    最后一步能交换的原因为![X^{T}v_{i}=v_{i}^{T}X](https://private.codecogs.com/gif.latex?X%5E%7BT%7Dv_%7Bi%7D%3Dv_%7Bi%7D%5E%7BT%7DX)表示一个标量，那么，对于叶区域![R_{i}](https://private.codecogs.com/gif.latex?R_%7Bi%7D)近似误差的期望值为：
![E\left \|X- \left ( X\cdot v_{i} \right )v_{i} \right \|^{2} =E[X^{T}X]-v_{i}^{T}E[X^{T}X]v_{i}=trace(\sum_{x}) - v_{i}^{T}\sum_{x}v_{i}........(4)](https://private.codecogs.com/gif.latex?E%5Cleft%20%5C%7CX-%20%5Cleft%20%28%20X%5Ccdot%20v_%7Bi%7D%20%5Cright%20%29v_%7Bi%7D%20%5Cright%20%5C%7C%5E%7B2%7D%20%3DE%5BX%5E%7BT%7DX%5D-v_%7Bi%7D%5E%7BT%7DE%5BX%5E%7BT%7DX%5Dv_%7Bi%7D%3Dtrace%28%5Csum_%7Bx%7D%29%20-%20v_%7Bi%7D%5E%7BT%7D%5Csum_%7Bx%7Dv_%7Bi%7D........%284%29)
    注意，第一个分量为常量，如果要最小化近似误差的期望值，需要最大化第二个部分，从主成分分析的角度来看，即![v_{i}](https://private.codecogs.com/gif.latex?v_%7Bi%7D)取值为协方差矩阵最大特征值对应特征向量，有：
![\lambda v_{i}=\sum_{x}v_{i}.....................(5)](https://private.codecogs.com/gif.latex?%5Clambda%20v_%7Bi%7D%3D%5Csum_%7Bx%7Dv_%7Bi%7D.....................%285%29)
    两边乘以![v_{i}^{T}](https://private.codecogs.com/gif.latex?v_%7Bi%7D%5E%7BT%7D)，有：
![\lambda= v_{i}^{T} \sum_{x}v_{i}=E(X\cdot v_{i})^{2}..........(6)](https://private.codecogs.com/gif.latex?%5Clambda%3D%20v_%7Bi%7D%5E%7BT%7D%20%5Csum_%7Bx%7Dv_%7Bi%7D%3DE%28X%5Ccdot%20v_%7Bi%7D%29%5E%7B2%7D..........%286%29)
    从上面分析可得，当样本X表示的方向与所处叶区域![R_{i}](https://private.codecogs.com/gif.latex?R_%7Bi%7D)对应方向越相近时，对应全局的近似误差越小。空间最优对应于表征误差最小。更直观一些的表述就是，如果我先用前C个样本初始化C个叶区域对应特征向量，第C+1个样本到来时，我就在前C个特征向量中选取与当前样本方向最一致的特征向量来表征该样本，这样就可以使得当前步骤对应全局最小近似误差，然后，如果我们能保障每一步都是全局最小近似误差，就可以达到整体的最小表征误差。
    3、时间最优：LCA对应最优的学习步长
    为了处理长期记忆和快速自适应规则，我们需要对问题的解进行增量式地优化，受生物突触学习的启发，假设对于LCA而言，神经内部的观测量(neuronal internal observation (NIO))定义为响应权重的输入：
![u(t) =\frac{x(t)\cdot v(t-1)}{\left \| v(t-1) \right \|}x(t).............................(7)](https://private.codecogs.com/gif.latex?u%28t%29%20%3D%5Cfrac%7Bx%28t%29%5Ccdot%20v%28t-1%29%7D%7B%5Cleft%20%5C%7C%20v%28t-1%29%20%5Cright%20%5C%7C%7Dx%28t%29.............................%287%29)
    突触权重向量![v(t)](https://private.codecogs.com/gif.latex?v%28t%29)是对一系列的观测量![U(t)=\left \{ u_{1} ,u_{2},...,u_{n}\right \}](https://private.codecogs.com/gif.latex?U%28t%29%3D%5Cleft%20%5C%7B%20u_%7B1%7D%20%2Cu_%7B2%7D%2C...%2Cu_%7Bn%7D%5Cright%20%5C%7D)评估得到的。假设在第t个时刻，NIO的学习率为![\eta _{t}](https://private.codecogs.com/gif.latex?%5Ceta%20_%7Bt%7D)，那么我们如何来自动的设定学习率![\left \{ \eta _{1},\eta _{2},...,\eta _{n} \right \}](https://private.codecogs.com/gif.latex?%5Cleft%20%5C%7B%20%5Ceta%20_%7B1%7D%2C%5Ceta%20_%7B2%7D%2C...%2C%5Ceta%20_%7Bn%7D%20%5Cright%20%5C%7D)，使得在每一个时刻对于神经权重向量![\hat{v}(t)](https://private.codecogs.com/gif.latex?%5Chat%7Bv%7D%28t%29)的评估值误差最小，这意味着，在每一个时刻t更新，使得：
![minimum-error(t)=\left \| \hat{v}(U(t))-v^{*} \right \|^{2}..........................(8)](https://private.codecogs.com/gif.latex?minimum-error%28t%29%3D%5Cleft%20%5C%7C%20%5Chat%7Bv%7D%28U%28t%29%29-v%5E%7B*%7D%20%5Cright%20%5C%7C%5E%7B2%7D..........................%288%29)
    由公式(5)有：
![\lambda v_{i}=\frac{1}{n}\sum_{t=1}^{n}x(t)x(t)^{T}v_{i}=\frac{1}{n}\sum_{t=1}^{n}(x(t)\cdot v_{i})x(t)...............................(9)](https://private.codecogs.com/gif.latex?%5Clambda%20v_%7Bi%7D%3D%5Cfrac%7B1%7D%7Bn%7D%5Csum_%7Bt%3D1%7D%5E%7Bn%7Dx%28t%29x%28t%29%5E%7BT%7Dv_%7Bi%7D%3D%5Cfrac%7B1%7D%7Bn%7D%5Csum_%7Bt%3D1%7D%5E%7Bn%7D%28x%28t%29%5Ccdot%20v_%7Bi%7D%29x%28t%29...............................%289%29)
    如果假定![v_{i}](https://private.codecogs.com/gif.latex?v_%7Bi%7D)的模长为![\lambda](https://private.codecogs.com/gif.latex?%5Clambda)，两边同时除以![v_{i}](https://private.codecogs.com/gif.latex?v_%7Bi%7D)的模长有：
![v_{i}=\frac{1}{n}\sum_{t=1}^{n}x(t)x(t)^{T}v_{i}=\frac{1}{n}\sum_{t=1}^{n}(\frac{x(t)\cdot v_{i}}{||v_{i}||})x(t)..........................(10)](https://private.codecogs.com/gif.latex?v_%7Bi%7D%3D%5Cfrac%7B1%7D%7Bn%7D%5Csum_%7Bt%3D1%7D%5E%7Bn%7Dx%28t%29x%28t%29%5E%7BT%7Dv_%7Bi%7D%3D%5Cfrac%7B1%7D%7Bn%7D%5Csum_%7Bt%3D1%7D%5E%7Bn%7D%28%5Cfrac%7Bx%28t%29%5Ccdot%20v_%7Bi%7D%7D%7B%7C%7Cv_%7Bi%7D%7C%7C%7D%29x%28t%29..........................%2810%29)
    从统计学的有效性角度来讲，对于同一个观测量集合而言，评估误差越小，评估值越有效。一般情况下，对于很多分布（高斯分布，指数分布）而言，最大似然估计的均值为样本的均值，而当评估值等于均值时，对应MLE最大，因此![v_{i}](https://private.codecogs.com/gif.latex?v_%7Bi%7D)取评估值得均值，可以达到统计学意义上的有效。
    式(10)对应于增量式评估的过程，但是这是一个batch处理版本，其对应的增量式版本为：
![v(t)=\frac{t-1}{t}v(t-1)+\frac{1}{t}u(t).......................................(11)](https://private.codecogs.com/gif.latex?v%28t%29%3D%5Cfrac%7Bt-1%7D%7Bt%7Dv%28t-1%29&plus;%5Cfrac%7B1%7D%7Bt%7Du%28t%29.......................................%2811%29)
    式(11)表明，为了得到当前时刻评估最优，我们要同时设置学习率1/t，及残差率(t-1)/t，学习率 1/t保障了最优的"step size"，这也即是LCA的第二个最优——步长大小最优。
    4、LCA算法流程
   1、序列初始化：使用前C个输入样本数据，初始化C个cells，并初始化cell的更新年龄为1
![v_{i}(0)=x_{i},i=1,2,...,C](https://private.codecogs.com/gif.latex?v_%7Bi%7D%280%29%3Dx_%7Bi%7D%2Ci%3D1%2C2%2C...%2CC)
![n_{i}=1,i=1,2,...,C](https://private.codecogs.com/gif.latex?n_%7Bi%7D%3D1%2Ci%3D1%2C2%2C...%2CC)
   2、学习和更新：![j=C+1,C+2,...](https://private.codecogs.com/gif.latex?j%3DC&plus;1%2CC&plus;2%2C...)
   1) 计算神经元响应：
![y_{i}=\frac{x(t)v_{i}(t-1)}{||v_{i}(t-1))||},i=1,2,...,C](https://private.codecogs.com/gif.latex?y_%7Bi%7D%3D%5Cfrac%7Bx%28t%29v_%7Bi%7D%28t-1%29%7D%7B%7C%7Cv_%7Bi%7D%28t-1%29%29%7C%7C%7D%2Ci%3D1%2C2%2C...%2CC)
  其中，![x(t)](https://private.codecogs.com/gif.latex?x%28t%29)表示第t个时刻对应样本，对应于第j个样本。
   2) 不同特征之间侧抑制和稀疏编码：
    按照神经元响应的大小顺序，选取前top-k个神经元进行激活，采用这种非迭代的排序机制替换掉了重复的迭代过程，进而保证了计算的有效性。然后使用线性函数对响应进行缩放，保证响应在0，1之间：
![{y}'_{i}=(y_{i}-y_{k+1})/(y_{1}-y_{k+1})](https://private.codecogs.com/gif.latex?%7By%7D%27_%7Bi%7D%3D%28y_%7Bi%7D-y_%7Bk&plus;1%7D%29/%28y_%7B1%7D-y_%7Bk&plus;1%7D%29)
   3) 最优Hebbian学习：
    使用时间可塑性设计，更新竞争优胜的前top-k个神经元
![v_{j}(t)=\omega _{1}v(t-1)+\omega _{2}y_{j}x(t)](https://private.codecogs.com/gif.latex?v_%7Bj%7D%28t%29%3D%5Comega%20_%7B1%7Dv%28t-1%29&plus;%5Comega%20_%7B2%7Dy_%7Bj%7Dx%28t%29)
![\omega _{1}](https://private.codecogs.com/gif.latex?%5Comega%20_%7B1%7D)对应残差率，![\omega _{2}](https://private.codecogs.com/gif.latex?%5Comega%20_%7B2%7D)对应于学习率：
![\omega _{1}=\frac{n_{j}-1-\mu (n_{j})}{n_{j}},\omega _{2}=\frac{1+\mu (n_{j})}{n_{j}}](https://private.codecogs.com/gif.latex?%5Comega%20_%7B1%7D%3D%5Cfrac%7Bn_%7Bj%7D-1-%5Cmu%20%28n_%7Bj%7D%29%7D%7Bn_%7Bj%7D%7D%2C%5Comega%20_%7B2%7D%3D%5Cfrac%7B1&plus;%5Cmu%20%28n_%7Bj%7D%29%7D%7Bn_%7Bj%7D%7D)
![\mu (n_{j})](https://private.codecogs.com/gif.latex?%5Cmu%20%28n_%7Bj%7D%29)对应于遗忘函数。
  4) 侧兴奋：这个对应于MILN中 *[Topographic](http://www.cse.msu.edu/~luciwmat/LuciwIJCNN08.pdf)这个概念*
  5) 长短期记忆：只更新优胜的top-k个神经元，其余的神经元即对应于长期记忆，更新的top-k个神经元对应于短期记忆。
   这里可以看到，CCIPCA与CCILCA最大的不同之处在于，**CCIPCA目的在于提取样本集数据对应最判别的前K个特征，实现对数据的降维，CCILCA目的在于寻找样本集中中C个样本分布密度最大的方向（相当于方向上的聚类），对应C个叶区域，进而将样本与样本进行区分。**同时在更新步骤中，当输入一个样本时，CCIPCA使用这个样本数据对所有的判别特征进行更新，CCILCA则只对与当前样本最匹配的前top-k个特征向量进行更新。
    最后看一下效果，如果采用增量评估的方式计算均值，对包含15个人的yale库提取15个叶成分，效果如图2所示。
![](https://img-blog.csdn.net/2018092400504483?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMxNDI1NTg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
                                                                            图2 采用增量评估的方式计算均值
     另外一种是计算均值时，直接采用先计算全局的均值，而不是采用增量评估的方式，效果如图3所示。
![](https://img-blog.csdn.net/20180924005236885?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMxNDI1NTg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
                                                                      图3 采用batch的方式预先计算均值
     这15个人示例如图4所示：
![](https://img-blog.csdn.net/20180924003030713?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMxNDI1NTg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
                                                                               图4 yale库示意图 
 未完待续~~~~~~~
参考文献：
[1] Bell A J, Sejnowski T J. The "independent components" of natural scenes are edge filters.[J]. Vision Research, 1997, 37(23):3327-38.
[2] Weng J, Luciw M. Dually Optimal Neuronal Layers: Lobe Component Analysis[J]. IEEE Transactions on Autonomous Mental Development, 2009, 1(1):68-85.
