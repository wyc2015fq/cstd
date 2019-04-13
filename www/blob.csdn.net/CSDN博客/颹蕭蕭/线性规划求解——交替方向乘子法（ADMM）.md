
# 线性规划求解——交替方向乘子法（ADMM） - 颹蕭蕭 - CSDN博客


2019年01月08日 20:32:34[颹蕭蕭](https://me.csdn.net/itnerd)阅读数：2135所属专栏：[线性规划](https://blog.csdn.net/column/details/32485.html)



### 原问题
$$
(1)
$$
$$
\min_x \;c^Tx\\s.t. \;Ax=b\\x\geq 0 \tag{1}
$$

### 对偶问题
$$
(2)
$$
$$
\max_y\;b^Ty\\s.t.\;A^Ty+s=c\\s\geq 0\tag{2}
$$
$A \in \R^{m\times n}, x \in \R^{n}, s \in \R^{n}, y \in \R^{m}$
---本文用交替方向乘子法（Alternating Direction Method of Multipilers）实现线性规划问题的求解器。

---首先，写出对偶问题的

---[增广拉格朗日函数](https://blog.csdn.net/itnerd/article/details/86012869)

---，

$$
L_t(x,y,s) = -b^Ty+x^T(A^Ty+s-c) + \frac{t}{2}||A^Ty+s-c||_2^2 \\ s.t. \quad s \geq 0
$$

---基于ADMM的迭代规则：

$$
y^{k+1} = arg \min_y L_t(y;x^k,s^k) \\ s^{k+1} = arg \min_s L_t(s;x^k,y^{k+1}), \quad s\geq 0 \\ x^{k+1} = x^{k} +t*(A^Ty^{k+1}+s^{k+1}-c)
$$
---直到结果收敛。针对该问题，其增广拉格朗日函数非常简单，可以写出迭代步的显式形式：

$$
y^{k+1} = arg \min_y L_t(y;x^k,s^k) \\ =-(AA^T)^{-1}\bigg((Ax^k-b)/t+A(s^k-c)\bigg)
$$

$$
s^{k+1} = arg \min_s L_t(s;x^k,y^{k+1}), \quad s\geq 0 \\ = max(c-A^Ty^{k+1}-\frac{x^k}{t},0)
$$

$$
x^{k+1} = x^{k} +t*(A^Ty^{k+1}+s^{k+1}-c)
$$

---代码如下：

---function [x] = LP_ADMM_dual(c, A, b, opts, x0)
m = size(A,1);
n = size(A,2);
% 随机初始化
y = randn(m,1);
x = x0; 
s = randn(n,1);
t = 0.1;  % ALM rate
S = A*A';
U = chol(S);
L = U'; %cholesky decomposition: S = L*U = U'*U
err = 1;
x_old = x;
while(err > 1e-6)
    
    y = -U\(L\((A*x-b)/t+A*(s-c)));  % 固定 x,s, 更新 y
    
    s = max(c-A'*y-x/t,0); % 固定 y,x, 更新 s
    
    x = x + (A'*y+s-c)*t;  % 固定 y,s, 更新 x
    
    err = norm(x-x_old);
    x_old = x;
end
end

---测试：

---% 生成数据
n = 100;
m = 20;
A = rand(m,n);
xs = full(abs(sprandn(n,1,m/n)));
b = A*xs;
y = randn(m,1);
s = rand(n,1).*(xs==0);
c = A'*y + s;
% 计算误差
errfun = @(x1, x2) norm(x1-x2)/(1+norm(x1));
% 标准答案
figure(1);
subplot(2,1,1);
stem(xs,'fill','k-.')
title('exact solu');
% ADMM 求解
opts = [];
tic;
[x1, out] = LP_ADMM_dual(c, A, b, opts, x0);
t1 = toc;
subplot(2,1,2);
stem(x1,'fill','k-.');
title('lp_admm_dual');
fprintf('lp-alm-dual: cpu: %5.2f, err-to-exact: %3.2e\n', t2, errfun(x1, xs));
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190108202816670.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2l0bmVyZA==,size_16,color_FFFFFF,t_70)
---lp-admm-dual: cpu:  0.08, err-to-exact: 1.17e-04

---又快又准！！！


