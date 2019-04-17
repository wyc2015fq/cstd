# 优化问题及其Lagrange对偶问题 - 浅梦的博客 - CSDN博客





2017年08月24日 01:08:55[浅梦s](https://me.csdn.net/u012151283)阅读数：3192








本文图片来自[http://shijuanfeng.blogbus.com/](http://shijuanfeng.blogbus.com/)

# 优化和凸优化问题

## 优化问题

![这里写图片描述](https://img-blog.csdn.net/20170824002356526?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjE1MTI4Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![这里写图片描述](https://img-blog.csdn.net/20170824002410181?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjE1MTI4Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
## 凸优化问题

![这里写图片描述](https://img-blog.csdn.net/20170824002504059?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjE1MTI4Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

# Lagrange函数和对偶函数

![这里写图片描述](https://img-blog.csdn.net/20170824002556147?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjE1MTI4Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![这里写图片描述](https://img-blog.csdn.net/20170824002629327?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjE1MTI4Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
## 对偶函数是凹函数

如果Lagrange函数关于$x$无下届，则对偶函数取值为$-\infty$。因为对偶函数是一族关于$(\lambda,v)$的仿射函数的逐点下确界，所以即使原问题不是凸的，对偶函数也是**凹函数**。

## 对偶函数是原问题最优值的下界

设$\widetilde x$是原文题的一个可行点，即$f_i(\widetilde x)\le 0$且$h_i(\widetilde x)=0$。根据假设$\lambda \ge0$， 


$\begin{align*}&\sum_{i=1}^m{\lambda_if_i(\widetilde x)}+\sum_{i=1}^p{v_ih_i(\widetilde x)}\le0 \\L(\widetilde x,\lambda,v)=f_0(\widetilde x)+&\sum_{i=1}^m{\lambda_if_i(\widetilde x)}+\sum_{i=1}^p{v_ih_i(\widetilde x)}\le f_0(\widetilde x)\\g(\lambda,v)=inf_{x\in D} &L(x,\lambda,v)\le L(\widetilde x,\lambda,v)\le f_0(\widetilde x)\end{align*}$

由于每一个可行点$\widetilde x$都满足上式，得证。
# Lagrange对偶问题

## Lagrange对偶问题形式

Lagange对偶函数给出了原问题最优值$p^*$的一个下界。下面求解从Lagrange函数得到的最好下界 


$\begin{align*}\text{maximize } g(\lambda,v)\\\text{subject to }\lambda \ge 0\end{align*}$

满足$\lambda \ge0$和$g(\lambda,v) \ge -\infty$的一组$(\lambda,v)$为**对偶可行解**。 

对偶问题的最优解称为**对偶最优解**或**最优Lagrange乘子**。
## Lagrange对偶问题是凸优化问题

极大化的目标函数是凹函数，且约束集合是凸集。**对偶问题的凸性和原问题是否是凸优化问题无关**。

# 弱对偶和强对偶

## 弱对偶

![这里写图片描述](https://img-blog.csdn.net/20170824005718259?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjE1MTI4Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

## 强对偶和Slater条件

![这里写图片描述](https://img-blog.csdn.net/20170824005819198?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjE1MTI4Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

## 强对偶和弱化Slater条件

![这里写图片描述](https://img-blog.csdn.net/20170824005839493?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjE1MTI4Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

# 互补松弛性和KKT条件

## 互补松弛条件

![这里写图片描述](https://img-blog.csdn.net/20170824010123568?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjE1MTI4Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

由于求和项$\sum_i{\lambda_i^*f_i(x^*)}=0$每一项都是非正，因此$\lambda_i^*f_i(x^*)=0,\space i=1,...,m.$

它对任意原问题最优解和对偶问题最优解都成立（当对偶性成立时）。进一步地 
$\lambda_o^* >0 \Longrightarrow f_i(x^*)=0$
## KKT优化条件

![这里写图片描述](https://img-blog.csdn.net/20170824010653003?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjE1MTI4Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

总结起来- 广义拉格朗日的梯度为零
- 所有关于$x$和KKT乘子的约束都满足
- 不等式约束显示的“互补松驰性”:$\lambda f(x) = 0$

## 凸优化问题的KKT条件

优化问题中的KKT条件为具有强队对偶性的必要条件，而对于凸优化问题，KKT条件则是充要条件。 
![这里写图片描述](https://img-blog.csdn.net/20170824010705556?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjE1MTI4Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![这里写图片描述](https://img-blog.csdn.net/20170824010717595?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjE1MTI4Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
参考资料

> 
深入理解拉格朗日乘子法（Lagrange Multiplier) 和KKT条件[http://blog.csdn.net/xianlingmao/article/details/7919597](http://blog.csdn.net/xianlingmao/article/details/7919597)











