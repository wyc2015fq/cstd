# python︱numpy、array——高级matrix（替换、重复、格式转换、切片） - 素质云笔记/Recorder... - CSDN博客





2016年08月23日 15:46:21[悟乙己](https://me.csdn.net/sinat_26917383)阅读数：36927








先学了R，最近刚刚上手python,所以想着将python和R结合起来互相对比来更好理解python。最好就是一句python，对应写一句R。

python中的numpy模块相当于R中的matirx矩阵格式，化为矩阵，很多内容就有矩阵的属性，可以方便计算。

以下符号：

**=R=**

代表着在R中代码是怎么样的。



   array模块定义了一种序列数据结构，看起来和list很相似，但是所有成员必须是相同基本类型。

   array-固定类型数据序列array作用是高效管理固定类型数值数据的序列。

   笔者在使用的时候，觉得array十分特别，笔者这样的新手，感觉有点驾驭不了。

————————————————————————————————————



速查手册：（来源：[NumPy 的 Array 介紹](https://blog.aweimeow.tw/2016/10/23/NumPy-%E7%9A%84-Array-%E4%BB%8B%E7%B4%B9/)）
`np.set_printoptions(suppress=True) # 不用科学计数法`


```python
# 属性
ndarray.shape: 多維陣列的大小（形狀）
ndarray.ndim: 多維陣列的維度
ndarray.itemsize: 陣列當中元素的大小（佔幾個 byte）
ndarray.nbytes: 整個陣列所有元素的大小總計
ndarray.T: 轉置矩陣，只能在維度 <= 2 的時候使用，與 self.transpose() 效果相同
ndarray.flat: 把陣列扁平化輸出

# 格式转换
ndarray.item: 類似 List 的 Index，把 Array 扁平化取得某 Index 的 value
ndarray.tolist: 把 NumPy.ndarray 輸出成 Python 原生 List 型態
ndarray.itemset: 把 ndarray 中的某個值（純量）改掉

# 维度操作
ndarray.reshape(shape): 把同樣的資料以不同的 shape 輸出（array 的 total size 要相同）
ndarray.resize(shape): 重新定義陣列的大小
ndarray.flatten(): 把多維陣列收合成一維陣列（扁平化＆Copy）
ndarray.ravel(): 回傳扁平化的陣列（無 Copy）

# 项目选择与操作
ndarray.take(indices): 根據輸入索引值來得到指定陣列
ndarray.put(indices, values): 根據索引值改變陣列 value
ndarray.repeat(times): 重複陣列的值（類似擴張）
ndarray.sort(): 把陣列當中的元素排序
ndarray.sum(): 加總多維陣列（可指定加總的維度根據）
```





```python
# 实用模块
np.squeeze(array)   # 去掉array的第一列
np.maximin(x,0,y)   # 比较两个值大小，若有小于0的，则为0
```















————————————————————————————————————

# 一、数据生成与复制、重复

## 1、数列生成

**构造单一数列**



```
arange(10)  =R=1:10   生成一个连贯的数列
arange(3,7)  =R=3:7
arange(3,10,2)  =R=  seq(3, 9, by = 2)
array(arange(4))  =R= matrix(1:4)
```



**生成的过程：**


`np.array([1,2])`
   需要np.，笔者在写的时候，常常用R的思维去写... 

出错： array(1,2)  array([1,2])  np.array([1,2],[1,2])



**类似cut分组**




`np.linspace(2.0, 3.0, num=5)   =R= cut(2:3,5)  #类似cut功能，在2,3之间分成5份`






**matrix矩阵组**


`ma=arange(10).reshape(5,2) #matrix(rep(1:10),nrow=5,ncol=2) 按行或列生成一定规则的 ones((2,3), dtype=int) =R= matrix(rep(1,6),2,3) #矩阵内元素都为1 random.random((2,3)) =R= matrix(runif(6),2,3) #生成随机数 ``构造空白数组：`
```
ones创建全1矩阵 
	zeros创建全0矩阵 
	eye创建单位矩阵 
	empty创建空矩阵（实际有值）
```
`import numpy as np a_ones = np.ones((3,4)) # 创建3*4的全1矩阵 print(a_ones) # 结果 [[ 1. 1. 1. 1.] [ 1. 1. 1. 1.] [ 1. 1. 1. 1.]] a_zeros = np.zeros((3,4)) # 创建3*4的全0矩阵 print(a_zeros) # 结果 [[ 0. 0. 0. 0.] [ 0. 0. 0. 0.] [ 0. 0. 0. 0.]] a_eye = np.eye(3) # 创建3阶单位矩阵 print(a_eye) # 结果 [ 1. 0. 0.] [ 0. 1. 0.] [ 0. 0. 1.]] a_empty = np.empty((3,4)) # 创建3*4的空矩阵 print(a_empty) # 结果 [[ 1.78006111e-306 -3.13259416e-294 4.71524461e-309 1.94927842e+289] [ 2.10230387e-309 5.42870216e+294 6.73606381e-310 3.82265219e-297] [ 6.24242356e-309 1.07034394e-296 2.12687797e+183 6.88703165e-315]]`



`等差、等比数列`




```
linspace()和matlab的linspace很类似，用于创建指定数量等间隔的序列，实际生成一个等差数列。
```
`import numpy as np a = np.linspace(0,10,7) # 生成首位是0，末位是10，含7个数的等差数列 print(a) # 结果 [ 0. 1.66666667 3.33333333 5. 6.66666667 8.33333333 10. ]`




linspace用于生成等差数列，而logspace用于生成等比数列。 

下面的例子用于生成首位是100，末位是102，含5个数的等比数列。






`import numpy as np a = np.logspace(0,2,5) print(a) # 结果 [ 1. 3.16227766 10. 31.6227766 100. ]`








## 2、复制/重复repeat、title



貌似是list/tuple/dict唯一一个拥有重复属性的吧？ 两个重复函数：repeat/tile



**repeat函数功能：对数组中的元素进行连续重复复制**

用法有两种：

1) numpy.repeat(a, repeats, axis=None)

2) a.repeats(repeats, axis=None)

