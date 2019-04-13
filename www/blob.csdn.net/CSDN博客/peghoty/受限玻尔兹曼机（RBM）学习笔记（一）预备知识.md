
# 受限玻尔兹曼机（RBM）学习笔记（一）预备知识 - peghoty - CSDN博客


2014年02月13日 23:36:26[皮果提](https://me.csdn.net/peghoty)阅读数：88930标签：[RBM																](https://so.csdn.net/so/search/s.do?q=RBM&t=blog)[受限玻尔兹曼机																](https://so.csdn.net/so/search/s.do?q=受限玻尔兹曼机&t=blog)[能量函数																](https://so.csdn.net/so/search/s.do?q=能量函数&t=blog)[对比散度																](https://so.csdn.net/so/search/s.do?q=对比散度&t=blog)[CD-k																](https://so.csdn.net/so/search/s.do?q=CD-k&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=对比散度&t=blog)[
																					](https://so.csdn.net/so/search/s.do?q=能量函数&t=blog)个人分类：[深度学习																](https://blog.csdn.net/peghoty/article/category/1451403)[机器学习																](https://blog.csdn.net/peghoty/article/category/1824627)[
							](https://blog.csdn.net/peghoty/article/category/1451403)
[
				](https://so.csdn.net/so/search/s.do?q=能量函数&t=blog)
[
			](https://so.csdn.net/so/search/s.do?q=能量函数&t=blog)
[
		](https://so.csdn.net/so/search/s.do?q=受限玻尔兹曼机&t=blog)
[
	](https://so.csdn.net/so/search/s.do?q=RBM&t=blog)

去年 6 月份写的博文《Yusuke Sugomori 的 C 语言 Deep Learning 程序解读》是囫囵吞枣地读完一个关于 DBN 算法的开源代码后的笔记，当时对其中涉及的算法原理基本不懂。近日再次学习 RBM，觉得有必要将其整理成笔记，算是对那个代码的一个补充。


**目录链接**
（一）预备知识
[（二）网络结构](http://blog.csdn.net/itplus/article/details/19168967)
[（三）能量函数和概率分布](http://blog.csdn.net/itplus/article/details/19168989)
（四）对数似然函数
[（五）梯度计算公式](http://blog.csdn.net/itplus/article/details/19207371)
[（六）对比散度算法](http://blog.csdn.net/itplus/article/details/19408143)
[（七）RBM 训练算法](http://blog.csdn.net/itplus/article/details/19408701)
（八）RBM 的评估
![](https://img-blog.csdn.net/20140213225431406)![](https://img-blog.csdn.net/20140213225446750)![](https://img-blog.csdn.net/20140213225502296)![](https://img-blog.csdn.net/20140213225515125)![](https://img-blog.csdn.net/20140213225527828)![](https://img-blog.csdn.net/20140314095732734)![](https://img-blog.csdn.net/20140213225553531)
![](https://img-blog.csdn.net/20140218224838375)


作者: peghoty
出处:[http://blog.csdn.net/itplus/article/details/19168937](http://blog.csdn.net/itplus/article/details/19168937)
欢迎转载/分享, 但请务必声明文章出处.


