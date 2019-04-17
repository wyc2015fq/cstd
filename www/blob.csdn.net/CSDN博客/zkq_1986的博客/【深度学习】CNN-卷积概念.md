# 【深度学习】CNN-卷积概念 - zkq_1986的博客 - CSDN博客





2017年05月23日 16:19:03[zkq_1986](https://me.csdn.net/zkq_1986)阅读数：187








**1 定义**

函数f和g的卷积(Convolution)的定义如下：


(f∗g)(t)=def ∫−∞∞f(τ)g(t−τ)dτ=∫−∞∞f(t−τ)g(τ)dτ.{\displaystyle
 {\begin{aligned}(f*g)(t)&\,{\stackrel {\mathrm {def} }{=}}\ \int _{-\infty }^{\infty }f(\tau )\,g(t-\tau )\,d\tau \\&=\int _{-\infty }^{\infty }f(t-\tau )\,g(\tau )\,d\tau .\end{aligned}}}![{\displaystyle {\begin{aligned}(f*g)(t)&\,{\stackrel {\mathrm {def} }{=}}\ \int _{-\infty }^{\infty }f(\tau )\,g(t-\tau )\,d\tau \\&=\int _{-\infty }^{\infty }f(t-\tau )\,g(\tau )\,d\tau .\end{aligned}}}](https://wikimedia.org/api/rest_v1/media/math/render/svg/a85f42436daa9e0bc3586e8d5374b5fa17c87f63)上式中的t可用x替代。


f*g表示函数g经过翻转和平移与函数f的重叠部分的积分。随着 x 的不同取值，这个积分就定义了一个新函数h(x)，称为函数f 与g 的卷积，记为h(x)＝(f*g)(x)。






# 2 物理含义
- Express each function in terms of a [dummy
 variable](https://en.wikipedia.org/wiki/Free_variables_and_bound_variables)τ.{\displaystyle \tau .}![\tau .](https://wikimedia.org/api/rest_v1/media/math/render/svg/871bb01391136d3551c8ea59059e106be2a403cd)
- Reflect one of the functions: g(τ){\displaystyle
 g(\tau )}![g(\tau )](https://wikimedia.org/api/rest_v1/media/math/render/svg/f3ca3806d8f1456510d15896379772656cd465da)→g(−τ).{\displaystyle
 g(-\tau ).}![g(-\tau ).](https://wikimedia.org/api/rest_v1/media/math/render/svg/1654ed026191fb24a5f4e237101c1cccabd82110)
- Add a time-offset, *t*, which allows g(t−τ){\displaystyle
 g(t-\tau )}![g(t-\tau )](https://wikimedia.org/api/rest_v1/media/math/render/svg/3cb1c1c6de1ad02f14ce3217246b956b66f1c5a2) to
 slide along the τ{\displaystyle \tau }![\tau](https://wikimedia.org/api/rest_v1/media/math/render/svg/38a7dcde9730ef0853809fefc18d88771f95206c)-axis.
- Start *t* at −∞ and slide it all the way to +∞. Wherever the two functions intersect, find the integral of their product. In other words, compute a sliding, weighted-sum of function f(τ){\displaystyle
 f(\tau )}![f(\tau )](https://wikimedia.org/api/rest_v1/media/math/render/svg/bcba00f11285b589b0ff57beeaf118defab2cfe8),
 where the weighting function is g(−τ).{\displaystyle g(-\tau ).}![g(-\tau ).](https://wikimedia.org/api/rest_v1/media/math/render/svg/1654ed026191fb24a5f4e237101c1cccabd82110)
The resulting waveform (not shown here) is the convolution of functions *f* and *g*.
![Convolution3.svg](https://upload.wikimedia.org/wikipedia/commons/thumb/7/79/Convolution3.svg/399px-Convolution3.svg.png)




3 例子


两个向量卷积，说白了就是多项式乘法。下面用个矩阵例子说明其工作原理。

a =

     8     1     6

     3     5     7

     4     9     2

b =

     2     3     2

     1     6     1

     2     1     5





矩阵a和矩阵b的卷积就是，把矩阵b上下翻转得到矩阵b'，然后让b'从矩阵a的顶部开始逐步往下平移，求取它们交集部分的积分（多项式乘积），作为卷积结果矩阵中的一行。多项式求解步骤如下。


![](https://img-blog.csdn.net/20170523164542938)


所得卷积矩阵为：

a*b =


    16    26    31    20    12

    14    68    55    68    20

    27    63   132    82    41

    10    46    94    53    37

     8    22    33    47    10









转载自：http://blog.sciencenet.cn/blog-268138-383185.html



