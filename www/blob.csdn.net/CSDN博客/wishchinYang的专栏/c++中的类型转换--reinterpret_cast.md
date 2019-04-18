# c++中的类型转换--reinterpret_cast - wishchinYang的专栏 - CSDN博客
2018年03月22日 15:06:24[wishchin](https://me.csdn.net/wishchin)阅读数：924
原文链接：  浅析[c++中的类型转换--reinterpret_cast](http://blog.csdn.net/wangshubo1989/article/details/49133667)转换
reinterpret_cast作用为： 
允许将任何指针转换为任何其他指针类型。 也允许将任何整数类型转换为任何指针类型以及反向转换。
看着上面的描述就有种放浪形骸的赶脚。更会让人不寒而栗，太随意！
语法还是老样子： 
reinterpret_cast < type-id > ( expression )
滥用 reinterpret_cast 运算符可能很容易带来风险。 除非所需转换本身是低级别的，否则应使用其他强制转换运算符之一。

