# 机器学习——朴素贝叶斯（Naive Bayes）详细解读 - 技术分享 - CSDN博客

2018年11月05日 16:24:20[丿一叶秋丶](https://me.csdn.net/zhuqiang9607)阅读数：1259


在机器学习中，朴素贝叶斯是一个分类模型，输出的预测值是离散值。在讲该模型之前首先有必要先了解**贝叶斯定理**，以该定理为基础的统计学派在统计学领域占据重要的地位，它是从观察者的角度出发，观察者所掌握的信息量左右了观察者对事件的认知。

## 贝叶斯公式

$P(A|B) = \frac{P(B|A)P(A)}{P(B)} = \frac{P(B|A)P(A)}{\sum_{A}^{ }P(B|A)P(A)}$

其中，$P(B|A)$是事件 B 在另一个事件 A已经发生条件下的概率,$\sum_{A}^{ }P(B|A)P(A)$表示A所有可能情况下的概率，现在要来求事件A在事件B发生情况下的条件概率$P(A|B)$，又称后验概率。

## 举例说明

#### 例1

工厂生产产品，合格产品的概率是0.9，误检的概率是0.2，误检情况中合格产品的概率是0.1，那合格产品被误检的概率是多少？

直接套用贝叶斯公式，可得：

$P(误检|合格) = \frac{P(合格|误检)P(误检)}{P(合格)} = \frac{0.1*0.2}{0.9}=\frac{1}{45}$

#### 例2

假设有个严重的疾病，发生的概率是万分之一，现在某人神经大条，怀疑得了该病，跑到医院检查，结果确有此病，不过医院的测试报告只有99%准确度（另外，也指正常人检测99%没问题，1%检测错误）。那么，他的死亡风险有多大？

咋一看，此人快要和世界说拜拜了，这个时候**贝叶斯公式**派上用场了，其中，用D表示该病（D=1，得病；D=0，健康），P表示报告准确度（T=1，准确；T=0，错误），可得:

$P(D=1|T=1) = \frac{P(T=1|D=1)P(D=1)}{P(T=1|D=1)P(D=1)+P(T=1|D=0)P(D=0)}= \frac{0.99*0.0001}{0.99*0.0001+0.01*0.9999}=0.0098$

可见，患病的几率很小，并没有想象中可怕。

## 朴素贝叶斯分类器（Naive Bayes Classifier）

用朴素贝叶斯分类器有个**前提**，需要假设所有的特征在数据集中是同等重要，但又相互独立，显然在现实情况中这是不太可能的，但还是要尽可能地满足它这个“**朴素**”的要求。

朴素贝叶斯的**朴素思想**，对于给定的待分类项,求解在此分类项出现的条件下，各个类别出现的概率，谁的概率最大，该分类项就属于哪个类别。

#### 朴素贝叶斯的公式

在特征项集合：$F=\left \{  f_{1}, f_{2}, f_{3}, \cdot \cdot \cdot, f_{m}\right    \}$ 和类别集合：$C=\left \{  c_{1}, c_{2}, c_{3}, \cdot \cdot \cdot, C_{n}\right \}$条件的下，然后根据贝叶斯定理，可得：

$P(c_{k}|f_{1}, f_{2}, f_{3}, \cdot \cdot \cdot, f_{m}) = \frac{P(c_{k})P(f_{1}, f_{2}, f_{3}, \cdot \cdot \cdot, f_{m}|c_{k})}{P(f_{1}, f_{2}, f_{3}, \cdot \cdot \cdot, f_{m})}(k=1,2,3,\cdot \cdot \cdot,n )$

由于都要除以$P(f_{1}, f_{2}, f_{3}, \cdot \cdot \cdot, f_{m})$,故原公式简化为：

$P(c_{k}|f_{1}, f_{2}, f_{3}, \cdot \cdot \cdot, f_{m}) = P(c_{k})P(f_{1}, f_{2}, f_{3}, \cdot \cdot \cdot, f_{m}|c_{k})$

各个特征属性独立，则公式变为：

$P(c_{k}|f_{1}, f_{2}, f_{3}, \cdot \cdot \cdot, f_{m})=P(c_{k})P(f_{1}|c_{k})P(f_{2}|c_{k})P(f_{3}|c_{k})\cdot \cdot \cdot P(f_{m}|c_{k})=P(c_{k})\prod_{j=1}^{m}P(f_{j}|c_{k})$

而所要求的就是*arg max*$P(c_{k}|f_{1}, f_{2}, f_{3}, \cdot \cdot \cdot, f_{m})$，不过针对离散情况，只要求出各个$P(f_{j}|c_{k})$就行，但是面对连续值，可以使用高斯分布来求值，高斯函数如下：

$P(x_{i}|c)=\frac{1}{\sqrt{2\pi\sigma^{2}}}exp(-\frac{(x_{i}-\mu )^{2}}{2\sigma ^{2}})$

而关键是要求解公式中的$\mu,\sigma^{2}$，可行的途径就是使用极大似然估计法（频率学派的方式，他们认为世界是确定的，有一个本体，这个本体的真值不变，而我们的目标是要找到这个真值或真值所在的范围），所以高斯函数中的$\mu,\sigma^{2}$是可以求解的。

#### 极大似然估计（Maximum Likelihood Estimation, MLE）

参数$\mu,\sigma^{2}$的似然函数记作$L(\mu,\sigma^{2})$，表示了m个样本$X_{1}, X_{2}, X_{3}, \cdot \cdot \cdot, X_{m}$在特征$c_{k}$上的联合概率分布：

$L(\mu,\sigma^{2})=\prod_{j=1}^{m}P(x_{i}^{j};\mu,\sigma^{2})=\prod_{j=1}^{m}\frac{1}{\sqrt{2\pi\sigma^{2}}}exp(-\frac{(x_{i}^{j}-\mu )^{2}}{2\sigma ^{2}})$

其中$x_{i}^{j}$表示的是第$j$个样本的第$i$个特征，为了方便求解，会在两边添加对数，可得：

$lnL(\mu,\sigma^{2})=ln\prod_{j=1}^{m}\frac{1}{\sqrt{2\pi\sigma^{2}}}exp(-\frac{(x_{i}^{j}-\mu )^{2}}{2\sigma ^{2}})$

化简可得：

$lnL(\mu,\sigma^{2})=-\frac{n}{2}ln2\pi-\frac{n}{2}ln\sigma^{2}-\frac{1}{2\sigma^{2}}\sum_{i=1}^{m}(x_{i}^{j}-\mu )^{2}$

分别对$\mu,\sigma^{2}$求偏导可得：

$\frac{\partial lnL(\mu,\sigma^{2})}{\partial\mu } =\frac{1}{\sigma^{2}}\sum_{i=1}^{m}(x_{i}^{j}-\mu )=0$

$\frac{\partial lnL(\mu,\sigma^{2})}{\partial\sigma^{2} } =-\frac{1}{2\sigma^{2}}\sum_{i=1}^{m}(1- \frac{(x_{i}^{j}-\mu )^{2}}{2\sigma^{2}}) )=0$

