# 独热编码(One-Hot Encoding) - YZXnuaa的博客 - CSDN博客
2018年04月16日 14:16:36[YZXnuaa](https://me.csdn.net/YZXnuaa)阅读数：181
## 【概念】
One-hot encoding是只存在一个1其余全为0的n位序列。也可以称它为二元向量，二元就是里面只有0和1.通常被用来描述一个状态机的某个状态。
## 【用处】
用于处理离散型特征。通过将离散特征通过one-hot编码映射到欧式空间，可以进一步计算特征之间的距离与相似度，然后在回归，分类，聚类等机器学习算法中使用。至于什么是离散型数据，欧式空间，特征距离和特征相似度，请同学自行学习，这里不做展开。
## 【应用】
在机器学习中，我们需要将用来训练的数据样本集合分类，转换成程序能够理解的向量。
比如我手里有三个图像样本，要识别出图片里的内容，其中包含三个维度，分别是 
 {  
    ‘sex’:[“male”, “female”], 
    ‘hair’:[‘short’,’long’,’bare’], 
    ‘hair color’:[‘black’,’gold’,’blue’] 
}
样本集合如下：
|id|sex|hair|hair color|
|----|----|----|----|
|1|male|short|black,gold|
|2|female|long|black,blue|
|3|male|short|gold|
|4|male|bare|black|
注意：hair color这栏存在多个值的原因在于，这个人可能染了一部分头发。其他维度也同理，没准图片中有几个人，好多美女。
## 【代码】
下面是[scikit-learn docs](http://scikit-learn.org/stable/modules/generated/sklearn.preprocessing.OneHotEncoder.html)提供的代码示例：
```
>>> from sklearn.preprocessing import OneHotEncoder
>>> enc = OneHotEncoder()
# 这里有四个样本，三个维度
>>> enc.fit([[0, 0, 3], [1, 1, 0], [0, 2, 1], [1, 0, 2]])  
OneHotEncoder(categorical_features='all', dtype=<... 'numpy.float64'>,
       handle_unknown='error', n_values='auto', sparse=True)
#fit函数算了下3个维度各自的值个数，第一列是[0,1,0,1]只有两个值，第二列是[0,1,2,0]故有三个值，第三列同上。
>>> enc.n_values_
array([2, 3, 4])
# 待理解，大致是0，2， 2+3， 2+3+4
>>> enc.feature_indices_
array([0, 2, 5, 9])
#转换序列，0 -> [1，0],为啥0不是[0,1]，这是函数分的，应该是有优化，不用在意。
#        1 -> [0,1,0]
#        1 -> [0,1,0,0]
>>> enc.transform([[0, 1, 1]]).toarray()
array([[ 1.,  0.,  0.,  1.,  0.,  0.,  1.,  0.,  0.]])
```
