# 乘法逆元...Orz - Vmetrio - 博客园







# [乘法逆元...Orz](https://www.cnblogs.com/wangmengmeng/p/5521305.html)





最近打的几场比赛,都出现了有关逆元的题目,今天就整理了一下...



求乘法逆元的几种方法:http://www.cnblogs.com/james47/p/3871782.html



博文转载链接:http://blog.csdn.net/acdreamers/article/details/8220787





今天我们来探讨逆元在**ACM-ICPC**竞赛中的应用，逆元是一个很重要的概念，必须学会使用它。



对于正整数![](http://img.blog.csdn.net/20140613102654328)和![](http://img.blog.csdn.net/20140613102712781)，如果有![](http://img.blog.csdn.net/20140613102734984)，那么把这个同余方程中![](http://img.blog.csdn.net/20140613102856531)的最小正整数解叫做![](http://img.blog.csdn.net/20140613102654328)模![](http://img.blog.csdn.net/20140613102712781)的逆元。



逆元一般用扩展欧几里得算法来求得，如果![](http://img.blog.csdn.net/20140613102712781)为素数，那么还可以根据费马小定理得到逆元为![](http://img.blog.csdn.net/20140613103413828)。



推导过程如下

![](http://img.blog.csdn.net/20140613104248984)



求现在来看一个逆元最常见问题，求如下表达式的值**（已知![](http://img.blog.csdn.net/20140613104752312)）**



![](http://img.blog.csdn.net/20140613104619203)



当然这个经典的问题有很多方法，最常见的就是扩展欧几里得，如果![](http://img.blog.csdn.net/20140613102712781)是素数，还可以用费马小定理。



但是你会发现费马小定理和扩展欧几里得算法求逆元是有局限性的，它们都会要求![](http://img.blog.csdn.net/20140613102654328)与![](http://img.blog.csdn.net/20140613102712781)互素。实际上我们还有一

种通用的求逆元方法，适合所有情况。公式如下



![](http://img.blog.csdn.net/20140613105646406)



现在我们来证明它，已知![](http://img.blog.csdn.net/20140613104752312)，证明步骤如下



![](http://img.blog.csdn.net/20140613112023328)














