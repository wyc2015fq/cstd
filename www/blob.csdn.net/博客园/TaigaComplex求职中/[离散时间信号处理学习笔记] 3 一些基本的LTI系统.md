# [离散时间信号处理学习笔记] 3. 一些基本的LTI系统 - TaigaComplex求职中 - 博客园







# [[离散时间信号处理学习笔记] 3. 一些基本的LTI系统](https://www.cnblogs.com/TaigaCon/p/8085733.html)





首先我们需要先对离散时间系统进行概念上的回顾：

$y[n] = T\{ x[n] \}$

上面的式子表征了离散时间系统，也就是把输入序列$x[n]$，映射称为$y[n]$的输出序列。

![image](https://images2017.cnblogs.com/blog/421096/201712/421096-20171222140813740-1346392460.png)

不过上述式子也可以有如下描述
- 对于某一时间点$n$，系统的输出$y[n]$可以通过$T\{x[n]\}$计算得到。
- 对整个系统来说，输入序列$x[n]$，会得到输出序列$T\{x[n]\}$。



按照上述第二条，单位脉冲响应就是：当输入单位脉冲$\delta[n]$时，会得到输出序列$T\{\delta[n]\}$

$h[n] = T\{ \delta[n] \}$

根据这个结论，如果我们知道了某个系统的表征式为$y[n] = T\{ x[n] \}$，那么把式子当中的$x[n]$替换成$\delta[n]$就能得到该系统的单位脉冲响应$h[n]$。



# 理想延迟系统

理想延迟系统的目的就是把输入序列做一定的时间延迟，然后进行输出。

定义：

$y[n] = x[n-n_d],\quad –\infty<n<\infty$

单位脉冲响应：

$h[n] = \delta[n-n_d]$





# 滑动平均系统

滑动平均系统的目的是取输入序列当中的某一段，对该段内的所有数值求得平均值作为输出。

定义：

$y[n] = \frac{1}{M_1+M_2+1}\displaystyle{ \sum_{k=-M_1}^{M_2}x[n-k] }$

单位脉冲响应：

$\begin{align*}
h[n] &=\frac{1}{M_1+M_2+1}\sum_{k=-M_1}^{M_2}\delta[n-k] \\
&=\left \{\begin{matrix}
\frac{1}{M_1+M_2+1}, & -M_1\leqslant n\leqslant M_2 \\ 
0, & \ else 
\end{matrix}\right.\\
&=\frac{1}{M_1+M_2+1}(u[n+M_1]-u[n-M_2-1]) \\
&=\frac{1}{M_1+M_2+1}(\delta[n+M_1]-\delta[n-M_2-1])*u[n]
\end{align*}$





# 累加器系统

累加器系统的目的就是把某一时间点之前的所有输入序列的值进行累加然后进行输出。

定义：

$y[n] = \displaystyle{ \sum_{k=-\infty}^{n}x[k] }$

单位脉冲响应：

$\begin{align*}
h[n] &= \sum_{k=-\infty}^{n}\delta[k]\\ 
 &=\sum_{k=-\infty}^{0}\delta[n+k] \\ 
 &= \left\{\begin{matrix}
1 &,n\geqslant 0 \\ 
0 &,n<0 
\end{matrix}\right. \\
&=u[n]
\end{align*}$





# 前向差分系统

前向差分系统的目的就是输出下一个输入x[n+1]与当前输入$x[n]$之间的差值。

定义：

$y[n] = x[n+1]-x[n]$

单位脉冲响应：

$h[n] = \delta[n+1]-\delta[n]$





# 后向差分系统

后向差分系统的目的就是输出当前输入$x[n]$与前一个输入$x[n-1]$之间的差值。

定义：

$y[n] = x[n]-x[n-1]$

单位脉冲响应：

$h[n] = \delta[n] – \delta[n-1]$












