# numpy中mat和python的list转换 - 数据之美的博客 - CSDN博客
2017年09月09日 02:23:28[看穿数据之美](https://me.csdn.net/oppo62258801)阅读数：1157
**list转换成mat**
numpy.mat(list)
![](https://img-blog.csdn.net/20160905212724466)
**mat转换成list**
mat.tolist()
![](https://img-blog.csdn.net/20160905212612747)
append(), extend()都是list的函数，所以要对矩阵进行类似操作的时候，需要现将矩阵通过上述方法转化成list，再进行操作。
