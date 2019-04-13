
# 凸函数的梯度的单调性 (Monotonicity of gradient) - 颹蕭蕭 - CSDN博客


2018年11月21日 11:29:40[颹蕭蕭](https://me.csdn.net/itnerd)阅读数：2479


可微函数$f$是凸函数 当且仅当$domf$是凸集，且
$$
(\bigtriangledown f(x)-\bigtriangledown f(y))^T(x-y)&gt;0, \;\; \forall x,y \in dom f
$$
即$\bigtriangledown f: \R^n \rightarrow \R^n$是单调映射（monotone mapping）。
---证明：

---如果

$$
f
$$
---是可微的凸函数，则有

$$
f(y) \geq f(x) + \bigtriangledown f(x)^T(y-x),\\ f(x) \geq f(y) + \bigtriangledown f(y)^T(x-y).
$$
---将上面两式相加得

$$
(\bigtriangledown f(x)-\bigtriangledown f(y))^T(x-y)&gt;0
$$

---如果

$$
\bigtriangledown f
$$
---是单调的，定义函数

$$
g
$$
---:

$$
g(t) = f(x+t(y-x)), \;t \in [0,1]\\ g&\#x27;(t) = \bigtriangledown f(x+t(y-x))^T(y-x)
$$
---则由

$$
g&\#x27;(t)
$$
---的连续性以及

$$
g&\#x27;(1)-g&\#x27;(0) &gt;0 \;且\; g&\#x27;(0)-g&\#x27;(0) = 0
$$
---得

$$
g&\#x27;(t) -g&\#x27;(0) \geq 0,\;\;
$$
---因此

$$
f(y) = g(1) = g(0) + \int_0^1 g&\#x27;(t)dt \geq g(0) + g&\#x27;(0) \\= f(x) + \bigtriangledown f(x))^T(y-x)
$$
---即

$$
f
$$
---为凸函数。


