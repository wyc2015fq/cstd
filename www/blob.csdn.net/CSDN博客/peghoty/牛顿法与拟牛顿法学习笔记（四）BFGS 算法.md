
# 牛顿法与拟牛顿法学习笔记（四）BFGS 算法 - peghoty - CSDN博客


2014年03月24日 00:53:04[皮果提](https://me.csdn.net/peghoty)阅读数：54608标签：[拟牛顿法																](https://so.csdn.net/so/search/s.do?q=拟牛顿法&t=blog)[BFGS																](https://so.csdn.net/so/search/s.do?q=BFGS&t=blog)[精确搜索																](https://so.csdn.net/so/search/s.do?q=精确搜索&t=blog)[Wolfe条件																](https://so.csdn.net/so/search/s.do?q=Wolfe条件&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=精确搜索&t=blog)[
																					](https://so.csdn.net/so/search/s.do?q=BFGS&t=blog)个人分类：[数学天地																](https://blog.csdn.net/peghoty/article/category/1505699)
[
																								](https://so.csdn.net/so/search/s.do?q=BFGS&t=blog)
[
				](https://so.csdn.net/so/search/s.do?q=拟牛顿法&t=blog)
[
			](https://so.csdn.net/so/search/s.do?q=拟牛顿法&t=blog)


机器学习算法中经常碰到非线性优化问题，如 Sparse Filtering 算法，其主要工作在于求解一个非线性极小化问题。在具体实现中，大多调用的是成熟的软件包做支撑，其中最常用的一个算法是 L-BFGS。为了解这个算法的数学机理，这几天做了一些调研，现把学习过程中理解的一些东西整理出来。

目录链接

(1)[牛顿法](http://blog.csdn.net/itplus/article/details/21896453)
(2)[拟牛顿条件](http://blog.csdn.net/itplus/article/details/21896619)
(3)[DFP 算法](http://blog.csdn.net/itplus/article/details/21896981)
(4)[BFGS 算法](http://blog.csdn.net/itplus/article/details/21897443)
(5)[L-BFGS 算法](http://blog.csdn.net/itplus/article/details/21897715)

![](https://img-blog.csdn.net/20140324001055187)![](https://img-blog.csdn.net/20140324001108453)![](https://img-blog.csdn.net/20140324001126671)![](https://img-blog.csdn.net/20140324001133312)
![](https://img-blog.csdn.net/20140324001248171)


作者: peghoty
出处:[http://blog.csdn.net/itplus/article/details/21897443](http://blog.csdn.net/itplus/article/details/21897443)
欢迎转载/分享, 但请务必声明文章出处.