```python
>>>import numpy as np

>>> a = np.arange(10)  

>>> a  

array([0, 1, 2, 3, 4, 5, 6, 7, 8, 9])  

>>> a.repeat(5)  

array([0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 4, 4, 4,  

        4, 4, 5, 5, 5, 5, 5, 6, 6, 6, 6, 6, 7, 7, 7, 7, 7, 8, 8, 8, 8, 8, 9,  

        9, 9, 9, 9])  

>>> a

np.array([0, 1, 2, 3, 4, 5, 6, 7, 8, 9])  #a数组的内容没改变
```





**tile函数功能：对整个数组进行复制拼接**

用法：numpy.tile(a, reps)

其中a为数组，reps为重复的次数



```python
>>> np.tile(a,2)  
array([0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9])  
>>> a
np.array([0, 1, 2, 3, 4, 5, 6, 7, 8, 9])  
 
>>> a=np.array([10,20])
>>>a
array([10,20])
>>>np.tile(a, (3,2)) #构造3*2个copy
array([[10, 20, 10, 20],  
          [10, 20, 10, 20],  
          [10, 20, 10, 20]])
```





——————————————————————————————————————————————————



# 二、array属性与统计、运算

# 1、矩阵属性


`ma.shape #输出5,2（矩阵横纵维度） ma.ndim =R=dim(ma) #维度,dim(data) ma.size #元素总数，5*2`







`In [26]: arr3.dtype #dtype方法返回数组的数据类型 Out[26]: dtype('int32')``缺失值填补：``example = np.where(np.isnan(example), 0, example) example = np.where(np.isnan(example), 0, example)`






## 2、数组拉直







拉直的办法有两个：arr3.ravel()  和  arr3.flatten()

**第一种办法：**


`a = arr3.ravel()    #通过ravel的方法将数组拉直（多维数组降为一维数组）`


