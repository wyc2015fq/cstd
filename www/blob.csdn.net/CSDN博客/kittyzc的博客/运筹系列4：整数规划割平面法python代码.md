# 运筹系列4：整数规划割平面法python代码 - kittyzc的博客 - CSDN博客
2018年08月15日 18:06:44[IE06](https://me.csdn.net/kittyzc)阅读数：492所属专栏：[运筹学算法python实现](https://blog.csdn.net/column/details/26511.html)
# 1. 从分支定界（branch and cut）到割平面（cutting plane）

**割平面简单来说，就是添加约束条件**。在分支定界算法中，添加的x≤floor[xs]和x≥ceil[xs]便是两个用来割平面的约束条件。 

分支定界法最终生成一颗树，当整数变量非常多时，求解节点会指数速度增加，因此需要使用一些方法提高求解速度，割平面法便是重要方法之一。分支的过程其实本身就是割平面的过程，floor[x]和ceil[x]之间的整个可行域在对x进行分支的过程中被切割掉了。

# 2.从单纯型表中寻找割平面

核心思想是：**将约束条件中的小数部分分离出来形成新的约束**。 

假设整数规划的线性松弛问题求解结果中有一个基变量$x_i=b_{i0}$不是整数，对应的约束条件为： 
$x_i+\Sigma_{j\in J}b_{ij} x_j= b_{i0}$

其中$J$是非基变量下标集合。 

令$Z(b) = floor(b)$，也就是b的整数部分；$S(b) = b-floor(b)$，也就是b的小数部分。则有： 
$S(b_i) - Σ_{j\in J}S(A_{ij})x_j=Z(b_i)+Σ_{j\in J}Z(A_{ij})x_j-Z(b_i)$

因此$S(b_i) - Σ_{j\in J}S(A_{ij})x_j$ 是一个整数。 

再结合 
$S(b_i) - Σ_{j\in J}S(A_{ij})x_j ≤S(b_i)< 1$

得到： 
$S(b_i) - Σ_{j\in J}S(A_{ij})x_j ≤ 0$

好了，这就是松弛问题每个非整数基变量带来的新的约束条件。为了转为标准型，还需要给这个约束条件添加一个剩余变量$x'$： 
$Σ_{j\in J}S(A_{ij})x_j -x'=S(b_i) $
# 3.python代码

基本框架还是用分支定界法，每次求解完之后添加割平面的约束条件：

```python
def add_new_restriction(matrix):
    new_column = np.zeros(matrix.shape[0]+1)
    new_line = np.zeros(matrix.shape[1])
    new_column[-1] = -1 
    #这里简单使用第一行约束条件为基础生成新约束条件。
    new_line = matrix[1, :] 
    for index in range(0, len(new_line)):
        number = np.array(new_line[index], dtype=float)
        if number.tolist().is_integer() == False:
            new_line[index] = math.floor(new_line[index])
    matrix = np.insert(matrix, matrix.shape[0], new_line, axis=0)
    matrix = np.insert(matrix, -1, new_column, axis=1)
    return matrix
```

