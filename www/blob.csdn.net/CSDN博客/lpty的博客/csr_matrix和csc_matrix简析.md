
# csr_matrix和csc_matrix简析 - lpty的博客 - CSDN博客

2018年04月10日 11:21:59[lpty](https://me.csdn.net/sinat_33741547)阅读数：2537



## 一、概念
csr_matrix(Compressed Sparse Row matrix)或csc_matric(Compressed Sparse Column marix)，为压缩稀疏矩阵的存储方式。这里均以scipy包中的方法作为例子，具体可看：[文档](https://docs.scipy.org/doc/scipy/reference/generated/scipy.sparse.csr_matrix.html)
## 二、简析
### 1、scipy.sparse.csr_matrix
`>>> indptr = np.array([0, 2, 3, 6])
>>> indices = np.array([0, 2, 2, 0, 1, 2])
>>> data = np.array([1, 2, 3, 4, 5, 6])
>>> csr_matrix((data, indices, indptr), shape=(3, 3)).toarray()
array([[1, 0, 2],
       [0, 0, 3],
       [4, 5, 6]])`上述方式为按照row行来压缩
（1）data表示数据，为[1, 2, 3, 4, 5, 6]
（2）shape表示矩阵的形状
（3）indices表示对应data中的数据，在压缩后矩阵中各行的下标，如：数据1在某行的0位置处，数据2在某行的2位置处，数据6在某行的2位置处。
（4）indptr表示压缩后矩阵中每一行所拥有数据的个数，如：[0 2 3 6]表示从第0行开始数据的个数，0表示默认起始点，0之后有几个数字就表示有几行，第一个数字2表示第一行有2 - 0 = 2个数字，因而数字1，2都第0行，第二行有3 - 2 = 1个数字，因而数字3在第1行，以此类推。
### 2、scipy.sparse.csc_matrix
`>>> indptr = np.array([0, 2, 3, 6])
>>> indices = np.array([0, 2, 2, 0, 1, 2])
>>> data = np.array([1, 2, 3, 4, 5, 6])
>>> csc_matrix((data, indices, indptr), shape=(3, 3)).toarray()
array([[1, 0, 4],
       [0, 0, 5],
       [2, 3, 6]])`上述方式为按照colums列来压缩，计算方式与按行类似。