```
In [24]: arr3
Out[24]:
array([[  1,   1,   2,   3],
[  5,   8,  13,  21],
[ 34,  55,  89, 144]])
```


**第二种办法：**





```
In [29]: b = arr3.flatten()  #通过flatten的方法将数组拉直
In [30]: b
Out[30]: array([  1,   1,   2,   3,   5,   8,  13,  21,  34,  55,  89, 144])
```







两者的区别在于**ravel方法**生成的是原数组的**视图**，无需占有内存空间，**但视图的改变会影响到原数组的变化**。而**flatten方法**返回的是**真实值**，其值的改变并不会影响原数组的更改。

通过下面的例子也许就能明白了：

```
In [31]: b[:3] = 0
In [32]: arr3
Out[32]:
array([[  1,   1,   2,   3],
[  5,   8,  13,  21],
[ 34,  55,  89, 144]])
```


通过更改b的值，原数组没有变化。









# 3、矩阵运算——相乘、求积


`(arange(4).reshape(2,2))* (arange(8).reshape(2,2)) #组内数字相乘 dot( (arange(4).reshape(2,2)),(array([[1,2],[3,5]]))) #矩阵相乘 ma*=2 #ma=ma*2,会原地的改变数组而不是创建一个新的数组 `
所有元素之积：


`prod() `
得到数组所有元素之积，是个数字。也可以aaa.sum(axis)，分别求每一行或者是每一列的元素之积

累计积：cumprod() 



all() ：如果所有元素为真，返回真；否则返回假 

特征值 ：linalg.eigvals() 

返回A的特征值 

# 4、矩阵运用函数——加总、求均值、累积和、极大值、极小值

```
ma.sum()
ma.min()
ma.max()
ma.sum(axis=0)  =R=apply(b,1,sum)  =R=colSums（data）      #axis=0代表纵向,列；axis=1，代表横向
ma.cumsum(axis=1)                       #按行，累计加总的结果
```
`（1）求和：`
ma.sum/min代表所有元素加总.

其中，如果是矩阵连加，有两种方式：array+array=矩阵，array.sum()=数值
`	第一种就是mat + mat,用加号，生成的还是矩阵，高纬度；``	第二种就是sum(mat)，用sum，一维单个数值.``同时注意，跟ma.sum()不一样，.sum()返回的是一个矩阵总和。`
场景一：矩阵相加-均值


`data_array + data_array - data_array.mean()`


场景二：矩阵小于某阈值的，赋值




`data.array[data.array< 0 ] = 0`




矩阵小于0的，都设置为0







场景三：矩阵变一维向量:ravel()


`data.array.ravel()`


**（2）求平均：**

获得矩阵中元素的平均值可以通过函数`mean()`。同样地，可以获得整个矩阵、行或列的平均值。



```
import numpy as np

a = np.array([[1,2,3],[4,5,6]])
print(a.mean()) #结果为： 3.5

# 同样地，可以通过关键字axis参数指定沿哪个方向获取平均值
print(a.mean(axis=0)) # 结果 [ 2.5  3.5  4.5]
print(a.mean(axis=1)) # 结果 [ 2.  5.]
```



**（3）进行正弦计算：**



```
>>> y = np.sin(x)
 >>> y
 array([  0.00000000e+00,   6.42787610e-01,   9.84807753e-01,
          8.66025404e-01,   3.42020143e-01,  -3.42020143e-01,
         -8.66025404e-01,  -9.84807753e-01,  -6.42787610e-01,
         -2.44921271e-16])
```




|矩阵函数|说明|
|----|----|
|np.sin(a)|对矩阵a中每个元素取正弦,sin(x)|
|np.cos(a)|对矩阵a中每个元素取余弦,cos(x)|
|np.tan(a)|对矩阵a中每个元素取正切,tan(x)|
|np.arcsin(a)|对矩阵a中每个元素取反正弦,arcsin(x)|
|np.arccos(a)|对矩阵a中每个元素取反余弦,arccos(x)|
|np.arctan(a)|对矩阵a中每个元素取反正切,arctan(x)|
|np.exp(a)|对矩阵a中每个元素取指数函数,ex|
|np.sqrt(a)|对矩阵a中每个元素开根号√x|





