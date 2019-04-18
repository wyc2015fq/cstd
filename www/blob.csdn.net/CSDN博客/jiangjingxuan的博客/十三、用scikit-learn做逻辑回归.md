# 十三、用scikit-learn做逻辑回归 - jiangjingxuan的博客 - CSDN博客





2017年01月25日 10:24:53[jiangjingxuan](https://me.csdn.net/jiangjingxuan)阅读数：483











![](http://www.shareditor.com/uploads/media/default/0001/01/thumb_136_default_big.jpeg)



一元线性、多元线性、多项式回归都属于广义的线性回归，这几类线性回归主要用于预测连续变量的值。本节介绍广义线性回归的另一种主要用于分类任务的形式：逻辑回归

请尊重原创，转载请注明来源网站[www.shareditor.com](http://www.shareditor.com)以及原始链接地址

## 二类分类问题

逻辑回归最广泛的应用就是二类分类，我们以脏话判别为例来利用逻辑回归，对一句话做脏话分析判断

输入样本如下：

是脏话：fuck you

是脏话：fuck you all

不是脏话：hello everyone



我们来预测以下两句话是否是脏话：

```
fuck me
hello boy
```



```python
# coding:utf-8

import sys
reload(sys)
sys.setdefaultencoding( "utf-8" )

from sklearn.feature_extraction.text import TfidfVectorizer
from sklearn.linear_model.logistic import LogisticRegression

X = []

# 前三行作为输入样本
X.append("fuck you")
X.append("fuck you all")
X.append("hello everyone")

# 后两句作为测试样本
X.append("fuck me")
X.append("hello boy")

# y为样本标注
y = [1,1,0]

vectorizer = TfidfVectorizer()

# 取X的前三句作为输入做tfidf转换
X_train = vectorizer.fit_transform(X[:-2])

# 取X的后两句用上句生成的tfidf做转换
X_test = vectorizer.transform(X[-2:])

# 用逻辑回归模型做训练
classifier = LogisticRegression()
classifier.fit(X_train, y)

# 做测试样例的预测
predictions = classifier.predict(X_test)
print predictions
```



输出结果如下：
`[1 0]`
判断成：

是脏话：fuck me

不是脏话：hello boy



