# pyspark.linalg模块学习 - littlely_ll的博客 - CSDN博客





2017年10月05日 15:55:29[littlely_ll](https://me.csdn.net/littlely_ll)阅读数：978








# class pyspark.ml.linalg.Vector

## 方法

toArray(): 把vector转换为numpy.ndarray

# class pyspark.ml.linalg.DenseVector(ar)

```
v = Vectors.dense([1.0, 2.0])
u = Vectors.dense([3.0, 4.0])
#可以进行加减乘除
v + u  #DenseVector([4.0, 6.0])
v * u #DenseVector([3.0, 8.0])
```

## 方法

dot(other): 计算两向量的点积，支持Numpy array, list, SparseVector, Scipy sparse 

norm(p)：计算SparseVector的正则化 

numNonzeros(): 非零元素个数 

size:  向量大小 

squared_distance(other)：到SparseVector或Numpy.array的平方距离 

toArray(): 返回一个SparseVector副本
# class pyspark.ml.linalg.Vectors

工厂方法

## 方法

dense(*elements): 创建一个dense vector 

Vectors.dense([1, 2, 3]) #DenseVector([1.0, 2.0, 3.0]) 

Vectors.dense(1.0, 2.0) #DenseVector([1.0, 2.0])
norm(vector,p):找到给定vecor的norm 

sparse(size, *args): 创建系数矩阵，可以使用字典，（index，value）对，切片数组和值 

Vectors.sparse(4, {1: 1.0, 3: 5.5}) # SparseVector(4, {1: 1.0, 3: 5.5}) 

Vectors.sparse(4, [(1, 1.0), (3, 5.5)]) #SparseVector(4, {1: 1.0, 3: 5.5}) 

saquare_distance(v1,v2)：v1和v2向量的平方距离，类型为SparseVector, DenseVector, np.array, array.array 

zeros(size):
# class pyspark.ml.linalg.DenseMatrix(numRows, numCols, values, isTransposed=False)

## 方法

toArray() 

toSparse()

```
m = DenseMatrix(2, 2, range(4))
m.toArray()
#array([[ 0.,  2.],
       [ 1.,  3.]])
```

# class pyspark.ml.linalg.SparseMatrix(numRows, numCols, colPtrs, rowIndices, values, isTransposed=False)

toArray() 

toDense()