**（4）累计和：**











```
aaa.cumsum()
2 结果为：array([ 10,  19,  27,  34,  40,  45,  87, 120, 122])
```





**（5）求最大值，最小值：**

获得矩阵中元素最大最小值的函数分别是`max`和`min`，可以获得整个矩阵、行或列的最大最小值。 

例如



```
import numpy as np

a = np.array([[1,2,3],[4,5,6]])
print(a.max()) #获取整个矩阵的最大值 结果： 6
print(a.min()) #结果：1

# 可以指定关键字参数axis来获得行最大（小）值或列最大（小）值
# axis=0 行方向最大（小）值，即获得每列的最大（小）值
# axis=1 列方向最大（小）值，即获得每行的最大（小）值
# 例如

print(a.max(axis=0))
# 结果为 [4 5 6]

print(a.max(axis=1))
# 结果为 [3 6]

# 要想获得最大最小值元素所在的位置，可以通过argmax函数来获得
print(a.argmax(axis=1))
# 结果为 [2 2]
```



（6）求方差与标准差

方差：比较简单，分别是np.sum(), np.mean(), np.var(), np.std()(这个是标准差)，关键是在加入axis参数以后要很好的区分



```python
>>> a
array([[6, 7, 1, 6],
       [1, 0, 2, 3],
       [7, 8, 2, 1]])
方差：
>>> np.var(a)
7.7222222222222223
>>> np.var(a,axis=0)
array([  6.88888889,  12.66666667,   0.22222222,   4.22222222])
>>> np.std(a,axis=0)
array([ 2.62466929,  3.55902608,  0.47140452,  2.05480467])
```



## 5、偏度、峰度



参考：[Python统计学一数据的概括性度量](http://www.itdadao.com/articles/c15a677831p0.html)、[《Python数据分析基础教程：Numpy学习指南》- 速记 - 第十章](http://www.voidcn.com/blog/silentwater/article/p-5760730.html)



在scipy模块中

**偏度（skewness）**、描述的是概率分布的偏斜（非对称）程度。偏度检验有两个返回值，其中第二个返回值为p-value，即观察到的数据集服从正态分布的概率，取值范围为0~1

**峰度（kurtosis）**描述的是概率分布曲线的陡峭程度。


- **偏态系数**：偏度(Skewness)亦称偏态、偏态系数，偏度是统计数据分布偏斜方向和程度的度量，是统计数据分布非对称程度的数字特征。Sk>0时，分布呈正偏态（右偏），Sk<0时，分布呈负偏态（左偏）。
- **峰态系数**：（Kurtosis)峰度系数是用来反映频数分布曲线顶端尖峭或扁平程度的指标。在正态分布情况下，峰度系数值是3。>3的峰度系数说明观察量更集中，有比正态分布更短的尾部；<3的峰度系数说明观测量不那么集中，有比正态分布更长的尾部，类似于矩形的均匀分布。峰度系数的标准误用来判断分布的正态性。峰度系数与其标准误的比值用来检验正态性。如果该比值绝对值大于2，将拒绝正态性。





```python
from scipy import stats
import matplotlib.pyplot as plt

generated = stats.norm.rvs(size=900)  #使用scipy.stats包按正态分布生成随机数。
print "Mean", "Std", stats.norm.fit(generated)  #用正态分布去拟合生成的数据，得到其均值和标准差。

#偏度（skewness）描述的是概率分布的偏斜（非对称）程度。偏度检验有两个返回值，其中第二个返回值为p-value，即观察到的数据集服从正态分布的概率，取值范围为0~1。
print "Skewtest", "pvalue", stats.skewtest(generated) 

#output
#Skewtest pvalue (-0.62120640688766893, 0.5344638245033837) 
#该数据集有53%的概率服从正态分布。

#峰度（kurtosis）描述的是概率分布曲线的陡峭程度。
print "Kurtosistest","pvalue",stats.kurtosistest(generated)

#正态性检验（normality test）可以检查数据集服从正态分布的程度。我们来做一个正态性检验。该检验同样有两个返回值，其中第二个返回值为p-value。
print "Normaltest", "pvalue", stats.normaltest(generated)

#得到95%处的数值如下
print "95 percentile", stats.scoreatpercentile(generated, 95)

#将前一步反过来，可以从数值1出发找到对应的百分比
print "Percentile at 1", stats.percentileofscore(generated, 1)
```



