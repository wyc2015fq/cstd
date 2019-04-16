# 重回机器学习-《python机器学习及实践》读书笔记一 - lyx的专栏 - CSDN博客





2018年09月26日 22:21:31[钱塘小甲子](https://me.csdn.net/qtlyx)阅读数：403标签：[机器学习](https://so.csdn.net/so/search/s.do?q=机器学习&t=blog)
个人分类：[机器学习																[量化投资](https://blog.csdn.net/qtlyx/article/category/6276672)](https://blog.csdn.net/qtlyx/article/category/5925605)








        以前也算比较系统接触过机器学习吧，记得最早的时候是大二，机器学习才刚开始提起，更多的是说统计学习。那个时候，深度学习似乎都还没有听过，看的第一本书也是一本外国人写的，一直拿鸢尾花数据集当例子的书。当时看完也没觉得什么，毕竟年轻，何况那个时候很多东西就是觉得好奇好玩而去学一下。

        后来也慢慢接触到，也编程实践过，不过一直都不怎么成体系。最近觉得，还是得再跟一下潮流，所以还是赶紧学习吧。《python机器学习及实践》这本书有点像量化界丁校友的那本书，很广泛；但是又比丁校友那本书好一点，就是具有很强的实践性。当然啦，既然看中实践性了，那么理论知识就不那么多了，所以全书的理论真的没有多少，如果之前完全没有接触过的同学，可能把代码全部敲了一遍也没有什么深刻的体会，所以还是应该先看一点偏理论的书吧。还好自己多少还有点ml的知识储备，花了半天不到就把这本书看完了，梳理了一遍的感觉还是很不错的，最大的收获大概就是明白怎么用sklearn了吧。以前都是很随意的使用，sklearn对我而言就是一些算法的包装而已，参数搜索、交叉验证什么的 全部自己上手，看完才发现，什么叫做工欲善其事，必先利其器。

        本人对监督学习比较感兴趣，毕竟量化投资本质上就是一个监督学习的问题，可能偶尔的小范围、小课题会涉及到非监督学习。根据书中给的监督学习经典模型的架构如下：
- 首先准备训练数据。可以是文本、图像、音频
- 然后抽取所需要的特征，形成特征向量
- 连同特征向量和对应的label放入模型中学习
- 采用同样的特征提取方法作用于新的测试数据，得到特征向量
- 使用预测模型预测结果（Expected label）

        这个看下来，自己吓了一跳，这不就是多因子框架吗?!准备数据这就不用说了；抽取特征，形成特征向量其实就是单因子的挖掘以及单因子的筛选，比如用滚动的IRIC之类的；第三步在传统的多因子体系中可能不存在显式训练模型，但是其实如果对IC、加权等方法理解深入的话，其实这就是一个线性模型的变现复杂版；然后是第四、五步，那就更加不用说了，就是因子打分、合成的情形了。当然啦，多因子模型到这里其实只是完成了一半还不到，后面还有组合层面。所以说，其实不管你用不用机器学习在多因子模型中，这个模型本身就是一个机器学习！

        然后，就开始上一段代码吧，代码不重要，重要的是sklearn使用的结构和步骤。

1.获取原始数据（特征提取，本例中已经提取）

```python
import pandas as pd
import numpy as np

column_names = ['Sample code number', 'Clump Thickness', 'Uniformity of Cell Size', 'Uniformity of Cell Shape', 'Marginal Adhesion', 'Signle Epithelial Cell Size',
                'Bare Nuclei', 'Bland Chrom', 'Normal Nucleoli', 'Mitoses', 'Class']
data = pd.read_csv('https://archive.ics.uci.edu/ml/machine-learning-databases/breast-cancer-wisconsin/breast-cancer-wisconsin.data', names=column_names)
data = data.replace(to_replace='?', value=np.nan)
data = data.dropna(how='any')
```

2.切割训练集和测试集

```python
from sklearn.cross_validation import train_test_split
# 数据集第一列是id，所以x数据从1到9，即1:10
X_train, X_test,y_train,y_test = train_test_split(data[column_names[1:10]], data[column_names[10]], test_size=0.25)
```

3.对数据进行标准化（有的模型天然不需要这一步，比如DT）

```python
from sklearn.preprocessing import StandardScaler
from sklearn.linear_model import LogisticRegression, SGDClassifier
ss = StandardScaler()X_train = ss.fit_transform(X_train)
X_test = ss.transform(X_test)
```

4.模型训练

```python
lr = LogisticRegression()
sgdc=SGDClassifier()
```

5.样本外测试

```python
from sklearn.metrics import classification_report
lr.fit(X_train, y_train)
lr_y_predict = lr.predict(X_test)
sgdc.fit(X_train, y_train)
sgdc_y_predict = sgdc.predict(X_test)
print 'accuracy:', lr.score(X_test, y_test)
print classification_report(y_test, lr_y_predict, target_names=['Benign', 'Maligant'])
print 'accuracy:', sgdc.score(X_test, y_test)
print classification_report(y_test, sgdc_y_predict, target_names=['Benign', 'Maligant'])
```

        详细的代码就不解释了，毕竟我不是书本的搬运工，书本中说的很明白。




