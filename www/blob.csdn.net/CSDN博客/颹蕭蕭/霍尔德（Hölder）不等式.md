
# 霍尔德（Hölder）不等式 - 颹蕭蕭 - CSDN博客


2018年10月27日 16:11:25[颹蕭蕭](https://me.csdn.net/itnerd)阅读数：1884


**引理**
设$p,q&gt;0，\frac1p+\frac1q=1.$则
$$
x^{\frac1p}y^{\frac1q} \leq  \frac xp+ \frac yq,\;\forall\;x,y\geq 0,
$$
等号仅当$x=y$时成立。
---证明：

---考察对数函数

$$
log(x)
$$
---，她显然是一个凹函数：

$$
log(\theta x+(1-\theta)y) \geq \theta log(x) +(1-\theta)log(y)
$$
---取

$$
\theta = \frac1p
$$
---，则

$$
1-\theta = \frac1q
$$
---，故

$$
log(\frac1p x+\frac1qy) \geq \frac1p log(x) +\frac1qlog(y)
$$
---两边同时去指数，得

$$
\frac xp+ \frac yq \geq x^{\frac1p}y^{\frac1q}
$$

---Hölder 不等式

---对引理中的不等式，做如下替换

$$
x_i = \frac{a_i^p}{\sum_{j=1}^{n}a_j^p},\;\;y_i = \frac{b_i^q}{\sum_{j=1}^{n}b_j^q}
$$
---得到 n 个不等式：

$$
\frac{a_ib_i}{(\sum_{j=1}^{n}a_j^p)^{\frac1p}(\sum_{j=1}^{n}b_j^q)^{\frac1q}} \leq \frac1p\frac{a_i^p}{\sum_{j=1}^{n}a_j^p}+\frac1q\frac{b_i^q}{\sum_{j=1}^{n}b_j^q}
$$
---将上式两边对

$$
i=1,2,···,n
$$
---求和，就得到

$$
\frac{\sum_{i=1}^{n}a_ib_i}{(\sum_{j=1}^{n}a_j^p)^{\frac1p}(\sum_{j=1}^{n}b_j^q)^{\frac1q}} \leq \frac1p+\frac1q = 1,
$$
$$
\Rightarrow\sum_{i=1}^{n}a_ib_i \leq (\sum_{j=1}^{n}a_j^p)^{\frac1p}(\sum_{j=1}^{n}b_j^q)^{\frac1q}
$$

---上式要求

$$
a_i,b_i \geq 0
$$
---。否则，需要给等式右端的

$$
a_i,b_i
$$
---加上绝对值，得到如下不等式：

$$
a^Tb \leq ||a||_p||b||_q
$$
---事实上，

$$
||·||_q
$$
---正是

$$
||·||_p
$$
---的

---[对偶范数](https://blog.csdn.net/itnerd/article/details/83447223)

---。


