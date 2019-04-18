# Lipschitz连续 - YZXnuaa的博客 - CSDN博客
2018年03月28日 12:21:57[YZXnuaa](https://me.csdn.net/YZXnuaa)阅读数：1733
Lipschitz连续。它其实就是在一个连续函数![f](https://www.zhihu.com/equation?tex=f)上面额外施加了一个限制，要求存在一个常数![K\geq 0](https://www.zhihu.com/equation?tex=K%5Cgeq+0)使得定义域内的任意两个元素![x_1](https://www.zhihu.com/equation?tex=x_1)和![x_2](https://www.zhihu.com/equation?tex=x_2)都满足
![|f(x_1) - f(x_2)| \leq K |x_1 - x_2|](https://www.zhihu.com/equation?tex=%7Cf%28x_1%29+-+f%28x_2%29%7C+%5Cleq+K+%7Cx_1+-+x_2%7C)
此时称函数![f](https://www.zhihu.com/equation?tex=f)的Lipschitz常数为![K](https://www.zhihu.com/equation?tex=K)。
简单理解，比如说![f](https://www.zhihu.com/equation?tex=f)的定义域是实数集合，那上面的要求就等价于![f](https://www.zhihu.com/equation?tex=f)的导函数绝对值不超过![K](https://www.zhihu.com/equation?tex=K)。再比如说![\log (x)](https://www.zhihu.com/equation?tex=%5Clog+%28x%29)就不是Lipschitz连续，因为它的导函数没有上界。Lipschitz连续条件限制了一个连续函数的最大局部变动幅度。
