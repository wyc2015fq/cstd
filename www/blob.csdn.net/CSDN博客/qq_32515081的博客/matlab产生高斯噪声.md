# matlab产生高斯噪声 - qq_32515081的博客 - CSDN博客
2018年03月09日 19:52:59[TengAdventure](https://me.csdn.net/qq_32515081)阅读数：5723
# randn
randn（random normal distribution）是一种产生标准[正态分布](https://baike.baidu.com/item/%E6%AD%A3%E6%80%81%E5%88%86%E5%B8%83)的[随机数](https://baike.baidu.com/item/%E9%9A%8F%E6%9C%BA%E6%95%B0)或[矩阵](https://baike.baidu.com/item/%E7%9F%A9%E9%98%B5)的函数，属于MATLAB函数。返回一个n*n的随机项的矩阵。如果n不是个数量，将返回错误信息。

## MATLAB函数randn简介
编辑
用法：
Y = [rand](https://baike.baidu.com/item/rand)n（n）
返回一个n*n的随机项的矩阵。如果n不是个数量，将返回错误信息。
Y = randn(m,n) 或 Y = randn([m n])
返回一个m*n的随机项矩阵。
Y = randn(m,n,p,...) 或 Y = randn([m n p...])
产生随机[数组](https://baike.baidu.com/item/%E6%95%B0%E7%BB%84)。
Y = randn(size(A))
返回一个和A有同样[维数](https://baike.baidu.com/item/%E7%BB%B4%E6%95%B0)大小的[随机数](https://baike.baidu.com/item/%E9%9A%8F%E6%9C%BA%E6%95%B0)组。
randn
返回一个每次都变化的数量。
## 应用举例
Example 1. R = [rand](https://baike.baidu.com/item/rand)n(3,4) 将生成[矩阵](https://baike.baidu.com/item/%E7%9F%A9%E9%98%B5)
R =
1.1650 0.3516 0.0591 0.8717
0.6268 -0.6965 1.7971 -1.4462
0.0751 1.6961 0.2641 -0.7012
For a histogram of the randn distribution, see hist.
Example 2. 产生一个随机分布的指定[均值](https://baike.baidu.com/item/%E5%9D%87%E5%80%BC)和[方差](https://baike.baidu.com/item/%E6%96%B9%E5%B7%AE)的矩阵：将randn产生的结果乘以[标准差](https://baike.baidu.com/item/%E6%A0%87%E5%87%86%E5%B7%AE)，然后加上期望均值即可。例如，产生均值为0.6，方差为0.1的一个5*5的[随机数](https://baike.baidu.com/item/%E9%9A%8F%E6%9C%BA%E6%95%B0)方式如下：
x = .6 + sqrt(0.1) * [rand](https://baike.baidu.com/item/rand)n(5)
x =
0.8713 0.4735 0.8114 0.0927 0.7672
0.9966 0.8182 0.9766 0.6814 0.6694
0.0960 0.8579 0.2197 0.2659 0.3085
0.1443 0.8251 0.5937 1.0475 -0.0864
0.7806 1.0080 0.5504 0.3454 0.5813
Example 3.设置randn到其默认的初始状态格式:randn('state', 0);每次初始化randn到不同的状态的格式randn('state', sum(100*clock))。
如何用randn产生两个相同的矩阵：
s = randn('state');%保存当前状态，
u1 = randn(100);%产生100个值，
randn('state',s);%复位状态，
u2 = randn(100); %并重复序列。格式调用如下：
s = randn('state');
u1 = randn(3)
randn('state',s);
u2 = randn(3)
运行结果如下：
u1 =1.6039 0.8957 -0.1072
-0.3728 -1.3913 -0.8276
0.7578 0.3116 0.0592
u2 =1.6039 0.8957 -0.1072
-0.3728 -1.3913 -0.8276
0.7578 0.3116 0.0592
从结果可以看出，这两个矩阵的各个元素均相同，所以这两个就是等价矩阵。
其他类似函数：[rand](https://baike.baidu.com/item/rand), randperm, sprand, sprandn
Y = randn(n)
Y = randn(m,n)
Y = randn([m n])
Y = randn(m,n,p,...)
Y = randn([m n p...])
Y = randn(size(A))
randn
s = randn('state')
