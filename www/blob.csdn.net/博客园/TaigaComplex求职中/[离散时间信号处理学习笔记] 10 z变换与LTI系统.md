# [离散时间信号处理学习笔记] 10. z变换与LTI系统 - TaigaComplex求职中 - 博客园







# [[离散时间信号处理学习笔记] 10. z变换与LTI系统](https://www.cnblogs.com/TaigaCon/p/8325840.html)





我们前面讨论了z变换，其实也是为了利用z变换分析LTI系统。



# 利用z变换得到LTI系统的单位脉冲响应

对于用差分方程描述的LTI系统而言，z变换将十分有用。有如下形式的差分方程：

$\displaystyle{ y[n] = –\sum_{k=1}^{N}\left(\frac{a_k}{a_0}\right)y[n-k]+\sum_{k=0}^{M}\left(\frac{b_k}{a_0}\right)x[n-k] }$

我们可以通过z变换得到上述式子的单位脉冲响应。

等式两边进行z变换

$\begin{align*}
Y(z)
&=z\left\{-\sum_{k=1}^{N} \left( \frac{a_k}{a_0} \right)y[n-k]+\sum_{k=0}^{M}\left(\frac{b_k}{a_0}\right)x[n-k]\right\}\\
&=z\left\{-\sum_{k=1}^{N} \left( \frac{a_k}{a_0} \right)y[n-k]\right\}+z\left\{\sum_{k=0}^{M}\left(\frac{b_k}{a_0}\right)x[n-k]\right\}\quad z\ linearity\ property\\
&=-\sum_{k=1}^{N} \left( \frac{a_k}{a_0} \right)z^{-k}Y(z) + \sum_{k=0}^{M}\left(\frac{b_k}{a_0}\right)z^{-k}X(z) \quad z\ time\ shift\ property\\ 
\end{align*}$

整理后可以得到

$Y(z)=\left(\frac{\displaystyle{ \sum_{k=0}^{M}b_kz^{-k} }}{\displaystyle{\sum_{k=1}^{N}a_kz^{-k}}} \right )X(z)$

另外，我们知道LTI系统是通过卷积来定义的

$\displaystyle{ y[n] = h[n]*x[n] }$

等式两边进行z变换，可以得到

$Y(z) = H(z)X(z)$

因此有

$H(z) = \frac{\displaystyle{ \sum_{k=0}^{M}b_kz^{-k} }}{\displaystyle{\sum_{k=1}^{N}a_kz^{-k}}}$

我们对$H(z)$进行z逆变换即可得到单位脉冲响应$h[n]$。$H(z)$被称为**系统函数**。



# 因果LTI系统的一些z变换特性

此外，我们这里讨论的差分方程是因果的，即有
- 系统满足[初始松弛条件](http://www.cnblogs.com/TaigaCon/p/8094422.html#initialrest)，也就是说如果输入为$x[n]=0,n< 0$，有

$y[-N] = y[-N+1]=\cdot\cdot\cdot=y[-1]=0$

- 因果LTI系统的单位脉冲响应满足[$h[n]=0,n<0$](http://www.cnblogs.com/TaigaCon/p/8058634.html#causal)，那么系统函数$H(z)$的收敛域呈现$|z|>R$。












