
# 共轭梯度法 (CG) 解线性方程组 - 颹蕭蕭 - CSDN博客


2019年01月08日 14:58:22[颹蕭蕭](https://me.csdn.net/itnerd)阅读数：1893


求解线性方程组：
$$
Ax = b
$$
其中$A\in S^m_{++} \subset \R^{m\times m}$，$b \in \R^m$。
共轭梯度法的matlab实现：
```python
function [x] = CG(A,b)
x = 0;   // 迭代初值
r = b;   // 初始残差
i_max = 50;  // 最大迭代次数
yita = 1e-6; // 残差限度
i = 0;
while sqrt(r'*r)> yita && i<i_max
    i = i+1;
    if i == 1
        p = r;
    else
        beta = r'*r/(r_before'*r_before);
        p = r + beta*p;
    end
    alpha = r'*r/(p'*A*p);
    x = x + alpha*p;
    r_before = r;
    r = r - alpha*A*p;
end
```
短短几行，效果奇佳。
测试：
```python
A = rand(20,20);
A = A*A';
xs = randn(20,1);
b = A*xs;
norm(xs - CG(A,b))
```
输出：
`ans =
   1.8305e-08`需要注意的是，方阵 A 必须是**对称正定**的，否则无法得到正确结果例如：
```python
A = rand(20,19);
A = A*A';  % 此时 A 的秩为 19, 非满秩，半正定！
xs = randn(20,1);
b = A*xs;
norm(xs - CG(A,b))
```
误差就很大了！
`ans =
    0.3442`

