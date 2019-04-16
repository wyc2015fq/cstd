# 重回机器学习-《python机器学习及实践》读书笔记二 - lyx的专栏 - CSDN博客





2018年10月09日 22:18:46[钱塘小甲子](https://me.csdn.net/qtlyx)阅读数：241








## 一.三个率

        机器学习模型训练好之后，会在样本外进行测试，然后我们可以得到三个“率”：
- 准确率
- 召回率
- 精确率

        其实这些也没有什么大不了的，大家如果学习过基本的统计学的话就会知道，这就是所谓的一类错误、二类错误的一个变体。

        首先是准确率，这个最好理解，就是你的模型在样本外测试中正确的次数。当然，我们讨论的前提都是一个二分类问题。这三个“率”是这么定义的：

![](https://img-blog.csdn.net/20181009215353688?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3F0bHl4/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

        这就是书中的公式，Accuracy就不说了。召回率比较有意思，是猜对的原本为真的数目作为分子，然后分母是所有猜对的数目。也就是你模型正确预测中某一类型的占比。精确率是某一类型总数中正确预测的占比。有点绕，理解一下就行。

        为了调和精确率和召回率，还有一个F1指标，其实就是精确率和召回率的调和平均。调和平均有一个特点，会对调和平均的对象的方差进行惩罚。

![](https://img-blog.csdn.net/20181009215759607?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3F0bHl4/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

        也就是说，精确率和召回率越接近，那么F1指标会越好，当然，前提是两个算术均值相同的前提下。

## 二、缺失值的填充

        机器学习建模中也好，量化模型建模中也好，数据预处理永远是逃不过的，也是最重要的步骤。而其中缺失值的填充是一个绕不开的话题。

        在书中的坦坦尼克的例子中，就用了一种最简单的缺失值填充的方法，均值填充。这种方法其实是在没有任何信息的前提下，处于不影响现有信息的目的而采用的。实际过程中，我们可能还会根据数据的分布采用中位数之类的。

        更加复杂的缺失值填充方法有很多，可以单独建立一个缺失值填充模型。譬如量化中的barra模型采用的因子值的缺失值填充方法，就是采用线性模型填充法。原本对于缺失值填充方法就很有艺术性。

```python
import pandas as pd
titanic = pd.read_csv('http://biostat.mc.vanderbilt.edu/wiki/pub/Main/DataSets/titanic.txt')
X = titanic[['pclass', 'age', 'sex']]
y = titanic['survived']
X['age'].fillna(X['age'].mean(), inplace=True)
```

        数据内容大概是这样的：

![](https://img-blog.csdn.net/20181009220742268?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3F0bHl4/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

## 三、类型数据

        sklearn中有自带的类型数据处理，其实就是变成一个哑变量。

```python
from sklearn.feature_extraction import DictVectorizer
vec = DictVectorizer(sparse=False)
vec.fit_transform(X.to_dict(orient='record'))
```

         最后得到的每一条记录变现下面这样的了：

![](https://img-blog.csdn.net/20181009220818476?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3F0bHl4/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

        简单解释一下，就是原来的age由于是数字，所以依然保留，而后面的pclass和sex则被sklearn自动识别为类别变量，然后自动转化为哑变量的。哑变量的含义如下，可以通过feature_names_方法来获得。

![](https://img-blog.csdn.net/20181009221218323?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3F0bHl4/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

        这里有一个不怎么常用的pandas的to_dict方法。这个方法给大家仔细解读一下。

## 四、to_dict方法

        默认的to_dict方法其实就是一层一层的字典，先是列名，然后是index，然后是value

![](https://img-blog.csdn.net/20181009221310337?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3F0bHl4/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

        我们上面用到的，record设置就变成了每一条记录变成一个字典，最后是一个list。这也就是record的含义了。

![](https://img-blog.csdn.net/20181009221410237?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3F0bHl4/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

        设置为list的话，那么字典中就是columns names最为keys，每一列，也就是每个属性下面的所有值作为一个list成为字典的values。

![](https://img-blog.csdn.net/20181009221459110?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3F0bHl4/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

![](https://img-blog.csdn.net/20181009221606862?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3F0bHl4/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

        还有一个属性是split这个比较有意思。首先是，讽刺啦index，data，columns。其实分别给出了整个dataframe的结构。

![](https://img-blog.csdn.net/20181009221622398?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3F0bHl4/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

![](https://img-blog.csdn.net/2018100922172368?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3F0bHl4/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)



