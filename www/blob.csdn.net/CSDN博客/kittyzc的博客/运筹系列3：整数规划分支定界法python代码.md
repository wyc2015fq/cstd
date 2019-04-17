# 运筹系列3：整数规划分支定界法python代码 - kittyzc的博客 - CSDN博客





2018年08月15日 17:51:27[IE06](https://me.csdn.net/kittyzc)阅读数：1714
个人分类：[python																[算法																[运筹学](https://blog.csdn.net/kittyzc/article/category/7940150)
所属专栏：[运筹学算法python实现](https://blog.csdn.net/column/details/26511.html)](https://blog.csdn.net/kittyzc/article/category/7940151)




# 1. 模型

整数规划的模型与线性规划基本相同，只是额外的添加了部分变量为整数的约束。

# 2. 求解步骤

整数规划求解的基本框架是分支定界法（Branch and bound，BnB）。首先去除整数约束得到“松弛模型”，使用线性规划的方法求解。若有某个变量不是整数，在松弛模型上分别添加约束：

```
x ≤ floor(A)
```

和

```
x ≥ ceil(A)
```

然后再分别求解，这个过程叫做**分支**。当节点求解结果中所有变量都是整数时，停止分支。这样不断迭代，形成了一棵树。

所谓的**定界**，指的是叶子节点产生后，相当于给问题定了一个下界。之后在求解过程中一旦某个节点的目标函数值小于这个下界，那就直接pass，不用再进行分支了；每次新产生叶子节点，则更新下界。

# 3. python算法实现

```
import math
from scipy.optimize import linprog
import sys

def integerPro(c, A, b, Aeq, beq,t=1.0E-12):
    res = linprog(c, A_ub=A, b_ub=b, A_eq=Aeq, b_eq=beq)
    if (type(res.x) is float or res.status != 0): 
        bestX = [sys.maxsize]*len(c)
    else:
        bestX = res.x
    bestVal = sum([x*y for x,y in zip(c, bestX)])
    if all(((x-math.floor(x))<t or (math.ceil(x)-x)<t) for x in bestX):
        return (bestVal,bestX)
    else:
        ind = [i for i, x in enumerate(bestX) if (x-math.floor(x))>t and (math.ceil(x)-x)>t][0]
        newCon1 = [0]*len(A[0])
        newCon2 = [0]*len(A[0])
        newCon1[ind] = -1
        newCon2[ind] = 1
        newA1 = A.copy()
        newA2 = A.copy()
        newA1.append(newCon1)
        newA2.append(newCon2)
        newB1 = b.copy()
        newB2 = b.copy()
        newB1.append(-math.ceil(bestX[ind]))
        newB2.append(math.floor(bestX[ind]))
        r1 = integerPro(c, newA1, newB1, Aeq, beq)
        r2 = integerPro(c, newA2, newB2, Aeq, beq)
        if r1[0] < r2[0]:
            return r1
        else:
            return r2
```

例子：输入

```
c = [3,4,1]
A = [[-1,-6,-2],[-2,0,0]]
b = [-5,-3]
Aeq = [[0,0,0]]
beq = [0]
print(integerPro(c, A, b, Aeq, beq))
```

输出

```
(8.0, array([2., 0., 2.]))
```

其中8是目标函数值，2，0，2是3个整数变量的值。](https://blog.csdn.net/kittyzc/article/category/7940149)





