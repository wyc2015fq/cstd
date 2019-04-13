
# word2vec 中的数学原理详解-基于 Negative Sampling 的模型 - Multiangle's Notepad - CSDN博客


2016年07月20日 17:35:02[multiangle](https://me.csdn.net/u014595019)阅读数：7018所属专栏：[自然语言处理](https://blog.csdn.net/column/details/13461.html)


转自:[http://blog.csdn.net/itplus/article/details/37998797](http://blog.csdn.net/itplus/article/details/37998797)
我之前写的是基于Hierarchical Softmax的模型。而在word2vec中，还有一种基于Negative Sampling的模型。我虽然没写，但是觉得这种方法还是蛮有用的，所以把别人写的资料贴上来，便于复习
![](https://img-blog.csdn.net/20140820234538209)[
](https://img-blog.csdn.net/20140820234538209)![](https://img-blog.csdn.net/20140820234548131)[
](https://img-blog.csdn.net/20140820234548131)![](https://img-blog.csdn.net/20140820234557818)[
](https://img-blog.csdn.net/20140820234557818)![](https://img-blog.csdn.net/20140820234419625)[
](https://img-blog.csdn.net/20140820234419625)![](https://img-blog.csdn.net/20140820234614791)[
](https://img-blog.csdn.net/20140820234614791)![](https://img-blog.csdn.net/20140820234438343)[
](https://img-blog.csdn.net/20140820234438343)![](https://img-blog.csdn.net/20140820234447828)[
](https://img-blog.csdn.net/20140820234447828)![](https://img-blog.csdn.net/20140820234723187)[
](https://img-blog.csdn.net/20140820234723187)![](https://img-blog.csdn.net/20140820234913833)[
](https://img-blog.csdn.net/20140820234913833)
[
](https://img-blog.csdn.net/20140820234913833)
[            ](https://img-blog.csdn.net/20140820234723187)
[
						](https://img-blog.csdn.net/20140820234447828)
[
	](https://img-blog.csdn.net/20140820234447828)[
](https://img-blog.csdn.net/20140820234438343)
[
  ](https://img-blog.csdn.net/20140820234614791)