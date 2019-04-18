# 理解XGBoost - 知乎
# 

SIGAI原创技术文章_*理解XGBoost*_PDF下载链接：[理解XGBoost](https://link.zhihu.com/?target=http%3A//www.tensorinfinity.com/paper_80.html)

XGBoost是当前炙手可热的算法，适合抽象数据的分析问题，在Kaggle等比赛中率获佳绩。市面上虽然有大量介绍XGBoost原理与使用的文章，但少有能清晰透彻的讲清其原理的。本文的目标是对XGBoost的原理进行系统而深入的讲解，帮助大家真正理解算法的原理。文章是对已经在清华达成出版社出版的[《机器学习与应用》](https://link.zhihu.com/?target=https%3A//item.jd.com/12504554.html)，雷明著的补充。在这本书里系统的讲解了集成学习、bagging与随机森林、boosting与各类AdaBoost算法的原理及其实现、应用。AdaBoost与梯度提升，XGBoost的推导都需要使用广义加法模型，对此也有深入的介绍。

理解XGBoost的原理需要决策树（尤其是分类与回归树），集成学习，广义加法模型，牛顿法等基础知识。其中，决策树在SIGAI之前的公众号文章“[理解决策树](https://link.zhihu.com/?target=http%3A//www.sigai.cn/paper_75.html)”中已经做了深入的讲解。集成学习在之前的公众号文章“[随机森林概述](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247485718%26idx%3D1%26sn%3Dc05c217af81173ae2c1301cbda5f7131%26chksm%3Dfdb69481cac11d975d86ff2e280371963d04b5709dfa0a9e874d637b7cf3844cad12be584094%26scene%3D21%23wechat_redirect)”， “[大话AdaBoost算法](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247484692%26idx%3D1%26sn%3D9b389aa65208c778dddf17c601afbee1%26chksm%3Dfdb69883cac1119593934734e94c3b71aa68de67bda8a946c1f9f9e1209c3b6f0bf18fed99b8%26scene%3D21%23wechat_redirect)”，“[理解AdaBoost算法](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247486478%26idx%3D1%26sn%3D8557d1ffbd2bc11027e642cc0a36f8ef%26chksm%3Dfdb69199cac1188ff006b7c4bdfcd17f15f521b759081813627be3b5d13715d7c41fccec3a3f%26scene%3D21%23wechat_redirect)”中已经做了讲解。牛顿法在之前的公众号文章“[理解梯度下降法](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247484111%26idx%3D1%26sn%3D4ed4480e849298a0aff828611e18f1a8%26chksm%3Dfdb69f58cac1164e844726bd429862eb7b38d22509eb4d1826eb851036460cb7ca5a8de7b9bb%26scene%3D21%23wechat_redirect)”，“[理解凸优化](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247484439%26idx%3D1%26sn%3D4fa8c71ae9cb777d6e97ebd0dd8672e7%26chksm%3Dfdb69980cac110960e08c63061e0719a8dc7945606eeef460404dc2eb21b4f5bdb434fb56f92%26scene%3D21%23wechat_redirect)”，“[理解牛顿法](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247484651%26idx%3D1%26sn%3Da0e4ca5edb868fe3eae9101b71dd7103%26chksm%3Dfdb6997ccac1106a61f51fe9f8fd532045cc5d13f6c75c2cbbf1a7c94c58bcdf5f2a6661facd%26scene%3D21%23wechat_redirect)”中已经进行了介绍。如果读者对这些知识还不清楚，建议先阅读这些文章。

## 分类与回归树

决策树时机器学习中古老的算法，可以看做是一组嵌套的判定规则，这组规则通过训练得到。从数学上看，决策树是一个分段常数函数，每个叶子节点对应空间中的一片区域，落入此区域的向量 ![x](https://www.zhihu.com/equation?tex=x) 被预测为该叶子节点的值。

分类与回归树是二叉决策树的一种，既可以用于分类问题，也可以用于回归问题。训练时要解决的核心问题是寻找最佳分裂来确定内部节点，递归构建树，为叶子节点赋予标签值。对于分类问题，寻找最佳分裂时的目标是最大化Gini纯度值，简化后的计算公式为

![G=\frac{\sum_iN_{L,i}^2}{N_L} + \frac{\sum_iN_{R,j}^2}{N_R}](https://www.zhihu.com/equation?tex=G%3D%5Cfrac%7B%5Csum_iN_%7BL%2Ci%7D%5E2%7D%7BN_L%7D+%2B+%5Cfrac%7B%5Csum_iN_%7BR%2Cj%7D%5E2%7D%7BN_R%7D)
其中 ![N_L](https://www.zhihu.com/equation?tex=N_L) 是分裂之后左子节点的训练样本数， ![N_{L,i}](https://www.zhihu.com/equation?tex=N_%7BL%2Ci%7D) 是左子节点中第 ![i](https://www.zhihu.com/equation?tex=i) 类样本数； ![N_R](https://www.zhihu.com/equation?tex=N_R) ​是分裂之后右子节点的训练样本数， ![N_{R,i}](https://www.zhihu.com/equation?tex=N_%7BR%2Ci%7D) 是右子节点中第 ![i](https://www.zhihu.com/equation?tex=i) 类样本数。对于回归问题，寻找最佳分裂时优化的目标是最大化回归误差的下降值

![E=\frac{1}{N_L}(\sum_{i=1}^{N_L}y_i)^2 + \frac{1}{N_R}(\sum_{i=1}^{N_R}y_i)^2](https://www.zhihu.com/equation?tex=E%3D%5Cfrac%7B1%7D%7BN_L%7D%28%5Csum_%7Bi%3D1%7D%5E%7BN_L%7Dy_i%29%5E2+%2B+%5Cfrac%7B1%7D%7BN_R%7D%28%5Csum_%7Bi%3D1%7D%5E%7BN_R%7Dy_i%29%5E2)

因为特征向量中有多个特征，因此实现时需要为每个特征分量计算最佳分裂阈值，然后取其中最优的。为单个特征计算最佳分裂阈值时，首先将该节点的所有训练样本按照该特征分量从小到大排序，然后依次以每个特征值作为阈值，将样本集分为左右两个子集，然后计算上面的分裂指标，取其最优值。具体原理在[《机器学习与应用》](https://link.zhihu.com/?target=https%3A//item.jd.com/12504554.html)一书中已经详细讲述。

第二个问题是叶子节点值的设定。对于分类问题，将叶子节点的值设置成本节点的训练样本集中出现概率最大的那个类；对于回归树，则设置为本节点训练样本标签值的均值。

## 集成学习

集成学习是机器学习中的一大类算法，它代表了一种朴素的思想：将多个机器学习模型组合起来使用，得到一个更强的模型。被组合的模型称为弱学习器，组合之后的模型称为强学习器。根据组合的策略不同，诞生了各种不同的算法。其中最常用的是bagging与boosting。前者的代表作是随机森林，后者的代表作是AdaBoost，梯度提升，XGBoost。

## 广义加法模型

在弱学习器的组合方案中，如果使用加法，即将多个弱学习器的预测函数相加得到强学习器，则称为广义加法模型。广义加法模型拟合的目标函数是多个基函数的线性组合

![F_{(x)} = \sum_{i=1}^{M}\beta_if(x;\gamma_i)](https://www.zhihu.com/equation?tex=F_%7B%28x%29%7D+%3D+%5Csum_%7Bi%3D1%7D%5E%7BM%7D%5Cbeta_if%28x%3B%5Cgamma_i%29)

其中 ![\gamma_i](https://www.zhihu.com/equation?tex=%5Cgamma_i) ​为基函数的参数， ![\beta_i](https://www.zhihu.com/equation?tex=%5Cbeta_i) ​为基函数的权重系数。训练时要确定的是基函数的参数和权重值，如果用决策树充当基函数，则参数为比较特征、比较阈值，叶子节点值等。训练时的目标是最小化对所有样本的损失函数

![\min_{\beta_j,\gamma_j}\sum_{i=1}^lL(y_i,\sum_{j=1}^M\beta_jf(x_i;\gamma_j))](https://www.zhihu.com/equation?tex=%5Cmin_%7B%5Cbeta_j%2C%5Cgamma_j%7D%5Csum_%7Bi%3D1%7D%5ElL%28y_i%2C%5Csum_%7Bj%3D1%7D%5EM%5Cbeta_jf%28x_i%3B%5Cgamma_j%29%29)
训练算法采用了逐步求精的策略，依次确定每个基函数的参数和权重，然后将其加入强学习器中，使得强学习器的精度提升。实际实现时，将当前已经得到的强学习器对训练样本的输出值当做常数。然后采用分阶段优化策略，先固定住权重值![\beta_i](https://www.zhihu.com/equation?tex=%5Cbeta_i) ​​，优化弱学习器。然后再将弱学习器当做常数，优化权重值![\beta_i](https://www.zhihu.com/equation?tex=%5Cbeta_i) ​。

以AdaBoost算法为例，强分类器对单个训练样本的损失为指数损失函数

![L(y,F(x)) = \exp(-yF(x))](https://www.zhihu.com/equation?tex=L%28y%2CF%28x%29%29+%3D+%5Cexp%28-yF%28x%29%29)

将广义加法模型的预测函数代入上面的损失函数中，得到算法训练时要优化的目标函数为

![(\beta_j,f_j)=\arg\min_{\beta,f}\sum_{i=1}^{l}\exp(-y_i(F_{j-1}(X_i)+\beta f(x_i)))](https://www.zhihu.com/equation?tex=%28%5Cbeta_j%2Cf_j%29%3D%5Carg%5Cmin_%7B%5Cbeta%2Cf%7D%5Csum_%7Bi%3D1%7D%5E%7Bl%7D%5Cexp%28-y_i%28F_%7Bj-1%7D%28X_i%29%2B%5Cbeta+f%28x_i%29%29%29)
这里将指数函数拆成了两部分，已有的强分类器 ![F_{j-1}](https://www.zhihu.com/equation?tex=F_%7Bj-1%7D) ，以及当前弱分类器 ![f](https://www.zhihu.com/equation?tex=f) 对训练样本的损失函数，前者在之前的迭代中已经求出，因此 ![F_{j-1}(X_i)](https://www.zhihu.com/equation?tex=F_%7Bj-1%7D%28X_i%29) 可以看成常数。这样目标函数可以简化为

![\min_{\beta,f}\sum_{i=1}^lw_i^{j-1}\exp(-\beta y_if(x_i))](https://www.zhihu.com/equation?tex=%5Cmin_%7B%5Cbeta%2Cf%7D%5Csum_%7Bi%3D1%7D%5Elw_i%5E%7Bj-1%7D%5Cexp%28-%5Cbeta+y_if%28x_i%29%29)
其中

![w_i^{j-1}=\exp(-y_iF_{j-1}(x_i))](https://www.zhihu.com/equation?tex=w_i%5E%7Bj-1%7D%3D%5Cexp%28-y_iF_%7Bj-1%7D%28x_i%29%29)

它只和前面的迭代得到的强分类器有关，与当前的弱分类器、弱分类器权重无关，这就是AdaBoost算法的样本权重。这个问题可以分两步求解，首先将 ![\beta](https://www.zhihu.com/equation?tex=%5Cbeta) 看成常数，优化 ![f(x_i)](https://www.zhihu.com/equation?tex=f%28x_i%29) ；然后固定 ![f(x_i)](https://www.zhihu.com/equation?tex=f%28x_i%29) ，优化 ![\beta](https://www.zhihu.com/equation?tex=%5Cbeta) 。由此得到了AdaBoost的训练算法。

从广义加法模型可以推导出种AdaBoost算法，它们的弱分类器不同，训练时优化的目标函数也不同，分别是：

```
离散型AdaBoost
实数型AdaBoost算法
LogitBoost
Gentle型AdaBoost​
```

各种AdaBoost算法的原理以及广义加法模型在《机器学习与应用》一书中有详细的讲述，限于篇幅，在这里不过多介绍。

除AdaBoost算法之外，boosting还有其他实现算法，如梯度提升算法。梯度提升算法采用了不同的思路，同样是使用广义加法模型，但在求解时采用了最速下降法（梯度下降法的变种）。同样是依次训练每个弱学习器，但训练弱学习器时没有为训练样本加上权重，而是为其计算伪标签值，该伪标签值是损失函数对当前已经求得的强学习器对训练样本的预测值 ![F_{j-1}(X_i)](https://www.zhihu.com/equation?tex=F_%7Bj-1%7D%28X_i%29) 的导数的负值：

![{y}'_i= -\left \lfloor \frac{\partial L(y_i,F(X_i))}{\partial F(X_i)} \right \rfloor_{F(x)=F_{m-1}(x)},i=1,\cdots,N](https://www.zhihu.com/equation?tex=%7By%7D%27_i%3D+-%5Cleft+%5Clfloor+%5Cfrac%7B%5Cpartial+L%28y_i%2CF%28X_i%29%29%7D%7B%5Cpartial+F%28X_i%29%7D+%5Cright+%5Crfloor_%7BF%28x%29%3DF_%7Bm-1%7D%28x%29%7D%2Ci%3D1%2C%5Ccdots%2CN)
然后用此次迭代时要训练的若学习器来拟合该目标。具体实现时分为了训练弱学习器，寻找最优步长的直线搜索（line search）这两步。

## 牛顿法

牛顿法是求解函数极值的数值优化（近似求解）算法。根据微积分中的Fermat定理，函数在点 ![x^*](https://www.zhihu.com/equation?tex=x%5E%2A) 处取得极值的必要条件是导数（对于多元函数是梯度）为0，即：

![\triangledown f(x^*)=0](https://www.zhihu.com/equation?tex=%5Ctriangledown+f%28x%5E%2A%29%3D0)
称 ![x^*](https://www.zhihu.com/equation?tex=x%5E%2A) 为函数的驻点。因此可以通过寻找函数的驻点求解函数的极值。直接计算函数的梯度然后解上面的方程组一般来是非常困难的，如果函数是一个复杂的非线性函数，这个方程组是一个非线性方程组，不易求解。因此大多采用迭代法近似计算。它从一个初始点 ![x_0](https://www.zhihu.com/equation?tex=x_0) ​开始，反复使用某种规则从 ![x_k](https://www.zhihu.com/equation?tex=x_k) 移动到下一个点 ![x_{k+1}](https://www.zhihu.com/equation?tex=x_%7Bk%2B1%7D) ，直至到达函数的极值点。这些规则一般会利用一阶导数信息即梯度；或者二阶导数信息即Hessian矩阵。牛顿法采用了一阶导数与二阶导数信息。

对多元函数在 ![x_0](https://www.zhihu.com/equation?tex=x_0) 处作二阶泰勒展开，有：

![f(x)= f(x_0)+\triangledown f(x_0)^T(x-x0)+\frac{1}{2}(x-x_0)^T\triangledown ^2f(x_0)(x-x_0)+o((x-x_0)^2)](https://www.zhihu.com/equation?tex=f%28x%29%3D+f%28x_0%29%2B%5Ctriangledown+f%28x_0%29%5ET%28x-x0%29%2B%5Cfrac%7B1%7D%7B2%7D%28x-x_0%29%5ET%5Ctriangledown+%5E2f%28x_0%29%28x-x_0%29%2Bo%28%28x-x_0%29%5E2%29)
忽略二次及以上的项，将函数近似成二次函数，并对上式两边同时对求梯度，得到函数的梯度为：

![\triangledown f(x) = \triangledown f(x_0)+\triangledown ^2f(x_0)(x-x_0)](https://www.zhihu.com/equation?tex=%5Ctriangledown+f%28x%29+%3D+%5Ctriangledown+f%28x_0%29%2B%5Ctriangledown+%5E2f%28x_0%29%28x-x_0%29)
其中 ![\triangledown ^2f(x_0)](https://www.zhihu.com/equation?tex=%5Ctriangledown+%5E2f%28x_0%29) 即为Hessian矩阵 ![H](https://www.zhihu.com/equation?tex=H) 。令函数的梯度为0，则有：

![\triangledown f(x_0) + \triangledown ^2f(x_0)(x-x_0) = 0](https://www.zhihu.com/equation?tex=%5Ctriangledown+f%28x_0%29+%2B+%5Ctriangledown+%5E2f%28x_0%29%28x-x_0%29+%3D+0)

解这个线性方程组可以得到：

![X=X_0 -(\triangledown ^2f(x_0))^{-1}\triangledown f(x_0)](https://www.zhihu.com/equation?tex=X%3DX_0+-%28%5Ctriangledown+%5E2f%28x_0%29%29%5E%7B-1%7D%5Ctriangledown+f%28x_0%29)

由于在泰勒展开中忽略了高阶项，因此这个解并不一定是函数的驻点，需要反复用这个公式进行迭代。从初始点 ![x_0](https://www.zhihu.com/equation?tex=x_0) 处开始，反复计算函数在处的Hessian矩阵和梯度向量，然后用下面的公式进行迭代：

![X_{k+1} = X_k -H_k^{-1}g_k](https://www.zhihu.com/equation?tex=X_%7Bk%2B1%7D+%3D+X_k+-H_k%5E%7B-1%7Dg_k)
最终会到达函数的驻点处。其中 ![-H^{-1}g](https://www.zhihu.com/equation?tex=-H%5E%7B-1%7Dg) 称为牛顿方向。迭代终止的条件是梯度的模接近于0，或者函数值下降小于指定阈值。对于一元函数，Hessian矩阵即为二阶导数，梯度向量即为一阶导数，迭代公式为

![x_{k+1}=x_k-\frac{f'(x_k)}{f''x_k}](https://www.zhihu.com/equation?tex=x_%7Bk%2B1%7D%3Dx_k-%5Cfrac%7Bf%27%28x_k%29%7D%7Bf%27%27x_k%7D)
在XGBoost的推导中将会使用此方法。

## XGBoost

XGBoost是对梯度提升算法的改进，求解损失函数极值时使用了牛顿法，将损失函数泰勒展开到二阶，另外在损失函数中加入了正则化项。训练时的目标函数由两部分构成，第一部分为梯度提升算法损失，第二部分为正则化项。XGBoost的损失函数定义为

![L(\phi )=\sum_{i=1}^{n}l(y_{i}^{'},y_{i})+\sum _{k}\Omega (f_{k})](https://www.zhihu.com/equation?tex=L%28%5Cphi+%29%3D%5Csum_%7Bi%3D1%7D%5E%7Bn%7Dl%28y_%7Bi%7D%5E%7B%27%7D%2Cy_%7Bi%7D%29%2B%5Csum+_%7Bk%7D%5COmega+%28f_%7Bk%7D%29)

其中 ![n](https://www.zhihu.com/equation?tex=n) 为训练样本数， ![l](https://www.zhihu.com/equation?tex=l) 是对单个样本的损失， ![y'_i](https://www.zhihu.com/equation?tex=y%27_i) 为预测值， ![y_i](https://www.zhihu.com/equation?tex=y_i) 为样本真实标签值， ![\phi](https://www.zhihu.com/equation?tex=%5Cphi) 为模型的参数。正则化项定义了模型的复杂程度

![\Omega (f)=\gamma T+\frac{1}{2}\lambda \left \| w \right \|^{2}](https://www.zhihu.com/equation?tex=%5COmega+%28f%29%3D%5Cgamma+T%2B%5Cfrac%7B1%7D%7B2%7D%5Clambda+%5Cleft+%5C%7C+w+%5Cright+%5C%7C%5E%7B2%7D)

其中 ![\lambda](https://www.zhihu.com/equation?tex=%5Clambda) 和 ![\gamma](https://www.zhihu.com/equation?tex=%5Cgamma) 为人工设置的系数， ![w](https://www.zhihu.com/equation?tex=w) 为决策树所有叶子节点值形成的向量， ![T](https://www.zhihu.com/equation?tex=T) 为叶子节点数。正则化项由叶子节点数，叶子节点值向量的模平方两项构成，第一项体现了决策树结构的复杂程度，第二项体现了决策树预测值的复杂程度。定义 ![q](https://www.zhihu.com/equation?tex=q) 为输入向量 ![x](https://www.zhihu.com/equation?tex=x) 到决策树叶子节点编号的映射，即根据输入向量确定用决策树的第几个叶子节点值来预测它的输出值

![i=q(x):\mathbb{R}^{d}\rightarrow \mathbb{Z}](https://www.zhihu.com/equation?tex=i%3Dq%28x%29%3A%5Cmathbb%7BR%7D%5E%7Bd%7D%5Crightarrow+%5Cmathbb%7BZ%7D)
其中 ![d](https://www.zhihu.com/equation?tex=d) 为特征向量的维数。因此 ![q](https://www.zhihu.com/equation?tex=q) 定义了树的结构， ![w](https://www.zhihu.com/equation?tex=w) 定义了树的输出值。决策树的映射函数可以写成

![f(x)=w_{q(x)}](https://www.zhihu.com/equation?tex=f%28x%29%3Dw_%7Bq%28x%29%7D)
与梯度提升算法相同，采用加法模型表示强学习器。假设 ![y'_{i,t}](https://www.zhihu.com/equation?tex=y%27_%7Bi%2Ct%7D) 第 ![i](https://www.zhihu.com/equation?tex=i) 个样本在第 ![t](https://www.zhihu.com/equation?tex=t) 次迭代时的强学习器预测值，训练时依次确定每一个弱学习器函数 ![f_t](https://www.zhihu.com/equation?tex=f_t) ，加到强学习器预测函数中，即最小化如下目标函数

![L_{t}=\sum_{i=1}^{n}l(y_{i},y_{i,t-1}+f_{t}(x_{i}))+\Omega (f_{t})](https://www.zhihu.com/equation?tex=L_%7Bt%7D%3D%5Csum_%7Bi%3D1%7D%5E%7Bn%7Dl%28y_%7Bi%7D%2Cy_%7Bi%2Ct-1%7D%2Bf_%7Bt%7D%28x_%7Bi%7D%29%29%2B%5COmega+%28f_%7Bt%7D%29)
实现时用贪婪法将 ![f_t](https://www.zhihu.com/equation?tex=f_t) 加入到模型中，以最小化目标函数值。注意，在这里 ![y_i](https://www.zhihu.com/equation?tex=y_i) 是常数， ![y'_{i,t-1}+f_t(x_i)](https://www.zhihu.com/equation?tex=y%27_%7Bi%2Ct-1%7D%2Bf_t%28x_i%29) 是损失函数的自变量，而也是常数。对于一般的损失函数无法求得上面优化问题的公式解。采用牛顿法近似求解，对目标函数在点处作二阶泰勒展开后得到

![L\approx \sum_{i=1}^{n}\left ( l(y_{i},y_{i,t-1})+\frac{1}{2}h_{i}f_{t}^{2}(x_{i}) \right )+\Omega (f_{t})](https://www.zhihu.com/equation?tex=L%5Capprox+%5Csum_%7Bi%3D1%7D%5E%7Bn%7D%5Cleft+%28+l%28y_%7Bi%7D%2Cy_%7Bi%2Ct-1%7D%29%2B%5Cfrac%7B1%7D%7B2%7Dh_%7Bi%7Df_%7Bt%7D%5E%7B2%7D%28x_%7Bi%7D%29+%5Cright+%29%2B%5COmega+%28f_%7Bt%7D%29)
损失函数的一阶导数为

![g_{i}=\frac{\partial l(y_{i},y_{t-1})}{\partial y_{t-1}}](https://www.zhihu.com/equation?tex=g_%7Bi%7D%3D%5Cfrac%7B%5Cpartial+l%28y_%7Bi%7D%2Cy_%7Bt-1%7D%29%7D%7B%5Cpartial+y_%7Bt-1%7D%7D)
与梯度提升算法相同，是将之前已经训练得到的强学习器对样本的预测值当做变量求导，这一点一定要理解，很多读者困惑的地方在于不知道这个导数是对谁求导。损失函数的二阶导数为

![h_{i}=\frac{\partial^{2}l(y_{i},y_{t-1})}{\partial ^{2}y_{t-1}}](https://www.zhihu.com/equation?tex=h_%7Bi%7D%3D%5Cfrac%7B%5Cpartial%5E%7B2%7Dl%28y_%7Bi%7D%2Cy_%7Bt-1%7D%29%7D%7B%5Cpartial+%5E%7B2%7Dy_%7Bt-1%7D%7D)
去掉 ![f_t(x_i)](https://www.zhihu.com/equation?tex=f_t%28x_i%29) 与无关的常数项，可以得到化简后的损失函数为

![L_{t}=\sum_{i=1}^{n}\left ( g_{i}f_{t}(x_{i})+\frac{1}{2}h_{i}f_{t}^{2}(x_{i}) \right )+\Omega (f_{t})](https://www.zhihu.com/equation?tex=L_%7Bt%7D%3D%5Csum_%7Bi%3D1%7D%5E%7Bn%7D%5Cleft+%28+g_%7Bi%7Df_%7Bt%7D%28x_%7Bi%7D%29%2B%5Cfrac%7B1%7D%7B2%7Dh_%7Bi%7Df_%7Bt%7D%5E%7B2%7D%28x_%7Bi%7D%29+%5Cright+%29%2B%5COmega+%28f_%7Bt%7D%29)
如果定义集合

![I_{J}=\left \{ i\mid q(x_{i})=j \right \}](https://www.zhihu.com/equation?tex=I_%7BJ%7D%3D%5Cleft+%5C%7B+i%5Cmid+q%28x_%7Bi%7D%29%3Dj+%5Cright+%5C%7D)

即预测值为第 ![j](https://www.zhihu.com/equation?tex=j) 个叶子节点的训练样本集合（样本下标集合）。由于每个训练样本只属于某一个叶子节点，目标函数可以拆分成对所有叶子节点损失函数的和

![\begin{align*} L_{t} &=\sum_{i=1}^{n}\left ( g_{i}f_{t}(x_{i})+\frac{1}{2}h_{i}f_{t}^{2}(x_{i}) \right )+\gamma T+\frac{1}{2}\lambda \sum_{j=1}^{T}w_{j}^{2} \\   &= \sum_{j=1}^{T}\left ( (\sum _{i\in I_{j}}g_{i})w_{j}+\frac{1}{2}(\sum _{i\in I_{j}}h_{i}+\lambda )w_{j}^{2} \right )+\gamma T \end{align*}](https://www.zhihu.com/equation?tex=%5Cbegin%7Balign%2A%7D+L_%7Bt%7D+%26%3D%5Csum_%7Bi%3D1%7D%5E%7Bn%7D%5Cleft+%28+g_%7Bi%7Df_%7Bt%7D%28x_%7Bi%7D%29%2B%5Cfrac%7B1%7D%7B2%7Dh_%7Bi%7Df_%7Bt%7D%5E%7B2%7D%28x_%7Bi%7D%29+%5Cright+%29%2B%5Cgamma+T%2B%5Cfrac%7B1%7D%7B2%7D%5Clambda+%5Csum_%7Bj%3D1%7D%5E%7BT%7Dw_%7Bj%7D%5E%7B2%7D+%5C%5C+++%26%3D+%5Csum_%7Bj%3D1%7D%5E%7BT%7D%5Cleft+%28+%28%5Csum+_%7Bi%5Cin+I_%7Bj%7D%7Dg_%7Bi%7D%29w_%7Bj%7D%2B%5Cfrac%7B1%7D%7B2%7D%28%5Csum+_%7Bi%5Cin+I_%7Bj%7D%7Dh_%7Bi%7D%2B%5Clambda+%29w_%7Bj%7D%5E%7B2%7D+%5Cright+%29%2B%5Cgamma+T+%5Cend%7Balign%2A%7D)

首先介绍叶子节点的值如何确定。如果决策树的结构即 ![q(x)](https://www.zhihu.com/equation?tex=q%28x%29) 确定，根据牛顿法可以得到第 ![j](https://www.zhihu.com/equation?tex=j) 个叶子节点的最优值为

![w_{j}^{*}=-\frac{\sum _{i\in I_{j}}g_{i}}{\sum _{i\in I_{j}}h_{i}+\lambda }](https://www.zhihu.com/equation?tex=w_%7Bj%7D%5E%7B%2A%7D%3D-%5Cfrac%7B%5Csum+_%7Bi%5Cin+I_%7Bj%7D%7Dg_%7Bi%7D%7D%7B%5Csum+_%7Bi%5Cin+I_%7Bj%7D%7Dh_%7Bi%7D%2B%5Clambda+%7D)
这是单个叶子节点的损失函数对 ![w_i](https://www.zhihu.com/equation?tex=w_i) 求导并令导数为 ![0](https://www.zhihu.com/equation?tex=0) 后解方程的结果。前面已经假定对单个样本的损失函数是凸函数，因此必定是极小值。单个叶子节点的损失函数对 ![w_j](https://www.zhihu.com/equation?tex=w_j) 的一阶导数为

![\frac{\partial }{\partial _{w_{j}}}\left ( (\sum _{i\in I_{j}}g_{i})w_{j}+\frac{1}{2}(\sum _{i\in I_{j}}h_{i}+\lambda )w_{j}^{2} \right )=\sum _{i\in I_{j}}g_{i}+(\sum _{i\in I_{j}}h_{i}+\lambda )w_{j}](https://www.zhihu.com/equation?tex=%5Cfrac%7B%5Cpartial+%7D%7B%5Cpartial+_%7Bw_%7Bj%7D%7D%7D%5Cleft+%28+%28%5Csum+_%7Bi%5Cin+I_%7Bj%7D%7Dg_%7Bi%7D%29w_%7Bj%7D%2B%5Cfrac%7B1%7D%7B2%7D%28%5Csum+_%7Bi%5Cin+I_%7Bj%7D%7Dh_%7Bi%7D%2B%5Clambda+%29w_%7Bj%7D%5E%7B2%7D+%5Cright+%29%3D%5Csum+_%7Bi%5Cin+I_%7Bj%7D%7Dg_%7Bi%7D%2B%28%5Csum+_%7Bi%5Cin+I_%7Bj%7D%7Dh_%7Bi%7D%2B%5Clambda+%29w_%7Bj%7D)
令其为0，即可得到上面的结果。

接下来说明如何确定决策树的结构，即寻找最佳分裂。将 ![w_j](https://www.zhihu.com/equation?tex=w_j) 的最优解代入损失函数，得到只含有 ![q](https://www.zhihu.com/equation?tex=q) 的损失函数

![\begin{aligned}  L_{t}(q)&=\sum_{j=1}^{T}\left ( (\sum _{i\in I_{j}}g_{i})(-\frac{\sum _{i\in I_{j}}g_{i}}{\sum _{i\in I_{j}}h_{i}+\lambda })+\frac{1}{2}(\sum _{i\in I_{j}}h_{i}+\lambda ) (-\frac{\sum _{i\in I_{j}}g_{i}}{\sum _{i\in I_{j}}h_{i}+\lambda })^{2}\right )+\gamma T \\   &=\sum_{j=1}^{T}\left ( -\frac{(\sum _{i\in I_{j}}g_{i})^{2}}{\sum _{i\in I_{j}}+\lambda }+\frac{1}{2}\frac{(\sum _{i\in I_{j}}g_{i})^{2}}{\sum _{i\in I_{j}}h_{i}+\lambda } \right )+\gamma T \\   &= -\frac{1}{2}\sum_{J=1}^{T}\frac{(\sum _{i\in I_{j}}g_{i})^{2}}{\sum _{i\in I_{j}}h_{i}+\lambda }+\gamma T  \end{aligned}](https://www.zhihu.com/equation?tex=%5Cbegin%7Baligned%7D++L_%7Bt%7D%28q%29%26%3D%5Csum_%7Bj%3D1%7D%5E%7BT%7D%5Cleft+%28+%28%5Csum+_%7Bi%5Cin+I_%7Bj%7D%7Dg_%7Bi%7D%29%28-%5Cfrac%7B%5Csum+_%7Bi%5Cin+I_%7Bj%7D%7Dg_%7Bi%7D%7D%7B%5Csum+_%7Bi%5Cin+I_%7Bj%7D%7Dh_%7Bi%7D%2B%5Clambda+%7D%29%2B%5Cfrac%7B1%7D%7B2%7D%28%5Csum+_%7Bi%5Cin+I_%7Bj%7D%7Dh_%7Bi%7D%2B%5Clambda+%29+%28-%5Cfrac%7B%5Csum+_%7Bi%5Cin+I_%7Bj%7D%7Dg_%7Bi%7D%7D%7B%5Csum+_%7Bi%5Cin+I_%7Bj%7D%7Dh_%7Bi%7D%2B%5Clambda+%7D%29%5E%7B2%7D%5Cright+%29%2B%5Cgamma+T+%5C%5C+++%26%3D%5Csum_%7Bj%3D1%7D%5E%7BT%7D%5Cleft+%28+-%5Cfrac%7B%28%5Csum+_%7Bi%5Cin+I_%7Bj%7D%7Dg_%7Bi%7D%29%5E%7B2%7D%7D%7B%5Csum+_%7Bi%5Cin+I_%7Bj%7D%7D%2B%5Clambda+%7D%2B%5Cfrac%7B1%7D%7B2%7D%5Cfrac%7B%28%5Csum+_%7Bi%5Cin+I_%7Bj%7D%7Dg_%7Bi%7D%29%5E%7B2%7D%7D%7B%5Csum+_%7Bi%5Cin+I_%7Bj%7D%7Dh_%7Bi%7D%2B%5Clambda+%7D+%5Cright+%29%2B%5Cgamma+T+%5C%5C+++%26%3D+-%5Cfrac%7B1%7D%7B2%7D%5Csum_%7BJ%3D1%7D%5E%7BT%7D%5Cfrac%7B%28%5Csum+_%7Bi%5Cin+I_%7Bj%7D%7Dg_%7Bi%7D%29%5E%7B2%7D%7D%7B%5Csum+_%7Bi%5Cin+I_%7Bj%7D%7Dh_%7Bi%7D%2B%5Clambda+%7D%2B%5Cgamma+T++%5Cend%7Baligned%7D)

此函数可以看做是对决策树结构优劣的一个度量，要求其极小值，类似于决策树寻找分裂时的不纯度指标。假设节点在分裂之前的训练样本集为 ![I](https://www.zhihu.com/equation?tex=I) ，分裂之后左子节点的训练样本集为 ![I_L](https://www.zhihu.com/equation?tex=I_L) ，右子节点的训练样本集为 ![I_R](https://www.zhihu.com/equation?tex=I_R) 。分裂之后的叶子节点数增加1，因此上面目标函数的第二项由 ![\gamma T](https://www.zhihu.com/equation?tex=%5Cgamma+T) 变为 ![\gamma(T+1)](https://www.zhihu.com/equation?tex=%5Cgamma%28T%2B1%29) ，二者的差值为 ![\gamma T](https://www.zhihu.com/equation?tex=%5Cgamma+T) 。将上面的目标函数取反， ![L_t(q)](https://www.zhihu.com/equation?tex=L_t%28q%29) 的极小值等价于求 ![-L_t(q)](https://www.zhihu.com/equation?tex=-L_t%28q%29) 的极大值。寻找最佳分裂的目标是使得损失函数的下降值最大化

![L_{split}=\frac{1}{2}\left ( \frac{(\sum _{i\in I_{L}}g_{i})^{2}}{\sum _{i\in I_{L}}h_{i}+\lambda }+\frac{(\sum _{i\in I_{R}}g_{i})^{2}}{\sum _{i\in I_{R}}h_{i}+\lambda} -\frac{(\sum _{i\in I}g_{i})^{2}}{\sum _{i\in I}h_{i}+\lambda}\right )-\gamma](https://www.zhihu.com/equation?tex=L_%7Bsplit%7D%3D%5Cfrac%7B1%7D%7B2%7D%5Cleft+%28+%5Cfrac%7B%28%5Csum+_%7Bi%5Cin+I_%7BL%7D%7Dg_%7Bi%7D%29%5E%7B2%7D%7D%7B%5Csum+_%7Bi%5Cin+I_%7BL%7D%7Dh_%7Bi%7D%2B%5Clambda+%7D%2B%5Cfrac%7B%28%5Csum+_%7Bi%5Cin+I_%7BR%7D%7Dg_%7Bi%7D%29%5E%7B2%7D%7D%7B%5Csum+_%7Bi%5Cin+I_%7BR%7D%7Dh_%7Bi%7D%2B%5Clambda%7D+-%5Cfrac%7B%28%5Csum+_%7Bi%5Cin+I%7Dg_%7Bi%7D%29%5E%7B2%7D%7D%7B%5Csum+_%7Bi%5Cin+I%7Dh_%7Bi%7D%2B%5Clambda%7D%5Cright+%29-%5Cgamma)
![\gamma](https://www.zhihu.com/equation?tex=%5Cgamma) 是常数，因此上式最右边的 ![-\gamma](https://www.zhihu.com/equation?tex=-%5Cgamma) 可以忽略。除了使用不同的分裂指标，其他过程与标准的决策树训练算法相同。在实现时将上面公式中的求和项定义为几个变量，分别是所有训练样本的一阶导数，二阶导数之和

![G=\sum _{i\in I}g_{i},H=\sum _{i\in I}h_{i}](https://www.zhihu.com/equation?tex=G%3D%5Csum+_%7Bi%5Cin+I%7Dg_%7Bi%7D%2CH%3D%5Csum+_%7Bi%5Cin+I%7Dh_%7Bi%7D)
左右子集样本的一阶导数，二阶导数之和

![G_{L}=\sum _{i\in I_{L}}g_{i},H_{L}=\sum _{i\in I_{L}}h_{i}\\ G_{R}=\sum _{i\in I_{R}}g_{i},H_{R}=\sum _{i\in I_{R}}h_{i}](https://www.zhihu.com/equation?tex=G_%7BL%7D%3D%5Csum+_%7Bi%5Cin+I_%7BL%7D%7Dg_%7Bi%7D%2CH_%7BL%7D%3D%5Csum+_%7Bi%5Cin+I_%7BL%7D%7Dh_%7Bi%7D%5C%5C+G_%7BR%7D%3D%5Csum+_%7Bi%5Cin+I_%7BR%7D%7Dg_%7Bi%7D%2CH_%7BR%7D%3D%5Csum+_%7Bi%5Cin+I_%7BR%7D%7Dh_%7Bi%7D)

寻找最佳分裂的算法流程为

输入数据： ![I](https://www.zhihu.com/equation?tex=I) ，当前节点的训练样本集，训练样本数为

输入数据： ![d](https://www.zhihu.com/equation?tex=d) ，特征向量维数

初始化分裂质量： ![score \leftarrow 0](https://www.zhihu.com/equation?tex=score+%5Cleftarrow+0)

计算所有样本的一阶导数，二阶导数之和：

![G\leftarrow \sum_{i\in I}g_i,H\leftarrow \sum_{i\in I}h_i](https://www.zhihu.com/equation?tex=G%5Cleftarrow+%5Csum_%7Bi%5Cin+I%7Dg_i%2CH%5Cleftarrow+%5Csum_%7Bi%5Cin+I%7Dh_i)

循环，对 ![k=1,\cdots,d](https://www.zhihu.com/equation?tex=k%3D1%2C%5Ccdots%2Cd)

对所有训练样本按照第 ![k](https://www.zhihu.com/equation?tex=k) 个特征升序排序

初始化左子集所有训练样本的一阶导数，二阶导数之和： ![G_L \leftarrow 0, H_L=0](https://www.zhihu.com/equation?tex=G_L+%5Cleftarrow+0%2C+H_L%3D0)

循环，对 ![j=1,\cdots,n](https://www.zhihu.com/equation?tex=j%3D1%2C%5Ccdots%2Cn) ，以第 ![j](https://www.zhihu.com/equation?tex=j) 个样本的第 ![k](https://www.zhihu.com/equation?tex=k) 个特征分量 ![x_{jk}](https://www.zhihu.com/equation?tex=x_%7Bjk%7D) 作为分裂阈值

计算左子集所有样本的一阶导数和二阶导数之和，在之前的基础上加上本次被从右

边分到左边的样本的一阶导数和二阶导数值即可：

![G\_L \leftarrow G\_L +g_i, H\_L \leftarrow H_L + h_j](https://www.zhihu.com/equation?tex=G%5C_L+%5Cleftarrow+G%5C_L+%2Bg_i%2C+H%5C_L+%5Cleftarrow+H_L+%2B+h_j)

计算右子集所有样本的一阶导数和二阶导数之和，可以根据总和，左子集的和快速

计算：

![G\_R \leftarrow G -G_L, H\_R \leftarrow H - H_L](https://www.zhihu.com/equation?tex=G%5C_R+%5Cleftarrow+G+-G_L%2C+H%5C_R+%5Cleftarrow+H+-+H_L)

计算分裂分数的最大值：

![score\leftarrow max\left ( score,\frac{G_{L}^{2}}{H_{L}+\lambda }+\frac{G_{R}^{2}}{H_{R}+\lambda }-\frac{G^{2}}{H+\lambda } \right )](https://www.zhihu.com/equation?tex=score%5Cleftarrow+max%5Cleft+%28+score%2C%5Cfrac%7BG_%7BL%7D%5E%7B2%7D%7D%7BH_%7BL%7D%2B%5Clambda+%7D%2B%5Cfrac%7BG_%7BR%7D%5E%7B2%7D%7D%7BH_%7BR%7D%2B%5Clambda+%7D-%5Cfrac%7BG%5E%7B2%7D%7D%7BH%2B%5Clambda+%7D+%5Cright+%29)

结束循环

返回：最大分裂质量 ![score](https://www.zhihu.com/equation?tex=score) 及其对应分裂（包括选用的特征，分裂阈值）

XGBoost实现时还使用了权重收缩与列采样技术，以抵抗过拟合。权重收缩的做法是在每训练出一棵决策树之后将其权重进行缩放，乘上系数 ![\eta](https://www.zhihu.com/equation?tex=%5Ceta) 。权重收缩降低了之前的决策树的影响，为将来要生成的决策树留下了更多的空间。列采样的做法与随机森林相同，每次寻找最佳分裂时只考虑一部分随机抽取的特征分量。

## 参考文献

[1] Jerome H Friedman. Greedy function approximation: A gradient boosting machine.  Annals of Statistics. 2001.
[2] Tianqi Chen, Carlos Guestrin. XGBoost: A Scalable Tree Boosting System. knowledge discovery and data mining. 2016.
[3] Jerome Friedman, Trevor Hastie and Robert Tibshirani. Additive logistic regression: a statistical view of boosting. Annals of Statistics 28(2), 337–407. 2000.

