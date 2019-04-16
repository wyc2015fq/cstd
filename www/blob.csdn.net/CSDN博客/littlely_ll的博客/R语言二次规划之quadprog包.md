# R语言二次规划之quadprog包 - littlely_ll的博客 - CSDN博客





2016年12月30日 22:04:31[littlely_ll](https://me.csdn.net/littlely_ll)阅读数：4535








## 二次规划

二次规划是非线性规划中一类特殊的数学规划问题，在如投资组合、约束最小二乘问题中都有应用。二次规划已经成为运筹学，经济数学，管理科学和组合优化科学的基本方法。 
**二次规划的一般形式：**


$min_{x}\quad q(x)=\frac{1}{2}x^TDx-d^Tx+c\\s.t. \quad A^Tx\ge b_0$

其中，x为向量，c为标量常数，G为Hessian矩阵，当G为半正定矩阵时，上式为凸二次规划，当G为正定矩阵时，上式为严格的凸二次规划，全局最小值是唯一的，A为约束系数矩阵。
## quadprog包

quadprog包是专门解决凸二次规划问题的包，其中的函数为solve.QP.但是该函数只解决严格的凸二次规划问题，求极小值。

> 
solve.QP(Dmat, dvec, Amat, bvec, meq=0, factorized=FALSE)  
**函数参数：**

   - Dmat: 为Hessian矩阵 

   - dvec: 为向量，和Dmat相对应 

   - Amat: 约束的系数矩阵，默认约束为“$\ge$” 

   - bvec: 为向量，和Amat相对应 

   - meq: 表示从哪一行开始Amat矩阵中的约束是等式，默认为0 
**VALUE:**

   - solution: 二次规划问题中向量的取值 

   - value: 标量，二次规划目标函数的取值 

   - unconstrained.solution: 没有约束条件下向量的取值
## R实现

假设一个例子： 

要求最小化$Q(x,y)=\frac{1}{2}(x^2+y^2)-3x-y$,则 
**目标函数：**


$Q(x,y)=\frac{1}{2}\pmatrix{x & y}\pmatrix{1 & 0\\ 0 & 2}\pmatrix{x\\y}-\pmatrix{3 & 1}\pmatrix{x\\ y}+6$
**约束条件：**


$x+y\ge 2\\-x+2y\ge 1\\2x-y\ge-4$

从上式可得： 


$D=\pmatrix{1 & 0\\0 &2}\quad d=\pmatrix{3\\1}\\A^T=\pmatrix{1 & 1\\-1 & 2\\ 2 & -1}\quad b_0=\pmatrix{2\\1\\-4}$

代码如下：
```
D <- matrix(c(1,0,0,2),nr=2)
d <- c(3,1)
A <- t(matrix(c(1,-1,2,1,2,-1),nr=3))##函数表达式中为A^T,而在程序中需要输入A。
b0 <- c(2,1,-4)
solve.QP(Dmat = D,dvec = d, Amat = A,bvec = b0)
```

$solution 

[1] 2.0 1.5

$value 

[1] -3.25

$unconstrained.solution 

[1] 3.0 0.5

$iterations 

[1] 2 0

$Lagrangian 

[1] 0 1 0

$iact 

[1] 2

在约束条件下，$x=2.0,y=1.5$,目标函数最小值为-3.25；在无约束条件下$x=3.0,y=0.5$。







