# 八、用scikit-learn做特征提取 - jiangjingxuan的博客 - CSDN博客





2017年01月25日 10:21:24[jiangjingxuan](https://me.csdn.net/jiangjingxuan)阅读数：534












![](http://www.shareditor.com/uploads/media/default/0001/01/thumb_136_default_big.jpeg)



现实世界中多数特征都不是连续变量，比如分类、文字、图像等，为了对非连续变量做特征表述，需要对这些特征做数学化表述，因此就用到了特征提取

请尊重原创，转载请注明来源网站[www.shareditor.com](http://www.shareditor.com)以及原始链接地址

## 分类变量的特征提取

比如城市作为一个特征，那么就是一系列散列的城市标记，这类特征我们用二进制编码来表示，是这个城市为1，不是这个城市为0

比如有三个城市：北京、天津、上海，我们用scikit-learn的DictVector做特征提取，如下：

```python
# coding:utf-8

import sys
reload(sys)
sys.setdefaultencoding( "utf-8" )

from sklearn.feature_extraction import DictVectorizer
onehot_encoder = DictVectorizer()
instances = [{'city': '北京'},{'city': '天津'}, {'city': '上海'}]
print(onehot_encoder.fit_transform(instances).toarray())
```

执行后输出：

```
[[ 0.  1.  0.]
 [ 0.  0.  1.]
 [ 1.  0.  0.]]
```

不同编码表示不同城市



## 文字特征提取

文字特征无非这几种：有这个词还是没有、这个词的TF-IDF

第一种情况用词库表示法，如下：

```python
# coding:utf-8

import sys
reload(sys)
sys.setdefaultencoding( "utf-8" )

from sklearn.feature_extraction.text import CountVectorizer
corpus = [
        'UNC played Duke in basketball',
        'Duke lost the basketball game' ]
vectorizer = CountVectorizer()
print vectorizer.fit_transform(corpus).todense()
print vectorizer.vocabulary_
```

执行后输出如下：

```python
[[1 1 0 1 0 1 0 1]
 [1 1 1 0 1 0 1 0]]
{u'duke': 1, u'basketball': 0, u'lost': 4, u'played': 5, u'game': 2, u'unc': 7, u'in': 3, u'the': 6}
```

数值为1表示词表中的这个词出现，为0表示未出现

词表中的数值表示单词的坐标位置

请尊重原创，转载请注明来源网站[www.shareditor.com](http://www.shareditor.com)以及原始链接地址

第二种情况TF-IDF表示词的重要性，如下：

```python
# coding:utf-8

import sys
reload(sys)
sys.setdefaultencoding( "utf-8" )

from sklearn.feature_extraction.text import TfidfVectorizer
corpus = [
        'The dog ate a sandwich and I ate a sandwich',
        'The wizard transfigured a sandwich' ]
vectorizer = TfidfVectorizer(stop_words='english')
print(vectorizer.fit_transform(corpus).todense())
print(vectorizer.vocabulary_)
```

执行后结果：

```python
[[ 0.75458397  0.37729199  0.53689271  0.          0.        ]
 [ 0.          0.          0.44943642  0.6316672   0.6316672 ]]
{u'sandwich': 2, u'wizard': 4, u'dog': 1, u'transfigured': 3, u'ate': 0}
```

值最高的是第一个句子中的ate，因为它在这一个句子里出现了两次

值最低的自然是本句子未出现的单词



## 数据标准化

数据标准化就是把数据转成均值为0，是单位方差的。比如对如下矩阵做标准化：

```python
# coding:utf-8

import sys
reload(sys)
sys.setdefaultencoding( "utf-8" )

from sklearn import preprocessing
import numpy as np
X = np.array([
    [0., 0., 5., 13., 9., 1.],
    [0., 0., 13., 15., 10., 15.],
    [0., 3., 15., 2., 0., 11.]
    ])
print(preprocessing.scale(X))
```



执行结果：

```
[[ 0.         -0.70710678 -1.38873015  0.52489066  0.59299945 -1.35873244]
 [ 0.         -0.70710678  0.46291005  0.87481777  0.81537425  1.01904933]
 [ 0.          1.41421356  0.9258201  -1.39970842 -1.4083737   0.33968311]]
```






