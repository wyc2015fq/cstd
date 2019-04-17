# 运筹系列2：线性规划两阶段法python代码 - kittyzc的博客 - CSDN博客





2018年08月15日 17:42:34[IE06](https://me.csdn.net/kittyzc)阅读数：591
所属专栏：[运筹学算法python实现](https://blog.csdn.net/column/details/26511.html)









# 1. 补充问题

上一节中的代码在运行时还有很多细节没有处理，这里补充两个比较重要的情况： 

1. 存在等式约束 

如果有等式约束，那么就没法通过添加松弛变量直接给出初始可行解，需要用大M法或者两阶段法求初始可行解。计算机求解一般使用二阶段法，即首先给等式约束条件添加**人工变量**，使得问题有一个初始可行解。注意人工变量和**松弛变量/剩余变量**不一样。松弛变量/剩余变量是用于将不等式变为等式（标准形），而人工变量则是在等式约束中额外添加一个变量，这个变量最后一定要等于0才行，否则就是没有初始可行解，不能进入第二阶段。 

在第一阶段，每个不等式约束对应一个松弛变量，每个等式约束对应一个人工变量。另外，b<0的不等式约束也需要添加人工变量，因为对应的松弛变量等于b不能作为基变量。 

2. 没有有限最优解（目标函数无界） 

对应的情况是：目标函数中有某个系数大于0，对应变量在约束条件中的系数全都≤0，问题结束，没有最优解。
# 2. python算法实现

首先假设问题除了不等式约束，还有等式约束： 

求解问题为： 

min $cx$

s.t. 
$A_{ub} x ≤ b_{ub}$
$A_{eq}  x = b_{eq}$

定义maxiter为最大迭代次数，tol为求解精度，OptimizeResult为格式输出函数，solve_simplex为上一节的求解函数。完整的求解步骤如下：
```
def linprog(c, A_ub=None, b_ub=None, A_eq=None, b_eq=None, maxiter=1000, tol=1.0E-12):
    cc = np.asarray(c)
    f0 = 0
    n = len(c)
    mub = len(b_ub)
    meq = len(b_eq)
    m = mub+meq
    n_slack = mub
    n_artificial = meq + np.count_nonzero(bub < 0)
    Aub_rows, Aub_cols = A_ub.shape
    Aeq_rows, Aeq_cols = A_eq.shape
    slcount = 0
    avcount = 0
    status = 0
    messages = {0: "Optimization terminated successfully.",
                1: "Iteration limit reached.",
                2: "Optimization failed. Unable to find a feasible"
                   " starting point.",
                3: "Optimization failed. The problem appears to be unbounded.",
                4: "Optimization failed. Singular matrix encountered."}
    # 建立第一阶段单纯型表。
    T = np.zeros([m+2, n+n_slack+n_artificial+1])
    T[-2, :n] = cc
    T[-2, -1] = f0
    b = T[:-2, -1]
    if meq > 0:
        T[:meq, :n] = Aeq
        b[:meq] = beq
    if mub > 0:
        T[meq:meq+mub, :n] = Aub
        b[meq:meq+mub] = bub
        np.fill_diagonal(T[meq:m, n:n+n_slack], 1)

    #第一阶段的基向量basis，记录下标。
    basis = np.zeros(m, dtype=int)
    r_artificial = np.zeros(n_artificial, dtype=int)
    for i in range(m):
        if i < meq or b[i] < 0:
            basis[i] = n+n_slack+avcount
            r_artificial[avcount] = i
            avcount += 1
            if b[i] < 0:
                b[i] *= -1
                T[i, :-1] *= -1
            T[i, basis[i]] = 1
            T[-1, basis[i]] = 1
        else:
            basis[i] = n+slcount
            slcount += 1

    for r in r_artificial:
        T[-1, :] = T[-1, :] - T[r, :]

    #第一阶段求解
    nit1, status = solve_simplex(T, n, basis)

    # 如果第一阶段的目标函数为0，则删除人工变量，进入第二阶段
    if abs(T[-1, -1]) <= sol:
        T = T[:-1, :]
        T = np.delete(T, np.s_[n+n_slack:n+n_slack+n_artificial], 1)
    else:
        status = 2

    if status != 0:
        message = messages[status]
        if disp:
            print(message)
        return OptimizeResult(x=np.nan, fun=-T[-1, -1], nit=nit1, status=status, message=message, success=False)
    nit2, status = solve_simplex(T, n, basis, maxiter=maxiter-nit1, phase=2, callback=callback, tol=tol, nit0=nit1)

    solution = np.zeros(n+n_slack+n_artificial)
    solution[basis[:m]] = T[:m, -1]
    x = solution[:n]
    slack = solution[n:n+n_slack]
    obj = -T[-1, -1]
    return OptimizeResult(x=x, fun=obj, nit=int(nit2), status=status, slack=slack, message=messages[status], success=(status == 0))
```

下面是例子：

```
import numpy as np
c=np.array([2,3,-5])
A_ub=np.array([[-2,5,-1],[1,3,1]])
B_ub=np.array([-10,12])
A_eq=np.array([[1,1,1]])
B_eq=np.array([7])
res=linprog(-c,A_ub,B_ub,A_eq,B_eq)
print(res)
```

输出为

```
fun: -14.571428571428571
 message: 'Optimization terminated successfully.'
     nit: 2
   slack: array([3.85714286, 0.57142857, 6.42857143, 7., 0.])
  status: 0
 success: True
       x: array([6.42857143, 0.57142857, 0.])
```





