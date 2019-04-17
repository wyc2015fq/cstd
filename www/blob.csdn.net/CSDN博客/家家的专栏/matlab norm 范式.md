# matlab norm 范式 - 家家的专栏 - CSDN博客





2011年10月25日 17:10:55[依海之燕](https://me.csdn.net/yihaizhiyan)阅读数：16282标签：[matlab																[编程](https://so.csdn.net/so/search/s.do?q=编程&t=blog)](https://so.csdn.net/so/search/s.do?q=matlab&t=blog)







%X为向量，求欧几里德范数，即 。

n = norm(X,inf) %求 -范数，即 。

n = norm(X,1) %求1-范数，即 。

n = norm(X,-inf) %求向量X的元素的绝对值的最小值，即 。

n = norm(X, p) %求p-范数，即 ，所以norm(X,2) = norm(X)。

命令 矩阵的范数函数 norm格式 n = norm(A) %A为矩阵，求欧几里德范数 ，等于A的最大奇异值。

n = norm(A,1) %求A的列范数 ，等于A的列向量的1-范数的最大值。

n = norm(A,2) %求A的欧几里德范数 ，和norm(A)相同。

n = norm(A,inf) %求行范数 ，等于A的行向量的1-范数的最大值即：max(sum(abs(A')))。

n = norm(A, 'fro' ) %求矩阵A的Frobenius范数 ，矩阵元p阶范数估计需要自己编程求，

计算公式如下

 举个例子吧a=magic(3)sum(sum(abs(a)^4))^(1/4)a = 8 1 6 3 5 7 4 9 2

ans = 19.7411

转自：[http://zhidao.baidu.com/question/245196523.html](http://zhidao.baidu.com/question/245196523.html)

![](http://hi.csdn.net/attachment/201110/25/0_1319533978Bsbi.gif)




