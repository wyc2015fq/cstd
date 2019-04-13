
# Factorization Machines 学习笔记（四）学习算法 - peghoty - CSDN博客


2014年10月28日 10:22:34[皮果提](https://me.csdn.net/peghoty)阅读数：31302标签：[FM																](https://so.csdn.net/so/search/s.do?q=FM&t=blog)[FactorizationMachine																](https://so.csdn.net/so/search/s.do?q=FactorizationMachine&t=blog)[稀疏特征																](https://so.csdn.net/so/search/s.do?q=稀疏特征&t=blog)[SGD																](https://so.csdn.net/so/search/s.do?q=SGD&t=blog)[ALS																](https://so.csdn.net/so/search/s.do?q=ALS&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=SGD&t=blog)[
																					](https://so.csdn.net/so/search/s.do?q=稀疏特征&t=blog)个人分类：[数据挖掘																](https://blog.csdn.net/peghoty/article/category/1451019)[机器学习																](https://blog.csdn.net/peghoty/article/category/1824627)[
							](https://blog.csdn.net/peghoty/article/category/1451019)
[
				](https://so.csdn.net/so/search/s.do?q=稀疏特征&t=blog)
[
			](https://so.csdn.net/so/search/s.do?q=稀疏特征&t=blog)
[
		](https://so.csdn.net/so/search/s.do?q=FactorizationMachine&t=blog)
[
	](https://so.csdn.net/so/search/s.do?q=FM&t=blog)

最近学习了一种叫做 Factorization Machines（简称 FM）的算法，它可对任意的实值向量进行预测。其主要优点包括: 1) 可用于高度稀疏数据场景；2) 具有线性的计算复杂度。本文将对 FM 框架进行简单介绍，并对其训练算法 — 随机梯度下降（SGD）法和交替最小二乘（ALS）法进行详细推导。
**相关链接**：
[（一）预测任务](http://blog.csdn.net/itplus/article/details/40534885)
[（二）模型方程](http://blog.csdn.net/itplus/article/details/40534923)
[（三）回归和分类](http://blog.csdn.net/itplus/article/details/40534951)
[（四）学习算法](http://blog.csdn.net/itplus/article/details/40536025)
![](https://img-blog.csdn.net/20141028095518218?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaXRwbHVz/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)![](https://img-blog.csdn.net/20141028095606640?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaXRwbHVz/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)![](https://img-blog.csdn.net/20141028095536330?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaXRwbHVz/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)![](https://img-blog.csdn.net/20141028095551368?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaXRwbHVz/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)![](https://img-blog.csdn.net/20141028095558544?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaXRwbHVz/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)![](https://img-blog.csdn.net/20141028095605611?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaXRwbHVz/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)![](https://img-blog.csdn.net/20141028095652375?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaXRwbHVz/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)![](https://img-blog.csdn.net/20141028095618934?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaXRwbHVz/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![](https://img-blog.csdn.net/20141028095707497?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaXRwbHVz/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
作者: peghoty
出处:[http://blog.csdn.net/itplus/article/details/40536025](http://blog.csdn.net/itplus/article/details/40536025)
欢迎转载/分享, 但请务必声明文章出处.