### 6、numpy的除法

很多情况会遇到，1/2=0的情况，所以需要了解一下。

精确除法

除法总是会返回真实的商，不管操作数是整形还是浮点型。执行from __future__ import division 指令就可以做到这一点。



```python
>>>from __future__ import division  
>>>1/2  
0.5
```

```python
>>>1//2 0 >>>1.0//2 0 >>>-1//2.0 -1
```



>>>1.0/2.0 0.5

地板除













从Python2.2开始，增加了一个操作符 // ，以执行地板除：//除法不管操作数为何种数值类型，总是会舍去小数部分，返回数字序列中比真正的商小的最接近的数字。



```python
>>>1//2  
0  
>>>1.0//2  
0  
>>>-1//2.0  
-1
```







传统除法

如果是整数除法则执行地板除，如果是浮点数除法则执行精确除法。

```python
>>>1/2  
0  
>>>1.0/2.0  
0.5
```



7、白化（Whitening）

来源于：[训练深度神经网络的必知技巧，你知道哪些？](https://mp.weixin.qq.com/s?__biz=MjM5ODU3OTIyOA==&mid=2650665248&idx=2&sn=125160098192ff8f7a9b870773664c0b&chksm=bec1cc5389b645453d3bfa08bd4932364455645cfdacfcb3acc2dd003f03d2495d5493639118&mpshare=1&scene=1&srcid=0626Zg0jh0pwSaeEG6ZuDIf9#rd)

白化相当于在零均值化和归一化操作之间插入一个旋转操作，将数据投影到主轴上。一张图片经过白化后，可以认为每个像素之间是统计独立的。然而白化很少在卷积神经网络中使用，可能原因是图像信息本来就是依靠像素之间的相对差异来体现的，白化让像素间去相关，让这种差异变得不确定，损失了信息。

将数据零均值化，再计算协方差矩阵（convariance matrix）来观察数据中的相关结构。



```python
X-=np.mean(X,axis=0)

cov=np.dot(X.T,X)/X.shape[0] #计算协方差矩阵
```


然后做去相关操作, 即通过将原始数据（零均值化后的数据）投影到特征基空间（eigenbasis）。





```python
U,S,V=np.linalg.svd(cov) #计算数据协方差矩阵的奇异值分解（SVDfactorization）

Xrot=np.dot(X,U) #对数据去相关
```





最后一步变换是白化，即把特征基空间的数据除以每个维度的特征值来标准化尺度。



```python
Xwhite=Xrot/np.sqrt(S+1e-5) #除以奇异值的平方根，注意到这里加了个 1e-5 是为了防止分母是 0 的情况。
```

PCA 白化的一个缺点是会增加数据中的噪声，因为它把输入数据的所有维度都延伸到相同的大小，这些维度中就包含噪音维度（往往表现为不相关的且方差较小）。这种缺点在实际操作中可以通过把 1e-5 增大到一个更大的值来引入更强的平滑。













——————————————————————————————————————————







# 三、numpy如何导出以及导入、数列格式转换

## 1、numpy如何导出、导入

参考：[Python Numpy数组保存](http://www.cnblogs.com/ice-daigua/archive/2012/11/16/2772674.html)



  Numpy提供了几种数据保存的方法。

   以3*4数组a为例：



 1. a.tofile("filename.bin")

      这种方法只能保存为二进制文件，且不能保存当前数据的行列信息，文件后缀不一定非要是bin，也可以为txt，但不影响保存格式，都是二进制。

      这种保存方法对数据读取有要求，需要手动指定读出来的数据的的dtype，如果指定的格式与保存时的不一致，则读出来的就是错误的数据。

       b = numpy.fromfile("filename.bin",dtype = **)

       读出来的数据是一维数组，需要利用

        b.shape = 3,4重新指定维数。



 2.numpy.save("filename.npy",a)

       利用这种方法，保存文件的后缀名字一定会被置为.npy，这种格式最好只用

       numpy.load("filename")来读取。



   3.numpy.savetxt("filename.txt",a)

      b =  numpy.loadtxt("filename.txt")

     用于处理一维和二维数组



## 2、数组格式转换







数组转换：**tolist将数组转换为列表，astype()强制转换数组的数据类型**，下面是两个函数的例子：



```
In [53]: b = a.tolist()
In [54]: b
Out[54]:
[[0, 1, 2, 3],
[4, 5, 6, 7],
[8, 9, 10, 11],
[12, 13, 14, 15],
[16, 17, 18, 19],
[20, 21, 22, 23]]
In [55]: type(b)
Out[55]: list

In [56]: c = a.astype(float)
In [57]: c
Out[57]:
array([[  0.,   1.,   2.,   3.],
[  4.,   5.,   6.,   7.],
[  8.,   9.,  10.,  11.],
[ 12.,  13.,  14.,  15.],
[ 16.,  17.,  18.,  19.],
[ 20.,  21.,  22.,  23.]])

In [58]: a.dtype
Out[58]: dtype('int32')
In [59]: c.dtype
Out[59]: dtype('float64')
```



这里有一个非常实际的问题：



一般结果输出都是array格式，然后我要加一个字符串形式进行，总不能.append的加，所以需要把array转化格式。譬如有一个名称为a的array格式数据。


`['a1.jpg',]  +  a.tolist()`
其中，[]中间有一个"，"，这个很有意思，如果你不加就是单纯的字符格式，需要加一个逗号，才能识别为[]  







**其他格式转化：**



1. list转化为numpy.ndarray：

np.array(example)

2. numpy.ndarray转化为list：

list(example)

3. numpy.ndarray转化为dataframe:

pd.DataFrame(example)

4. dataframe转化为numpy.ndarray：

example.values[:, :]





————————————————————————————————————————————————————



#   四、array**添加数据、切片、合并**



**1、array添加数据**



```
a=[]

#append
a.append([1,2])

#insert
a.insert(2,1)
a.insert(2,[1,2])
```



   append加在后面，insert(位置，内容)可以加在指定位置。这边笔者又要吐槽自己了...以为又在使用R，**如果a是array格式的，append是不可以使用的。只有a=[]元组的时候，才可以append加进去。**



**   注意append用法：其中append用在list之中，在DataFrame/array无法使用**





**2、切片过程：**



```
>>>Array[0:]　　——>切片从前面序号“0”开始到结尾，包括“0”位
　　[2, 3, 9, 1, 4, 7, 6, 8]

　　>>>Array[:-1]　　——>切片从后面序号“-1”到最前，不包括“-1”位
　　[2, 3, 9, 1, 4, 7, 6]

　　>>>Array[3:-2]　　——>切从前面序号“3”开始(包括)到从后面序号“-2”结束(不包括)
　　[1, 4, 7]

　　>>>Array[3::2]　　——>从前面序号“3”(包括)到最后，其中分隔为“2”
　　[1, 7, 8]
```



## 3、numpy对象纵向合并



用numpy中的concatenation函数进行合并。

![](https://img-blog.csdn.net/20170223112234067)





## 4、用逻辑符bool定位出numpy中的内容



```python
vector = numpy.array([5, 10, 15, 20])
print(vector)
[ 5 10 15 20]
equal_to_ten = (vector == 10)
print(equal_to_ten)
[False  True False False]
# 输出只有相对于位布尔值是True位置上的值
print(vector[equal_to_ten])
[10]
```











## 5、横向拼接





```
In [40]: arr3
Out[40]:
array([[  0,   0,   0,   3],
[  5,   8,  13,  21],
[ 34,  55,  89, 144]])

In [41]: arr4
Out[41]:
array([[ 1,  2,  3,  4],
[ 5,  6,  7,  8],
[ 9, 10, 11, 12]])

In [42]: np.hstack((arr3,arr4))
Out[42]:
array([[  0,   0,   0,   3,   1,   2,   3,   4],
[  5,   8,  13,  21,   5,   6,   7,   8],
[ 34,  55,  89, 144,   9,  10,  11,  12]])
```


**横向拼接**arr3和arr4两个数组，但必须满足两个数组的行数相同。









```
In [43]: np.vstack((arr3,arr4))  
Out[43]:
array([[  0,   0,   0,   3],
[  5,   8,  13,  21],
[ 34,  55,  89, 144],
[  1,   2,   3,   4],
[  5,   6,   7,   8],
[  9,  10,  11,  12]])
```



纵向拼接arr3和arr4两个数组，但必须满足两个数组的列数相同。



```
In [44]: np.column_stack((arr3,arr4))    #与hstack函数具有一样的效果
Out[44]:
array([[  0,   0,   0,   3,   1,   2,   3,   4],
[  5,   8,  13,  21,   5,   6,   7,   8],
[ 34,  55,  89, 144,   9,  10,  11,  12]])

In [45]: np.row_stack((arr3,arr4))    #与vstack函数具有一样的效果
Out[45]:
array([[  0,   0,   0,   3],
[  5,   8,  13,  21],
[ 34,  55,  89, 144],
[  1,   2,   3,   4],
[  5,   6,   7,   8],
[  9,  10,  11,  12]])
```









————————————————————————————————————————



# 延展一：range的用法



   一开始还是R的思维以为[1:2]就可以得到一个序列，python里面不是，需要range，有点像R里面的rep

   range(0,2) =R= [1,2]

   range(0,10,2) 0-9每隔2个取数一次



[xrange](http://www.nowamagic.net/academy/tag/xrange) 用法与 range 完全相同，所不同的是生成的不是一个list对象，而是一个生成器。



```
>>> xrange(5)

xrange(5)

>>> list(xrange(5))

[0, 1, 2, 3, 4]

>>> xrange(1,5)

xrange(1, 5)

>>> list(xrange(1,5))

[1, 2, 3, 4]

>>> xrange(0,6,2)

xrange(0, 6, 2)

>>> list(xrange(0,6,2))
[0, 2, 4]
```

参考：[Python xrange与range的区别](http://www.nowamagic.net/academy/detail/1302446)



————————————————————————————————————————



## 延伸二：高纬度array表示[0,0,0,0]



一个array有四个维度：[1,1024,19,19]，代表的意思是，有1024个19*19矩阵，如果要抽取其中一个19*19的矩阵，则表示为：

[0,1,:,:]



————————————————————————————————————————



## 延伸三：array中数据的替换

ndarray.itemset: 把 ndarray 中的某個值（純量）改掉，使用範例如下：



```python
>>> x = np.array([[[1, 2, 3], [4, 5, 6]], [[7, 8, 9], [10, 11, 12]]], np.int32)
>>> x
array([[[ 1,  2,  3],
            [ 4,  5,  6]],

        [[ 7,  8,  9],
            [10, 11, 12]]], dtype=int32)
# 把 index = 1 的 value 改成 999
>>> x.itemset(1, 999)
>>> x
array([[[  1, 999,   3],
        [  4,   5,   6]],

        [[  7,   8,   9],
        [ 10,  11,  12]]], dtype=int32)
# 把 index = (1, 1, 2) 的值改成 888
>>> x.itemset((1, 1, 2), 888)
>>> x
array([[[  1, 999,   3],
            [  4,   5,   6]],

        [[  7,   8,   9],
            [ 10,  11, 888]]], dtype=int32)
```







