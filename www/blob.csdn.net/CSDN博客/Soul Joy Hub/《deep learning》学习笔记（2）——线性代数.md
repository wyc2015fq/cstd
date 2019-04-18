# 《deep learning》学习笔记（2）——线性代数 - Soul Joy Hub - CSDN博客

2017年09月12日 10:48:58[卓寿杰_SoulJoy](https://me.csdn.net/u011239443)阅读数：555标签：[深度学习																[线性代数																[numpy](https://so.csdn.net/so/search/s.do?q=numpy&t=blog)](https://so.csdn.net/so/search/s.do?q=线性代数&t=blog)](https://so.csdn.net/so/search/s.do?q=深度学习&t=blog)
个人分类：[深度学习																[机器学习](https://blog.csdn.net/u011239443/article/category/6268728)](https://blog.csdn.net/u011239443/article/category/6990149)

所属专栏：[神经网络与深度学习](https://blog.csdn.net/column/details/16408.html)


[http://blog.csdn.net/u011239443/article/details/77942575](http://blog.csdn.net/u011239443/article/details/77942575)

# 2.1 标量、向量、矩阵和张量

![](http://upload-images.jianshu.io/upload_images/1621805-9970fa00383c2351.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

在numpy中，可以用以下方式生成各种维度的张量：

```python
>>> import numpy as np
## 生成元素全为0的二维张量，两个维度分别为３，４
>>> np.zeros((3,4))
array([[ 0.,  0.,  0.,  0.],
       [ 0.,  0.,  0.,  0.],
       [ 0.,  0.,  0.,  0.]])
## 生成三维的随机张量，三个维度分别为２,3,4
>>> np.random.rand(2,3,4)
array([[[ 0.93187582,  0.4942617 ,  0.23241437,  0.82237576],
        [ 0.90066163,  0.30151126,  0.89734992,  0.56656615],
        [ 0.54487942,  0.80242768,  0.477167  ,  0.6101814 ]],

       [[ 0.61176321,  0.11454075,  0.58316117,  0.36850871],
        [ 0.18480808,  0.12397686,  0.22586973,  0.35246394],
        [ 0.01192416,  0.5990322 ,  0.34527612,  0.424322  ]]])
```

![](http://upload-images.jianshu.io/upload_images/1621805-e50330726963da8f.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

```python
## 生成包含５个元素的向量ｘ并将其转置
>>> x = np.arange(5).reshape(1,-1)
>>> x
array([[0, 1, 2, 3, 4]])
>>> x.T
array([[0],
       [1],
       [2],
       [3],
       [4]])
## 生成3*4的矩阵并转置
>>> A = np.arange(12).reshape(3,4)
>>> A
array([[ 0,  1,  2,  3],
       [ 4,  5,  6,  7],
       [ 8,  9, 10, 11]])
>>> A.T
array([[ 0,  4,  8],
       [ 1,  5,  9],
       [ 2,  6, 10],
       [ 3,  7, 11]])
## 生成2*3*4的张量
>>> B = np.arange(24).reshape(2,3,4)
>>> B
array([[[ 0,  1,  2,  3],
        [ 4,  5,  6,  7],
        [ 8,  9, 10, 11]],

       [[12, 13, 14, 15],
        [16, 17, 18, 19],
        [20, 21, 22, 23]]])
## 将B的01两个维度转置
>>> B.transpose(1,0,2)
array([[[ 0,  1,  2,  3],
        [12, 13, 14, 15]],

       [[ 4,  5,  6,  7],
        [16, 17, 18, 19]],

       [[ 8,  9, 10, 11],
        [20, 21, 22, 23]]])
```

# 2.2 矩阵和向量相乘

![](http://upload-images.jianshu.io/upload_images/1621805-c05deeec1ae50a4e.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

```python
>>> A = np.arange(6).reshape(3,2)
>>> B = np.arange(6).reshape(2,3)
>>> A
array([[0, 1],
       [2, 3],
       [4, 5]])
>>> B
array([[0, 1, 2],
       [3, 4, 5]])
>>> np.matmul(A,B)
array([[ 3,  4,  5],
       [ 9, 14, 19],
       [15, 24, 33]])
```
- 元素对应运算(Element-wise Operation)：针对形状相同张量的运算统称，包括元素对应乘积、相加等，即对两个张量相同位置的元素进行加减乘除等运算。元素对应乘积（element-wise product）或者Hadamard 乘积（Hadamard product），记为 A ⊙ B:

```python
>>> A = np.arange(6).reshape(3,2)
>>> A*A
array([[ 0,  1],
       [ 4,  9],
       [16, 25]])
>>> A + A
array([[ 0,  2],
       [ 4,  6],
       [ 8, 10]])
>>> A + A
array([[ 0,  2],
       [ 4,  6],
       [ 8, 10]])
```

# 2.3 单位矩阵和逆矩阵

![](http://upload-images.jianshu.io/upload_images/1621805-13170ab9c3e04ded.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

```python
>>> np.eye(4)
array([[ 1.,  0.,  0.,  0.],
       [ 0.,  1.,  0.,  0.],
       [ 0.,  0.,  1.,  0.],
       [ 0.,  0.,  0.,  1.]])
```

![](http://upload-images.jianshu.io/upload_images/1621805-1fa8b57c32a1c58f.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

```python
>>> A = np.arange(4).reshape(2,2)
>>> A
array([[0, 1],
       [2, 3]])
>>> np.linalg.inv(A)
array([[-1.5,  0.5],
       [ 1. ,  0. ]])
```

# 2.4 线性相关和生成子空间

![](http://upload-images.jianshu.io/upload_images/1621805-566b0cf13232df50.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

如果一组向量中的任意一个向量都不能表示成其他向量的线性组合，那么这组向量被称为线性无关。

# 2.5 范数

在机器学习中，我们经常使用被称为范数（norm）的函数衡量向量大小： 
![](http://upload-images.jianshu.io/upload_images/1621805-01da8d6011f50b5e.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

有时候我们可能也希望衡量矩阵的大小。在深度学习中，最常见的做法是使 

用Frobenius 范数（Frobenius norm）：

![](http://upload-images.jianshu.io/upload_images/1621805-10dd18ae3cfe3091.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

# 2.6 特殊类型的矩阵和向量

对角矩阵（diagonal matrix）只在主对角线上含有非零元素，其他位置都是零。

![](http://upload-images.jianshu.io/upload_images/1621805-d222b4f52b777ed7.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

![](http://upload-images.jianshu.io/upload_images/1621805-b950136266938900.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

如果这些向量不仅互相正交，并且范数都为 1，那么我们称它们是标准正交（orthonormal）。

![](http://upload-images.jianshu.io/upload_images/1621805-99ab7e99ce324965.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

# 2.7 特征分解

![](http://upload-images.jianshu.io/upload_images/1621805-0b128c063aa8c1e1.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

![](http://upload-images.jianshu.io/upload_images/1621805-2d686564491f5cca.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

# 2.8 奇异值分解

![](http://upload-images.jianshu.io/upload_images/1621805-566c5a3f5c6aa572.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

# 2.9 Moore-Penrose 伪逆

![](http://upload-images.jianshu.io/upload_images/1621805-015f8b8c23261cb3.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

# 2.10 迹运算

![](http://upload-images.jianshu.io/upload_images/1621805-fc6712eb9d511893.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

![](http://upload-images.jianshu.io/upload_images/1621805-8a840f99da3ad569.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

# 2.11 行列式

行列式，记作 det(A)，是一个将方阵 A 映射到实数的函数。行列式等于矩阵特征值的乘积。行列式的绝对值可以用来衡量矩阵参与矩阵乘法后空间扩大或者缩小了多少。如果行列式是 0，那么空间至少沿着某一维完全收缩了，使其失去了所有的体积。如果行列式是 1，那么这个转换保持空间体积不变。 

行列式通常使用迹运算来求解。

# 2.12 实例：主成分分析

参阅：[《机器学习实战》（十三）—— PCA](http://blog.csdn.net/u011239443/article/details/77363466)
![](https://img-blog.csdn.net/20170912104826110?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMTIzOTQ0Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

