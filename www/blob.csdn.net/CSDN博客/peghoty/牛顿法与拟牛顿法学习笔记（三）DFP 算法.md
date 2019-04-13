
# 牛顿法与拟牛顿法学习笔记（三）DFP 算法 - peghoty - CSDN博客


2014年03月24日 00:52:06[皮果提](https://me.csdn.net/peghoty)阅读数：30368标签：[拟牛顿法																](https://so.csdn.net/so/search/s.do?q=拟牛顿法&t=blog)[DFP算法																](https://so.csdn.net/so/search/s.do?q=DFP算法&t=blog)[Davidon																](https://so.csdn.net/so/search/s.do?q=Davidon&t=blog)[Fletcher																](https://so.csdn.net/so/search/s.do?q=Fletcher&t=blog)[Powell																](https://so.csdn.net/so/search/s.do?q=Powell&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=Fletcher&t=blog)[
																					](https://so.csdn.net/so/search/s.do?q=Davidon&t=blog)个人分类：[数学天地																](https://blog.csdn.net/peghoty/article/category/1505699)
[
																								](https://so.csdn.net/so/search/s.do?q=Davidon&t=blog)
[
				](https://so.csdn.net/so/search/s.do?q=DFP算法&t=blog)
[
			](https://so.csdn.net/so/search/s.do?q=DFP算法&t=blog)
[
		](https://so.csdn.net/so/search/s.do?q=拟牛顿法&t=blog)

机器学习算法中经常碰到非线性优化问题，如 Sparse Filtering 算法，其主要工作在于求解一个非线性极小化问题。在具体实现中，大多调用的是成熟的软件包做支撑，其中最常用的一个算法是 L-BFGS。为了解这个算法的数学机理，这几天做了一些调研，现把学习过程中理解的一些东西整理出来。

目录链接

(1)[牛顿法](http://blog.csdn.net/itplus/article/details/21896453)
(2)[拟牛顿条件](http://blog.csdn.net/itplus/article/details/21896619)
(3)[DFP 算法](http://blog.csdn.net/itplus/article/details/21896981)
(4)[BFGS 算法](http://blog.csdn.net/itplus/article/details/21897443)
(5)[L-BFGS 算法](http://blog.csdn.net/itplus/article/details/21897715)

![](https://img-blog.csdn.net/20140324000528609)![](https://img-blog.csdn.net/20140324000539484)
![](https://img-blog.csdn.net/20140324000740687)


作者: peghoty
出处:[http://blog.csdn.net/itplus/article/details/21896981](http://blog.csdn.net/itplus/article/details/21896981)
欢迎转载/分享, 但请务必声明文章出处.



