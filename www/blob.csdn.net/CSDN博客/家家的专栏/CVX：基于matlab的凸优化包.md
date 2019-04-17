# CVX：基于matlab的凸优化包 - 家家的专栏 - CSDN博客





2013年04月03日 11:17:03[依海之燕](https://me.csdn.net/yihaizhiyan)阅读数：7869








官方网址：[http://cvxr.com/cvx/](http://cvxr.com/cvx/)

package下载网址：[http://cvxr.com/cvx/download/](http://cvxr.com/cvx/download/)

在ECCV2012的作者文章中，有用到CVX~

原来听往届的师兄，讲到过一系列的凸优化算法~

但是对于我这个跨专业的菜鸟来说，总是不明白~

今天看到这个数据包，虽然不是很明白，但是觉得还是很有用~

所以记录一下相关资料，并做个简单的翻译和介绍。



CVX：是一个解决凸优化问题的，基于matlab的模型系统。

CVX把matlab转化为一个模型语言，使其约束和目标函数都可以用标准的matlab表达方式书写。

（自己感觉，类似于一种伪码。简单明了，一看就明白了~）

在官方网站中，为了更加形象的说明，附有一个凸优化模型的例子：

![](https://img-my.csdn.net/uploads/201304/03/1364959144_3458.png)

这种有约束的凸优化方程，是非常常见的。

利用CVX package，可以方便求解，code如下：

m = 20; n = 10; p = 4;

A = randn(m,n); b = randn(m,1);

C = randn(p,n); d = randn(p,1); e = rand;

cvx_begin

    variable x(n)

    minimize( norm( A * x - b, 2 ) )

    subject to

        C * x == d

        norm( x, Inf ) <= e

cvx_end

我已经运行过了，ok~

举一反三，以后遇到类似的凸优化算法，就迎刃而解了~



