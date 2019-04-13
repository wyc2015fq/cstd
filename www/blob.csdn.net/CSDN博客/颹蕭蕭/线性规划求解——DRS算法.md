
# 线性规划求解——DRS算法 - 颹蕭蕭 - CSDN博客


2019年01月08日 23:15:44[颹蕭蕭](https://me.csdn.net/itnerd)阅读数：1604标签：[线性规划																](https://so.csdn.net/so/search/s.do?q=线性规划&t=blog)[DRS																](https://so.csdn.net/so/search/s.do?q=DRS&t=blog)[LP																](https://so.csdn.net/so/search/s.do?q=LP&t=blog)[迭代																](https://so.csdn.net/so/search/s.do?q=迭代&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=LP&t=blog)[
																					](https://so.csdn.net/so/search/s.do?q=DRS&t=blog)个人分类：[凸优化																](https://blog.csdn.net/itnerd/article/category/8105675)[机器学习																](https://blog.csdn.net/itnerd/article/category/8068411)[
							](https://blog.csdn.net/itnerd/article/category/8105675)
所属专栏：[线性规划](https://blog.csdn.net/column/details/32485.html)
[
																	](https://so.csdn.net/so/search/s.do?q=DRS&t=blog)
[
				](https://so.csdn.net/so/search/s.do?q=线性规划&t=blog)
[
			](https://so.csdn.net/so/search/s.do?q=线性规划&t=blog)

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
---DRS算法

---考虑优化问题：

$$
\min_x\quad f(x) = g(x) + h(x)
$$
---其中

$$
g(x)
$$
---和

$$
h(x)
$$
---均为闭凸函数。

---Douglas-Rachford 迭代

---从任意初始点

$$
z^{(0)}
$$
---， 重复以下步骤：

$$
x^{k} = prox_{th}(z^{k-1})
$$
---(3)

$$
y^{k} = prox_{tg}(2x^k-z^{k-1})  \tag{3}
$$
$$
z^{k} = z^{k-1} + y^{k} - x^{k}
$$
---直至收敛。

---注：

$$
prox_f(x) = arg\min_u\quad f(u) + \frac{1}{2}||u-x||_2^2
$$

$$
prox_{tf}(x) = arg\min_u\quad tf(u) + \frac{1}{2}||u-x||_2^2 \\ = arg\min_u\quad f(u) + \frac{1}{2t}||u-x||_2^2
$$

---言归正传，针对线性规划的

---原问题

---，将其写成无约束优化问题：

$$
\min_x \quad c^Tx + \mathbb{I}_{Ax=b}(x) + \mathbb{I}_{x\geq 0}(x)
$$
---其中

$$
\mathbb{I}_S
$$
---表示集合

$$
S
$$
---的示性函数，即

$$
\mathbb{I}_S(x)=\left\{
             \begin{array}{lr}
             0 ,\quad x \in S&amp;  \\
             \infty, \quad otherwise&amp;  
             \end{array}
\right.
$$
---因此可以将目标函数拆分成两部分：

$$
g(x) = c^Tx+\mathbb{I}_{Ax=b}(x) \\  h(x) =  \mathbb{I}_{x\geq 0}(x)
$$
---然后利用 Douglas-Rachford 迭代即可求出问题的解

$$
x
$$
---。

---下面来看具体细节：

$$
prox_{tg}
$$
---和

$$
prox_{th}
$$
---解析表达式是什么？

---先来看简单的：

$$
prox_{th}(x) = arg\min_u\quad h(u) + \frac{1}{2t}||u-x||_2^2
$$
---也就是说

$$
prox_{th}(x)
$$
---是在固定

$$
x
$$
---时下述问题的解：

$$
\min_u \quad  \frac{1}{2t}||u-x||_2^2 \\ s.t. \quad u\geq 0
$$
---说白了，就是

$$
x
$$
---在第一象限的投影！所以

---(4)

$$
prox_{th}(x) = max(x,0) \tag{4}
$$

---接下来稍复杂，

$$
prox_{tg}(x)
$$
---是在固定

$$
x
$$
---时下述问题的解：

$$
\min_u \quad  c^Tu + \frac{1}{2t}||u-x||_2^2 \\ s.t. \quad Au=b
$$

---写出拉格朗日函数：

$$
L(u,\lambda) = c^Tu + \frac{1}{2t}||u-x||_2^2 +\lambda^T(Au-b)
$$
---由最优性条件：

$$
0 = \nabla_uL = c+A^T\lambda + \frac{1}{t}(u-x) \\ \Rightarrow u = x - t(c+A^T\lambda)
$$
---代入约束条件：

$$
0 = Au-b = Ax - tAc - tAA^T\lambda - b \\ \Rightarrow \lambda = (tAA^T)^{-1} \big( A(x-tc) - b \big)
$$
---解得

$$
u = x - t(c+A^T\lambda)=x - t\bigg(c+A^T(tAA^T)^{-1} \big( A(x-tc) - b \big)\bigg)
$$
---即

---(5)

$$
prox_{tg}(x) = x - t\bigg(c+A^T(tAA^T)^{-1} \big( A(x-tc) - b \big)\bigg) \tag{5}
$$

---将 (4)(5) 代入 Douglas-Rachford 迭代(3)，便是求解的全过程。

---上代码！

---function [x] = LP_DRS_primal(c, A, b, opts, x0)
m = size(A,1);
n = size(A,2);
z = randn(n,1);  %随机初始化
x = x0;   % 随机初始化
S = A*A';
U = chol(S);
L = U'; %cholesky decomposition: S = L*U = U'*U
t = 0.1; % 超参数
prox_th = @(x) max(x,0);
prox_tg = @(x) x-t*(c+A'*((t*U)\(L\(A*(x-t*c)-b))));
err = 1;
x_old = x;
while(err > 1e-6)
    
   x = prox_th(z);
   
   y = prox_tg(2*x-z);
   
   z = z + y - x;
    
   err = norm(x-x_old);
   x_old = x;
end
end

---看效果！

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
% DRS 求解
opts = [];
tic;
[x1] = LP_DRS_primal(c, A, b, opts, x0);
t1 = toc;
subplot(2,1,2);
stem(x1,'fill','k-.');
title('lp_drs_primal');
fprintf('lp-drs-primal: cpu: %5.2f, err-to-exact: %3.2e\n', t2, errfun(x1, xs));
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190109132816280.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2l0bmVyZA==,size_16,color_FFFFFF,t_70)
---看速度！

---[
](https://img-blog.csdnimg.cn/20190109132816280.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2l0bmVyZA==,size_16,color_FFFFFF,t_70)

---lp-drs-primal: cpu:  0.14, err-to-exact: 2.92e-04

---[
            ](https://img-blog.csdnimg.cn/20190109132816280.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2l0bmVyZA==,size_16,color_FFFFFF,t_70)


