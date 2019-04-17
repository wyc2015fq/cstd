# PMF Model - lijunweiyhn的博客 - CSDN博客





2016年07月04日 22:53:41[_Junwei](https://me.csdn.net/lijunweiyhn)阅读数：1400








> 
**转载自：http://blog.csdn.net/shenxiaolu1984/article/details/50372909**



Mnih, Andriy, and Ruslan Salakhutdinov. “Probabilistic matrix factorization.” Advances in neural information processing systems. 2007.



本篇论文发表于2007年NIPS。Ruslan Salakhutdinov来自多伦多大学，16年转入CMU。Andriy Mnih同样来自多伦多大学，师从Hinton。PMF算法（Probabilistic Matrix Factorization）是现代推荐系统的基础算法之一。

## 问题描述


设有个用户，部电影。一个评分系统可以用矩阵来表示。 

推荐系统问题如下：矩阵中只有部分元素是已知的（用户只给一部分电影打过分），且往往非常稀疏，需要求出缺失的部分。 

除了推荐系统，这个模型也可以用来描述任意“成对”作用的系统。例如：由若干球队组成的联赛，两支球队间的历史比分即为的已知元素，需要预测尚未进行的比赛结果。这里是一个方阵。
## 基本思路


本文采取low-dimensional factor模型，也称为low rank模型来处理这个问题。其核心思想是：用户和电影之间的关系（即用户对电影的偏好）可以由较少的几个因素的线性组合决定。

> 
例子

用户是否喜欢一部电影取决于三个因素：是娱乐片还是文艺片，是外文片还是华语片，演员是否出名。 

用三维向量来描述一个用户（假设取值在[-1,1]之间）：他比较喜欢娱乐片，只看外文片，对演员要求一般，小众一点更好。 

对于一部电影，用另一个三维向量来描述：这是一部众星云集的-国产-娱乐大作。 

可以算出这个用户对于这部电影的喜好程度  ：相当不喜欢。

用矩阵语言来描述，就是评分矩阵可以分解为两个低维矩阵的乘积，其中矩阵描述个用户的属性，矩阵描述部电影的属性。 

根据矩阵秩的性质，的秩不超过的最小尺寸。


实际上，由于系统噪音存在，不可能做出这样的完美分解，另外包含很多未知元素。所以问题转化为： 

- 对一个近似矩阵进行分解

- 要求近似矩阵在观测到的评分部分和观测矩阵尽量相似 

- 为了防止过拟合，需要对做某种形式的约束

用贝叶斯观点来说，是观测到的值，描述了系统的内部特征，是需要估计的。

## 基础PMF模型


使用如下两个假设 

- 观测噪声（观测评分矩阵和近似评分矩阵之差）为高斯分布 

- 用户属性和电影属性均为高斯分布

利用第一个假设，可以写出完整观测矩阵的概率密度函数。其中是观测噪声的方差，人工设定。 







利用第二个假设，可以写出用户、电影属性的概率密度函数。其中是先验噪声的方差，人工设定。 







综合以上两个概率密度函数，利用经典的后验概率推导，可以得到 






## 基础PMF求解


最大化上述概率，则可以通过已有的观测矩阵估计出系统参数。


为了计算方便，对后验概率取对数 






> 
高斯分布公式及其对数形式： 














由于后验概率中的方差都是预设常数，故只有第二项和待优化的有关。 

最大化上述对数后验概率，等价于最小化如下能量函数： 



做参数替换约掉一个变量： 






如果系统先验方差无穷大（即无法对系统参数做约束），则上式只剩第一项，退化为一个SVD分解问题。


刚才的几步推导中，为了书写简便实际上做了一些省略：矩阵的概率密度应该等于其元素概率密度的乘积。取对数之后，即等于其元素概率密度的和。 



其中是标量，都是维度为D的向量。后两项相当于约束了内部特征矩阵的范数。标记表示用户i是否对电影j评分。



最后，为了限制评分的范围，对高斯函数的均值施加logistic函数，其取值在(-1,1)之间。最终的能量函数是： 







至此，可以使用梯度下降方法，通过求解中的每一个元素。


需要估计的参数数量为。对于每一个参数，由于能量函数第一项只在有观测时需要计算，所以所需时间相对于观测数量为线性（？）。

> 
性能

1998年至2005年Netflix数据，设定D=30，使用Matlab，在30分钟内完成训练。


## 控制模型复杂度


最简单的控制复杂度的方法是调整特征维度：约大，模型越精确，但也越容易过拟合。应该和用户的打分数量相关：如果用户看过的电影多，则可以用较多特征来描述，可以使用较大的D。 

但实际数据往往是不均衡的：电影爱好者给出的打分很多，而很多用户只会给一两部电影打分。


较好的方法是选择一个中等尺度的，之后调整。 
大说明观测噪声大，则第一个误差项不靠谱，较大，应较多依赖后两个正则项：要求系统参数的绝对值较小；反之，大，说明系统参数本身方差大，较小，允许的绝对值较大

## 带有自适应先验的PMF


先验的超参数(hyperparameter)：可以从训练样本中估计。这两个和前述类似。 




类似地，可以给设定先验，轮流对参数和超参数使用梯度下降或者EM算法更新。


## 限制性PMF


“用户是否给某部电影打过分”这个信息本身就能一定程度上说明用户的属性。Constrained PMF尝试把引入到模型中去。这也是本文的创新之处。


用矩阵表述电影对用户的影响。其中第k行表示，如果用户看过第k部电影，则用户应该具有属性。


用户属性U由两部分组成：和之前相同的高斯部分，以及用“看过”矩阵加权的结果。 







其中服从方差为的0均值高斯分布。 

在已知的情况下，同样用梯度下降方法可以求解。


下图用概率图模型表示基础PMF(左)和限定性PMF(右)： 
![](https://img-blog.csdn.net/20151222142302952)
## 实验


涉及的数据集如下
|数据集|打分|用户|电影|
|----|----|----|----|
|Netflix Train|100,480K|480K|17K|
|Netflix Valid|1,408K|-|-|
|Netflix Test|2,817K|-|-|


为了提高训练速度，采用了mini-batche方法：每100K个观测（用户给某部电影打分），更新一次待求参数。learning rate = 0.005， momentum = 0.9。

> 
梯度下降的learning rate和momentum参见这个[链接](http://www.willamette.edu/~gorr/classes/cs449/momrate.html)

简而言之，学习率决定每一步大小，动量避免曲折过于严重。



可以看出限定性PMF比基础PMF的优越性 
![](https://img-blog.csdn.net/20151222144340696)
## 扩展


第6章总结中提到： Efficiency in training PMF models comes from finding only point estimates of model parameters and hyperparameters, instead of inferring the full posterior distribution over them. 

这里的point estimation指的是只估计了的一个值，而没有估计它们的概率分布，所以大大提高了速度。但是其缺点是容易过拟合。 

与之相对的，还可以使用贝叶斯估计，把系统参数当成一个随机变量。具体可以参看这篇博客：[贝叶斯PMF](http://blog.csdn.net/shenxiaolu1984/article/details/50405659)，介绍同作者的这篇论文：
> 
Salakhutdinov, Ruslan, and A. Mnih. “Bayesian probabilistic matrix factorization using markov chain monte carlo.” International Conference on Machine Learning 2008:880-887.



另外，如果需要考虑一些明确的从属信息，例如评分的用户身份、评分发生的时间等，可以参看这篇博客：[DPMF](http://blog.csdn.net/shenxiaolu1984/article/details/50382566)，介绍这篇论文：

> 
Adams, Ryan Prescott, George E. Dahl, and Iain Murray. “Incorporating side information in probabilistic matrix factorization with gaussian processes.” arXiv preprint arXiv:1003.4944 (2010).













