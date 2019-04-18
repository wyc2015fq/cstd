# 四、数据标准化  Scikit-learn Preprocessing - Nicole的博客 - CSDN博客
2018年06月08日 16:23:59[Nicole_Liang](https://me.csdn.net/weixin_39541558)阅读数：113
# 一、标准化、去均值、方差缩放（variance scaling）
## 1.原理介绍
   通过将属性值按比例缩放，使之落入一个小的特定区间，如0.0~1.0，对属性规范化。有很多数据规范化的方法，这里我们将介绍三种：**最小--最大规范化，z-score规范化，按小数定标规范化和最大绝对值缩放。**
下面简单介绍着几种规范化的原理：
### （1）**最小--最大规范化**
  最小--最大规范化对原始数据进行线性变换。假定minA和maxA分别是属性A的最小值和最大值。最小--最大规范化通过计算：
          v'  = (v - min) /（max - min）* (new_max - new_min)  + new_min 
这样可以将A的值映射到区间[new_min , new_max]中的v'。
### （2）**z-score规范化**
又称为零均值 规范化，将属性A的值基于均值和方差规范化，在不知道属性A的最小值和最大值的时候，或者是离群值左右了最小--最大规范化时，该方法是有用的。
      v' = v - mean / std
### (3)**按小数定标规范化**
通过移动属性A的小数点位置进行规范化，小数点的移动位数依赖于A的最大绝对值，由下式计算：
                                                     v' = v / 10^j
其中j是使得Max（| v' |） <1的最小整数。
例如：假定属性A的取值范围是-986 ~ 917。A的最大绝对值为986.使用小数定标规范化，用1000（即j = 3）除每个数，这样，-986就会被规范化为-0.986，而917也会被规范化成-0.917.
### （4）**最大绝对值缩放**
        它只通过除以每个特征的最大值将训练数据特征缩放至 [-1, 1]。这就意味着，训练数据应该是已经零中
心化或者是稀疏数据。 
## 2.代码实现
### （1）preprocessing.scale:(z-score规范化)
```python
from sklearn import preprocessing
import numpy as np
X = np.array([[ 1., -1.,  2.],
              [ 2.,  0.,  0.],
              [ 0.,  1., -1.]])
X_scaled = preprocessing.scale(X)  #对每一列的值进行标准化
X_scaled                                          
Out[622]: 
array([[ 0.        , -1.22474487,  1.33630621],
       [ 1.22474487,  0.        , -0.26726124],
       [-1.22474487,  1.22474487, -1.06904497]])
X_scaled.mean(axis=0)  #标准化后数据的均值为0.方差为1
Out[623]: array([0., 0., 0.])
X_scaled.std(axis=0)
Out[624]: array([1., 1., 1.])
```
### (2)class:`StandardScaler`
     preprocessing``模块也提供了一个实用类:class:`StandardScaler` ,它使用 ``Transformer 接口在训练集上计算均值和标准差，以便于在后续的测试集上进行相同的缩放.
```python
scaler = preprocessing.StandardScaler().fit(X)
scaler
Out[626]: StandardScaler(copy=True, with_mean=True, with_std=True)
scaler.mean_                                      
Out[627]: array([1.        , 0.        , 0.33333333])
scaler.scale_                                       
Out[628]: array([0.81649658, 0.81649658, 1.24721913])
scaler.transform(X) #计算均值和标准差                           
Out[629]: 
array([[ 0.        , -1.22474487,  1.33630621],
       [ 1.22474487,  0.        , -0.26726124],
       [-1.22474487,  1.22474487, -1.06904497]])
#缩放类对象可以在新的数据上实现和训练集相同缩放操作:
scaler.transform([[-1.,  1., 0.]])
Out[631]: array([[-2.44948974,  1.22474487, -0.26726124]])
```
### 3.特征缩放到特定范围：（最小--最大规范化）preprocessing.MinMaxScaler()
      另外一个可选的缩放操作是将特征缩放至给定的最小、最大值范围，经常是[0,1]。 或者也可以将每个特征的最大绝对值转换至单位大小。这两类操作可以分别通过使用:class:`MinMaxScaler`或者:class:`MaxAbsScaler`实现。
使用这种方法的目的包括：
1、对于方差非常小的属性可以增强其稳定性。
2、维持稀疏矩阵中为0的条目。
```python
X_train = np.array([[ 1., -1.,  2.],
                    [ 2.,  0.,  0.],
                    [ 0.,  1., -1.]])
min_max_scaler = preprocessing.MinMaxScaler()
X_train_minmax = min_max_scaler.fit_transform(X_train)   #使用最小--最大规范化对原始数据进行规范化，默认映射到[0,1]
X_train_minmax
Out[677]: 
array([[0.5       , 0.        , 1.        ],
       [1.        , 0.5       , 0.33333333],
       [0.        , 1.        , 0.        ]])
#同样的转换实例可以被用与在训练过程中不可见的测试数据:实现和训练数据一致的缩放和移位操作:
X_test = np.array([[ -3., -1.,  4.]])
X_test_minmax = min_max_scaler.transform(X_test)
X_test_minmax
Out[681]: array([[-1.5       ,  0.        ,  1.66666667]])
##你也可以通过查看缩放器(scaler)的属性，来观察在训练集中学习到的转换操作的基本性质:
min_max_scaler.scale_                             
Out[683]: array([0.5       , 0.5       , 0.33333333])
min_max_scaler.min_                               
Out[684]: array([0.        , 0.5       , 0.33333333])
#如果:class:MinMaxScaler`被提供了一个精确的``feature_range=(min, max)`，完整的公式是:
X_std = (X - X.min(axis=0)) / (X.max(axis=0) - X.min(axis=0))
X_scaled = X_std * (max - min) + min
###自定义缩放的范围
min_max_scaler = preprocessing.MinMaxScaler(feature_range=(1, 2))
X_train_minmax = min_max_scaler.fit_transform(X_train) 
X_train_minmax
Out[689]: 
array([[1.5       , 1.        , 2.        ],
       [2.        , 1.5       , 1.33333333],
       [1.        , 2.        , 1.        ]])
```
### 4.**最大绝对值缩放**
    MaxAbsScaler 工作原理非常相似,但是它只通过除以每个特征的最大值将训练数据特征缩放至 [-1, 1]。这就意味着，训练数据应该是已经零中心化或者是稀疏数据。 例子::用先前例子的数据实现最大绝对值缩放操作:
```python
X_train = np.array([[ 1., -1.,  2.],
                    [ 2.,  0.,  0.],
                    [ 0.,  1., -1.]])
max_abs_scaler = preprocessing.MaxAbsScaler()
X_train_maxabs = max_abs_scaler.fit_transform(X_train)
X_train_maxabs                # doctest +NORMALIZE_WHITESPACE^
Out[693]: 
array([[ 0.5, -1. ,  1. ],
       [ 1. ,  0. ,  0. ],
       [ 0. ,  1. , -0.5]])
X_test = np.array([[ -3., -1.,  4.]])
X_test_maxabs = max_abs_scaler.transform(X_test)
X_test_maxabs                 
Out[696]: array([[-1.5, -1. ,  2. ]])
max_abs_scaler.scale_  
Out[697]: array([2., 1., 2.])
```
# 二、正则化（Normalization）
     正则化的过程是将每个样本缩放到单位范数（每个样本点的范数为1），如果后面要使用如二次型（点积）或者其他和方法计算两个样本之间的相似性，这个方法会很有用。
    Normalization主要思想是对每个样本计算其P-范数，然后对该样本中每个元素初一该范数，这样处理的结果是使得每个处理后的样本的p-范数（|1 - norm|，|2 - norm|）等于1.
 p-范数的计算公式：||X||p=(|x1|^p+|x2|^p+...+|xn|^p)^1/p
该方法主要应用于文本分类和聚类中。例如，对于两个TF-IDF向量的I2-norm进行点积，就可以得到这两个向量的余弦相似性。
**方法1：使用sklearn.preprocessing.normalize()函数**
```python
X = [[ 1., -1.,  2.],
    [ 2.,  0.,  0.],
    [ 0.,  1., -1.]]
X_normalized = preprocessing.normalize(X, norm='l2')
X_normalized  
Out[700]: 
array([[ 0.40824829, -0.40824829,  0.81649658],
       [ 1.        ,  0.        ,  0.        ],
       [ 0.        ,  0.70710678, -0.70710678]])
```
**方法2：sklearn.preprocessing.StandardScaler类**
```python
normalizer = preprocessing.Normalizer().fit(X)  # fit函数没有任何效果
normalizer
Out[702]: Normalizer(copy=True, norm='l2')
normalizer.transform(X)                            
Out[703]: 
array([[ 0.40824829, -0.40824829,  0.81649658],
       [ 1.        ,  0.        ,  0.        ],
       [ 0.        ,  0.70710678, -0.70710678]])
normalizer.transform([[-1.,  1., 0.]])   
Out[704]: array([[-0.70710678,  0.70710678,  0.        ]])
```
# 三、二值化（Binarization）
    特征的二值化主要是为了将数据特征转变成boolean变量。在sklearn中，sklearn.preprocessing.Binarizer函数可以实现这一功能。实例如下：
```python
X = [[ 1., -1.,  2.],
     [ 2.,  0.,  0.],
     [ 0.,  1., -1.]]
binarizer = preprocessing.Binarizer().fit(X)  # fit does nothing
binarizer
Out[707]: Binarizer(copy=True, threshold=0.0)
binarizer.transform(X)
Out[708]: 
array([[1., 0., 1.],
       [1., 0., 0.],
       [0., 1., 0.]])
##可以改变二值器的阈值:
binarizer = preprocessing.Binarizer(threshold=1.1)
binarizer.transform(X)
Out[710]: 
array([[0., 0., 1.],
       [1., 0., 0.],
       [0., 0., 0.]])
```
# 四、分类特征编码
       在很多时候变量的取值是分类数据，而不是连续的数值特征，比如一个人的性别可以是["male","female"],["from Europe","from  US","from Asia"], ["user Firefox","use Chrome","user Internet Explorer"]这样的编码，例如["male" . "from US", "use Internet Explorer"]可以表示成[0, 1, 3]
      这样的整数特征并不能在scikit - learn中直接使用，因为这样连续的输入，solve会认为类别之间是有序的，但实际上是无序的。（就好像浏览器的类别数据是无序的）
    可以将类别特征转换成scikit - learn估计其可以使用的特征的方法有one - of - k或者one - hot编码，该方法是:class:OneHotEncoder`的一个实现。该方法将每个类别特征的 ``m` 可能值转换成``m``个二进制特征值，当然只有一个是激活值。
```python
enc = preprocessing.OneHotEncoder()
enc.fit([[0, 0, 3], [1, 1, 0], [0, 2, 1], [1, 0, 2]])  
Out[712]: 
OneHotEncoder(categorical_features='all', dtype=<class 'numpy.float64'>,
       handle_unknown='error', n_values='auto', sparse=True)
enc.transform([[0, 1, 3]]).toarray()
Out[713]: array([[1., 0., 0., 1., 0., 0., 0., 0., 1.]])
```
        默认情况下，每个特征使用几维的数值由数据集自动推断。当然，你也可以通过使用参数``n_values``来精确指定。 在我们的例 子数据集中，有两个可能得性别类别，三个洲，四个网络浏览器。接着，我们训练编码算法，并用来对一个样本数据进行转换。 在结果中，前两个数值是性别编码，紧接着的三个数值是洲编码，最后的四个数值是浏览器编码。
