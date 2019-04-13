
# 用累积分布函数（CDF）计算期望 - 颹蕭蕭 - CSDN博客


2019年01月01日 16:50:30[颹蕭蕭](https://me.csdn.net/itnerd)阅读数：2823


一般计算期望的方法为：
$$
E(x) = \sum_x xP(x)
$$
或者
$$
E(x) = \int xP(x)dx
$$

---但如果我们已知

---非负

---随机变量的累积分布函数(CDF)为

$$
F(x)
$$
---时，可以用如下方式计算：

$$
E(x) = \int_0^\infty 1-F(x)dx
$$

---或者对于取值为离散自然数的随机变量

$$
E(x) = \sum_{n=0}^\infty Pr(x\geq n)
$$

---证明1：

$$
E(x) = \int_0^{\infty} yP(y)dy =  \int_0^{\infty} \int_0^yP(y)dxdy \\=  \int_0^{\infty} \int_x^{\infty}P(y)dydx = \int_0^{\infty} 1-F(x)dx
$$

---证明2：

$$
E(x) = \sum_{k=0}^{\infty} kPr(x=k) = \sum_{k=0}^{\infty}\sum_{n=0}^{k} Pr(x=k) \\=\sum_{n=0}^{\infty} \sum_{k=n}^{\infty}Pr(x=k) = \sum_{n=0}^\infty Pr(x\geq n)
$$


