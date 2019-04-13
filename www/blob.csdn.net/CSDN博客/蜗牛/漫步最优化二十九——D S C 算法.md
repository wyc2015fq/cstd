
# 漫步最优化二十九——D.S.C.算法 - 蜗牛 - CSDN博客


2017年10月23日 18:35:15[会敲键盘的猩猩](https://me.csdn.net/u010182633)阅读数：327



$\textbf{你是我的小公主，}$
$\textbf{像宠你宠你宠你。}$
$\textbf{你是我的小公主，}$
$\textbf{我的天空是晴是雨是彩虹，}$
$\textbf{受到了你的操控。}$
$\textbf{你是我的小公主，}$
$\textbf{让幸福不再是幻想，}$
$\textbf{而是实实在在的感受。}$
$\textbf{你是我的小公主，}$
$\textbf{我愿做你的青蛙王子，}$
$\textbf{呱呱呱。}$
$\qquad\textbf{——畅宝宝的傻逼哥哥}$
前面描述的方法要么是搜索，要么是近似，Davies,Swann,Campey发明了一种算法，它结合了搜索法与近似法，用搜索法来确定包含$x^*$的范围，而用近似法来生成$x^*$的估计值。
对于这种方法，从$x$的正方向或者反方向估计$f(x)$，直到包含$x^*$，然后用二次插值形式来预测$x^*$，重复执行这个过程直到达到所求的精度。
算法的输入包括初始点$x_{0,1}$，初始增长量$\delta_1$，常数$K$以及优化容忍误差$\varepsilon$。
在第$k$次迭代，初始点$x_{0,k}$与初始增长量$\delta_k$是可以得到的，我们需要求出下次迭代的$x_{0,k+1},\delta_{k+1}$。
初始时刻，估计$f(x)$在点$x_{0,k}-\delta_k,x_{0,k},x_{0,k}+\delta_k$处的值，会有三种可能的情况，即
f(x_{0,k}-\delta_k)>f(x_{0,k})>f(x_{0,k}+\delta_k)
f(x_{0,k}-\delta_k)<f(x_{0,k})<f(x_{0,k}+\delta_k)
f(x_{0,k}-\delta_k)\geq f(x_{0,k})\leq f(x_{0,k}+\delta_k)
对于第一种情况，$f(x)$的最小值位于正方向，所以增加$x$的值并计算相应的$f(x)$，直到出现较大的为止。如果这种情况在第$n$次发生，那么区间$[x_{0,k},x_{n,k}]$包含$x^*$，相邻的两个区间是呈几何增长的，所以这个过程会产生如下的点序列：

$$
\begin{align*}
x_{0,k}&\\
x_{1,k}&=x_{0,k}+\delta_k\\
x_{2,k}&=x_{1,k}+2\delta_k\\
x_{3,k}&=x_{2,k}+4\delta_k\\
&\vdots\\
x_{n,k}&=x_{n-1,k}+2^{n-1}\delta_k
\end{align*}
$$
如图1所示。显然，最新产生的区间是前者的两倍，并且如果在点

$$
x_{m,k}=x_{n-1,k}+2^{n-2}\delta_k
$$
处将其分成两个相等的区间，那么我们会得到四个等区间的点。

![这里写图片描述](https://img-blog.csdn.net/20171023182728021?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDE4MjYzMw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)[ ](https://img-blog.csdn.net/20171023182728021?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDE4MjYzMw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
图1
如果计算$f(x)$在点$x_{m,k}$处的值，我们能得到

$$
\begin{align*}
f_{n-2,k}&\equiv f(x_{n-2,k})\\
f_{n-1,k}&\equiv f(x_{n-1,k})\\
f_{m,k}&\equiv f(x_m,k)\\
f_{n,k}&\equiv f(x_n,k)
\end{align*}
$$
如果$f_{m,k}\geq f_{n-1,k}$，那么$x^*$位于区间$[x_{n-2,k},x_{m,k}]$中(如图2所示)，利用二次插值的结论可得$x^*$的估计值为

$$
x_{0,k+1}=x_{n-1,k}+\frac{2^{n-2}\delta_k(f_{n-2,k}-f_{m,k})}{2(f_{n-2,k}-2f_{n-1,k}+f_{m,k})}
$$
同样地，如果$f_{m,k}<f_{n-1,k}$，那么$x^*$位于区间$[x_{n-1,k},x_{n,k}]$中(如图3所示)，$x^*$的估计值为

$$
x_{0,k+1}=x_{m,k}+\frac{2^{n-2}\delta_k(f_{n-1,k}-f_{n,k})}{2(f_{n-1,k}-2f_{m,k}+f_{n,k})}
$$

![这里写图片描述](https://img-blog.csdn.net/20171023182807267?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDE4MjYzMw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)[ ](https://img-blog.csdn.net/20171023182807267?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDE4MjYzMw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
图2
对于第二种情况，$x^*$位于反方向，所以$x$沿着$\delta_k,2\delta_k,\ldots$递减直到定位到$f(x)$的最小值，这个过程与第一种情况一样，只需要将$\delta_k$的符号改为负即可。
![这里写图片描述](https://img-blog.csdn.net/20171023182835991?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDE4MjYzMw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)[ ](https://img-blog.csdn.net/20171023182835991?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDE4MjYzMw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
图3
对于第三种情况，$x^*$位于$x_{0,k}-\delta_k,x_{0,k}+\delta_k$之间，如果

$$
\begin{align*}
f_{-1,k}=f(x_{0,k}-\delta_k)\\
f_{0,k}=f(x_{0,k})\\
f_{1,k}=f(x_{0,k}+\delta_k)
\end{align*}
$$
那么根据二次插值的结论可得$x^*$得估计值为

$$
x_{0,k+1}=x_{0,k}+\frac{\delta_k(f_{-1,k}-f_{1,k})}{2(f_{-1,k}-2f_{0,k}+f_{1,k})}
$$
对于第$k$次迭代，我们重新定义增长量为

$$
\delta_{k+1}=K\delta_k
$$
其中$K$是0到1之间的常数。使用这个常量的动机是随着越来越靠近解，我们会慢慢找到$x$的小区间，因此需要改变算法的步长，比较合适的$K$为0.1。

