# Lourakis/ Levenberg-Marquardt nonlinear least squares algorithms in C/C++ - 工作笔记 - CSDN博客





2012年01月18日 14:49:20[Eason.wxd](https://me.csdn.net/App_12062011)阅读数：5895标签：[算法																[优化](https://so.csdn.net/so/search/s.do?q=优化&t=blog)](https://so.csdn.net/so/search/s.do?q=算法&t=blog)
个人分类：[信号处理](https://blog.csdn.net/App_12062011/article/category/1063270)








源码网址：

[http://www.ics.forth.gr/~lourakis/levmar/](http://www.ics.forth.gr/~lourakis/levmar/)



在没有LAPACK（线性代数软件包）时，可用自带的LU分解算法求解线性问题的解。用LM方法求解非线性最优化问题得到的是局部最优解，解对初值有依赖性，迭代初值估计是重点和难点




