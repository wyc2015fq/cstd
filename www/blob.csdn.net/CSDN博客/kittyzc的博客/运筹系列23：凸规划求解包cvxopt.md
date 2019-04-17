# 运筹系列23：凸规划求解包cvxopt - kittyzc的博客 - CSDN博客





2019年04月09日 14:59:52[IE06](https://me.csdn.net/kittyzc)阅读数：20
个人分类：[算法																[运筹学](https://blog.csdn.net/kittyzc/article/category/7940150)](https://blog.csdn.net/kittyzc/article/category/7940151)








# 1. 软件介绍

## 1.1 安装

在机器学习问题中，我们常常需要对多优化问题进行求解，用一般的线性规划求解器没有办法求解了。cvxopt是一个可靠的求解包，安装方法如下：

```python
pip install cvxopt
```

## 1.2 使用说明

优化器：使用op(目标函数，约束条件)进行定义，调用solve()可以求解，调用objective.value()可以获得求解之后的目标值。op也可以从MPS标准格式的文件中读取数据。

变量：使用variable()进行声明，可以传入整数定义变量数组。求解之后可以调用value变量获取值

约束：可以直接写不等式表达式数组，或者用matrix矩阵形式表达

矩阵：下面举例说一下怎么用。注意直接转换matrix和转化为array后再转换为matrix的行/列是不一样的。
```python
>>> from cvxopt import matrix
>>> A = matrix(1, (1,4))
>>> print(A)
[ 1  1  1  1]
>>> A = matrix(1.0, (1,4))
>>> print(A)
[ 1.00e+00  1.00e+00  1.00e+00  1.00e+00]
>>> A = matrix([0, 1, 2, 3], (2,2))
>>> A = matrix((0, 1, 2, 3), (2,2))
>>> A = matrix(range(4), (2,2))
>>> print(A)
[ 0  2]
[ 1  3]
>>> A = matrix([1., 2., 3., 4., 5., 6.], (2,3))
>>> print(A)
[ 1.00e+00  3.00e+00  5.00e+00]
[ 2.00e+00  4.00e+00  6.00e+00]
>>> B = matrix(A, (3,2))
>>> print(B)
[ 1.00e+00  4.00e+00]
[ 2.00e+00  5.00e+00]
[ 3.00e+00  6.00e+00]
>>> from numpy import array
>>> x = array([[1., 2., 3.], [4., 5., 6.]])
>>> x
array([[ 1.  2.  3.]
       [ 4.  5.  6.]])
>>> print(matrix(x))
[ 1.00e+00  2.00e+00  3.00e+00]
[ 4.00e+00  5.00e+00  6.00e+00]
>>> print(matrix([[1., 2.], [3., 4.], [5., 6.]]))
[ 1.00e+00  3.00e+00  5.00e+00]
[ 2.00e+00  4.00e+00  6.00e+00]
>>> A1 = matrix([1, 2], (2,1))
>>> B1 = matrix([6, 7, 8, 9, 10, 11], (2,3))
>>> B2 = matrix([12, 13, 14, 15, 16, 17], (2,3))
>>> B3 = matrix([18, 19, 20], (1,3))
>>> C = matrix([[A1, 3.0, 4.0, 5.0], [B1, B2, B3]])
>>> print(C)
[ 1.00e+00  6.00e+00  8.00e+00  1.00e+01]
[ 2.00e+00  7.00e+00  9.00e+00  1.10e+01]
[ 3.00e+00  1.20e+01  1.40e+01  1.60e+01]
[ 4.00e+00  1.30e+01  1.50e+01  1.70e+01]
[ 5.00e+00  1.80e+01  1.90e+01  2.00e+01]
>>> A = spmatrix([2,-1,2,-2,1,4,3], [1,2,0,2,3,2,0], [0,0,1,1,2,3,4])
>>> print(A)
  [    0      2.00e+00     0         0      3.00e+00]
  [ 2.00e+00     0         0         0         0    ]
  [-1.00e+00 -2.00e+00     0      4.00e+00     0    ]
  [    0         0      1.00e+00     0         0    ]
```

# 2. 示例

## 2.1 线性规划

![在这里插入图片描述](https://img-blog.csdnimg.cn/20190111143942424.png)

第一种方式：
```python
>>> from cvxopt.modeling import *
>>> x = variable()
>>> y = variable()
>>> c1 = ( 2*x+y <= 3 )
>>> c2 = ( x+2*y <= 3 )
>>> c3 = ( x >= 0 )
>>> c4 = ( y >= 0 )
>>> lp1 = op(-4*x-5*y, [c1,c2,c3,c4])
>>> lp1.solve()
>>> lp1.status
'optimal'
>>> print(lp1.objective.value())
[-9.00e+00]
>>> print(x.value)
[ 1.00e+00]
>>> print(y.value)
[ 1.00e+00]
>>> print(c1.multiplier.value)
[ 1.00e+00]
>>> print(c2.multiplier.value)
[ 2.00e+00]
>>> print(c3.multiplier.value)
[ 2.87e-08]
>>> print(c4.multiplier.value)
[ 2.80e-08]
```

第二种方式，数组和矩阵形式。注意矩阵形式里面，要把变量大于等于0也添加到约束条件里面：

```python
>>> from cvxopt.modeling import *
>>> x = variable(2)
>>> A = matrix([[2.,1.,-1.,0.], [1.,2.,0.,-1.]])
>>> b = matrix([3.,3.,0.,0.])
>>> c = matrix([-4.,-5.])
>>> ineq = ( A*x <= b )
>>> lp2 = op(dot(c,x), ineq)
>>> lp2.solve()
>>> print(lp2.objective.value())
[-9.00e+00]
>>> print(x.value)
[ 1.00e+00]
[ 1.00e+00]
>>> print(ineq.multiplier.value)
[1.00e+00]
[2.00e+00]
[2.87e-08]
[2.80e-08]
```

第三种方式：

```python
>>> from cvxopt import matrix, solvers
>>> c = matrix([-4., -5.])
>>> G = matrix([[2., 1., -1., 0.], [1., 2., 0., -1.]])
>>> h = matrix([3., 3., 0., 0.])
>>> sol = solvers.lp(c, G, h)
>>> print(sol['x'])
[ 1.00e+00]
[ 1.00e+00]
```

## 2.2 凸优化示例

cvxopt.solvers.qp(P, q[, G, h[, A, b[, solver[, initvals]]]])

对应的标准型为：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190111163103768.png)

下面举一个例子：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190111162800438.png)
```python
from cvxopt import matrix, solvers

P = 2*matrix([[2, .5], [.5, 1]])
q = matrix([1.0, 1.0])
G = matrix([[-1.0,0.0],[0.0,-1.0]])
h = matrix([0.0,0.0])
A = matrix([1.0, 1.0], (1,2))
b = matrix(1.0)

sol=solvers.qp(P, q, G, h, A, b)
print(sol['x'])
print(sol['primal objective'])
```

更多例子可以参考 [http://cvxopt.org/examples/index.html](http://cvxopt.org/examples/index.html)







