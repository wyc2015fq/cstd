# sklearn  处理文本和分类属性[LabelBinarizer, LabelEncoder,OneHotEncoder] - 墨流觞的博客 - CSDN博客





2018年09月23日 11:06:27[墨氲](https://me.csdn.net/dss_dssssd)阅读数：219








### sklearn 利用LabelBinarizer, LabelEncoder,OneHotEncoder来处理文本和分类属性

对于分类和文本属性，需要将其转换为离散的数值特征才能喂给机器学习算法，常用的是转化为 one-hot编码格式。

```python
```python
df = pd.DataFrame({'ocean_proximity':["<1H OCEAN","<1H OCEAN","NEAR OCEAN","INLAND", "<1H OCEAN", "INLAND"],
                  'population': [339.0, 113.0, 462.0, 353.9,1463.9, 570.0]})
```
```

#### 1. LabelEncoder

将文本属性转化为离散的数值属性

```python
```python
from sklearn.preprocessing import LabelEncoder
encoder = LabelEncoder()
df_cat = df["ocean_proximity"]
df_cat_encoder = encoder.fit_transform(housing_cat)
df_cat_encoder
```
```

out:  返回的是一个numpy的数组

> 
array([0, 0, 2, 1, 0, 1], dtype=int64)


可以使用`encoder.classes_`来查看类别

```python
```python
encoder.classes_
```
```

out:

> 
array([’<1H OCEAN’, ‘INLAND’, ‘NEAR OCEAN’], dtype=object)


**注意：**

机器学习算法通常认为两个相差较小的值要比两个相差较大的值更相似，但是 0和2（<1H OCEAN 和 NEAR OCEAN ）要比0和1（<1H OCEAN 和 INLAND）更相似，为解决这个问题，采用one_hot编码. one_hot编码任何两值之间的距离相等。

#### OneHotEncoder

将LabelEncoder得到的值转化为one_hot编码格式

```python
```python
from sklearn.preprocessing import OneHotEncoder
encoder = OneHotEncoder()
df_cat_1hot = encoder.fit_transform(df_cat_encoder.reshape(-1, 1))
df_cat_1hot
```
```

out:

> 
<6x3 sparse matrix of type ‘<class ‘numpy.float64’>’

with 6 stored elements in Compressed Sparse Row format>


返回的是一个scipy的稀疏矩阵，可以占据更少的内存

```python
```python
# toarray()转换为numpy的数组
df_cat_1hot. toarray()
```
```

out:

> 
array([[1., 0., 0.],

[1., 0., 0.],

[0., 0., 1.],

[0., 1., 0.],

[1., 0., 0.],

[0., 1., 0.]])
**注意:**  one_hot中fit_transform讲解
`fit_transform(X, y=None)`

参数：X ：shape [n_samples, n_feature]- n_samples: 样本数量
- n_features: 特征值数量

按列处理数据，下面一个例子说明一下：

```python
```python
df = pd.DataFrame([[0, 0, 3], [1, 1, 0], [0, 2, 1],[1, 0, 2]])
df
# out:
   0	1	2
0	0	0	3
1	1	1	0
2	0	2	1
3	1	0	2
df_encodered = encoder.fit_transform(df)
df_encodered.toarray()

# out : 
array([[1., 0., 1., 0., 0., 0., 0., 0., 1.],
       [0., 1., 0., 1., 0., 1., 0., 0., 0.],
       [1., 0., 0., 0., 1., 0., 1., 0., 0.],
       [0., 1., 1., 0., 0., 0., 0., 1., 0.]])
```
```

**分析一下：**

df有4个样本，3列。

第一列为[0,1]有两类，所以转换为one_hot时，为10 和 01，

第一列为[0,1,2]有两类，所以转换为one_hot时，为10 0和 010, 001

第一列为[0,1,2,3]有两类，所以转换为one_hot时，为10 00和 0100, 0010,0001
可以看出输出df_encodered的前两列为df第1列的one_hot编码，第3，4，5列为df第2列的one_hot编码，后4列为df第3列的one_hot编码

#### LabelBinarizer

LabelBinarizer一步到位，直接完成上述两部操作哦

```python
```python
from sklearn.preprocessing import LabelBinarizer
encoder = LabelBinarizer()
df_cat_1hot = encoder.fit_transform(df_cat)
df_cat_1hot

# out:
array([[1, 0, 0],
       [1, 0, 0],
       [0, 0, 1],
       [0, 1, 0],
       [1, 0, 0],
       [0, 1, 0]])
```
```

默认输出为numpy的array，在LabelBinarizer中传入sparse_output=True,可以返回稀疏矩阵

```python
```python
from sklearn.preprocessing import LabelBinarizer
encoder = LabelBinarizer(sparse_output=True)
df_cat_1hot = encoder.fit_transform(df_cat)
df_cat_1hot
```
```

out:

> 
<6x3 sparse matrix of type ‘<class ‘numpy.int32’>’

with 6 stored elements in Compressed Sparse Row format>








