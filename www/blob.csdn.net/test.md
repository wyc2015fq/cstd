# 运筹系列1：线性规划单纯形法python代码 - kittyzc的博客 - CSDN博客
2018年08月15日 17:06:32[IE06](https://me.csdn.net/kittyzc)阅读数：1588所属专栏：[运筹学算法python实现](https://blog.csdn.net/column/details/26511.html)
# 1. 模型

常见的线性规划模型如下： 

max $z = cx$

s.t. $Ax = b$
# 2. 求解步骤

假设B是基变量集合，通过矩阵的线性变换，基变量可由非基变量表示： 
$x'_i=c_i+\Sigma_{j\notin B}m_{i,j}x'_j,(i\in B)$

目标函数z也可以完全由非基变量表示： 
$z = z_0+\Sigma_{j\notin B}c_jx'_j$

当达到最优解时，**目标函数中所有的系数c≤0**，这样非基变量等于0时，目标函数可以取到最大值。以此为目标，**每次将最大的正系数max{$c_j$}对应的非基变量替换为基变量，同时将min{$b_j/a_{i,j}$}对应的基变量替换为非基变量**。这个进基/出基的过程称为pivoting。
# 3. python算法实现

这里假设原问题都是小于等于约束，这样添加松弛变量之后，问题一定有初始可行解；同时假设问题存在有限最优解。特殊情况将在下一节进行处理。代码为：

```python
import numpy as np

def pivot():
    l = list(d[0][:-2])
    jnum = l.index(max(l)) #转入编号
    m = []
    for i in range(bn):
        if d[i][jnum] == 0:
            m.append(0.)
        else:
            m.append(d[i][-1]/d[i][jnum])
    inum = m.index(min([x for x in m[1:] if x!=0]))  #转出下标
    s[inum-1] = jnum
    r = d[inum][jnum]
    d[inum] /= r
    for i in [x for x in range(bn) if x !=inum]:
        r = d[i][jnum]
        d[i] -= r * d[inum]

def solve():
    flag = True
    while flag:
        if max(list(d[0][:-1])) <= 0: #直至所有系数小于等于0
            flag = False
        else:
            pivot()

def printSol():
    for i in range(cn - 1):
        if i in s:
            print("x"+str(i)+"=%.2f" % d[s.index(i)+1][-1])
        else:
            print("x"+str(i)+"=0.00")
    print("objective is %.2f"%(-d[0][-1]))
```

调用的例子：

```
d = np.loadtxt("data.txt", dtype=np.float)
(bn,cn) = d.shape
s = list(range(cn-bn,cn-1)) #基变量列表
solve()
printSol()
```

data.txt文件中的内容为：

1 14 6 0 0 0 0 0

1 1 1 1 0 0 0 4

1 0 0 0 1 0 0 2

0 0 1 0 0 1 0 3

0 3 1 0 0 0 1 6

代表的求解模型是：

max $z = x_0+14*x_1+6*x_2$

s.t.

$x_0 + x_1 + x_2 \leq 4$

$x_0 \leq 2$

$x_2 \leq3$

$3*x_1 + x_2 \leq 6$

运行后输出结果为：

x0=0.00

x1=1.00

x2=3.00

x3=0.00

x4=2.00

x5=0.00

x6=0.00

objective is 32.00

# 4. 写后感

将simplex用代码写出来，才觉得以前纠结那么久的问题原来那么简单。两三行代码能说清楚的事，何必写一堆看得人眼花缭乱的数学公式呢。 

另外，线性规划还有一些很基础的理论要掌握好： 

1. 极点和极方向的理论，这个是单纯型法的理论基础。可以参考[这里](https://wenku.baidu.com/view/1a60ce6125c52cc58bd6be23.html)

2. 对偶理论，这个在以后经常会用到。