求解有：

$\mu=\frac{1}{m}\sum_{i=1}^{m}x_{i}^{j}$

$\sigma^{2}=\frac{1}{m}\sum_{i=1}^{m}(x_{i}^{j}-\mu )^{2}$

其实就是求某个类别对应每个特征的平均值(mean)和方差(variance)，有了参数$\mu,\sigma^{2}$，就可以很容易估计第$i$个特征的分布，从而求解$x_{i}$的概率。

#### 例3

有一些鸢尾花数据，总共有150行，其中特征有四个，分别是萼片的长度，萼片的宽度，花瓣的长度，花瓣的宽度，类别有三个，分别是setosa、versicolor和virginica，然后给你一组待预测数据[3.1, 4.4, 2.1, 3.1]，估计它属于哪个类？(样本地址：[https://download.csdn.net/download/zhuqiang9607/10764313](https://download.csdn.net/download/zhuqiang9607/10764313))

**代码：**

```
import pandas as pd
from sklearn.naive_bayes import GaussianNB

data = pd.read_csv()
class_features = ["Sepal.Length","Sepal.Width","Petal.Length","Petal.Width"]
X_train = data[class_features]
y_train = data["Species"]
gnb = GaussianNB()
gnb.fit(X_train, y_train)
print (gnb.predict(X_test))
```

如果想深入地了解高斯贝叶斯的代码实现，可以访问github进行下载：[https://github.com/AutumnBoat/MachineLearning/blob/master/GaussianNaiveBayes.ipynb](https://github.com/AutumnBoat/MachineLearning/blob/master/GaussianNaiveBayes.ipynb)

