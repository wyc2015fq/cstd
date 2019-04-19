# C语言运算符的优先级 - xqhrs232的专栏 - CSDN博客
2013年10月25日 17:30:17[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：910
原文地址::[http://see.xidian.edu.cn/cpp/html/462.html](http://see.xidian.edu.cn/cpp/html/462.html)
相关文章
1、C语言运算符优先级 详细列表----[http://www.slyar.com/blog/c-operator-priority.html](http://www.slyar.com/blog/c-operator-priority.html)
2、[C++运算符优先级](http://blog.csdn.net/nicky_zs/article/details/4053146)----[http://blog.csdn.net/nicky_zs/article/details/4053146](http://blog.csdn.net/nicky_zs/article/details/4053146)
## 一、运算符的优先级表
C 语言的符号众多，由这些符号又组合成了各种各样的运算符。既然是运算符就一定有其特定的优先级，下表就是C 语言运算符的优先级表：
![](http://see.xidian.edu.cn/cpp/uploads/allimg/120205/1-12020519200Y14.jpg)
![](http://see.xidian.edu.cn/cpp/uploads/allimg/120205/1-120205192104B8.jpg)
![](http://see.xidian.edu.cn/cpp/uploads/allimg/120205/1-120205192201338.jpg)
![](http://see.xidian.edu.cn/cpp/uploads/allimg/120205/1-120205192249116.jpg)
![](http://see.xidian.edu.cn/cpp/uploads/allimg/120205/1-120205192324S8.jpg)
注：同一优先级的运算符，运算次序由结合方向所决定。
上表不容易记住。其实也用不着死记，用得多，看得多自然就记得了。也有人说不用记这些东西，只要记住乘除法的优先级比加减法高就行了，别的地方一律加上括号。这在你自己写代码的时候，确实可以，但如果是你去阅读和理解别人的代码呢？别人不一定都加上括号了吧？所以，记住这个表，我个人认为还是很有必要的。
## 二、一些容易出错的优先级问题
上表中，优先级同为1 的几种运算符如果同时出现，那怎么确定表达式的优先级呢？这是很多初学者迷糊的地方。下表就整理了这些容易出错的情况：
![](http://see.xidian.edu.cn/cpp/uploads/allimg/120205/1-120205192420113.jpg)
这些容易出错的情况，希望读者好好在编译器上调试调试，这样印象会深一些。一定要多调试，光靠看代码，水平是很难提上来的。调试代码才是最长水平的。
